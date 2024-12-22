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
  template <typename Tip1> friend int presjek(Stek<Tip1> s1, Stek<Tip1> s2);
  Tip najveci();
  void sortiraj();
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

template <typename Tip> void Stek<Tip>::sortiraj() {
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

template <typename Tip> int presjek(Stek<Tip> s1, Stek<Tip> s2) {
  s1.sortiraj();
  s2.sortiraj();
  int brojac = 0;
  auto *it1 = s1.top;
  while (it1 != nullptr) {
    auto *it2 = s2.top;
    while (it2 != nullptr) {
      if (it1->el == it2->el)
        brojac++;
      it2 = it2->sljedeci;
    }
    it1 = it1->sljedeci;
  }
  return brojac;
}

int main() {
  Stek<char> s, s1;
  s.stavi('A');
  s.stavi('D');
  s.stavi('F');
  s.stavi('C');
  s1.stavi('E');
  s1.stavi('B');
  s1.stavi('F');
  s1.stavi('D');
  s1.stavi('C');
  s1.stavi('G');
  int b = presjek(s, s1);
  std::cout << "Imaju " << b << " zajednicka elementa";
  return 0;
}