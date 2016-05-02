#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include "board.h"
#define NUMPLAYER 5
#define OPTIONSGAME 2
#define NUMMENU 3
#define NUMMODE 5
#define MAXBALLS 121

int chooseMode(SDL_Surface* screen, TTF_Font* font)
{
	Uint32 time;
	const char* labels[NUMMODE] = {"Player1 vs Player2", "Player vs IA1", "Player vs IA2", "IA1 vs IA2", "BACK"};
	SDL_Surface* modes[NUMMODE];
	SDL_Surface* imgmain;
	SDL_Rect pos[NUMMODE];
	bool selected[NUMMODE] = {false, false, false, false, false};
	SDL_Color color[3] = {{0,0,0,0}, {255,0,0,0}, {255,255,255,0}};
	int x, y;
	int i, j;

	i = 0;
	while (i < NUMMODE - 1)
	{
		modes[i] = TTF_RenderText_Solid(font, labels[i], color[0]);
		pos[i].x = 300;
		pos[i].y = 253 + (i * 70);
		i++;
	}
	modes[i] = TTF_RenderText_Solid(font, labels[i], color[2]);
	pos[NUMMODE - 1].x = 55;
	pos[NUMMODE - 1].y = 555;

 	imgmain = IMG_Load("images/main.png");
	SDL_BlitSurface(imgmain, NULL, screen, NULL);
	SDL_Flip(screen);
	SDL_Event event;
	while (1)
	{
		time = SDL_GetTicks();
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
							fprintf(stderr, "showmode i = %d\n", i);
							return (i);
						}
					}
					break;
				case SDL_KEYDOWN:
 					if (event.key.keysym.sym == SDLK_ESCAPE)
					{
						for (i = 0; i < NUMMODE; i++)
							SDL_FreeSurface(modes[0]);
						SDL_FreeSurface(imgmain);
						return (4);
					}
			}
		}
		for (i = 0; i < NUMMODE; i++)
			SDL_BlitSurface(modes[i], NULL, screen, &pos[i]);
		SDL_Flip(screen);
		if (1000/30 > (SDL_GetTicks() - time))
			SDL_Delay(1000/30 - (SDL_GetTicks() - time));
	}
}

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

int chooseStart(SDL_Surface* screen, TTF_Font* font, const char *name)
{
	Uint32 time;
	const char* labels[NUMMENU] = {"NO", "YES", "BACK"};
	const char* part1 = "Do you want : ";
	const char* part2 = "to play first ?";
	SDL_Surface* choices[NUMMENU];
	SDL_Surface* print_name[NUMMENU];
	SDL_Surface* imgmain;
	SDL_Rect pos[6];
	bool selected[NUMMENU] = {false, false, false};
	SDL_Color color[3] = {{0,0,0,0}, {255,0,0,0}, {255,255,255,0}};
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
	SDL_Event event;
	while (1)
	{
		time = SDL_GetTicks();
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
		if (1000/30 > (SDL_GetTicks() - time))
			SDL_Delay(1000/30 - (SDL_GetTicks() - time));
	}
}

int chooseMenu(SDL_Surface* screen, TTF_Font* font)
{
	Uint32 time;
	const char* labels[NUMMENU] = {"New Game", "Load Recent Game", "Exit"};
	SDL_Surface* menus[NUMMENU];
	SDL_Surface* imgmain;
	SDL_Rect pos[NUMMENU];
	bool selected[NUMMENU] = {false, false, false};
	SDL_Color color[2] = {{0,0,0,0},{255,0,0,0}};
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
	SDL_Event event;
	while (1)
	{
		time = SDL_GetTicks();
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
		if (1000/30 > (SDL_GetTicks() - time))
			SDL_Delay(1000/30 - (SDL_GetTicks() - time));
	}
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
	int			a;
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
						a = (calcright[1].y - calcright[0].y) / (calcright[1].x - calcright[0].x);
					else
						a = (calcright[2].y - calcright[1].y) / (calcright[2].x - calcright[1].x);
					b = calcright[1].y - a * calcright[1].x;
					yres = a * x + b;
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

void freeandprint(SDL_Surface *screen, TTF_Font* font, SDL_Surface *str, const char *word, SDL_Color color, SDL_Rect pos)
{
	SDL_FreeSurface(str);
	str = TTF_RenderText_Solid(font, word, color);
	SDL_BlitSurface(str, NULL, screen, &pos);
}

int playNewGame(SDL_Surface* screen, TTF_Font* font, t_hexBoard tab[N][N], int mode, int start)
{
	Uint32 time;
	const char* labels[NUMMODE] = {"UNDO", "HISTORY", "PAUSE"};
	const char* players[NUMPLAYER] = {"Player", "Player1", "Player2", "IA1", "IA2"};
	const char* sentence1 = "It's your turn : ";
	const char* sentence2 = "Last Turn : ";
	e_Color ecolor;
	char last_x[3];
	char last_y[3];
	SDL_Surface* menu[NUMMENU];
	SDL_Surface* str[6];
	SDL_Surface* imgmain;
	SDL_Rect pos[9];
	SDL_Rect point;
	//SDL_Rect blackrect;
	bool selected[NUMMENU] = {false, false, false};
	SDL_Color color[4] = {{0,0,0,0}, {255,0,0,0}, {255,255,255,0}, {0,0,255,0}};
	int x, y;
	int i, j;
	int play;
	int count;

	i = 0;
	count = 0;
	ecolor = RED;
	last_x[0] = '\0';
	last_y[0] = '\0';
	while (i < NUMMENU)
	{
		menu[i] = TTF_RenderText_Solid(font, labels[i], color[2]);
		pos[i].x = 23;
		pos[i].y = 232 + (i * 50);
		i++;
	}
	str[0] = TTF_RenderText_Solid(font, sentence1, color[2]);
	pos[3].x = 300;
	pos[3].y = 56;
	if (mode == 0)
	{
		if (start)
		{
			str[1] = TTF_RenderText_Solid(font, players[1], color[1]);
			str[2] = TTF_RenderText_Solid(font, players[2], color[0]);
		}
		else
		{
			str[1] = TTF_RenderText_Solid(font, players[1], color[0]);
			str[2] = TTF_RenderText_Solid(font, players[2], color[1]);
		}
	}
	else if (mode == 1 || mode == 2)
	{
		if (start)
		{
			str[1] = TTF_RenderText_Solid(font, players[0], color[1]);
			if (mode == 1)
				str[2] = TTF_RenderText_Solid(font, players[3], color[0]);
			else
				str[2] = TTF_RenderText_Solid(font, players[4], color[0]);
		}
		else
		{
			str[1] = TTF_RenderText_Solid(font, players[0], color[0]);
			if (mode == 1)
				str[2] = TTF_RenderText_Solid(font, players[3], color[1]);
			else
				str[2] = TTF_RenderText_Solid(font, players[4], color[1]);
		}
	}
	else
	{
		if (start)
		{
			str[1] = TTF_RenderText_Solid(font, players[3], color[1]);
			str[2] = TTF_RenderText_Solid(font, players[4], color[0]);
		}
		else
		{
			str[1] = TTF_RenderText_Solid(font, players[3], color[0]);
			str[2] = TTF_RenderText_Solid(font, players[4], color[1]);
		}
	}
	pos[4].x = 357;
	pos[4].y = 116;
	str[3] = TTF_RenderText_Solid(font, sentence2, color[2]);
	pos[5].x = 186;
	pos[6].x = 450;
	pos[7].x = 520;
	pos[8].x = 560;
	pos[5].y = pos[6].y = pos[7].y = pos[8].y = 557;
	str[4] = TTF_RenderText_Solid(font, last_y, color[2]);
	str[5] = TTF_RenderText_Solid(font, last_x, color[2]);

 	imgmain = IMG_Load("images/board.png");
	SDL_BlitSurface(imgmain, NULL, screen, NULL);
	SDL_BlitSurface(str[0], NULL, screen, &pos[3]);
	SDL_BlitSurface(str[3], NULL, screen, &pos[5]);
	SDL_Flip(screen);
	SDL_Event event;
	while (1)
	{
		time = SDL_GetTicks();
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
							for (j = 0; j < NUMMENU; j++)
								SDL_FreeSurface(menu[j]);
							for (j = 0; j < 6; j++)
								SDL_FreeSurface(str[j]);
							SDL_FreeSurface(imgmain);
							return (i);
						}
					}
					point = findIndex(x, y);
					fprintf(stderr, "x = %d\n, y = %d\n", point.x, point.y);
					if (point.x != -1 && point.y != -1)
					{
						play = addColorGrid(tab, ecolor, point.y, point.x);
						if (play != 2)
						{
							freeandprint(screen, font, str[4], last_y, color[0], pos[7]);
							freeandprint(screen, font, str[5], last_x, color[0], pos[8]);
							drawBall(screen, ecolor, point);
							count++;
							if (ecolor == RED)
								ecolor = BLUE;
							else
								ecolor = RED;
							sprintf(last_x, "%d", point.x);
							sprintf(last_y, "%d", point.y);
							freeandprint(screen, font, str[4], last_y, color[2], pos[7]);
							freeandprint(screen, font, str[5], last_x, color[2], pos[8]);
							SDL_Flip(screen);
						}
					}
					break;
				case SDL_KEYDOWN:
 					if (event.key.keysym.sym == SDLK_ESCAPE)
					{
						for (j = 0; j < NUMMENU; j++)
							SDL_FreeSurface(menu[j]);
						for (j = 0; j < 6; j++)
							SDL_FreeSurface(str[j]);
						SDL_FreeSurface(imgmain);
						return (2);
					}
			}
		}
		for (i = 0; i < NUMMENU; i++)
			SDL_BlitSurface(menu[i], NULL, screen, &pos[i]);
		SDL_Flip(screen);
		if (1000/30 > (SDL_GetTicks() - time))
			SDL_Delay(1000/30 - (SDL_GetTicks() - time));
	}
}


int	main(void)
{
	SDL_Surface *screen = NULL;
	TTF_Font *font;
	int menu;
	int init_newgame;
	//int load_recentgame;
	int flag;
	int start;
	int	mode;
	t_hexBoard tab[N][N];

	menu = 0;
	init_newgame = 0;
//	load_recentgame = 0;
	TTF_Init();
	font = TTF_OpenFont("ASMAN.TTF", 30);
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
		if (menu == 0)
		{
			if (init_newgame == 0)
			{
				initGrid(tab);
				init_newgame = 1;
				//load_recentgame = 0;
			}
			mode = 0;
			flag = 1;
			while (mode != 5 && (mode = chooseMode(screen, font)) != 5)
			{
				fprintf(stderr, "mode = %d\n", mode);
				if (mode == 0)
				{
					if ((start = chooseStart(screen, font, "Player1")) == 3)
						mode = 5;
					else if (start != 2)
						playNewGame(screen, font, tab, mode, start);
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
			if (mode == 5 && flag)
				menu = 2;
		}
	}
	SDL_FreeSurface(screen);
	TTF_CloseFont(font);
	TTF_Quit();
	SDL_Quit();
		
			
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
	SDL_Quit();
*/	exit(EXIT_SUCCESS);
}
