
.PHONY: .libuv .libuvpp

LIB=-L../lib
INCL=-I../include

CPPFLAGS=-g -Wall -std=c++17 $(INCL)
CFLAGS=-g -Wall -std=c++17 $(INCL)
LDFLAGS=$(LIB)

FF := $(shell find * -name "*.cpp")
FF += $(shell find * -name "*.c")
FFS := $(FF:.cpp=)
YAFFS := $(FFS:.c=)
dSYM := $(FF:.cpp=.dSYM)
YAdSYM = $(dSYM:.c=.dSYM)

all: .libuv .libuvpp $(YAFFS)

$(YAFFS): ../lib/libuv.a ../lib/libuvpp.a

.libuv : ../lib/libuv.a
	make -C ..

.libuvpp : ../lib/libuvpp.a

clean:
	make clean -C ..
	rm -rf $(YAFFS:.c=) $(YAdSYM) .DS_Store

