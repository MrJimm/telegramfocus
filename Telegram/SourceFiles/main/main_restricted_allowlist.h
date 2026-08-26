#pragma once

#include "base/flat_set.h"
#include "base/timer.h"
#include "data/data_peer_id.h"

#include <rpl/event_stream.h>
#include <rpl/lifetime.h>

#include <QtCore/QString>

#include <memory>
#include <optional>

class QFileSystemWatcher;

namespace Main {

class Session;

enum class RestrictedAllowlistFolderImportState {
	Loading,
	Missing,
	Present,
};

struct RestrictedAllowlistFolderImportResult {
	RestrictedAllowlistFolderImportState state
		= RestrictedAllowlistFolderImportState::Loading;
	base::flat_set<quint64> ids;
};

enum class RestrictedAllowlistReadState {
	Loaded,
	NotReady,
	Invalid,
};

struct RestrictedAllowlistReadResult {
	RestrictedAllowlistReadState state
		= RestrictedAllowlistReadState::NotReady;
	base::flat_set<quint64> ids;
};

class RestrictedAllowlist final {
public:
	explicit RestrictedAllowlist(not_null<Session*> session);
	~RestrictedAllowlist();

	void start();
	[[nodiscard]] bool isAllowed(PeerId peerId) const;
	[[nodiscard]] QString path() const;
	[[nodiscard]] rpl::producer<> changes() const;
	[[nodiscard]] RestrictedAllowlistReadState readState() const;
	[[nodiscard]] rpl::producer<> invalidTransitions() const;

private:
	void scheduleReload();
	void reload();
	void refreshWatcherPaths();
	void apply(base::flat_set<quint64> allowedSerializedPeerIds);
	void refreshVisibleState();
	void refreshOpenWindows();
	void updateReadState(RestrictedAllowlistReadState state);

	[[nodiscard]] RestrictedAllowlistReadResult readFile() const;

	const not_null<Session*> _session;
	std::unique_ptr<QFileSystemWatcher> _watcher;
	base::Timer _reloadTimer;
	base::flat_set<quint64> _allowedSerializedPeerIds;
	rpl::event_stream<> _changes;
	RestrictedAllowlistReadState _readState
		= RestrictedAllowlistReadState::NotReady;
	rpl::event_stream<> _invalidTransitions;
	rpl::lifetime _lifetime;
};

} // namespace Main
