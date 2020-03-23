#####################################################################
# @file Makefile
# @brief 
#
# @version 1.0
# @author yangyoufa
#####################################################################

OS := $(shell uname)

TARGETDIR1=~/PRG/XNode/3party
LIBUVPPTAR1=${TARGETDIR1}/libuvpp

TARGETDIR2=~/PRG/XNetTop/3party
LIBUVPPTAR2=${TARGETDIR2}/libuvpp

.PHONY: all premake install clean distclean 

all: .build .obj
	cd .build && make

.build .obj premake: premake.lua
	premake --os=macosx --file=premake.lua gmake

install: all xnode xnettop

xnode:
	mkdir -p ${TARGETDIR1}
	cp -rf 3party/* ${TARGETDIR1}
	mkdir -p ${LIBUVPPTAR1}/include
	mkdir -p ${LIBUVPPTAR1}/lib
	cp -rf include/* ${LIBUVPPTAR1}/include
	cp -rf lib/* ${LIBUVPPTAR1}/lib

xnettop:
	mkdir -p ${TARGETDIR2}
	cp -rf 3party/* ${TARGETDIR2}
	mkdir -p ${LIBUVPPTAR2}/include
	mkdir -p ${LIBUVPPTAR2}/lib
	cp -rf include/* ${LIBUVPPTAR2}/include
	cp -rf lib/* ${LIBUVPPTAR2}/lib

clean: 
	rm -rf .obj .build

test:

release debug:

doxy:
	@(SED) -i -e s\,INPUT\ *=.*$$,INPUT\ =\ "$(DIRS)"\,g Doxyfile 

doc:
	doxygen

