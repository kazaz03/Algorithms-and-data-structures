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
  void skini(const Tip &el);
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
vremenska kompleksnost rješenja
*/

template <typename Tip> void Red<Tip>::veci_od_asr() {
  Tip suma = Tip{};
  auto it = pocetak;
  while (it != nullptr) {
    suma += it->el;
    it = it->sljedeci;
  }
  double asr = suma / br_el;
  it = pocetak;
  Cvor *prosli = nullptr;
  while (it != nullptr) {
    bool poc = false;
    bool brisan = false;
    Cvor *slj = it->sljedeci;
    if (it->el > asr) {
      if (it == pocetak) {
        poc = true;
        pocetak = it->sljedeci;
      }
      if (prosli) {
        prosli->sljedeci = nullptr;
        prosli->sljedeci = it->sljedeci;
      }
      it->sljedeci = nullptr;
      delete it;
      brisan = true;
      br_el--;
    }
    if (!poc) {
      prosli = it;
    }
    it = slj;
  }
}

int main() {
  Red<int> r1, r2;
  r1.stavi(-12);
  r1.stavi(12);
  r1.stavi(3);
  r1.veci_od_asr();
  std::cout << r1.brojElemenata();
  r1.brisi();
  return 0;
}