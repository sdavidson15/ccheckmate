#include "../include/special_moves.h"

const int8_t white_castle_king_side = 0b0001;
const int8_t white_castle_queen_side = 0b0010;
const int8_t black_castle_king_side = 0b0100;
const int8_t black_castle_queen_side = 0b1000;
const int8_t all_castles_possible = 0b1111;

int handle_pawn_conversion(u_int8_t board[8][8], move mv, turn color)
{
    // TODO: let the user pick
    u_int8_t p = board[mv.dest_r][mv.dest_c];
    if (!is_pawn(p))
        return 0;

    if (color == WHITE && is_white(p) && mv.dest_r == 0)
        insert_piece(board, new_piece(queen, color), mv.dest_r, mv.dest_c);
    else if (color == BLACK && is_black(p) && mv.dest_r == 7)
        insert_piece(board, new_piece(queen, color), mv.dest_r, mv.dest_c);

    return 0;
}

int handle_en_passant(u_int8_t board[8][8], move mv)
{
    u_int8_t p = remove_piece(board, mv.src_r, mv.src_c);
    insert_piece(board, p, mv.dest_r, mv.dest_c);
    remove_piece(board, mv.src_r, mv.dest_c);
    return 0;
}

int can_en_passant(u_int8_t board[8][8], u_int8_t p, move prev, int row, int col)
{
    u_int8_t other = board[row][col];
    return is_pawn(other) &&
           is_opponent(p, other) &&
           prev.dest_r == row &&
           prev.dest_c == col &&
           abs(prev.src_r - prev.dest_r) == 2;
}

int is_en_passant(u_int8_t board[8][8], move mv)
{
    u_int8_t p = board[mv.src_r][mv.src_c];
    return is_pawn(p) && is_empty(board[mv.dest_r][mv.dest_c]) && mv.dest_c != mv.src_c;
}

int handle_castle(u_int8_t board[8][8], int8_t *castle_cfg, turn trn, special type)
{
    if (type != KING_CASTLE && type != QUEEN_CASTLE)
        return 0;

    // move the king
    int kng_row = ((trn == WHITE) ? 7 : 0);
    int kng_col = 4;
    int kng_new_col = kng_col + ((type == KING_CASTLE) ? 2 : -2);
    u_int8_t kng = remove_piece(board, kng_row, kng_col);
    insert_piece(board, kng, kng_row, kng_new_col);

    // move the rook
    int rk_row = kng_row;
    int rk_col = (type == KING_CASTLE) ? 7 : 0;
    int rk_new_col = rk_col + ((type == KING_CASTLE) ? -2 : 3);
    u_int8_t rk = remove_piece(board, rk_row, rk_col);
    insert_piece(board, rk, rk_row, rk_new_col);

    // update the castle config
    move mv = {0, 0, 0, 0, type, 0};
    update_castle_config(kng, mv, castle_cfg);

    return 0;
}

int update_castle_config(u_int8_t p, move mv, int8_t *castle_cfg)
{
    if (is_empty(p))
        return 0;

    turn color = (is_white(p)) ? WHITE : BLACK;
    if (mv.sp == KING_CASTLE || mv.sp == QUEEN_CASTLE || is_king(p))
    {
        disable_king_castle(castle_cfg, color);
        disable_queen_castle(castle_cfg, color);
    }

    if (is_rook(p) && mv.src_c == 7)
        disable_king_castle(castle_cfg, color);
    else if (is_rook(p) && mv.src_c == 0)
        disable_queen_castle(castle_cfg, color);

    return 0;
}

int can_castle_king_side(turn color, int8_t castle_cfg)
{
    return (color == WHITE) ? castle_cfg & white_castle_king_side : castle_cfg & black_castle_king_side;
}

int can_castle_queen_side(turn color, int8_t castle_cfg)
{
    return (color == WHITE) ? castle_cfg & white_castle_queen_side : castle_cfg & black_castle_queen_side;
}

int disable_king_castle(int8_t *castle_cfg, turn color)
{
    if (color == WHITE)
        *castle_cfg = *castle_cfg & 0b1110;
    else
        *castle_cfg = *castle_cfg & 0b1011;

    return 0;
}

int disable_queen_castle(int8_t *castle_cfg, turn color)
{
    if (color == WHITE)
        *castle_cfg = *castle_cfg & 0b1101;
    else
        *castle_cfg = *castle_cfg & 0b0111;

    return 0;
}