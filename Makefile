PREFIX = /usr

CC = gcc
LD = gcc
INSTALL = install

LIB = goomba

CFLAGS = -Wall -I./include -I/opt/local/include -fPIC -g
LDFLAGS = -L/opt/local/lib -lSDL -lSDL_ttf
LIBNAME = lib$(LIB)

OBJS = src/config.o src/control.o src/font.o src/gui.o src/item.o src/key.o

all: $(LIBNAME).so test

$(LIBNAME).so: $(OBJS)
	$(LD) -o $(LIBNAME).so -shared $(LDFLAGS) $(OBJS)

install: all
	$(INSTALL) -m 755 -t $(PREFIX)/lib $(LIBNAME).so
	$(INSTALL) -m 755 -d $(PREFIX)/include/$(LIB)
	$(INSTALL) -m 644 -t $(PREFIX)/include/$(LIB) include/$(LIB)/*.h

test: src/test.o $(LIBNAME).so
	$(LD) -o test src/test.o $(LDFLAGS) -L. -l$(LIB)

.c.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -f $(LIBNAME).so *.o src/*.o test

