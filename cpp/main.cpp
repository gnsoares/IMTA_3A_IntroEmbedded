#include <iostream>
#include <string>

#include "board.h"

#define DEBUG 0

void get_place_args(Board* game, Player currentPlayer, Size* size, int* source_line, int* source_column);
void get_move_args(int* source_line, int* source_column, int* target_line, int* target_column);

int main() {
    char action;
    int source_line;
    int source_column;
    int target_line;
    int target_column;
    Size size;  // inputs
    StatusEnum action_result = INVALID;
    Player winner = NO_PLAYER;

    Board* game = new Board();
    Player currentPlayer = PLAYER_1;

    while (winner == NO_PLAYER) {
#if DEBUG
        game->debug();
#endif

        std::cout << *game << std::endl;

        while (action_result != OK) {  // get player action until it's valid
            std::cout << "Player " << (currentPlayer == PLAYER_1 ? "1" : "2") << " Action:\n1: Place, 2: Move, [Q]uit? ";
            std::cin >> action;
            switch (action) {
                case '1':
                    // get place arguments and try to place piece
                    get_place_args(game, currentPlayer, &size,
                                   &source_line, &source_column);
                    action_result = game->placePiece(currentPlayer, size,
                                                     source_line, source_column);
                    break;
                case '2':
                    // get move arguments and try to move piece
                    get_move_args(&source_line, &source_column,
                                  &target_line, &target_column);
                    action_result = game->movePiece(source_line, source_column,
                                                    target_line, target_column);
                    break;
                case 'q':
                    return 0;
                default:
                    break;
            }
            // give input to user if action not valid
            if (action_result == INVALID) {
                std::cout << std::endl
                          << "Invalid arguments. Try again" << std::endl;
            }
        }

        action_result = INVALID;
        winner = game->getWinner();
        currentPlayer = game->nextPlayer(currentPlayer);
    }

    std::cout << *game << std::endl
              << "Player " << winner << " WINS!\n";

    return 0;
}

void get_tile_column_and_line(int* line, int* column) {
    char ans[20] = "";
    char column_ans;
    int line_ans;

    // ask piece location until it's a valid answer
    std::cin >> ans;
    column_ans = ans[0];
    line_ans = (int)(ans[1] - '0');
    while (
        column_ans < 'a' ||
        column_ans > ('a' + DIMENSIONS) ||
        line_ans < 1 ||
        line_ans > DIMENSIONS) {
        std::cout << "Invalid option. Try again: (Ex.: a2, b3) ";
        std::cin >> ans;
        column_ans = ans[0];
        line_ans = (int)(ans[1] - '0');
    }

    // map user input to tile location
    *column = (int)(column_ans - 'a');
    *line = DIMENSIONS - line_ans;
}

void get_piece_size_to_place(Board* game, Player currentPlayer, Size* size) {
    char ans[20] = "";

    // get which pieces are available to present options to the user
    // and validate answer
    int small_available = game->getNbPiecesInHouse(currentPlayer, SMALL) > 0;
    int medium_available = game->getNbPiecesInHouse(currentPlayer, MEDIUM) > 0;
    int large_available = game->getNbPiecesInHouse(currentPlayer, LARGE) > 0;
    int n_available = small_available + medium_available + large_available;
    int not_valid_ans = 1;

    // auxiliary vars to present options to the user and validate answer
    char pieces_available[50] = "";
    char possible_answers[6] = {'s', 'S', 'm', 'M', 'l', 'L'};

    if (n_available == 1) {
        // 1 size available: get it
        *size = small_available ? SMALL : (medium_available ? MEDIUM : LARGE);
    } else {
        if (n_available == 3) {
            // all sizes available: print them all
            strcpy(pieces_available, "[S]mall, [M]edium or [L]arge: ");
        } else if (n_available == 2) {
            // 2 sizes available: find which ones and print them
            if (small_available) {
                strcat(pieces_available, "[S]mall or ");
                if (medium_available) {
                    strcat(pieces_available, "[M]edium: ");
                } else {
                    strcat(pieces_available, "[L]arge: ");
                    // change possible answers accordingly
                    possible_answers[2] = 'l';
                    possible_answers[3] = 'L';
                }
            } else {
                strcpy(pieces_available, "[M]edium or [L]arge: ");
                // change possible answers accordingly
                possible_answers[0] = 'm';
                possible_answers[1] = 'M';
                possible_answers[2] = 'l';
                possible_answers[3] = 'L';
            }
        }

        // ask piece size until it's a valid answer
        std::cout << "Which piece size? " << pieces_available;
        std::cin >> ans;
        for (int i = 0; i < 2 * n_available; i++) {
            not_valid_ans *= ans[0] != possible_answers[i];
        }
        while (not_valid_ans) {
            std::cout << "Piece size not available. Try again: " << pieces_available;
            std::cin >> ans;
            for (int i = 0; i < 2 * n_available; i++) {
                not_valid_ans *= ans[0] != possible_answers[i];
            }
        }

        // map user input to piece size
        if (ans[0] == 's' || ans[0] == 'S')
            *size = SMALL;
        else if (ans[0] == 'm' || ans[0] == 'M')
            *size = MEDIUM;
        else
            *size = LARGE;
    }
}
void get_place_args(Board* game,
                    Player currentPlayer,
                    Size* size,
                    int* place_line,
                    int* place_column) {
    get_piece_size_to_place(game, currentPlayer, size);
    std::cout << "Which tile? (Ex.: b3) ";
    get_tile_column_and_line(place_line, place_column);
}
void get_move_args(int* source_line,
                   int* source_column,
                   int* target_line,
                   int* target_column) {
    std::cout << "Original piece tile? (Ex.: b3) ";
    get_tile_column_and_line(source_line, source_column);
    std::cout << "New piece tile? (Ex.: b3) ";
    get_tile_column_and_line(target_line, target_column);
}
