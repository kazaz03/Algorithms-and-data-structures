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
int spoji(Stek<int> s1, Stek<int> s2, Stek<int>& s3)
Funkcija prima stek s1 koji je sortiran rastući, stek s2 koji je sortiran
opadajući i s3 koji je prazan. Potrebno je prebaciti elemente iz steka s1 i s2 u
stek s3 tako da stek bude sortiran rastući pri čemu je dozvoljeno koristiti samo
tri data steka, tj. nisu dozvoljene dodatne kolekcije. Primjer: Neka je stek s1:
{1, 3, 5, 8} a s2: {9, 6, 4, 2} (prvi element je na dnu steka). Nakon pozvane
funkcije spoji stek s3 treba da bude {1, 2, 3, 4, 5, 6, 8, 9}.
*/

int spoji(Stek<int> s1, Stek<int> s2, Stek<int> &s3) {
  int brojel = s1.brojElemenata() + s2.brojElemenata();
  // s1: 1,3,5 (na vrhu je 5)  s2: 8,4,2 (na vrhu je 2)
  while (s2.brojElemenata() != 0) {
    int el = s2.vrh();
    s3.stavi(el);
    s2.skini();
  }
  // s1:1,3,5 s2:prazan   s3:2,4,8
  while (s1.brojElemenata() != 0 && s3.brojElemenata() != 0) {
    int el1 = s1.vrh();
    int el3 = s3.vrh();
    if (el1 > el3) {
      s2.stavi(el1);
      s1.skini();
    } else {
      s2.stavi(el3);
      s3.skini();
    }
  }
  while (s1.brojElemenata() != 0) {
    s2.stavi(s1.vrh());
    s1.skini();
  }
  while (s3.brojElemenata() != 0) {
    s2.stavi(s3.vrh());
    s3.skini();
  }
  while(s2.brojElemenata()!=0)
  {
      s3.stavi(s2.vrh());
      s2.skini();
  }
  return brojel;
}

int main() {
  Stek<int> s1, s2, s3;
  s1.stavi(1);
  s1.stavi(3);
  s1.stavi(5);
  s1.stavi(6);
  s1.stavi(12);
  s2.stavi(11);
  s2.stavi(10);
  s2.stavi(9);
  s2.stavi(8);
  s2.stavi(7);
  s2.stavi(4);
  s2.stavi(2);
  spoji(s1, s2, s3);
  while(s3.brojElemenata()!=0)
   {
       std::cout<<s3.vrh()<<" ";
       s3.skini();
   }
  return 0;
}