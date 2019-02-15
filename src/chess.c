#include "../include/chess.h"

int main()
{
    setlocale(LC_CTYPE, "");
    wchar_t board[8][8] = {
        {rb, nb, bb, qb, kb, bb, nb, rb},
        {pb, pb, pb, pb, pb, pb, pb, pb},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {pw, pw, pw, pw, pw, pw, pw, pw},
        {rw, nw, bw, qw, kw, bw, nw, rw}};

    draw(board);
    play(board);

    return 0;
}

void draw(wchar_t board[8][8])
{
    printf("\n");
    for (int r = 0; r < 8; r++)
    {
        printf("   +---+---+---+---+---+---+---+---+ \n");
        printf(" %d ", 8 - r);
        for (int c = 0; c < 8; c++)
        {
            printf("| ");

            wchar_t p = board[r][c];
            if (p > pw)
            {
                wprintf(ANSI_BLUE L"%lc " ANSI_RESET, p);
            }
            else if (p)
            {
                wprintf(ANSI_YELLOW L"%lc " ANSI_RESET, p);
            }
            else
            {
                printf("  ");
            }

            if (c == 8 - 1)
                printf("|\n");
        }

        if (r == 8 - 1)
            printf("   +---+---+---+---+---+---+---+---+ \n");
    }
    printf("     a   b   c   d   e   f   g   h\n");
}

void play(wchar_t board[8][8])
{
    turn trn = WHITE;
    int8_t castleCfg = allCastlesPossible;
    move prev = {0, 0, 0, 0, FIRST_MOVE, 1};

    while (1)
    {
        char input[inputLen];
        printf("\nEnter a move: ");
        scanf("%s", input);
        if (input[0] == 'q')
            break;

        move mv = parse(board, input);
        mv = validate(board, mv, prev, castleCfg, trn);
        if (!mv.isValid)
        {
            printf("Invalid move\n");
            continue;
        }

        switch (mv.sp)
        {
        case KING_CASTLE:
            castleCfg = handleKingSideCastle(board, castleCfg, trn);
            break;
        case QUEEN_CASTLE:
            castleCfg = handleQueenSideCastle(board, castleCfg, trn);
            break;
        case EN_PASSANT:
            handleEnPassant(board, mv);
            break;
        default:
            board[mv.destR][mv.destC] = board[mv.srcR][mv.srcC];
            board[mv.srcR][mv.srcC] = 0;

            handlePawnConversion(board, mv, trn);
            castleCfg = updateCastleConfig(board[mv.srcR][mv.srcC], mv, castleCfg);
        }

        prev = mv;
        trn = (trn == WHITE) ? BLACK : WHITE;
        draw(board);
    }
}

move parse(wchar_t board[8][8], char input[inputLen])
{
    if (strlen(input) == 2 && !strcmp(input, "OO"))
    {
        move mv = {0, 0, 0, 0, KING_CASTLE, 1};
        return mv;
    }

    if (strlen(input) == 3 && !strcmp(input, "OOO"))
    {
        move mv = {0, 0, 0, 0, QUEEN_CASTLE, 1};
        return mv;
    }

    int srcCol = input[0] - 97, srcRow = 8 - atoi(&input[1]);
    int destCol = input[2] - 97, destRow = 8 - atoi(&input[3]);
    move mv = {srcRow, srcCol, destRow, destCol, 0, 1};
    return mv;
}

move validate(wchar_t board[8][8], move mv, move prev, int8_t castleCfg, turn trn)
{
    move validated = {mv.srcR, mv.srcC, mv.destR, mv.destC, mv.sp, 0};
    if (mv.sp == KING_CASTLE)
    {
        wchar_t king = (trn == WHITE) ? kw : kb;
        int kingR = (trn == WHITE) ? 7 : 0, kingC = 4;
        if (!canKingSideCastle(king, castleCfg) || !isEmpty(board[kingR][kingC + 1]) || !isEmpty(board[kingR][kingC + 2]))
        {
            printf("Cannot perform king side castle. ");
            return validated;
        }
        else
        {
            validated.isValid = 1;
            return validated;
        }
    }

    if (mv.sp == QUEEN_CASTLE)
    {
        wchar_t king = (trn == WHITE) ? kw : kb;
        int kingR = (trn == WHITE) ? 7 : 0, kingC = 4;
        if (!canQueenSideCastle(king, castleCfg) || !isEmpty(board[kingR][kingC - 1]) || !isEmpty(board[kingR][kingC - 2]))
        {
            printf("Cannot perform queen side castle. ");
            return validated;
        }
        else
        {
            validated.isValid = 1;
            return validated;
        }
    }

    if (outOfBounds(mv.srcR, mv.srcC))
    {
        printf("Cannot move from out of bounds. ");
        return validated;
    }

    if (outOfBounds(mv.destR, mv.destC))
    {
        printf("Cannot move to out of bounds. ");
        return validated;
    }

    wchar_t p = board[mv.srcR][mv.srcC];
    if (isEmpty(p))
    {
        printf("No piece at source. ");
        return validated;
    }

    if ((trn == WHITE && isBlack(p)) || (trn == BLACK && isWhite(p)))
    {
        printf("Cannot move opponent's piece. ");
        return validated;
    }

    int moves[32][2];
    int numMoves = possibleMoves(board, mv.srcR, mv.srcC, prev, moves);
    for (int i = 0; i < numMoves; i++)
    {
        int mr = moves[i][0], mc = moves[i][1];
        if (mr == mv.destR && mc == mv.destC)
        {
            if (isEnPassant(board, mv))
                validated.sp = EN_PASSANT;

            validated.isValid = 1;
            break;
        }
    }

    return validated;
}
