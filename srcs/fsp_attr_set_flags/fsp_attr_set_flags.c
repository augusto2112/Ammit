/*------------------------ Project Ammit Test ---------------------------------
------ Source repo: Linux Kernel (github.com/torvalds/linux)-------------------
------ Description: Part of the Linux kernel's mouse input drivers, this-------
------ function sets a few flags in the device's configuration.----------------
------ Location: linux/drivers/input/mouse/sentelic.c.c----------------------*/

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/* By default, we understand bool (as a convenience). */
typedef int bool;
#define false 0
#define true 1

/* Forward declarations */

/* Type definitions */
struct psmouse {struct fsp_data* private; } ;
struct fsp_data {int /*<<< orphan*/  flags; } ;
//typedef  size_t ssize_t ;

/* Variables and functions */
 size_t EINVAL ; 
 int /*<<< orphan*/  FSPDRV_FLAG_EN_OPC ; 

__attribute__((used)) static ssize_t fsp_attr_set_flags(struct psmouse *psmouse, void *data,
					const char *buf, size_t count)
{
	struct fsp_data *pad = psmouse->private;
	size_t i;

	for (i = 0; i < count; i++) {
		switch (buf[i]) {
		// INSERT HERE
		// START 1
		case 'C':
			pad->flags |= FSPDRV_FLAG_EN_OPC;
			break;
		// END 1
		// START 2
		case 'c':
			pad->flags &= ~FSPDRV_FLAG_EN_OPC;
			break;
		// END 2
		default:
			return -EINVAL;
		}
	}
	return count;
}

struct psmouse *input_psm;
void *input_data;
char *input_buf;
size_t input_count;

void setup(unsigned long SIZE, char *elements, float *chances, int num_elements) {
	input_psm = (struct psmouse*) malloc(sizeof(struct psmouse));
	input_psm->private = (struct fsp_data*) malloc(sizeof(struct fsp_data));
	input_data = NULL;
	input_buf = (char *) malloc(SIZE * sizeof(char));
	input_count = (size_t) SIZE;

	for (unsigned long i = 0; i < SIZE; i++) {
		float prob = rand()/(float)RAND_MAX;
		int chance_index = -1;
		float total_prob = 0.0;
		do {
			total_prob += chances[++chance_index];
		} while (total_prob < prob && chance_index < num_elements);
		input_buf[i] = elements[chance_index];
	}
}

int parse(int argc, char** argv) {
  if (argc != 3) {
    fprintf(stderr, "Syntax: %s SIZE CHANCE_FIRST\n", argv[0]);
    return 0;
  } else {
    const unsigned SIZE = atoi(argv[1]);
    const float CHANCE_FIRST = atof(argv[2]);
    assert(CHANCE_FIRST >= 0.0 && CHANCE_FIRST <= 1.0 && 
    "Element distribution probabilities should be between 0 and 1");

    float chances[] = {CHANCE_FIRST};
    char elements[] = {'C', 'c'};

    setup(SIZE, elements, chances, 1);
    return 1;
  }
}

int main(int argc, char** argv) {
  if (parse(argc, argv)) {
    clock_t start;
    clock_t end;
    start = clock();
    fsp_attr_set_flags(input_psm, input_data, input_buf, input_count);
    end = clock();
    double seconds = (float)(end - start) / CLOCKS_PER_SEC;
    printf("%.2lf", seconds);
  } else {
    return 127;
  }
  return 0;
}
