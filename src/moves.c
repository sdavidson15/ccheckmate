#include "../include/moves.h"

int possibleMoves(wchar_t board[8][8], int row, int col, move prev, int moves[32][2])
{
    wchar_t p = board[row][col];
    if (isEmpty(p))
        return 0;

    int numMoves = 0;
    if (p == kw || p == kb)
        return kingMoves(board, p, row, col, moves);

    if (p == rw || p == rb || p == qw || p == qb)
        numMoves += rookMoves(board, p, row, col, moves);

    if (p == bw || p == bb || p == qw || p == qb)
        return numMoves + bishopMoves(board, p, row, col, moves);

    if (p == nw || p == nb)
        return knightMoves(board, p, row, col, moves);

    if (p == pw || p == pb)
        return pawnMoves(board, p, prev, row, col, moves);

    return numMoves;
}

int kingMoves(wchar_t board[8][8], wchar_t p, int row, int col, int moves[32][2])
{
    int numMoves = 0;
    for (int r = row - 1; r < row + 2; r++)
    {
        for (int c = col - 1; c < col + 2; c++)
        {
            if ((row == r && col == c) || outOfBounds(r, c) || isTeammate(p, board[r][c]))
                continue;

            moves[numMoves][0] = r;
            moves[numMoves][1] = c;
            numMoves++;
        }
    }

    // TODO: detect checks. Store a check map, update it with each move.
    return numMoves;
}

int rookMoves(wchar_t board[8][8], wchar_t p, int row, int col, int moves[32][2])
{
    int numMoves = 0;
    for (int r = row + 1; r < 8; r++)
    {
        if (outOfBounds(r, col) || isTeammate(p, board[r][col]))
            break;

        moves[numMoves][0] = r;
        moves[numMoves][1] = col;
        numMoves++;

        if (isOpponent(p, board[r][col]))
            break;
    }

    for (int r = row - 1; r >= 0; r--)
    {
        if (outOfBounds(r, col) || isTeammate(p, board[r][col]))
            break;

        moves[numMoves][0] = r;
        moves[numMoves][1] = col;
        numMoves++;

        if (isOpponent(p, board[r][col]))
            break;
    }

    for (int c = col + 1; c < 8; c++)
    {
        if (outOfBounds(row, c) || isTeammate(p, board[row][c]))
            break;

        moves[numMoves][0] = row;
        moves[numMoves][1] = c;
        numMoves++;

        if (outOfBounds(row, c) || isOpponent(p, board[row][c]))
            break;
    }

    for (int c = col - 1; c >= 0; c--)
    {
        if (outOfBounds(row, c) || isTeammate(p, board[row][c]))
            break;

        moves[numMoves][0] = row;
        moves[numMoves][1] = c;
        numMoves++;

        if (isOpponent(p, board[row][c]))
            break;
    }

    return numMoves;
}

int bishopMoves(wchar_t board[8][8], wchar_t p, int row, int col, int moves[32][2])
{
    int numMoves = 0;

    int r = row + 1, c = col + 1;
    while (r < 8 && c < 8)
    {
        if (outOfBounds(r, c) || isTeammate(p, board[r][c]))
            break;

        moves[numMoves][0] = r;
        moves[numMoves][1] = c;
        numMoves++;

        if (isOpponent(p, board[r][c]))
            break;

        r++, c++;
    }

    r = row - 1, c = col - 1;
    while (r >= 0 && c >= 0)
    {
        if (outOfBounds(r, c) || isTeammate(p, board[r][c]))
            break;

        moves[numMoves][0] = r;
        moves[numMoves][1] = c;
        numMoves++;

        if (isOpponent(p, board[r][c]))
            break;

        r--, c--;
    }

    r = row + 1, c = col - 1;
    while (r < 8 && c >= 0)
    {
        if (outOfBounds(r, c) || isTeammate(p, board[r][c]))
            break;

        moves[numMoves][0] = r;
        moves[numMoves][1] = c;
        numMoves++;

        if (isOpponent(p, board[r][c]))
            break;

        r++, c--;
    }

    r = row - 1, c = col + 1;
    while (r >= 0 && c < 8)
    {
        if (outOfBounds(r, c) || isTeammate(p, board[r][c]))
            break;

        moves[numMoves][0] = r;
        moves[numMoves][1] = c;
        numMoves++;

        if (isOpponent(p, board[r][c]))
            break;

        r--, c++;
    }

    return numMoves;
}

int knightMoves(wchar_t board[8][8], wchar_t p, int row, int col, int moves[32][2])
{
    int numMoves = 0;

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
        if (outOfBounds(r, c) || isTeammate(p, board[r][c]))
            continue;

        moves[numMoves][0] = r;
        moves[numMoves][1] = c;
        numMoves++;
    }

    return numMoves;
}

int pawnMoves(wchar_t board[8][8], wchar_t p, move prev, int row, int col, int moves[32][2])
{
    int numMoves = 0;

    int startRow = (p == pb) ? 1 : 6;
    int dir = (p == pb) ? 1 : -1;
    int nextRow = row + dir;
    int jumpRow = nextRow + dir;

    int r = nextRow, c = col;
    if (!outOfBounds(r, c) && isEmpty(board[r][c]))
    {
        moves[numMoves][0] = r;
        moves[numMoves][1] = c;
        numMoves++;
    }

    r = jumpRow, c = col;
    if (row == startRow && isEmpty(board[jumpRow][col]) && isEmpty(board[nextRow][col]))
    {
        moves[numMoves][0] = r;
        moves[numMoves][1] = c;
        numMoves++;
    }

    r = nextRow, c = col - 1;
    if (!outOfBounds(r, c) && !isTeammate(p, board[r][c]) &&
        (isOpponent(p, board[r][c]) || canEnPassant(board, p, prev, row, c)))
    {
        moves[numMoves][0] = r;
        moves[numMoves][1] = c;
        numMoves++;
    }

    r = nextRow, c = col + 1;
    if (!outOfBounds(r, c) && !isTeammate(p, board[r][c]) &&
        (isOpponent(p, board[r][c]) || canEnPassant(board, p, prev, row, c)))
    {
        moves[numMoves][0] = r;
        moves[numMoves][1] = c;
        numMoves++;
    }

    return numMoves;
}