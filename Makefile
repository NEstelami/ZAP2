CC := g++

ifneq (, $(shell which ccache))
CC := ccache $(CC)
endif

CFLAGS := -g -std=c++17 -I ZAPD -I lib/assimp/include -I lib/elfio -I lib/json/include -I lib/stb -I lib/tinygltf -I lib/libgfxd -I lib/tinyxml2 -O2 -rdynamic

UNAME := $(shell uname)

FS_INC =
ifneq ($(UNAME), Darwin)
    FS_INC += -lstdc++fs
endif

SRC_DIRS := ZAPD ZAPD/ZRoom ZAPD/ZRoom/Commands ZAPD/Overlays ZAPD/HighLevel ZAPD/OpenFBX

CPP_FILES := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.cpp))
CPP_FILES += lib/tinyxml2/tinyxml2.cpp
O_FILES   := $(CPP_FILES:.cpp=.o)

all: genbuildinfo ZAPD.out copycheck

genbuildinfo:
	python3 ZAPD/genbuildinfo.py

copycheck: ZAPD.out
	python3 copycheck.py

clean:
	rm -f $(O_FILES) ZAPD.out
	rm -f lib/libgfxd/libgfxd.a

rebuild: clean all

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

libgfxd:
	cd lib/libgfxd && $(MAKE) -j

ZAPD.out: libgfxd $(O_FILES)
	$(CC) $(CFLAGS) $(O_FILES) lib/libgfxd/libgfxd.a -o $@ $(FS_INC)
