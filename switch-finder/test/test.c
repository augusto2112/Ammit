#include <stdio.h>

int f(int *a, int n) {
  int sum = 0;
  for (int i = 0; i < n; i++) {
    switch (a[i] % 4) {
    case 1:
      sum += i * 2;
    case 2:
      sum += i * 3;
    case 3:
      sum += 54;
    }
  }
  return sum;
}

struct ST {
  int X;
  double Y;
};


int g(struct ST st) {
  switch (st.X % 4) {
    case 1:
      return st.X * 2;
    case 2:
      return st.X * 3;
    case 3:
      return 54;
  }

  return 0;
}

int h(struct ST **st, int N) {
  int sum = 0;
  for (int i = 0; i < N; i++) {
    switch (st[i]->X % 4) {
    case 1:
      sum += i * 2;
    case 2:
      sum += i * 3;
    case 3:
      sum += 54;
    }
  }
  return sum;
}


int main() {
//  printf("%i\n", sum);
  return 0;
}