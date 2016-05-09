#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hexsave.h"

int main(void)
{
	FILE *tmp;
	char str1[20]="";
	char str2[300]="";
	int py;
	int px;
	t_hexBoard board[N][N];
	printf("\ntest countlinegame :");
	if ((tmp = fopen(".tmpgame.txt", "w")) != NULL)
	{
		fprintf(tmp,"\\game\n\\play R 2 1\n\\play B 1 2\n\\play R 3 1\n");
		fclose(tmp);
		if (countlinegame()==3)
			printf(" OK");
		else
			printf(" KO");
		remove(".tmpgame.txt");
	}
	
	printf("\ntest printPlay :");
	if ((tmp = fopen(".tmpgame.txt", "w+")) != NULL)
	{
		fprintf(tmp,"\\play R 1 2\n");
		printPlay(&tmp,BLUE,4,2);
		fseek(tmp,0,SEEK_SET);
		while (fgets(str1,20,tmp) != NULL)
			strcat(str2,str1);
		if (strcmp(str2,"\\play R 1 2\n\\play B 4 2\n")==0)
			printf(" OK");
		else
			printf(" KO");
		fclose(tmp);
		remove(".tmpgame.txt");
	}
	
	printf("\ntest undoGame :");
	str2[0]='\0';
	if ((tmp = fopen(".tmpgame.txt", "w")) != NULL)
	{
		fprintf(tmp,"\\play R 1 2\n\\play B 1 3\n");
		fseek(tmp,0,SEEK_SET);
		fclose(tmp);
		undoGame(1);
		tmp=fopen(".tmpgame.txt", "r");
		while (fgets(str1,20,tmp) != NULL)
			strcat(str2,str1);
		if (strcmp(str2,"\\play R 1 2\n")==0)
			printf(" OK");
		else
			printf(" KO");
		fclose(tmp);
	}
	remove(".tmpgame.txt");
	
	printf("\ntest loadUndo :");
	if ((tmp = fopen(".tmpgame.txt", "w")) != NULL)
	{
		fprintf(tmp,"\\play R 1 2\n\\play B 1 3\n");
		fseek(tmp,0,SEEK_SET);
		fclose(tmp);
		initGrid(board,HARD_RESET);
		loadUndo(board,&py,&px);
		if(py==1 && px==3 && board[1][3].color==BLUE)
			printf(" OK");
		else
			printf(" KO");
	}
	fclose(tmp);
	remove(".tmpgame.txt");
	
	printf("\ntest saveBoard :");
	str2[0]='\0';
	initGrid(board,HARD_RESET);
	board[0][0].color=RED;
	board[3][4].color=BLUE;
	board[1][2].color=RED;
	saveBoard("board.txt",board);
	if ((tmp = fopen("board.txt", "r")) != NULL)
	{
		fseek(tmp,0,SEEK_SET);
		while (fgets(str1,20,tmp) != NULL)
			strcat(str2,str1);
		if (strcmp(str2,"\\board\nR..........\n..R........\n...........\n....B......\n...........\n...........\n...........\n...........\n...........\n...........\n...........\n\\endboard\n")==0)
			printf(" OK");
		else
			printf(" KO");
		fclose(tmp);
	}
	remove("board.txt");

	
	return (0);
}
