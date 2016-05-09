#include <stdio.h>
#include "hexboard.h"
#include <stdbool.h>

int main(void)
{
	t_hexBoard board[N][N];
	int i =1;
	printf("\ntest initGrid :");
	initGrid(board, HARD_RESET);
	t_hexCell tmp = board[0][0];
	while (tmp.right!=NULL)
	{
		tmp=*(tmp.right);
		i++;
	}
	if (i==11)
		printf(" OK");
	else
		printf(" KO");
	i=1;
	tmp=board[0][0];
	while (tmp.downleft!=NULL)
	{
		tmp=*(tmp.downleft);
		i++;
	}
	if (i==11)
		printf(" OK");
	else
		printf(" KO");
/*-----------------------------------------------------------------*/
	printf("\ntest same_color :");
	tmp.color=BLUE;
	if(same_color(&tmp,BLUE)==true)
		printf(" OK");
	else
		printf(" KO");
	if(same_color(&tmp,RED)==false)
		printf(" OK");
	else
		printf(" KO");
	if(same_color(&tmp,DULL)==false)
		printf(" OK");
	else
		printf(" KO");
/*-----------------------------------------------------------------*/
	printf("\ntest win_condition :");
	initGrid(board,SOFT_RESET);
	tmp=board[0][0];
	board[0][1].color=RED;
	board[0][1].state=RIGHT;
	tmp.color=RED;
	if(win_condition(&tmp)==true)
		printf(" OK");
	else
		printf(" KO");
	board[0][1].color=BLUE;
	if(win_condition(&tmp)==false)
		printf(" OK");
	else
		printf(" KO");
	initGrid(board,SOFT_RESET);
	board[1][1].color=BLUE;
	board[1][1].state=LEFT;
	board[3][1].color=BLUE;
	board[3][1].state=RIGHT;
	tmp=board[2][1];
	tmp.color=RED;
	if(win_condition(&tmp)==false)
		printf(" OK");
	else
		printf(" KO");
	tmp.color=BLUE;
	if(win_condition(&tmp)==true)
		printf(" OK");
	else
		printf(" KO");
	initGrid(board,SOFT_RESET);
	board[1][1].color=BLUE;
	board[1][1].state=LEFT;
	board[4][1].color=BLUE;
	board[4][1].state=RIGHT;
	tmp=board[2][1];
	tmp.color=BLUE;
	if(win_condition(&tmp)==false)
		printf(" OK");
	else
		printf(" KO");
	
/*-----------------------------------------------------------------*/
	printf("\ntest spread :");
	initGrid(board,SOFT_RESET);
	board[0][1].state=R_POS;
	board[0][2].state=R_POS;
	tmp=board[0][0];
	tmp.color=RED;
	tmp.state=LEFT;
	spread(&tmp,tmp.color,tmp.state);
	if(board[0][2].state==LEFT)
		printf(" OK");
	else
		printf(" KO");
	initGrid(board,SOFT_RESET);
	board[0][1].state=B_POS;
	board[0][2].state=R_POS;
	tmp=board[0][0];
	tmp.color=RED;
	tmp.state=LEFT;
	spread(&tmp,tmp.color,tmp.state);
	if (board[0][2].state!=LEFT)
		printf(" OK");
	else
		printf(" KO");
	initGrid(board,SOFT_RESET);
	board[0][1].state=R_POS;
	board[0][2].state=R_POS;
	tmp=board[0][0];
	tmp.color=BLUE;
	tmp.state=LEFT;
	spread(&tmp,tmp.color,tmp.state);
	if (board[0][2].state!=LEFT)
		printf(" OK");
	else
		printf(" KO");
/*-----------------------------------------------------------------*/
	printf("\ntest addColorGrid :");
	initGrid(board,SOFT_RESET);
	board[0][0].color=RED;
	if (addColorGrid(board,RED,0,0)==2)
		printf(" OK");
	else
		printf (" KO");
	initGrid(board,SOFT_RESET);
	board[0][0].color=RED;
	board[0][0].state=LEFT;
	board[0][2].color=RED;
	board[0][2].state=RIGHT;
	if (addColorGrid(board,RED,0,1)==1)
		printf(" OK");
	else
		printf (" KO");
	initGrid(board,SOFT_RESET);
	if (addColorGrid(board,BLUE,1,0)==0 && board[1][0].state==B_POS)
		printf(" OK");
	else
		printf (" KO");
	initGrid(board,SOFT_RESET);
	if (addColorGrid(board,RED,0,1)==0 && board[0][1].state==R_POS)
		printf(" OK");
	else
		printf (" KO");
	initGrid(board,SOFT_RESET);
	board[0][1].state=R_POS;
	board[0][2].state=R_POS;
	if (addColorGrid(board,RED,0,0)==0 && board[0][2].state==LEFT)
		printf(" OK");
	else
		printf (" KO");
	return (0);
}
