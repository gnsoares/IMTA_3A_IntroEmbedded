#ifndef _GAME_H_
#define _GAME_H_

#include "board.h"

/**
 * @file game.h
 *
 * @brief Gobblet game helper functions for user interaction.
 */

/**
 * @brief the different types of actions a user can take.
 */
typedef enum { NOP,
               PLACE,
               MOVE } action;

/**
 * @brief prints the string representing a piece with different color depeding on the player and symbol depending on the size.
 *
 * @param p the piece owner
 * @param s the piece size
 */
void print_tile_string(player p, size s);

/**
 * @brief prints the current board state.
 *
 * @param b the board structure
 */
void show_board(board b);

/**
 * @brief gets inputs from the user about next player action.
 *
 * @param current_player the player that's going to play
 * @param next_action where to store the action chosen
 */
void get_player_next_action(player current_player, action *next_action);

/**
 * @brief gets inputs from the user about a tile to be chosen.
 *
 * User is presented with possible options and can choose by inputting first
 * letter of chosen action.
 *
 * @param line line number of tile [0, DIMENSIONS)
 * @param column column number of tile [0, DIMENSIONS)
 */
void get_tile_column_and_line(int *line, int *column);

/**
 * @brief gets inputs from the user about a piece size to be placed.
 *
 * User is presented with possible options (if more than one) and can choose by
 * inputting first letter of chosen size.
 *
 * @param b the board structure
 * @param current_player the player that's going to play
 * @param piece_size where to store the piece size chosen
 */
void get_piece_size_to_place(board b,
                             player current_player,
                             size *piece_size);

/**
 * @brief gets inputs from the user about which piece size and location to.
 *
 * @param b the board structure
 * @param current_player the player that's going to play
 * @param piece_size where to store the piece size chosen
 * @param place_line where to store the piece line number
 * @param place_column where to store the piece column number
 */
void get_place_args(board b,
                    player current_player,
                    size *piece_size,
                    int *place_line,
                    int *place_column);

/**
 * @brief gets inputs from the user which piece to be moved to which location.
 *
 * @param source_line where to store the original piece line number
 * @param source_column where to store the original piece column number
 * @param target_line where to store the target piece line number
 * @param target_column where to store the target piece column number
 */
void get_move_args(int *source_line,
                   int *source_column,
                   int *target_line,
                   int *target_column);

#endif
