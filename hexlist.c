#include <stdio.h>
#include <stdlib.h>
#include "hexboard.h"

typedef struct s_AIelement
{
	e_Color color;
	int x;
	int y;
	int	cost;
	struct s_AIelement *prev;
}				t_AIelement;

typedef struct		s_Cell
{
	t_AIelement elem;
	struct s_Cell	*next;
}					t_Cell;

typedef struct s_Cell *t_hexList;

typedef struct s_Path
{
	int size;
	t_hexList list;
}				t_Path;

void create_Cell(t_hexList *hL, t_AIelement e, t_AIelement *prev)
{
	if ((*hL = (t_hexList)malloc(sizeof(t_Cell))) == NULL)
	{
		perror("malloc failed\n");
		exit(EXIT_FAILURE);
	}
	(*hL)->elem = e;
	((*hL)->elem).prev = prev;
	(*hL)->next = NULL;
}

t_AIelement createElement(e_Color color, int x, int y, int cost)
{
	t_AIelement elem;

	elem.color = color;
	elem.x = x;
	elem.y = y;
	elem.cost = cost;
	elem.prev = NULL;
	return (elem);
}

int	xycolor(t_AIelement elem, e_Color color)
{
	if (color == BLUE)
		return (elem.y);
	return (elem.x);
}

int infElement(t_AIelement e1, t_AIelement e2)
{
	return (e1.cost <= e2.cost);
}

void initAIgrid(t_AIelement AIgrid[N][N], t_hexBoard tab[N][N])
{
	int i, j;

	for (i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
		{
			AIgrid[i][j].color = tab[i][j].color;
			AIgrid[i][j].y = i;
			AIgrid[i][j].x = j;
			if (AIgrid[i][j].color == DULL)
				AIgrid[i][j].cost = -1;
			else
				AIgrid[i][j].cost = 0;
		}
	}
}

void initPath(t_Path *path)
{
	path->list = NULL;
	path->size = 0;
}

void addList(t_hexList *hL, t_AIelement e, t_AIelement prev)
{
	t_hexList index;
	t_hexList tmp;

	if (*hL == NULL)
		create_Cell(hL, e, &prev);
	else
	{
		index = *hL;
		create_Cell(&tmp, e, &prev);
		while (index->next != NULL && infElement(e, index->next->elem))
			index = index->next;
		tmp->next = index->next;
		index->next = tmp;
	}
}

t_hexList delList(t_hexList *hL)
{
	t_hexList tmp;

	tmp = NULL;
	if (*hL != NULL)
	{
		tmp = *hL;
		*hL = (*hL)->next;
	}
	return (tmp);
}
/*
void newopenlist(t_AIelement grid[N][N], t_hexList *openlist, t_hexList l)
{
	if (*openlist == NULL)


	{
	}
}

t_AIelement spreadAI(t_AIelement grid[N][N], t_hexList *openlist, e_Color color)
{
	int i;
	int j;
	int flag;
	t_hexList tmp;

	if (*openlist != NULL)
	{
		if (xycolor((*openlist)->elem, color) == N)
			return (*openlist)->elem;
		else
		{
			tmp = delList(openlist);
			if (tmp->elem.x == 0)
		 	newopenlist(openlist, tmp);
	}
	
*/
