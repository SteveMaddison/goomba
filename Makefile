CFLAGS = -I./include -fPIC -g
LIB = goomba

SRCS = src/config.c src/font.c src/gui.c src/item.c

all: test

$(LIB): $(LIB).so

$(LIB).so: $(SRCS)
	gcc $(CFLAGS) -o $(LIB).so -shared $(SRCS)

test: test.o $(LIB)
	gcc -o test test.o $(LIB).so -lSDL -lSDL_ttf

test.o: src/test.c
	gcc $(CFLAGS) -o test.o -c src/test.c

