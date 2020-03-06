
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

all: $(YAFFS)

$(YAFFS) : ../lib/libuv.a ../lib/libuv++.a

clean:
	rm -rf $(YAFFS:.c=) $(YAdSYM) .DS_Store

