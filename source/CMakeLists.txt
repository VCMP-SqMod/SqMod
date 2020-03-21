# Create the Squirrel module
add_library(SquirrelModule MODULE
	SqBase.hpp
	Main.cpp
	Register.cpp
	Core.cpp Core.hpp
	Logger.cpp Logger.hpp
	Base/AABB.cpp Base/AABB.hpp
	Base/Algo.cpp Base/Algo.hpp
	Base/Circle.cpp Base/Circle.hpp
	Base/Color3.cpp Base/Color3.hpp
	Base/Color4.cpp Base/Color4.hpp
	Base/Quaternion.cpp Base/Quaternion.hpp
	Base/ScriptSrc.cpp Base/ScriptSrc.hpp
	Base/Shared.cpp Base/Shared.hpp
	Base/Sphere.cpp Base/Sphere.hpp
	Base/Vector2.cpp Base/Vector2.hpp
	Base/Vector2i.cpp Base/Vector2i.hpp
	Base/Vector3.cpp Base/Vector3.hpp
	Base/Vector4.cpp Base/Vector4.hpp
	Entity/Blip.cpp Entity/Blip.hpp
	Entity/Checkpoint.cpp Entity/Checkpoint.hpp
	Entity/Keybind.cpp Entity/Keybind.hpp
	Entity/Object.cpp Entity/Object.hpp
	Entity/Pickup.cpp Entity/Pickup.hpp
	Entity/Player.cpp Entity/Player.hpp
	Entity/Vehicle.cpp Entity/Vehicle.hpp
	Library/Chrono.cpp Library/Chrono.hpp
	Library/Chrono/Date.cpp Library/Chrono/Date.hpp
	Library/Chrono/Datetime.cpp Library/Chrono/Datetime.hpp
	Library/Chrono/Time.cpp Library/Chrono/Time.hpp
	Library/Chrono/Timer.cpp Library/Chrono/Timer.hpp
	Library/Chrono/Timestamp.cpp Library/Chrono/Timestamp.hpp
	Library/Crypt.cpp Library/Crypt.hpp
	Library/Crypt/AES.cpp Library/Crypt/AES.hpp
	Library/Crypt/Hash.cpp Library/Crypt/Hash.hpp
	Library/IO.cpp Library/IO.hpp
	Library/IO/File.cpp Library/IO/File.hpp
	Library/IO/INI.cpp Library/IO/INI.hpp
	Library/Numeric.cpp Library/Numeric.hpp
	Library/Numeric/LongInt.cpp Library/Numeric/LongInt.hpp
	Library/Numeric/Math.cpp Library/Numeric/Math.hpp
	Library/Numeric/Random.cpp Library/Numeric/Random.hpp
	Library/String.cpp Library/String.hpp
	Library/System.cpp Library/System.hpp
	Library/System/Dir.cpp Library/System/Dir.hpp
	Library/System/Environment.cpp Library/System/Environment.hpp
	Library/System/Path.cpp Library/System/Path.hpp
	Library/Utils.cpp Library/Utils.hpp
	Library/Utils/Buffer.cpp Library/Utils/Buffer.hpp
	Misc/Broadcast.cpp
	Misc/Constants.cpp
	Misc/Exports.cpp
	Misc/Register.cpp
	Misc/Areas.cpp Misc/Areas.hpp
	Misc/Command.cpp Misc/Command.hpp
	Misc/Functions.cpp Misc/Functions.hpp
	Misc/Model.cpp Misc/Model.hpp
	Misc/Player.cpp Misc/Player.hpp
	Misc/Routine.cpp Misc/Routine.hpp
	Misc/Signal.cpp Misc/Signal.hpp
	Misc/Tasks.cpp Misc/Tasks.hpp
	Misc/Vehicle.cpp Misc/Vehicle.hpp
	Misc/Weapon.cpp Misc/Weapon.hpp
)
#
if(FORCE_32BIT_BIN)
	set_target_properties(SquirrelModule PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")
endif()

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	target_compile_definitions(SquirrelModule PRIVATE _SQ64)
endif()

set_target_properties(SquirrelModule PROPERTIES PREFIX "")

if(WIN32)
	if(CMAKE_SIZEOF_VOID_P EQUAL 8 AND NOT FORCE_32BIT_BIN)
		set_target_properties(SquirrelModule PROPERTIES OUTPUT_NAME "mod_squirrel_64")
	else()
		set_target_properties(SquirrelModule PROPERTIES OUTPUT_NAME "mod_squirrel_32")
	endif()
else()
	if(CMAKE_SIZEOF_VOID_P EQUAL 8 AND NOT FORCE_32BIT_BIN)
		set_target_properties(SquirrelModule PROPERTIES OUTPUT_NAME "mod_squirrel_64")
	else()
		set_target_properties(SquirrelModule PROPERTIES OUTPUT_NAME "mod_squirrel_32")
	endif()
endif(32)

target_include_directories(SquirrelModule PRIVATE ${CMAKE_CURRENT_LIST_DIR})

if(WIN32)
  target_link_libraries(SquirrelModule wsock32 ws2_32)
endif()
