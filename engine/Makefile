# Build engine
CC = kos32-g++
AR = kos32-ar

KOLIBRI_SDK = D:/kolibrios/contrib/sdk
MSYS = D:/MinGW/msys/1.0

ENGINE = ../bin/kos32/libkoengine.a
CSRC = $(shell find src/ -name *.c) 
CPPSRC = $(filter-out src/entry.cpp, $(shell find src/ -name *.cpp))
TARGETS = $(CSRC:src/%.c=obj/kos32/%.o) $(CPPSRC:src/%.cpp=obj/kos32/%.o)

CFLAGS = -DKOS32 -D_DEBUG -std=c++11 -c -O2 -fno-ident -fomit-frame-pointer -Wno-pointer-arith -U__WIN32__ -U_Win32 -U_WIN32 -U__MINGW32__ -UWIN32 
IFLAGS = -I$(KOLIBRI_SDK)/sources/newlib/libc/include -I$(KOLIBRI_SDK)/sources/libstdc++-v3/include -Isrc -ID:/source/kolibri

default: engine wengine

engine: $(ENGINE)

$(ENGINE): $(TARGETS) Makefile
	$(AR) crs $(ENGINE) $(TARGETS)
	cp -r resources ../bin/kos32

obj/kos32/%.o: src/%.c Makefile
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(IFLAGS) $< -o $@
	
obj/kos32/%.o: src/%.cpp Makefile
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(IFLAGS) $< -o $@

.PHONY: clean wclean

clean:
	rm -rf $(shell find obj/ -name *.o) $(ENGINE)

WCC = g++
WAR = ar

WENGINE = ../bin/win32/libkoengine.a
WTARGETS = $(CSRC:src/%.c=obj/win32/%.o) $(CPPSRC:src/%.cpp=obj/win32/%.o)

WCFLAGS = -std=c++11 -c -O2 -D_DEBUG
WIFLAGS = -Isrc -ID:/source/kolibriw

wengine: $(WENGINE)

$(WENGINE): $(WTARGETS) Makefile
	$(WAR) crs $(WENGINE) $(WTARGETS)
	cp -r resources ../bin/win32

obj/win32/%.o: src/%.c Makefile
	@mkdir -p $(dir $@)
	$(WCC) $(WCFLAGS) $(WIFLAGS) $< -o $@
	
obj/win32/%.o: src/%.cpp Makefile
	@mkdir -p $(dir $@)
	$(WCC) $(WCFLAGS) $(WIFLAGS) $< -o $@

wclean:
	rm -rf $(shell find obj/win32 -name *.o) $(WENGINE)