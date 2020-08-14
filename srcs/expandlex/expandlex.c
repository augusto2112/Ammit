/*------------------------ Project Ammit Test ---------------------------------
------ Source repo: Quake-III-Arena (github.com/id-Software/Quake-III-Arena)---
------ Description: Part of the modified LCC compiler used to compile Quake3---
------ game modules, this function is part of the compiler's lexer. It expands-
------ the lexer's Finite State Machine table size to the closest power of two-
------ to favor the use of bitshift operations, to improve their efficiency.--- 
------ Location: Quake-III-Arena/lcc/cpp/lex.c-------------------------------*/

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef unsigned long size_t;  // Customize by platform.
/* By default, we understand bool (as a convenience). */
// typedef int bool;
#define false 0
#define true 1

/* Forward declarations */

/* Type definitions */
struct fsm {scalar_t__ state; int* ch; int nextstate; } ;

/* Variables and functions */
#define  SECOND 130 
#define  THIRD 129 
#define  FIRST 128 
#define FOURTH 131

size_t EOB ; 
size_t EOFC ; 
int MAXSTATE ; 
int QBSBIT ; 
int S_EOB ; 
int S_EOF ; 
int S_SELF ; 
int** bigfsm ; 
struct fsm* fsm ; 

void
expandlex(void)
{
	/*const*/ struct fsm *fp;
	int i, j, nstate;
  int k = 0;
	for (fp = fsm; fp->state>=0; fp++) {
    // printf("%i\n", k);
    k++;
		for (i=0; fp->ch[i]; i++) {
      // printf("%i\n", i);
			nstate = fp->nextstate;
			if (nstate >= S_SELF)
				nstate = ~nstate;
			switch (fp->ch[i]) {
          // INSERT HERE
		// START 1

			case FIRST:		/* random characters */
				for (j=0; j<256; j++) 
          bigfsm[j][fp->state] = nstate;          
				continue;
        // END 1
        // START 2
			case SECOND:
				for (j=0; j<=256; j++)
					if (('a'<=j&&j<='z') || ('A'<=j&&j<='Z')
					  || j=='_')
						bigfsm[j][fp->state] = nstate;
				continue;
        // END 2
        // START 3
			case THIRD:
				for (j='0'; j<='9'; j++)
					bigfsm[j][fp->state] = nstate;
				continue;
        // END 3
        // START 4
			default:
				bigfsm[fp->ch[i]][fp->state] = nstate;
        // END 4
			}
		}
	}
	/* install special cases for ? (trigraphs),  \ (splicing), runes, and EOB */
	for (i=0; i<MAXSTATE; i++) {
		for (j=0; j<0xFF; j++)
			if (j=='?' || j=='\\') {
				if (bigfsm[j][i]>0)
					bigfsm[j][i] = ~bigfsm[j][i];
				bigfsm[j][i] &= ~QBSBIT;
			}
		bigfsm[EOB][i] = ~S_EOB;
		if (bigfsm[EOFC][i]>=0)
			bigfsm[EOFC][i] = ~S_EOF;
	}
}

void setup(unsigned long SIZE1, int SIZE2, int *elements, float *chances, int num_elements) {
  S_SELF = 0;
  MAXSTATE = 0;
  fsm = (struct fsm *)malloc(SIZE1 * sizeof(struct fsm));
  bigfsm = (int **)malloc(SIZE1 * sizeof(int *));
  for (unsigned long i = 0; i < SIZE1; i++) {
    bigfsm[i] = (int *)malloc(sizeof(int) * 257);
    fsm[i].state = 1;
    fsm[i].nextstate = rand() % 2;
    fsm[i].ch = (int *)malloc((SIZE2 + 1) * sizeof(int));
    for (int j = 0; j < SIZE2; j++) {
      float prob = rand()/(float)RAND_MAX;
      int chance_index = -1;
      float total_prob = 0.0;
      do {
        total_prob += chances[++chance_index];
      } while (total_prob < prob && chance_index < num_elements);
      fsm[i].ch[j] = elements[chance_index];
    }
    fsm[i].ch[SIZE2] = 0;
  }  
  fsm[SIZE1- 1].state = -1;
}

int parse(int argc, char** argv) {
  if (argc != 6) {
    fprintf(stderr, "Sintax: %s SIZE1 SIZE2 CHANCE_FIRST CHANCE_SECOND CHANCE_THIRD\n", argv[0]);
    return 0;
  } else {
    const unsigned SIZE1 = atoi(argv[1]);
    const unsigned SIZE2 = atoi(argv[2]);
    const float CHANCE_FIRST = atof(argv[3]);
    const float CHANCE_SECOND = atof(argv[4]);
    const float CHANCE_THIRD = atof(argv[5]);
    assert(CHANCE_FIRST + CHANCE_SECOND  + CHANCE_THIRD >= 0.0 && CHANCE_FIRST + CHANCE_SECOND  + CHANCE_THIRD <= 1.0 && 
    "Element distribution probabilities should be between 0 and 1");

    float chances[] = {CHANCE_FIRST, CHANCE_SECOND, CHANCE_THIRD};
    int elements[] = {FIRST, SECOND, THIRD, FOURTH};

    setup(SIZE1, SIZE2, elements, chances, 3);
    return 1;
  }
}

int main(int argc, char** argv) {
  if (parse(argc, argv)) {
    clock_t start;
    clock_t end;
    start = clock();
    expandlex();
    end = clock();
    double seconds = (float)(end - start) / CLOCKS_PER_SEC;
    printf("%.2lf", seconds);
  } else {
    return 1;
  } 
}
