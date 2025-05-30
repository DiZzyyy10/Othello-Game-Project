#ifndef OTHELLO_H_INCLUDED
#define OTHELLO_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

    // Constants and Macros
#define SIDE 8
#define ASIDE (SIDE + 2)
#define BOARDSIZE (ASIDE * ASIDE)
#define UP (-ASIDE)
#define DOWN ASIDE
#define RIGHT 1
#define LEFT (-1)

#define B 1
#define W 2
#define N 3

#define BLACK_TURN 0
#define WHITE_TURN 1

#define X(pos) ((pos) % ASIDE)
#define Y(pos) ((pos) / ASIDE)
#define TURNCOLOR(turn) ((turn) + 1)
#define OPPONENT(turn) (!(turn))

#define TRUE 1
#define FALSE 0

#define MOVENUM 32
#define PASSMOVE 99

#define MAXDEPTH 5
#define GAME_INFINITY 1000 
#define SEARCH_LIMIT_DEPTH 128


    typedef char Move;

    
    struct Position {
        unsigned char board[BOARDSIZE];
        unsigned char stonenum[2];
    };

    
    extern const char* piece[3];
    extern const char* abc[8];
    extern int turn;
    extern int ply;
    extern Move nextmove;
    extern unsigned char stonenum[2];
    extern struct Position history[SEARCH_LIMIT_DEPTH];
    extern int evalboard[BOARDSIZE];
    extern unsigned char board[BOARDSIZE];



    static inline int getposition(int x, int y) {
        return y * ASIDE + x;
    }

    void output(void);
    int isLegalMove(Move pos);
    int generateMoves(Move moves[]);
    int isTerminalNode(int isPass);
    int getTerminalValue(void);
    int getEvaluationValue(void);
    void makeMove(Move pos, int depth);
    void unmakeMove(int depth);
    int search(int depth);
    void comPlayer(void);
    Move manPlayer(void);
    Move randplayer(void);
    void init(void);

#ifdef __cplusplus
}
#endif

#endif 