CC= gcc
CFLAGS= -c -nodefaultlibs -mmmx -mtune=pentiumpro -O2 -w

OBJS= clip.o vertex.o api.o list.o init.o matrix.o texture.o \
      misc.o clear.o light.o select.o get.o error.o \
      zbuffer.o zline.o zdither.o ztriangle.o \
      zmath.o image_util.o msghandling.o \
      arrays.o specbuf.o glu.o kosgl.o wingl.o
	        

INCLUDES = -I../include
LIB = libTinyGL.a
LPATH = ../../bin/kos32

all: windows kolibri

windows: LPATH= ../../bin/win32
windows: $(LIB)

kolibri: CFLAGS+= -DKOS32 
kolibri: $(LIB)

$(LIB): $(OBJS)
	rm -f $(LIB)
	ar rcs $(LIB) $(OBJS)
	cp $@ $(LPATH)
	rm $(LIB)

clean:
	rm -f *~ *.o *.a ../../bin/kos32/$(LIB) ../../bin/win32/$(LIB)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $*.c

clip.o: zgl.h zfeatures.h
vertex.o: zgl.h zfeatures.h
light.o: zgl.h zfeatures.h
matrix.o: zgl.h zfeatures.h
list.o: zgl.h opinfo.h zfeatures.h
arrays.c: zgl.h zfeatures.h
specbuf.o: zgl.h zfeatures.h
glx.o: zgl.h zfeatures.h
nglx.o: zgl.h zfeatures.h
zline.o: zgl.h zfeatures.h zline.h
ztriangle.o: ztriangle.c ztriangle.h zgl.h zfeatures.h
	$(CC) $(CFLAGS) $(INCLUDES) -c $*.c
