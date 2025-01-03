#include <iostream>

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

template <typename Tip> class NizLista : public Lista<Tip> {
  Tip **niz = nullptr;
  int velicina;
  int trenutni_indeks;
  int kapacitet;
  void PovecajNiz() {
    if (velicina == 0)
      return;
    Tip **novi = new Tip *[int(kapacitet * 2)] {};
    for (int i = 0; i < velicina; i++)
      novi[i] = new Tip(*niz[i]);
    // novi[velicina] = new Tip{};
    for (int i = 0; i < velicina; i++) {
      delete niz[i];
    }
    delete[] niz;
    niz = novi;
    novi = nullptr;
    kapacitet = kapacitet * 2;
  }

public:
  NizLista() : trenutni_indeks(-1), velicina(0), kapacitet(500) {
    niz = new Tip *[kapacitet] {};
  };
  int brojElemenata() const override { return velicina; }
  const Tip &trenutni() const override {
    if (velicina == 0)
      throw std::logic_error("Lista nema elemenata");
    return *niz[trenutni_indeks];
  }
  Tip &trenutni() override {
    if (velicina == 0)
      throw std::logic_error("Lista nema elemenata");
    return *niz[trenutni_indeks];
  }
  bool prethodni() override {
    if (velicina == 0)
      throw std::logic_error("Lista nema elemenata");
    if (trenutni_indeks - 1 < 0)
      return false;
    trenutni_indeks--;
    return true;
  }
  bool sljedeci() override {
    if (velicina == 0)
      throw std::logic_error("Lista nema elemenata");
    if (trenutni_indeks + 1 == velicina)
      return false;
    trenutni_indeks++;
    return true;
  }
  void pocetak() override {
    if (velicina == 0)
      throw std::logic_error("Lista nema elemenata");
    trenutni_indeks = 0;
  }
  void kraj() override {
    if (velicina == 0)
      throw std::logic_error("Lista nema elemenata");
    trenutni_indeks = velicina - 1;
  }
  void obrisi() override;
  void dodajIspred(const Tip &el) override;
  void dodajIza(const Tip &el) override;
  Tip operator[](int indeks) const override;
  Tip &operator[](int indeks) override;
  NizLista &operator=(const NizLista &N);
  NizLista &operator=(NizLista &&N);
  NizLista(const NizLista &N);
  NizLista(NizLista &&N);
  ~NizLista() {
    for (int i = 0; i < kapacitet; i++) {
      delete niz[i];
      niz[i] = nullptr;
    }
    delete[] niz;
    niz = nullptr;
    velicina=0;
  }
};

template <typename Tip> Tip &NizLista<Tip>::operator[](int indeks) {
  if (indeks < 0 || indeks >= velicina)
    throw std::out_of_range("Neispravan indeks.");
  return *niz[indeks];
}

template <typename Tip> Tip NizLista<Tip>::operator[](int indeks) const {
  if (indeks < 0 || indeks >= velicina)
    throw std::out_of_range("Neispravan indeks.");
  return *niz[indeks];
}

template <typename Tip> NizLista<Tip>::NizLista(const NizLista<Tip> &N) {
  if (N.velicina == 0)
    return;
  niz = new Tip *[N.kapacitet] {};
  for (int i = 0; i < N.velicina; i++) {
    niz[i] = new Tip(*N.niz[i]);
  }
  velicina = N.velicina;
  trenutni_indeks = N.trenutni_indeks;
  kapacitet = N.kapacitet;
}

template <typename Tip> NizLista<Tip>::NizLista(NizLista<Tip> &&N) {
  if (N.velicina == 0)
    return;
  niz = N.niz;
  velicina = N.velicina;
  trenutni_indeks = N.trenutni_indeks;
  kapacitet = N.kapacitet;
}

template <typename Tip>
NizLista<Tip> &NizLista<Tip>::operator=(NizLista<Tip> &&N) {
  if (this == &N)
    return *this;
  std::swap(velicina, N.velicina);
  std::swap(trenutni_indeks, N.trenutni_indeks);
  std::swap(niz, N.niz);
  std::swap(kapacitet, N.kapacitet);
  return *this;
}

template <typename Tip>
NizLista<Tip> &NizLista<Tip>::operator=(const NizLista<Tip> &N) {
  if (this == &N)
    return *this;
  for (int i = 0; i < velicina; i++)
    delete niz[i];
  delete[] niz;
  niz = nullptr;
  niz = new Tip *[N.kapacitet] {};
  for (int i = 0; i < N.velicina; i++)
    niz[i] = new Tip(*N.niz[i]);
  velicina = N.velicina;
  trenutni_indeks = N.trenutni_indeks;
  kapacitet = N.kapacitet;
  return *this;
}

template <typename Tip> void NizLista<Tip>::dodajIspred(const Tip &el) {
  if (velicina == 0) {
    trenutni_indeks++;
    niz[trenutni_indeks] = new Tip(el);
    velicina++;
  } else {
    if (velicina + 1 > kapacitet) {
      PovecajNiz();
    }
    niz[velicina] = new Tip{};
    for (int i = velicina; i > trenutni_indeks; i--) {
      Tip *p = niz[i];
      niz[i] = niz[i - 1];
      niz[i - 1] = p;
    }
    *niz[trenutni_indeks] = el;
    trenutni_indeks++;
    velicina++;
  }
}

template <typename Tip> void NizLista<Tip>::dodajIza(const Tip &el) {
  if (velicina == 0) {
    trenutni_indeks++;
    niz[trenutni_indeks] = new Tip(el);
    velicina++;
  } else {
    if (velicina + 1 > kapacitet) {
      PovecajNiz();
    }
    niz[velicina] = new Tip{};
    for (int i = velicina; i > trenutni_indeks + 1; i--) {
      Tip *p = niz[i];
      niz[i] = niz[i - 1];
      niz[i - 1] = p;
    }
    *niz[trenutni_indeks + 1] = el;
    velicina++;
  }
}

template <typename Tip> void NizLista<Tip>::obrisi() {
  if (velicina == 0)
    throw std::logic_error("Lista nema elemenata");
  for (int i = trenutni_indeks; i < velicina - 1; i++) {
    // Tip *a = niz[i];
    *niz[i] = *niz[i + 1];
    // niz[i + 1] = a;
  }
  delete niz[velicina - 1];
  niz[velicina - 1] = nullptr;
  velicina--;
  if (velicina == 0) {
    trenutni_indeks = -1;
  }
}

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

int main() {
  NizLista<int> niz;
  niz.dodajIspred(34);
  niz.dodajIza(23);
  niz.sljedeci();
  niz.dodajIza(45);
  niz.prethodni();
  niz.dodajIspred(0);
  niz.dodajIza(-3);
  niz.sljedeci();
  niz.sljedeci();
  niz.dodajIspred(222);
  niz.obrisi();
  for(int i=0; i<niz.brojElemenata(); i++) std::cout<<niz[i]<<" ";
  std::cout<<std::endl;
  niz.pocetak();
  NizLista<int> niz2=niz;
  niz2.dodajIspred(23);
  for(int i=0; i<niz2.brojElemenata(); i++) std::cout<<niz2[i]<<" ";
  std::cout<<std::endl;
  JednostrukaLista<int> lista;
  lista.dodajIspred(12);
  lista.dodajIza(23);
  lista.obrisi();
  lista.dodajIspred(33);
  lista.sljedeci();
  lista.obrisi();
  lista.dodajIspred(2);
  lista.dodajIspred(0);
  lista.dodajIza(9);
  for(int i=0; i<lista.brojElemenata(); i++)std::cout<<lista[i]<<" ";
  std::cout<<std::endl;
  JednostrukaLista<int> lista2=lista;
  for(int i=0; i<lista2.brojElemenata(); i++) 
  {
      lista2.obrisi();
      lista2.sljedeci();
  }
  JednostrukaLista<int> c2;
  c2.dodajIza(23);
  c2.dodajIza(45);
  c2.dodajIza(-43);
  std::cout<<c2.trenutni();
  return 0;
}