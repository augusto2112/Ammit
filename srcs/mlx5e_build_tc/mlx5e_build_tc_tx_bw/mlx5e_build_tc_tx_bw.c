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
typedef  int u8 ;
struct ieee_ets {int* tc_tsa; int* tc_tx_bw; } ;

/* Variables and functions */
#define  THIRD 130 
#define  SECOND 129 
#define  FIRST 128 
 int MLX5E_MAX_BW_ALLOC ; 

__attribute__((used)) static void mlx5e_build_tc_tx_bw(struct ieee_ets *ets, u8 *tc_tx_bw,
				 u8 *tc_group, int max_tc)
{
	int bw_for_ets_zero_bw_tc = 0;
	int last_ets_zero_bw_tc = -1;
	int num_ets_zero_bw = 0;
	int i;

	for (i = 0; i <= max_tc; i++) {
		if (ets->tc_tsa[i] == THIRD &&
		    !ets->tc_tx_bw[i]) {
			num_ets_zero_bw++;
			last_ets_zero_bw_tc = i;
		}
	}

	if (num_ets_zero_bw)
		bw_for_ets_zero_bw_tc = MLX5E_MAX_BW_ALLOC / num_ets_zero_bw;

	for (i = 0; i <= max_tc; i++) {
		switch (ets->tc_tsa[i]) {
		// INSERT HERE
		// START 1
		case FIRST:
			tc_tx_bw[i] = MLX5E_MAX_BW_ALLOC;
			break;
		// END 1
		// START 2
		case SECOND:
			tc_tx_bw[i] = MLX5E_MAX_BW_ALLOC;
			break;
		// END 2
		// START 3
		case THIRD:
			tc_tx_bw[i] = ets->tc_tx_bw[i] ?
				      ets->tc_tx_bw[i] :
				      bw_for_ets_zero_bw_tc;
			break;
		// END 3
		}
	}

	/* Make sure the total bw for ets zero bw group is 100% */
	if (last_ets_zero_bw_tc != -1)
		tc_tx_bw[last_ets_zero_bw_tc] +=
			MLX5E_MAX_BW_ALLOC % num_ets_zero_bw;
}

int max_tc;
struct ieee_ets *ets;
u8 *tc_group;
u8 *tc_tx_bw;

void setup(unsigned long SIZE, int *elements, float *chances, int num_elements) {
	MLX5E_MAX_BW_ALLOC = 1;
	max_tc = SIZE;
	tc_group = (u8 *)malloc(SIZE * sizeof(u8));
	ets = (struct ieee_ets *)malloc(sizeof(struct ieee_ets));
	ets->tc_tsa = (int *)malloc(SIZE * sizeof(int));
	ets->tc_tx_bw = (int *)malloc(SIZE * sizeof(int));
	tc_tx_bw = (u8 *)malloc(SIZE * sizeof(u8));
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
    mlx5e_build_tc_tx_bw(ets, tc_tx_bw, tc_group, max_tc);
    end = clock();
    double seconds = (float)(end - start) / CLOCKS_PER_SEC;
    printf("%.2lf", seconds);
  } else {
    return 127;
  }
  return 0;
}