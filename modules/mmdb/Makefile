SQ_SRCDIR += $(SQ_MODDIR)/mmdb

SQ_TARGET_NAME = mmdb
SQ_TARGET_TITLE = MaxmindDB

SQ_OUTPUT = $(SQ_OUTDIR)/$(SQ_TARGET_NAME_PREFIX)$(SQ_TARGET_NAME)$(SQ_ARCH).$(SQ_BEXT)
SQ_DEFINES += -DSQMOD_PLUGIN_API
SQ_INCLUDES += -I"$(SQ_EXTDIR)/MaxmindDB" -I"$(SQ_SRCDIR)"

ifeq ($(SQ_PLAT),win)
	SQ_DYNAMIC_LINKER_OPTIONS_ += -lWs2_32
endif

C_OBJECTS= "$(SQ_OBJDIR)/external/MaxmindDB/maxminddb.$(SQ_OEXT)"

PP_OBJECTS= "$(SQ_OBJDIR)/modules/mmdb/Common.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/modules/mmdb/Database.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/modules/mmdb/Description.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/modules/mmdb/EntryData.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/modules/mmdb/EntryDataList.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/modules/mmdb/Handle/Database.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/modules/mmdb/LookupResult.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/modules/mmdb/Metadata.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/modules/mmdb/Module.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/modules/mmdb/SearchNode.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/modules/mmdb/SockAddr.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/shared/Base/Buffer.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/shared/Base/Module.$(SQ_OEXT)" \
			"$(SQ_OBJDIR)/shared/SqMod.$(SQ_OEXT)"

.PHONY := default

build:
	$(SQ_C_COMPILER) $(SQ_C_OPTIONS) -std=c99 -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unused-parameter $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_EXTDIR)/MaxmindDB/maxminddb.c" -o "$(SQ_OBJDIR)/external/MaxmindDB/maxminddb.$(SQ_OEXT)"
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Common.cpp" -o "$(SQ_OBJDIR)/modules/mmdb/Common.$(SQ_OEXT)"  
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Database.cpp" -o "$(SQ_OBJDIR)/modules/mmdb/Database.$(SQ_OEXT)"  
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Description.cpp" -o "$(SQ_OBJDIR)/modules/mmdb/Description.$(SQ_OEXT)"  
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/EntryData.cpp" -o "$(SQ_OBJDIR)/modules/mmdb/EntryData.$(SQ_OEXT)"  
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/EntryDataList.cpp" -o "$(SQ_OBJDIR)/modules/mmdb/EntryDataList.$(SQ_OEXT)"  
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Handle/Database.cpp" -o "$(SQ_OBJDIR)/modules/mmdb/Handle/Database.$(SQ_OEXT)"  
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/LookupResult.cpp" -o "$(SQ_OBJDIR)/modules/mmdb/LookupResult.$(SQ_OEXT)"  
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Metadata.cpp" -o "$(SQ_OBJDIR)/modules/mmdb/Metadata.$(SQ_OEXT)"  
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/Module.cpp" -o "$(SQ_OBJDIR)/modules/mmdb/Module.$(SQ_OEXT)"  
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/SearchNode.cpp" -o "$(SQ_OBJDIR)/modules/mmdb/SearchNode.$(SQ_OEXT)"  
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SRCDIR)/SockAddr.cpp" -o "$(SQ_OBJDIR)/modules/mmdb/SockAddr.$(SQ_OEXT)"  
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SHRDIR)/Base/Buffer.cpp" -o "$(SQ_OBJDIR)/shared/Base/Buffer.$(SQ_OEXT)"  
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SHRDIR)/Base/Module.cpp" -o "$(SQ_OBJDIR)/shared/Base/Module.$(SQ_OEXT)"  
	$(SQ_PP_COMPILER) $(SQ_C_OPTIONS) $(SQ_PP_OPTIONS) $(SQ_DEFINES) $(SQ_INCLUDES) -c "$(SQ_SHRDIR)/SqMod.cpp" -o "$(SQ_OBJDIR)/shared/SqMod.$(SQ_OEXT)"  


link:
	$(SQ_PP_LINKER_DYNAMIC) $(SQ_DYNAMIC_LINKER_OPTIONS) -L"$(SQ_LIBDIR)" $(C_OBJECTS) $(PP_OBJECTS) -o "$(SQ_OUTPUT)" $(SQ_DYNAMIC_LINKER_OPTIONS_)

default: build link

clean:
	rm -rf "$(SQ_OUTPUT)"
	rm -rf $(C_OBJECTS) $(PP_OBJECTS)
