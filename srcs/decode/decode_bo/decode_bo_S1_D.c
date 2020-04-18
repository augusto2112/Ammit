#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/* Forward declarations */
typedef struct TYPE_4__ TYPE_2__;
typedef struct TYPE_3__ TYPE_1__;

/* Type definitions */
struct TYPE_4__ {
  int opcode;
  unsigned long* cexp;
  int* regs;
  TYPE_1__* code;
};

struct TYPE_3__ {
  int nr_operands;
  int* fields;
};

/* Variables and functions */
#define FMT_BO_OFF10 2 
#define FMT_BO_S1_D  4 
#define FMT_BO_S2    8 
TYPE_2__ dec_insn; 

static void decode_bo() {
  int i;
  unsigned long o1, o2;

  for (i = 0; i < dec_insn.code->nr_operands; ++i) {
    switch (dec_insn.code->fields[i]) {
      case FMT_BO_S2:
        dec_insn.regs[i] = (dec_insn.opcode & 0x0000f000) >> 12;
        break;

      case FMT_BO_S1_D:
        dec_insn.regs[i] = (dec_insn.opcode & 0x00000f00) >> 8;
        break;

      case FMT_BO_OFF10:
        o1 = (dec_insn.opcode & 0x003f0000) >> 16;
        o2 = (dec_insn.opcode & 0xf0000000) >> 22;
        dec_insn.cexp[i] = o1 | o2;
        break;
    }
  }
}

int setup(int argc, char** argv) {
  if (argc != 3) {
    fprintf(stderr, "Sintaxe: %s SIZE [2/4/8]\n", argv[0]);
    return 0;
  } else {
    const unsigned SIZE = atoi(argv[1]);
    const int CODE = atoi(argv[2]);
    assert(CODE == 2 || CODE == 4 || CODE == 8);
    dec_insn.opcode = ~0U;
    dec_insn.cexp =  (unsigned long*)malloc(SIZE * sizeof(unsigned long));
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
    decode_bo();
    end = clock();
    double seconds = (float)(end - start) / CLOCKS_PER_SEC;
    printf("%.2lf, ", seconds);
  }
}
