/*****************************************************************************
 *
 *  PROJECT:     Multi Theft Auto
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        core/CDiscordRichPresence.h
 *
 *  Multi Theft Auto is available from https://www.multitheftauto.com/
 *
 *****************************************************************************/
#pragma once

#include <core/CDiscordInterface.h>
#include <ctime>

class CDiscordRichPresence : public Discord::CRichPresenceInterface
{
public:
    CDiscordRichPresence() = default;
    ~CDiscordRichPresence();
    CDiscordRichPresence(const CDiscordRichPresence&) = delete;
    CDiscordRichPresence(CDiscordRichPresence&&) = delete;
    CDiscordRichPresence& operator=(const CDiscordRichPresence&) = delete;
    CDiscordRichPresence& operator=(CDiscordRichPresence&&) = delete;

public:
    void Initialize();
    void Shutdown();
    void DoPulse();

public:
    void                 Enable() override;
    void                 Disable() override;
    const Discord::User* GetUser() override;
    void                 UpdatePresence() override;
    void                 SetMainMenuPresence() override;
    void                 SetServerPresence(const std::string& strServerName, std::size_t uiPlayerCount, std::size_t uiMaxPlayerCount) override;
    void                 SetPresenceServerName(const std::string& strServerName) override;
    void                 SetPresencePlayerCount(std::size_t uiPlayerCount) override;
    void                 SetPresenceMaxPlayerCount(std::size_t uiMaxPlayerCount) override;

private:
    static void HandleReady(const struct DiscordUser* pLocalUser);
    static void HandleDisconnected(int iErrorCode, const char* szMessage);
    static void HandleError(int iErrorCode, const char* szMessage);
    static void HandleJoin(const char* szSecret);
    static void HandleSpectate(const char* szSecret);
    static void HandleJoinRequest(const struct DiscordUser* pRequestUser);

private:
    bool                      m_bInitialized = false;
    bool                      m_bEnabled = false;
    std::time_t               m_tPresenceTime = 0;
    Discord::ePresenceState   m_PresenceState = Discord::ePresenceState::NONE;
    Discord::eConnectionState m_ConnectionState = Discord::eConnectionState::OFFLINE;
    Discord::User             m_DiscordUser;
    std::string               m_strServerName;
    std::size_t               m_uiPlayerCount = 0U;
    std::size_t               m_uiMaxPlayerCount = 0U;
};
