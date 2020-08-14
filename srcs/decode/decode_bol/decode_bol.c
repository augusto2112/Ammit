/*------------------------ Project Ammit Test ---------------------------------
------ Source repo: radare2 (github.com/radareorg/radare2) --------------------
------ Description: Part of radare's TriCore architecture disassembler, this---
------ function decodes a specific instruction type in TriCore's ISA.----------
------ Location: radare2/lib2/asm/arch/tricore/gnu/tricore-dis.c ------------*/

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
typedef  struct TYPE_4__   TYPE_2__ ;
typedef  struct TYPE_3__   TYPE_1__ ;

/* Type definitions */
struct TYPE_4__ {int opcode; unsigned long* cexp; int* regs; TYPE_1__* code; } ;
struct TYPE_3__ {int nr_operands; int* fields; } ;

/* Variables and functions */
#define  FIRST 130 
#define  THIRD 129 
#define  SECOND 128 
 TYPE_2__ dec_insn ; 

__attribute__((used)) static void
decode_bol ()
{
  int i;
  unsigned long o1, o2, o3;

  for (i = 0; i < dec_insn.code->nr_operands; ++i) {
	  switch (dec_insn.code->fields[i]) {
      // INSERT HERE
			// START 1
      case FIRST:
        o1 = (dec_insn.opcode & 0x003f0000) >> 16;
        o2 = (dec_insn.opcode & 0xf0000000) >> 22;
        o3 = (dec_insn.opcode & 0x0fc00000) >> 12;
        dec_insn.cexp[i] = o1 | o2 | o3;
        break;
      // END 1
      // START 2
      case SECOND:
        dec_insn.regs[i] = (dec_insn.opcode & 0x0000f000) >> 12;
        break;
      // END 2
      // START 3
      case THIRD:
        dec_insn.regs[i] = (dec_insn.opcode & 0x00000f00) >> 8;
        break;
      // END 3
	  }
  }
}


void setup(unsigned long SIZE, int *elements, float *chances, int num_elements) {
  dec_insn.opcode = ~0U;
  dec_insn.cexp = (unsigned long*)malloc(SIZE * sizeof(unsigned long));

  dec_insn.regs =  (int*)malloc(SIZE * sizeof(int));
  dec_insn.code = (TYPE_1__*)malloc(sizeof(TYPE_1__));
  dec_insn.code->nr_operands = SIZE;
  dec_insn.code->fields = (int*)malloc(SIZE * sizeof(int));
  for (unsigned long i = 0; i < SIZE; i++) {
      float prob = rand()/(float)RAND_MAX;
      int chance_index = -1;
      float total_prob = 0.0;
      do {
        total_prob += chances[++chance_index];
      } while (total_prob < prob && chance_index < num_elements);
      dec_insn.code->fields[i] = elements[chance_index];
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
    decode_bol();
    end = clock();
    double seconds = (float)(end - start) / CLOCKS_PER_SEC;
    printf("%.2lf", seconds);
  } else {
    return 1;
  } 
}
