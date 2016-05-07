#include <stdbool.h>
#include <stddef.h>
#include "hexboard.h"

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

