#####################################################################
# @file Makefile
# @brief 
#
# @version 1.0
# @author yangyoufa
#####################################################################

OS := $(shell uname)

.PHONY: all premake install clean distclean 

all: .build .obj
	cd .build && make

.build .obj premake: premake.lua
	premake --os=macosx --file=premake.lua gmake

install:

clean: 
	rm -rf .obj .build

test:

release debug:

doxy:
	@(SED) -i -e s\,INPUT\ *=.*$$,INPUT\ =\ "$(DIRS)"\,g Doxyfile 

doc:
	doxygen

