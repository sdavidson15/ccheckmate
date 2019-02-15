#ifndef SPECIAL_MOVES_H
#define SPECIAL_MOVES_H

#include <stdlib.h>

#include "helpers.h"

typedef enum
{
    WHITE,
    BLACK
} turn;

typedef enum
{
    FIRST_MOVE = 1,
    KING_CASTLE,
    QUEEN_CASTLE,
    EN_PASSANT
} special;

typedef struct
{
    int srcR;
    int srcC;
    int destR;
    int destC;
    special sp;
    int isValid;
} move;

extern const int8_t whiteCastleKingSide;
extern const int8_t whiteCastleQueenSide;
extern const int8_t blackCastleKingSide;
extern const int8_t blackCastleQueenSide;
extern const int8_t allCastlesPossible;

void handlePawnConversion(wchar_t[8][8], move, turn);
void handleEnPassant(wchar_t[8][8], move);
int canEnPassant(wchar_t[8][8], wchar_t, move, int, int);
int isEnPassant(wchar_t[8][8], move);
int8_t handleKingSideCastle(wchar_t[8][8], int8_t, turn);
int8_t handleQueenSideCastle(wchar_t[8][8], int8_t, turn);
int8_t updateCastleConfig(wchar_t, move, int8_t);
int canKingSideCastle(wchar_t, int8_t);
int canQueenSideCastle(wchar_t, int8_t);

#endif