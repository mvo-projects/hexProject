#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include "hexboard.h"
#include "hexdisplay.h"
#include "hexsave.h"
#include "hexgame.h"

int	main(void)
{
	SDL_Surface *screen = NULL;
	TTF_Font *font;
	int menu;
	int flag;
	int start;
	int	mode;
	int play;
	int turn;
	int x;
	int y;
	t_hexBoard tab[N][N];
	initGrid(tab, HARD_RESET);
	play = 0;
	menu = 0;
	if (TTF_Init() == -1)
	{
		fprintf(stderr, "Could not initialize Font: %s\n", TTF_GetError());
		exit(EXIT_FAILURE);
	}
	font = TTF_OpenFont("font/hex.TTF", 30);
	if ((SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) == -1))
	{
		fprintf(stderr, "Could not initialize SDL: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}
	putenv("SDL_VIDEO_WINDOW_POS=center");
	if ((screen = SDL_SetVideoMode(800, 600, 32, SDL_HWSURFACE)) == NULL)
	{
		fprintf(stderr, "Could not initialize video mode : %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}
	SDL_WM_SetCaption("hexProject", NULL);
	while (menu != 2 && (menu = chooseMenu(screen, font)) != 2)
	{
		if (menu == 0 || menu == 1)
		{
			mode = 0;
			flag = 1;
			while (mode != 5 && (mode = chooseMode(screen, font)) != 5)
			{
				initGrid(tab, SOFT_RESET);
				if (mode == 0)
				{
					if ((start = chooseStart(screen, font, "Player1")) == 3)
						mode = 5;
					else if (start != 2)
					{
						x = -1;
						y = -1;
						turn = 1;
						if (menu == 1)
							turn = loadGame(tab, &y, &x);
						if ((play = playGame(screen, font, tab, mode, start, turn, y, x)) == 5)
							mode = 5;
						else if (play == 0)
						{
							do
							{
								initGrid(tab, SOFT_RESET);
								turn = loadUndo(tab, &y, &x);
								if (tab[y][x].color == DULL)
									y = x = -1;
								play = playGame(screen, font, tab, mode, start, turn, y, x);
							}while (play == 0);
						}
						if (play == 3 || play == 4 || play == 6)
						{
							if (play == 3)
								display_winner(screen, RED);
							else if (play == 4)
								display_winner(screen, BLUE);
							mode = 5;
							flag = 0;
						}
						else if (play == 5)
							mode = 5;
					}
				}
				else if (mode == 1 || mode == 2)
				{
					if ((start = chooseStart(screen, font, "Player")) == 3)
						mode = 5;
				}
				else if (mode == 3)
				{
					if ((start = chooseStart(screen, font, "IA1")) == 3)
						mode = 5;
				}
				else if (mode == 4)
				{
					mode = 5;
					flag = 0;
				}
			}
		}
		if (mode == 5 && flag)
			menu = 2;
		removetmpfiles();
	}
	SDL_FreeSurface(screen);
	TTF_CloseFont(font);
	TTF_Quit();
	SDL_Quit();
	exit(EXIT_SUCCESS);
}	
