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

void get_player_next_action(player current_player, action *next_action) {
    char ans[20] = "";

    printf("Player %d: What is your next action? [P]lace or [M]ove: ", current_player);
    scanf("%s", ans);
    while (ans[0] != 'p' && ans[0] != 'P' && ans[0] != 'm' && ans[0] != 'M') {
        printf("Invalid option. Try again: [P]lace or [M]ove: ");
        scanf("%s", ans);
    }

    if (ans[0] == 'p' || ans[0] == 'P')
        *next_action = PLACE;
    else
        *next_action = MOVE;
}

void get_tile_column_and_line(int *line, int *column) {
    char ans[20] = "";
    char column_ans;
    int line_ans;

    scanf("%s", ans);
    column_ans = ans[0];
    line_ans = (int)(ans[1] - '0');
    while (column_ans < 'a' || column_ans > 'c' || line_ans < 1 || line_ans > 3) {
        printf("Invalid option. Try again: (Ex.: a2, b3) ");
        scanf("%s", ans);
        column_ans = ans[0];
        line_ans = (int)(ans[1] - '0');
    }

    *column = (int)(column_ans - 'a');
    *line = 3 - line_ans;
}

void get_piece_size_to_place(board b,
                             player current_player,
                             size *piece_size) {
    char ans[20] = "";

    int small_available = get_nb_piece_in_house(b, current_player, SMALL) > 0;
    int medium_available = get_nb_piece_in_house(b, current_player, MEDIUM) > 0;
    int large_available = get_nb_piece_in_house(b, current_player, LARGE) > 0;
    int n_available = small_available + medium_available + large_available;
    int not_valid_ans = 1;

    char pieces_available[50] = "";
    char possible_answers[6] = {'s', 'S', 'm', 'M', 'l', 'L'};

    if (n_available == 1) {
        *piece_size = small_available ? SMALL : (medium_available ? MEDIUM : LARGE);
    } else {
        if (n_available == 3) {
            strcpy(pieces_available, "[S]mall, [M]edium or [L]arge: ");
        } else if (n_available == 2) {
            if (small_available) {
                strcat(pieces_available, "[S]mall or ");
                if (medium_available) {
                    strcat(pieces_available, "[M]edium: ");
                } else {
                    strcat(pieces_available, "[L]arge: ");
                    possible_answers[2] = 'l';
                    possible_answers[3] = 'L';
                }
            } else {
                strcpy(pieces_available, "[M]edium or [L]arge: ");
                possible_answers[0] = 'm';
                possible_answers[1] = 'M';
                possible_answers[2] = 'l';
                possible_answers[3] = 'L';
            }
        }
        printf("Which piece size? %s", pieces_available);
        scanf("%s", ans);
        for (int i = 0; i < 2 * n_available; i++) {
            not_valid_ans *= ans[0] != possible_answers[i];
        }
        while (not_valid_ans) {
            printf("Invalid option. Try again: %s", pieces_available);
            scanf("%s", ans);
            for (int i = 0; i < 2 * n_available; i++) {
                not_valid_ans *= ans[0] != possible_answers[i];
            }
        }
        if (ans[0] == 's' || ans[0] == 'S')
            *piece_size = SMALL;
        else if (ans[0] == 'm' || ans[0] == 'M')
            *piece_size = MEDIUM;
        else
            *piece_size = LARGE;
    }
}

void get_place_args(board b,
                    player current_player,
                    size *piece_size,
                    int *place_line,
                    int *place_column) {
    get_piece_size_to_place(b, current_player, piece_size);
    printf("Which tile? (Ex.: b3) ");
    get_tile_column_and_line(place_line, place_column);
}

void get_move_args(int *source_line,
                   int *source_column,
                   int *target_line,
                   int *target_column) {
    printf("Original piece tile? (Ex.: b3) ");
    get_tile_column_and_line(source_line, source_column);
    printf("New piece tile? (Ex.: b3) ");
    get_tile_column_and_line(target_line, target_column);
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
            get_player_next_action(current_player, &next_action);
            if (next_action == PLACE) {
                get_place_args(game, current_player, &piece_size,
                               &place_line, &place_column);
                action_result = place_piece(game, current_player, piece_size,
                                            place_line, place_column);

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
                get_move_args(&source_line, &source_column,
                              &target_line, &target_column);
                action_result = move_piece(game,
                                           source_line, source_column,
                                           target_line, target_column);

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
