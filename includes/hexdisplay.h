#ifndef HEXDISPLAY_H
# define HEXDISPLAY_H
# include <SDL/SDL.h>
# include <SDL/SDL_ttf.h>
# include "hexboard.h"
# define NUMPAUSE 4
# define NUMMODE 5

void display_winner(SDL_Surface *screen, e_Color color);
void blit_Player(SDL_Surface *screen, SDL_Surface *str[6], SDL_Rect pos[9], int trigger, int index);
void drawBall(SDL_Surface* screen, e_Color color, int y, int x);
void displayBoard(SDL_Surface* screen, t_hexBoard tab[N][N]);
int print_pause(SDL_Surface* screen, TTF_Font *font, t_hexBoard tab[N][N]);
int chooseMode(SDL_Surface* screen, TTF_Font* font);
int chooseStart(SDL_Surface* screen, TTF_Font* font, const char *name);
int chooseMenu(SDL_Surface* screen, TTF_Font* font);
void freeandprint(SDL_Surface *screen, TTF_Font* font, SDL_Surface **str, const char *word, SDL_Color color, SDL_Rect pos);

#endif
