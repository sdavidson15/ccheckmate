#include "../include/check.h"

int position_is_check(u_int8_t board[8][8], int row, int col, turn color)
{
    int moves[32][2], r, c;
    move mv;
    u_int8_t p = new_piece(queen, color), other;
    int num_moves = possible_moves(board, p, row, col, mv, moves);
    for (int i = 0; i < num_moves; i++)
    {
        r = moves[i][0], c = moves[i][1];
        other = board[r][c];
        if (is_opponent(p, other))
        {
            if (is_queen(other) || is_rook(other) || is_bishop(other))
                return 1;
            else if (is_pawn(other) && col != c)
            {
                int dir = (color == BLACK) ? 1 : -1;
                if (row == r + dir)
                    return 1;
            }
        }
    }

    p = new_piece(knight, color);
    num_moves = possible_moves(board, p, row, col, mv, moves);
    for (int i = 0; i < num_moves; i++)
    {
        r = moves[i][0], c = moves[i][1];
        other = board[r][c];
        if (is_opponent(p, other) && is_knight(other))
            return 1;
    }

    return 0;
}

move validate_move(u_int8_t board[8][8], move mv, move prev, int8_t castle_cfg, turn trn)
{
    move validated = {mv.src_r, mv.src_c, mv.dest_r, mv.dest_c, mv.sp, 0};
    if (mv.sp == KING_CASTLE)
    {
        int kng_row = (trn == WHITE) ? 7 : 0;
        int kng_col = 4;
        u_int8_t kng = new_piece(king, trn);
        if (!can_castle_king_side(trn, castle_cfg) ||
            !is_empty(board[kng_row][kng_col + 1]) || !is_empty(board[kng_row][kng_col + 2]) ||
            position_is_check(board, kng_row, kng_col + 1, trn) || position_is_check(board, kng_row, kng_col + 2, trn))
        {
            printf("Cannot perform king side castle. ");
            return validated;
        }
        else
        {
            validated.is_valid = 1;
            return validated;
        }
    }

    if (mv.sp == QUEEN_CASTLE)
    {
        int kng_row = (trn == WHITE) ? 7 : 0;
        int kng_col = 4;
        u_int8_t kng = new_piece(king, trn);
        if (!can_castle_queen_side(trn, castle_cfg) ||
            !is_empty(board[kng_row][kng_col - 1]) || !is_empty(board[kng_row][kng_col - 2]) ||
            position_is_check(board, kng_row, kng_col - 1, trn) || position_is_check(board, kng_row, kng_col - 2, trn))
        {
            printf("Cannot perform queen side castle. ");
            return validated;
        }
        else
        {
            validated.is_valid = 1;
            return validated;
        }
    }

    if (out_of_bounds(mv.src_r, mv.src_c))
    {
        printf("Cannot move from out of bounds. ");
        return validated;
    }

    if (out_of_bounds(mv.dest_r, mv.dest_c))
    {
        printf("Cannot move to out of bounds. ");
        return validated;
    }

    u_int8_t p = board[mv.src_r][mv.src_c];
    if (is_empty(p))
    {
        printf("No piece at source. ");
        return validated;
    }

    if ((trn == WHITE && is_black(p)) || (trn == BLACK && is_white(p)))
    {
        printf("Cannot move opponent's piece. ");
        return validated;
    }

    int moves[32][2];
    int num_moves = possible_moves(board, board[mv.src_r][mv.src_c], mv.src_r, mv.src_c, prev, moves);
    for (int i = 0; i < num_moves; i++)
    {
        int mr = moves[i][0], mc = moves[i][1];
        if (mr == mv.dest_r && mc == mv.dest_c)
        {
            if (is_en_passant(board, mv))
                validated.sp = EN_PASSANT;

            validated.is_valid = 1;
            break;
        }
    }

    if (validated.is_valid)
    {
        int kng_row, kng_col;
        u_int8_t projected[8][8];
        for (int r = 0; r < 8; r++)
        {
            for (int c = 0; c < 8; c++)
            {
                projected[r][c] = board[r][c];
                if (is_king(board[r][c]) && is_teammate(p, board[r][c]))
                    kng_row = r, kng_col = c;
            }
        }

        if (is_king(p))
            kng_row = mv.dest_r, kng_col = mv.dest_c;

        perform_move(projected, mv, &castle_cfg, trn);
        if (position_is_check(projected, kng_row, kng_col, trn))
        {
            printf("Cannot leave your king in check. ");
            validated.is_valid = 0;
        }
    }

    return validated;
}
