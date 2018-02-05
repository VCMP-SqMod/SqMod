SQ_SRCDIR += $(SQ_MODDIR)/mysql

SQ_TARGET_NAME = mysql
SQ_TARGET_TITLE = MySQL

SQ_OUTPUT = $(SQ_OUTDIR)/$(SQ_TARGET_NAME_PREFIX)$(SQ_TARGET_NAME)$(SQ_ARCH).$(SQ_BEXT)
SQ_DEFINES += -DSQMOD_PLUGIN_API

ifeq ($(SQ_PLAT),win)
	SQ_INCLUDES += -I"$(SQ_INCDIR)/mysql" -I"$(SQ_CFGDIR)/mysql"
	SQ_DYNAMIC_LINKER_OPTIONS_ += -lmysql
else
	SQ_DYNAMIC_LINKER_OPTIONS_ += -lmysqlclient
endif

SQ_INCLUDES += -I"$(SQ_SRCDIR)"

PP_OBJECTS= "$(SQ_OBJDIR)/modules/mysql/Account.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/modules/mysql/Common.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/modules/mysql/Connection.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/modules/mysql/Convert.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/modules/mysql/Field.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/modules/mysql/Handle/Connection.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/modules/mysql/Handle/ResultSet.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/modules/mysql/Handle/Statement.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/modules/mysql/Module.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/modules/mysql/Parameter.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/modules/mysql/ResultSet.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/modules/mysql/Savepoint.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/modules/mysql/Statement.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/modules/mysql/Transaction.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/modules/mysql/Wrapper/CharsetInfo.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/shared/Base/Buffer.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/shared/Base/Module.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/shared/SqMod.$(SQ_OEXT)"

.PHONY := default

build:
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Account.cpp" -o "$(SQ_OBJDIR)/modules/mysql/Account.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Common.cpp" -o "$(SQ_OBJDIR)/modules/mysql/Common.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Connection.cpp" -o "$(SQ_OBJDIR)/modules/mysql/Connection.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Convert.cpp" -o "$(SQ_OBJDIR)/modules/mysql/Convert.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Field.cpp" -o "$(SQ_OBJDIR)/modules/mysql/Field.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Handle/Connection.cpp" -o "$(SQ_OBJDIR)/modules/mysql/Handle/Connection.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Handle/ResultSet.cpp" -o "$(SQ_OBJDIR)/modules/mysql/Handle/ResultSet.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Handle/Statement.cpp" -o "$(SQ_OBJDIR)/modules/mysql/Handle/Statement.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Module.cpp" -o "$(SQ_OBJDIR)/modules/mysql/Module.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Parameter.cpp" -o "$(SQ_OBJDIR)/modules/mysql/Parameter.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/ResultSet.cpp" -o "$(SQ_OBJDIR)/modules/mysql/ResultSet.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Savepoint.cpp" -o "$(SQ_OBJDIR)/modules/mysql/Savepoint.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Statement.cpp" -o "$(SQ_OBJDIR)/modules/mysql/Statement.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Transaction.cpp" -o "$(SQ_OBJDIR)/modules/mysql/Transaction.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Wrapper/CharsetInfo.cpp" -o "$(SQ_OBJDIR)/modules/mysql/Wrapper/CharsetInfo.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SHRDIR)/Base/Buffer.cpp" -o "$(SQ_OBJDIR)/shared/Base/Buffer.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SHRDIR)/Base/Module.cpp" -o "$(SQ_OBJDIR)/shared/Base/Module.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SHRDIR)/SqMod.cpp" -o "$(SQ_OBJDIR)/shared/SqMod.$(SQ_OEXT)"

link:
	$(SQ_PP_LINKER_DYNAMIC) $(SQ_DYNAMIC_LINKER_OPTIONS) -L"$(SQ_LIBDIR)" $(PP_OBJECTS) -o "$(SQ_OUTPUT)" $(SQ_DYNAMIC_LINKER_OPTIONS_)

default: build link

clean:
	rm -rf "$(SQ_OUTPUT)"
	rm -rf $(PP_OBJECTS)
