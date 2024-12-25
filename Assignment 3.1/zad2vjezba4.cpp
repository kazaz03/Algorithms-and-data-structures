#include <iostream>

using namespace std;

template <typename Tip> class Stek;

template <typename Tip> class Red {
  struct Cvor {
    Tip el;
    Cvor *sljedeci;
  };
  Cvor *pocetak;
  Cvor *kraj;
  int br_el;
  void brisanje_el() {
    if (br_el == 0)
      return;
    auto it = pocetak;
    while (it != nullptr) {
      Cvor *sljedeci = it->sljedeci;
      it->sljedeci = nullptr;
      delete it;
      it = sljedeci;
    }
    pocetak = nullptr;
    kraj = nullptr;
    br_el = 0;
  }

public:
  Red() : br_el(0), kraj(nullptr), pocetak(nullptr){};
  ~Red() { brisanje_el(); }
  Red(const Red &R);
  Red(Red &&R) {
    pocetak = R.pocetak;
    kraj = R.kraj;
    br_el = R.br_el;
  }
  Red &operator=(const Red &R);
  Red &operator=(Red &&R);
  void brisi() { brisanje_el(); };
  void stavi(const Tip &el);
  Tip skini();
  Tip &celo() {
    if (!br_el)
      throw std::logic_error("Red je prazan");
    return pocetak->el;
  }
  int brojElemenata() const { return br_el; };
  void veci_od_asr();
  template <typename Tip1>
  friend void red_stek(Stek<Tip1> &stek, Red<Tip1> &red, int pocetnaDuzinaReda);
};

template <typename Tip> Red<Tip> &Red<Tip>::operator=(const Red &R) {
  if (this == &R)
    return *this;
  brisanje_el();
  auto it = R.pocetak;
  bool prvi = true;
  Cvor *prosli = nullptr;
  while (it != nullptr) {
    Cvor *novi = new Cvor{it->el, nullptr};
    if (prvi) {
      pocetak = novi;
      prosli = pocetak;
    } else {
      prosli->sljedeci = novi;
      prosli = novi;
    }
    it = it->sljedeci;
    prvi = false;
  }
  kraj = prosli;
  prosli = nullptr;
  br_el = R.br_el;
  return *this;
}

template <typename Tip> Red<Tip> &Red<Tip>::operator=(Red &&R) {
  if (this == &R)
    return *this;
  std::swap(pocetak, R.pocetak);
  std::swap(kraj, R.kraj);
  std::swap(br_el, R.br_el);
  return *this;
}

template <typename Tip> Red<Tip>::Red(const Red &R) {
  auto it = R.pocetak;
  bool prvi = true;
  Cvor *prosli = nullptr;
  while (it != nullptr) {
    Cvor *novi = new Cvor{it->el, nullptr};
    if (prvi) {
      pocetak = novi;
      prosli = pocetak;
    } else {
      prosli->sljedeci = novi;
      prosli = novi;
    }
    it = it->sljedeci;
    prvi = false;
  }
  kraj = prosli;
  prosli = nullptr;
  br_el = R.br_el;
}

template <typename Tip> void Red<Tip>::stavi(const Tip &el) {
  Cvor *novi = new Cvor{el, nullptr};
  if (br_el == 0) {
    pocetak = novi;
    kraj = novi;
    br_el++;
  } else {
    kraj->sljedeci = novi;
    kraj = novi;
    br_el++;
  }
}

template <typename Tip> Tip Red<Tip>::skini() {
  if (!br_el)
    throw std::logic_error("Red je prazan");
  Tip el = pocetak->el;
  Cvor *sljed = pocetak->sljedeci;
  pocetak->sljedeci = nullptr;
  delete pocetak;
  pocetak = sljed;
  br_el--;
  if (br_el == 0)
    kraj = nullptr;
  return el;
}

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
  template <typename Tip1>
  friend void red_stek(Stek<Tip1> &stek, Red<Tip1> &red, int pocetnaDuzinaReda);
};

template <typename Tip> Stek<Tip> &Stek<Tip>::operator=(const Stek<Tip> &S) {
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
Napisati funkciju koja prima stek, red i varijablu pocetnaDuzinaReda tipa int
koja će elemente iz reda ubaciti na dno steka u obrnutom redoslijedu. Primjer:
Neka je dat red sa elementima {1, 2, 3, 4} gdje je element 1 na čelu reda i stek
sa elementima {5, 6, 7, 8} gdje je element 8 na vrhu steka. Rezultantni stek se
sastoji od elemenata {1, 2, 3, 4, 5, 6, 7, 8} gdje je element 8 na vrhu steka.
Ustanoviti koja je vremenska kompleksnost rješenja. Napomena: Zadatak riješiti
koristeći samo ulazne parametre, nije dozvoljeno korištenje pomoćnih kolekcija
*/

template <typename Tip1>
void red_stek(Stek<Tip1> &stek, Red<Tip1> &red, int pocetnaDuzinaReda) {
  int duzinaSteka = stek.brojElemenata();
  auto it = stek.top;
  while (stek.brojElemenata() != 0) {
    it = stek.top;
    red.stavi(it->el);
    stek.skini();
  }
  auto it1 = red.pocetak;
  int brojac = 0;
  while (brojac < pocetnaDuzinaReda) {
    it1 = red.pocetak;
    red.stavi(it1->el);
    red.skini();
    brojac++;
  }
  brojac = 0;
  it1 = red.pocetak;
  while (brojac < duzinaSteka) {
    it1 = red.pocetak;
    stek.stavi(it1->el);
    red.skini();
    brojac++;
  }
  brojac = 0;
  auto i = stek.top;
  while (brojac < duzinaSteka) {
    i = stek.top;
    red.stavi(i->el);
    stek.skini();
    brojac++;
  }
  brojac = 0;
  while (brojac < duzinaSteka + pocetnaDuzinaReda) {
    auto k = red.pocetak;
    stek.stavi(k->el);
    red.skini();
    brojac++;
  }
}

int main() {
  Red<int> r1;
  r1.stavi(1);
  r1.stavi(2);
  r1.stavi(3);
  r1.stavi(4);
  Stek<int> s1;
  s1.stavi(5);
  s1.stavi(6);
  s1.stavi(7);
  s1.stavi(8);
  red_stek(s1, r1, r1.brojElemenata());
  int br=s1.brojElemenata();
  int brojac=0;
  while(brojac<br)
  {
      std::cout<<s1.vrh()<<" ";
      s1.skini();
      brojac++;
  }
  return 0;
}