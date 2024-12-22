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
  int brojElemenata() const { return velicina; };
  Tip najveci();
  Tip najmanji();
  void sortirajrastuci();
  void sortirajopadajuci();
  bool skini(Tip el);
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
  // s1 da je rastuci 1 3 5 8
  // s2 da je opadajuci 9 6 4 2
  s1.sortirajrastuci();
  s2.sortirajopadajuci();
  // s3 rastuci
  int vel1 = s1.brojElemenata();
  int vel2 = s2.brojElemenata();
  while (vel1 != 0 && vel2 != 0) {
    int el1 = s1.najveci();
    int el2 = s2.najveci();
    bool prviveci = false;
    if (el1 > el2)
      prviveci = true;
    if (prviveci) {
      s3.stavi(el1);
      s1.skini(el1);
      vel1--;
    } else {
      s3.stavi(el2);
      s2.skini(el2);
      vel2--;
    }
  }
  while (vel1 > 0) {
    int vel = s1.najveci();
    s1.skini(vel);
    s3.stavi(vel);
    vel1--;
  }
  while (vel2 > 0) {
    int vel = s2.najveci();
    s2.skini(vel);
    s3.stavi(vel);
    vel2--;
  }
  return s3.brojElemenata();
}

template <typename Tip> bool Stek<Tip>::skini(Tip el) {
  jelprazan();
  if (top->el == el) {
    Cvor *sljedeci = top->sljedeci;
    top->sljedeci = nullptr;
    delete top;
    top = sljedeci;
    velicina--;
    return true;
  } else {
    Cvor *it = top;
    Cvor *prosli = nullptr;
    bool obrisan = false;
    while (it != nullptr) {
      if (it->el == el) {
        Cvor *slj = it->sljedeci;
        it->sljedeci = nullptr;
        delete it;
        prosli->sljedeci = slj;
        obrisan = true;
      }
      if (obrisan) {
        velicina--;
        return true;
      }
      prosli = it;
      it = it->sljedeci;
    }
  }
  return false;
}

template <typename Tip> Tip Stek<Tip>::najveci() {
  jelprazan();
  Tip najveci = vrh();
  Cvor *it = top;
  while (it != nullptr) {
    if (it->el >= najveci)
      najveci = it->el;
    it = it->sljedeci;
  }
  return najveci;
}

template <typename Tip> Tip Stek<Tip>::najmanji() {
  jelprazan();
  Tip najmanji = vrh();
  Cvor *it = top;
  while (it != nullptr) {
    if (it->el <= najmanji)
      najmanji = it->el;
    it = it->sljedeci;
  }
  return najmanji;
}

template <typename Tip> void Stek<Tip>::sortirajrastuci() {
  // na vrhu je najmanji npr 6 3 4 8 2 bude 2 3 4 6 8 tjst da je top 2
  Stek<Tip> privremeni;
  while (brojElemenata() != 0) {
    Tip veliki = najveci();
    skini(veliki);
    privremeni.stavi(veliki);
  }
  top = privremeni.top;
  velicina = privremeni.velicina;
  privremeni.top = nullptr;
  privremeni.velicina = 0;
}

template <typename Tip> void Stek<Tip>::sortirajopadajuci() {
  Stek<Tip> privremeni;
  while (brojElemenata() != 0) {
    Tip mali = najmanji();
    skini(mali);
    privremeni.stavi(mali);
  }
  top = privremeni.top;
  velicina = privremeni.velicina;
  privremeni.top = nullptr;
  privremeni.velicina = 0;
}

int main() {
  Stek<int> s, s1, s3;
  s.stavi(8);
  s.stavi(5);
  s.stavi(1);
  s.stavi(3);
  s1.stavi(4);
  s1.stavi(7);
  s1.stavi(2);
  s1.stavi(9);
  s1.stavi(6);
  int a = spoji(s, s1, s3);
  for (int i = 0; i < a; i++) {
    std::cout << s3.vrh() << " ";
    s3.skini();
  }
  return 0;
}