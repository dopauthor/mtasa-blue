-- Source: https://github.com/discordapp/discord-rpc/commit/e32d001809c4aad56cef2a5321b54442d830174f
project "discord-rpc"
    language "C++"
    kind "SharedLib"
    targetname "discord-rpc"
    targetdir(buildpath("mta"))

    includedirs {
        "include",
        "thirdparty/rapidjson-1.1.0/include",
    }

    defines {
        "DISCORD_DYNAMIC_LIB",
        "DISCORD_BUILDING_SDK",
    }

    links "Psapi"

    -- warnings "Extra"

    -- disablewarnings {
    --     4100, -- unreferenced formal parameter
    --     4514, -- unreferenced inline
    --     4625, -- copy constructor deleted
    --     5026, -- move constructor deleted
    --     4626, -- move assignment operator deleted
    --     4668, -- not defined preprocessor macro
    --     4710, -- function not inlined
    --     4711, -- function was inlined
    --     4820, -- structure padding
    --     4946, -- reinterpret_cast used between related classes
    --     5027, -- move assignment operator was implicitly defined as deleted
    -- }

    files {
        "premake5.lua",
        "include/**.h",
        "src/backoff.h",
        "src/connection_win.cpp",
        "src/connection.h",
        "src/discord_register_win.cpp",
        "src/discord_rpc.cpp",
        "src/dllmain.h",
        "src/msg_queue.h",
        "src/rpc_connection.cpp",
        "src/rpc_connection.h",
        "src/serialization.cpp",
        "src/serialization.h",
        "./thirdparty/rapidjson-1.1.0/include/**.h",
    }

    vpaths { 
        ["Include/*"]             = "include/**.h",
        ["Include/Third-Party/*"] = "thirdparty/rapidjson/include/**.h",
        ["Source/*"]              = { "src/**.cpp", "src/**.h" },
        ["*"]                     = "premake5.lua"
    }

    filter "architecture:x64"
        flags { "ExcludeFromBuild" } 
    filter "system:not windows"
        flags { "ExcludeFromBuild" }
