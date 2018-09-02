/*****************************************************************************
 *
 *  PROJECT:     Multi Theft Auto
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        core/CDiscordRichPresence.cpp
 *
 *  Multi Theft Auto is available from https://www.multitheftauto.com/
 *
 *****************************************************************************/
#include "StdInc.h"
#include "CDiscordRichPresence.h"
#include <discord_rpc.h>

using namespace Discord;

constexpr const char* DISCORD_APPLICATION_ID = "389006734896922624";

static CDiscordRichPresence* pInstance = nullptr;

CDiscordRichPresence::~CDiscordRichPresence()
{
    Shutdown();
}

void CDiscordRichPresence::Initialize()
{
    if (m_bInitialized)
        return;

    pInstance = this;

    DiscordEventHandlers handlers = { 0 };
    handlers.ready = HandleReady;
    handlers.disconnected = HandleDisconnected;
#if MTA_DEBUG
    handlers.errored = HandleError;
    handlers.joinGame = HandleJoin;
    handlers.spectateGame = HandleSpectate;
    handlers.joinRequest = HandleJoinRequest;
#endif

    Discord_Initialize(DISCORD_APPLICATION_ID, &handlers, 1, nullptr);
    m_bInitialized = true;
}

void CDiscordRichPresence::Shutdown()
{
    if (!m_bInitialized)
        return;

    pInstance = nullptr;
    Discord_Shutdown();
    m_bInitialized = false;
    m_ConnectionState = eConnectionState::OFFLINE;
}

void CDiscordRichPresence::DoPulse()
{
    if (!m_bInitialized || !m_bEnabled)
        return;

    Discord_RunCallbacks();
}

void CDiscordRichPresence::Enable()
{
    if (m_bEnabled)
        return;

    if (!m_bInitialized)
        Initialize();

    m_bEnabled = true;
    UpdatePresence();
}

void CDiscordRichPresence::Disable()
{
    if (!m_bEnabled)
        return;

    m_bEnabled = false;

    if (m_bInitialized)
        Discord_ClearPresence();
}

const User* CDiscordRichPresence::GetUser()
{
    if (m_ConnectionState == eConnectionState::OFFLINE)
        return nullptr;

    return &m_DiscordUser;
}

void CDiscordRichPresence::UpdatePresence()
{
    if (!m_bInitialized || !m_bEnabled || m_ConnectionState == eConnectionState::OFFLINE || m_PresenceState == ePresenceState::NONE)
        return;

    DiscordRichPresence presence = { 0 };
    presence.largeImageKey = "default";
    presence.startTimestamp = m_tPresenceTime;

    switch (m_PresenceState)
    {
    case ePresenceState::MAIN_MENU:
        presence.state = "In Main Menu";
        break;
    case ePresenceState::ON_SERVER:
        presence.state = "In Game";
        presence.details = m_strServerName.c_str();
        presence.partySize = m_uiPlayerCount;
        presence.partyMax = m_uiMaxPlayerCount;
        break;
    default:
        break;
    }

    Discord_UpdatePresence(&presence);
}

void CDiscordRichPresence::SetMainMenuPresence()
{
    m_PresenceState = ePresenceState::MAIN_MENU;
    m_tPresenceTime = std::time(nullptr);
    UpdatePresence();
}

void CDiscordRichPresence::SetServerPresence(bool bOutdatedServer)
{
    if (bOutdatedServer)
    {
        // TODO: Read server name and player limit from server query protocol
    }

    m_PresenceState = ePresenceState::ON_SERVER;
    m_tPresenceTime = std::time(nullptr);
    UpdatePresence();
}

void CDiscordRichPresence::SetPresenceServerName(const std::string& strServerName)
{
    m_strServerName = strServerName;
}

void CDiscordRichPresence::SetPresencePlayerCount(std::size_t uiPlayerCount)
{
    m_uiPlayerCount = uiPlayerCount;
}

void CDiscordRichPresence::SetPresenceMaxPlayerCount(std::size_t uiMaxPlayerCount)
{
    m_uiMaxPlayerCount = uiMaxPlayerCount;
}

void CDiscordRichPresence::HandleReady(const DiscordUser* pLocalUser)
{
    if (!pInstance)
        return;

    Discord::User& user = pInstance->m_DiscordUser;
    user.SetUserID(pLocalUser->userId);
    user.SetUsername(pLocalUser->username);
    user.SetDiscriminator(pLocalUser->discriminator);

    pInstance->m_ConnectionState = eConnectionState::READY;
    pInstance->UpdatePresence();
}

void CDiscordRichPresence::HandleDisconnected(int iErrorCode, const char* szMessage)
{
    if (!pInstance)
        return;

    pInstance->m_ConnectionState = eConnectionState::OFFLINE;
}

void CDiscordRichPresence::HandleError(int iErrorCode, const char* szMessage)
{
    OutputDebugLine(SString{ "Discord: error (%d: %s)\n", iErrorCode, szMessage }.c_str());
}

void CDiscordRichPresence::HandleJoin(const char* szSecret)
{
    OutputDebugLine(SString{ "Discord: join (%s)\n", szSecret }.c_str());
}

void CDiscordRichPresence::HandleSpectate(const char* szSecret)
{
    OutputDebugLine(SString{ "Discord: spectate (%s)\n", szSecret }.c_str());
}

void CDiscordRichPresence::HandleJoinRequest(const DiscordUser* pRequestUser)
{
    OutputDebugLine(SString{ "Discord: join request from user %s#%s - %s\n",
        pRequestUser->username,
        pRequestUser->discriminator,
        pRequestUser->userId }.c_str());
}
