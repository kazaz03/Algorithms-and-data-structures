#include <iostream>

using namespace std;

template <typename Tip> class JednostrukaLista;
template <typename Tip> class Lista;

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
  friend void zadatak3(Red<Tip1> &red, JednostrukaLista<Tip1> &lista, int n);
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

template <typename Tip> class Lista {
public:
  Lista(){};
  virtual ~Lista() {}
  virtual int brojElemenata() const = 0;
  virtual const Tip &trenutni() const = 0;
  virtual Tip &trenutni() = 0;
  virtual bool prethodni() = 0;
  virtual bool sljedeci() = 0;
  virtual void pocetak() = 0;
  virtual void kraj() = 0;
  virtual void obrisi() = 0;
  virtual void dodajIspred(const Tip &el) = 0;
  virtual void dodajIza(const Tip &el) = 0;
  virtual Tip &operator[](int indeks) = 0;
  virtual Tip operator[](int indeks) const = 0;
};

template <typename Tip> class JednostrukaLista : public Lista<Tip> {
  struct Cvor {
    Tip el;
    Cvor *sljedeci;
  };                   // svaki element u listi je cvor
  Cvor *prvi;          // na prvi cvor u listi
  Cvor *trenutni_cvor; // na trenutni u listi
public:
  ~JednostrukaLista() {
    if (brojElemenata() == 0)
      return;
    Cvor *it1 = prvi, *it2 = prvi;
    while (it2 != nullptr) {
      it2 = it1->sljedeci;
      it1->sljedeci = nullptr;
      delete it1;
      it1 = it2;
    }
    trenutni_cvor = nullptr;
    prvi = nullptr;
  }
  JednostrukaLista &operator=(const JednostrukaLista &L);
  JednostrukaLista &operator=(JednostrukaLista &&L);
  JednostrukaLista(const JednostrukaLista &L);
  JednostrukaLista(JednostrukaLista &&L);
  JednostrukaLista() : prvi(nullptr), trenutni_cvor(nullptr){};
  int brojElemenata() const override;
  const Tip &trenutni() const override {
    if (brojElemenata() == 0)
      throw std::logic_error("Lista nema elemenata");
    return trenutni_cvor->el;
  }
  Tip &trenutni() override {
    if (brojElemenata() == 0)
      throw std::logic_error("Lista nema elemenata");
    return trenutni_cvor->el;
  }
  bool prethodni() override;
  bool sljedeci() override {
    if (brojElemenata() == 0)
      throw std::logic_error("Lista nema elemenata");
    if (trenutni_cvor->sljedeci == nullptr)
      return false;
    if (brojElemenata() != 1) {
      trenutni_cvor = trenutni_cvor->sljedeci;
    }
    return true;
  }
  void pocetak() override {
    if (brojElemenata() == 0)
      throw std::logic_error("Lista nema elemenata");
    trenutni_cvor = prvi;
  }
  void kraj() override {
    if (brojElemenata() == 0)
      throw std::logic_error("Lista nema elemenata");
    auto it = prvi;
    while (it->sljedeci != nullptr) {
      it = it->sljedeci;
    }
    trenutni_cvor = it;
  }
  void obrisi() override;
  void dodajIspred(const Tip &el) override;
  void dodajIza(const Tip &el) override;
  Tip operator[](int indeks) const override {
    if (brojElemenata() == 0)
      throw std::logic_error("Lista nema elemenata");
    if (indeks < 0 || indeks >= brojElemenata())
      throw std::out_of_range("Neispravan indeks.");
    int brojac = 0;
    auto a = prvi;
    for (auto it = prvi; it != nullptr; it = it->sljedeci) {
      brojac++;
      if (brojac - 1 == indeks) {
        a = it;
        break;
      }
    }
    return a->el;
  }
  Tip &operator[](int indeks) override {
    if (brojElemenata() == 0)
      throw std::logic_error("Lista nema elemenata");
    if (indeks < 0 || indeks >= brojElemenata())
      throw std::out_of_range("Neispravan indeks.");
    int brojac = 0;
    auto a = prvi;
    for (auto it = prvi; it != nullptr; it = it->sljedeci) {
      brojac++;
      if (brojac - 1 == indeks) {
        a = it;
        break;
      }
    }
    return a->el;
  }
  template <typename Tip1>
  friend void zadatak3(Red<Tip1> &red, JednostrukaLista<Tip1> &lista, int n);
};

template <typename Tip>
JednostrukaLista<Tip>::JednostrukaLista(const JednostrukaLista<Tip> &L) {
  bool p = false;
  Cvor *a = prvi;
  bool trenutni = false;
  Cvor *pamtiprosli = nullptr;
  for (auto it = L.prvi; it != nullptr; it = it->sljedeci) {
    if (L.trenutni_cvor == it) {
      trenutni = true;
    }
    if (!p) {
      prvi = new Cvor{it->el, nullptr};
      pamtiprosli = prvi;
      if (trenutni)
        trenutni_cvor = prvi;
    } else {
      a = new Cvor{it->el, nullptr};
      pamtiprosli->sljedeci = a;
      pamtiprosli = a;
      if (trenutni)
        trenutni_cvor = a;
    }
    p = true;
  }
}

template <typename Tip>
JednostrukaLista<Tip>::JednostrukaLista(JednostrukaLista<Tip> &&L) {
  prvi = L.prvi;
  trenutni_cvor = L.trenutni_cvor;
  L.prvi = nullptr;
  L.trenutni_cvor = nullptr;
}

template <typename Tip>
JednostrukaLista<Tip> &
JednostrukaLista<Tip>::operator=(const JednostrukaLista<Tip> &L) {
  if (this == &L)
    return *this;
  // prvo obrisat od trenutnog sadrzaj
  Cvor *it1 = prvi, *it2 = prvi;
  while (it2 != nullptr) {
    it2 = it1->sljedeci;
    it1->sljedeci = nullptr;
    delete it1;
    it1 = it2;
  }
  trenutni_cvor = nullptr;
  prvi = nullptr;
  bool p = false;
  Cvor *a = prvi;
  bool trenutni = false;
  Cvor *pamtiprosli = nullptr;
  for (auto it = L.prvi; it != nullptr; it = it->sljedeci) {
    if (L.trenutni_cvor == it) {
      trenutni = true;
    }
    if (!p) {
      prvi = new Cvor{it->el, nullptr};
      pamtiprosli = prvi;
      if (trenutni)
        trenutni_cvor = prvi;
    } else {
      a = new Cvor{it->el, nullptr};
      pamtiprosli->sljedeci = a;
      pamtiprosli = a;
      if (trenutni)
        trenutni_cvor = a;
    }
    p = true;
  }
  return *this;
}

template <typename Tip>
JednostrukaLista<Tip> &
JednostrukaLista<Tip>::operator=(JednostrukaLista<Tip> &&L) {
  std::swap(this->prvi, L->prvi);
  std::swap(this->trenutni_cvor, L->trenutni_cvor);
  return *this;
}

template <typename Tip> void JednostrukaLista<Tip>::dodajIspred(const Tip &el) {
  if (brojElemenata() == 0) {
    Cvor *it = new Cvor{el, nullptr};
    prvi = it;
    trenutni_cvor = it;
  } else if (brojElemenata() == 1 || prvi == trenutni_cvor) {
    Cvor *novi = new Cvor{el, nullptr};
    novi->sljedeci = trenutni_cvor;
    prvi = novi;
  } else {
    Cvor *isprednjega = prvi;
    Cvor *novi = new Cvor{el, nullptr};
    while (isprednjega->sljedeci != trenutni_cvor) {
      isprednjega = isprednjega->sljedeci;
    }
    isprednjega->sljedeci = novi;
    novi->sljedeci = trenutni_cvor;
  }
}

template <typename Tip> void JednostrukaLista<Tip>::dodajIza(const Tip &el) {
  if (brojElemenata() == 0) {
    Cvor *it = new Cvor{el, nullptr};
    prvi = it;
    trenutni_cvor = it;
  } else {
    Cvor *ispred = trenutni_cvor->sljedeci;
    Cvor *it = new Cvor{el, nullptr};
    trenutni_cvor->sljedeci = it;
    it->sljedeci = ispred;
  }
}

template <typename Tip> void JednostrukaLista<Tip>::obrisi() {
  if (brojElemenata() == 1) {
    delete trenutni_cvor;
    trenutni_cvor = nullptr;
    prvi = nullptr;
  } else if (prvi != trenutni_cvor &&
             trenutni_cvor->sljedeci !=
                 nullptr) { // pozicija nije ni poc ni kraj
    Cvor *it = prvi;
    while (it->sljedeci != trenutni_cvor) {
      it = it->sljedeci;
    }
    it->sljedeci = trenutni_cvor->sljedeci;
    Cvor *za_obrisat = trenutni_cvor;
    trenutni_cvor = trenutni_cvor->sljedeci;
    za_obrisat->sljedeci = nullptr;
    delete za_obrisat;
    za_obrisat = nullptr;
  } else if (trenutni_cvor->sljedeci == nullptr) // pozicija je kraj
  {
    Cvor *it = prvi;
    while (it->sljedeci != trenutni_cvor) {
      it = it->sljedeci;
    }
    it->sljedeci = nullptr;
    delete trenutni_cvor;
    trenutni_cvor = it;
  } else {
    Cvor *it = trenutni_cvor->sljedeci; // pozicija je pocetak
    trenutni_cvor->sljedeci = nullptr;
    delete trenutni_cvor;
    prvi = it;
    trenutni_cvor = it;
  }
}

template <typename Tip> bool JednostrukaLista<Tip>::prethodni() {
  if (brojElemenata() == 0)
    throw std::logic_error("Lista nema elemenata");
  if (trenutni_cvor == prvi)
    return false;
  auto it = prvi;
  while (it->sljedeci != trenutni_cvor) {
    it = it->sljedeci;
  }
  trenutni_cvor = it;
  if (trenutni_cvor == nullptr)
    return false;
  return true;
}

template <typename Tip> int JednostrukaLista<Tip>::brojElemenata() const {
  int brojac = 0;
  for (auto it = prvi; it != nullptr; it = it->sljedeci) {
    brojac++;
  }
  return brojac;
}

/*
Napisati funkciju koja prima red, listu i varijablu n tipa int i modifikuje red
tako da elementi iz liste i reda budu naizmjenično poredani gdje je svaki
isprepleteni dio reda i liste veličine n a počinje se sa elementima reda. U
slučaju razlike u dužini reda i liste, višak elemenata se postavlja na kraj
reda. Primjer 1: Neka je dat red sa elementima {1, 2, 3, 4} gdje je element 1 na
čelu reda i lista sa elementima {1, 2, 3, 4} i neka je n=1. Rezultantni red je
{1, 1, 2, 2, 3, 3, 4, 4}. Primjer 2: Neka je dat red sa elementima {A, A, A, A,
A, A, A} gdje je element A na čelu reda i lista sa elementima {B, B, B, B} i
neka je n=2. Rezultantni red je {A, A, B, B, A, A, B, B, A, A, A}. Napomena:
Zadatak riješiti koristeći samo ulazne parametre, nije dozvoljeno korištenje
pomoćnih kolekcija. Također, nije dozvoljena ni modifikacija liste.
*/

template <typename Tip1>
void zadatak3(Red<Tip1> &red, JednostrukaLista<Tip1> &lista, int n) {
  int duzinareda = red.brojElemenata();
  int duzinaliste = lista.brojElemenata();
  auto it_red = red.pocetak;
  auto it_lista = lista.prvi;
  int brojac = 0;
  int k = 0;
  int h = duzinareda;
  if (duzinareda > duzinaliste)
    h = duzinareda;
  else
    h = duzinaliste;
  while (brojac < h) {
    int brojac2 = 0;
    bool izadji = false;
    while (brojac2 < n) {
      if (brojac2 == duzinareda)
        break;
      if (duzinareda > duzinaliste && k == h) {
        izadji = true;
        break;
      }
      it_red = red.pocetak;
      red.stavi(it_red->el);
      red.skini();
      brojac2++;
      k++;
    }
    brojac2 = 0;
    while (brojac2 < n) {
      if (it_lista == nullptr)
        break;
      red.stavi(it_lista->el);
      it_lista = it_lista->sljedeci;
      brojac2++;
    }
    if (izadji)
      break;
    brojac = n + brojac;
  }
  red.br_el = duzinareda + duzinaliste;
}

int main() {
  Red<int> red;
  red.stavi(1);
  red.stavi(2);
  red.stavi(3);
  red.stavi(4);
  red.stavi(5);
  red.stavi(6);
  JednostrukaLista<int> lista;
  lista.dodajIspred(1);
  lista.dodajIza(4);
  lista.dodajIza(3);
  lista.dodajIza(2);
  zadatak3(red, lista, 3);
  int b = red.brojElemenata();
  int a = 0;
  while (a < b) {
    std::cout << red.celo() << " ";
    red.skini();
    a++;
  }
  return 0;
}