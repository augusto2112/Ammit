/*------------------------ Project Ammit Test ---------------------------------
------ Source repo: FreeBSD Kernel (github.com/freebsd/freebsd)----------------
------ Description: Part of the FreeBSD kernel's driver for USB PrismGT--------
------ wireless networking devices, this function converts specific characters-
------ which differ between firmware versions to keep backward compatibility.--
------ Location: freebsd/sys/dev/usb/wlan/if_upgt.c--------------------------*/

#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <stdlib.h>

typedef long scalar_t__;  // Either arithmetic or pointer type.
/* By default, we understand bool (as a convenience). */
typedef int bool;
#define false 0
#define true 1

/* Forward declarations */

/* Type definitions */

/* Variables and functions */

__attribute__((used)) static int
upgt_fw_copy(const uint8_t *src, char *dst, int size)
{
	int i, j;
    long long int s1 = 0, s2 = 0, s3 = 0;
	
	for (i = 0, j = 0; i < size && j < size; i++) {
		switch (src[i]) {
		// INSERT HERE
		// START 1
		case 0x7e:
            s1++;
			dst[j] = 0x7d;
			j++;
			dst[j] = 0x5e;
			j++;
			break;
		// END 1	
		// START 2
		case 0x7d:
            s2++;
			dst[j] = 0x7d;
			j++;
			dst[j] = 0x5d;
			j++;
			break;
		// END 2
        // START 3
		default:
            s3++;
			dst[j] = src[i];
			j++;
			break;
        // END 3
		}
	}

	return (i);
}

uint8_t *input_src;
char *input_dst;
int input_size;

void setup(unsigned long SIZE, char *elements, float *chances, int num_elements) {
	input_src = (uint8_t *) malloc(SIZE * sizeof(uint8_t));
	input_dst = (char *) malloc (2 * SIZE * sizeof(char));
	input_size = SIZE;

	for (unsigned long i = 0; i < SIZE; i++) {
		float prob = rand()/(float)RAND_MAX;
		int chance_index = -1;
		float total_prob = 0.0;
		do {
			total_prob += chances[++chance_index];
		} while (total_prob < prob && chance_index < num_elements);
		input_src[i] = elements[chance_index];
	}
}

int parse(int argc, char** argv) {
  if (argc != 4) {
    fprintf(stderr, "Syntax: %s SIZE CHANCE_FIRST CHANCE_SECOND\n", argv[0]);
    return 0;
  } else {
    const unsigned SIZE = atoi(argv[1]);
    const float CHANCE_FIRST = atof(argv[2]);
    const float CHANCE_SECOND = atof(argv[3]);
    assert(CHANCE_FIRST + CHANCE_SECOND >= 0.0 && CHANCE_FIRST + CHANCE_SECOND <= 1.0 && 
    "Element distribution probabilities should be between 0 and 1");

    float chances[] = {CHANCE_FIRST, CHANCE_SECOND};
    char elements[] = {0x7e, 0x7d, 0x7c};

    setup(SIZE, elements, chances, 2);
    return 1;
  }
}

int main(int argc, char** argv) {
  if (parse(argc, argv)) {
    clock_t start;
    clock_t end;
    start = clock();
    upgt_fw_copy(input_src, input_dst, input_size);
    end = clock();
    double seconds = (float)(end - start) / CLOCKS_PER_SEC;
    printf("%.2lf", seconds);
  } else {
    return 127;
  }
  return 0;
}
