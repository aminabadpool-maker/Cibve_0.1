CC = gcc
CFLAGS = -Wall -O2 -I../raylib/src -DGRAPHICS_API_OPENGL_ES2
LDFLAGS = -L../raylib/src -lraylib -lEGL -lGLESv2 -lm -lpthread -ldl -lrt -lX11

all: cibve

../raylib/src/libraylib.a:
	$(MAKE) -C ../raylib/src PLATFORM=PLATFORM_DESKTOP GRAPHICS_API=GRAPHICS_API_OPENGL_ES2

cibve: main.c ../raylib/src/libraylib.a
	$(CC) $(CFLAGS) main.c -o cibve $(LDFLAGS)

clean:
	rm -f cibve *.o
	$(MAKE) -C ../raylib/src clean
