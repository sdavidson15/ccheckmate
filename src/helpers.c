#include "../include/helpers.h"

const wchar_t kw = 0x2654;
const wchar_t qw = 0x2655;
const wchar_t rw = 0x2656;
const wchar_t bw = 0x2657;
const wchar_t nw = 0x2658;
const wchar_t pw = 0x2659;
const wchar_t kb = 0x265A;
const wchar_t qb = 0x265B;
const wchar_t rb = 0x265C;
const wchar_t bb = 0x265D;
const wchar_t nb = 0x265E;
const wchar_t pb = 0x265F;

int outOfBounds(int row, int col)
{
    return row < 0 || row > 7 || col < 0 || col > 7;
}

int isOpponent(wchar_t me, wchar_t other)
{
    return me && other && !isTeammate(me, other);
}

int isTeammate(wchar_t me, wchar_t other)
{
    return me && other && (isWhite(me) == isWhite(other));
}

int isBlack(wchar_t p)
{
    return p && !isWhite(p);
}

int isWhite(wchar_t p)
{
    return p && p <= pw;
}

int isPawn(wchar_t p)
{
    return p == pw || p == pb;
}

int isEmpty(wchar_t p)
{
    return !p;
}