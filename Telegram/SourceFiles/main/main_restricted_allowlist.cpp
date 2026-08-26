/*
This file is part of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/telegramdesktop/tdesktop/blob/master/LEGAL
*/
#include "main/main_restricted_allowlist.h"

#include "core/application.h"
#include "main/main_session.h"

#include "data/data_chat_filters.h"
#include "data/data_channel.h"
#include "data/data_peer.h"
#include "data/data_session.h"
#include "data/data_stories.h"
#include "data/data_user.h"
#include "history/history.h"
#include "window/notifications_manager.h"
#include "window/window_session_controller.h"

#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QFileSystemWatcher>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonObject>
#include <QtCore/QSaveFile>

namespace Main {
namespace {

constexpr auto kReloadDebounce = crl::time(300);
const auto kDefaultFolderName = u"Allowlist"_q;
const auto kProfilesKey = u"profiles"_q;
const auto kAccountKey = u"account"_q;
const auto kSessionUniqueIdKey = u"sessionUniqueId"_q;
const auto kUsernameKey = u"username"_q;
const auto kFolderNameKey = u"folderName"_q;
const auto kSyncFromFolderKey = u"syncFromFolder"_q;

[[nodiscard]] std::optional<quint64> ParseSerializedPeerId(
		const QJsonValue &value) {
	if (value.isString()) {
		bool ok = false;
		const auto parsed = value.toString().toULongLong(&ok);
		return ok ? std::optional<quint64>(parsed) : std::nullopt;
	} else if (value.isDouble()) {
		const auto number = value.toDouble();
		const auto parsed = quint64(number);
		return (number >= 0.) && (double(parsed) == number)
			? std::optional<quint64>(parsed)
			: std::nullopt;
	} else if (value.isObject()) {
		const auto object = value.toObject();
		for (const auto &key : {
			u"serializedPeerId"_q,
			u"peerId"_q,
			u"id"_q,
		}) {
			if (const auto parsed = ParseSerializedPeerId(object.value(key))) {
				return parsed;
			}
		}
	}
	return std::nullopt;
}

[[nodiscard]] std::optional<quint64> ParseSpecialPeerToken(
		not_null<const Session*> session,
		const QJsonValue &value) {
	if (!value.isString()) {
		return std::nullopt;
	}
	const auto token = value.toString().trimmed().toLower();
	if (token == u"self"_q
		|| token == u"me"_q
		|| token == u"saved_messages"_q
		|| token == u"saved-messages"_q
		|| token == u"savedmessages"_q) {
		return SerializePeerId(session->user()->id);
	}
	return std::nullopt;
}

void AppendParsedIds(
		not_null<const Session*> session,
		base::flat_set<quint64> *result,
		const QJsonValue &value) {
	if (!value.isArray()) {
		return;
	}
	for (const auto &entry : value.toArray()) {
		if (const auto parsed = ParseSpecialPeerToken(session, entry)) {
			result->emplace(*parsed);
			continue;
		}
		if (const auto parsed = ParseSerializedPeerId(entry)) {
			result->emplace(*parsed);
		}
	}
}

[[nodiscard]] QString CurrentSessionUniqueId(
		not_null<const Session*> session) {
	return QString::number(session->uniqueId());
}

[[nodiscard]] QString CurrentUsername(not_null<const Session*> session) {
	return session->user()->username().trimmed();
}

[[nodiscard]] QJsonObject MakeAccountObject(not_null<const Session*> session) {
	auto result = QJsonObject();
	result.insert(kSessionUniqueIdKey, CurrentSessionUniqueId(session));
	result.insert(kUsernameKey, CurrentUsername(session));
	return result;
}

[[nodiscard]] bool MatchesAccount(
		not_null<const Session*> session,
		const QJsonObject &account) {
	if (account.isEmpty()) {
		return false;
	}
	const auto username = CurrentUsername(session);
	const auto sessionUniqueId = CurrentSessionUniqueId(session);
	const auto boundUsername = account.value(kUsernameKey).toString().trimmed();
	const auto boundSessionUniqueId = account.value(
		kSessionUniqueIdKey).toString().trimmed();
	const auto usernameMatches = !username.isEmpty()
		&& !boundUsername.isEmpty()
		&& !boundUsername.compare(username, Qt::CaseInsensitive);
	const auto sessionMatches = !boundSessionUniqueId.isEmpty()
		&& (boundSessionUniqueId == sessionUniqueId);
	return usernameMatches || sessionMatches;
}

[[nodiscard]] bool LooksLikeLegacyProfile(const QJsonObject &object) {
	for (const auto &key : {
		kAccountKey,
		kFolderNameKey,
		kSyncFromFolderKey,
		u"allowedPeers"_q,
		u"allowedPeerIds"_q,
		u"manualPeers"_q,
		u"importedPeers"_q,
	}) {
		if (object.contains(key)) {
			return true;
		}
	}
	return false;
}

[[nodiscard]] QJsonObject LegacyProfileFromRoot(const QJsonObject &object) {
	auto result = QJsonObject();
	for (const auto &key : {
		kAccountKey,
		kFolderNameKey,
		kSyncFromFolderKey,
		u"allowedPeers"_q,
		u"allowedPeerIds"_q,
		u"manualPeers"_q,
		u"importedPeers"_q,
	}) {
		if (object.contains(key)) {
			result.insert(key, object.value(key));
		}
	}
	return result;
}

[[nodiscard]] QString ProfileFolderName(const QJsonObject &profile) {
	const auto configured = profile.value(kFolderNameKey).toString().trimmed();
	return configured.isEmpty() ? kDefaultFolderName : configured;
}

[[nodiscard]] bool SyncFromFolderEnabled(const QJsonObject &profile) {
	return !profile.contains(kSyncFromFolderKey)
		|| profile.value(kSyncFromFolderKey).toBool(true);
}

[[nodiscard]] bool MergeCurrentProfileDefaults(
		not_null<const Session*> session,
		QJsonObject *profile) {
	auto changed = false;
	const auto expectedAccount = MakeAccountObject(session);
	auto account = profile->value(kAccountKey).toObject();
	if (account != expectedAccount) {
		account = expectedAccount;
		profile->insert(kAccountKey, account);
		changed = true;
	}
	const auto folderName = ProfileFolderName(*profile);
	if (profile->value(kFolderNameKey).toString() != folderName) {
		profile->insert(kFolderNameKey, folderName);
		changed = true;
	}
	if (!profile->contains(kSyncFromFolderKey)) {
		profile->insert(kSyncFromFolderKey, true);
		changed = true;
	}
	for (const auto &key : { u"manualPeers"_q, u"importedPeers"_q }) {
		if (!profile->contains(key)) {
			profile->insert(key, QJsonArray());
			changed = true;
		}
	}
	return changed;
}

[[nodiscard]] base::flat_set<quint64> ParseProfilePeerIds(
		not_null<const Session*> session,
		const QJsonObject &profile) {
	auto result = base::flat_set<quint64>();
	AppendParsedIds(session, &result, profile.value(u"allowedPeers"_q));
	AppendParsedIds(session, &result, profile.value(u"allowedPeerIds"_q));
	AppendParsedIds(session, &result, profile.value(u"manualPeers"_q));
	AppendParsedIds(session, &result, profile.value(u"importedPeers"_q));
	return result;
}

[[nodiscard]] QJsonArray SerializePeerIds(
		const base::flat_set<quint64> &ids) {
	auto result = QJsonArray();
	for (const auto id : ids) {
		result.push_back(QString::number(id));
	}
	return result;
}

[[nodiscard]] bool UpdateImportedPeers(
		QJsonObject *profile,
		const base::flat_set<quint64> &ids) {
	const auto updated = SerializePeerIds(ids);
	if (profile->value(u"importedPeers"_q).toArray() == updated) {
		return false;
	}
	profile->insert(u"importedPeers"_q, updated);
	return true;
}

[[nodiscard]] std::optional<base::flat_set<quint64>> ReadFolderImportedPeers(
		not_null<const Session*> session,
		const QString &folderName) {
	const auto &filters = session->data().chatsFilters();
	if (!filters.loaded()) {
		return std::nullopt;
	}
	for (const auto &filter : filters.list()) {
		if (!filter.id()) {
			continue;
		}
		const auto title = filter.titleText().text.trimmed();
		if (title.compare(folderName, Qt::CaseInsensitive) != 0) {
			continue;
		}
		auto result = base::flat_set<quint64>();
		for (const auto &history : filter.always()) {
			result.emplace(SerializePeerId(history->peer->id));
		}
		return result;
	}
	return std::nullopt;
}

[[nodiscard]] bool WriteConfigFile(
		const QString &filePath,
		const QJsonObject &object) {
	const auto directoryPath = QFileInfo(filePath).absolutePath();
	if (!QDir().mkpath(directoryPath)) {
		return false;
	}
	QSaveFile file(filePath);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
		return false;
	}
	const auto bytes = QJsonDocument(object).toJson(QJsonDocument::Indented);
	if (file.write(bytes) != bytes.size()) {
		file.cancelWriting();
		return false;
	}
	return file.commit();
}

} // namespace

RestrictedAllowlist::RestrictedAllowlist(not_null<Session*> session)
: _session(session)
, _watcher(std::make_unique<QFileSystemWatcher>())
, _reloadTimer([=] { reload(); }) {
	QObject::connect(
		_watcher.get(),
		&QFileSystemWatcher::fileChanged,
		[=](const QString &) { scheduleReload(); });
	QObject::connect(
		_watcher.get(),
		&QFileSystemWatcher::directoryChanged,
		[=](const QString &) { scheduleReload(); });
	_session->data().chatsFilters().changed(
	) | rpl::start_with_next([=] {
		scheduleReload();
	}, _lifetime);
}

RestrictedAllowlist::~RestrictedAllowlist() = default;

void RestrictedAllowlist::start() {
	reload();
}

bool RestrictedAllowlist::isAllowed(PeerId peerId) const {
	// Saved Messages is the self peer and must remain visible.
	if (peerId == _session->userPeerId()) {
		return true;
	}
	return _allowedSerializedPeerIds.contains(SerializePeerId(peerId));
}

QString RestrictedAllowlist::path() const {
	return cWorkingDir() + u"tdata/restricted-allowlist.json"_q;
}

rpl::producer<> RestrictedAllowlist::changes() const {
	return _changes.events();
}

void RestrictedAllowlist::scheduleReload() {
	_reloadTimer.callOnce(kReloadDebounce);
}

void RestrictedAllowlist::reload() {
	refreshWatcherPaths();
	const auto parsed = readFile();
	if (!parsed || (*parsed == _allowedSerializedPeerIds)) {
		return;
	}
	apply(*parsed);
}

void RestrictedAllowlist::refreshWatcherPaths() {
	const auto filePath = path();
	const auto directoryPath = QFileInfo(filePath).absolutePath();
	if (!_watcher->directories().contains(directoryPath)
		&& QFileInfo(directoryPath).isDir()) {
		_watcher->addPath(directoryPath);
	}
	const auto exists = QFileInfo(filePath).isFile();
	const auto watched = _watcher->files().contains(filePath);
	if (exists && !watched) {
		_watcher->addPath(filePath);
	} else if (!exists && watched) {
		_watcher->removePath(filePath);
	}
}

void RestrictedAllowlist::apply(
		base::flat_set<quint64> allowedSerializedPeerIds) {
	_allowedSerializedPeerIds = std::move(allowedSerializedPeerIds);
	refreshVisibleState();
	refreshOpenWindows();
	_changes.fire({});
}

	void RestrictedAllowlist::refreshVisibleState() {
		const auto refreshHistory = [&](not_null<PeerData*> peer) {
			if (const auto history = _session->data().historyLoaded(peer)) {
				if (!_session->isRestrictedPeerAllowed(peer)) {
					Core::App().notifications().clearFromHistory(history);
					history->updateChatListExistence();
				} else {
					history->updateChatListSortPosition();
				}
			}
		};
	_session->data().enumerateUsers([&](not_null<UserData*> peer) {
		refreshHistory(not_null<PeerData*>(peer.get()));
	});
	_session->data().enumerateGroups(refreshHistory);
	_session->data().enumerateBroadcasts([&](not_null<ChannelData*> peer) {
		refreshHistory(not_null<PeerData*>(peer.get()));
	});
	_session->data().stories().refreshRestrictedState();
}

void RestrictedAllowlist::refreshOpenWindows() {
	for (const auto &window : _session->windows()) {
		const auto peer = window->activeChatCurrent().peer();
		if (peer && !_session->isRestrictedPeerAllowed(peer)) {
			window->clearSectionStack(
				{ anim::type::normal, anim::activation::background });
		}
	}
}

std::optional<base::flat_set<quint64>> RestrictedAllowlist::readFile() const {
	const auto filePath = path();
	auto root = QJsonObject();
	auto changed = false;
	if (QFileInfo(filePath).isFile()) {
		QFile file(filePath);
		if (!file.open(QIODevice::ReadOnly)) {
			return std::nullopt;
		}
		const auto bytes = file.readAll();
		file.close();

		auto error = QJsonParseError();
		const auto document = QJsonDocument::fromJson(bytes, &error);
		if (error.error != QJsonParseError::NoError || !document.isObject()) {
			return std::nullopt;
		}
		root = document.object();
	}

	auto profiles = root.value(kProfilesKey).toArray();
	if (profiles.isEmpty() && LooksLikeLegacyProfile(root)) {
		profiles.push_back(LegacyProfileFromRoot(root));
		root.insert(kProfilesKey, profiles);
		changed = true;
	}

	auto selected = -1;
	for (auto i = 0; i != profiles.size(); ++i) {
		const auto profile = profiles.at(i).toObject();
		if (MatchesAccount(_session, profile.value(kAccountKey).toObject())) {
			selected = i;
			break;
		}
	}
	if (selected < 0) {
		selected = profiles.size();
		profiles.push_back(QJsonObject());
		changed = true;
	}

	auto profile = profiles.at(selected).toObject();
	if (MergeCurrentProfileDefaults(_session, &profile)) {
		changed = true;
	}
	if (SyncFromFolderEnabled(profile)) {
		if (const auto imported = ReadFolderImportedPeers(
				_session,
				ProfileFolderName(profile))) {
			if (UpdateImportedPeers(&profile, *imported)) {
				changed = true;
			}
		}
	}

	if (profiles.at(selected).toObject() != profile) {
		profiles[selected] = profile;
		changed = true;
	}
	if (root.value(kProfilesKey).toArray() != profiles) {
		root.insert(kProfilesKey, profiles);
		changed = true;
	}
	if (changed) {
		WriteConfigFile(filePath, root);
	}
	return ParseProfilePeerIds(_session, profile);
}

} // namespace Main
