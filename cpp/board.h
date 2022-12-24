#ifndef BOARD_H
#define BOARD_H

#include <iostream>

#include "cell.h"
#include "config.h"
#include "piece.h"
#include "playerEnum.h"
#include "statusEnum.h"

class Board {
   private:
    Cell state[DIMENSIONS][DIMENSIONS];
    int p1Pieces[NB_SIZE];
    int p2Pieces[NB_SIZE];

    Cell& getCellAt(int line, int column);
    int* getHousesOfPlayer(Player player);
    Piece getPlayerPiece(Player player, Size size);
    Player checkRow(int row);
    Player checkColumn(int column);
    Player checkMainDiagonal();
    Player checkSecDiagonal();

   public:
    Board();
    Board(const Board& other);
    ~Board();

    void debug();

    Player nextPlayer(Player currentPlayer) const;
    Player getPlaceHolder(int line, int column);
    Size getPieceSize(int line, int column);
    int getNbPiecesInHouse(Player checkedPlayer, Size pieceSize);

    StatusEnum placePiece(Player currentPlayer, Size pieceSize, int line, int column);
    StatusEnum movePiece(int sourceLine, int sourceColumn, int targetLine, int targetColumn);

    Player getWinner();

    friend std::ostream& operator<<(std::ostream& stream, Board& board);
};

#endif  // BOARD_H
