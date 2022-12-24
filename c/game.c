
#include "game.h"

#include <stdio.h>
#include <string.h>

#include "board.h"

void print_tile_string(player p, size s) {
    // no piece in tile: print whitespace
    if (p == NO_PLAYER || s == NONE) {
        printf("   |");
        return;
    }

    printf(" ");
    if (p == PLAYER_1) {
        // player 1 piece in tile: print it blue
        printf("\033[1;34m");
        if (s == SMALL)
            printf("*");
        else if (s == MEDIUM)
            printf("x");
        else
            printf("X");

    } else {
        // player 2 piece in tile: print it yellow
        printf("\033[1;33m");
        if (s == SMALL)
            printf(".");
        else if (s == MEDIUM)
            printf("o");
        else
            printf("O");
    }

    // reset color and print delimiter
    printf("\033[0m |");
}

void show_board(board b) {
    int line = 0;
    printf("\n");

    // first loop goes 2n+1 times because n pieces + n + 1 separators
    for (int i = 0; i < 2 * DIMENSIONS + 1; i++) {
        if (i % 2 == 0) {
            // even line: print separator
            printf("  +");
            for (int k = 0; k < DIMENSIONS; k++)
                printf("---+");
        } else {
            // odd line: print piece tiles with line indicator on the left side
            line = (int)i / 2;
            printf("%d |", DIMENSIONS - line);
            for (int j = 0; j < DIMENSIONS; j++)
                print_tile_string(get_place_holder(b, line, j),
                                  get_piece_size(b, line, j));
        }

        // auxiliary information to the game
        switch (i) {
            case 0:
                // first line
                printf("\t  Player 1 pieces:\n");
                break;
            case 1:
                // second line: first row of player 1 pieces
                printf(
                    "\t     %s   %s   %s\n",
                    get_nb_piece_in_house(b, PLAYER_1, SMALL) > 0 ? "\033[1;34m*\033[0m" : " ",
                    get_nb_piece_in_house(b, PLAYER_1, MEDIUM) > 0 ? "\033[1;34mx\033[0m" : " ",
                    get_nb_piece_in_house(b, PLAYER_1, LARGE) > 0 ? "\033[1;34mX\033[0m" : " ");
                break;
            case 2:
                // third line: second row of player 1 pieces
                printf(
                    "\t     %s   %s   %s\n",
                    get_nb_piece_in_house(b, PLAYER_1, SMALL) > 1 ? "\033[1;34m*\033[0m" : " ",
                    get_nb_piece_in_house(b, PLAYER_1, MEDIUM) > 1 ? "\033[1;34mx\033[0m" : " ",
                    get_nb_piece_in_house(b, PLAYER_1, LARGE) > 1 ? "\033[1;34mX\033[0m" : " ");
                break;
            case 3:
                // fourth line
                printf("\t  Player 2 pieces:\n");
                break;
            case 4:
                // fifth line: first row of player 2 pieces
                printf(
                    "\t     %s   %s   %s\n",
                    get_nb_piece_in_house(b, PLAYER_2, SMALL) > 0 ? "\033[1;33m.\033[0m" : " ",
                    get_nb_piece_in_house(b, PLAYER_2, MEDIUM) > 0 ? "\033[1;33mo\033[0m" : " ",
                    get_nb_piece_in_house(b, PLAYER_2, LARGE) > 0 ? "\033[1;33mO\033[0m" : " ");
                break;
            case 5:
                // sixth line: second row of player 2 pieces
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

    // print column indicators below
    printf("    a   b   c\n\n");
}

void get_player_next_action(player current_player, action *next_action) {
    char ans[20] = "";

    // ask player action until it's a valid answer
    printf("Player %d: What is your next action? [P]lace or [M]ove: ", current_player);
    scanf("%s", ans);
    while (ans[0] != 'p' && ans[0] != 'P' && ans[0] != 'm' && ans[0] != 'M') {
        printf("Invalid option. Try again: [P]lace or [M]ove: ");
        scanf("%s", ans);
    }

    // map user input to player action
    if (ans[0] == 'p' || ans[0] == 'P')
        *next_action = PLACE;
    else
        *next_action = MOVE;
}

void get_tile_column_and_line(int *line, int *column) {
    char ans[20] = "";
    char column_ans;
    int line_ans;

    // ask piece location until it's a valid answer
    scanf("%s", ans);
    column_ans = ans[0];
    line_ans = (int)(ans[1] - '0');
    while (column_ans < 'a' || column_ans > ('a' + DIMENSIONS) || line_ans < 1 || line_ans > DIMENSIONS) {
        printf("Invalid option. Try again: (Ex.: a2, b3) ");
        scanf("%s", ans);
        column_ans = ans[0];
        line_ans = (int)(ans[1] - '0');
    }

    // map user input to tile location
    *column = (int)(column_ans - 'a');
    *line = DIMENSIONS - line_ans;
}

void get_piece_size_to_place(board b,
                             player current_player,
                             size *piece_size) {
    char ans[20] = "";

    // get which pieces are available to present options to the user
    // and validate answer
    int small_available = get_nb_piece_in_house(b, current_player, SMALL) > 0;
    int medium_available = get_nb_piece_in_house(b, current_player, MEDIUM) > 0;
    int large_available = get_nb_piece_in_house(b, current_player, LARGE) > 0;
    int n_available = small_available + medium_available + large_available;
    int not_valid_ans = 1;

    // auxiliary vars to present options to the user and validate answer
    char pieces_available[50] = "";
    char possible_answers[6] = {'s', 'S', 'm', 'M', 'l', 'L'};

    if (n_available == 1) {
        // 1 size available: get it
        *piece_size = small_available ? SMALL : (medium_available ? MEDIUM : LARGE);
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

        // map user input to piece size
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
