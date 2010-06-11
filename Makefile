CFLAGS = -I./include
LIB = goomba

SRCS = src/config.c src/gui.c src/item.c

all: test

$(LIB): $(LIB).so

$(LIB).so: $(SRCS)
	gcc $(CFLAGS) -o $(LIB).so -shared $(SRCS)

test: test.o $(LIB)
	gcc -o test test.o $(LIB).so

test.o: src/test.c
	gcc $(CFLAGS) -o test.o -c src/test.c

