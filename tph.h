#ifndef TPH
#define TPH

//constants
#define GMS 5 	//max no of conditions in a gene
#define DL 60 	//DNA Length
#define PS 200	//POP size
#define MC 5 	//mutate chance out of a 100
#define GC 3000 //generations to generate

//returns 1 if player 1 wins, 2 otherwise
int victory_check(int* board, int last_player);

//returns 1 if condition is met
int condition_check(char* condition, int* board, int player);

//adds probabilities to int* prob
int action_add(char* action, int* prob);

//returns the action
int action_check(int* prob, int* board);

//generate DNA
void gen_DNA(int len, char* output);

//grab probability function from 1. DNA, 2. board state
void prob_get(char** ifs, char** thens, int* board, int player, int* prob);

//print board
void printb(int* board);

//void play processes 1 turn for player 1 and player 2 respectively
void play( char** ifs1, char** ifs2, char** thens1, char** thens2, int* board, int* victory );

//void get winner returns 1 if player 1 wins, 2 if player 2 wins
int get_win( char* DNA1, char* DNA2 );

//void wait waits for X s
void wait(double ms);

//gene splicing takes 2 strings returns 1 string assuming srand
//is done beforehand
void gene_splice(char* DNA1, char* DNA2, char* child);


//pop_gen generates a population of size PS in a text file
//name of text file given by name
void gen_popn(char* name);

//get_scores changes an integer array popscores, from genDATA
//from popname.txt, and then breeds to resname.txt
void get_scores(char* popname,  char* resname);

//breeds, taking the variables:
//to write a new file with new population called resname
void breed(char** DNAs, int* scores, char* resname);

#endif