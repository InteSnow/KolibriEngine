# Build game
CC = kos32-g++
LD = kos32-ld
OBJCOPY = kos32-objcopy

KOLIBRI_SDK = D:/kolibrios/contrib/sdk
MSYS = D:/MinGW/msys/1.0

ENGINE = ../bin/kos32/libkoengine.a
GAME = ../bin/kos32/game.kex
CSRC = $(shell find src/ -name *.c)
CPPSRC = $(shell find src/ -name *.cpp) 
TARGETS = $(CSRC:src/%.c=obj/kos32/%.o) $(CPPSRC:src/%.cpp=obj/kos32/%.o) obj/kos32/entry.o

CFLAGS = -std=c++11 -c -O2 -D_DEBUG -DKOS32 -fno-ident -fomit-frame-pointer -U__WIN32__ -U_Win32 -U_WIN32 -U__MINGW32__ -UWIN32
IFLAGS = -I$(KOLIBRI_SDK)/sources/newlib/libc/include -I$(KOLIBRI_SDK)/sources/libstdc++-v3/include -I../engine/src -ID:/source/kolibri
SCRIPT = app.lds
LDFLAGS = -static -nostdlib -T$(SCRIPT) --image-base 0 
LFLAGS = -L$(MSYS)/home/autobuild/tools/win32/mingw32/lib -L../bin/kos32
LIBS = -lkoengine -lTinyGL -lGL.dll -lglsl -lstdc++ -lsupc++ -lgcc -lc.dll 

TRANSFER = F:/

default: game wgame

RESOURCES = $(shell find ../engine/resources/)

game: $(GAME)
	
$(GAME): $(TARGETS) $(ENGINE) Makefile
	$(LD) $(LDFLAGS) $(LFLAGS) $(TARGETS) -o $(GAME) $(LIBS)
	$(OBJCOPY) $(GAME) -O binary
	cp -r resources ../bin/kos32
	
obj/kos32/%.o: src/%.c Makefile
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(IFLAGS) $< -o $@

obj/kos32/%.o: src/%.cpp Makefile
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(IFLAGS) $< -o $@

obj/kos32/entry.o: ../engine/src/entry.cpp Makefile
	$(CC) $(CFLAGS) $(IFLAGS) $< -o $@

.PHONY: clean wclean

clean:
	rm -rf $(shell find obj/kos32 -name *.o) $(GAME)

WCC = g++
WLD = ld

WENGINE = ../bin/win32/libkoengine.a
WGAME = ../bin/win32/game.exe
WTARGETS = $(CSRC:src/%.c=obj/win32/%.o) $(CPPSRC:src/%.cpp=obj/win32/%.o) obj/win32/entry.o

WCFLAGS = -std=c++11 -c -O2 -D_DEBUG
WIFLAGS = -I../engine/src -ID:/source/kolibriw
WLDFLAGS = -static -static-libgcc -static-libstdc++
WLFLAGS = -L../bin/win32
WLIBS = -lkoengine -lglew32 -lgdi32 -lopengl32

wgame: $(WGAME)

$(WGAME): $(WTARGETS) $(WENGINE) Makefile
	$(WCC) $(WLDFLAGS) $(WLFLAGS) $(WTARGETS) -o $(WGAME) $(WLIBS)
	cp -r resources ../bin/win32
	cp -r ../bin $(TRANSFER)

obj/win32/%.o: src/%.c Makefile
	@mkdir -p $(dir $@)
	$(WCC) $(WCFLAGS) $(WIFLAGS) $< -o $@

obj/win32/%.o: src/%.cpp Makefile
	@mkdir -p $(dir $@)
	$(WCC) $(WCFLAGS) $(WIFLAGS) $< -o $@

obj/win32/entry.o: ../engine/src/entry.cpp Makefile
	$(WCC) $(WCFLAGS) $(WIFLAGS) $< -o $@

wclean:
	rm -rf $(shell find obj/win32 -name *.o) $(WGAME)