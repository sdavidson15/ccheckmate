#ifndef HELPERS_H
#define HELPERS_H

#include <wchar.h>

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
    int src_r;
    int src_c;
    int dest_r;
    int dest_c;
    special sp;
    int is_valid;
} move;

extern const u_int8_t king;
extern const u_int8_t queen;
extern const u_int8_t rook;
extern const u_int8_t bishop;
extern const u_int8_t knight;
extern const u_int8_t pawn;

int insert_piece(u_int8_t[8][8], u_int8_t, int, int);
u_int8_t remove_piece(u_int8_t[8][8], int, int);
u_int8_t new_piece(u_int8_t, int color);
wchar_t printable_piece(u_int8_t);
int out_of_bounds(int, int);
u_int8_t get_piece_code(u_int8_t);
u_int8_t get_piece_value(u_int8_t);
int is_empty(u_int8_t);
int is_white(u_int8_t);
int is_black(u_int8_t);
int is_opponent(u_int8_t, u_int8_t);
int is_teammate(u_int8_t, u_int8_t);
int is_king(u_int8_t);
int is_queen(u_int8_t);
int is_rook(u_int8_t);
int is_bishop(u_int8_t);
int is_knight(u_int8_t);
int is_pawn(u_int8_t);

#endif