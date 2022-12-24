#include "board.h"

#include <iostream>
#include <vector>

#include "cell.h"
#include "config.h"
#include "piece.h"
#include "playerEnum.h"
#include "statusEnum.h"

Board::Board() {
    for (int i = 0; i < DIMENSIONS; i++)
        for (int j = 0; j < DIMENSIONS; j++)
            state[i][j] = Cell();
    for (int i = 0; i < NB_SIZE; i++)
        p1Pieces[i] = p2Pieces[i] = 2;
}
Board::Board(const Board& other) {
    for (int i = 0; i < DIMENSIONS; i++)
        for (int j = 0; j < DIMENSIONS; j++)
            state[i][j] = other.state[i][j];
    for (int i = 0; i < NB_SIZE; i++) {
        p1Pieces[i] = other.p1Pieces[i];
        p2Pieces[i] = other.p2Pieces[i];
    }
}
Board::~Board() {}

void Board::debug() {
    std::cout << "DEBUG BOARD =====================================\n";
    for (int i = 0; i < DIMENSIONS; i++) {
        for (int j = 0; j < DIMENSIONS; j++) {
            std::cout << "(" << i << ", " << j << ") " << getPlaceHolder(i, j) << " " << getPieceSize(i, j) << " ";
            getCellAt(i, j).debug();
            std::cout << std::endl;
        }
    }
    std::cout << "=================================================\n";
}

Cell& Board::getCellAt(int line, int column) {
    return state[line][column];
}
// int* Board::getHousesOfPlayer(Player player) {}

Player Board::nextPlayer(Player currentPlayer) const {
    return currentPlayer == PLAYER_1 ? PLAYER_2 : PLAYER_1;
}
Player Board::getPlaceHolder(int line, int column) {
    return getCellAt(line, column).peek().getOwner();
}
Size Board::getPieceSize(int line, int column) {
    return getCellAt(line, column).peek().getSize();
}
int Board::getNbPiecesInHouse(Player checkedPlayer, Size pieceSize) {
    if (checkedPlayer == PLAYER_1)
        return p1Pieces[pieceSize - 1];
    if (checkedPlayer == PLAYER_2)
        return p2Pieces[pieceSize - 1];
    return -1;
}

Piece Board::getPlayerPiece(Player player, Size size) {
    if (player == PLAYER_1)
        p1Pieces[size - 1] -= 1;
    if (player == PLAYER_2)
        p2Pieces[size - 1] -= 1;
    return Piece(player, size);
}
StatusEnum Board::placePiece(Player currentPlayer,
                             Size pieceSize,
                             int line,
                             int column) {
    if (!getNbPiecesInHouse(currentPlayer, pieceSize))
        return INVALID;
    Cell& c = getCellAt(line, column);
    Piece p = getPlayerPiece(currentPlayer, pieceSize);
    if (!c.canPush(p))
        return INVALID;
    c.push(p);
    return OK;
}
StatusEnum Board::movePiece(int sourceLine,
                            int sourceColumn,
                            int targetLine,
                            int targetColumn) {
    Cell& sourceCell = getCellAt(sourceLine, sourceColumn);
    Cell& targetCell = getCellAt(targetLine, targetColumn);
    if (!targetCell.canPush(sourceCell.peek()))
        return INVALID;
    targetCell.push(sourceCell.pop());
    return OK;
}

Player Board::checkRow(int row) {
    Player p = getPlaceHolder(row, 0);
    for (int i = 1; i < DIMENSIONS; i++)
        if (p != getPlaceHolder(row, i))
            return NO_PLAYER;
    std::cout << "Row " << row << " " << p;
    return p;
}
Player Board::checkColumn(int column) {
    Player p = getPlaceHolder(0, column);
    for (int i = 1; i < DIMENSIONS; i++)
        if (p != getPlaceHolder(i, column))
            return NO_PLAYER;
    return p;
}
Player Board::checkMainDiagonal() {
    Player p = getPlaceHolder(0, 0);
    for (int i = 1; i < DIMENSIONS; i++)
        if (p != getPlaceHolder(i, i))
            return NO_PLAYER;
    return p;
}
Player Board::checkSecDiagonal() {
    Player p = getPlaceHolder(0, DIMENSIONS - 1);
    for (int i = 1; i < DIMENSIONS; i++)
        if (p != getPlaceHolder(i, DIMENSIONS - i - 1))
            return NO_PLAYER;
    return p;
}
Player Board::getWinner() {
    Player winner;
    if ((winner = checkMainDiagonal()) != NO_PLAYER)
        return winner;
    if ((winner = checkSecDiagonal()) != NO_PLAYER)
        return winner;
    std::vector<Player> winners;
    for (int i = 0; i < DIMENSIONS; i++)
        winners.push_back(checkRow(i));
    if (std::count(winners.begin(), winners.end(), NO_PLAYER) != 3) {
        if (std::count(winners.begin(), winners.end(), PLAYER_1) >= 1) {
            if (std::count(winners.begin(), winners.end(), PLAYER_2) >= 1)
                return NO_PLAYER;
            return PLAYER_1;
        }
        return PLAYER_2;
    }
    winners.clear();
    for (int i = 0; i < DIMENSIONS; i++)
        winners.push_back(checkColumn(i));
    if (std::count(winners.begin(), winners.end(), NO_PLAYER) != 3) {
        if (std::count(winners.begin(), winners.end(), PLAYER_1) >= 1) {
            if (std::count(winners.begin(), winners.end(), PLAYER_2) >= 1)
                return NO_PLAYER;
            return PLAYER_1;
        }
        return PLAYER_2;
    }
    return NO_PLAYER;
}

std::ostream& operator<<(std::ostream& stream, Board& board) {
    int line = 0;
    stream << std::endl;

    // first loop goes 2n+1 times because n pieces + n + 1 separators
    for (int i = 0; i < 2 * DIMENSIONS + 1; i++) {
        if (i % 2 == 0) {
            // even line: print separator
            stream << "  +";
            for (int k = 0; k < DIMENSIONS; k++)
                stream << "---+";
        } else {
            // odd line: print piece tiles with line indicator on the left side
            line = (int)i / 2;
            stream << DIMENSIONS - line << " |";
            for (int j = 0; j < DIMENSIONS; j++)
                stream << *(new Piece(board.getPlaceHolder(line, j), board.getPieceSize(line, j)));
        }

        // auxiliary information to the game
        switch (i) {
            case 0:
                // first line
                stream << "\t  Player 1 pieces:" << std::endl;
                break;
            case 1:
                // second line: first row of player 1 pieces
                stream << "\t     "
                       << (board.getNbPiecesInHouse(PLAYER_1, SMALL) >
                                   0
                               ? "\033[1;34m*\033[0m"
                               : " ")
                       << "   " << (board.getNbPiecesInHouse(PLAYER_1, MEDIUM) > 0 ? "\033[1;34mx\033[0m" : " ")
                       << "   " << (board.getNbPiecesInHouse(PLAYER_1, LARGE) > 0 ? "\033[1;34mX\033[0m" : " ") << std::endl;
                break;
            case 2:
                // third line: second row of player 1 pieces
                stream << "\t     "
                       << (board.getNbPiecesInHouse(PLAYER_1, SMALL) >
                                   1
                               ? "\033[1;34m*\033[0m"
                               : " ")
                       << "   " << (board.getNbPiecesInHouse(PLAYER_1, MEDIUM) > 1 ? "\033[1;34mx\033[0m" : " ")
                       << "   " << (board.getNbPiecesInHouse(PLAYER_1, LARGE) > 1 ? "\033[1;34mX\033[0m" : " ") << std::endl;
                break;
            case 3:
                // fourth line
                stream << "\t  Player 2 pieces:" << std::endl;
                break;
            case 4:
                // fifth line: first row of player 2 pieces
                stream << "\t     "
                       << (board.getNbPiecesInHouse(PLAYER_2, SMALL) >
                                   0
                               ? "\033[1;33m.\033[0m"
                               : " ")
                       << "   " << (board.getNbPiecesInHouse(PLAYER_2, MEDIUM) > 0 ? "\033[1;33mo\033[0m" : " ")
                       << "   " << (board.getNbPiecesInHouse(PLAYER_2, LARGE) > 0 ? "\033[1;33mO\033[0m" : " ") << std::endl;
                break;
            case 5:
                // sixth line: second row of player 2 pieces
                stream << "\t     "
                       << (board.getNbPiecesInHouse(PLAYER_2, SMALL) >
                                   1
                               ? "\033[1;33m.\033[0m"
                               : " ")
                       << "   " << (board.getNbPiecesInHouse(PLAYER_2, MEDIUM) > 1 ? "\033[1;33mo\033[0m" : " ")
                       << "   " << (board.getNbPiecesInHouse(PLAYER_2, LARGE) > 1 ? "\033[1;33mO\033[0m" : " ") << std::endl;
                break;
            default:
                stream << std::endl;
                break;
        }
    }

    // print column indicators below
    stream << "    a   b   c" << std::endl
           << std::endl;

    return stream;
}
