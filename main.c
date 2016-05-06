#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <assert.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include "board.h"
#define NUMPLAYER 5
#define OPTIONSGAME 2
#define NUMMENU 3
#define NUMMODE 5
#define MAXBALLS 121


void save_Game(t_hexBoard tab[N][N])
{
	FILE *fboard;
	FILE *fgame;
	FILE *fsave;
	char *str;

	str = (char *)malloc(sizeof(char) * 15);
	assert(str != NULL);
	if ((fsave = fopen("save.txt", "w")) == NULL)
	{
		fprintf(stderr, "Failed saving file...\n");
		exit(EXIT_FAILURE);
	}
	fprintf(fsave, "\\hex\n");
	saveBoard(".tmpboard.txt", tab);
	if ((fboard = fopen(".tmpboard.txt", "r")) == NULL)
	{
		fprintf(stderr, "here\n");
		fclose(fsave);
		fprintf(stderr, "Failed opening file...\n");
		exit(EXIT_FAILURE);
	}
	while (!feof(fboard) && fgets(str, 15, fboard) != NULL)
	{
		if (feof(fboard))
			break;
		fprintf(fsave, "%s", str);
	}
	if ((fgame = fopen(".tmpgame.txt", "r")) == NULL)
	{
		fprintf(stderr, "here2\n");
		fclose(fsave);
		fclose(fboard);
		fprintf(stderr, "Failed opening file...\n");
		exit(EXIT_FAILURE);
	}
	while (!feof(fgame) && fgets(str, 15, fgame) != NULL)
	{
		if (feof(fgame))
			break;
		fprintf(fsave, "%s", str);
	}
	fprintf(fsave, "\\endgame\n");
	fprintf(fsave, "\\endhex\n");
	fclose(fboard);
	fclose(fgame);
	fclose(fsave);
}

int print_pause(SDL_Surface* screen, TTF_Font *font, t_hexBoard tab[N][N])
{
	const char* pause[NUMMENU] = {"Continue", "Save Game", "Exit" };
	const char* save = "Game saved.";
	SDL_Surface *menus[NUMMENU + 1];
	SDL_Rect pos[NUMMENU + 1];
	SDL_Event event;
	bool selected[NUMMENU] = {false, false, false};
	int	i, j, x, y;
	SDL_Color color[3] = {{255, 255, 255, 0}, {255, 0, 0, 0}, {0, 0, 0, 0}};

	for (i = 0; i < NUMMENU; i++)
	{
		menus[i] = TTF_RenderText_Solid(font, pause[i], color[0]);
		pos[i].x = 340;
		pos[i].y = 250 + (i * 50);
	}
	pos[i].x = 652;
	pos[i].y = 25;
	SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format,0x00,0x00,0x00));
	while (1)
	{
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_QUIT:
					for (i = 0; i < NUMMENU; i++)
						SDL_FreeSurface(menus[i]);
					return (2);
				case SDL_KEYDOWN:
 					if (event.key.keysym.sym == SDLK_ESCAPE)
					{
						for (i = 0; i < NUMMENU; i++)
							SDL_FreeSurface(menus[i]);
						return (0);
					}
				case SDL_MOUSEMOTION:
					x = event.motion.x;
					y = event.motion.y;
					for (i = 0; i < NUMMENU; i++)
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
					for (i = 0; i < NUMMENU; i++)
					{
						if (x >= pos[i].x && x <= (pos[i].x + pos[i].w) && y >= pos[i].y && y <= (pos[i].y + pos[i].h))
						{
							if (i == 1)
							{
								save_Game(tab);
								menus[NUMMENU] = TTF_RenderText_Solid(font, save, color[0]);
								SDL_BlitSurface(menus[NUMMENU], NULL, screen, &pos[NUMMENU]);
								SDL_Flip(screen);
								SDL_FreeSurface(menus[NUMMENU]);
								sleep(1);
								menus[NUMMENU] = TTF_RenderText_Solid(font, save, color[2]);
								SDL_BlitSurface(menus[NUMMENU], NULL, screen, &pos[NUMMENU]);
								SDL_Flip(screen);
								SDL_FreeSurface(menus[NUMMENU]);
							}
							else
							{
								for (j = 0; j < NUMMENU; j++)
									SDL_FreeSurface(menus[j]);
								return (i);
							}
						}
					}
					break;
			}
		}
		for (i = 0; i < NUMMENU; i++)
			SDL_BlitSurface(menus[i], NULL, screen, &pos[i]);
		SDL_Flip(screen);
	}
}

char * delete_newline(char *str)
{
	int i;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '\n')
		{
			str[i] = '\0';
			break;
		}
		i++;
	}
	return (str);
}

void stock_history(char *str[10])
{
	int nb_line;
	int	i;
	FILE *fgame;
	char *tmp;

	tmp = (char *)malloc(sizeof(char) * 15);
	assert(tmp != NULL);
	nb_line = countlinegame();

	i = 0;
	if (nb_line > 0 && (fgame = fopen(".tmpgame.txt", "r")) != NULL)
	{
		fseek(fgame, sizeof(char) * 6, SEEK_SET);
		while (nb_line > 10)
		{
			if (fgets(tmp, 15, fgame) == NULL)
			{
				fprintf(stderr, "Error stock_history\n");
				exit(EXIT_FAILURE);
			}
			nb_line--;
		}
		while (i < nb_line && feof(fgame) == 0)
		{
			fseek(fgame, sizeof(char) * 6, SEEK_CUR);
			if (fgets(str[i], 15, fgame) != NULL)
			{
				str[i] = delete_newline(str[i]);
				fprintf(stderr, "str = %s\n", str[i]);
				if (feof(fgame) != 0)
					break;
				i++;
			}
		}
		fclose(fgame);
	}
	free(tmp);
}

void print_allpos(SDL_Rect pos[12])
{
	int i;

	for (i = 0; i < 12; i++)
		fprintf(stderr, "pos[%d].x = %d, pos[%d].y = %d\n", i, pos[i].x, i, pos[i].y);
}

void init_history(TTF_Font *font, SDL_Rect pos[12], SDL_Surface *modes[12], char *str[10])
{
	int i;
	int	j;
	SDL_Color color = {0, 0, 0, 0};

	for (j = 0, i = 2; i < 12; j++, i++)
	{
		if (i % 2 == 0)
			pos[i].x = 255;
		else
			pos[i].x = 462;
		pos[i].y = 220 + (j * 25);
		modes[i] = TTF_RenderText_Blended(font, str[j], color);
	}
}

int print_history(SDL_Surface* screen, TTF_Font *font)
{
	const char* back = "BACK";
	const char* title = "HISTORY";
	SDL_Surface* modes[12];
	SDL_Surface* imgmain;
	SDL_Rect	pos[12];
	char *str[10];
	bool selected = false;
	SDL_Color color[3] = {{0,0,0,0}, {255,0,0,0}, {255,255,255,0}};
	SDL_Event event;
	int	i;
	int x, y;

//	pos = (SDL_Rect *)malloc(sizeof(SDL_Rect) * 12);
//	assert(pos != NULL);
	for (i = 0; i < 10; i++)
	{
		str[i] = (char *)malloc(sizeof(char) * 15);
		assert(str[i] != NULL);
		str[i][0] = '\0';
	}
	stock_history(str);
	modes[0] = TTF_RenderText_Solid(font, back, color[2]);
	modes[1] = TTF_RenderText_Solid(font, title, color[1]);
	pos[0].x = 55;
	pos[0].y = 555;
	pos[1].x = 342;
	pos[1].y = 145;
	init_history(font, pos, modes, str);
	print_allpos(pos);
	imgmain = IMG_Load("images/main.png");
	SDL_BlitSurface(imgmain, NULL, screen, NULL);
	for (i = 2; i < 12; i++)
		SDL_BlitSurface(modes[i], NULL, screen, &pos[i]);
	SDL_Flip(screen);
	while (1)
	{
		while (SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_QUIT:
					for (i = 0; i < 2; i++)
						SDL_FreeSurface(modes[i]);
					SDL_FreeSurface(imgmain);
					return (1);	
				case SDL_KEYDOWN:
 					if (event.key.keysym.sym == SDLK_ESCAPE)
					{
						for (i = 0; i < 2; i++)
							SDL_FreeSurface(modes[i]);
						SDL_FreeSurface(imgmain);
						return (0);
					}
				case SDL_MOUSEMOTION:
					x = event.motion.x;
					y = event.motion.y;
					if (x >= pos[0].x && x <= pos[0].x + pos[0].w && y >= pos[0].y && y <= pos[0].y + pos[0].h)
					{
						if (!selected)
						{
							selected = true;
							SDL_FreeSurface(modes[0]);
							modes[0] = TTF_RenderText_Solid(font, back, color[1]);
						}
					}
					else
					{
						if (selected)
						{
							selected = false;
							SDL_FreeSurface(modes[0]);
							modes[0] = TTF_RenderText_Solid(font, back, color[2]);
						}
					}
					break;
				case SDL_MOUSEBUTTONDOWN:
					x = event.button.x;
					y = event.button.y;
					if (x >= pos[0].x && x <= pos[0].x + pos[0].w && y >= pos[0].y && y <= pos[0].y + pos[0].h)
					{
						for (i = 0; i < 2; i++)
							SDL_FreeSurface(modes[i]);
						SDL_FreeSurface(imgmain);
						return (0);
					}
					break;
			}
		}
		for (i = 0; i < 2; i++)
			SDL_BlitSurface(modes[i], NULL, screen, &pos[i]);
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
		pos[i].x = 300;
		pos[i].y = 253 + (i * 70);
		i++;
	}
	modes[i] = TTF_RenderText_Solid(font, labels[i], color[2]);
	pos[i].x = 55;
	pos[i].y = 555;

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

 
void drawBall(SDL_Surface* screen, e_Color color, int y, int x)
{
	SDL_Surface *ball;
	SDL_Rect	topleft;
	SDL_Rect	pos;
	float		gap1;
	float		gap2;

	topleft.x = 301;
	topleft.y = 168;
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
/*
void drawBall(SDL_Surface* screen, e_Color color, SDL_Rect point)
{
	SDL_Surface *ball;
	SDL_Rect	topleft;
	SDL_Rect	pos;
	int			gap1;
	int			gap2;

	topleft.x = 302;
	topleft.y = 167;
	gap1 = 27;
	gap2 = 15;
	if (color == RED)
		ball = IMG_Load("images/redball.png");
	else
		ball = IMG_Load("images/blueball.png");
	pos.y = topleft.y + point.y * gap1;
	pos.x = topleft.x + ((point.x * 2) - point.y) * gap2;
	SDL_BlitSurface(ball, NULL, screen, &pos);
	SDL_FreeSurface(ball);
} 
*/

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

int chooseStart(SDL_Surface* screen, TTF_Font* font, const char *name)
{
//	Uint32 time;
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
		pos[i].x = 300 + (i * 150);
		pos[i].y = screen->clip_rect.h/2 - choices[0]->clip_rect.h/2;
		i++;
	}
	choices[2] = TTF_RenderText_Solid(font, labels[2], color[2]);
	pos[2].x = 55;
	pos[2].y = 555;
	print_name[0] = TTF_RenderText_Solid(font, part1, color[0]);
	pos[3].x = 195;
	pos[3].y = 175;
	print_name[1] = TTF_RenderText_Solid(font, name, color[0]);
	pos[4].x = 375;
	pos[4].y = 175;
	print_name[2] = TTF_RenderText_Solid(font, part2, color[0]);
	pos[5].x = 485;
	pos[5].y = 175;
	

 	imgmain = IMG_Load("images/main.png");
	SDL_BlitSurface(imgmain, NULL, screen, NULL);
	SDL_BlitSurface(print_name[0], NULL, screen, &pos[3]);
	SDL_BlitSurface(print_name[1], NULL, screen, &pos[4]);
	SDL_BlitSurface(print_name[2], NULL, screen, &pos[5]);

	SDL_Flip(screen);
	while (1)
	{
//		time = SDL_GetTicks();
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
/*		if (1000/30 > (SDL_GetTicks() - time))
			SDL_Delay(1000/30 - (SDL_GetTicks() - time));*/
	}
}

int chooseMenu(SDL_Surface* screen, TTF_Font* font)
{
//	Uint32 time;
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
		pos[i].x = 300;
		pos[i].y = 273 + (i * 70);
		i++;
	}

 	imgmain = IMG_Load("images/main.png");
	SDL_BlitSurface(imgmain, NULL, screen, NULL);
	SDL_Flip(screen);
	while (1)
	{
//		time = SDL_GetTicks();
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
	/*	if (1000/30 > (SDL_GetTicks() - time))
			SDL_Delay(1000/30 - (SDL_GetTicks() - time));
*/	}
}

SDL_Rect insertPoint(int x, int y)
{
	SDL_Rect res;

	res.x = x;
	res.y = y;
	return (res);
}

SDL_Rect findIndex(int x, int y)
{
	SDL_Rect	topleft;
	SDL_Rect	bottomleft;
	SDL_Rect	topmid;
	SDL_Rect	midleft;
	SDL_Rect	res;
	SDL_Rect	calcright[3];
	float		a;
	int			b;
	int			yres;
	int			i;
	int			j;
	int			gap1;
	int			gap2;
	int			gap3;
	int			gap4;

	topleft.x = midleft.x = bottomleft.x = 302;
	topleft.y = topmid.y = 168;
	midleft.y = 176;
	bottomleft.y = 195;
	topmid.x = 317;
	gap1 = midleft.y - topleft.y;
	gap2 = bottomleft.y - topleft.y;
	gap3 = topmid.x - topleft.x;
	gap4 = 2 * gap3;
	res = insertPoint(-1, -1);
	for (i = 0; i < N + 1; i++)
	{
		if (y >= topleft.y && y <= (topleft.y + gap2))
		{
			topleft.x = topleft.x - (i * gap3);
			for (j = 0; j < N + 1; j++)
			{
				if (x >= topleft.x && x <= (topleft.x + gap4))
				{
					calcright[0] = insertPoint(topleft.x, topleft.y + gap1);
					calcright[1] = insertPoint(topleft.x + gap3, topleft.y);
					calcright[2] = insertPoint(topleft.x + gap4, topleft.y + gap1);	
					if (x >= calcright[0].x && x <= calcright[1].x)
						a = (calcright[1].y - calcright[0].y) / (float)(calcright[1].x - calcright[0].x);
					else
						a = (calcright[2].y - calcright[1].y) / (float)(calcright[2].x - calcright[1].x);
					b = calcright[1].y - a * calcright[1].x;
					yres = a * x + b;
					fprintf(stderr, " yres = %d\n, a = %f\n, x = %d\n, b = %d\n", yres, a, x, b);
					if (y < yres)
					{
						if (a < 0)
						{
							if (i != 0 && j != 0)
								res = insertPoint(j - 1, i - 1);
							return (res);
						}
						else if (a > 0)
						{
							if (i != 0 && j != N)
								res = insertPoint(j, i - 1);
							return (res);
						}
					}
					else
					{
						if (i != N && j != N)
							res = insertPoint(j, i);
						return (res);
					}
						
				}
				topleft.x += gap4;
			}
			return (res);
		}
		topleft.y += gap2;
	}
	return (res);
}

void freeandprint(SDL_Surface *screen, TTF_Font* font, SDL_Surface **str, const char *word, SDL_Color color, SDL_Rect pos)
{
	if (*str != NULL)
		SDL_FreeSurface(*str);
	*str = TTF_RenderText_Solid(font, word, color);
	SDL_BlitSurface(*str, NULL, screen, &pos);
}

void addPlayersWithMode(SDL_Surface *str[6], TTF_Font *font, const char *players[NUMPLAYER], int mode, int start, SDL_Color color[4], e_Color ecolor)
{
	if (start == 1)
	{
		if (mode == 0)
		{
			if (ecolor == RED)
				str[1] = TTF_RenderText_Solid(font, players[1], color[1]);
			else if (ecolor == BLUE)
				str[1] = TTF_RenderText_Solid(font, players[1], color[3]);
			else
				str[1] = TTF_RenderText_Solid(font, players[1], color[0]);
			str[2] = TTF_RenderText_Solid(font, players[2], color[0]);
		}
		else if (mode == 1 || mode == 2)
		{
			if (ecolor == RED)
				str[1] = TTF_RenderText_Solid(font, players[0], color[1]);
			else if (ecolor == BLUE)
				str[1] = TTF_RenderText_Solid(font, players[0], color[3]);
			else
				str[1] = TTF_RenderText_Solid(font, players[0], color[0]);
			if (mode == 1)
				str[2] = TTF_RenderText_Solid(font, players[3], color[0]);
			else
				str[2] = TTF_RenderText_Solid(font, players[4], color[0]);
		}
		else
		{
			if (ecolor == RED)
				str[1] = TTF_RenderText_Solid(font, players[3], color[1]);
			else if (ecolor == BLUE)
				str[1] = TTF_RenderText_Solid(font, players[3], color[3]);
			else
				str[1] = TTF_RenderText_Solid(font, players[3], color[0]);
			str[2] = TTF_RenderText_Solid(font, players[4], color[0]);
		}
	}
	else if (start == 0)
	{
		if (mode == 0)
		{
			str[1] = TTF_RenderText_Solid(font, players[1], color[0]);
			if (ecolor == RED)
				str[2] = TTF_RenderText_Solid(font, players[2], color[1]);
			else if (ecolor == BLUE)
				str[2] = TTF_RenderText_Solid(font, players[2], color[3]);
			else	
				str[2] = TTF_RenderText_Solid(font, players[2], color[0]);
		}
		else if (mode == 1 || mode == 2)
		{
			str[1] = TTF_RenderText_Solid(font, players[0], color[0]);
			if (mode == 1)
			{
				if (ecolor == RED)
					str[2] = TTF_RenderText_Solid(font, players[3], color[1]);
				else if (ecolor == BLUE)
					str[2] = TTF_RenderText_Solid(font, players[3], color[3]);
				else
					str[2] = TTF_RenderText_Solid(font, players[3], color[0]);
			}
			else
			{
				if (ecolor == RED)
					str[2] = TTF_RenderText_Solid(font, players[4], color[1]);
				else if (ecolor == BLUE)
					str[2] = TTF_RenderText_Solid(font, players[4], color[3]);
				else
					str[2] = TTF_RenderText_Solid(font, players[4], color[0]);
			}
		}
		else
		{
			str[1] = TTF_RenderText_Solid(font, players[3], color[0]);
			if (ecolor == RED)
				str[2] = TTF_RenderText_Solid(font, players[4], color[1]);
			else if (ecolor == BLUE)
				str[2] = TTF_RenderText_Solid(font, players[4], color[3]);
			else
				str[2] = TTF_RenderText_Solid(font, players[4], color[0]);
		}
	}
}

int playNewGame(SDL_Surface* screen, TTF_Font* font, t_hexBoard tab[N][N], int mode, int start, int turn, int lasty, int lastx)
{
	const char* labels[NUMMODE] = {"UNDO", "HISTORY", "PAUSE"};
	const char* players[NUMPLAYER] = {"Player", "Player1", "Player2", "IA1", "IA2"};
	const char* sentence1 = "It's your turn : ";
	const char* sentence2 = "Last Turn : ";
	e_Color ecolor;
	char last_x[3];
	char last_y[3];
	FILE *fgame;
	SDL_Surface* menu[NUMMENU];
	SDL_Surface* str[6];
	SDL_Surface* imgmain;
	SDL_Rect pos[9];
	SDL_Rect point;
	SDL_Event event;
	//SDL_Rect blackrect;
	bool selected[NUMMENU] = {false, false, false};
	SDL_Color color[4] = {{0,0,0,0}, {255,0,0,0}, {255,255,255,0}, {0,0,255,0}};
	int x, y;
	int i, j;
	int flag;
	int play;
	int count;

	i = 0;
	count = 0;
	flag = 0;
	ecolor = RED;
 	imgmain = IMG_Load("images/board.png");
	SDL_BlitSurface(imgmain, NULL, screen, NULL);
	fprintf(stderr, "ici\n");
	while (i < NUMMENU)
	{
		menu[i] = TTF_RenderText_Solid(font, labels[i], color[2]);
		pos[i].x = 23;
		pos[i].y = 232 + (i * 50);
		i++;
	}
	pos[3].x = 300;
	pos[3].y = 56;
	pos[4].x = 357;
	pos[4].y = 116;
	pos[5].x = 186;
	pos[5].y = pos[6].y = pos[7].y = pos[8].y = 557;
	pos[6].x = 350;
	pos[7].x = 520;
	pos[8].x = 560;
	str[0] = TTF_RenderText_Solid(font, sentence1, color[2]);
	str[3] = TTF_RenderText_Solid(font, sentence2, color[2]);
	if (lastx == -1 || lasty == -1)
	{
		last_x[0] = '\0';
		last_y[0] = '\0';
	}
	else
	{
		sprintf(last_x, "%d", lastx);
		sprintf(last_y, "%d", lasty);
		if (tab[lasty][lastx].color == RED)
			ecolor = BLUE;
		else
			ecolor = RED;
		addPlayersWithMode(str, font, players, mode, turn, color, tab[lasty][lastx].color);
		if (turn)
		{
			SDL_BlitSurface(str[2], NULL, screen, &pos[6]);
			SDL_BlitSurface(str[1], NULL, screen, &pos[6]);
		}
		else
		{
			SDL_BlitSurface(str[1], NULL, screen, &pos[6]);
			SDL_BlitSurface(str[2], NULL, screen, &pos[6]);
		}
	}
	start = !(start ^ turn);
	addPlayersWithMode(str, font, players, mode, start, color, ecolor);
	if (start)
	{
		SDL_BlitSurface(str[2], NULL, screen, &pos[4]);
		SDL_BlitSurface(str[1], NULL, screen, &pos[4]);
	}
	else
	{
		SDL_BlitSurface(str[1], NULL, screen, &pos[4]);
		SDL_BlitSurface(str[2], NULL, screen, &pos[4]);
	}
	start = 1 - start;
	if (tab[lasty][lastx].color != DULL)
	{
		str[4] = TTF_RenderText_Solid(font, last_y, color[2]);
		str[5] = TTF_RenderText_Solid(font, last_x, color[2]);
	}
	else
	{
		str[4] = TTF_RenderText_Solid(font, last_y, color[0]);
		str[5] = TTF_RenderText_Solid(font, last_x, color[0]);
	}
	SDL_BlitSurface(str[0], NULL, screen, &pos[3]);
	SDL_BlitSurface(str[3], NULL, screen, &pos[5]);
	SDL_BlitSurface(str[4], NULL, screen, &pos[7]);
	SDL_BlitSurface(str[5], NULL, screen, &pos[8]);
	displayBoard(screen, tab);
	SDL_Flip(screen);
	fgame = NULL;
	while (1)
	{
		while (SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_QUIT:
					for (i = 0; i < NUMMENU; i++)
						SDL_FreeSurface(menu[i]);
					for (i = 0; i < 6; i++)
						SDL_FreeSurface(str[i]);
					SDL_FreeSurface(imgmain);
					return (5);
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
								SDL_FreeSurface(menu[i]);
								menu[i] = TTF_RenderText_Solid(font,labels[i],color[1]);
							}
						}
						else
						{
							if (selected[i])
							{
								selected[i] = false;
								SDL_FreeSurface(menu[i]);
								menu[i] = TTF_RenderText_Solid(font,labels[i],color[2]);
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
							if (fgame != NULL)
							{
								fclose(fgame);
								fgame = NULL;
							}
							if (i == 0)
							{
								for (j = 0; j < NUMMENU; j++)
									SDL_FreeSurface(menu[j]);
								for (j = 0; j < 6; j++)
									SDL_FreeSurface(str[j]);
								SDL_FreeSurface(imgmain);
								if (i == 0)
								{
									count = countlinegame();
									undoGame(count);
								}
								return (i);
							}
							else
							{
								SDL_SaveBMP(screen, ".map.bmp");
								if (i == 1)
								{
									if (print_history(screen, font))
										flag = 1;
								}
								else if (i == 2)
								{
									if (print_pause(screen, font, tab) == 2)
										flag = 1;
								}
								if (flag)
								{
									for (j = 0; j < NUMMENU; j++)
										SDL_FreeSurface(menu[j]);
									for (j = 0; j < 6; j++)
										SDL_FreeSurface(str[j]);
									SDL_FreeSurface(imgmain);
									return (5);
								}
								SDL_FreeSurface(imgmain);
								imgmain = IMG_Load(".map.bmp");
								SDL_BlitSurface(imgmain, NULL, screen, NULL);
								SDL_Flip(screen);
							}
							
						}
					}
					point = findIndex(x, y);
					fprintf(stderr, "x = %d, y = %d\n", point.x, point.y);
					if (point.x != -1 && point.y != -1)
					{
						play = addColorGrid(tab, ecolor, point.y, point.x);
						if (play != 2)
						{
							printPlay(&fgame, ecolor, point.y, point.x);
							count++;
							
							if (1 - start)
							{
								SDL_BlitSurface(str[2], NULL, screen, &pos[6]);
								SDL_BlitSurface(str[1], NULL, screen, &pos[6]);
							}
							else
							{
								SDL_BlitSurface(str[1], NULL, screen, &pos[6]);
								SDL_BlitSurface(str[2], NULL, screen, &pos[6]);
							}
							freeandprint(screen, font, &str[4], last_y, color[0], pos[7]);
							freeandprint(screen, font, &str[5], last_x, color[0], pos[8]);
							drawBall(screen, ecolor, point.y, point.x);
							SDL_Flip(screen);
							if (play == 1)
							{
								for (j = 0; j < NUMMENU; j++)
									SDL_FreeSurface(menu[j]);
								for (j = 0; j < 6; j++)
									SDL_FreeSurface(str[j]);
								SDL_FreeSurface(imgmain);
								if (ecolor == RED)
									return (3);
								return (4);
							}
							if (ecolor == RED)
								ecolor = BLUE;
							else
								ecolor = RED;
							SDL_FreeSurface(str[2]);
							SDL_FreeSurface(str[1]);
							addPlayersWithMode(str, font, players, mode, start, color, ecolor);
							if (start)
							{
								SDL_BlitSurface(str[2], NULL, screen, &pos[4]);
								SDL_BlitSurface(str[1], NULL, screen, &pos[4]);
							}
							else
							{
								SDL_BlitSurface(str[1], NULL, screen, &pos[4]);
								SDL_BlitSurface(str[2], NULL, screen, &pos[4]);
							}
							start = 1 - start;
							sprintf(last_x, "%d", point.x);
							sprintf(last_y, "%d", point.y);
							freeandprint(screen, font, &str[4], last_y, color[2], pos[7]);
							freeandprint(screen, font, &str[5], last_x, color[2], pos[8]);
							SDL_Flip(screen);
						}
					}
					break;
				case SDL_KEYDOWN:
 					if (event.key.keysym.sym == SDLK_ESCAPE)
					{
						SDL_SaveBMP(screen, ".map.bmp");
						if (print_pause(screen, font, tab) == 2)
						{
							for (j = 0; j < NUMMENU; j++)
								SDL_FreeSurface(menu[j]);
							for (j = 0; j < 6; j++)
								SDL_FreeSurface(str[j]);
							SDL_FreeSurface(imgmain);
							return (5);
						}
						SDL_FreeSurface(imgmain);
						imgmain = IMG_Load(".map.bmp");
						SDL_BlitSurface(imgmain, NULL, screen, NULL);
						SDL_Flip(screen);
					}
			}
		}
		for (i = 0; i < NUMMENU; i++)
			SDL_BlitSurface(menu[i], NULL, screen, &pos[i]);
		SDL_Flip(screen);
	//	if (1000/30 > (SDL_GetTicks() - time))
	//		SDL_Delay(1000/30 - (SDL_GetTicks() - time));
	}
}


void removetmpfiles(void)
{
	remove(".tmpgame.txt");
	remove(".tmpboard.txt");
	remove(".map.bmp");
}

int	main(void)
{
	SDL_Surface *screen = NULL;
	TTF_Font *font;
	int menu;
//	int init_newgame;
	//int load_recentgame;
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
//	init_newgame = 0;
//	load_recentgame = 0;
	if (TTF_Init() == -1)
	{
		fprintf(stderr, "Could not initialize Font: %s\n", TTF_GetError());
		exit(EXIT_FAILURE);
	}
	font = TTF_OpenFont("hex.TTF", 30);
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
		fprintf(stderr, "menu = %d\n", menu);
		if (menu == 0 || menu == 1)
		{
			mode = 0;
			flag = 1;
			while (mode != 5 && (mode = chooseMode(screen, font)) != 5)
			{
				initGrid(tab, SOFT_RESET);
				fprintf(stderr, "mode = %d\n", mode);
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
						if ((play = playNewGame(screen, font, tab, mode, start, turn, y, x)) == 5)
							mode = 5;
						else if (play == 0)
						{
							do
							{
								initGrid(tab, SOFT_RESET);
								turn = loadUndo(tab, &y, &x);
								if (tab[y][x].color == DULL)
									y = x = -1;
								fprintf(stderr, "turn = %d\n y = %d\nx = %d\n", turn, y, x);
								play = playNewGame(screen, font, tab, mode, start, turn, y, x);
							}while (play == 0);
						}
						if (play == 3)
							fprintf(stderr, "RED won\n");
						else if (play == 4)
							fprintf(stderr, "BLUE won\n");
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
/*	none = IMG_Load("images/menu.png");
	opt1 = IMG_Load("images/button_jouer.png");
	opt2 = IMG_Load("images/button_charger.png");
	opt3 = IMG_Load("images/button_quitter.png");
	menu = none;
	SDL_BlitSurface(menu, NULL, screen, NULL);
	SDL_Flip(screen);
	end = false;
	while (end != true)
	{
		time = SDL_GetTicks();
		while (SDL_PollEvent(&event) != 0)
		{
			if (event.type == SDL_QUIT)
				end = true;
			else if (event.type == SDL_KEYDOWN)
			{
				if (showmenu(screen, font) == 2)
					end = true;
				break;
			} 
			else if (event.type == SDL_MOUSEMOTION)
			{
				x = event.motion.x;
				y = event.motion.y;
				if (x > 300 && x < 500)
				{
					if (y > 274 && y < 320)
						menu = opt1;
					else if (y > 337 && y < 395)
						menu = opt2;
					else if (y > 414 && y < 470)
						menu = opt3;
					else
						menu = none;
				}
			}
			SDL_BlitSurface(menu, NULL, screen, NULL);
			SDL_Flip(screen);
			if (1000/30 > (SDL_GetTicks()-time))
				SDL_Delay(1000/30 - (SDL_GetTicks()-time));
		}
	}
	SDL_FreeSurface(none);
	SDL_FreeSurface(opt1);
	SDL_FreeSurface(opt2);
	SDL_FreeSurface(opt3);
	SDL_FreeSurface(menu);

	screen = menu = none = opt1 = opt2 = opt3 = NULL;
	SDL_Quit();*/
