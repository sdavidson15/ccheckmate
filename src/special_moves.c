#include "../include/special_moves.h"

const int8_t whiteCastleKingSide = 1;
const int8_t whiteCastleQueenSide = 2;
const int8_t blackCastleKingSide = 4;
const int8_t blackCastleQueenSide = 8;
const int8_t allCastlesPossible = 15;

void handlePawnConversion(wchar_t board[8][8], move mv, turn trn)
{
    // TODO: let the user pick
    wchar_t convertTo = (trn == WHITE) ? qw : qb;
    if (trn == WHITE && board[mv.destR][mv.destC] == pw && mv.destR == 0)
        board[mv.destR][mv.destC] = convertTo;
    if (trn == BLACK && board[mv.destR][mv.destC] == pb && mv.destR == 7)
        board[mv.destR][mv.destC] = convertTo;
}

void handleEnPassant(wchar_t board[8][8], move mv)
{
    board[mv.destR][mv.destC] = board[mv.srcR][mv.srcC];
    board[mv.srcR][mv.srcC] = 0;
    board[mv.srcR][mv.destC] = 0;
}

int canEnPassant(wchar_t board[8][8], wchar_t pawn, move prev, int row, int col)
{
    wchar_t other = board[row][col];
    return isPawn(other) &&
           isOpponent(pawn, other) &&
           prev.destR == row &&
           prev.destC == col &&
           abs(prev.srcR - prev.destR) == 2;
}

int isEnPassant(wchar_t board[8][8], move mv)
{
    wchar_t p = board[mv.srcR][mv.srcC];
    return isPawn(p) && isEmpty(board[mv.destR][mv.destC]) && mv.destC != mv.srcC;
}

int8_t handleKingSideCastle(wchar_t board[8][8], int8_t castleCfg, turn trn)
{
    // move the king
    wchar_t king = (trn == WHITE) ? kw : kb;
    int kingR = ((trn == WHITE) ? 7 : 0), kingC = 4;
    board[kingR][kingC + 2] = king;
    board[kingR][kingC] = 0;

    // move the rook
    wchar_t rook = (trn == WHITE) ? rw : rb;
    int rookR = kingR, rookC = 7;
    board[rookR][rookC - 2] = rook;
    board[rookR][rookC] = 0;

    // update the castle config
    castleCfg -= (trn == WHITE) ? whiteCastleKingSide : blackCastleKingSide;
    if (canQueenSideCastle(king, castleCfg))
        castleCfg -= (trn == WHITE) ? whiteCastleQueenSide : blackCastleQueenSide;

    return castleCfg;
}

int8_t handleQueenSideCastle(wchar_t board[8][8], int8_t castleCfg, turn trn)
{
    // move the king
    wchar_t king = (trn == WHITE) ? kw : kb;
    int kingR = ((trn == WHITE) ? 7 : 0), kingC = 4;
    board[kingR][kingC - 2] = king;
    board[kingR][kingC] = 0;

    // move the rook
    wchar_t rook = (trn == WHITE) ? rw : rb;
    int rookR = kingR, rookC = 0;
    board[rookR][rookC + 3] = rook;
    board[rookR][rookC] = 0;

    // update the castle config
    castleCfg -= (trn == WHITE) ? whiteCastleQueenSide : blackCastleQueenSide;
    if (canKingSideCastle(king, castleCfg))
        castleCfg -= (trn == WHITE) ? whiteCastleKingSide : blackCastleKingSide;

    return castleCfg;
}

int8_t updateCastleConfig(wchar_t p, move mv, int8_t castleCfg)
{
    if (p == kw && canKingSideCastle(p, castleCfg))
        castleCfg -= whiteCastleKingSide;
    else if (p == kw && canQueenSideCastle(p, castleCfg))
        castleCfg -= whiteCastleQueenSide;
    else if (p == kb && canKingSideCastle(p, castleCfg))
        castleCfg -= blackCastleKingSide;
    else if (p == kb && canQueenSideCastle(p, castleCfg))
        castleCfg -= blackCastleQueenSide;
    else if (p == rw && mv.srcC == 8 && canKingSideCastle(kw, castleCfg))
        castleCfg -= whiteCastleKingSide;
    else if (p == rw && mv.srcC == 0 && canQueenSideCastle(kw, castleCfg))
        castleCfg -= whiteCastleQueenSide;
    else if (p == rb && mv.srcC == 8 && canKingSideCastle(kb, castleCfg))
        castleCfg -= blackCastleKingSide;
    else if (p == rb && mv.srcC == 0 && canQueenSideCastle(kb, castleCfg))
        castleCfg -= blackCastleQueenSide;

    return castleCfg;
}

int canKingSideCastle(wchar_t king, int8_t castleCfg)
{
    if (isWhite(king))
        return castleCfg & whiteCastleKingSide;

    return castleCfg & blackCastleKingSide;
}

int canQueenSideCastle(wchar_t king, int8_t castleCfg)
{
    if (isWhite(king))
        return castleCfg & whiteCastleQueenSide;

    return castleCfg & blackCastleQueenSide;
}