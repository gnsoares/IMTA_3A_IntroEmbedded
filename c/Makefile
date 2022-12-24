.PHONY: main.o game.o board.o

all: main.o game.o board.o
	gcc -o play main.o game.o board.o

main.o:
	gcc -o main.o -c main.c

game.o:
	gcc -o game.o -c game.c

clean:
	rm *.o || true
