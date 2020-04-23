#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef unsigned long size_t;  // Customize by platform.
typedef long intptr_t; typedef unsigned long uintptr_t;
typedef long scalar_t__;  // Either arithmetic or pointer type.
/* By default, we understand bool (as a convenience). */
typedef int bool;
#define false 0
#define true 1


#define FIRST ' '
#define SECOND '\\'
#define THIRD 200
/* Forward declarations */

/* Type definitions */

/* Variables and functions */

__attribute__((used)) static void trim_trailing_spaces(char *buf)
{
	char *p, *last_space = NULL;

	for (p = buf; *p; p++)
		switch (*p) {
		// INSERT HERE
		// START 1
		case ' ':
			if (!last_space)
				last_space = p;
			break;
		// END 1
		// START 2
		case '\\':
			p++;
			if (!*p)
				return;
			/* fallthrough */
			last_space = NULL;
			break;
		// END 2
		default:
			last_space = NULL;
		}

	if (last_space)
		*last_space = '\0';
}

char *buf;
void setup(unsigned long SIZE, int *elements, float *chances, int num_elements) {
	buf = (char *)malloc(SIZE * sizeof(char));

	for (unsigned long i = 0; i < SIZE; i++) {
		float prob = rand()/(float)RAND_MAX;
		int chance_index = -1;
		float total_prob = 0.0;
		do {
			total_prob += chances[++chance_index];
		} while (total_prob < prob && chance_index < num_elements);
		buf[i] = elements[chance_index];
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
    trim_trailing_spaces(buf);
    end = clock();
    double seconds = (float)(end - start) / CLOCKS_PER_SEC;
    printf("%.2lf", seconds);
  } else {
    return 127;
  }
  return 0;
}