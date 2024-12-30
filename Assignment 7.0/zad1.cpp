#include <fstream>
#include <iostream>
#include <vector>
#include <time.h>
#include <sstream>

using namespace std;

template <typename Tip> void selection_sort(Tip *niz, int vel) {
  if (niz == nullptr)
    return;
  for (int i = 0; i < vel - 1; i++) {
    int indeks = i;
    Tip min = niz[i];
    for (int j = i + 1; j < vel; j++) {
      if (niz[j] < min) {
        min = niz[j];
        indeks = j;
      }
    }
    niz[indeks] = niz[i];
    niz[i] = min;
  }
}

template <typename Tip> void bubble_sort(Tip *niz, int vel) {
  if (niz == nullptr)
    return;
  for (int i = 0; i < vel - 1; i++) {
    for (int j = 0; j < vel - 1 - i; j++) {
      if (niz[j] > niz[j + 1]) {
        Tip temp = niz[j + 1];
        niz[j + 1] = niz[j];
        niz[j] = temp;
      }
    }
  }
}

template <typename Tip> int particija(Tip *niz, int prvi, int zadnji) {
  Tip pivot = niz[prvi];
  int p = prvi + 1;
  while (p <= zadnji && niz[p]<=pivot) p++;
  for (int i = p+1; i <= zadnji; i++) {
     if (niz[i] < pivot) {
     std::swap(niz[i],niz[p]);
     p++;
     }
    }
     std::swap(niz[p-1],niz[prvi]);
     return p-1;
}

template <typename Tip> void quick_sort(Tip *niz, int vel) {
  static int prvi = 0;
  static int zadnji = vel - 1;
  if(prvi==zadnji) return;
  if (zadnji > prvi) {
    int j = particija(niz, prvi, zadnji);
    int a = zadnji;
    zadnji = j-1;
    quick_sort(niz, vel);
    prvi = j + 1;
    zadnji = a;
    quick_sort(niz, vel);
  }
}

template <typename Tip> void merge(Tip *niz, int l, int p, int q, int h) {
  Tip *temp = new Tip[l + h + 10];
  for (int i = l; i <= h; i++) {
    temp[i] = niz[i];
  }
  int i = l;
  int j = q;
  int k = l;
  while (i <= p && j <= h) {
    if (temp[i] < temp[j]) {
      niz[k] = temp[i];
      i++;
    } else {
      niz[k] = temp[j];
      j++;
    }
    k++;
  }
  while (i <= p) {
    niz[k] = temp[i];
    k++;
    i++;
  }
  while (j <= h) {
    niz[k] = temp[j];
    k++;
    j++;
  }
  delete[] temp;
}

template <typename Tip> void merge_sort(Tip *niz, int vel) {
  static int l = 0;       // l-low
  static int h = vel - 1; // h-high
  if (h == l)
    return;
  if (h > l) {
    int mid = (l + h - 1) / 2;
    int a = l;
    int b = h;
    h = mid;
    merge_sort(niz, vel);
    l = mid + 1;
    h = b;
    merge_sort(niz, vel);
    l = a;
    merge(niz, l, mid, mid + 1, h);
  }
}

void ucitaj(string filename, int *&niz, int &vel) {
  std::vector<int> unos;
  std::ifstream izlaz(filename);
  if (izlaz.is_open()) {
    int broj;
    while (izlaz >> broj) {
      unos.push_back(broj);
    }
    izlaz.close();
  } else
    return;
  niz = new int[unos.size()];
  for (int i = 0; i < unos.size(); i++)
    niz[i] = unos.at(i);
  vel = unos.size();
}

void generisi(string filename, int vel) {
  std::ofstream ulaz(filename);
  for (int i = 0; i < vel; i++) {
    ulaz << char(std::rand());
  }
  ulaz.close();
}

bool provjera(int *niz, int vel)
{
    if(niz==nullptr) return true;
    for(int i=0; i<vel-1; i++)
    {
        if(niz[i]>niz[i+1]) return false;
    }
    return true;
}

void funkcija(std::string filename, int *niz, int &vel, std::string izlaz) {
  ucitaj(filename, niz, vel);
  std::cout<<"Niz glasi: ";
  for(int i=0; i<vel; i++)
  {
      std::cout<<niz[i]<<" ";
  }
  std::cout<<std::endl;
  int unos;
  std::cout<<"Unesi algoritam: 0-bubble_sort, 1-selection_sort, 2-quick_sort, 3-merge_sort: ";
  std::cin>>unos;
  clock_t vrijeme1=clock();
  switch(unos)
  {
      case(0):
      {
          bubble_sort(niz,vel);
          break;
      }
      case(1):
      {
          selection_sort(niz,vel);
          break;
      }
      case(2):
      {
          quick_sort(niz,vel);
          break;
      }
      case(3):
      {
          merge_sort(niz,vel);
          break;
      }
      default:
      {
          std::cout<<"Ne valja unos;";
          return;
      }
  }
  clock_t vrijeme2=clock();
  int vrijeme=(vrijeme2-vrijeme1) /(CLOCKS_PER_SEC/1000);
  std::cout<<"Vrijeme izvrsenja: "<<vrijeme<<" ms."<<std::endl;
  bool a=provjera(niz,vel);
  if(a)
  {
      std::cout<<"Niz je sortiran i glasi: ";
      for(int i=0; i<vel; i++)
      {
          std::cout<<niz[i]<<" ";
      }
      std::cout<<std::endl;
      std::ofstream datoteka(izlaz);
      for(int i=0; i<vel; i++)
      {
          char znak='0'+niz[i];
          datoteka.put(znak);
          if(i!=vel-1) datoteka.put(' ');
      }
      datoteka.close();
  }else std::cout<<"Niz nije sortiran";
}

int main() {
    int *niz=nullptr, vel=0;
    funkcija("ulaz.txt",niz,vel,"izlaz.txt");
    return 0;
}