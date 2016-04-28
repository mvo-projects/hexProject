#include <stdlib.h>
#define N 11

typedef enum { NONE, RIGHT, LEFT, UP, DOWN, B_POS, R_POS } e_State;
typedef enum { NONE, BLUE, RED } e_Color

typedef struct		s_Coord
{
	double			x;
	double			y;
}					t_Coord;

typedef struct		s_hexCell
{
	e_Color			color;
	e_State			state;
	struct s_Cell	*upleft;
	struct s_Cell	*upright;
	struct s_Cell	*left;
	struct s_Cell	*right;
	struct s_Cell	*downleft;
	struct s_Cell	*downright;
}					t_hexCell;

typedef struct s_Cell *t_hexTab;

void initGrid(t_hexTab ***tab)
{
	int i;
	int	j;

	i = 0;
	while (i < N)
	{
		j = 0;
		while (j < N)
		{
			tab[i][j]->state = NONE;
			tab[i][j]->color = NONE;
			if (i != 0)
			{
				(*tab[i][j])->upright = &(*tab[i - 1][j]);
				if (j != 0)
					(*tab[i][j])->upleft = &(*tab[i - 1][j - 1]);
			}
			if (i != N - 1)
			{
				(*tab[i][j])->downleft = &(*tab[i + 1][j]);
				if (j != N - 1)
					(*tab[i][j])->downright = &(*tab[i + 1][j + 1]);
			}
			if (j != N - 1)
				(*tab[i][j])->right = &(*tab[i][j + 1]);
			if (j != 0)
				(*tab[i][j])->left = &(*tab[i][j - 1]);
			if (i == 0)
			{
				(*tab[i][j])->upright = NULL;
				(*tab[i][j])->upleft = NULL;
			}
			else if (i == N - 1)
			{
				(*tab[i][j])->downright = NULL;
				(*tab[i][j])->downleft = NULL;
			}
			if (j == 0)
			{
				(*tab[i][j])->upleft = NULL;
				(*tab[i][j])->left = NULL;
			}
			else if (j == N - 1)
			{
				(*tab[i][j])->right = NULL;
				(*tab[i][j])->downright = NULL;
			}
			j++;
		}
		i++;
	}
}


