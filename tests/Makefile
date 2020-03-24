
.PHONY: .libuv .libuvpp

LIB=-L../lib -L../../Core/lib -L../../Core/3party/tcmalloc/lib
INCL=-I../include -I../../Core/include

CPPFLAGS=-g -Wall -std=c++17 $(INCL) -D_DEBUG -DUSE_TC_MALLOC
CFLAGS=-g -Wall -std=c++17 $(INCL) -D_DEBUG -DUSE_TC_MALLOC
LDFLAGS=$(LIB) -lcore -ltcmalloc_minimal

FF := $(shell find * -name "*.cpp")
FF += $(shell find * -name "*.c")
FFS := $(FF:.cpp=)
YAFFS := $(FFS:.c=)
dSYM := $(FF:.cpp=.dSYM)
YAdSYM = $(dSYM:.c=.dSYM)

all: .libuv .libuvpp $(YAFFS)

$(YAFFS): ../3Party/libuv/lib/libuv.a ../lib/libuvpp.a

.libuv : ../3Party/libuv/lib/libuv.a
	make -C ..

.libuvpp : ../lib/libuvpp.a

install :
	make install -C ..

clean:
	make clean -C ..
	rm -rf $(YAFFS:.c=) $(YAdSYM) .DS_Store

