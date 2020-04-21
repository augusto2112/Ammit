#include <time.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

struct DataStruct {
  int x;
};

typedef struct DataStruct Data;

Data** init(const unsigned SIZE, const unsigned DENSITY) {
  Data** vec = (Data**)malloc(SIZE * sizeof(Data*));
  for (int i = 0; i < SIZE; i++) {
    if (i % DENSITY == 0) {
      Data* d = (Data*)malloc(sizeof(Data));
      d->x = i;
      vec[i] = d;
    } else {
      vec[i] = NULL;
    }
  }
  return vec;
}

long iterateNotNULL(Data** vec, const unsigned SIZE) {
  long pSum = 0;
  long nSum = 0;
  unsigned pCounter = 0;
  unsigned nCounter = 0;
  for (int i = 0; i < SIZE; i++) {
    int jmp = (vec[i] != NULL);
    switch (jmp) {
      // INSERT HERE
			// START 1
      case 0:
        nSum += 1;
        nCounter++;
        break;
      // END 1
      // START 2
      case 1:
        pSum += vec[i]->x;
        pCounter++;
        break;
      // END 2
      default:
        break;
    }
  }
  return pSum - nSum;
}

void run_experiment(Data** vec, unsigned SIZE, unsigned ITERs) {
  float sum_avg = 0.0;
  clock_t start;
  clock_t end;
  start = clock();
  for (int i = 0; i < ITERs; i++) {
    sum_avg += iterateNotNULL(vec, SIZE);
  }
  end = clock();
  double seconds = (float)(end - start) / CLOCKS_PER_SEC;
  printf("%.2lf", seconds);
}

int main(int argc, char** argv) {
  if (argc == 4) {
    const unsigned N = atoi(argv[1]);
    const unsigned K = atoi(argv[2]);
    const unsigned T = atoi(argv[3]);

    Data** vec = init(N, K);
    run_experiment(vec, N, T);
  } else {
    printf("Syntax: vec[N] mod[K] tests[T]\n");
    return 1;
  }
  return 0;
}
