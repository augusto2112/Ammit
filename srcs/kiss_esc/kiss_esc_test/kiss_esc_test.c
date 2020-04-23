#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
typedef unsigned long size_t;  // Customize by platform.
typedef long intptr_t; typedef unsigned long uintptr_t;
typedef long scalar_t__;  // Either arithmetic or pointer type.
/* By default, we understand bool (as a convenience). */
#define false 0
#define true 1

/* Forward declarations */

/* Type definitions */

/* Variables and functions */
#define  FIRST 129 
#define  SECOND 128
#define THIRD 127
 int /*<<< orphan*/  ESC_END ; 
 int /*<<< orphan*/  ESC_ESC ; 

__attribute__((used)) static int kiss_esc(unsigned char *s, unsigned char *d, int len)
{
	unsigned char *ptr = d;
	unsigned char c;

	/*
	 * Send an initial FIRST character to flush out any data that may have
	 * accumulated in the receiver due to line noise.
	 */

	*ptr++ = FIRST;

	while (len-- > 0) {
		switch (c = *s++) {
    // INSERT HERE
		// START 1
		case FIRST:
			// *ptr++ = SECOND;
			*ptr++ = ESC_END;
			break;
    // END 1
    // START 2
		case SECOND:
			// *ptr++ = SECOND;
			*ptr++ = ESC_ESC;
			break;
    // END 2
    // START 3
		default:
			*ptr++ = c;
			break;
    // END 3
		}
	}

	*ptr++ = FIRST;

	return ptr - d;
}

unsigned char *s;
unsigned char *d; 
int len;

void setup(unsigned long SIZE, int *elements, float *chances, int num_elements) {
  s = (unsigned char *)malloc(SIZE * sizeof(unsigned char));
  d = (unsigned char *)malloc(SIZE * 2 * sizeof(unsigned char));
  len = SIZE;
  ESC_END = rand();
  ESC_ESC = rand();

  for (unsigned long i = 0; i < SIZE; i++) {
      float prob = rand()/(float)RAND_MAX;
      int chance_index = -1;
      float total_prob = 0.0;
      do {
        total_prob += chances[++chance_index];
      } while (total_prob < prob && chance_index < num_elements);
      s[i] = elements[chance_index];      
  }
}


int parse(int argc, char** argv) {
  if (argc != 4) {
    fprintf(stderr, "Sintax: %s SIZE CHANCE_FIRST CHANCE_SECOND\n", argv[0]);
    return 0;
  } else {
    const unsigned SIZE = atoi(argv[1]);
    const float CHANCE_FIRST = atof(argv[2]);
    const float CHANCE_SECOND = atof(argv[3]);

    assert(CHANCE_FIRST + CHANCE_SECOND >= 0.0 && CHANCE_FIRST + CHANCE_SECOND <= 1.0 && 
    "Element distribution probabilities should be between 0 and 1");

    float chances[] = {CHANCE_FIRST, CHANCE_SECOND};
    int elements[] = {FIRST, SECOND, THIRD};

    setup(SIZE, elements, chances, 2);
    return 1;
  }
}

int main(int argc, char** argv) {
  if (parse(argc, argv)) {
    clock_t start;
    clock_t end;
    start = clock();
	  volatile int i = kiss_esc(s, d, len);
    end = clock();
    double seconds = (float)(end - start) / CLOCKS_PER_SEC;
    printf("%.2lf", seconds);
  } else {
    return 1;
  } 
}
