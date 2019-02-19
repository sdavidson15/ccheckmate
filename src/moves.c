#include "../include/moves.h"

int perform_move(u_int8_t board[8][8], move mv, int8_t *castle_cfg, turn color)
{
    if (mv.sp == KING_CASTLE || mv.sp == QUEEN_CASTLE)
        handle_castle(board, castle_cfg, color, mv.sp);
    else if (mv.sp == EN_PASSANT)
        handle_en_passant(board, mv);
    else
    {
        u_int8_t p = remove_piece(board, mv.src_r, mv.src_c);
        insert_piece(board, p, mv.dest_r, mv.dest_c);

        handle_pawn_conversion(board, mv, color);
        update_castle_config(board[mv.dest_r][mv.dest_c], mv, castle_cfg);
    }

    return 0;
}

int possible_moves(u_int8_t board[8][8], u_int8_t p, int row, int col, move prev, int moves[32][2])
{
    if (is_empty(p))
        return 0;

    int num_moves = 0;
    if (is_king(p))
        return king_moves(board, p, row, col, moves);
    else if (is_queen(p))
        return queen_moves(board, p, row, col, moves);
    else if (is_rook(p))
        return rook_moves(board, p, row, col, moves);
    else if (is_bishop(p))
        return bishop_moves(board, p, row, col, moves);
    else if (is_knight(p))
        return knight_moves(board, p, row, col, moves);
    else if (is_pawn(p))
        return pawn_moves(board, p, prev, row, col, moves);

    return num_moves;
}

int king_moves(u_int8_t board[8][8], u_int8_t p, int row, int col, int moves[32][2])
{
    int num_moves = 0;
    for (int r = row - 1; r < row + 2; r++)
    {
        for (int c = col - 1; c < col + 2; c++)
        {
            if ((row == r && col == c) || out_of_bounds(r, c) || is_teammate(p, board[r][c]))
                continue;

            moves[num_moves][0] = r;
            moves[num_moves][1] = c;
            num_moves++;
        }
    }

    return num_moves;
}

int queen_moves(u_int8_t board[8][8], u_int8_t p, int row, int col, int moves[32][2])
{
    int num_r_moves = rook_moves(board, p, row, col, moves);

    int b_moves[32][2];
    int num_b_moves = bishop_moves(board, p, row, col, b_moves);
    for (int i = 0; i < num_b_moves; i++)
    {
        moves[num_r_moves + i][0] = b_moves[i][0];
        moves[num_r_moves + i][1] = b_moves[i][1];
    }

    return num_r_moves + num_b_moves;
}

int rook_moves(u_int8_t board[8][8], u_int8_t p, int row, int col, int moves[32][2])
{
    u_int8_t other;
    int num_moves = 0;
    for (int r = row + 1; r < 8; r++)
    {
        other = board[r][col];
        if (out_of_bounds(r, col) || is_teammate(p, other))
            break;

        moves[num_moves][0] = r;
        moves[num_moves][1] = col;
        num_moves++;

        if (!is_empty(other))
            break;
    }

    for (int r = row - 1; r >= 0; r--)
    {
        other = board[r][col];
        if (out_of_bounds(r, col) || is_teammate(p, other))
            break;

        moves[num_moves][0] = r;
        moves[num_moves][1] = col;
        num_moves++;

        if (!is_empty(other))
            break;
    }

    for (int c = col + 1; c < 8; c++)
    {
        other = board[row][c];
        if (out_of_bounds(row, c) || is_teammate(p, other))
            break;

        moves[num_moves][0] = row;
        moves[num_moves][1] = c;
        num_moves++;

        if (!is_empty(other))
            break;
    }

    for (int c = col - 1; c >= 0; c--)
    {
        other = board[row][c];
        if (out_of_bounds(row, c) || is_teammate(p, other))
            break;

        moves[num_moves][0] = row;
        moves[num_moves][1] = c;
        num_moves++;

        if (!is_empty(other))
            break;
    }

    return num_moves;
}

int bishop_moves(u_int8_t board[8][8], u_int8_t p, int row, int col, int moves[32][2])
{
    u_int8_t other;
    int num_moves = 0;
    int r = row + 1, c = col + 1;
    while (r < 8 && c < 8)
    {
        other = board[r][c];
        if (out_of_bounds(r, c) || is_teammate(p, other))
            break;

        moves[num_moves][0] = r;
        moves[num_moves][1] = c;
        num_moves++;

        if (!is_empty(other))
            break;

        r++, c++;
    }

    r = row - 1, c = col - 1;
    while (r >= 0 && c >= 0)
    {
        other = board[r][c];
        if (out_of_bounds(r, c) || is_teammate(p, other))
            break;

        moves[num_moves][0] = r;
        moves[num_moves][1] = c;
        num_moves++;

        if (!is_empty(other))
            break;

        r--, c--;
    }

    r = row + 1, c = col - 1;
    while (r < 8 && c >= 0)
    {
        other = board[r][c];
        if (out_of_bounds(r, c) || is_teammate(p, other))
            break;

        moves[num_moves][0] = r;
        moves[num_moves][1] = c;
        num_moves++;

        if (!is_empty(other))
            break;

        r++, c--;
    }

    r = row - 1, c = col + 1;
    while (r >= 0 && c < 8)
    {
        other = board[r][c];
        if (out_of_bounds(r, c) || is_teammate(p, other))
            break;

        moves[num_moves][0] = r;
        moves[num_moves][1] = c;
        num_moves++;

        if (!is_empty(other))
            break;

        r--, c++;
    }

    return num_moves;
}

int knight_moves(u_int8_t board[8][8], u_int8_t p, int row, int col, int moves[32][2])
{
    u_int8_t other;
    int num_moves = 0;

    int temp[8][2] = {
        {row - 2, col - 1},
        {row - 2, col + 1},
        {row - 1, col + 2},
        {row + 1, col + 2},
        {row + 2, col + 1},
        {row + 2, col - 1},
        {row + 1, col - 2},
        {row - 1, col - 2}};

    for (int i = 0; i < 8; i++)
    {
        int r = temp[i][0], c = temp[i][1];
        other = board[r][c];
        if (out_of_bounds(r, c) || is_teammate(p, other))
            continue;

        moves[num_moves][0] = r;
        moves[num_moves][1] = c;
        num_moves++;
    }

    return num_moves;
}

int pawn_moves(u_int8_t board[8][8], u_int8_t p, move prev, int row, int col, int moves[32][2])
{
    int num_moves = 0;

    int start_row = (is_black(p)) ? 1 : 6;
    int dir = (is_black(p)) ? 1 : -1;
    int next_row = row + dir;
    int jump_row = next_row + dir;

    int r = next_row, c = col;
    if (!out_of_bounds(r, c) && is_empty(board[r][c]))
    {
        moves[num_moves][0] = r;
        moves[num_moves][1] = c;
        num_moves++;
    }

    r = jump_row, c = col;
    if (row == start_row && is_empty(board[jump_row][col]) && is_empty(board[next_row][col]))
    {
        moves[num_moves][0] = r;
        moves[num_moves][1] = c;
        num_moves++;
    }

    int captures[32][2];
    int num_captures = pawn_captures(board, p, prev, row, col, captures);
    for (int i = 0; i < num_captures; i++)
    {
        moves[num_moves][0] = captures[i][0];
        moves[num_moves][1] = captures[i][1];
        num_moves++;
    }

    return num_moves;
}

int pawn_captures(u_int8_t board[8][8], u_int8_t p, move prev, int row, int col, int captures[32][2])
{
    int num_moves = 0;

    int dir = is_black(p) ? 1 : -1;
    int r = row + dir, c = col - 1;
    if (!out_of_bounds(r, c) && !is_teammate(p, board[r][c]) &&
        (is_opponent(p, board[r][c]) || can_en_passant(board, p, prev, row, c)))
    {
        captures[num_moves][0] = r;
        captures[num_moves][1] = c;
        num_moves++;
    }

    c = col + 1;
    if (!out_of_bounds(r, c) && !is_teammate(p, board[r][c]) &&
        (is_opponent(p, board[r][c]) || can_en_passant(board, p, prev, row, c)))
    {
        captures[num_moves][0] = r;
        captures[num_moves][1] = c;
        num_moves++;
    }

    return num_moves;
}