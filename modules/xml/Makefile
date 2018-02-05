SQ_SRCDIR += $(SQ_MODDIR)/xml

SQ_TARGET_NAME = xml
SQ_TARGET_TITLE = XML

SQ_OUTPUT = $(SQ_OUTDIR)/$(SQ_TARGET_NAME_PREFIX)$(SQ_TARGET_NAME)$(SQ_ARCH).$(SQ_BEXT)
SQ_DEFINES += -DSQMOD_PLUGIN_API
SQ_INCLUDES += -I"$(SQ_EXTDIR)/PUGIXML" -I"$(SQ_SRCDIR)"

PP_OBJECTS= "$(SQ_OBJDIR)/external/PUGIXML/pugixml.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/modules/xml/Attribute.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/modules/xml/Common.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/modules/xml/Document.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/modules/xml/Handle/Document.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/modules/xml/Module.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/modules/xml/Node.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/modules/xml/Text.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/modules/xml/Wrapper/ParseResult.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/shared/Base/Buffer.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/shared/Base/Module.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/shared/SqMod.$(SQ_OEXT)"

.PHONY := default

build:
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unused-parameter -Wno-unused-function $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_EXTDIR)/PUGIXML/pugixml.cpp" -o "$(SQ_OBJDIR)/external/PUGIXML/pugixml.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Attribute.cpp" -o "$(SQ_OBJDIR)/modules/xml/Attribute.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Common.cpp" -o "$(SQ_OBJDIR)/modules/xml/Common.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Document.cpp" -o "$(SQ_OBJDIR)/modules/xml/Document.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Handle/Document.cpp" -o "$(SQ_OBJDIR)/modules/xml/Handle/Document.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Module.cpp" -o "$(SQ_OBJDIR)/modules/xml/Module.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Node.cpp" -o "$(SQ_OBJDIR)/modules/xml/Node.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Text.cpp" -o "$(SQ_OBJDIR)/modules/xml/Text.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Wrapper/ParseResult.cpp" -o "$(SQ_OBJDIR)/modules/xml/Wrapper/ParseResult.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SHRDIR)/Base/Buffer.cpp" -o "$(SQ_OBJDIR)/shared/Base/Buffer.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SHRDIR)/Base/Module.cpp" -o "$(SQ_OBJDIR)/shared/Base/Module.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SHRDIR)/SqMod.cpp" -o "$(SQ_OBJDIR)/shared/SqMod.$(SQ_OEXT)"

link:
	$(SQ_PP_LINKER_DYNAMIC) $(SQ_DYNAMIC_LINKER_OPTIONS) -L"$(SQ_LIBDIR)" $(PP_OBJECTS) -o "$(SQ_OUTPUT)" $(SQ_DYNAMIC_LINKER_OPTIONS_)

default: build link

clean:
	rm -rf "$(SQ_OUTPUT)"
	rm -rf $(PP_OBJECTS)
