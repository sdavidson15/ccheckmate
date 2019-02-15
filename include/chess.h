#ifndef CHESS_H
#define CHESS_H

#include <locale.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "moves.h"

#define ANSI_BLUE "\x1b[34m"
#define ANSI_YELLOW "\x1b[33m"
#define ANSI_RESET "\x1b[0m"

const int inputLen = 5;

void draw(wchar_t[8][8]);
void play(wchar_t[8][8]);

move parse(wchar_t[8][8], char[inputLen]);
move validate(wchar_t[8][8], move, move, int8_t, turn);

#endif