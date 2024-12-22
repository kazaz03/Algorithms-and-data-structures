#include <iostream>

template <typename Tip> class Lista {
private:
  void operator=(const Lista &); // Zaštita za dodjelu
  Lista(const Lista &);

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
    velicina = 0;
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

/*
Napisati kod funkcije čiji je prototip
bool palindrom(const Lista<char>& rijec)
koja vraća true ako riječ sadržana u Listi tipa char predstavlja palindrom
(jednaka je s početka i kraja), a false ako ne predstavlja. Primjer palindroma:
RATAR
ANAVOLIMILOVANA
Pretpostaviti da string sadrži isključivo velika slova engleske abecede.
Napomena: Za ovaj zadatak je potrebno koristiti klase Lista
*/

bool palindrom(const Lista<char> &rijec) {
  if (rijec.brojElemenata() == 0)
    return false;
  int velicina = rijec.brojElemenata();
  int brojac = 0;
  int a = 1;
  while (brojac < velicina / 2) {
    if (rijec[brojac] != rijec[velicina - a])
      return false;
    brojac++;
    a++;
  }
  return true;
}

int main() {
  NizLista<char> recenica;
  recenica.dodajIspred('R');
  recenica.sljedeci();
  recenica.dodajIza('A');
  recenica.sljedeci();
  recenica.dodajIza('T');
  recenica.sljedeci();
  recenica.dodajIza('A');
  recenica.sljedeci();
  recenica.dodajIza('R');
  bool a = palindrom(recenica);
  if (!a)
    std::cout << "Rijec nije palindrom";
  else
    std::cout << "Rijec je palindrom";
  return 0;
}