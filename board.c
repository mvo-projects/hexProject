#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "board.h"

void initGrid(t_hexBoard tab[N][N], int mode)
{
	int i;
	int	j;

	i = 0;
	while (i < N)
	{
		j = 0;
		while (j < N)
		{
			tab[i][j].state = NONE;
			tab[i][j].color = DULL;
			if (mode == HARD_RESET)
			{
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
	if (hT->left == NULL && hT->color == RED)
	{
		hT->state = LEFT;
		left = true;
	}
	else if (hT->upright == NULL && hT->color == BLUE)
	{
		hT->state = LEFT;
		left = true;
	}
	else if (hT->right == NULL && hT->color == RED)
	{
		hT->state = RIGHT;
		right = true;
	}
	else if (hT->downleft == NULL && hT->color == BLUE)
	{
		hT->state = RIGHT;
		right = true;
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

void undoGame(int line)
{
	FILE	*old_one;
	FILE	*new_one;
	char	*str;

	old_one = NULL;
	new_one = NULL;
	str = (char *)malloc(sizeof(char) * 15);
	assert(str != NULL);
	if (line != 0 && (old_one = fopen(".tmpgame.txt", "r")) != NULL)
	{
		if ((new_one = fopen(".tmpgame1.txt", "w")) != NULL)
		{
			while (line != 0 && feof(old_one) == 0 && fgets(str, 15, old_one) != NULL)
			{
				if (feof(old_one) != 0)
					break;
				line--;
				fprintf(new_one, "%s", str);
			}
		}
	}
	if (old_one != NULL)
		fclose(old_one);
	if (new_one != NULL)
		fclose(new_one);
	free(str);
	remove(".tmpgame.txt");
	rename(".tmpgame1.txt", ".tmpgame.txt");
}

int loadUndo(t_hexBoard tab[N][N], int *py, int *px)
{
	FILE *fgame;
	char *str;
	e_Color color;
	int		i;
	int		turn;

	turn = 1;
	str = (char *)malloc(sizeof(char) * 15);
	assert(str != NULL);
	if ((fgame = fopen(".tmpgame.txt", "r")) != NULL)
	{
		i = 0;
		while (feof(fgame) == 0 && fgets(str, 15, fgame) != NULL)
		{
			if (feof(fgame) != 0)
				break;
			if (i != 0)
			{
				if (str[6] == 'B')
					color = BLUE;
				else
					color = RED;
				*py = atoi(&str[8]);
				*px = atoi(&str[10]);
				addColorGrid(tab, color, *py, *px);
				turn = 1 - turn;
			}
			else
				i = 1;
		}
	}
	return (turn);
}

int loadGame(t_hexBoard tab[N][N], int *y, int *x)
{
	FILE	*fhex;
	FILE	*fgame;
	char	*str;
	e_Color color;
	int		turn;

	turn = 1;
	str = (char *)malloc(sizeof(char) * 15);
	assert(str != NULL);
	if ((fhex = fopen("save.txt", "r")) != NULL)
	{
		if ((fgame = fopen(".tmpgame.txt", "w")) == NULL)
		{
			fprintf(stderr, "failed to open .tmpgame\n");
			fclose(fhex);
			free(str);
			exit(EXIT_FAILURE);
		}
		fprintf(fgame, "\\game\n");
		fseek(fhex, sizeof(char) * (N * (N + 1) + 28), SEEK_SET);
		while (fgets(str, 15, fhex) != NULL && strcmp(str, "\\endgame\n"))
		{
			fprintf(fgame, "%s", str);
			if (feof(fhex) != 0)
				break;
			if (str[6] == 'B')
				color = BLUE;
			else if (str[6] == 'R')
				color = RED;
			*y = atoi(&str[8]);
			*x = atoi(&str[10]);
			addColorGrid(tab, color, *y, *x);
			turn = 1 - turn;
		}
		fclose(fhex);
		fclose(fgame);
	}
	free(str);
	return (turn);
}


int countlinegame(void)
{
	FILE *fgame;
	char *str;
	int	count;

	count = 0;
	str = (char *)malloc(sizeof(char) * 15);
	if ((fgame = fopen(".tmpgame.txt", "r")) != NULL)
	{
		while (feof(fgame) == 0 && fgets(str, 15, fgame) != NULL)
		{
			if (feof(fgame) != 0)
				break;
			count++;
		}
		count = count - 1;
		fclose(fgame);
	}
	return (count);
}

void printPlay(FILE **fgame, e_Color color, int x, int y)
{
	if (*fgame == NULL)
	{
		*fgame = fopen(".tmpgame.txt", "a");
		if (countlinegame() < 0)
			fprintf(*fgame, "\\game\n");
	}
	if (color == BLUE)
		fprintf(*fgame, "\\play B %d %d\n", x, y);
	else
		fprintf(*fgame, "\\play R %d %d\n", x, y);
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
	undoGame("save.txt", 10);
	//loadGame("save.txt", tab);
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
