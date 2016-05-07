#ifndef HEXGAME_H
# define HEXGAME_H
# include <SDL/SDL.h>
# include <SDL/SDL_ttf.h>
# include "hexboard.h"
# define NUMPLAYER 5
# define NUMMENU 3
# define LASTTURN 6
# define THISTURN 4

SDL_Rect insertPoint(int x, int y);
SDL_Rect findIndex(int x, int y);
void initPlayersWithMode(SDL_Surface *str[6], TTF_Font *font, const char *players[NUMPLAYER], int mode, int start, SDL_Color color[4], e_Color ecolor);
void init_positionGame(SDL_Rect pos[9]);
int playGame(SDL_Surface* screen, TTF_Font* font, t_hexBoard tab[N][N], int mode, int start, int turn, int lasty, int lastx);
void removetmpfiles(void);

#endif
