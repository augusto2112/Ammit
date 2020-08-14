/*------------------------ Project Ammit Test ---------------------------------
------ Source repo: Linux Kernel (github.com/torvalds/linux)-------------------
------ Description: Part of the Linux Kernel's support for Motorolla 68000-----
------ architecture CPUs, this function performs fixes to the memory layout----
------ of program ELF binaries built for that architecture.--------------------
------ Location: linux/arch/m68k/kernel/module.c-----------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <stdint.h>

/* By default, we understand bool (as a convenience). */
typedef long scalar_t__;  // Either arithmetic or pointer type.
typedef int bool;
#define false 0
#define true 1

/* Forward declarations */

/* Type definitions */
typedef  int /*<<< orphan*/  u32 ;
typedef  int /*<<< orphan*/  u16 ;
struct module {int dummy; } ;
struct m68k_fixup_info {int type; unsigned long *addr; } ;

/* Variables and functions */
#define  m68k_fixup_memoffset 129 
#define  m68k_fixup_vnode_shift 128 
 int /*<<< orphan*/  m68k_memoffset ; 
 int /*<<< orphan*/  m68k_virt_to_node_shift ; 

void module_fixup(struct module *mod, struct m68k_fixup_info *start,
		  struct m68k_fixup_info *end)
{
	struct m68k_fixup_info *fixup;

	for (fixup = start; fixup < end; fixup++) {
		switch (fixup->type) {
		// INSERT HERE
		// START 1
		case m68k_fixup_memoffset:
			*(u32 *)fixup->addr = m68k_memoffset;
			break;
		// END 1
		// START 2
		case m68k_fixup_vnode_shift:
			*(u16 *)fixup->addr += m68k_virt_to_node_shift;
			break;
		// END 2
		}
	}
}

struct module *input_mod;
struct m68k_fixup_info *input_start;
struct m68k_fixup_info *input_end;

void setup(unsigned long SIZE, int *elements, float *chances, int num_elements) {
	struct m68k_fixup_info *input;

	input_mod = (struct module*) malloc(sizeof(struct module));
	input = (struct m68k_fixup_info *) malloc(SIZE*sizeof(struct m68k_fixup_info));
	input_start = &input[0];
	input_end = &input[SIZE-1];

	for (unsigned long i = 0; i < SIZE; i++) {
		input[i].addr = (unsigned long*) malloc(sizeof(unsigned long));
		float prob = rand()/(float)RAND_MAX;
		int chance_index = -1;
		float total_prob = 0.0;
		do {
			total_prob += chances[++chance_index];
		} while (total_prob < prob && chance_index < num_elements);
		input[i].type = elements[chance_index];
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
    int elements[] = {m68k_fixup_memoffset, m68k_fixup_vnode_shift};

    setup(SIZE, elements, chances, 1);
    return 1;
  }
}

int main(int argc, char** argv) {
  if (parse(argc, argv)) {
    clock_t start;
    clock_t end;
    start = clock();
    module_fixup(input_mod, input_start, input_end);
    end = clock();
    double seconds = (float)(end - start) / CLOCKS_PER_SEC;
    printf("%.2lf", seconds);
  } else {
    return 127;
  }
  return 0;
}
