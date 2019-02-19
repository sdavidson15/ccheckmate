#ifndef CHESS_H
#define CHESS_H

#include <locale.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <uuid/uuid.h>

#include "check.h"

#define ANSI_BLUE "\x1b[34m"
#define ANSI_YELLOW "\x1b[33m"
#define ANSI_RESET "\x1b[0m"

const int buffer_size = 1024;
const int input_len = 5;

int draw(u_int8_t[8][8]);
int play(u_int8_t[8][8]);
move parse(u_int8_t[8][8], char[input_len]);

int buffer_move(char[buffer_size], char[input_len]);
int write_buffer(char *, char *);

#endif