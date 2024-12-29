#include <iostream>
#include <vector>

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
  };          // svaki element u listi je cvor
  Cvor *prvi; // na prvi cvor u listi
  Cvor *trenutni_cvor;
  Cvor *dajcvor() { return trenutni_cvor; } // na trenutni u listi
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
  void ispisUnazad();
  void rekurzija(Cvor *cvor);
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

template <typename Tip> void JednostrukaLista<Tip>::rekurzija(Cvor *cvor) {
  if (cvor->sljedeci == nullptr)
    {std::cout << cvor->el;
     return;}
  Tip broj = cvor->el;
  rekurzija(cvor->sljedeci);
  std::cout << broj << " ";
  return;
}

template <typename Tip> void JednostrukaLista<Tip>::ispisUnazad() {
  rekurzija(prvi);
  std::cout << std::endl;
}

/*
Napisati funkciju
int binarna_pretraga(Tip* niz, int n, Tip element)
koja vraća indeks na kojem se element nalazi u nizu, odnosno -1 
ako se element ne nalazi u nizu. Smatrati da je niz sortiran u 
rastućem redoslijedu i da je za Tip definiran operator<.
Očekivana vremenska složenost algoritma je O(logn).
*/

template<typename Tip>
int binarna_pretraga(Tip *niz, int n, Tip element)
{
    int dno=0; 
    int kraj=n-1; 
    int srednji=(kraj-dno)/2;
    while(dno<=kraj)
    {
        if(niz[srednji]<element)
        {
            dno=srednji+1;
        }else if(niz[srednji]>element)
        {
            kraj=srednji-1;
        }
        srednji=(kraj-dno)/2+dno;
        if(niz[srednji]==element) return srednji;
    }
    return -1;
}

double pow(double a, unsigned int n)
{
    if(n==1) return a;
    double stepen=a*pow(a,n-1);
    return stepen;
}

int main() {
  JednostrukaLista<int> lista;
  lista.dodajIza(1);
  lista.sljedeci();
  lista.dodajIza(2);
  lista.sljedeci();
  lista.dodajIza(3);
  lista.sljedeci();
  lista.dodajIza(4);
  lista.pocetak();
  int brojac = 0;
  //lista.ispisUnazad();
  int niz[]={1,4,7,8,12,70,90};
  int n=8; 
  int pozicija=binarna_pretraga(niz,n,4);
  std::cout<<pozicija;
  double k=pow(2,5);
  std::cout<<" "<<k;
  return 0;
}