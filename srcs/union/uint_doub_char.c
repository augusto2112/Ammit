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

Data** init(const unsigned SIZE, const float PChar, const float PUint) {
  Data** vec = (Data**)malloc(SIZE * sizeof(Data*));
  for (long i = 0; i < SIZE; i++) {
    float prob = rand()/(float)RAND_MAX;
    Data* d = (Data*)malloc(sizeof(Data));
    if (prob < PChar) {
      d->tag = CHAR;
      d->data.c = i % 127;
    } else if (prob < (PChar + PUint)) {
      d->tag = UINT;
      d->data.c = i;
    } else {
      d->tag = DOUB;
      d->data.c = 1.0/(i + 1);
    }
    vec[i] = d;
  }
  return vec;
}

double iterate(Data** vec, const unsigned SIZE) {
  double pSum;
  for (int i = 0; i < SIZE; i++) {
    switch (vec[i]->tag) {
      case UINT:
        pSum += vec[i]->data.u;
        break;
      case DOUB:
        pSum += vec[i]->data.d;
        break;
      case CHAR:
        pSum += vec[i]->data.c;
        break;
      default:
        printf("Invalid option\n");
    }
  }
  return pSum;
}

void run_experiment(Data** vec, unsigned SIZE, unsigned ITERs) {
  double sum_avg = 0;
  clock_t start;
  clock_t end;
  start = clock();
  for (int i = 0; i < ITERs; i++) {
    sum_avg += iterate(vec, SIZE);
  }
  end = clock();
  double seconds = (float)(end - start) / CLOCKS_PER_SEC;
  printf("%.2lf (%.2lf) ", seconds, sum_avg);
}

int main(int argc, char** argv) {
  if (argc == 6) {
    // Read the arguments:
    const unsigned N = atoi(argv[1]);
    const float C = atof(argv[2]);
    const float U = atof(argv[3]);
    const unsigned T = atoi(argv[4]);
    const unsigned X = atoi(argv[5]);
    assert((C + U >= 0.0 && C + U < 1.0) && "Invalid probabilities.");
    // Initialize the vector of unions:
    Data** vec = init(N, C, U);
    // Run the experiments:
    for (int i = 0; i < X; i++) {
      run_experiment(vec, N, T);
    }
    printf("\n");
  } else {
    printf("Syntax: vec[N] pChar[C] pUint[U] tests[T] experiments[X]\n");
  }
  return 0;
}
