#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "tph.h"

int victory_check(int* board, int last_player){
	//combinations:
	//012 345 678 n+i, i++, n+=3
	//036 147 258 n+3i, i++, n+=1
	//048         n+4i, i++, n=0
	//246         n+2i, i++, n=2

	int tmp[3];
	int i;

	for (i = 0; i<3; i++) tmp[i] = board[0+i];
	if (tmp[0] == tmp [1] && tmp[0] == tmp[2]  && tmp[0] == last_player) return last_player;
	for (i = 0; i<3; i++) tmp[i] = board[3+i];
	if (tmp[0] == tmp [1] && tmp[0] == tmp[2]  && tmp[0] == last_player) return last_player;
	for (i = 0; i<3; i++) tmp[i] = board[6+i];
	if (tmp[0] == tmp [1] && tmp[0] == tmp[2]  && tmp[0] == last_player) return last_player;

	for (i = 0; i<3; i++) tmp[i] = board[0+3*i];
	if (tmp[0] == tmp [1] && tmp[0] == tmp[2]  && tmp[0] == last_player) return last_player;
	for (i = 0; i<3; i++) tmp[i] = board[1+3*i];
	if (tmp[0] == tmp [1] && tmp[0] == tmp[2]  && tmp[0] == last_player) return last_player;
	for (i = 0; i<3; i++) tmp[i] = board[2+3*i];
	if (tmp[0] == tmp [1] && tmp[0] == tmp[2]  && tmp[0] == last_player) return last_player;

	for (i = 0; i<3; i++) tmp[i] = board[0+4*i];
	if (tmp[0] == tmp [1] && tmp[0] == tmp[2]  && tmp[0] == last_player) return last_player;
	for (i = 0; i<3; i++) tmp[i] = board[2+2*i];
	if (tmp[0] == tmp [1] && tmp[0] == tmp[2]  && tmp[0] == last_player) return last_player;

	return 0;
}

int condition_check(char* condition, int* board, int player){

	//check size
	int i,size=0, true = 1;
	for (i=0; i<30 && condition[i] != '\0'; i++) size++;

	int opponent;

	if (player == 1) opponent = 2;
	if (player == 2) opponent = 1;

	for (i=0; i<size; i++){
		if ((int)condition[i] >= 97 && (int)condition[i] <= 105 && board[((int)condition[i]) - 97] != 0) true=0;
		if ((int)condition[i] >= 106 && (int)condition[i] <= 114 && board[((int)condition[i]) - 106] != player) true=0;
		if ((int)condition[i] >= 115 && (int)condition[i] <= 123 && board[((int)condition[i]) - 115] != opponent) true=0;
	}
	
	return true;
}

int action_add(char* action, int* prob){

		prob[((int)action[0])-49] += 1;
		return 0;
}

int action_check(int* prob, int* board){
	int true=1;
	int valid[9] = {1,1,1,1,1,1,1,1,1};

	//check validity of all actions
	int i;
	for (i=0; i<9; i++) if (board[i] != 0) valid[i] = 0;

	//every single action invalid
	int tie = 1;
	for (i=0; i<9; i++) if (valid[i] != 0) tie = 0;
	if (tie == 1) return 9;

	//check highest probability:
	int high_p=0;
	for (i=0; i<9; i++){
		if (valid[i] == 1 && prob[i] > high_p) high_p = prob[i];
	}	
	
	//check no of highest probabilities:
	int count = 0;
	for (i=0; i<9; i++){
		if (valid[i] == 1 && prob[i] == high_p) count++;
	}
	
	//get random no between 1 to count inclusive, unless count==1
	int trigger;
	if (count != 1){
		do{
			trigger = (rand()%(count+1));
		} while (trigger == 0);
	} else trigger = 1;
	
	//step through until count == trigger
	count = 0;
	for (i=0; i<9; i++){
		if (valid[i] == 1 && prob[i] == high_p) count++;
		if (count == trigger) return i;
	}

	return 0;
}

void gen_DNA(int len, char* output){
	
	int gene_count = 0;
	int c_n , c_c , g_d , a_n;
	char dummystr[2] = " ";
	char DNA[1000]="";

	for(; gene_count < len; gene_count++){
		strcat(DNA, "~");
		c_n = (rand()%GMS)+1;
		for (c_c = 0; c_c < c_n; c_c++) {
			g_d = (rand()%27)+97;
			dummystr[0] = (char)g_d;
			strcat(DNA, dummystr);
		}
		strcat(DNA, "~");
		a_n = (rand()%9)+49;
		dummystr[0] = a_n;
		strcat(DNA, dummystr);
	}

	strcpy(output, DNA);
	return;
}

void prob_get(char** ifs, char** thens, int* board, int player, int* prob){
	
	int i;
	int con_ac;	
	for ( i = 0; i<9 ; i++) prob[i] = 0;

	for (i = 0; i < DL; i++) {
		con_ac = condition_check(ifs[i], board, player);
		if (con_ac == 1) action_add(thens[i], prob);
	}



	return;
}

void printb(int* board){
	
	for (int i = 0; i<9; i++){
		if (i==3 || i==6) printf("-\n-   -   -   -\n");
		printf("- %d ", board[i]);
		if (i==8) printf("-");
	}
	printf("\n=============\n");
	return;
}

void play( char** ifs1, char** ifs2, char** thens1, char** thens2, int* board, int* victory ) {
	
	//temp probability function pf
	int pf[9];
	//temp action variable
	int act;
	//victory variable
	int vic;

	//player 1 plays
///////////////////////////////////////////////

	prob_get(ifs1, thens1, board, 1, pf);

	act = action_check(pf, board);

	//tiebreaker
	if (act == 9) {
		*victory = 3;	
		return;
	}

	//update board
	board[act] = 1;

	//victory check
	vic = victory_check(board, 1);
	if (vic == 1) {
		*victory = 1;
		return;
	}


	//player 2 plays
	prob_get(ifs2, thens2, board, 2, pf);

	act = action_check(pf, board);

	//tiebreaker
	if (act == 9) {
		*victory = 3;	
		return;
	}

	//update board
	board[act] = 2;
	//victory check
	vic = victory_check(board, 2);
	if (vic == 2) {
		*victory = 2;
		return;
	}

	
	*victory = 0;
	return;
}

int get_win( char* DNA1o, char* DNA2o ){
	int victory = 0;
	int* pvic = &victory;
	int i;
	int board[9] = {0,0,0,0,0,0,0,0,0};
////////////////////////////////////////////////////
	int con_ac;



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

	char** ifs2 = (char**) malloc (DL*sizeof(char*));
	for (i = 0; i<DL ; i++){
		ifs2[i] = (char*) malloc (8*sizeof(char));
		strcpy(ifs2[i], "");
	}
	
	char** thens2 = (char**) malloc (DL*sizeof(char*));
	for (i = 0; i<DL ; i++){
		thens2[i] = (char*) malloc (8*sizeof(char));
		strcpy(thens2[i], "");
	}

	char* token;
	token = (char*) malloc (10*sizeof(char));


	i=0;

	char DNA1[700];
	char DNA2[700];

	strcpy(DNA1, DNA1o);
	strcpy(DNA2, DNA2o);


	
/////////////ifs thens1////////////////////////////////////
	token = strtok(DNA1, "~");
	while (token != NULL) {
		strcpy(ifs1[i], token);
		token = strtok(NULL, "~");
		strcpy(thens1[i], token);
		token = strtok(NULL, "~");
		i++;
	}
////////////ifs thens 2//////////////////////////////////
	i=0;

	token = strtok(DNA2, "~");

	while (token != NULL) {
		strcpy(ifs2[i], token);
		token = strtok(NULL, "~");
		strcpy(thens2[i], token);
		token = strtok(NULL, "~");
		i++;
	}
//////////////////////////////////////////////////////////
	while ( victory == 0 ) {
		play(ifs1, ifs2, thens1, thens2, board, pvic);
	}

	for (i = 0; i<DL ; i++){
		free(ifs1[i]);
		free(thens1[i]);
		free(ifs2[i]);
		free(thens2[i]);
	}
	free(ifs1);
	free(thens1);
	free(ifs2);
	free(thens2);

	switch(victory){
		case 1:
		return 1;
		break;
		case 2:
		return 2;
		break;
		case 3:
		return 3;
		break;
	}
}

void wait(double ms){
	time_t prev = time(NULL);
	time_t curr = time(NULL);
	while((double)difftime(curr,prev) <= ms){
		curr = time(NULL);
	}
	return;
}

void gene_splice(char* DNA1o, char* DNA2o, char* child){
	
	char DNA1[701];
	char DNA2[701];



	strcpy(DNA1, DNA1o);



	
	strcpy(DNA2, DNA2o);


	
	char** set1;
	char** set2;

	set1 = (char**) malloc (DL*sizeof(char*));
	set2 = (char**) malloc (DL*sizeof(char*));

	int i;
	for (i = 0; i<DL; i++) {
		set1[i] = (char*) malloc (11*sizeof(char));
		strcpy(set1[i], "~");
	}
	for (i = 0; i<DL; i++) {
		set2[i] = (char*) malloc (11*sizeof(char));
		strcpy(set2[i], "~");
	}



	char* token;
	token = (char*) malloc(6*sizeof(char));

	//sets of genes for DNA1
	token = strtok(DNA1, "~");
	strcat(set1[0], token);
	strcat(set1[0], "~");
	i = 0;
	token = strtok(NULL, "~");
	strcat(set1[0], token);	

	token = strtok(NULL, "~");
	while (token != NULL) {
		i++;
		strcat(set1[i], token);
		strcat(set1[i], "~");
		token = strtok(NULL, "~");
		strcat(set1[i], token);
		token = strtok(NULL, "~");
	}

	

	//sets of genes for DNA2
	token = strtok(DNA2, "~");
	strcat(set2[0], token);
	strcat(set2[0], "~");
	i = 0;
	token = strtok(NULL, "~");
	strcat(set2[0], token);	



	token = strtok(NULL, "~");
	while (token != NULL) {
		i++;
		if(token == NULL) puts ( "warning wawrning token null here");
		strcat(set2[i], token);
		strcat(set2[i], "~");
		token = strtok(NULL, "~");
		strcat(set2[i], token);
		token = strtok(NULL, "~");
	}



	//dots arrays with DL3/4 '1's
	int* pick;
	int dummyint;
	pick = (int*) malloc (DL*sizeof(int));

	for(i = 0; i<DL; i++) pick[i] = 0;

	for (i=0; i<((DL/2)+(DL/4)); i++) {
		dummyint = (rand()%DL);
		while (pick[dummyint] != 0) {
			dummyint = (rand()%DL);
		}
		pick[dummyint] = 1;
	}


	
	//puts all the genes
	char* dDNA;
	dDNA = (char*) malloc (500*sizeof(char));
	strcpy(dDNA, "");

	int chance = 0;
	char dummy[15] = "";
	for (i=0; i<DL; i++) {

		//mutates the gene chance (0.1)
		chance = rand()%100;
		gen_DNA(1, dummy);
		if (chance <= MC) {
			strcat(dDNA, dummy);
			//puts("mutate done");
		}			
		else if (pick[i] == 1 && chance > MC) strcat(dDNA, set1[i]);
		else if (pick[i] == 0 && chance > MC) strcat (dDNA, set2[i]);
	}
	

	
	strcpy(child, dDNA);
			
	
	for (i = 0; i<DL; i++) free (set1[i]);
	for (i = 0; i<DL; i++) free (set2[i]);
	free(set1);
	free(set2);
	free(dDNA);

	return;
}

void gen_popn(char* name) {
	FILE* pop = fopen(name, "w+");
	char* DNA = (char*) malloc (DL*12*sizeof(char));
	
	for (int i = 0; i<PS; i++) {
		gen_DNA(DL, DNA);
		fputs(DNA, pop);
		fprintf(pop, "\n");
	}

	fclose(pop);

	free(DNA);
	
	return;
}

void get_scores(char* popname, char* resname) {
	
	int i,j,k;
	char dummy1[501] = "", dummy2[501] = "";
	
	int* scores = (int*) malloc (PS*sizeof(int));
	for ( i = 0; i<PS;i++ ) scores[i] = 0;
	
	char** DNAs = (char**) malloc (PS*sizeof(char*));
	for ( i = 0; i<PS;i++ ){
		DNAs[i] = (char*) malloc (DL*12*sizeof(char));
	}

	for(i=0; i<PS; i++) {
		strcpy(DNAs[i], "");
	}

	FILE* stream = fopen(popname, "r");

	char str[2] = " ";
	
	i=0;
	str[0] = fgetc(stream);
	while (str[0] != EOF) {
		if (str[0] == '\n') {
			i++;
		}
		if (str[0] != '\n') {
			strcat(DNAs[i], str);
		}
		str[0] = fgetc(stream);
	}

//	//n plays with everyone until PS
	for (i = 0; i < PS; i++){
		for (j = 0; j < PS; j++){
			strcpy(dummy1, DNAs[i]);
			strcpy(dummy2, DNAs[j]);

			k = get_win(dummy1, dummy2);
			if (k == 1) scores[i] += 20;
			if (k == 2) scores[i] -= 50;
			if (k == 0) scores[i] += 1;


		}
		if (scores[i] <= 0) scores[i] = 0;

		printf("%d ", scores[i]);
	}

//	//n plays with random until PS (plays 30 games)
//	//generate DNA for randomness
//	char RAND[501] = "";
//	for (i = 0; i < DL; i++) strcat(RAND, "~ajs~1");
//	puts("strcat");
//	for (i = 0; i < PS; i++){
//		for (j = 0; j < 100; j++){
//			strcpy(dummy1, DNAs[i]);
//			strcpy(dummy2, RAND);
//			k = get_win(dummy1, dummy2);
//			if (k == 1) scores[i] += 20;
//			if (k == 0) scores[i] += 4;
//			if (k == 2) scores[i] -= 100;
//		}
//		if (scores[i] <= 0) scores[i] = 0;
//		printf("%d ", scores[i]);
//	}
	puts("all scores updated");	

	fclose(stream);
	
	breed(DNAs, scores, resname);

	puts("breed done");

	
	for (i=0; i<PS; i++) free(DNAs[i]);
	free(DNAs);
	free(scores);

	
}
	
void breed(char** DNAs, int* scores, char* resname){
	
	int totalsc = 0;
	int i,j,k,l,m, v;
	char dum;
	
	char str[501] = "";
	
	for (i=0; i<PS; i++) totalsc = totalsc + scores[i];

	FILE* output = fopen(resname, "w+");

	for (i=0; i<PS; i++) {
		for (j=0; j<((int)(((float)(scores[i])/(float)(totalsc))*PS)); j++) {
			k = rand()%totalsc;
			l=0;
			m=0;
			v=0;
			for (; v == 0;){
				l = l+scores[m];
				if ( l <= k ) m++;
				else v = 1;
			}

			gene_splice(DNAs[i], DNAs[m], str);
			fputs(str, output);
			fprintf( output, "\n");
		}
	}
	


	fclose(output);

	output = fopen(resname, "r");

	k=0;
	dum = fgetc(output);
	while (dum != EOF) {
		if (dum == '\n') k++;
		dum = fgetc(output);
	}
	
	fclose(output);

	output = fopen(resname, "a+");	

	for (i=0; i<(PS-k); i++) {
		gen_DNA(DL, str);
		fputs(str, output);
		if (i != PS-k-1) fprintf( output, "\n");		
	}

	fclose(output);
	puts("breed done");
	return;	
			

}





















