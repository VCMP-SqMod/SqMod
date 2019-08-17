SQ_SRCDIR += $(SQ_BASEDIR)/source

SQ_TARGET_NAME = squirrel
SQ_TARGET_TITLE = Squirrel

SQ_OUTPUT = $(SQ_OUTDIR)/$(SQ_TARGET_NAME_PREFIX)$(SQ_TARGET_NAME)$(SQ_ARCH).$(SQ_BEXT)

SQ_INCLUDES += -I"$(SQ_EXTDIR)/Hash" -I"$(SQ_EXTDIR)/Squirrel" -I"$(SQ_EXTDIR)/Squirrel/Lib" -I"$(SQ_SRCDIR)"

C_OBJECTS = "$(SQ_OBJDIR)/external/B64/decode.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/external/B64/encode.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/external/Common/byte_order.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/external/Common/whirlpool.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/external/Common/whirlpool_sbox.$(SQ_OEXT)"

PP_OBJECTS= "$(SQ_OBJDIR)/external/Squirrel/Lib/sqstdaux.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/external/Squirrel/Lib/sqstdblob.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/external/Squirrel/Lib/sqstdio.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/external/Squirrel/Lib/sqstdmath.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/external/Squirrel/Lib/sqstdrex.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/external/Squirrel/Lib/sqstdstream.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/external/Squirrel/Lib/sqstdstring.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/external/Squirrel/Lib/sqstdsystem.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/external/Squirrel/sqapi.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/external/Squirrel/sqapiex.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/external/Squirrel/sqbaselib.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/external/Squirrel/sqclass.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/external/Squirrel/sqcompiler.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/external/Squirrel/sqdebug.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/external/Squirrel/sqfuncstate.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/external/Squirrel/sqlexer.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/external/Squirrel/sqmem.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/external/Squirrel/sqobject.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/external/Squirrel/sqstate.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/external/Squirrel/sqtable.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/external/Squirrel/sqvm.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/external/Common/aes256.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/external/Hash/crc32.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/external/Hash/digest.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/external/Hash/keccak.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/external/Hash/md5.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/external/Hash/sha1.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/external/Hash/sha256.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/external/Hash/sha3.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/shared/Base/Buffer.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/shared/Base/Plugin.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/source/Base/AABB.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/source/Base/Algo.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/source/Base/Circle.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/source/Base/Color3.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/source/Base/Color4.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/source/Base/Quaternion.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/source/Base/ScriptSrc.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/source/Base/Shared.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/source/Base/Sphere.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/source/Base/Vector2.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/source/Base/Vector2i.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/source/Base/Vector3.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/source/Base/Vector4.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/source/Core.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/source/Entity/Blip.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/source/Entity/Checkpoint.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/source/Entity/Keybind.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/source/Entity/Object.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/source/Entity/Pickup.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/source/Entity/Player.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/source/Entity/Vehicle.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/source/Library/Chrono.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/source/Library/Chrono/Date.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/source/Library/Chrono/Datetime.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/source/Library/Chrono/Time.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/source/Library/Chrono/Timer.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/source/Library/Chrono/Timestamp.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/source/Library/Crypt.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/source/Library/Crypt/AES.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/source/Library/Crypt/Hash.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/source/Library/IO.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/source/Library/IO/File.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/source/Library/IO/INI.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/source/Library/Numeric.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/source/Library/Numeric/LongInt.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/source/Library/Numeric/Math.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/source/Library/Numeric/Random.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/source/Library/String.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/source/Library/System.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/source/Library/System/Dir.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/source/Library/System/Environment.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/source/Library/System/Path.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/source/Library/Utils.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/source/Library/Utils/Buffer.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/source/Logger.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/source/Main.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/source/Misc/Areas.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/source/Misc/Broadcast.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/source/Misc/Command.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/source/Misc/Constants.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/source/Misc/Exports.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/source/Misc/Functions.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/source/Misc/Model.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/source/Misc/Player.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/source/Misc/Register.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/source/Misc/Routine.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/source/Misc/Signal.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/source/Misc/Tasks.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/source/Misc/Vehicle.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/source/Misc/Weapon.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/source/Register.$(SQ_OEXT)"

.PHONY := default

build:
	$(SQ_C_COMPILER) $(SQ_C_OPTIONS) -Wno-format -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unused-parameter $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_EXTDIR)/B64/decode.c" -o "$(SQ_OBJDIR)/external/B64/decode.$(SQ_OEXT)"
	$(SQ_C_COMPILER) $(SQ_C_OPTIONS) -Wno-format -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unused-parameter $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_EXTDIR)/B64/encode.c" -o "$(SQ_OBJDIR)/external/B64/encode.$(SQ_OEXT)"
	$(SQ_C_COMPILER) $(SQ_C_OPTIONS) -Wno-format -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unused-parameter $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_EXTDIR)/Common/byte_order.c" -o "$(SQ_OBJDIR)/external/Common/byte_order.$(SQ_OEXT)"
	$(SQ_C_COMPILER) $(SQ_C_OPTIONS) -Wno-format -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unused-parameter $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_EXTDIR)/Common/whirlpool.c" -o "$(SQ_OBJDIR)/external/Common/whirlpool.$(SQ_OEXT)"
	$(SQ_C_COMPILER) $(SQ_C_OPTIONS) -Wno-format -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unused-parameter $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_EXTDIR)/Common/whirlpool_sbox.c" -o "$(SQ_OBJDIR)/external/Common/whirlpool_sbox.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) -Wno-format -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unused-parameter $(SQ_PP_OPTIONS) -fno-exceptions -fno-rtti -fno-strict-aliasing $(SQ_DEFINES) -DGARBAGE_COLLECTOR $(SQ_INCLUDES) -c "$(SQ_EXTDIR)/Squirrel/Lib/sqstdaux.cpp" -o "$(SQ_OBJDIR)/external/Squirrel/Lib/sqstdaux.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) -Wno-format -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unused-parameter $(SQ_PP_OPTIONS) -fno-exceptions -fno-rtti -fno-strict-aliasing $(SQ_DEFINES) -DGARBAGE_COLLECTOR $(SQ_INCLUDES) -c "$(SQ_EXTDIR)/Squirrel/Lib/sqstdblob.cpp" -o "$(SQ_OBJDIR)/external/Squirrel/Lib/sqstdblob.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) -Wno-format -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unused-parameter $(SQ_PP_OPTIONS) -fno-exceptions -fno-rtti -fno-strict-aliasing $(SQ_DEFINES) -DGARBAGE_COLLECTOR $(SQ_INCLUDES) -c "$(SQ_EXTDIR)/Squirrel/Lib/sqstdio.cpp" -o "$(SQ_OBJDIR)/external/Squirrel/Lib/sqstdio.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) -Wno-format -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unused-parameter $(SQ_PP_OPTIONS) -fno-exceptions -fno-rtti -fno-strict-aliasing $(SQ_DEFINES) -DGARBAGE_COLLECTOR $(SQ_INCLUDES) -c "$(SQ_EXTDIR)/Squirrel/Lib/sqstdmath.cpp" -o "$(SQ_OBJDIR)/external/Squirrel/Lib/sqstdmath.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) -Wno-format -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unused-parameter $(SQ_PP_OPTIONS) -fno-exceptions -fno-rtti -fno-strict-aliasing $(SQ_DEFINES) -DGARBAGE_COLLECTOR $(SQ_INCLUDES) -c "$(SQ_EXTDIR)/Squirrel/Lib/sqstdrex.cpp" -o "$(SQ_OBJDIR)/external/Squirrel/Lib/sqstdrex.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) -Wno-format -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unused-parameter $(SQ_PP_OPTIONS) -fno-exceptions -fno-rtti -fno-strict-aliasing $(SQ_DEFINES) -DGARBAGE_COLLECTOR $(SQ_INCLUDES) -c "$(SQ_EXTDIR)/Squirrel/Lib/sqstdstream.cpp" -o "$(SQ_OBJDIR)/external/Squirrel/Lib/sqstdstream.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) -Wno-format -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unused-parameter $(SQ_PP_OPTIONS) -fno-exceptions -fno-rtti -fno-strict-aliasing $(SQ_DEFINES) -DGARBAGE_COLLECTOR $(SQ_INCLUDES) -c "$(SQ_EXTDIR)/Squirrel/Lib/sqstdstring.cpp" -o "$(SQ_OBJDIR)/external/Squirrel/Lib/sqstdstring.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) -Wno-format -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unused-parameter $(SQ_PP_OPTIONS) -fno-exceptions -fno-rtti -fno-strict-aliasing $(SQ_DEFINES) -DGARBAGE_COLLECTOR $(SQ_INCLUDES) -c "$(SQ_EXTDIR)/Squirrel/Lib/sqstdsystem.cpp" -o "$(SQ_OBJDIR)/external/Squirrel/Lib/sqstdsystem.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) -Wno-format -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unused-parameter $(SQ_PP_OPTIONS) -fno-exceptions -fno-rtti -fno-strict-aliasing $(SQ_DEFINES) -DGARBAGE_COLLECTOR $(SQ_INCLUDES) -c "$(SQ_EXTDIR)/Squirrel/sqapi.cpp" -o "$(SQ_OBJDIR)/external/Squirrel/sqapi.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) -Wno-format -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unused-parameter $(SQ_PP_OPTIONS) -fno-exceptions -fno-rtti -fno-strict-aliasing $(SQ_DEFINES) -DGARBAGE_COLLECTOR $(SQ_INCLUDES) -c "$(SQ_EXTDIR)/Squirrel/sqapiex.cpp" -o "$(SQ_OBJDIR)/external/Squirrel/sqapiex.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) -Wno-format -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unused-parameter $(SQ_PP_OPTIONS) -fno-exceptions -fno-rtti -fno-strict-aliasing $(SQ_DEFINES) -DGARBAGE_COLLECTOR $(SQ_INCLUDES) -c "$(SQ_EXTDIR)/Squirrel/sqbaselib.cpp" -o "$(SQ_OBJDIR)/external/Squirrel/sqbaselib.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) -Wno-format -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unused-parameter $(SQ_PP_OPTIONS) -fno-exceptions -fno-rtti -fno-strict-aliasing $(SQ_DEFINES) -DGARBAGE_COLLECTOR $(SQ_INCLUDES) -c "$(SQ_EXTDIR)/Squirrel/sqclass.cpp" -o "$(SQ_OBJDIR)/external/Squirrel/sqclass.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) -Wno-format -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unused-parameter $(SQ_PP_OPTIONS) -fno-exceptions -fno-rtti -fno-strict-aliasing $(SQ_DEFINES) -DGARBAGE_COLLECTOR $(SQ_INCLUDES) -c "$(SQ_EXTDIR)/Squirrel/sqcompiler.cpp" -o "$(SQ_OBJDIR)/external/Squirrel/sqcompiler.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) -Wno-format -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unused-parameter $(SQ_PP_OPTIONS) -fno-exceptions -fno-rtti -fno-strict-aliasing $(SQ_DEFINES) -DGARBAGE_COLLECTOR $(SQ_INCLUDES) -c "$(SQ_EXTDIR)/Squirrel/sqdebug.cpp" -o "$(SQ_OBJDIR)/external/Squirrel/sqdebug.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) -Wno-format -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unused-parameter $(SQ_PP_OPTIONS) -fno-exceptions -fno-rtti -fno-strict-aliasing $(SQ_DEFINES) -DGARBAGE_COLLECTOR $(SQ_INCLUDES) -c "$(SQ_EXTDIR)/Squirrel/sqfuncstate.cpp" -o "$(SQ_OBJDIR)/external/Squirrel/sqfuncstate.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) -Wno-format -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unused-parameter $(SQ_PP_OPTIONS) -fno-exceptions -fno-rtti -fno-strict-aliasing $(SQ_DEFINES) -DGARBAGE_COLLECTOR $(SQ_INCLUDES) -c "$(SQ_EXTDIR)/Squirrel/sqlexer.cpp" -o "$(SQ_OBJDIR)/external/Squirrel/sqlexer.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) -Wno-format -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unused-parameter $(SQ_PP_OPTIONS) -fno-exceptions -fno-rtti -fno-strict-aliasing $(SQ_DEFINES) -DGARBAGE_COLLECTOR $(SQ_INCLUDES) -c "$(SQ_EXTDIR)/Squirrel/sqmem.cpp" -o "$(SQ_OBJDIR)/external/Squirrel/sqmem.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) -Wno-format -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unused-parameter $(SQ_PP_OPTIONS) -fno-exceptions -fno-rtti -fno-strict-aliasing $(SQ_DEFINES) -DGARBAGE_COLLECTOR $(SQ_INCLUDES) -c "$(SQ_EXTDIR)/Squirrel/sqobject.cpp" -o "$(SQ_OBJDIR)/external/Squirrel/sqobject.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) -Wno-format -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unused-parameter $(SQ_PP_OPTIONS) -fno-exceptions -fno-rtti -fno-strict-aliasing $(SQ_DEFINES) -DGARBAGE_COLLECTOR $(SQ_INCLUDES) -c "$(SQ_EXTDIR)/Squirrel/sqstate.cpp" -o "$(SQ_OBJDIR)/external/Squirrel/sqstate.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) -Wno-format -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unused-parameter $(SQ_PP_OPTIONS) -fno-exceptions -fno-rtti -fno-strict-aliasing $(SQ_DEFINES) -DGARBAGE_COLLECTOR $(SQ_INCLUDES) -c "$(SQ_EXTDIR)/Squirrel/sqtable.cpp" -o "$(SQ_OBJDIR)/external/Squirrel/sqtable.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) -Wno-format -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unused-parameter $(SQ_PP_OPTIONS) -fno-exceptions -fno-rtti -fno-strict-aliasing $(SQ_DEFINES) -DGARBAGE_COLLECTOR $(SQ_INCLUDES) -c "$(SQ_EXTDIR)/Squirrel/sqvm.cpp" -o "$(SQ_OBJDIR)/external/Squirrel/sqvm.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) -Wno-format -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unused-parameter $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_EXTDIR)/Common/aes256.cpp" -o "$(SQ_OBJDIR)/external/Common/aes256.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) -Wno-format -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unused-parameter $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_EXTDIR)/Hash/crc32.cpp" -o "$(SQ_OBJDIR)/external/Hash/crc32.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) -Wno-format -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unused-parameter $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_EXTDIR)/Hash/digest.cpp" -o "$(SQ_OBJDIR)/external/Hash/digest.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) -Wno-format -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unused-parameter $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_EXTDIR)/Hash/keccak.cpp" -o "$(SQ_OBJDIR)/external/Hash/keccak.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) -Wno-format -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unused-parameter $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_EXTDIR)/Hash/md5.cpp" -o "$(SQ_OBJDIR)/external/Hash/md5.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) -Wno-format -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unused-parameter $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_EXTDIR)/Hash/sha1.cpp" -o "$(SQ_OBJDIR)/external/Hash/sha1.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) -Wno-format -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unused-parameter $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_EXTDIR)/Hash/sha256.cpp" -o "$(SQ_OBJDIR)/external/Hash/sha256.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) -Wno-format -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unused-parameter $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_EXTDIR)/Hash/sha3.cpp" -o "$(SQ_OBJDIR)/external/Hash/sha3.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SHRDIR)/Base/Buffer.cpp" -o "$(SQ_OBJDIR)/shared/Base/Buffer.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SHRDIR)/Base/Plugin.cpp" -o "$(SQ_OBJDIR)/shared/Base/Plugin.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Base/AABB.cpp" -o "$(SQ_OBJDIR)/source/Base/AABB.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Base/Algo.cpp" -o "$(SQ_OBJDIR)/source/Base/Algo.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Base/Circle.cpp" -o "$(SQ_OBJDIR)/source/Base/Circle.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Base/Color3.cpp" -o "$(SQ_OBJDIR)/source/Base/Color3.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Base/Color4.cpp" -o "$(SQ_OBJDIR)/source/Base/Color4.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Base/Quaternion.cpp" -o "$(SQ_OBJDIR)/source/Base/Quaternion.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Base/ScriptSrc.cpp" -o "$(SQ_OBJDIR)/source/Base/ScriptSrc.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Base/Shared.cpp" -o "$(SQ_OBJDIR)/source/Base/Shared.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Base/Sphere.cpp" -o "$(SQ_OBJDIR)/source/Base/Sphere.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Base/Vector2.cpp" -o "$(SQ_OBJDIR)/source/Base/Vector2.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Base/Vector2i.cpp" -o "$(SQ_OBJDIR)/source/Base/Vector2i.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Base/Vector3.cpp" -o "$(SQ_OBJDIR)/source/Base/Vector3.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Base/Vector4.cpp" -o "$(SQ_OBJDIR)/source/Base/Vector4.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Core.cpp" -o "$(SQ_OBJDIR)/source/Core.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Entity/Blip.cpp" -o "$(SQ_OBJDIR)/source/Entity/Blip.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Entity/Checkpoint.cpp" -o "$(SQ_OBJDIR)/source/Entity/Checkpoint.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Entity/Keybind.cpp" -o "$(SQ_OBJDIR)/source/Entity/Keybind.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Entity/Object.cpp" -o "$(SQ_OBJDIR)/source/Entity/Object.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Entity/Pickup.cpp" -o "$(SQ_OBJDIR)/source/Entity/Pickup.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Entity/Player.cpp" -o "$(SQ_OBJDIR)/source/Entity/Player.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Entity/Vehicle.cpp" -o "$(SQ_OBJDIR)/source/Entity/Vehicle.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Library/Chrono.cpp" -o "$(SQ_OBJDIR)/source/Library/Chrono.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Library/Chrono/Date.cpp" -o "$(SQ_OBJDIR)/source/Library/Chrono/Date.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Library/Chrono/Datetime.cpp" -o "$(SQ_OBJDIR)/source/Library/Chrono/Datetime.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Library/Chrono/Time.cpp" -o "$(SQ_OBJDIR)/source/Library/Chrono/Time.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Library/Chrono/Timer.cpp" -o "$(SQ_OBJDIR)/source/Library/Chrono/Timer.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Library/Chrono/Timestamp.cpp" -o "$(SQ_OBJDIR)/source/Library/Chrono/Timestamp.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Library/Crypt.cpp" -o "$(SQ_OBJDIR)/source/Library/Crypt.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Library/Crypt/AES.cpp" -o "$(SQ_OBJDIR)/source/Library/Crypt/AES.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Library/Crypt/Hash.cpp" -o "$(SQ_OBJDIR)/source/Library/Crypt/Hash.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Library/IO.cpp" -o "$(SQ_OBJDIR)/source/Library/IO.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Library/IO/File.cpp" -o "$(SQ_OBJDIR)/source/Library/IO/File.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Library/IO/INI.cpp" -o "$(SQ_OBJDIR)/source/Library/IO/INI.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Library/Numeric.cpp" -o "$(SQ_OBJDIR)/source/Library/Numeric.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Library/Numeric/LongInt.cpp" -o "$(SQ_OBJDIR)/source/Library/Numeric/LongInt.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Library/Numeric/Math.cpp" -o "$(SQ_OBJDIR)/source/Library/Numeric/Math.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Library/Numeric/Random.cpp" -o "$(SQ_OBJDIR)/source/Library/Numeric/Random.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Library/String.cpp" -o "$(SQ_OBJDIR)/source/Library/String.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Library/System.cpp" -o "$(SQ_OBJDIR)/source/Library/System.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Library/System/Dir.cpp" -o "$(SQ_OBJDIR)/source/Library/System/Dir.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Library/System/Environment.cpp" -o "$(SQ_OBJDIR)/source/Library/System/Environment.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Library/System/Path.cpp" -o "$(SQ_OBJDIR)/source/Library/System/Path.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Library/Utils.cpp" -o "$(SQ_OBJDIR)/source/Library/Utils.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Library/Utils/Buffer.cpp" -o "$(SQ_OBJDIR)/source/Library/Utils/Buffer.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Logger.cpp" -o "$(SQ_OBJDIR)/source/Logger.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Main.cpp" -o "$(SQ_OBJDIR)/source/Main.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Misc/Areas.cpp" -o "$(SQ_OBJDIR)/source/Misc/Areas.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Misc/Broadcast.cpp" -o "$(SQ_OBJDIR)/source/Misc/Broadcast.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Misc/Command.cpp" -o "$(SQ_OBJDIR)/source/Misc/Command.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Misc/Constants.cpp" -o "$(SQ_OBJDIR)/source/Misc/Constants.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Misc/Exports.cpp" -o "$(SQ_OBJDIR)/source/Misc/Exports.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Misc/Functions.cpp" -o "$(SQ_OBJDIR)/source/Misc/Functions.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Misc/Model.cpp" -o "$(SQ_OBJDIR)/source/Misc/Model.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Misc/Player.cpp" -o "$(SQ_OBJDIR)/source/Misc/Player.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Misc/Register.cpp" -o "$(SQ_OBJDIR)/source/Misc/Register.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Misc/Routine.cpp" -o "$(SQ_OBJDIR)/source/Misc/Routine.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Misc/Signal.cpp" -o "$(SQ_OBJDIR)/source/Misc/Signal.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Misc/Tasks.cpp" -o "$(SQ_OBJDIR)/source/Misc/Tasks.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Misc/Vehicle.cpp" -o "$(SQ_OBJDIR)/source/Misc/Vehicle.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Misc/Weapon.cpp" -o "$(SQ_OBJDIR)/source/Misc/Weapon.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Register.cpp" -o "$(SQ_OBJDIR)/source/Register.$(SQ_OEXT)"

link:
	$(SQ_PP_LINKER_DYNAMIC) $(SQ_DYNAMIC_LINKER_OPTIONS) -L"$(SQ_LIBDIR)" $(C_OBJECTS) $(PP_OBJECTS) -o "$(SQ_OUTPUT)" $(SQ_DYNAMIC_LINKER_OPTIONS_)

default: build link

clean:
	rm -rf "$(SQ_OUTPUT)"
	rm -rf $(C_OBJECTS) $(PP_OBJECTS)
