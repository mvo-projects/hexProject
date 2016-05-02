#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "board.h"

void initGrid(t_hexBoard tab[N][N])
{
	int i;
	int	j;

	i = 0;
	tab[1][0].color = RED;
	while (i < N)
	{
		j = 0;
		while (j < N)
		{
			tab[i][j].state = NONE;
			tab[i][j].color = DULL;
			if (i > 0)
			{
				tab[i][j].upright = &(tab[i - 1][j]);
				if (j != 0)
					tab[i][j].upleft = &(tab[i - 1][j - 1]);
			}
			if (i < N - 1)
			{
				tab[i][j].downleft = &(tab[i + 1][j]);
				if (j < N - 1)
					tab[i][j].downright = &(tab[i + 1][j + 1]);
			}
			if (j < N - 1)
				tab[i][j].right = &(tab[i][j + 1]);
			if (j > 0)
				tab[i][j].left = &(tab[i][j - 1]);
			if (i == 0)
			{
				tab[i][j].upright = NULL;
				tab[i][j].upleft = NULL;
			}
			else if (i == N - 1)
			{
				tab[i][j].downright = NULL;
				tab[i][j].downleft = NULL;
			}
			if (j == 0)
			{
				tab[i][j].upleft = NULL;
				tab[i][j].left = NULL;
			}
			else if (j == N - 1)
			{
				tab[i][j].right = NULL;
				tab[i][j].downright = NULL;
			}
			j++;
		}
		i++;
	}
}

bool same_color(t_hexBoard *hT, e_Color color)
{
	if (hT != NULL)
	{
		if (hT->color == color)
			return (true);
	}
	return (false);
}

bool win_condition(t_hexBoard *hT)
{
	bool left;
	bool right;

	left = false;
	right = false;
	if (hT->upleft == NULL)
	{
		if (hT->downleft == NULL && hT->color == BLUE)
		{
			hT->state = RIGHT;
			right = true;
		}
		else
		{
			hT->state = LEFT;
			left = true;
		}
	}
	if (hT->downright == NULL)
	{
		if (hT->upright == NULL && hT->color == BLUE)
		{
			hT->state = LEFT;
			left = true;
		}
		else
		{
			hT->state = RIGHT;
			right = true;
		}
	}
	if (same_color(hT->upleft, hT->color))
	{
		if (hT->upleft->state == LEFT)
		{
			hT->state = LEFT;
			left = true;
		}
		else if (hT->upleft->state == RIGHT)
		{
			hT->state = RIGHT;
			right = true;
		}
	}
	if (same_color(hT->upright, hT->color))
	{
		if (hT->upright->state == LEFT)
		{
			hT->state = LEFT;
			left = true;
		}
		else if (hT->upright->state == RIGHT)
		{
			hT->state = RIGHT;
			right = true;
		}
	}
	if (same_color(hT->left, hT->color))
	{
		if (hT->left->state == LEFT)
		{
			hT->state = LEFT;
			left = true;
		}
		else if (hT->left->state == RIGHT)
		{
			hT->state = RIGHT;
			right = true;
		}
	}
	if (same_color(hT->right, hT->color))
	{
		if (hT->right->state == LEFT)
		{
			hT->state = LEFT;
			left = true;
		}
		else if (hT->right->state == RIGHT)
		{
			hT->state = RIGHT;
			right = true;
		}
	}
	if (same_color(hT->downleft, hT->color))
	{
		if (hT->downleft->state == LEFT)
		{
			hT->state = LEFT;
			left = true;
		}
		else if (hT->downleft->state == RIGHT)
		{
			hT->state = RIGHT;
			right = true;
		}
	}
	if (same_color(hT->downright, hT->color))
	{
		if (hT->downright->state == LEFT)
		{
			hT->state = LEFT;
			left = true;
		}
		else if (hT->downright->state == RIGHT)
		{
			hT->state = RIGHT;
			right = true;
		}
	}
	return (left & right);
}

void spread(t_hexBoard *tab, e_Color color, e_State state)
{
	if (color == BLUE)
	{
		if (tab->upleft != NULL && tab->upleft->state == B_POS)
		{
			tab->upleft->state = state;
			spread(tab->upleft, color, state);
		}
		if (tab->upright != NULL && tab->upright->state == B_POS)
		{
			tab->upright->state = state;
			spread(tab->upright, color, state);
		}
		if (tab->left != NULL && tab->left->state == B_POS)
		{
			tab->left->state = state;
			spread(tab->left, color, state);
		}
		if (tab->right != NULL && tab->right->state == B_POS)
		{
			tab->right->state = state;
			spread(tab->right, color, state);
		}
		if (tab->downleft != NULL && tab->downleft->state == B_POS)
		{
			tab->downleft->state = state;
			spread(tab->downleft, color, state);
		}
		if (tab->downright != NULL && tab->downright->state == B_POS)
		{
			tab->downright->state = state;
			spread(tab->downright, color, state);
		}
	}
	else
	{
		if (tab->upleft != NULL && tab->upleft->state == R_POS)
		{
			tab->upleft->state = state;
			spread(tab->upleft, color, state);
		}
		if (tab->upright != NULL && tab->upright->state == R_POS)
		{
			tab->upright->state = state;
			spread(tab->upright, color, state);
		}
		if (tab->left != NULL && tab->left->state == R_POS)
		{
			tab->left->state = state;
			spread(tab->left, color, state);
		}
		if (tab->right != NULL && tab->right->state == R_POS)
		{
			tab->right->state = state;
			spread(tab->right, color, state);
		}
		if (tab->downleft != NULL && tab->downleft->state == R_POS)
		{
			tab->downleft->state = state;
			spread(tab->downleft, color, state);
		}
		if (tab->downright != NULL && tab->downright->state == R_POS)
		{
			tab->downright->state = state;
			spread(tab->downright, color, state);
		}
	}
}

int addColorGrid(t_hexBoard tab[N][N], e_Color color, int y, int x)
{
	if (tab[y][x].color == DULL)
	{
		tab[y][x].color = color;
		if (win_condition(&(tab[y][x])))
			return (1);
		else if (tab[y][x].state == NONE)
		{
			if (color == BLUE)
				tab[y][x].state = B_POS;
			else
				tab[y][x].state = R_POS;
		}
		else
			spread(&(tab[y][x]), color, tab[y][x].state);
		return (0);
	}
	return (2);
}

void loadGame(const char *name, t_hexBoard tab[N][N])
{
	FILE	*fhex;
	char	*str;
	e_Color color;
	int		x;
	int		y;

	str = (char *)malloc(sizeof(char) * 20);
	assert(str != NULL);
	if ((fhex = fopen(name, "r")) != NULL)
	{
		fseek(fhex, sizeof(char) * (N * (N + 1) + 28), SEEK_SET);
		while (fgets(str, 20, fhex) != NULL && strcmp(str, "\\endgame\n"))
		{
			if (str[6] == 'B')
				color = BLUE;
			else if (str[6] == 'R')
				color = RED;
			else
			{
				free(str);
				fprintf(stderr, "1 : erreur dans le fichier save.txt\n");
				exit(EXIT_FAILURE);
			}
			x = atoi(&str[8]);
			y = atoi(&str[10]);
			if (x < 0 || x > 10 || y < 0 || y > 10)
			{
				free(str);
				fprintf(stderr, "2 : erreur dans le fichier save.txt\n");
				exit(EXIT_FAILURE);
			}
			if (addColorGrid(tab, color, x, y) == 1)
			{
				if (color == BLUE)
					printf("BLUE WON !!\n");
				else
					printf("RED WON !!\n");
			}
		}
		fclose(fhex);
	}
	free(str);
}

void printPlay(FILE *fgame, e_Color color, int x, int y)
{
	if (fgame == NULL)
	{
		fgame = fopen("game.txt", "a+");
		fprintf(fgame, "\\game\n");
	}
	if (color == BLUE)
		fprintf(fgame, "\\play B %d %d\n", x, y);
	else
		fprintf(fgame, "\\play R %d %d\n", x, y);
}

void saveBoard(const char *name, t_hexBoard tab[N][N])
{
	FILE		*fboard;
	int			i;
	int			j;

	fboard = fopen(name, "w");
	fprintf(fboard, "\\board\n");
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
		{
			if (tab[i][j].color == RED)
				fprintf(fboard, "R");
			else if (tab[i][j].color == BLUE)
				fprintf(fboard, "B");
			else
				fprintf(fboard, ".");
		}
		fprintf(fboard, "\n");
	}
	fprintf(fboard, "\\endboard\n");
	fclose(fboard);
}

/*

int main(void)
{
	t_hexBoard tab[N][N];

	initGrid(tab);
	loadGame("save.txt", tab);
	addColorGrid(tab, RED, 3, 4);
	addColorGrid(tab, RED, 2, 4);
	printf("%d\n", tab[3][4].state);
	for (i = 0; i < 5; i++)
	{
		if((x = addColorGrid(tab, RED, 1, i)) == 1)
			printf("RED WIN\n");
		else if (x == 0)
			printf("NOT YET\n");
	}
	for (i = N - 1; i > 5; i--)
	{
		if((x = addColorGrid(tab, RED, 1, i)) == 1)
			printf("RED WIN\n");
		else if (x == 0)
			printf("NOT YET\n");
	}
	if((x = addColorGrid(tab, RED, 1, 5)) == 1)
			printf("RED WIN\n");
	if (tab[0][0].color == RED)
		printf("OK");
	printf("%d\n", tab[3][4].state);
	printf("%d\n", tab[2][4].state);
	saveBoard("board.txt", tab);
	remove("board.txt");
	return (0);
}*/
