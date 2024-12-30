#include <iostream>
#include <limits>

template <typename Tip> void countingSort(Tip *a, int vel, int *bNiz, int k) {
  int *c = new int[k+1];
  for (int i = 0; i < vel; i++) {
    c[a[i]]++;
  }
  for(int i=1; i<=k; i++)
  {
      c[i]=c[i]+c[i-1];
  }
  for(int i=vel-1; i>=0; i--)
  {
      c[a[i]]--;
      bNiz[c[a[i]]]=a[i];
  }
}

int main() {
  int *niz = new int[7]{1, 5, 2, 3, 2,7,9};
  int *sortiran = new int[7];
  countingSort(niz, 7, sortiran, 9);
  std::cout << "Niz glasi: ";
  for (int i = 0; i < 7; i++) {
    std::cout << sortiran[i] << " ";
  }
  return 0;
}