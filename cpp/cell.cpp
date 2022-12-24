
#include "cell.h"

#include "config.h"
#include "piece.h"

Cell::Cell() : index(0) {}
Cell::Cell(const Cell& other) : index(other.index) {
    for (int i = 0; i < index; i++)
        stack[i] = other.stack[i];
}

void Cell::debug() {
    Piece p;
    std::cout << this << " (" << index << " pieces): ";
    for (int i = 0; i < index; i++) {
        p = stack[i];
        if (i > 0) {
            std::cout << ", ";
        }
        std::cout << " [P"
                  << (p.getOwner() == PLAYER_1 ? "1" : "2")
                  << ":"
                  << (p.getSize() == SMALL ? "S" : p.getSize() == MEDIUM ? "M"
                                                                         : "L")
                  << "]";
    }
}

bool Cell::canPush(const Piece& piece) {
    return peek().getSize() < piece.getSize();
}
void Cell::push(const Piece& piece) {
    stack[index] = piece;
    index += 1;
}
bool Cell::canPop() {
    return peek().getSize() > NONE;
}
Piece& Cell::pop() {
    index -= 1;
    return stack[index];
}
Piece Cell::peek() {
    if (index == 0)
        return Piece();
    return stack[index - 1];
}

Cell& Cell::operator=(const Cell& other) {
    index = other.index;
    for (int i = 0; i < index; i++)
        stack[i] = other.stack[i];
    return *this;
}
