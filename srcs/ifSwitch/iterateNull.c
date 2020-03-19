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

long iterateNULL(Data** vec, const unsigned SIZE) {
  long pSum = 0;
  long nSum = 0;
  unsigned pCounter = 0;
  unsigned nCounter = 0;
  for (int i = 0; i < SIZE; i++) {
    int jmp = (vec[i] == NULL);
    switch (jmp) {
      case 0:
        pSum += vec[i]->x;
        pCounter++;
        break;
      case 1:
        nSum += 1;
        nCounter++;
        break;
      default:
        printf("Invalid option\n");
    }
  }
  return pSum - nSum;
}

void run_experiment(Data** vec, unsigned SIZE, unsigned ITERs) {
  long sum_avg = 0;
  clock_t start;
  clock_t end;
  start = clock();
  for (int i = 0; i < ITERs; i++) {
    sum_avg += iterateNULL(vec, SIZE);
  }
  end = clock();
  double seconds = (float)(end - start) / CLOCKS_PER_SEC;
  printf("%.2lf (%ld) ", seconds, sum_avg);
}

int main(int argc, char** argv) {
  if (argc == 5) {
    const unsigned N = atoi(argv[1]);
    const unsigned K = atoi(argv[2]);
    const unsigned T = atoi(argv[3]);
    const unsigned X = atoi(argv[4]);

    Data** vec = init(N, K);
    for (int i = 0; i < X; i++) {
      run_experiment(vec, N, T);
    }
    printf("\n");
  } else {
    printf("Syntax: vec[N] mod[K] tests[T] experiments[X]\n");
  }
  return 0;
}
