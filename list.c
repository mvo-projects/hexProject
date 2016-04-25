#include <stdlib.h>

typedef enum { RIGHT, LEFT, UP, DOWN, NONE, POSSESS } e_State;

typedef struct		s_Coord
{
	int				x;
	int				y;
}					t_Coord;

typedef struct		s_Cell
{
	t_Coord			xy;
	e_State			state;
	struct s_Cell	*upleft;
	struct s_Cell	*upright;
	struct s_Cell	*left;
	struct s_Cell	*right;
	struct s_Cell	*downleft;
	struct s_Cell	*downright;
}					t_Cell;

typedef struct s_Cell *t_List;

void init_
