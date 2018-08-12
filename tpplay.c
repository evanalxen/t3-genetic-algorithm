#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "tph.h"

int main(){

	char DNA[501] = "";
	puts("Enter DNA:");
	gets(DNA);
	int i = 0;
	/////////////ifsthensmalloc//////////////////
	char** ifs1 = (char**) malloc (DL*sizeof(char*));
	for (i = 0; i<DL ; i++){
		ifs1[i] = (char*) malloc (8*sizeof(char));
		strcpy(ifs1[i], "");
	}
	
	char** thens1 = (char**) malloc (DL*sizeof(char*));
	for (i = 0; i<DL ; i++){
		thens1[i] = (char*) malloc (8*sizeof(char));
		strcpy(thens1[i], "");
	}

	char* token;
	token = (char*) malloc (10*sizeof(char));

	/////ifs thens1//////////////////////////
	i=0;
	token = strtok(DNA, "~");
	while (token != NULL) {
		strcpy(ifs1[i], token);
		token = strtok(NULL, "~");
		strcpy(thens1[i], token);
		token = strtok(NULL, "~");
		i++;
	}	

	////////////plays////////////////////////
	int kp = 1;
	int turn = -1; //turn = 1 when pc goes first, -1 otherwise
	int vic = 0;
	int board[9] = {0,0,0,0,0,0,0,0,0};
	int pf[9];
	int tie = 0;
	int a,act,valid;
	
	while (kp == 1) {

		for (int i = 0; i<9; i++) board[i] = 0;
		vic = 0;
		turn = turn * -1;
		///////////////////USER STARTS
		if (turn == 1){	
			puts("You are '1'. Start.");	
			while ( vic == 0 ) {
				
				printb(board);

				
				do {
					valid = 1;
					printf("move: ");
					scanf("%d", &a);
					if ( a < 1 || a > 9 ) valid = 0;
					if ( board[a-1] != 0) valid = 0;
				} while (valid == 0);
				//update board
				board[a-1] = 1;

				//victory check
				vic = victory_check(board, 1);
	
				//tie check
				tie = 1;
				for (int i = 0 ; i<9; i++) if (board[i] == 0) tie = 0;
				if (tie == 1 && vic == 0) vic = 3;

				//ai moves
				if(vic == 0) {
					prob_get(ifs1, thens1, board, 2, pf);
					act = action_check(pf, board);

					//update board
					board[act] = 2;
				

				//victory check
				vic = victory_check(board, 2);

				//tie check
				tie = 1;
				for (int i = 0 ; i<9; i++) if (board[i] == 0) tie = 0;
				if (tie == 1 && vic == 0) vic = 3;

				}

			}
			switch(vic){
				case 1:
				printf("Winner is user! Winning board:\n");
				printb(board);
				break;
				case 2:
				printf("Winner is AI! Winning board:\n");
				printb(board);
				break;
				case 3:
				printf("It is a tie! Ending board:\n");
				printb(board);
				break;
			}

			
			do {
				valid = 1;
				printf("\nPlay again? (1/0)");
				scanf("%d", &kp);
				if (kp != 0 && kp != 1) valid = 0;
			} while (valid == 0);
		}
		//////////////////////AI STARTS

		if (turn == -1){
			puts("You are '2'. AI moves first.");
			while ( vic == 0 ) {

				//ai moves
				if(vic == 0) {
					prob_get(ifs1, thens1, board, 1, pf);
					act = action_check(pf, board);

					//update board
					board[act] = 1;
				

				//victory check
				vic = victory_check(board, 1);

				//tie check
				tie = 1;
				for (int i = 0 ; i<9; i++) if (board[i] == 0) tie = 0;
				if (tie == 1 && vic == 0) vic = 3;

				}
				
				printb(board);

				if (vic == 0){
					do {
						valid = 1;
						printf("move: ");
						scanf("%d", &a);
						if ( a < 1 || a > 9 ) valid = 0;
						if ( board[a-1] != 0) valid = 0;
					} while (valid == 0);
					//update board
					board[a-1] = 2;

					//victory check
					vic = victory_check(board, 2);
	
					//tie check
					tie = 1;
					for (int i = 0 ; i<9; i++) if (board[i] == 0) tie = 0;
					if (tie == 1 && vic == 0) vic = 3;
				}



			}
			switch(vic){
				case 2:
				printf("Winner is user! Winning board:\n");
				printb(board);
				break;
				case 1:
				printf("Winner is AI! Winning board:\n");
				printb(board);
				break;
				case 3:
				printf("It is a tie! Ending board:\n");
				printb(board);
				break;
			}

			
			do {
				valid = 1;
				printf("\nPlay again? (1/0)");
				scanf("%d", &kp);
				if (kp != 0 && kp != 1) valid = 0;
			} while (valid == 0);
		}
			
	}

	return 0;
}