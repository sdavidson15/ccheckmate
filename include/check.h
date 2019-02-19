#ifndef CHECK_H
#define CHECK_H

#include "moves.h"

int position_is_check(u_int8_t[8][8], int, int, turn);
move validate_move(u_int8_t[8][8], move, move, int8_t, turn);

#endif