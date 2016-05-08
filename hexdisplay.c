#include <unistd.h>
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include "hexboard.h"
#include "hexsave.h"
#include "hexgame.h"
#include "hexdisplay.h"

/*
void print_allpos(SDL_Rect pos[12])
{
	int i;

	for (i = 0; i < 12; i++)
		fprintf(stderr, "pos[%d].x = %d, pos[%d].y = %d\n", i, pos[i].x, i, pos[i].y);
}
*/

void display_winner(SDL_Surface *screen, e_Color color)
{
	SDL_Surface *imgmain;
	if (color == RED)
		imgmain = IMG_Load("images/redWon.png");
	else
		imgmain = IMG_Load("images/blueWon.png");
	SDL_BlitSurface(imgmain, NULL, screen, NULL);
	SDL_Flip(screen);
	sleep(2);
	SDL_FreeSurface(imgmain);
}

void blit_Player(SDL_Surface *screen, SDL_Surface *str[6], SDL_Rect pos[9], int trigger, int index)
{
	if (trigger)
	{
		SDL_BlitSurface(str[2], NULL, screen, &pos[index]);
		SDL_BlitSurface(str[1], NULL, screen, &pos[index]);
	}
	else
	{
		SDL_BlitSurface(str[1], NULL, screen, &pos[index]);
		SDL_BlitSurface(str[2], NULL, screen, &pos[index]);
	}
}

void drawBall(SDL_Surface* screen, e_Color color, int y, int x)
{
	SDL_Surface *ball;
	SDL_Rect	topleft;
	SDL_Rect	pos;
	float		gap1;
	float		gap2;

	topleft = insertPoint(301, 168);
	gap1 = 26.5;
	gap2 = 15.1;
	if (color == RED)
		ball = IMG_Load("images/redball.png");
	else
		ball = IMG_Load("images/blueball.png");
	pos.y = topleft.y + y * gap1;
	pos.x = topleft.x + (((x * 2) - y) * gap2);
	SDL_BlitSurface(ball, NULL, screen, &pos);
	SDL_FreeSurface(ball);
}

void displayBoard(SDL_Surface* screen, t_hexBoard tab[N][N])
{
	int y;
	int x;

	for (y = 0; y < N; y++)
	{
		for (x = 0; x < N; x++)
		{
			if (tab[y][x].color != DULL)
			{
				if (tab[y][x].color == BLUE)
					drawBall(screen, BLUE, y, x);
				else if (tab[y][x].color == RED)
					drawBall(screen, RED, y, x);
			}
		}
	}
}

int print_pause(SDL_Surface* screen, TTF_Font *font, t_hexBoard tab[N][N])
{
	const char* pause[NUMPAUSE] = {"Continue", "Save Game", "Main Menu", "Exit" };
	const char* save = "Game saved.";
	SDL_Surface *menus[NUMPAUSE + 1];
	SDL_Rect pos[NUMPAUSE + 1];
	SDL_Event event;
	bool selected[NUMPAUSE] = {false, false, false, false};
	int	i, j, x, y;
	SDL_Color color[3] = {{255, 255, 255, 0}, {255, 0, 0, 0}, {0, 0, 0, 0}};

	for (i = 0; i < NUMPAUSE; i++)
	{
		menus[i] = TTF_RenderText_Solid(font, pause[i], color[0]);
		pos[i] = insertPoint(340, (220 + (i * 50)));
	}
	pos[i] = insertPoint(652, 25);
	SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format,0x00,0x00,0x00));
	while (1)
	{
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_QUIT:
					for (i = 0; i < NUMPAUSE; i++)
						SDL_FreeSurface(menus[i]);
					return (3);
				case SDL_KEYDOWN:
 					if (event.key.keysym.sym == SDLK_ESCAPE)
					{
						for (i = 0; i < NUMPAUSE; i++)
							SDL_FreeSurface(menus[i]);
						return (0);
					}
				case SDL_MOUSEMOTION:
					x = event.motion.x;
					y = event.motion.y;
					for (i = 0; i < NUMPAUSE; i++)
					{
						if (x >= pos[i].x && x <= (pos[i].x + pos[i].w) && y >= pos[i].y && y <= (pos[i].y + pos[i].h))
						{
							if (selected[i] == false)
							{
								selected[i] = true;
								SDL_FreeSurface(menus[i]);
								menus[i] = TTF_RenderText_Solid(font, pause[i], color[1]);
							}
						}
						else
						{
							if (selected[i] == true)
							{
								selected[i] = false;
								SDL_FreeSurface(menus[i]);
								menus[i] = TTF_RenderText_Solid(font, pause[i], color[0]);
							}
						}
					}
					break;
				case SDL_MOUSEBUTTONDOWN:
					x = event.button.x;
					y = event.button.y;
					for (i = 0; i < NUMPAUSE; i++)
					{
						if (x >= pos[i].x && x <= (pos[i].x + pos[i].w) && y >= pos[i].y && y <= (pos[i].y + pos[i].h))
						{
							if (i == 1)
							{
								save_Game(tab);
								menus[NUMPAUSE] = TTF_RenderText_Solid(font, save, color[0]);
								SDL_BlitSurface(menus[NUMPAUSE], NULL, screen, &pos[NUMPAUSE]);
								SDL_Flip(screen);
								SDL_FreeSurface(menus[NUMPAUSE]);
								sleep(1);
								menus[NUMPAUSE] = TTF_RenderText_Solid(font, save, color[2]);
								SDL_BlitSurface(menus[NUMPAUSE], NULL, screen, &pos[NUMPAUSE]);
								SDL_Flip(screen);
								SDL_FreeSurface(menus[NUMPAUSE]);
							}
							else
							{
								for (j = 0; j < NUMPAUSE; j++)
									SDL_FreeSurface(menus[j]);
								return (i);
							}
						}
					}
					break;
			}
		}
		for (i = 0; i < NUMPAUSE; i++)
			SDL_BlitSurface(menus[i], NULL, screen, &pos[i]);
		SDL_Flip(screen);
	}
}

int chooseMode(SDL_Surface* screen, TTF_Font* font)
{
	const char* labels[NUMMODE] = {"Player1 vs Player2", "Player vs IA1", "Player vs IA2", "IA1 vs IA2", "BACK"};
	SDL_Surface* modes[NUMMODE];
	SDL_Surface* imgmain;
	SDL_Rect pos[NUMMODE];
	bool selected[NUMMODE] = {false, false, false, false, false};
	SDL_Color color[3] = {{0,0,0,0}, {255,0,0,0}, {255,255,255,0}};
	int x, y;
	int i, j;
	SDL_Event event;

	i = 0;
	while (i < NUMMODE - 1)
	{
		modes[i] = TTF_RenderText_Solid(font, labels[i], color[0]);
		pos[i] = insertPoint(300, (253 + i * 70));
		i++;
	}
	modes[i] = TTF_RenderText_Solid(font, labels[i], color[2]);
	pos[i] = insertPoint(55, 555);

 	imgmain = IMG_Load("images/main.png");
	SDL_BlitSurface(imgmain, NULL, screen, NULL);
	SDL_Flip(screen);
	while (1)
	{
		while (SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_QUIT:
					for (i = 0; i < NUMMODE; i++)
						SDL_FreeSurface(modes[i]);
					SDL_FreeSurface(imgmain);
					return (5);
				case SDL_MOUSEMOTION:
					x = event.motion.x;
					y = event.motion.y;
					for (i = 0; i < NUMMODE; i++)
					{
						if (x >= pos[i].x && x <= pos[i].x + pos[i].w && y >= pos[i].y && y <= pos[i].y + pos[i].h)
						{
							if (!selected[i])
							{
								selected[i] = 1;
								SDL_FreeSurface(modes[i]);
								modes[i] = TTF_RenderText_Solid(font,labels[i],color[1]);
							}
						}
						else
						{
							if (selected[i])
							{
								selected[i] = 0;
								SDL_FreeSurface(modes[i]);
								if (i != NUMMODE - 1)	
									modes[i] = TTF_RenderText_Solid(font,labels[i],color[0]);
								else
									modes[i] = TTF_RenderText_Solid(font,labels[i],color[2]);
							}
						}
					}
					break;
				case SDL_MOUSEBUTTONDOWN:
					x = event.button.x;
					y = event.button.y;
					for (i = 0; i < NUMMODE; i++)
					{
						if (x >= pos[i].x && x <= pos[i].x + pos[i].w && y >= pos[i].y && y <= pos[i].y + pos[i].h)
						{
							for (j = 0; j < NUMMODE; j++)
								SDL_FreeSurface(modes[j]);
							SDL_FreeSurface(imgmain);
							return (i);
						}
					}
					break;
				case SDL_KEYDOWN:
 					if (event.key.keysym.sym == SDLK_ESCAPE)
					{
						for (i = 0; i < NUMMODE; i++)
							SDL_FreeSurface(modes[i]);
						SDL_FreeSurface(imgmain);
						return (4);
					}
			}
		}
		for (i = 0; i < NUMMODE; i++)
			SDL_BlitSurface(modes[i], NULL, screen, &pos[i]);
		SDL_Flip(screen);
	}
}

int chooseStart(SDL_Surface* screen, TTF_Font* font, const char *name)
{
	const char* labels[NUMMENU] = {"NO", "YES", "BACK"};
	const char* part1 = "Do you want : ";
	const char* part2 = "to play first ?";
	SDL_Surface* choices[NUMMENU];
	SDL_Surface* print_name[NUMMENU];
	SDL_Surface* imgmain;
	SDL_Rect pos[6];
	bool selected[NUMMENU] = {false, false, false};
	SDL_Color color[3] = {{0,0,0,0}, {255,0,0,0}, {255,255,255,0}};
	SDL_Event event;
	int x, y;
	int i, j;

	i = 0;
	while (i < NUMMENU - 1)
	{
		choices[i] = TTF_RenderText_Solid(font, labels[i], color[0]);
		pos[i] = insertPoint((300 + (i * 150)), (screen->clip_rect.h/2 - choices[0]->clip_rect.h/2));
		i++;
	}
	choices[2] = TTF_RenderText_Solid(font, labels[2], color[2]);
	print_name[0] = TTF_RenderText_Solid(font, part1, color[0]);
	print_name[1] = TTF_RenderText_Solid(font, name, color[0]);
	print_name[2] = TTF_RenderText_Solid(font, part2, color[0]);
	pos[2] = insertPoint(55, 555);
	pos[3] = insertPoint(195, 175);
	pos[4] = insertPoint(375, 175);
	pos[5] = insertPoint(485, 175);
 	imgmain = IMG_Load("images/main.png");
	SDL_BlitSurface(imgmain, NULL, screen, NULL);
	SDL_BlitSurface(print_name[0], NULL, screen, &pos[3]);
	SDL_BlitSurface(print_name[1], NULL, screen, &pos[4]);
	SDL_BlitSurface(print_name[2], NULL, screen, &pos[5]);

	SDL_Flip(screen);
	while (1)
	{
		while (SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_QUIT:
					for (i = 0; i < NUMMENU; i++)
					{
						SDL_FreeSurface(choices[i]);
						SDL_FreeSurface(print_name[i]);
					}
					SDL_FreeSurface(imgmain);
					return (3);
				case SDL_MOUSEMOTION:
					x = event.motion.x;
					y = event.motion.y;
					for (i = 0; i < NUMMENU; i++)
					{
						if (x >= pos[i].x && x <= pos[i].x + pos[i].w && y >= pos[i].y && y <= pos[i].y + pos[i].h)
						{
							if (!selected[i])
							{
								selected[i] = true;
								SDL_FreeSurface(choices[i]);
								choices[i] = TTF_RenderText_Solid(font,labels[i],color[1]);
							}
						}
						else
						{
							if (selected[i])
							{
								selected[i] = false;
								SDL_FreeSurface(choices[i]);
								if (i != NUMMENU - 1)	
									choices[i] = TTF_RenderText_Solid(font,labels[i],color[0]);
								else
									choices[i] = TTF_RenderText_Solid(font,labels[i],color[2]);
							}
						}
					}
					break;
				case SDL_MOUSEBUTTONDOWN:
					x = event.button.x;
					y = event.button.y;
					for (i = 0; i < NUMMENU; i++)
					{
						if (x >= pos[i].x && x <= pos[i].x + pos[i].w && y >= pos[i].y && y <= pos[i].y + pos[i].h)
						{
							for (j = 0; j < NUMMENU; j++)
							{
								SDL_FreeSurface(choices[j]);
								SDL_FreeSurface(print_name[j]);
							}
							SDL_FreeSurface(imgmain);
							return (i);
						}
					}
					break;
				case SDL_KEYDOWN:
 					if (event.key.keysym.sym == SDLK_ESCAPE)
					{
						for (j = 0; j < NUMMENU; j++)
						{
							SDL_FreeSurface(choices[j]);
							SDL_FreeSurface(print_name[j]);
						}
						SDL_FreeSurface(imgmain);
						return (2);
					}
			}
		}
		for (i = 0; i < NUMMENU; i++)
			SDL_BlitSurface(choices[i], NULL, screen, &pos[i]);
		SDL_Flip(screen);
	}
}

int chooseMenu(SDL_Surface* screen, TTF_Font* font)
{
	const char* labels[NUMMENU] = {"New Game", "Load Recent Game", "Exit"};
	SDL_Surface* menus[NUMMENU];
	SDL_Surface* imgmain;
	SDL_Rect pos[NUMMENU];
	bool selected[NUMMENU] = {false, false, false};
	SDL_Color color[2] = {{0,0,0,0},{255,0,0,0}};
	SDL_Event event;
	int x, y;
	int i, j;

	i = 0;
	while (i < NUMMENU)
	{
		menus[i] = TTF_RenderText_Solid(font, labels[i], color[0]);
		pos[i] = insertPoint(300, (273 + (i * 70)));
		i++;
	}

 	imgmain = IMG_Load("images/main.png");
	SDL_BlitSurface(imgmain, NULL, screen, NULL);
	SDL_Flip(screen);
	while (1)
	{
		while (SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_QUIT:
					for (i = 0; i < NUMMENU; i++)
						SDL_FreeSurface(menus[i]);
					SDL_FreeSurface(imgmain);
					return (2);
				case SDL_MOUSEMOTION:
					x = event.motion.x;
					y = event.motion.y;
					for (i = 0; i < NUMMENU; i++)
					{
						if (x >= pos[i].x && x <= pos[i].x + pos[i].w && y >= pos[i].y && y <= pos[i].y + pos[i].h)
						{
							if (!selected[i])
							{
								selected[i] = true;
								SDL_FreeSurface(menus[i]);
								menus[i] = TTF_RenderText_Solid(font,labels[i],color[1]);
							}
						}
						else
						{
							if (selected[i])
							{
								selected[i] = false;
								SDL_FreeSurface(menus[i]);
								menus[i] = TTF_RenderText_Solid(font,labels[i],color[0]);
							}
						}
					}
					break;
				case SDL_MOUSEBUTTONDOWN:
					x = event.button.x;
					y = event.button.y;
					for (i = 0; i < NUMMENU; i++)
					{
						if (x >= pos[i].x && x <= pos[i].x + pos[i].w && y >= pos[i].y && y <= pos[i].y + pos[i].h)
						{
							for (j = 0; j < NUMMENU; j++)
								SDL_FreeSurface(menus[j]);
							SDL_FreeSurface(imgmain);
							return (i);
						}
					}
					break;
				case SDL_KEYDOWN:
 					if (event.key.keysym.sym == SDLK_ESCAPE)
					{
						for (j = 0; j < NUMMENU; j++)
							SDL_FreeSurface(menus[j]);
						SDL_FreeSurface(imgmain);
						return (2);
					}
			}
		}
		for (i = 0; i < NUMMENU; i++)
			SDL_BlitSurface(menus[i], NULL, screen, &pos[i]);
		SDL_Flip(screen);
	}
}

void freeandprint(SDL_Surface *screen, TTF_Font* font, SDL_Surface **str, const char *word, SDL_Color color, SDL_Rect pos)
{
	if (*str != NULL)
		SDL_FreeSurface(*str);
	*str = TTF_RenderText_Solid(font, word, color);
	SDL_BlitSurface(*str, NULL, screen, &pos);
}
