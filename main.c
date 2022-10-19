#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "board.h"

/**
 * @file main.c
 * @brief a simple illustration of how to include and use board.h and board.o.
 */

typedef enum { NOP,
               PLACE,
               MOVE } action;

void print_tile_string(player p, size s) {
    if (p == NO_PLAYER || s == NONE) {
        printf("   |");
        return;
    }
    printf(" ");
    if (p == PLAYER_1) {
        printf("\033[1;34m");
        if (s == SMALL)
            printf("*");
        else if (s == MEDIUM)
            printf("x");
        else
            printf("X");

    } else {
        printf("\033[1;33m");
        if (s == SMALL)
            printf(".");
        else if (s == MEDIUM)
            printf("o");
        else
            printf("O");
    }
    printf("\033[0m |");
}

void show_board(board b) {
    int line;
    printf("\n");
    for (int i = 0; i < 2 * DIMENSIONS + 1; i++) {
        if (i % 2 == 0) {
            printf("  +");
            for (int k = 0; k < DIMENSIONS; k++)
                printf("---+");
        } else {
            line = (int)i / 2;
            printf("%d |", 3 - line);
            for (int j = 0; j < DIMENSIONS; j++)
                print_tile_string(get_place_holder(b, line, j),
                                  get_piece_size(b, line, j));
        }
        switch (i) {
            case 0:
                printf("\t  Player 1 pieces:\n");
                break;
            case 1:
                printf(
                    "\t     %s   %s   %s\n",
                    get_nb_piece_in_house(b, PLAYER_1, SMALL) > 0 ? "\033[1;34m*\033[0m" : " ",
                    get_nb_piece_in_house(b, PLAYER_1, MEDIUM) > 0 ? "\033[1;34mx\033[0m" : " ",
                    get_nb_piece_in_house(b, PLAYER_1, LARGE) > 0 ? "\033[1;34mX\033[0m" : " ");
                break;
            case 2:
                printf(
                    "\t     %s   %s   %s\n",
                    get_nb_piece_in_house(b, PLAYER_1, SMALL) > 1 ? "\033[1;34m*\033[0m" : " ",
                    get_nb_piece_in_house(b, PLAYER_1, MEDIUM) > 1 ? "\033[1;34mx\033[0m" : " ",
                    get_nb_piece_in_house(b, PLAYER_1, LARGE) > 1 ? "\033[1;34mX\033[0m" : " ");
                break;
            case 3:
                printf("\t  Player 2 pieces:\n");
                break;
            case 4:
                printf(
                    "\t     %s   %s   %s\n",
                    get_nb_piece_in_house(b, PLAYER_2, SMALL) > 0 ? "\033[1;33m.\033[0m" : " ",
                    get_nb_piece_in_house(b, PLAYER_2, MEDIUM) > 0 ? "\033[1;33mo\033[0m" : " ",
                    get_nb_piece_in_house(b, PLAYER_2, LARGE) > 0 ? "\033[1;33mO\033[0m" : " ");
                break;
            case 5:
                printf(
                    "\t     %s   %s   %s\n",
                    get_nb_piece_in_house(b, PLAYER_2, SMALL) > 1 ? "\033[1;33m.\033[0m" : " ",
                    get_nb_piece_in_house(b, PLAYER_2, MEDIUM) > 1 ? "\033[1;33mo\033[0m" : " ",
                    get_nb_piece_in_house(b, PLAYER_2, LARGE) > 1 ? "\033[1;33mO\033[0m" : " ");
                break;
            default:
                printf("\n");
                break;
        }
    }
    printf("    a   b   c\n\n");
}

action get_player_next_action(player current_player) {
    action res = 0;

    printf("Player %d: What is your next action?\n", current_player);
    printf("1 - Place\n");
    printf("2 - Move\n");
    scanf("%d", &res);
    while (res < PLACE || res > MOVE) {
        printf("Invalid option. Try again:\n");
        printf("1 - Place\n");
        printf("2 - Move\n");
        scanf("%d", &res);
    }

    return res;
}

void get_place_args(player current_player,
                    size *piece_size,
                    int *place_line,
                    int *place_column) {
    printf("Player %d: Which size? ", current_player);
    scanf("%d", piece_size);
    printf("Player %d: Which line? ", current_player);
    scanf("%d", place_line);
    printf("Player %d: Which column? ", current_player);
    scanf("%d", place_column);
}

void get_move_args(player current_player,
                   int *source_line,
                   int *source_column,
                   int *target_line,
                   int *target_column) {
    printf("Player %d: Original piece line? ", current_player);
    scanf("%d", source_line);
    printf("Player %d: Original piece column? ", current_player);
    scanf("%d", source_column);
    printf("Player %d: New piece line? ", current_player);
    scanf("%d", target_line);
    printf("Player %d: New piece column? ", current_player);
    scanf("%d", target_column);
}

int main(int args, char **argv) {
    board game = new_game();
    printf("A board is created.\n");

    player winner = get_winner(game);
    player current_player = PLAYER_1;

    action next_action = NOP;
    int action_result = -1;
    size piece_size = NONE;
    int place_line = 0, place_column = 0;
    int source_line = 0, source_column = 0, target_line = 0, target_column = 0;

    while (winner == NO_PLAYER) {
        show_board(game);
        do {
            next_action = get_player_next_action(current_player);
            if (next_action == PLACE) {
                get_place_args(current_player, &piece_size, &place_line, &place_column);
                action_result = place_piece(game, current_player, piece_size, place_line, place_column);

                switch (action_result) {
                    case 1:
                        printf("Piece size %d not available for you. Try again!\n\n", piece_size);
                        break;
                    case 2:
                        printf("Piece size %d too small for this tile. Try again!\n\n", piece_size);
                        break;
                    case 3:
                        printf("You entered invalid values. Try again!\n\n");
                        break;
                    default:
                        break;
                }

            } else {
                get_move_args(current_player, &source_line, &source_column, &target_line, &target_column);
                action_result = move_piece(game, source_line, source_column, target_line, target_column);

                switch (action_result) {
                    case 1:
                        printf("No piece at (%d, %d). Try again!\n\n", source_line, source_column);
                        break;
                    case 2:
                        printf("Piece at (%d, %d) too small for target (%d, %d). Try again!\n\n", source_line, source_column, target_line, target_column);
                        break;
                    case 3:
                        printf("You entered invalid values. Try again!\n\n");
                        break;
                    default:
                        break;
                }
            }
        } while (action_result);

        winner = get_winner(game);
        current_player = next_player(current_player);
    }
    show_board(game);
    printf("Player %d WINS!\n", winner);

    destroy_game(game);
    printf("Board destroyed.\n");

    return 0;
}
