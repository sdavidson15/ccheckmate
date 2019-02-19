#include "../include/helpers.h"

const u_int8_t king = 0x01;
const u_int8_t queen = 0x92;
const u_int8_t rook = 0x53;
const u_int8_t bishop = 0x34;
const u_int8_t knight = 0x35;
const u_int8_t pawn = 0x16;

int insert_piece(u_int8_t board[8][8], u_int8_t p, int row, int col)
{
    board[row][col] = p;
    return 0;
}

u_int8_t remove_piece(u_int8_t board[8][8], int row, int col)
{
    u_int8_t p = board[row][col];
    board[row][col] = 0;
    return p;
}

u_int8_t new_piece(u_int8_t p, int color)
{
    if (color == BLACK)
        return p | 0b1000;

    return p;
}

wchar_t printable_piece(u_int8_t p)
{
    if (is_empty(p))
        return ' ';

    wchar_t printable = get_piece_code(p) + 0x2653;
    if (is_black(p))
        return printable + 6;

    return printable;
}

int out_of_bounds(int row, int col)
{
    return row < 0 || row > 7 || col < 0 || col > 7;
}

u_int8_t get_piece_code(u_int8_t p)
{
    return p & 0x07;
}

u_int8_t get_piece_value(u_int8_t p)
{
    return p & 0xF0;
}

int is_empty(u_int8_t p)
{
    return !get_piece_code(p);
}

int is_white(u_int8_t p)
{
    return !is_empty(p) && !(p & 0b1000);
}

int is_black(u_int8_t p)
{
    return !is_empty(p) && (p & 0b1000);
}

int is_opponent(u_int8_t p, u_int8_t o)
{
    return !is_empty(p) && !is_empty(o) && is_white(p) != is_white(o);
}

int is_teammate(u_int8_t p, u_int8_t o)
{
    return !is_empty(p) && !is_empty(o) && is_white(p) == is_white(o);
}

int is_king(u_int8_t p)
{
    return get_piece_code(p) == (king & 0x0F);
}

int is_queen(u_int8_t p)
{
    return get_piece_code(p) == (queen & 0x0F);
}

int is_rook(u_int8_t p)
{
    return get_piece_code(p) == (rook & 0x0F);
}

int is_bishop(u_int8_t p)
{
    return get_piece_code(p) == (bishop & 0x0F);
}

int is_knight(u_int8_t p)
{
    return get_piece_code(p) == (knight & 0x0F);
}

int is_pawn(u_int8_t p)
{
    return get_piece_code(p) == (pawn & 0x0F);
}