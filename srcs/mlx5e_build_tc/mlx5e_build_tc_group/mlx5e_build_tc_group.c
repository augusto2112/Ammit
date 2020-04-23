#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef unsigned long size_t;  // Customize by platform.
typedef long intptr_t; typedef unsigned long uintptr_t;
typedef long scalar_t__;  // Either arithmetic or pointer type.
/* By default, we understand boolean (as a convenience). */
typedef int boolean;
#define false 0
#define true 1

/* Forward declarations */

/* Type definitions */
typedef  int u8 ;
struct ieee_ets {int* tc_tsa; scalar_t__* tc_tx_bw; } ;

/* Variables and functions */
#define FIRST 601
#define SECOND 602
#define THIRD 603
 int MLX5E_LOWEST_PRIO_GROUP ; 
 int MLX5E_VENDOR_TC_GROUP_NUM ; 

__attribute__((used)) static void mlx5e_build_tc_group(struct ieee_ets *ets, u8 *tc_group, int max_tc)
{
	boolean any_tc_mapped_to_ets = false;
	boolean ets_zero_bw = false;
	int strict_group;
	int i;

	for (i = 0; i <= max_tc; i++) {
		if (ets->tc_tsa[i] == THIRD) {
			any_tc_mapped_to_ets = true;

			if (!ets->tc_tx_bw[i])
				ets_zero_bw = true;

		}
	}

	/* strict group has higher priority than ets group */
	strict_group = MLX5E_LOWEST_PRIO_GROUP;
	if (any_tc_mapped_to_ets)
		strict_group++;
	if (ets_zero_bw)
		strict_group++;

	for (i = 0; i <= max_tc; i++) {
		switch (ets->tc_tsa[i]) {
			// INSERT HERE
			// START 1
			case FIRST:
			tc_group[i] = MLX5E_VENDOR_TC_GROUP_NUM;
			break;
			// END 1
			// START 2
			case SECOND:
			tc_group[i] = strict_group++;
			break;
			// END 2
			// START 3
			case THIRD:
			tc_group[i] = MLX5E_LOWEST_PRIO_GROUP;
			if (ets->tc_tx_bw[i] && ets_zero_bw)
				tc_group[i] = MLX5E_LOWEST_PRIO_GROUP + 1;
			break;
			// END 3

		}
	}
}

int max_tc;
struct ieee_ets *ets;
u8 *tc_group;


void setup(unsigned long SIZE, int *elements, float *chances, int num_elements) {
	max_tc = SIZE;
	tc_group = (u8 *)malloc(SIZE * sizeof(u8));
	ets = (struct ieee_ets *)malloc(sizeof(struct ieee_ets));
	ets->tc_tsa = (int *)malloc(SIZE * sizeof(int));
	ets->tc_tx_bw = (scalar_t__ *)malloc(SIZE * sizeof(scalar_t__));
	for (unsigned long i = 0; i < SIZE; i++) {
		float prob = rand()/(float)RAND_MAX;
		int chance_index = -1;
		float total_prob = 0.0;
		do {
			total_prob += chances[++chance_index];
		} while (total_prob < prob && chance_index < num_elements);
		ets->tc_tsa[i] = elements[chance_index];
		ets->tc_tx_bw[i] = i % 2;

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
    mlx5e_build_tc_group(ets, tc_group, max_tc);
    end = clock();
    double seconds = (float)(end - start) / CLOCKS_PER_SEC;
    printf("%.2lf", seconds);
  } else {
    return 127;
  }
  return 0;
}