#include <iostream>

using namespace std;

template <typename Tip> class DvostraniRed {
  struct Cvor {
    Tip el;
    Cvor *sljedeci;
    Cvor *prethodni;
  };
  Cvor *pocetak;
  Cvor *kraj;
  int br_el;

public:
  DvostraniRed() : pocetak(nullptr), kraj(nullptr), br_el(0){};
  ~DvostraniRed() { brisi(); }
  void brisi() {
    if (br_el == 0)
      return;
    while (pocetak != nullptr) {
      Cvor *temp = pocetak->sljedeci;
      delete pocetak;
      pocetak = temp;
    }
    kraj = nullptr;
    br_el = 0;
  }
  DvostraniRed(DvostraniRed &&DR) {
    pocetak = DR.pocetak;
    kraj = DR.kraj;
    br_el = DR.br_el;
  }
  DvostraniRed(const DvostraniRed &DR) {
      pocetak=nullptr;
      kraj=nullptr; br_el=0;
    auto it = DR.pocetak;
    while (it != nullptr) {
      Cvor *temp = it->sljedeci;
      staviNaVrh(it->el);
      it = temp;
    }
    br_el = DR.br_el;
  }
  DvostraniRed &operator=(const DvostraniRed &DR) {
    if (this == &DR)
      return *this;
    brisi();
    auto it = DR.pocetak;
    while (it != nullptr) {
      Cvor *temp = it->sljedeci;
      staviNaVrh(it->el);
      it = temp;
    }
    br_el = DR.br_el;
    return *this;
  }
  DvostraniRed &operator=(DvostraniRed &&DR) {
    std::swap(pocetak, DR.pocetak);
    std::swap(kraj, DR.kraj);
    std::swap(br_el, DR.br_el);
    return *this;
  }
  int brojElemenata() const { return br_el; }
  void staviNaVrh(const Tip &el) {
    Cvor *novi = new Cvor{el, nullptr, nullptr};
    if (br_el == 0) {
      pocetak = novi;
      kraj = novi;
    } else {
      kraj->sljedeci = novi;
      novi->prethodni = kraj;
      kraj = novi;
    }
    br_el++;
  }
  Tip skiniSaVrha() {
    if (br_el == 0)
      throw std::logic_error("Nema elemenata");
    Tip element = kraj->el;
    if (br_el == 1)
      delete kraj;
    else {
      Cvor *prosli = kraj->prethodni;
      kraj->prethodni = nullptr;
      prosli->sljedeci = nullptr;
      delete kraj;
      kraj = prosli;
    }
    br_el--;
    if (br_el == 0) {
      kraj = nullptr;
      pocetak = nullptr;
    }
    return element;
  }
  void staviNaCelo(const Tip &el) {
    Cvor *novi = new Cvor{el, nullptr, nullptr};
    if (br_el == 0) {
      pocetak = novi;
      kraj = novi;
    } else {
      pocetak->prethodni = novi;
      novi->sljedeci = pocetak;
      pocetak = novi;
    }
    br_el++;
  }
  Tip skiniSaCela() {
    if (br_el == 0)
      throw std::logic_error("Nema elemenata");
    Tip element = pocetak->el;
    if (br_el == 1) {
      delete pocetak;
    } else {
      Cvor *temp = pocetak->sljedeci;
      pocetak->sljedeci = nullptr;
      temp->prethodni = nullptr;
      delete pocetak;
      pocetak = temp;
    }
    br_el--;
    if (br_el == 0) {
      pocetak = nullptr;
      kraj = nullptr;
    }
    return element;
  }
  Tip vrh() const {
    if (br_el == 0)
      throw std::logic_error("Nema elemenata");
    return kraj->el;
  }
  Tip celo() const {
    if (br_el == 0)
      throw std::logic_error("Nema elemenata");
    return pocetak->el;
  }
};

int main() {
  DvostraniRed<int> red;
  red.staviNaCelo(12);
  red.staviNaCelo(-1);
  red.skiniSaCela();
  red.staviNaCelo(89);
  std::cout<<red.brojElemenata()<<std::endl;
  red.staviNaVrh(2);
  red.brisi();
  red.staviNaCelo(2);
  std::cout<<red.brojElemenata()<<std::endl<<" "<<red.vrh();
  red.skiniSaVrha();
  return 0;
}