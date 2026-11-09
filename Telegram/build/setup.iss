#define MyAppShortName "Telegram Focus"
#define MyAppName "Telegram Focus"
#define MyAppPublisher "MrJimmsky"
#define MyAppURL "https://github.com/MrJimm/telegramfocus"
#define MyAppSupportURL "https://github.com/MrJimm/telegramfocus/issues"
#define MyAppUpdatesURL "https://github.com/MrJimm/telegramfocus/releases"
#define MyAppExeName "TelegramFocus.exe"
#define MyAppId "5AF40AB4-E710-4527-BDD0-5554817EACA6"
#define CurrentYear GetDateTimeString('yyyy','','')

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{{#MyAppId}}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppCopyright={#MyAppPublisher} 2014-{#CurrentYear}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppSupportURL}
AppUpdatesURL={#MyAppUpdatesURL}
DefaultDirName={userappdata}\{#MyAppName}
DefaultGroupName={#MyAppName}
AllowNoIcons=yes
OutputDir={#ReleasePath}
SetupIconFile={#SourcePath}..\Resources\art\icon256.ico
UninstallDisplayName={#MyAppName}
UninstallDisplayIcon={app}\{#MyAppExeName}
; Discard legacy uninstall records that recursively removed user profiles.
UninstallLogMode=overwrite
Compression=lzma
SolidCompression=yes
DisableStartupPrompt=yes
PrivilegesRequired=lowest
VersionInfoVersion={#MyAppVersion}.0
CloseApplications=yes
DisableDirPage=no
DisableProgramGroupPage=no
WizardStyle=modern
; This community build is intentionally unsigned. The upstream signing key is private.

#ifndef MyOutputBaseFilename
  #if MyBuildTarget == "winarm"
    #define MyOutputBaseFilename "TelegramFocus-" + MyAppVersionFull + "-arm64-setup"
  #elif MyBuildTarget == "win64"
    #define MyOutputBaseFilename "TelegramFocus-" + MyAppVersionFull + "-x64-setup"
  #else
    #define MyOutputBaseFilename "TelegramFocus-" + MyAppVersionFull + "-x86-setup"
  #endif
#endif
OutputBaseFilename={#MyOutputBaseFilename}

#if MyBuildTarget == "winarm"
  ArchitecturesAllowed="arm64"
  #define ArchModulesFolder "arm64"
  AppVerName={#MyAppName} {#MyAppVersion} arm64
#elif MyBuildTarget == "win64"
  ArchitecturesAllowed="x64compatible"
  ArchitecturesInstallIn64BitMode="x64compatible"
  #define ArchModulesFolder "x64"
  AppVerName={#MyAppName} {#MyAppVersion} 64bit
#else
  #define ArchModulesFolder "x86"
  AppVerName={#MyAppName} {#MyAppVersion} 32bit
#endif

#define ModulesFolder "modules\" + ArchModulesFolder

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "it";      MessagesFile: "compiler:Languages\Italian.isl"
Name: "es";      MessagesFile: "compiler:Languages\Spanish.isl"
Name: "de";      MessagesFile: "compiler:Languages\German.isl"
Name: "nl";      MessagesFile: "compiler:Languages\Dutch.isl"
Name: "pt_BR";   MessagesFile: "compiler:Languages\BrazilianPortuguese.isl"
Name: "ru";      MessagesFile: "compiler:Languages\Russian.isl"
Name: "fr";      MessagesFile: "compiler:Languages\French.isl"
Name: "ua";      MessagesFile: "compiler:Languages\Ukrainian.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"

[Files]
Source: "{#ReleasePath}\Telegram.exe"; DestDir: "{app}"; DestName: "{#MyAppExeName}"; Flags: ignoreversion
#if MyBuildTarget != "winarm"
Source: "{#ReleasePath}\{#ModulesFolder}\d3d\d3dcompiler_47.dll"; DestDir: "{app}\{#ModulesFolder}\d3d"; Flags: ignoreversion
#endif
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{group}\{#MyAppShortName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{group}\{cm:UninstallProgram,{#MyAppShortName}}"; Filename: "{uninstallexe}"
Name: "{userdesktop}\{#MyAppShortName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppShortName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

[Code]
procedure CurUninstallStepChanged(CurUninstallStep: TUninstallStep);
var ResultCode: Integer;
begin
  if CurUninstallStep = usUninstall then
  begin
    Exec(ExpandConstant('{app}\{#MyAppExeName}'), '-cleanup', '', SW_HIDE, ewWaitUntilTerminated, ResultCode);
  end;
end;
