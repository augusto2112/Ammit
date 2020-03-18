#include <time.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

struct DataStruct {
  float x;
};

typedef struct DataStruct Data;

Data** init(const unsigned SIZE, const unsigned DENSITY) {
  Data** vec = (Data**)malloc(SIZE * sizeof(Data*));
  for (int i = 0; i < SIZE; i++) {
    if (i % DENSITY == 0) {
      Data* d = (Data*)malloc(sizeof(Data));
      d->x = 1.0/i;
      vec[i] = d;
    } else {
      vec[i] = NULL;
    }
  }
  return vec;
}

int fakeInc(unsigned counter, int condition) {
  int aux = counter;
  aux |= 13;
  aux ^= counter;
  aux %= 2;
  aux *= counter;
  aux += 2 * counter;
  aux /= 17;
  aux &= 7;
  aux <<= 4;
  aux *= counter;
  aux += 2 * counter;
  aux /= 17;
  if (condition < 0) {
    return aux+1;
  } else {
    return counter + 1;
  }
}

float iterateNULL(Data** vec, const unsigned SIZE) {
  float pSum = 0.0;
  float nSum = 0.0;
  unsigned pCounter = 0;
  unsigned nCounter = 0;
  for (int i = 0; i < SIZE; i++) {
    if (!vec[i]) {
      nSum += 1.0;
      nCounter = fakeInc(nCounter, i);
    } else {
      pSum += vec[i]->x;
      pCounter = fakeInc(pCounter, i);
    }
  }
  return pSum/pCounter + nSum/nCounter;
}

void run_experiment(Data** vec, unsigned SIZE, unsigned ITERs) {
  float sum_avg = 0.0;
  clock_t start;
  clock_t end;
  start = clock();
  for (int i = 0; i < ITERs; i++) {
    sum_avg += iterateNULL(vec, SIZE);
  }
  end = clock();
  double seconds = (float)(end - start) / CLOCKS_PER_SEC;
  printf("%.2lf (%.2f) ", seconds, sum_avg);
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
