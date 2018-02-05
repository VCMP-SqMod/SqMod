SQ_SRCDIR += $(SQ_MODDIR)/sqlite

SQ_TARGET_NAME = sqlite
SQ_TARGET_TITLE = SQLite

SQ_OUTPUT = $(SQ_OUTDIR)/$(SQ_TARGET_NAME_PREFIX)$(SQ_TARGET_NAME)$(SQ_ARCH).$(SQ_BEXT)
SQ_DEFINES += -DSQMOD_PLUGIN_API -DSQMOD_PLUGIN_API -DSCRAT_USE_EXCEPTIONS -DSCRAT_USE_CXX11_OPTIMIZATIONS -DSQLITE_ENABLE_FTS3\=1 -DSQLITE_ENABLE_FTS4\=1 -DSQLITE_ENABLE_FTS5\=1 -DSQLITE_ENABLE_JSON1\=1 -DSQLITE_ENABLE_RTREE\=1
SQ_INCLUDES += -I"$(SQ_EXTDIR)/SQLite" -I"$(SQ_SRCDIR)"

C_OBJECTS= "$(SQ_OBJDIR)/external/SQLite/sqlite3.$(SQ_OEXT)"

PP_OBJECTS= "$(SQ_OBJDIR)/modules/sqlite/Column.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/modules/sqlite/Common.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/modules/sqlite/Connection.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/modules/sqlite/Constants.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/modules/sqlite/Handle/Connection.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/modules/sqlite/Handle/Statement.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/modules/sqlite/Module.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/modules/sqlite/Parameter.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/modules/sqlite/Statement.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/modules/sqlite/Transaction.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/shared/Base/Buffer.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/shared/Base/Module.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/shared/SqMod.$(SQ_OEXT)"

.PHONY := default

build:
	$(SQ_C_COMPILER) $(SQ_C_OPTIONS) -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unused-parameter $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_EXTDIR)/SQLite/sqlite3.c" -o "$(SQ_OBJDIR)/external/SQLite/sqlite3.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Column.cpp" -o "$(SQ_OBJDIR)/modules/sqlite/Column.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Common.cpp" -o "$(SQ_OBJDIR)/modules/sqlite/Common.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Connection.cpp" -o "$(SQ_OBJDIR)/modules/sqlite/Connection.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Constants.cpp" -o "$(SQ_OBJDIR)/modules/sqlite/Constants.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Handle/Connection.cpp" -o "$(SQ_OBJDIR)/modules/sqlite/Handle/Connection.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Handle/Statement.cpp" -o "$(SQ_OBJDIR)/modules/sqlite/Handle/Statement.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Module.cpp" -o "$(SQ_OBJDIR)/modules/sqlite/Module.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Parameter.cpp" -o "$(SQ_OBJDIR)/modules/sqlite/Parameter.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Statement.cpp" -o "$(SQ_OBJDIR)/modules/sqlite/Statement.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Transaction.cpp" -o "$(SQ_OBJDIR)/modules/sqlite/Transaction.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SHRDIR)/Base/Buffer.cpp" -o "$(SQ_OBJDIR)/shared/Base/Buffer.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SHRDIR)/Base/Module.cpp" -o "$(SQ_OBJDIR)/shared/Base/Module.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SHRDIR)/SqMod.cpp" -o "$(SQ_OBJDIR)/shared/SqMod.$(SQ_OEXT)"

link:
	$(SQ_PP_LINKER_DYNAMIC) $(SQ_DYNAMIC_LINKER_OPTIONS) -L"$(SQ_LIBDIR)" $(C_OBJECTS) $(PP_OBJECTS) -o "$(SQ_OUTPUT)" $(SQ_DYNAMIC_LINKER_OPTIONS_)

default: build link

clean:
	rm -rf "$(SQ_OUTPUT)"
	rm -rf $(C_OBJECTS) $(PP_OBJECTS)
