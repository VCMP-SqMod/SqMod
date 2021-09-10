# Create the DPP library
add_library(DPP STATIC
    include/dpp/nlohmann/json.hpp
    include/dpp/nlohmann/json_fwd.hpp
    include/dpp/auditlog.h
    include/dpp/ban.h
    include/dpp/cache.h
    include/dpp/channel.h
    include/dpp/cluster.h
    include/dpp/commandhandler.h
    include/dpp/discord.h
    include/dpp/discordclient.h
    include/dpp/discordevents.h
    include/dpp/discordvoiceclient.h
    include/dpp/dispatcher.h
    include/dpp/dpp.h
    include/dpp/dtemplate.h
    include/dpp/emoji.h
    include/dpp/event.h
    include/dpp/export.h
    include/dpp/guild.h
    include/dpp/httplib.h
    include/dpp/integration.h
    include/dpp/intents.h
    include/dpp/invite.h
    include/dpp/json_fwd.hpp
    include/dpp/message.h
    include/dpp/presence.h
    include/dpp/prune.h
    include/dpp/queues.h
    include/dpp/role.h
    include/dpp/slashcommand.h
    include/dpp/sslclient.h
    include/dpp/stringops.h
    include/dpp/user.h
    include/dpp/version.h
    include/dpp/voiceregion.h
    include/dpp/voicestate.h
    include/dpp/webhook.h
    include/dpp/wsclient.h
    src/dpp/ban.cpp
    src/dpp/cache.cpp
    src/dpp/channel.cpp
    src/dpp/cluster.cpp
    src/dpp/commandhandler.cpp
    src/dpp/discordclient.cpp
    src/dpp/discordevents.cpp
    src/dpp/discordvoiceclient.cpp
    src/dpp/dispatcher.cpp
    src/dpp/dtemplate.cpp
    src/dpp/emoji.cpp
    src/dpp/guild.cpp
    src/dpp/httplib.cpp
    src/dpp/integration.cpp
    src/dpp/invite.cpp
    src/dpp/managed.cpp
    src/dpp/message.cpp
    src/dpp/presence.cpp
    src/dpp/prune.cpp
    src/dpp/queues.cpp
    src/dpp/role.cpp
    src/dpp/slashcommand.cpp
    src/dpp/sslclient.cpp
    src/dpp/user.cpp
    src/dpp/utility.cpp
    src/dpp/voiceregion.cpp
    src/dpp/voicestate.cpp
    src/dpp/webhook.cpp
    src/dpp/wsclient.cpp
    src/dpp/auditlog.cpp
    src/dpp/events/application_command_update.cpp
    src/dpp/events/channel_create.cpp
    src/dpp/events/channel_delete.cpp
    src/dpp/events/channel_pins_update.cpp
    src/dpp/events/channel_update.cpp
    src/dpp/events/guild_ban_add.cpp
    src/dpp/events/guild_ban_remove.cpp
    src/dpp/events/guild_create.cpp
    src/dpp/events/guild_delete.cpp
    src/dpp/events/guild_emojis_update.cpp
    src/dpp/events/guild_integrations_update.cpp
    src/dpp/events/guild_join_request_delete.cpp
    src/dpp/events/guild_member_add.cpp
    src/dpp/events/guild_member_remove.cpp
    src/dpp/events/guild_member_update.cpp
    src/dpp/events/guild_members_chunk.cpp
    src/dpp/events/guild_role_create.cpp
    src/dpp/events/guild_role_delete.cpp
    src/dpp/events/guild_role_update.cpp
    src/dpp/events/guild_stickers_update.cpp
    src/dpp/events/guild_update.cpp
    src/dpp/events/integration_create.cpp
    src/dpp/events/integration_delete.cpp
    src/dpp/events/integration_update.cpp
    src/dpp/events/interaction_create.cpp
    src/dpp/events/invite_create.cpp
    src/dpp/events/invite_delete.cpp
    src/dpp/events/logger.cpp
    src/dpp/events/message_create.cpp
    src/dpp/events/message_delete.cpp
    src/dpp/events/message_delete_bulk.cpp
    src/dpp/events/message_reaction_add.cpp
    src/dpp/events/message_reaction_remove.cpp
    src/dpp/events/message_reaction_remove_all.cpp
    src/dpp/events/message_reaction_remove_emoji.cpp
    src/dpp/events/message_update.cpp
    src/dpp/events/presence_update.cpp
    src/dpp/events/ready.cpp
    src/dpp/events/resumed.cpp
    src/dpp/events/stage_instance_create.cpp
    src/dpp/events/stage_instance_delete.cpp
    src/dpp/events/thread_create.cpp
    src/dpp/events/thread_delete.cpp
    src/dpp/events/thread_list_sync.cpp
    src/dpp/events/thread_member_update.cpp
    src/dpp/events/thread_members_update.cpp
    src/dpp/events/thread_update.cpp
    src/dpp/events/typing_start.cpp
    src/dpp/events/user_update.cpp
    src/dpp/events/voice_server_update.cpp
    src/dpp/events/voice_state_update.cpp
    src/dpp/events/webhooks_update.cpp
    src/dpp/events/application_command_create.cpp
    src/dpp/events/application_command_delete.cpp
)
# Configure include folders
target_include_directories(DPP PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src)
target_include_directories(DPP PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src/dpp)
target_include_directories(DPP PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
# Link to built-in third-party libraries
target_link_libraries(DPP PUBLIC fmt::fmt)
# Project defines
target_compile_definitions(DPP PUBLIC DPP_BUILD=1)
# Platform defines
if(WIN32)
    target_compile_definitions(DPP PRIVATE _WIN32_WINNT=0x0601 OPENSSL_SYS_WIN32=1 _WINSOCK_DEPRECATED_NO_WARNINGS=1 WIN32_LEAN_AND_MEAN=1 _CRT_SECURE_NO_WARNINGS=1 _CRT_NONSTDC_NO_DEPRECATE=1)
    if (MINGW)
        target_compile_definitions(DPP PUBLIC WIN32=1)
    endif()
    target_link_libraries(DPP PRIVATE Crypt32)
endif()
# Third-party library preferences
set(THREADS_PREFER_PTHREAD_FLAG ON)
# Find required third-party libraries
find_package(Threads REQUIRED)
find_package(ZLIB REQUIRED)
find_package(OpenSSL REQUIRED)
# Link to required third-party libraries
target_link_libraries(DPP PUBLIC Threads::Threads ZLIB::ZLIB OpenSSL::Crypto OpenSSL::SSL)
# Include the custom module folder
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/cmake/")
# Enable voice support in discord library
if (ENABLE_DISCORD_VOICE)
    # Look for sodium and opus libraries
    include("cmake/FindSodium.cmake")
    include("cmake/FindOpus.cmake")
    # Was the opus library found?
    if(DEFINED OPUS_FOUND)
        message(STATUS "Opus library was found")
        # Link to opus library
        target_include_directories(DPP PUBLIC ${OPUS_INCLUDE_DIRS})
        target_link_libraries(DPP PUBLIC ${OPUS_LIBRARIES})
        # Was the sodium library found?
        if(DEFINED sodium_VERSION_STRING)
            message(STATUS "Sodium library was found")
            message(STATUS "DPP voice support enabled")
            # Let the code know about this
            target_compile_definitions(DPP PRIVATE HAVE_VOICE=1)
            # Link to sodium library
            target_include_directories(DPP PUBLIC ${sodium_INCLUDE_DIR})
            target_link_libraries(DPP PUBLIC ${sodium_LIBRARY_RELEASE})
        endif()
    endif()
endif()
