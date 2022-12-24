#include "piece.h"

#include <iostream>

#include "playerEnum.h"

Piece::Piece()
    : owner(NO_PLAYER),
      size(NONE) {}
Piece::Piece(Player owner, Size size)
    : owner(owner),
      size(size) {}
Piece::Piece(const Piece& other)
    : owner(other.getOwner()),
      size(other.getSize()) {}

Player Piece::getOwner() const {
    return owner;
}
Size Piece::getSize() const {
    return size;
}

Piece& Piece::operator=(const Piece& other) {
    owner = other.getOwner();
    size = other.getSize();
    return *this;
}
std::ostream& operator<<(std::ostream& stream, Piece& piece) {
    Player p = piece.getOwner();
    Size s = piece.getSize();
    // no piece in tile: print whitespace
    if (p == NO_PLAYER || s == NONE) {
        stream << "   |";
        return stream;
    }

    stream << " ";
    if (p == PLAYER_1) {
        // player 1 piece in tile: print it blue
        stream << "\033[1;34m";
        if (s == SMALL)
            stream << "*";
        else if (s == MEDIUM)
            stream << "x";
        else
            stream << "X";

    } else {
        // player 2 piece in tile: print it yellow
        stream << "\033[1;33m";
        if (s == SMALL)
            stream << ".";
        else if (s == MEDIUM)
            stream << "o";
        else
            stream << "O";
    }

    // reset color and print delimiter
    stream << "\033[0m |";
    return stream;
}
