#include <iostream>

using namespace std;

template <typename Tip> class Stek {
  struct Cvor {
    Tip el;
    Cvor *sljedeci;
  };
  Cvor *top;
  int velicina;
  void jelprazan() {
    if (velicina == 0)
      throw std::logic_error("Stek je prazan");
  }
  void obrisiel() {
    while (top != nullptr) {
      Cvor *a = top->sljedeci;
      top->sljedeci = nullptr;
      delete top;
      top = a;
    }
    velicina = 0;
  }

public:
  Stek() : velicina(0), top(nullptr){};
  ~Stek() { obrisiel(); }
  Stek(const Stek &S);
  Stek(Stek &&S) {
    top = S.top;
    velicina = S.velicina;
  }
  Stek &operator=(const Stek &S);
  Stek &operator=(Stek &&S) {
    std::swap(top, S.top);
    std::swap(velicina, S.velicina);
    return *this;
  }
  void brisi() { obrisiel(); } // done
  void stavi(const Tip &el);   // done
  Tip skini();                 // done
  Tip &vrh() {
    jelprazan();
    return top->el;
  }
  Tip vrh() const {
    jelprazan();
    return top->el;
  }
  int brojElemenata() const { return velicina; }
};

template <typename Tip>
Stek<Tip> &Stek<Tip>::operator=(const Stek<Tip> &S) { // popravi
  if (this == &S)
    return *this;
  obrisiel();
  auto it = S.top;
  int vel = S.brojElemenata();
  while (vel > 0) {
    it = S.top;
    for (int i = 0; i < vel - 1; i++) {
      it = it->sljedeci;
    }
    vel--;
    stavi(it->el);
  }
  return *this;
}

template <typename Tip> Stek<Tip>::Stek(const Stek<Tip> &S) {
  velicina = 0;
  top = nullptr;
  if (this == &S)
    return;
  auto it = S.top;
  int vel = S.brojElemenata();
  while (vel > 0) {
    it = S.top;
    for (int i = 0; i < vel - 1; i++) {
      it = it->sljedeci;
    }
    vel--;
    stavi(it->el);
  }
}

template <typename Tip> Tip Stek<Tip>::skini() {
  jelprazan();
  Tip element = top->el;
  Cvor *it = top->sljedeci;
  top->sljedeci = nullptr;
  delete top;
  top = it;
  velicina--;
  return element;
}

template <typename Tip> void Stek<Tip>::stavi(const Tip &el) {
  if (top == nullptr) {
    top = new Cvor{el, nullptr};
    velicina++;
  } else {
    Cvor *novi = new Cvor{el, nullptr};
    novi->sljedeci = top;
    top = novi;
    velicina++;
    novi = nullptr;
  }
}

/*
Napisati funkciju:
int presjek(Stek<Tip> s1, Stek<Tip> s2)
koja prima dva sortirana generička steka (tako da se na vrhu steka nalazi
najmanji element), a vraća broj elemenata u njihovom presjeku odnosno broj
elemenata koji se javljaju u oba steka. Pretpostaviti da se elementi ne
ponavljaju u istom steku. Primjer: Neka su u prvom steku elementi A C D F, a u
drugom B C D E F G. Funkcija treba vratiti broj 3 pošto postoje tri elementa
koji se nalaze u oba steka (a to su C D F).
*/

template <typename Tip> int presjek(Stek<Tip> s1, Stek<Tip> s2) {
  // s1: d,c,b,a   s2: d,b,a
  int brojac = 0;
  while (s1.brojElemenata() != 0 && s2.brojElemenata() != 0) {
    if (s1.vrh() == s2.vrh()) {
      s1.skini();
      s2.skini();
      brojac++;
    } else {
      if (s1.brojElemenata() > s2.brojElemenata())
        s1.skini();
      else
        s2.skini();
    }
  }
  return brojac;
}

int main() {
  Stek<char> s1, s2;
  s1.stavi('Z');
  s1.stavi('B');
  s1.stavi('G');
  s1.stavi('L');
  s2.stavi('A');
  s2.stavi('B');
  s2.stavi('D');
  int s = presjek(s1, s2);
  std::cout <<"Imaju "<<s<<" zajednicka elementa";
  return 0;
}