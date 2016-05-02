#ifndef BOARD_H
# define BOARD_H
# include <stdbool.h>
# define N 11

typedef enum color { DULL, BLUE, RED } e_Color;
typedef enum game_mode { P_vs_P, P_vs_IA1, P_vs_IA2 } e_GameMode; 
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

typedef struct s_hexCell t_hexBoard;

void initGrid(t_hexBoard tab[N][N]);
bool same_color(t_hexBoard *hT, e_Color color);
bool win_condition(t_hexBoard *hT);
void spread(t_hexBoard *tab, e_Color color, e_State state);
int	addColorGrid(t_hexBoard tab[N][N], e_Color color, int x, int y);
void loadGame (const char *name, t_hexBoard tab[N][N]);
void printPlay(FILE *fgame, e_Color color, int x, int y);
void saveBoard(const char *name, t_hexBoard tab[N][N]);

#endif
