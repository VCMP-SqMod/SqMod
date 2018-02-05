SQ_SRCDIR += $(SQ_MODDIR)/sample

SQ_TARGET_NAME = sample
SQ_TARGET_TITLE = Sample

SQ_OUTPUT = $(SQ_OUTDIR)/$(SQ_TARGET_NAME_PREFIX)$(SQ_TARGET_NAME)$(SQ_ARCH).$(SQ_BEXT)
SQ_DEFINES += -DSQMOD_PLUGIN_API
SQ_INCLUDES += -I"$(SQ_SRCDIR)"

PP_OBJECTS= "$(SQ_OBJDIR)/modules/sample/Common.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/modules/sample/Module.$(SQ_OEXT)"

.PHONY := default

build:
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Common.cpp" -o "$(SQ_OBJDIR)//modules/sample/Common.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Module.cpp" -o "$(SQ_OBJDIR)//modules/sample/Module.$(SQ_OEXT)"

link:
	$(SQ_PP_LINKER_DYNAMIC) $(SQ_DYNAMIC_LINKER_OPTIONS) -L"$(SQ_LIBDIR)" $(PP_OBJECTS) -o "$(SQ_OUTPUT)" $(SQ_DYNAMIC_LINKER_OPTIONS_)

default: build link

clean:
	rm -rf "$(SQ_OUTPUT)"
	rm -rf $(PP_OBJECTS)
