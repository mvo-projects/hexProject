#include <stdio.h>
#include <stddef.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include "hexboard.h"
#include "hexgame.h"
#include "hexsave.h"
#include "hexdisplay.h"
#include "hexhistory.h"

SDL_Rect insertPoint(int x, int y)
{
	SDL_Rect res;

	res.x = x;
	res.y = y;
	res.h = 0;
	res.w = 0;
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
	int			gap1, gap2, gap3, gap4;
	int			i, j;
	float		a;
	int			b;
	int			yres;

	topleft = insertPoint(302, 168);
	midleft = insertPoint(302, 176);
	bottomleft = insertPoint(302, 195);
	topmid = insertPoint(317, 168);
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

void initPlayersWithMode(SDL_Surface *str[6], TTF_Font *font, const char *players[NUMPLAYER], int mode, int start, SDL_Color color[4], e_Color ecolor)
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

void init_positionGame(SDL_Rect pos[9])
{
	pos[3] = insertPoint(300, 56);
	pos[4] = insertPoint(357, 116);
	pos[5] = insertPoint(186, 557);
	pos[6] = insertPoint(350, 557);
	pos[7] = insertPoint(520, 557);
	pos[8] = insertPoint(560, 557);
/*
	pos[3].x = 300;
	pos[3].y = 56;
	pos[4].x = 357;
	pos[4].y = 116;
	pos[5].x = 186;
	pos[5].y = pos[6].y = pos[7].y = pos[8].y = 557;
	pos[6].x = 350;
	pos[7].x = 520;
	pos[8].x = 560;
*/
}

int playGame(SDL_Surface* screen, TTF_Font* font, t_hexBoard tab[N][N], int mode, int start, int turn, int lasty, int lastx)
{
	const char* labels[NUMMENU] = {"UNDO", "HISTORY", "PAUSE"};
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
	bool selected[NUMMENU] = {false, false, false};
	SDL_Color color[4] = {{0,0,0,0}, {255,0,0,0}, {255,255,255,0}, {0,0,255,0}};
	int x, y;
	int i, j;
	int flag, pause;
	int play;
	int count;

	i = 0;
	count = 0;
	flag = 0;
	ecolor = RED;
 	imgmain = IMG_Load("images/board.png");
	SDL_BlitSurface(imgmain, NULL, screen, NULL);
	while (i < NUMMENU)
	{
		menu[i] = TTF_RenderText_Solid(font, labels[i], color[2]);
		pos[i] = insertPoint(23, (232 + (i * 50)));
		i++;
	}
	init_positionGame(pos);
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
		initPlayersWithMode(str, font, players, mode, turn, color, tab[lasty][lastx].color);
		blit_Player(screen, str, pos, turn, LASTTURN);
	}
	start = !(start ^ turn);
	initPlayersWithMode(str, font, players, mode, start, color, ecolor);
	blit_Player(screen, str, pos, start, THISTURN);
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
								count = countlinegame();
								undoGame(count);
								return (0);
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
									pause = print_pause(screen, font, tab);
									if (pause == 3 || pause == 2)
										flag = 1;
								}
								if (flag)
								{
									for (j = 0; j < NUMMENU; j++)
										SDL_FreeSurface(menu[j]);
									for (j = 0; j < 6; j++)
										SDL_FreeSurface(str[j]);
									SDL_FreeSurface(imgmain);
									if (pause == 2)
										return (6);
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
					if (point.x != -1 && point.y != -1)
					{
						play = addColorGrid(tab, ecolor, point.y, point.x);
						if (play != 2)
						{
							printPlay(&fgame, ecolor, point.y, point.x);
							count++;
							blit_Player(screen, str, pos, (1 - start), LASTTURN);
							freeandprint(screen, font, &str[4], last_y, color[0], pos[7]);
							freeandprint(screen, font, &str[5], last_x, color[0], pos[8]);
							drawBall(screen, ecolor, point.y, point.x);
							SDL_Flip(screen);
							if (play == 1)
							{
								//SDL_SaveBMP(screen, "lastvictory.bmp");
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
							initPlayersWithMode(str, font, players, mode, start, color, ecolor);
							blit_Player(screen, str, pos, start, THISTURN);
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
						pause = print_pause(screen, font, tab);
						if (pause == 2 || pause == 3)
						{
							for (j = 0; j < NUMMENU; j++)
								SDL_FreeSurface(menu[j]);
							for (j = 0; j < 6; j++)
								SDL_FreeSurface(str[j]);
							SDL_FreeSurface(imgmain);
							if (pause == 2)
								return (6);
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
	}
}

void removetmpfiles(void)
{
	remove(".tmpgame.txt");
	remove(".tmpboard.txt");
	remove(".map.bmp");
}
