#include <stdio.h>

#include "board.h"
#include "game.h"

/**
 * @file main.c
 * @brief main game execution
 */

int main(int args, char **argv) {
    // initialize game
    board game = new_game();
    printf("A board is created.\n");

    // game control variables
    player winner = get_winner(game);
    player current_player = PLAYER_1;
    action next_action = NOP;
    int action_result = -1;
    size piece_size = NONE;
    int place_line = 0, place_column = 0;
    int source_line = 0, source_column = 0, target_line = 0, target_column = 0;

    // game goes on until someone wins
    while (winner == NO_PLAYER) {
        show_board(game);

        do {  // get player action until it's valid
            get_player_next_action(current_player, &next_action);
            if (next_action == PLACE) {
                // get place arguments and try to place piece
                get_place_args(game, current_player, &piece_size,
                               &place_line, &place_column);
                action_result = place_piece(game, current_player, piece_size,
                                            place_line, place_column);

                // give input to user if action not valid
                switch (action_result) {
                    case 1:
                        printf("Piece size not available for you. Try again!\n\n");
                        break;
                    case 2:
                        printf("Piece size too small for this tile. Try again!\n\n");
                        break;
                    case 3:
                        printf("You entered invalid values. Try again!\n\n");
                        break;
                    default:
                        break;
                }

            } else {
                // get move arguments and try to move piece
                get_move_args(&source_line, &source_column,
                              &target_line, &target_column);
                action_result = move_piece(game,
                                           source_line, source_column,
                                           target_line, target_column);

                // give input to user if action not valid
                switch (action_result) {
                    case 1:
                        printf("No piece at given location. Try again!\n\n");
                        break;
                    case 2:
                        printf("Piece too small for target location. Try again!\n\n");
                        break;
                    case 3:
                        printf("You entered invalid values. Try again!\n\n");
                        break;
                    default:
                        break;
                }
            }
        } while (action_result);

        // check if a player won after last action and change current player
        winner = get_winner(game);
        current_player = next_player(current_player);
    }

    // game is finished: show final board, display winner and destroy game structure
    show_board(game);
    printf("Player %d WINS!\n", winner);
    destroy_game(game);
    printf("Board destroyed.\n");

    return 0;
}
