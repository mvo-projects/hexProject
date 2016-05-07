#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "hexboard.h"
#include "hexsave.h"

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
		fprintf(fsave, "\\game\n");
	else
	{
		while (!feof(fgame) && fgets(str, 15, fgame) != NULL)
		{
			fprintf(fsave, "%s", str);
			if (feof(fgame))
				break;
		}
	}
	fprintf(fsave, "\\endgame\n");
	fprintf(fsave, "\\endhex\n");
	fclose(fboard);
	fclose(fgame);
	fclose(fsave);
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
