#include <time.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

struct DataStruct {
  int x;
};

typedef struct DataStruct Data;

Data **vec;
unsigned SIZE;

long iterateNULL() {
  long pSum = 0;
  long nSum = 0;
  unsigned pCounter = 0;
  unsigned nCounter = 0;
  for (int i = 0; i < SIZE; i++) {
    int jmp = (vec[i] == NULL);
    switch (jmp) {
      // INSERT HERE
			// START 1
      case 0:
        pSum += vec[i]->x;
        pCounter++;
        break;
      // END 1
      // START 2
      case 1:
        nSum += 1;
        nCounter++;
        break;
      // END 2
      default:
        break;;
    }
  }
  return pSum - nSum;
}


void setup(float *chances) {
  vec = (Data**)malloc(SIZE * sizeof(Data*));
  for (int i = 0; i < SIZE; i++) {
    float prob = rand()/(float)RAND_MAX;

    if (prob < chances[0]) {
      Data* d = (Data*)malloc(sizeof(Data));
      d->x = i;
      vec[i] = d;
    } else {
      vec[i] = NULL;
    }
  }
}

int parse(int argc, char** argv) {
  if (argc != 3) {
    fprintf(stderr, "Sintax: %s SIZE CHANCE_FIRST\n", argv[0]);
    return 0;
  } else {
    SIZE = atoi(argv[1]);
    const float CHANCE_FIRST = atof(argv[2]);
    assert(CHANCE_FIRST >= 0.0 && CHANCE_FIRST <= 1.0 && 
    "Element distribution probabilities should be between 0 and 1");

    float chances[] = {CHANCE_FIRST};

    setup(chances);
    return 1;
  }
}

int main(int argc, char** argv) {
  if (parse(argc, argv)) {
    clock_t start;
    clock_t end;
    start = clock();
    volatile long n = iterateNULL();
    end = clock();
    double seconds = (float)(end - start) / CLOCKS_PER_SEC;
    printf("%.2lf", seconds);
  } else {
    return 1;
  }
}
