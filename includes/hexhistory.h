#ifndef HEXHISTORY_H
# define HEXHISTORY_H
# include <SDL/SDL.h>
# include <SDL/SDL_ttf.h>
# define MAXHISTORY 10

void stock_history(char *str[MAXHISTORY]);
void init_history(TTF_Font *font, SDL_Rect pos[12], SDL_Surface *modes[12], char *str[MAXHISTORY]);
int print_history(SDL_Surface* screen, TTF_Font *font);

#endif
