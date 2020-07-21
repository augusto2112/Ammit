#include <stdio.h>
#include <math.h>

int external_non_library_func();

/*3-case switch within a loop, there's a function call, but it's to printf,
which is a standard library function so it is fine.*/
int f(int *a, int n) {
  int sum = 0;
  for (int i = 0; i < n; i++) {
    switch (a[i] % 3) {
      case 0:
        sum += i * 2;
        break;
      case 1:
        sum += i * 3;
        break;
      case 2:
        sum += 54;
        break;
    }
  }

  printf("sum is: %d\n", sum);
  return sum;
}

struct ST {
  int X;
  double Y;
};

int dummy() {
  return 42;
}

/*3-case switch, but no loop, bad!*/
int g(struct ST st) {
  switch (st.X % 3) {
    case 0:
      return st.X * 2;
    case 1:
      return st.X * 3;
    case 2:
      return sqrt(54);
  }

  return 0;
}

/*3-case switch within a loop. There's a function call to dummy(), but luckily
dummy() is defined in this translation unit, so it's fine!*/
int h(struct ST **st, int N) {
  int sum = 0;
  for (int i = 0; i < N; i++) {
    switch (st[i]->X % 3) {
      case 0:
        sum += i * 2;
        break;
      case 1:
        sum += i * 3;
        break;
      case 2:
        sum += dummy();
        break;
    }
  }
  return sum;
}

/*3-case switch within a loop, which is great. Unfortunately, there's a call
to an external non-standard function. Bad!*/
int x(int *a, int n) {
  int sum = 0;
  for (int i = 0; i < n; i++) {
    switch (a[i] % 3) {
      case 0:
        sum += i * 2;
        break;
      case 1:
        sum += i * 3;
        break;
      case 2:
        sum += external_non_library_func();
        break;
    }
  }
  return sum;
}

int main() {
  return 0;
}
