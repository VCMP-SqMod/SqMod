SQ_SRCDIR += $(SQ_MODDIR)/irc

SQ_TARGET_NAME = irc
SQ_TARGET_TITLE = IRC

SQ_OUTPUT = $(SQ_OUTDIR)/$(SQ_TARGET_NAME_PREFIX)$(SQ_TARGET_NAME)$(SQ_ARCH).$(SQ_BEXT)
SQ_DEFINES += -DSQMOD_PLUGIN_API
SQ_INCLUDES += -I"$(SQ_EXTDIR)/IRC" -I"$(SQ_SRCDIR)"

ifeq ($(SQ_PLAT),win)
	SQ_DEFINES +=  -DWIN32
	SQ_DYNAMIC_LINKER_OPTIONS_ += -lWs2_32
endif

C_OBJECTS= "$(SQ_OBJDIR)/external/LibIRC/libircclient.$(SQ_OEXT)"

PP_OBJECTS= "$(SQ_OBJDIR)/modules/irc/Common.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/modules/irc/Constants.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/modules/irc/Module.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/modules/irc/Session.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/shared/Base/Buffer.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/shared/Base/Module.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/shared/SqMod.$(SQ_OEXT)"

.PHONY := default

build:
	$(SQ_C_COMPILER) $(SQ_C_OPTIONS) -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unused-parameter -Wno-sign-compare -Wno-type-limits -Wno-misleading-indentation -Wno-nonnull $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_EXTDIR)/LibIRC/libircclient.c" -o "$(SQ_OBJDIR)/external/LibIRC/libircclient.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Common.cpp" -o "$(SQ_OBJDIR)/modules/irc/Common.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Constants.cpp" -o "$(SQ_OBJDIR)/modules/irc/Constants.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Module.cpp" -o "$(SQ_OBJDIR)/modules/irc/Module.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Session.cpp" -o "$(SQ_OBJDIR)/modules/irc/Session.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SHRDIR)/Base/Buffer.cpp" -o "$(SQ_OBJDIR)/shared/Base/Buffer.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SHRDIR)/Base/Module.cpp" -o "$(SQ_OBJDIR)/shared/Base/Module.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SHRDIR)/SqMod.cpp" -o "$(SQ_OBJDIR)/shared/SqMod.$(SQ_OEXT)"

link:
	$(SQ_PP_LINKER_DYNAMIC) $(SQ_DYNAMIC_LINKER_OPTIONS) -L"$(SQ_LIBDIR)" $(C_OBJECTS) $(PP_OBJECTS) -o "$(SQ_OUTPUT)" $(SQ_DYNAMIC_LINKER_OPTIONS_)

default: build link

clean:
	rm -rf "$(SQ_OUTPUT)"
	rm -rf $(C_OBJECTS) $(PP_OBJECTS)
