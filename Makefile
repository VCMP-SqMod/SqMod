export SQ_BASEDIR=$(CURDIR)
export SQ_MAKE = make
export SQ_C_COMPILER = gcc
export SQ_PP_COMPILER = g++
export SQ_C_LINKER_STATIC = ar
export SQ_PP_LINKER_STATIC = ar
export SQ_C_LINKER_DYNAMIC = gcc
export SQ_PP_LINKER_DYNAMIC = g++
export SQ_TARGET_NAME_PREFIX
export SQ_C_OPTIONS = -Wall -Wextra -Wno-error -Wno-implicit-fallthrough
export SQ_PP_OPTIONS = -std=c++14
export SQ_STATIC_LINKER_OPTIONS = -rc -s
export SQ_DYNAMIC_LINKER_OPTIONS = -shared
export SQ_DYNAMIC_LINKER_OPTIONS_
export SQ_DEFINES
export SQ_INCLUDES
export SQ_LIBRARIES
export SQ_MAKE_THREADS = 1
export SQ_DEBUG_POSTFIX

SQ_MAKE += -j$(SQ_MAKE_THREADS)

ifndef SQ_TARGET_NAME_PREFIX
	SQ_TARGET_NAME_PREFIX = mod_
endif

export SQ_PLAT
export SQ_ARCH
export SQ_COMP
export SQ_BEXT
export SQ_OEXT = o
export SQ_LEXT = a

ifeq ($(OS),Windows_NT)
	SQ_PLAT = win
	SQ_COMP = mingw
	SQ_BEXT = dll

	ifeq ($(PROCESSOR_ARCHITECTURE),AMD64)
		SQ_ARCH = 64
	else
		SQ_ARCH = 32
	endif
else
	SQ_PLAT = linux
	SQ_COMP = gcc
	SQ_BEXT = so

	UNAME_P := $(shell uname -p)
	ifeq ($(UNAME_P),x86_64)
		SQ_ARCH = 64
	else
		SQ_ARCH = 32
	endif
	UNAME_M := $(shell uname -m)
	ifeq ($(UNAME_M),x86_64)
		SQ_ARCH = 64
	else
		SQ_ARCH = 32
	endif
endif

ifdef ARCH
	SQ_ARCH = $(ARCH)
endif

ifdef DEBUG
	SQ_DEBUG_POSTFIX = -d
endif

export SQ_INCDIR=$(SQ_BASEDIR)/include
export SQ_SHRDIR=$(SQ_BASEDIR)/shared
export SQ_MODDIR=$(SQ_BASEDIR)/modules
export SQ_EXTDIR=$(SQ_BASEDIR)/external
export SQ_BASE_CFGDIR=$(SQ_BASEDIR)/config
export SQ_BASE_LIBDIR=$(SQ_BASEDIR)/lib
export SQ_BASE_OBJDIR=$(SQ_BASEDIR)/obj
export SQ_BASE_BINDIR=$(SQ_BASEDIR)/bin
export SQ_CFGDIR=$(SQ_BASE_CFGDIR)/$(SQ_COMP)$(SQ_ARCH)
export SQ_LIBDIR=$(SQ_BASE_LIBDIR)/$(SQ_COMP)$(SQ_ARCH)
export SQ_OBJDIR=$(SQ_BASE_OBJDIR)/$(SQ_COMP)$(SQ_ARCH)
export SQ_BINDIR=$(SQ_BASE_BINDIR)/$(SQ_PLAT)$(SQ_ARCH)$(SQ_DEBUG_POSTFIX)
export SQ_OUTDIR=

ifdef STANDALONE
	SQ_OUTDIR = $(SQ_BINDIR)/standalone
	SQ_C_OPTIONS += -static-libgcc -static-libstdc++ -enable-static
else
	SQ_OUTDIR = $(SQ_BINDIR)
endif

SQ_INCLUDES += -I"$(SQ_INCDIR)" -I"$(SQ_EXTDIR)/Common" -I"$(SQ_SHRDIR)" -I"$(SQ_BASE_CFGDIR)/common" -I"$(SQ_CFGDIR)"

ifndef DEBUG
	SQ_DEFINES += -DNDEBUG
	SQ_C_OPTIONS += -O3
	SQ_DYNAMIC_LINKER_OPTIONS_ += -s
else
	SQ_DEFINES += -D_DEBUG
	SQ_C_OPTIONS += -g
endif

ifeq ($(SQ_PLAT),win)
	SQ_DYNAMIC_LINKER_OPTIONS +=  -Wl,--dll
else
	SQ_DEFINES += -DLINUX
	SQ_C_OPTIONS += -fPIC
	SQ_DYNAMIC_LINKER_OPTIONS += -Wl,-Bsymbolic
endif

ifeq ($(SQ_ARCH),64)
	SQ_DEFINES += -D_SQ64
	SQ_C_OPTIONS += -m64
	SQ_DYNAMIC_LINKER_OPTIONS_ += -m64
else
	SQ_C_OPTIONS += -m32
	SQ_DYNAMIC_LINKER_OPTIONS_ += -m32
endif

ifdef STANDALONE
	ifeq ($(SQ_PLAT),win)
		SQ_DYNAMIC_LINKER_OPTIONS_ += -static
	else
		SQ_DYNAMIC_LINKER_OPTIONS_ += -Bstatic
	endif
endif

SQ_DEFINES += -DSCRAT_USE_EXCEPTION -DSCRAT_USE_CXX11_OPTIMIZATIONS

.PHONY := all

export SQ_TARGET

ifdef BUILD
	SQ_TARGET += build
endif
ifdef LINK
	SQ_TARGET += link
endif
ifdef DEFAULT
	SQ_TARGET = default
endif
ifdef CLEAN
	SQ_TARGET += clean
endif

all: folders mod_squirrel mod_sqlite mod_xml mod_mmdb mod_irc mod_mysql

mod_squirrel:
	cd $(SQ_BASEDIR)/source; $(SQ_MAKE) $(SQ_TARGET)

mod_sqlite:
	cd $(SQ_MODDIR)/sqlite; $(SQ_MAKE) $(SQ_TARGET)

mod_xml:
	cd $(SQ_MODDIR)/xml; $(SQ_MAKE) $(SQ_TARGET)

mod_mmdb:
	cd $(SQ_MODDIR)/mmdb; $(SQ_MAKE) $(SQ_TARGET)

mod_irc:
	cd $(SQ_MODDIR)/irc; $(SQ_MAKE) $(SQ_TARGET)

mod_mysql:
	cd $(SQ_MODDIR)/mysql; $(SQ_MAKE) $(SQ_TARGET)

mod_sample:
	cd $(SQ_MODDIR)/sample; $(SQ_MAKE) $(SQ_TARGET)

build:
	cd $(SQ_MODDIR)/sqlite; $(SQ_MAKE) build
	cd $(SQ_MODDIR)/xml; $(SQ_MAKE) build
	cd $(SQ_MODDIR)/mmdb; $(SQ_MAKE) build
	cd $(SQ_MODDIR)/irc; $(SQ_MAKE) build
	cd $(SQ_MODDIR)/mysql; $(SQ_MAKE) build
	cd source; $(SQ_MAKE) build

link:
	cd $(SQ_MODDIR)/sqlite; $(SQ_MAKE) link
	cd $(SQ_MODDIR)/xml; $(SQ_MAKE) link
	cd $(SQ_MODDIR)/mmdb; $(SQ_MAKE) link
	cd $(SQ_MODDIR)/irc; $(SQ_MAKE) link
	cd $(SQ_MODDIR)/mysql; $(SQ_MAKE) link
	cd source; $(SQ_MAKE) link

clean:
	cd $(SQ_MODDIR)/sqlite; $(SQ_MAKE) clean
	cd $(SQ_MODDIR)/xml; $(SQ_MAKE) clean
	cd $(SQ_MODDIR)/mmdb; $(SQ_MAKE) clean
	cd $(SQ_MODDIR)/irc; $(SQ_MAKE) clean
	cd $(SQ_MODDIR)/mysql; $(SQ_MAKE) clean
	cd source; $(SQ_MAKE) clean

folders:
	mkdir -p $(SQ_LIBDIR)
	mkdir -p $(SQ_OUTDIR)
	mkdir -p $(SQ_OBJDIR)
	mkdir -p $(SQ_OBJDIR)/source/Base
	mkdir -p $(SQ_OBJDIR)/source/Entity
	mkdir -p $(SQ_OBJDIR)/source/Misc
	mkdir -p $(SQ_OBJDIR)/source/Library/Chrono
	mkdir -p $(SQ_OBJDIR)/source/Library/Crypt
	mkdir -p $(SQ_OBJDIR)/source/Library/IO
	mkdir -p $(SQ_OBJDIR)/source/Library/Numeric
	mkdir -p $(SQ_OBJDIR)/source/Library/System
	mkdir -p $(SQ_OBJDIR)/source/Library/Utils
	mkdir -p $(SQ_OBJDIR)/shared/Base
	mkdir -p $(SQ_OBJDIR)/external/Common
	mkdir -p $(SQ_OBJDIR)/external/B64
	mkdir -p $(SQ_OBJDIR)/external/Hash
	mkdir -p $(SQ_OBJDIR)/external/LibIRC
	mkdir -p $(SQ_OBJDIR)/external/MaxmindDB
	mkdir -p $(SQ_OBJDIR)/external/PUGIXML
	mkdir -p $(SQ_OBJDIR)/external/SQLite
	mkdir -p $(SQ_OBJDIR)/external/Squirrel/Lib
	mkdir -p $(SQ_OBJDIR)/modules/irc
	mkdir -p $(SQ_OBJDIR)/modules/mmdb/Handle
	mkdir -p $(SQ_OBJDIR)/modules/mysql/Handle
	mkdir -p $(SQ_OBJDIR)/modules/mysql/Wrapper
	mkdir -p $(SQ_OBJDIR)/modules/sqlite/Handle
	mkdir -p $(SQ_OBJDIR)/modules/xml/Handle
	mkdir -p $(SQ_OBJDIR)/modules/xml/Wrapper
