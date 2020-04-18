#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/* Forward declarations */
typedef  struct TYPE_4__   TYPE_2__ ;
typedef  struct TYPE_3__   TYPE_1__ ;

/* Type definitions */
struct TYPE_4__ {
  int* cexp;
  int opcode;
  int* regs;
  TYPE_1__* code;
};

struct TYPE_3__ {
  int nr_operands;
  int* fields;
};

/* Variables and functions */
#define  FMT_SSRO_OFF4 2 
#define  FMT_SSRO_S1   4 
TYPE_2__ dec_insn ; 

static void decode_ssro() {
  int i;
  for (i = 0; i < dec_insn.code->nr_operands; ++i) {
    switch (dec_insn.code->fields[i]) {
      case FMT_SSRO_OFF4:
        dec_insn.cexp[i] = (dec_insn.opcode & 0xf000) >> 12;
        break;
      case FMT_SSRO_S1:
        dec_insn.regs[i] = (dec_insn.opcode & 0x0f00) >> 8;
        break;
    }
  }
}

int setup(int argc, char** argv) {
  if (argc != 3) {
    fprintf(stderr, "Sintaxe: %s SIZE [2/4]\n", argv[0]);
    return 0;
  } else {
    const unsigned SIZE = atoi(argv[1]);
    const int CODE = atoi(argv[2]);
    assert(CODE == 2 || CODE == 4);
    dec_insn.opcode = ~0U;
    dec_insn.cexp =  (int*)malloc(SIZE * sizeof(int));
    dec_insn.regs =  (int*)malloc(SIZE * sizeof(int));
    dec_insn.code = (TYPE_1__*)malloc(sizeof(TYPE_1__));
    dec_insn.code->nr_operands = SIZE;
    dec_insn.code->fields = (int*)malloc(SIZE * sizeof(int));
    for (int i = 0; i < dec_insn.code->nr_operands; i++) {
      dec_insn.code->fields[i] = CODE;
    }
    return 1;
  }
}

int main(int argc, char** argv) {
  if (setup(argc, argv)) {
    clock_t start;
    clock_t end;
    start = clock();
    decode_ssro();
    end = clock();
    double seconds = (float)(end - start) / CLOCKS_PER_SEC;
    printf("%.2lf, ", seconds);
  }
}
