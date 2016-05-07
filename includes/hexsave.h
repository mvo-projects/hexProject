#ifndef HEXSAVE_H
# define HEXSAVE_H
# include "hexboard.h"

char * delete_newline(char *str);
int countlinegame(void);
void printPlay(FILE **fgame, e_Color color, int x, int y);
void undoGame(int line);
int loadUndo(t_hexBoard tab[N][N], int *py, int *px);
int loadGame(t_hexBoard tab[N][N], int *y, int *x);
void saveBoard(const char *name, t_hexBoard tab[N][N]);
void save_Game(t_hexBoard tab[N][N]);

#endif
