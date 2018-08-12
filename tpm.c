#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "tph.h"

int main(){

	srand((unsigned)time(NULL));

	int score[PS];
	
	//gen_popn("generation one.txt");
	get_scores("generation one.txt", "generation z.txt");

	for (int a = 0; a < GC; a++) {
		if (a%2 == 0) get_scores("generation z.txt", "generation y.txt");
		if (a%2 == 1) get_scores("generation y.txt", "generation z.txt");
		printf("gen done %d\n", a);
	}
	return 0;
}