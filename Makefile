#####################################################################
# @file Makefile
# @brief 
#
# @version 1.0
# @author yangyoufa
#####################################################################

OS := $(shell uname)

TARGETDIR=~/PRG/XNode/3Party

.PHONY: all premake install clean distclean 

all: .build .obj
	cd .build && make

.build .obj premake: premake.lua
	premake --os=macosx --file=premake.lua gmake

install:
	mkdir -p ${TARGETDIR}/libuvpp/include
	mkdir -p ${TARGETDIR}/lib
	cp -rf include/* ${TARGETDIR}/libuvpp/include
	cp -rf lib/* ${TARGETDIR}/lib/

clean: 
	rm -rf .obj .build

test:

release debug:

doxy:
	@(SED) -i -e s\,INPUT\ *=.*$$,INPUT\ =\ "$(DIRS)"\,g Doxyfile 

doc:
	doxygen

