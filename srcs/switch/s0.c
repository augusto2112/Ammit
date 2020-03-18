#include <time.h>
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char** argv) {
  if (argc == 6) {
    long sum = 0;
    clock_t start;
    clock_t end;
    float seconds;
    const int N = atoi(argv[1]);
    const int M = atoi(argv[2]);
    const int K = atoi(argv[3]);
    const char I = argv[4][0];
    const int T = atoi(argv[5]);
    int first = 17;
    int second = 42;
    char vec[N];
    if (I == 'i') {
      first = 42;
      second = 17;
    }
    for (int i = 0; i < N; i++) {
      vec[i] = (i % K) ? first : second;
    }
    for (int k = 0; k < T; k++) {
      sum = 0;
      start = clock();
      for (int j = 0; j < M; j++) {
        for (int i = 0; i < N; i++) {
          switch(vec[i]) {
            case 42:
              sum += 42;
              break;
            case 25:
              sum ^= 25;
              break;
            case 17:
              sum += 17;
              break;
            default:
              printf("Other number\n");
              break;
          }
        }
      }
      end = clock();
      seconds = (float)(end - start) / CLOCKS_PER_SEC;
      printf("%.2f ", seconds);
    }
    printf("\nSum = %ld\n", sum);
  } else {
    printf("Syntax: vec[N] loop[M] mod[K] inv[i] tests[T]\n");
  }
  return 0;
}
