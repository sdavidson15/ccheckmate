#ifndef MOVES_H
#define MOVES_H

#include "special_moves.h"

int perform_move(u_int8_t[8][8], move, int8_t *, turn);
int possible_moves(u_int8_t[8][8], u_int8_t, int, int, move, int[32][2]);
int king_moves(u_int8_t[8][8], u_int8_t, int, int, int[32][2]);
int queen_moves(u_int8_t[8][8], u_int8_t, int, int, int[32][2]);
int rook_moves(u_int8_t[8][8], u_int8_t, int, int, int[32][2]);
int bishop_moves(u_int8_t[8][8], u_int8_t, int, int, int[32][2]);
int knight_moves(u_int8_t[8][8], u_int8_t, int, int, int[32][2]);
int pawn_moves(u_int8_t[8][8], u_int8_t, move, int, int, int[32][2]);
int pawn_captures(u_int8_t[8][8], u_int8_t, move, int, int, int[32][2]);

#endif