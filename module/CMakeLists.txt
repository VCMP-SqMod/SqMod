# Create the Squirrel module
add_library(SqModule MODULE SqBase.hpp Main.cpp
	# VCMP
	VCMP/vcmp.h
	VCMP/vcmp20.h
	VCMP/vcmp21.h
	# Sqrat
	Sqrat/sqratAllocator.h
	Sqrat/sqratArray.h
	Sqrat/sqratClass.h
	Sqrat/sqratClassType.h
	Sqrat/sqratConst.h
	Sqrat/sqratFunction.h
	Sqrat/sqratGlobalMethods.h
	Sqrat/sqratLightObj.h
	Sqrat/sqratMemberMethods.h
	Sqrat/sqratObject.h
	Sqrat/sqratOverloadMethods.h
	Sqrat/sqratScript.h
	Sqrat/sqratTable.h
	Sqrat/sqratTypes.h
	Sqrat/sqratUtil.h
	# Base
	Base/AABB.cpp Base/AABB.hpp
	Base/Circle.cpp Base/Circle.hpp
	Base/Color3.cpp Base/Color3.hpp
	Base/Color4.cpp Base/Color4.hpp
	Base/Quaternion.cpp Base/Quaternion.hpp
	Base/Shared.cpp Base/Shared.hpp
	Base/Sphere.cpp Base/Sphere.hpp
	Base/Vector2.cpp Base/Vector2.hpp
	Base/Vector2i.cpp Base/Vector2i.hpp
	Base/Vector3.cpp Base/Vector3.hpp
	Base/Vector4.cpp Base/Vector4.hpp
	# Core
	Core/Areas.cpp Core/Areas.hpp
	Core/Buffer.cpp Core/Buffer.hpp
	Core/Command.cpp Core/Command.hpp
	Core/Common.cpp Core/Common.hpp
	Core/Entity.cpp Core/Entity.hpp
	Core/Routine.cpp Core/Routine.hpp
	Core/Script.cpp Core/Script.hpp
	Core/Signal.cpp Core/Signal.hpp
	Core/Tasks.cpp Core/Tasks.hpp
	Core/Utility.cpp Core/Utility.hpp
	# Entity
	Entity/Blip.cpp Entity/Blip.hpp
	Entity/Checkpoint.cpp Entity/Checkpoint.hpp
    Entity/KeyBind.cpp Entity/KeyBind.hpp
	Entity/Object.cpp Entity/Object.hpp
	Entity/Pickup.cpp Entity/Pickup.hpp
	Entity/Player.cpp Entity/Player.hpp
	Entity/Vehicle.cpp Entity/Vehicle.hpp
	# Library
	Library/Chrono.cpp Library/Chrono.hpp
	Library/Chrono/Date.cpp Library/Chrono/Date.hpp
	Library/Chrono/Datetime.cpp Library/Chrono/Datetime.hpp
	Library/Chrono/Time.cpp Library/Chrono/Time.hpp
	Library/Chrono/Timer.cpp Library/Chrono/Timer.hpp
	Library/Chrono/Timestamp.cpp Library/Chrono/Timestamp.hpp
	Library/IO.cpp Library/IO.hpp
	Library/IO/Buffer.cpp Library/IO/Buffer.hpp
	Library/IO/File.cpp Library/IO/File.hpp
	Library/IO/INI.cpp Library/IO/INI.hpp
	Library/MMDB.cpp Library/MMDB.hpp
	Library/Numeric.cpp Library/Numeric.hpp
	Library/Numeric/Long.cpp Library/Numeric/Long.hpp
	Library/Numeric/Math.cpp Library/Numeric/Math.hpp
	Library/Numeric/Random.cpp Library/Numeric/Random.hpp
	Library/String.cpp Library/String.hpp
	Library/System.cpp Library/System.hpp
	Library/System/Dir.cpp Library/System/Dir.hpp
	Library/System/Env.cpp Library/System/Env.hpp
	Library/System/Path.cpp Library/System/Path.hpp
	Library/Utils.cpp Library/Utils.hpp
	Library/Utils/Announce.cpp Library/Utils/Announce.hpp
	Library/Utils/Map.cpp Library/Utils/Map.hpp
	Library/Utils/Vector.cpp Library/Utils/Vector.hpp
	Library/ZMQ.cpp Library/ZMQ.hpp
	# Misc
	Misc/Broadcast.cpp
	Misc/Constants.cpp
	Misc/Register.cpp
	Misc/Algo.cpp Misc/Algo.hpp
	Misc/Functions.cpp Misc/Functions.hpp
	Misc/Model.cpp Misc/Model.hpp
	Misc/Player.cpp Misc/Player.hpp
	Misc/Vehicle.cpp Misc/Vehicle.hpp
	Misc/Weapon.cpp Misc/Weapon.hpp
	# POCO
	PocoLib/Crypto.cpp PocoLib/Crypto.hpp
	PocoLib/Data.cpp PocoLib/Data.hpp
	PocoLib/JSON.cpp PocoLib/JSON.hpp
	PocoLib/Net.cpp PocoLib/Net.hpp
	PocoLib/RegEx.cpp PocoLib/RegEx.hpp
	PocoLib/Register.cpp PocoLib/Register.hpp
	PocoLib/Time.cpp PocoLib/Time.hpp
	PocoLib/Util.cpp PocoLib/Util.hpp
	PocoLib/XML.cpp PocoLib/XML.hpp
	# 
	Core.cpp Core.hpp
	Logger.cpp Logger.hpp
	Register.cpp
)
# Link to windows libraries if on windows
if(WIN32 OR MINGW)
	target_compile_definitions(SqModule PRIVATE _WIN32_WINNT=0x0601)
	target_link_libraries(SqModule wsock32 ws2_32 shlwapi)
endif()
# Link to base libraries
target_link_libraries(SqModule Squirrel FmtLib SimpleINI TinyDir ConcurrentQueue maxminddb libzmq-static)
# Link to POCO libraries
target_link_libraries(SqModule Poco::Foundation Poco::Crypto Poco::Data Poco::Net Poco::JSON Poco::XML)
# Does POCO have SQLite support?
if(ENABLE_DATA_SQLITE)
	# Link the libraries
	target_link_libraries(SqModule Poco::DataSQLite)
	# Inform the plugin that it can make use of this library
	target_compile_definitions(SqModule PRIVATE SQMOD_POCO_HAS_SQLITE=1)
endif()
# Does POCO have MySLQ support?
find_package(MySQL)
if(MYSQL_FOUND)
	# Link the libraries
	target_link_libraries(SqModule Poco::DataMySQL)
	# Inform the plugin that it can make use of this library
	target_compile_definitions(SqModule PRIVATE SQMOD_POCO_HAS_MYSQL=1)
endif()
# Does POCO have PostgreSQL support?
find_package(PostgreSQL)
if(POSTGRESQL_FOUND)
	# Link the libraries
	target_link_libraries(SqModule Poco::DataPostgreSQL)
	# Inform the plugin that it can make use of this library
	target_compile_definitions(SqModule PRIVATE SQMOD_POCO_HAS_POSTGRESQL=1)
endif()
# Determine if build mode
if(${CMAKE_BUILD_TYPE} MATCHES "(Release)+")
	target_compile_definitions(SqModule PRIVATE NDEBUG=1)
else()
	target_compile_definitions(SqModule PRIVATE _DEBUG=1 SQMOD_EXCEPTLOC=1)
endif()
# Force 32-bit binaries when necessary
if(FORCE_32BIT_BIN)
	set_target_properties(SqModule PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")
endif()
# Don't prefix the module binary.
set_target_properties(SqModule PROPERTIES PREFIX "")
# Customize module binary name/
if(WIN32)
	if(CMAKE_SIZEOF_VOID_P EQUAL 8 AND NOT FORCE_32BIT_BIN)
		set_target_properties(SqModule PROPERTIES OUTPUT_NAME "mod_squirrel_64")
	else()
		set_target_properties(SqModule PROPERTIES OUTPUT_NAME "mod_squirrel_32")
	endif()
else(WIN32)
	if(CMAKE_SIZEOF_VOID_P EQUAL 8 AND NOT FORCE_32BIT_BIN)
		set_target_properties(SqModule PROPERTIES OUTPUT_NAME "mod_squirrel_64")
	else()
		set_target_properties(SqModule PROPERTIES OUTPUT_NAME "mod_squirrel_32")
	endif()
endif(WIN32)
# Include current directory in the search path
target_include_directories(SqModule PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(SqModule PRIVATE ${CMAKE_CURRENT_LIST_DIR}/VCMP)
target_include_directories(SqModule PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Sqrat)
# Copy module into the plug-ins folder
add_custom_command(TARGET SqModule POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:SqModule> "${PROJECT_SOURCE_DIR}/bin/plugins")
# Copy several dependent DLLs on windows to make distribution easier (used mainly by people that distribute builds)
if(WIN32 AND MINGW AND COPY_DEPENDENCIES)
	get_filename_component(MINGW_BIN_PATH ${CMAKE_C_COMPILER} DIRECTORY REALPATH)
	if(NOT IS_DIRECTORY ${MINGW_BIN_PATH})
	    get_filename_component(MINGW_BIN_PATH ${CMAKE_CXX_COMPILER} DIRECTORY REALPATH)
	endif()
	# Make sure the deps folder exists
	file(MAKE_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/deps")
	# Copy dependencies into the plug-ins folder (only so it can be distributed with the DLL)
	file(COPY "${MINGW_BIN_PATH}/zlib1.dll" DESTINATION "${PROJECT_SOURCE_DIR}/bin/deps")
	file(COPY "${MINGW_BIN_PATH}/libpq.dll" DESTINATION "${PROJECT_SOURCE_DIR}/bin/deps")
	file(COPY "${MINGW_BIN_PATH}/libzstd.dll" DESTINATION "${PROJECT_SOURCE_DIR}/bin/deps")
	file(COPY "${MINGW_BIN_PATH}/libpsl-5.dll" DESTINATION "${PROJECT_SOURCE_DIR}/bin/deps")
	file(COPY "${MINGW_BIN_PATH}/libcurl-4.dll" DESTINATION "${PROJECT_SOURCE_DIR}/bin/deps")
	file(COPY "${MINGW_BIN_PATH}/libssh2-1.dll" DESTINATION "${PROJECT_SOURCE_DIR}/bin/deps")
	file(COPY "${MINGW_BIN_PATH}/libidn2-0.dll" DESTINATION "${PROJECT_SOURCE_DIR}/bin/deps")
	file(COPY "${MINGW_BIN_PATH}/libintl-8.dll" DESTINATION "${PROJECT_SOURCE_DIR}/bin/deps")
	file(COPY "${MINGW_BIN_PATH}/libxml2-2.dll" DESTINATION "${PROJECT_SOURCE_DIR}/bin/deps")
	file(COPY "${MINGW_BIN_PATH}/liblzma-5.dll" DESTINATION "${PROJECT_SOURCE_DIR}/bin/deps")
	file(COPY "${MINGW_BIN_PATH}/libiconv-2.dll" DESTINATION "${PROJECT_SOURCE_DIR}/bin/deps")
	file(COPY "${MINGW_BIN_PATH}/libmariadb.dll" DESTINATION "${PROJECT_SOURCE_DIR}/bin/deps")
	file(COPY "${MINGW_BIN_PATH}/libbrotlidec.dll" DESTINATION "${PROJECT_SOURCE_DIR}/bin/deps")
	file(COPY "${MINGW_BIN_PATH}/libbrotlicommon.dll" DESTINATION "${PROJECT_SOURCE_DIR}/bin/deps")
	file(COPY "${MINGW_BIN_PATH}/libunistring-2.dll" DESTINATION "${PROJECT_SOURCE_DIR}/bin/deps")
	file(COPY "${MINGW_BIN_PATH}/libnghttp2-14.dll" DESTINATION "${PROJECT_SOURCE_DIR}/bin/deps")
	file(COPY "${MINGW_BIN_PATH}/libwinpthread-1.dll" DESTINATION "${PROJECT_SOURCE_DIR}/bin/deps")
	file(COPY "${MINGW_BIN_PATH}/libstdc++-6.dll" DESTINATION "${PROJECT_SOURCE_DIR}/bin/deps")
	if(CMAKE_SIZEOF_VOID_P EQUAL 8 AND NOT FORCE_32BIT_BIN)
		file(COPY "${MINGW_BIN_PATH}/libgcc_s_seh-1.dll" DESTINATION "${PROJECT_SOURCE_DIR}/bin/deps")
		file(COPY "${MINGW_BIN_PATH}/libssl-1_1-x64.dll" DESTINATION "${PROJECT_SOURCE_DIR}/bin/deps")
		file(COPY "${MINGW_BIN_PATH}/libcrypto-1_1-x64.dll" DESTINATION "${PROJECT_SOURCE_DIR}/bin/deps")
	else()
		file(COPY "${MINGW_BIN_PATH}/libssl-1_1.dll" DESTINATION "${PROJECT_SOURCE_DIR}/bin/deps")
		file(COPY "${MINGW_BIN_PATH}/libcrypto-1_1.dll" DESTINATION "${PROJECT_SOURCE_DIR}/bin/deps")
	endif()
	if(POCO_UNBUNDLED)
		file(COPY "${MINGW_BIN_PATH}/libexpat-1.dll" DESTINATION "${PROJECT_SOURCE_DIR}/bin/deps")
		file(COPY "${MINGW_BIN_PATH}/libsqlite3-0.dll" DESTINATION "${PROJECT_SOURCE_DIR}/bin/deps")
	endif()
endif()
