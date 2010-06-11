CFLAGS = -I./include

SRCS = src/config.c src/gui.c src/item.c src/main.c

all: main

main: $(SRCS)
	gcc $(CFLAGS) -o main $(SRCS)

