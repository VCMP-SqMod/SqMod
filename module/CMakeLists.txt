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
	Library/Crypt.cpp Library/Crypt.hpp
	Library/CURL.cpp Library/CURL.hpp
	Library/IO.cpp Library/IO.hpp
	Library/IO/Buffer.cpp Library/IO/Buffer.hpp
	Library/IO/File.cpp Library/IO/File.hpp
	Library/IO/INI.cpp Library/IO/INI.hpp
	Library/IO/JSON.cpp Library/IO/JSON.hpp
	Library/IO/XML.cpp Library/IO/XML.hpp
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
target_link_libraries(SqModule Squirrel FmtLib SimpleINI TinyDir ConcurrentQueue cpr::cpr)
# Link to POCO libraries
target_link_libraries(SqModule Poco::Foundation Poco::Encodings Poco::Crypto Poco::Util Poco::Data Poco::Net Poco::JSON Poco::XML Poco::Zip Poco::JWT Poco::Redis)
# Determine if build mode
if(CMAKE_BUILD_TYPE MATCHES "(Release)+")
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
# Custmize module binary name/
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
# Copy module into the plugins folder
add_custom_command(TARGET SqModule POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:SqModule> "${PROJECT_SOURCE_DIR}/bin/plugins")
