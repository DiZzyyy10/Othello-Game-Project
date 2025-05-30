
#include "othello.h" 

#include <stdio.h>  
#include <stdlib.h> 
#include <string.h>  


const char* piece[3] = { "  ", "Åú", "Åõ" }; 
const char* abc[8] = { "a","b","c","d","e","f","g","h" };
int turn;
int ply;
Move nextmove;
unsigned char stonenum[2];
struct Position history[SEARCH_LIMIT_DEPTH];

int evalboard[BOARDSIZE] = {
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,  99, -15,  20,   4,   4,  20, -15,  99,   0,
    0, -15, -30,  -3,  -3,  -3,  -3, -30, -15,   0,
    0,  20,  -3,   5,   1,   1,   5,  -3,  20,   0,
    0,   4,  -3,   1,   1,   1,   1,  -3,   4,   0,
    0,   4,  -3,   1,   1,   1,   1,  -3,   4,   0,
    0,  20,  -3,   5,   1,   1,   5,  -3,  20,   0,
    0, -15, -30,  -3,  -3,  -3,  -3, -30, -15,   0,
    0,  99, -15,  20,   4,   4,  20, -15,  99,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
};

unsigned char board[BOARDSIZE] = {
    N, N, N, N, N, N, N, N, N, N,
    N, 0, 0, 0, 0, 0, 0, 0, 0, N,
    N, 0, 0, 0, 0, 0, 0, 0, 0, N,
    N, 0, 0, 0, 0, 0, 0, 0, 0, N,
    N, 0, 0, 0, B, W, 0, 0, 0, N,
    N, 0, 0, 0, W, B, 0, 0, 0, N,
    N, 0, 0, 0, 0, 0, 0, 0, 0, N,
    N, 0, 0, 0, 0, 0, 0, 0, 0, N,
    N, 0, 0, 0, 0, 0, 0, 0, 0, N,
    N, N, N, N, N, N, N, N, N, N
};




void output(void) {
    int x, y;
    printf("   a  b  c  d  e  f  g  h \n");
    for (y = 1; y <= SIDE; y++) {
        printf("%d |", y);
        for (x = 1; x <= SIDE; x++)
            printf("%s|", piece[board[getposition(x, y)]]);
        printf("\n");
    }
}

int isLegalMove(Move pos) {
    int dirx, diry, dir;
    int pos1;
    int color = TURNCOLOR(turn);
    int opponentcolor = TURNCOLOR(OPPONENT(turn));

    if (board[pos] != 0)
        return FALSE;

    for (dirx = -1; dirx <= 1; dirx++) {
        for (diry = -ASIDE; diry <= ASIDE; diry += ASIDE) {
            dir = dirx + diry;
            if (dir == 0)
                continue;
            pos1 = pos + dir;
            if (board[pos1] != opponentcolor)
                continue;
            do {
                pos1 += dir;
            } while (board[pos1] == opponentcolor);
            if (board[pos1] != color)
                continue;
            return TRUE;
        }
    }
    return FALSE;
}

int generateMoves(Move moves[]) {
    int num = 0;
    int pos;
    for (pos = 0; pos < BOARDSIZE; pos++) {
        if (board[getposition(X(pos), Y(pos))] == N) continue;
        if (board[pos] == 0) { 
            if (isLegalMove(pos))
                moves[num++] = pos;
        }
    }
    return num;
}

int isTerminalNode(int isPass) {
    int num;
    Move moves[MOVENUM];
    if (!isPass) {
        num = generateMoves(moves);
        if (num > 0)
            return FALSE;
    }
    turn = OPPONENT(turn);
    num = generateMoves(moves);
    turn = OPPONENT(turn); 
    if (num == 0)
        return TRUE;
    return FALSE;
}


int getTerminalValue(void) {
    int diff = stonenum[turn] - stonenum[OPPONENT(turn)];
    if (diff > 0)
        return GAME_INFINITY; 
    else if (diff == 0)
        return 0;
    else
        return -GAME_INFINITY;
}

int getEvaluationValue(void) {
    int pos, value, c;
    Move moves[MOVENUM];
    value = generateMoves(moves);
    turn = OPPONENT(turn);
    value -= generateMoves(moves);
    turn = OPPONENT(turn); // Switch back
    value *= 30;
    for (pos = 0; pos < BOARDSIZE; pos++) {
        c = board[pos];
        if (c == 0 || c == N) // Skip empty and sentinel
            continue;
        else if (c == TURNCOLOR(turn))
            value += evalboard[pos];
        else
            value -= evalboard[pos];
    }
    return value;
}

void makeMove(Move pos, int depth) {
    int pos1, dir, dirx, diry, count = 0;

    int color = TURNCOLOR(turn);
    int opponentcolor = TURNCOLOR(OPPONENT(turn));

    if (depth < SEARCH_LIMIT_DEPTH) { 
        memcpy(history[depth].board, board, sizeof(board));
        memcpy(history[depth].stonenum, stonenum, sizeof(stonenum));
    }


    if (pos == PASSMOVE) {
        turn = OPPONENT(turn);
        return;
    }
    board[pos] = color;

    for (dirx = -1; dirx <= 1; dirx++) {
        for (diry = -ASIDE; diry <= ASIDE; diry += ASIDE) {
            dir = dirx + diry;
            if (dir == 0)
                continue;
            pos1 = pos + dir;
            if (board[pos1] != opponentcolor)
                continue;
            do {
                pos1 += dir;
            } while (board[pos1] == opponentcolor);
            if (board[pos1] != color)
                continue;

            pos1 = pos + dir; 
            while (board[pos1] == opponentcolor) { 
                board[pos1] = color;
                pos1 += dir;
                count++;
            }
        }
    }
    stonenum[turn] += count + 1;
    stonenum[OPPONENT(turn)] -= count;
    turn = OPPONENT(turn);
}

void unmakeMove(int depth) {
    if (depth < SEARCH_LIMIT_DEPTH) {
        memcpy(board, history[depth].board, sizeof(board));
        memcpy(stonenum, history[depth].stonenum, sizeof(stonenum));

        turn = OPPONENT(turn); 
    }
}

int search(int depth) {
    int i;
    int movenum;
    Move moves[MOVENUM];
    int value;
    int bestvalue = -GAME_INFINITY - 1; 

    if (depth >= MAXDEPTH)
        return getEvaluationValue();

    movenum = generateMoves(moves);
    if (movenum == 0) {
        if (isTerminalNode(TRUE)) 
            return getTerminalValue();
        else
            moves[movenum++] = PASSMOVE; 
    }

    for (i = 0; i < movenum; i++) {
        makeMove(moves[i], depth);
        value = -search(depth + 1);
        unmakeMove(depth);
        if (value > bestvalue) {
            bestvalue = value;
            if (depth == 0)
                nextmove = moves[i];
        }
    }
    return bestvalue;
}

void comPlayer(void) {
    int value;
    printf("Com Thinking...\n"); 
    value = search(0);
    printf("value = %d\n", value);
    if (value == GAME_INFINITY) 
        printf("Computer Finds Win!!\n");
}

Move manPlayer(void) {
    char line[256];
    int x, y, num, move_val;
    Move moves[MOVENUM];
    num = generateMoves(moves);

    if (num == 0) {
        printf("PASS!\n");
        printf("Press Enter!\n");
        fgets(line, 256, stdin);
        return PASSMOVE;
    }
    do {
        do {
            printf("x(a-h):");
            fgets(line, 256, stdin);
        } while (line[0] < 'a' || line[0] > 'h');
        x = line[0] - 'a' + 1;

        do {
            printf("y(1-8):");
            fgets(line, 256, stdin);
        } while (line[0] < '1' || line[0] > '8');
        y = line[0] - '0';

        move_val = getposition(x, y);
        if (isLegalMove(move_val)) {
            break;
        }
        printf("Illegal Move!\n");
    } while (1);
    return (Move)move_val;
}

Move randplayer(void) {
    int num;
    Move moves[MOVENUM];
    num = generateMoves(moves);
    printf("RandPlayer\n"); 
    if (num == 0)
        return PASSMOVE;
    return moves[rand() % num];
}

void init(void) {
    turn = BLACK_TURN;
    ply = 0; 
    stonenum[BLACK_TURN] = 2;
    stonenum[WHITE_TURN] = 2;


    unsigned char initial_board[BOARDSIZE] = {
        N, N, N, N, N, N, N, N, N, N,
        N, 0, 0, 0, 0, 0, 0, 0, 0, N,
        N, 0, 0, 0, 0, 0, 0, 0, 0, N,
        N, 0, 0, 0, 0, 0, 0, 0, 0, N,
        N, 0, 0, 0, B, W, 0, 0, 0, N,
        N, 0, 0, 0, W, B, 0, 0, 0, N,
        N, 0, 0, 0, 0, 0, 0, 0, 0, N,
        N, 0, 0, 0, 0, 0, 0, 0, 0, N,
        N, 0, 0, 0, 0, 0, 0, 0, 0, N,
        N, N, N, N, N, N, N, N, N, N
    };
    memcpy(board, initial_board, sizeof(board));
}