#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#define N 11

typedef enum color { DULL, BLUE, RED } e_Color;
typedef enum state { NONE, LEFT, RIGHT, B_POS, R_POS } e_State;

typedef struct		s_Coord
{
	double			x;
	double			y;
}					t_Coord;

typedef struct			s_hexCell
{
	e_Color				color;
	e_State				state;
	struct s_hexCell	*upleft;
	struct s_hexCell	*upright;
	struct s_hexCell	*left;
	struct s_hexCell	*right;
	struct s_hexCell	*downleft;
	struct s_hexCell	*downright;
}						t_hexCell;

typedef struct s_hexCell t_hexTab;

void initGrid(t_hexTab tab[N][N])
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
/*
void initGrid(t_hexTab ***tab)
{
	int i;
	int	j;

	i = 0;
	(*tab[1][0]).color = RED;
	while (i < N)
	{
		j = 0;
		while (j < N)
		{
			tab[i][j]->state = NONE;
			tab[i][j]->color = DULL;
			if (i > 0)
			{
				tab[i][j]->upright = tab[i - 1][j];
				if (j != 0)
					tab[i][j]->upleft = tab[i - 1][j - 1];
			}
			if (i < N - 1)
			{
				tab[i][j]->downleft = tab[i + 1][j];
				if (j < N - 1)
					tab[i][j]->downright = tab[i + 1][j + 1];
			}
			if (j < N - 1)
				tab[i][j]->right = tab[i][j + 1];
			if (j > 0)
				tab[i][j]->left = tab[i][j - 1];
			if (i == 0)
			{
				tab[i][j]->upright = NULL;
				tab[i][j]->upleft = NULL;
			}
			else if (i == N - 1)
			{
				tab[i][j]->downright = NULL;
				tab[i][j]->downleft = NULL;
			}
			if (j == 0)
			{
				tab[i][j]->upleft = NULL;
				tab[i][j]->left = NULL;
			}
			else if (j == N - 1)
			{
				tab[i][j]->right = NULL;
				tab[i][j]->downright = NULL;
			}
			j++;
		}
		i++;
	}
}
*/
bool same_color(t_hexTab *hT, e_Color color)
{
	if (hT != NULL)
	{
		if (hT->color == color)
			return (true);
	}
	return (false);
}

bool win_condition(t_hexTab *hT)
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
/*
void spread(t_hexTab **tab, e_Color color, e_State state)
{
	if (color == BLUE)
	{
		if ((*tab)->upleft != NULL && (*tab)->upleft->state == B_POS)
		{
			spread(&((*tab)->upleft), color, state);
			(*tab)->upleft->state = state;
		}
		if ((*tab)->upright != NULL && (*tab)->upright->state == B_POS)
		{
			spread(&((*tab)->upright), color, state);
			(*tab)->upright->state = state;
		}
		if ((*tab)->left != NULL && (*tab)->left->state == B_POS)
		{
			spread(&((*tab)->left), color, state);
			(*tab)->left->state = state;
		}
		if ((*tab)->right != NULL && (*tab)->right->state == B_POS)
		{
			spread(&((*tab)->right), color, state);
			(*tab)->right->state = state;
		}
		if ((*tab)->downleft != NULL && (*tab)->downleft->state == B_POS)
		{
			spread(&((*tab)->downleft), color, state);
			(*tab)->downleft->state = state;
		}
		if ((*tab)->downright != NULL && (*tab)->downright->state == B_POS)
		{
			spread(&((*tab)->downright), color, state);
			(*tab)->downright->state = state;
		}
	}
	else
	{
		if ((*tab)->upleft != NULL && (*tab)->upleft->state == R_POS)
		{
			printf("HEY\n");
			spread(&((*tab)->upleft), color, state);
			(*tab)->upleft->state = state;
		}
		if ((*tab)->upright != NULL && (*tab)->upright->state == R_POS)
		{
			printf("HEY\n");
			spread(&((*tab)->upright), color, state);
			(*tab)->upright->state = state;
		}
		if ((*tab)->left != NULL && (*tab)->left->state == R_POS)
		{
			printf("HEY\n");
			spread(&((*tab)->left), color, state);
			(*tab)->left->state = state;
		}
		if ((*tab)->right != NULL && (*tab)->right->state == R_POS)
		{
			printf("HEY\n");
			spread(&((*tab)->right), color, state);
			(*tab)->right->state = state;
		}
		if ((*tab)->downleft != NULL && (*tab)->downleft->state == R_POS)
		{
			printf("HEY\n");
			spread(&((*tab)->downleft), color, state);
			(*tab)->downleft->state = state;
		}
		if ((*tab)->downright != NULL && (*tab)->downright->state == R_POS)
		{
			printf("HEY\n");
			spread(&((*tab)->downright), color, state);
			(*tab)->downright->state = state;
		}
	}
}*/

void spread(t_hexTab *tab, e_Color color, e_State state)
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

int addColorGrid(t_hexTab tab[N][N], e_Color color, int x, int y)
{
	if (tab[x][y].color == DULL)
	{
		tab[x][y].color = color;
		if (win_condition(&(tab[x][y])))
			return (1);
		else if (tab[x][y].state == NONE)
		{
			if (color == BLUE)
				tab[x][y].state = B_POS;
			else
				tab[x][y].state = R_POS;
		}
		else
			spread(&(tab[x][y]), color, tab[x][y].state);
		return (0);
	}
	return (2);
}
/*
int addColorGridv2(t_hexTab ***tab, e_Color color, int x, int y)
{
	if (tab[x][y]->color == DULL)
	{
		tab[x][y]->color = color;
		if (win_condition(&(*tab[x][y])))
			return (1);
		else if (tab[x][y]->state == NONE)
		{
			if (color == BLUE)
				tab[x][y]->state = B_POS;
			else
				tab[x][y]->state = R_POS;
		}
		else
			spread(&(tab[x][y]), color, tab[x][y]->state);
		return (0);
	}
	return (2);
}
*/
int main(void)
{
	t_hexTab tab[N][N];
	int i;
	int x;

	initGrid(tab);
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
	return (0);
}
