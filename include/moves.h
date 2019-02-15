#ifndef MOVES_H
#define MOVES_H

#include "special_moves.h"

int possibleMoves(wchar_t[8][8], int, int, move, int[32][2]);
int kingMoves(wchar_t[8][8], wchar_t, int, int, int[32][2]);
int rookMoves(wchar_t[8][8], wchar_t, int, int, int[32][2]);
int bishopMoves(wchar_t[8][8], wchar_t, int, int, int[32][2]);
int knightMoves(wchar_t[8][8], wchar_t, int, int, int[32][2]);
int pawnMoves(wchar_t[8][8], wchar_t, move, int, int, int[32][2]);

#endif