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
struct TYPE_4__ {int* cexp; int opcode; TYPE_1__* code; } ;
struct TYPE_3__ {int nr_operands; int* fields; char* args; } ;

/* Variables and functions */
#define  SECOND 129 
#define  FIRST 128 
 TYPE_2__ dec_insn ; 

__attribute__((used)) static void
decode_sbrn ()
{
  int i;

  for (i = 0; i < dec_insn.code->nr_operands; ++i) {
	  switch (dec_insn.code->fields[i]) {
      // INSERT HERE
			// START 1
      case FIRST:
        if (dec_insn.code->args[i] == '5') {
          dec_insn.cexp[i] = (dec_insn.opcode & 0xf000) >> 12;
          dec_insn.cexp[i] |= (dec_insn.opcode & 0x0080) >> 3;
        } else {
          dec_insn.cexp[i] = (dec_insn.opcode & 0xf000) >> 12;
        }
        break;
      // END 1
      // START 2
      case SECOND:
        dec_insn.cexp[i] = (dec_insn.opcode & 0x0f00) >> 8;
        break;
      // END 2
	  }
  }
}


void setup(unsigned long SIZE, int *elements, float *chances, int num_elements) {
  dec_insn.opcode = ~0U;
  dec_insn.cexp =  (int*)malloc(SIZE * sizeof(int));
  dec_insn.code = (TYPE_1__*)malloc(sizeof(TYPE_1__));
  dec_insn.code->nr_operands = SIZE;
  dec_insn.code->fields = (int*)malloc(SIZE * sizeof(int));
  dec_insn.code->args = (char*)malloc(SIZE * sizeof(char));
  for (unsigned long i = 0; i < SIZE; i++) {
      float prob = rand()/(float)RAND_MAX;
      int chance_index = -1;
      float total_prob = 0.0;
      do {
        total_prob += chances[++chance_index];
      } while (total_prob < prob && chance_index < num_elements);
      dec_insn.code->fields[i] = elements[chance_index];
      dec_insn.code->args[i] = rand() % 2 ? '5' : 0;
  }
}


int parse(int argc, char** argv) {
  if (argc != 3) {
    fprintf(stderr, "Sintax: %s SIZE CHANCE_FIRST\n", argv[0]);
    return 0;
  } else {
    const unsigned SIZE = atoi(argv[1]);
    const float CHANCE_FIRST = atof(argv[2]);
    assert(CHANCE_FIRST >= 0.0 && CHANCE_FIRST <= 1.0 && 
    "Element distribution probabilities should be between 0 and 1");

    float chances[] = {CHANCE_FIRST};
    int elements[] = {FIRST, SECOND};

    setup(SIZE, elements, chances, 1);
    return 1;
  }
}

int main(int argc, char** argv) {
  if (parse(argc, argv)) {
    clock_t start;
    clock_t end;
    start = clock();
    decode_sbrn();
    end = clock();
    double seconds = (float)(end - start) / CLOCKS_PER_SEC;
    printf("%.2lf", seconds);
  } else {
    return 1;
  }
}
