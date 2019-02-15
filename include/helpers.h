#ifndef HELPERS_H
#define HELPERS_H

#include <wchar.h>

extern const wchar_t kw;
extern const wchar_t qw;
extern const wchar_t rw;
extern const wchar_t bw;
extern const wchar_t nw;
extern const wchar_t pw;
extern const wchar_t kb;
extern const wchar_t qb;
extern const wchar_t rb;
extern const wchar_t bb;
extern const wchar_t nb;
extern const wchar_t pb;

int outOfBounds(int, int);
int isTeammate(wchar_t, wchar_t);
int isOpponent(wchar_t, wchar_t);
int isBlack(wchar_t);
int isWhite(wchar_t);
int isPawn(wchar_t);
int isEmpty(wchar_t);

#endif