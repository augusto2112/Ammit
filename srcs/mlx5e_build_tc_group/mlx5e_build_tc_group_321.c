#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define NULL ((void*)0)
typedef unsigned long size_t;  // Customize by platform.
typedef long intptr_t; typedef unsigned long uintptr_t;
typedef long scalar_t__;  // Either arithmetic or pointer type.
/* By default, we understand bool (as a convenience). */
typedef int bool;
#define false 0
#define true 1

/* Forward declarations */

/* Type definitions */
typedef  int u8 ;
struct ieee_ets {int* tc_tsa; scalar_t__* tc_tx_bw; } ;

/* Variables and functions */
#define  IEEE_8021QAZ_TSA_ETS 3 
#define  IEEE_8021QAZ_TSA_STRICT 2 
#define  IEEE_8021QAZ_TSA_VENDOR 1 
 int MLX5E_LOWEST_PRIO_GROUP ; 
 int MLX5E_VENDOR_TC_GROUP_NUM ; 

__attribute__((used)) static void mlx5e_build_tc_group(struct ieee_ets *ets, u8 *tc_group, int max_tc)
{
	bool any_tc_mapped_to_ets = false;
	bool ets_zero_bw = false;
	int strict_group;
	int i;

	for (i = 0; i <= max_tc; i++) {
		if (ets->tc_tsa[i] == IEEE_8021QAZ_TSA_ETS) {
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
		case IEEE_8021QAZ_TSA_ETS:
			tc_group[i] = MLX5E_LOWEST_PRIO_GROUP;
			if (ets->tc_tx_bw[i] && ets_zero_bw)
				tc_group[i] = MLX5E_LOWEST_PRIO_GROUP + 1;
			break;
		case IEEE_8021QAZ_TSA_STRICT:
			tc_group[i] = strict_group++;
			break;
		case IEEE_8021QAZ_TSA_VENDOR:
			tc_group[i] = MLX5E_VENDOR_TC_GROUP_NUM;
			break;
		}
	}
}

int max_tc;
struct ieee_ets *ets;
u8 *tc_group;

int setup(int argc, char** argv) {
  if (argc != 3) {
    fprintf(stderr, "Sintaxe: %s SIZE [128/129/130]\n", argv[0]);
    return 0;
  } else {
    const unsigned SIZE = atoi(argv[1]);
    const int CODE = atoi(argv[2]);
    assert(CODE == IEEE_8021QAZ_TSA_ETS || CODE == IEEE_8021QAZ_TSA_STRICT || CODE == IEEE_8021QAZ_TSA_VENDOR);
	max_tc = SIZE;
	tc_group = malloc(SIZE * sizeof(u8));
	ets = malloc(sizeof(struct ieee_ets));
	ets->tc_tsa = malloc(SIZE * sizeof(int));
	ets->tc_tx_bw = malloc(SIZE * sizeof(scalar_t__));
	for (int i = 0; i < SIZE; i++) {
		ets->tc_tsa[i] = CODE;
		ets->tc_tx_bw[i] = 0;
	}
    return 1;
  }
}

int main(int argc, char** argv) {
  if (setup(argc, argv)) {
    clock_t start;
    clock_t end;
    start = clock();
    mlx5e_build_tc_group(ets, tc_group, max_tc);
    end = clock();
    double seconds = (float)(end - start) / CLOCKS_PER_SEC;
    printf("%.2lf, ", seconds);
  }
}