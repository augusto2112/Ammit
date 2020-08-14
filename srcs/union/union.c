#include <time.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

union Union {
  char c;
  unsigned u;
  double d;
};

struct Str1 {
  int tag;
  union Union data;
};

typedef struct Str1 Data;

enum TYPES {CHAR, UINT, DOUB};



Data **vec;
unsigned long SIZE;

double iterate() {
  double pSum;
  for (unsigned long i = 0; i < SIZE; i++) {
    switch (vec[i]->tag) {
      // INSERT HERE
      // START 1
      case CHAR:
        pSum += vec[i]->data.c;
        break;
      // END 1
      // START 2
      case UINT:
        pSum += vec[i]->data.u;
        break;
      // END 2
      // START 3
      case DOUB:
        pSum += vec[i]->data.d;
        break;
      // END 3
      default:
        break;
    }
  }
  return pSum;
}


void setup(float *chances) {
  vec = (Data**)malloc(SIZE * sizeof(Data*));
  for (unsigned long i = 0; i < SIZE; i++) {
    Data* d = (Data*)malloc(sizeof(Data));
    float prob = rand()/(float)RAND_MAX;
    if (prob < chances[0]) {
      d->tag = CHAR;
      d->data.c = i % 127;
    } else if (prob < chances[1]) {
      d->tag = UINT;
      d->data.u = i;
    } else {
      d->tag = DOUB;
      d->data.d = 1.0/(i + 1);
    }
    vec[i] = d;
  }
}

int parse(int argc, char** argv) {
  if (argc != 4) {
    fprintf(stderr, "Sintax: %s SIZE CHANCE_FIRST CHANCE SECOND\n", argv[0]);
    return 0;
  } else {
    SIZE = atoi(argv[1]);
    const float CHANCE_FIRST = atof(argv[2]);
    const float CHANCE_SECOND = atof(argv[3]);

    assert(CHANCE_FIRST + CHANCE_SECOND >= 0.0 && CHANCE_FIRST + CHANCE_SECOND <= 1.0 && 
    "Element distribution probabilities should be between 0 and 1");

    float chances[] = {CHANCE_FIRST, CHANCE_SECOND};

    setup(chances);

    return 1;
  }
}

int main(int argc, char** argv) {
  if (parse(argc, argv)) {
    clock_t start;
    clock_t end;
    start = clock();
    volatile double d = iterate(); // volatile is needed or sele the call is optimized away
    end = clock();
    double seconds = (float)(end - start) / CLOCKS_PER_SEC;
    printf("%.2lf", seconds);
  } else {
    return 1;
  } 
}
