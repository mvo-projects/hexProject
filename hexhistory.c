#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include "hexsave.h"
#include "hexgame.h"
#include "hexhistory.h"

void stock_history(char *str[MAXHISTORY])
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
		while (nb_line > MAXHISTORY)
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
				if (feof(fgame) != 0)
					break;
				i++;
			}
		}
		fclose(fgame);
	}
	free(tmp);
}

void init_history(TTF_Font *font, SDL_Rect pos[12], SDL_Surface *modes[12], char *str[MAXHISTORY])
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
	char *str[MAXHISTORY];
	bool selected = false;
	SDL_Color color[3] = {{0,0,0,0}, {255,0,0,0}, {255,255,255,0}};
	SDL_Event event;
	int	i;
	int x, y;

	for (i = 0; i < MAXHISTORY; i++)
	{
		str[i] = (char *)malloc(sizeof(char) * 15);
		assert(str[i] != NULL);
		str[i][0] = '\0';
	}
	stock_history(str);
	modes[0] = TTF_RenderText_Solid(font, back, color[2]);
	modes[1] = TTF_RenderText_Solid(font, title, color[1]);
	pos[0] = insertPoint(55, 555);
	pos[1] = insertPoint(342, 145);
	init_history(font, pos, modes, str);
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

