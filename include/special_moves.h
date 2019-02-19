#ifndef SPECIAL_MOVES_H
#define SPECIAL_MOVES_H

#include <stdlib.h>

#include "helpers.h"

extern const int8_t white_castle_king_side;
extern const int8_t white_castle_queen_side;
extern const int8_t black_castle_king_side;
extern const int8_t black_castle_queen_side;
extern const int8_t all_castles_possible;

int handle_pawn_conversion(u_int8_t[8][8], move, turn);
int handle_en_passant(u_int8_t[8][8], move);
int can_en_passant(u_int8_t[8][8], u_int8_t, move, int, int);
int is_en_passant(u_int8_t[8][8], move);
int handle_castle(u_int8_t[8][8], int8_t *, turn, special);
int update_castle_config(u_int8_t, move, int8_t *);
int can_castle_king_side(turn, int8_t);
int can_castle_queen_side(turn, int8_t);
int disable_king_castle(int8_t *, turn);
int disable_queen_castle(int8_t *, turn);

#endif