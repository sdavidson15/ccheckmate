#include "../include/chess_example.h"

char fuuid[7];

int main()
{
    setlocale(LC_CTYPE, "");

    u_int8_t board[8][8] = {
        {0x5B, 0x3D, 0x3C, 0x0A, 0x99, 0x3C, 0x3D, 0x5B},
        {0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E},
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
        {0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16},
        {0x53, 0x35, 0x34, 0x92, 0x01, 0x34, 0x35, 0x53}};

    draw(board);
    play(board);

    return 0;
}

int draw(u_int8_t board[8][8])
{
    printf("\n");
    for (int r = 0; r < 8; r++)
    {
        printf("   +---+---+---+---+---+---+---+---+ \n");
        printf(" %d ", 8 - r);
        for (int c = 0; c < 8; c++)
        {
            printf("| ");

            u_int8_t p = board[r][c];
            wchar_t pp = printable_piece(p);
            if (is_black(p))
                wprintf(ANSI_BLUE L"%lc " ANSI_RESET, pp);
            else if (is_white(p))
                wprintf(ANSI_YELLOW L"%lc " ANSI_RESET, pp);
            else
                printf("  ");

            if (c == 7)
                printf("|\n");
        }

        if (r == 7)
            printf("   +---+---+---+---+---+---+---+---+ \n");
    }
    printf("     a   b   c   d   e   f   g   h\n");

    return 0;
}

int play(u_int8_t board[8][8])
{
    char buffer[buffer_size] = "";
    //write_buffer("Start\n", "w");

    turn trn = WHITE;
    int8_t castle_cfg = all_castles_possible;
    int8_t *castle_cfg_ptr = &castle_cfg;
    move prev = {0, 0, 0, 0, FIRST_MOVE, 1};

    while (1)
    {
        char input[input_len];
        printf("\nEnter a move: ");
        scanf("%s", input);
        if (input[0] == 'q')
        {
            //write_buffer(buffer, "a");
            printf("\n");
            break;
        }

        move mv = parse(board, input);
        mv = validate_move(board, mv, prev, castle_cfg, trn);
        if (!mv.is_valid)
        {
            printf("Invalid move\n");
            continue;
        }
        perform_move(board, mv, castle_cfg_ptr, trn);
        //buffer_move(buffer, input);

        prev = mv;
        //trn = (trn == WHITE) ? BLACK : WHITE;
        draw(board);
    }

    return 0;
}

move parse(u_int8_t board[8][8], char input[input_len])
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

    int src_col = input[0] - 97, src_row = 8 - atoi(&input[1]);
    int dest_col = input[2] - 97, dest_row = 8 - atoi(&input[3]);
    move mv = {src_row, src_col, dest_row, dest_col, 0, 1};
    return mv;
}

int get_uuid(char *fuuid)
{
    if (strlen(fuuid) > 0)
        return 0;

    uuid_t binuuid;
    uuid_generate_random(binuuid);
    char *uuid = malloc(37);
    uuid_unparse(binuuid, uuid);
    char sub_uuid[7];
    memcpy(sub_uuid, uuid, 7);
    sub_uuid[6] = '\0';
    strcpy(fuuid, sub_uuid);

    return 0;
}

int get_filename(char filename[19])
{
    get_uuid(fuuid);
    strcpy(filename, "moves/moves_");
    strcat(filename, fuuid);
    strcat(filename, ".chss");

    return 0;
}

int buffer_move(char buffer[buffer_size], char move_str[input_len])
{
    if (strlen(buffer) + strlen(move_str) >= buffer_size)
    {
        printf("writing to the buffer");
        write_buffer(buffer, "a");
        strcpy(buffer, move_str);
        strcat(buffer, "\n");
        return 0;
    }

    strcat(buffer, move_str);
    strcat(buffer, "\n");
    return 0;
}

int write_buffer(char *buffer, char *mode)
{
    char filename[19];
    get_filename(filename);
    FILE *f = fopen(filename, mode);
    if (f == NULL)
    {
        printf("Failed to write to a file.\n");
        return 1;
    }

    fprintf(f, "%s", buffer);
    fclose(f);
    return 0;
}
