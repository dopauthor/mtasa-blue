/*****************************************************************************
 *
 *  PROJECT:     Multi Theft Auto
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        sdk/core/CDiscordInterface.h
 *
 *  Multi Theft Auto is available from https://www.multitheftauto.com/
 *
 *****************************************************************************/
#pragma once

#include <string>
#include <cstddef>

namespace Discord
{
    enum class ePresenceState
    {
        NONE,
        MAIN_MENU,
        ON_SERVER,
    };

    enum class eConnectionState
    {
        OFFLINE,
        READY,
    };

    class User
    {
    public:
        void SetUserID(const char* szUserID) { m_strUserID = std::string(szUserID); }
        void SetUsername(const char* szUsername) { m_strUsername = std::string(szUsername); }
        void SetDiscriminator(const char* szDiscriminator) { m_strDiscriminator = std::string(szDiscriminator); }

        const std::string& GetUserID() const noexcept { return m_strUserID; }
        const std::string& GetUsername() const noexcept { return m_strUsername; }
        const std::string& GetDiscriminator() const noexcept { return m_strDiscriminator; }

    private:
        std::string m_strUserID;
        std::string m_strUsername;
        std::string m_strDiscriminator;
    };

    class CRichPresenceInterface
    {
    public:
        virtual void        Enable() = 0;
        virtual void        Disable() = 0;
        virtual const User* GetUser() = 0;
        virtual void        UpdatePresence() = 0;
        virtual void        SetMainMenuPresence() = 0;
        virtual void        SetServerPresence(const std::string& strServerName, std::size_t uiPlayerCount, std::size_t uiMaxPlayerCount) = 0;
        virtual void        SetPresenceServerName(const std::string& strServerName) = 0;
        virtual void        SetPresencePlayerCount(std::size_t uiPlayerCount) = 0;
        virtual void        SetPresenceMaxPlayerCount(std::size_t uiMaxPlayerCount) = 0;
    };
}
