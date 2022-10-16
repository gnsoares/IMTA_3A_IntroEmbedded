.PHONY: main.o board.o

all: main.o board.o
	gcc -o play main.o board.o

main.o:
	gcc -o main.o -c main.c

clean:
	rm *.o || true
