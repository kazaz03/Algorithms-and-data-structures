#include <iostream>

using namespace std;

int fib(int n) {
  if (n == 0)
    return 0;
  if (n == 1)
    return 1;
  return fib(n - 2) + fib(n - 1);
}

int fib2_0(int n, int trenutni = 0, int rezultat = 1) {
  if (n == 0) {
    return trenutni;
  } else if (n == 1) {
    return rezultat;
  } else {
    return fib2_0(n - 1, rezultat, trenutni + rezultat);
  }
}

int nzd(int x, int y) {
  if (y == 0)
    return x;
  return nzd(y, x % y);
}

int main() {
  std::cout << fib2_0(2);
  return 0;
}