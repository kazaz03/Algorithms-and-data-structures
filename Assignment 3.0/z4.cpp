#include <iostream>

using namespace std;

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

/*
Napisati funkciju koja prima red i modifikuje ga tako da budu izbačene sve
vrijednosti veće od srednje vrijednosti njegovih elemenata. Ustanoviti koja je
vremenska kompleksnost rješenja. Napomena: Zadatak riješiti koristeći samo
ulazni red, nije dozvoljeno korištenje pomoćnih kolekcija.
*/

template <typename Tip> void Red<Tip>::veci_od_asr() {
  Tip suma = {};
  auto it = pocetak;
  while (it != nullptr) {
    suma += it->el;
    it = it->sljedeci;
  }
  double asr = double(suma) / br_el;
  it = pocetak;
  int brojac = 0;
  int broj = br_el;
  while (brojac < broj) {
    it = pocetak;
    if (it->el > asr) {
      skini();
    } else {
      stavi(it->el);
      skini();
    }
    brojac++;
  }
}

int main() {
  Red<int> r1, r2;
  r1.stavi(2);
  r1.stavi(3);
  r1.stavi(4);
  r1.stavi(5);
  r1.stavi(1);
  r2.stavi(10);
  r1.veci_od_asr();
  int brel=r1.brojElemenata();
  for(int i=0; i<brel; i++)
  {
      std::cout<<r1.celo()<<" ";
      r1.skini();
  }
  return 0;
}