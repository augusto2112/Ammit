/*------------------------ Project Ammit Test ---------------------------------
------ Source repo: Linux kernel (github.com/torvalds/linux) ------------------
------ Description: Part of the implementation for the Linux kernel's perf pro-
------ filing tool, this function parses debugging breakpoints in the input----
------ input program.----------------------------------------------------------
------ Location: linux/tools/perf/util/parse_breakpoint_type.c---------------*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

typedef long scalar_t__;  // Either arithmetic or pointer type.
/* By default, we understand bool (as a convenience). */
typedef int bool;
#define false 0
#define true 1

/* Forward declarations */

/* Type definitions */
struct perf_event_attr {int bp_type; } ;

/* Variables and functions */
 int EINVAL ; 
 int HW_BREAKPOINT_R ; 
 int HW_BREAKPOINT_W ; 
 int HW_BREAKPOINT_X ; 

__attribute__((used)) static int
parse_breakpoint_type(const char *type, struct perf_event_attr *attr, int SIZE)
{
	int i;

	for (i = 0; i < SIZE; i++) {
		if (!type || !type[i])
			break;

#define CHECK_SET_TYPE(bit)		\
do {					\
	if (attr->bp_type & bit)	\
		return -EINVAL;		\
	else				\
		attr->bp_type |= bit;	\
} while (0)

		switch (type[i]) {
        // INSERT HERE
        // START 1
		case 'r':
			CHECK_SET_TYPE(HW_BREAKPOINT_R);
			break;
        // END 1
        // START 2
		case 'w':
			CHECK_SET_TYPE(HW_BREAKPOINT_W);
			break;
        // END 2
        // START 3
		case 'x':
			CHECK_SET_TYPE(HW_BREAKPOINT_X);
			break;
        // END 3
		default:
			return -EINVAL;
		}
	}

#undef CHECK_SET_TYPE

	if (!attr->bp_type) /* Default */
		attr->bp_type = HW_BREAKPOINT_R | HW_BREAKPOINT_W;

	return 0;
}

char *input_type;
struct perf_event_attr* input_perf;
unsigned int SIZE;
void setup(unsigned long SIZE, char *elements, float *chances, int num_elements) {
  input_perf = (struct perf_event_attr*) malloc(sizeof(struct perf_event_attr));

  input_type = (char *) malloc(SIZE * sizeof(char));
  for (unsigned long i = 0; i < SIZE; i++) {
      float prob = rand()/(float)RAND_MAX;
      int chance_index = -1;
      float total_prob = 0.0;
      do {
        total_prob += chances[++chance_index];
      } while (total_prob < prob && chance_index < num_elements);
      input_type[i] = elements[chance_index];      
  }
}

int parse(int argc, char** argv) {
  if (argc != 4) {
    fprintf(stderr, "Sintax: %s SIZE CHANCE_FIRST CHANCE_SECOND\n", argv[0]);
    return 0;
  } else {
    SIZE = atoi(argv[1]);
    const float CHANCE_FIRST = atof(argv[2]);
    const float CHANCE_SECOND = atof(argv[3]);

    assert(CHANCE_FIRST + CHANCE_SECOND >= 0.0 && CHANCE_FIRST + CHANCE_SECOND <= 1.0 && 
    "Element distribution probabilities should be between 0 and 1");

    float chances[] = {CHANCE_FIRST, CHANCE_SECOND};
    char elements[] = {'r', 'w', 'x'};

    setup(SIZE, elements, chances, 2);
    return 1;
  }
}

int main(int argc, char** argv) {
  if (parse(argc, argv)) {
    clock_t start;
    clock_t end;
    start = clock();
	volatile int i = parse_breakpoint_type(input_type, input_perf, SIZE);
    end = clock();
    double seconds = (float)(end - start) / CLOCKS_PER_SEC;
    printf("%.2lf", seconds);
  } else {
    return 1;
  }
  return 0;
}
