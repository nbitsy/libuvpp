#####################################################################
# @file Makefile
# @brief 
#
# @version 1.0
# @author yangyoufa
#####################################################################

OS := $(shell uname)

TARGETDIR=~/PRG/XNode/3Party
LIBUVPPTAR=${TARGETDIR}/libuvpp

.PHONY: all premake install clean distclean 

all: .build .obj
	cd .build && make

.build .obj premake: premake.lua
	premake --os=macosx --file=premake.lua gmake

install: all
	mkdir -p ${TARGETDIR}
	cp -rf 3Party/* ${TARGETDIR}
	mkdir -p ${LIBUVPPTAR}/include
	mkdir -p ${LIBUVPPTAR}/lib
	cp -rf include/* ${LIBUVPPTAR}/include
	cp -rf lib/* ${LIBUVPPTAR}/lib

clean: 
	rm -rf .obj .build

test:

release debug:

doxy:
	@(SED) -i -e s\,INPUT\ *=.*$$,INPUT\ =\ "$(DIRS)"\,g Doxyfile 

doc:
	doxygen

