.PHONY: main.o board.o cell.o piece.o

all: main.o
	g++ -o play main.o board.o cell.o piece.o

main.o: board.o
	g++ -o main.o -c main.cpp

board.o: cell.o
	g++ -o board.o -c board.cpp

cell.o: piece.o
	g++ -o cell.o -c cell.cpp

piece.o:
	g++ -o piece.o -c piece.cpp

clean:
	rm *.o || true
