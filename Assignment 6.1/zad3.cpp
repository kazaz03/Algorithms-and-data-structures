#include <iostream>
#include <time.h>
#include <utility>
#include <vector>

using namespace std;

template <typename Tip1, typename Tip2> class Mapa {
public:
  Mapa<Tip1, Tip2>(){};
  virtual ~Mapa<Tip1, Tip2>(){};
  virtual int brojElemenata() const = 0;
  virtual void obrisi() = 0;
  virtual void obrisi(const Tip1 &kljuc) = 0;
  virtual Tip2 &operator[](Tip1 k) = 0;
  virtual Tip2 operator[](Tip1 k) const = 0;
};

template <typename Tip1, typename Tip2>
class NizMapa : public Mapa<Tip1, Tip2> {
public:
  int kapacitet;
  int br_el;
  std::pair<Tip1, Tip2> **niz;

public:
  NizMapa<Tip1, Tip2>()
      : Mapa<Tip1, Tip2>(), kapacitet(10000), br_el(0),
        niz(new std::pair<Tip1, Tip2> *[kapacitet] {}){};
  ~NizMapa<Tip1, Tip2>() {
    for (int i = 0; i < br_el; i++)
      delete niz[i];
    delete[] niz;
    br_el = 0;
    kapacitet = 10000;
  }
  NizMapa<Tip1, Tip2>(const NizMapa<Tip1, Tip2> &N) {
    niz = new std::pair<Tip1, Tip2> *[N.kapacitet] {};
    for (int i = 0; i < N.br_el; i++) {
      niz[i] = new std::pair<Tip1, Tip2>(*N.niz[i]);
    }
    br_el = N.br_el;
    kapacitet = N.kapacitet;
    if (N.br_el == 0)
      niz = nullptr;
  }
  NizMapa<Tip1, Tip2>(NizMapa<Tip1, Tip2> &&N) {
    kapacitet = N.kapacitet;
    br_el = N.br_el;
    niz = N.niz;
  }
  NizMapa<Tip1, Tip2> &operator=(const NizMapa<Tip1, Tip2> &N) {
    if (this == &N)
      return *this;
    for (int i = 0; i < br_el; i++)
      delete niz[i];
    delete[] niz;
    niz = new std::pair<Tip1, Tip2> *[N.kapacitet] {};
    for (int i = 0; i < N.br_el; i++) {
      niz[i] = new std::pair<Tip1, Tip2>(*N.niz[i]);
    }
    br_el = N.br_el;
    kapacitet = N.kapacitet;
    return *this;
  }
  NizMapa<Tip1, Tip2> &operator=(NizMapa<Tip1, Tip2> &&N) {
    if (this == &N)
      return *this;
    std::swap(kapacitet, N.kapacitet);
    std::swap(br_el, N.br_el);
    std::swap(niz, N.niz);
    return *this;
  }
  int brojElemenata() const override { return br_el; }
  int Kapacitet() const { return kapacitet; }
  void obrisi() override {
    if (!br_el)
      throw std::range_error("Nema elemenata");
    for (int i = 0; i < br_el; i++) {
      delete niz[i];
    }
    br_el = 0;
  }
  void obrisi(const Tip1 &kljuc) override {
    if (!br_el)
      throw std::range_error("Nema elemenata");
    int indeks = -1;
    for (int i = 0; i < br_el; i++) {
      if (niz[i]->first == kljuc) {
        indeks = i;
        delete niz[i];
        break;
      }
    }
    if (indeks == -1)
      throw std::range_error("Nema kljuca");
    for (int i = indeks; i < br_el - 1; i++) {
      niz[i] = niz[i + 1];
    }
    niz[br_el - 1] = nullptr;
    br_el--;
  }
  Tip2 &operator[](Tip1 k) override {
    if (br_el == kapacitet) {
      std::pair<Tip1, Tip2> **temp =
          new std::pair<Tip1, Tip2> *[10000 + kapacitet] {};
      for (int i = 0; i < br_el; i++) {
        temp[i] = new std::pair<Tip1, Tip2>(*niz[i]);
        delete niz[i];
      }
      delete[] niz;
      kapacitet = kapacitet + 10000;
      niz = temp;
      temp = nullptr;
    }
    for (int i = 0; i < br_el; i++) {
      if (niz[i]->first == k)
        return niz[i]->second;
    }
    niz[br_el] = new std::pair<Tip1, Tip2>(k, Tip2());
    br_el++;
    return niz[br_el - 1]->second;
  }
  Tip2 operator[](Tip1 k) const override {
    for (int i = 0; i < br_el; i++) {
      if (niz[i]->first == k)
        return niz[i]->second;
    }
    return Tip2();
  }
};

template <typename Tip1, typename Tip2>
class BinStabloMapa : public Mapa<Tip1, Tip2> {
private:
  struct Cvor {
    Tip1 kljuc;
    Tip2 vrijednost;
    Cvor *desno_podstablo = nullptr;
    Cvor *lijevo_podstablo = nullptr;
    Cvor *roditelj = nullptr;
    Cvor(Tip1 kljuc, Tip2 vrijednost, Cvor *d, Cvor *l, Cvor *r) {
      this->kljuc = kljuc;
      this->vrijednost = vrijednost;
      desno_podstablo = d;
      lijevo_podstablo = l;
      roditelj = r;
    }
  };
  Cvor *stablo;
  int br_el;
  void brisanje(Cvor *cvor) {
    if (cvor == nullptr)
      return;
    brisanje(cvor->desno_podstablo);
    brisanje(cvor->lijevo_podstablo);
    delete cvor;
  }
  Cvor *pronadjicvor(Tip1 kljuc, Cvor *cvor) const {
    if (cvor == nullptr || cvor->kljuc == kljuc)
      return cvor;
    if (kljuc > cvor->kljuc)
      return pronadjicvor(kljuc, cvor->desno_podstablo);
    return pronadjicvor(kljuc, cvor->lijevo_podstablo);
  }
  Cvor *dodaj(Tip1 kljuc, Cvor *cvor, Cvor *roditelj = nullptr) {
    if (cvor == nullptr) {
      cvor = new Cvor(kljuc, Tip2(), nullptr, nullptr, roditelj);
      return cvor;
    }
    if (kljuc > cvor->kljuc) {
      cvor->desno_podstablo = dodaj(kljuc, cvor->desno_podstablo, cvor);
    } else if (kljuc < cvor->kljuc) {
      cvor->lijevo_podstablo = dodaj(kljuc, cvor->lijevo_podstablo, cvor);
    }
    return cvor;
  }

public:
  ~BinStabloMapa<Tip1, Tip2>() {
    brisanje(stablo);
    br_el = 0;
    stablo = nullptr;
  }
  BinStabloMapa<Tip1, Tip2>() {
    stablo = nullptr;
    br_el = 0;
  }
  int brojElemenata() const override { return br_el; }
  void obrisi() override {
    brisanje(stablo);
    br_el = 0;
    stablo = nullptr;
  }
  Tip2 operator[](Tip1 k) const override {
    Cvor *temp = pronadjicvor(k, stablo);
    if (temp != nullptr)
      return temp->vrijednost;
    return Tip2();
  }
  Tip2 &operator[](Tip1 k) override {
    if (br_el == 0) {
      stablo = new Cvor(k, Tip2(), nullptr, nullptr, nullptr);
      br_el++;
      return stablo->vrijednost;
    }
    Cvor *temp = pronadjicvor(k, stablo);
    if (temp == nullptr) {
      dodaj(k, stablo);
      br_el++;
      temp = pronadjicvor(k, stablo);
    }
    return temp->vrijednost;
  }
  BinStabloMapa<Tip1, Tip2>(const BinStabloMapa<Tip1, Tip2> &nova) {
    br_el = 0;
    stablo = nullptr;
    kopirajStablo(stablo, nova.stablo, nullptr);
  } //
  void kopirajStablo(Cvor *&c1, Cvor *c2, Cvor *roditelj) {
    if (c2 == nullptr)
      return;
    c1 = new Cvor(c2->kljuc, c2->vrijednost, nullptr, nullptr, roditelj);
    kopirajStablo(c1->lijevo_podstablo, c2->lijevo_podstablo, c1);
    kopirajStablo(c1->desno_podstablo, c2->desno_podstablo, c1);
    br_el++;
  }
  BinStabloMapa<Tip1, Tip2> &operator=(const BinStabloMapa<Tip1, Tip2> &nova) {
    if (this == &nova)
      return *this;
    brisanje(stablo);
    br_el = 0;
    stablo = nullptr;
    kopirajStablo(stablo, nova.stablo, nullptr);
    return *this;
  }
  void obrisi(const Tip1 &kljuc) override {
    Cvor *temp = pronadjicvor(kljuc, stablo);
    if (temp == nullptr) {
      throw std::logic_error("Kljuc ne postoji");
    }
    // ako je cvor bez djece
    if (temp->desno_podstablo == nullptr && temp->lijevo_podstablo == nullptr) {
      if (temp == stablo)
        stablo = nullptr;
      else if (temp == temp->roditelj->desno_podstablo) {
        temp->roditelj->desno_podstablo = nullptr;
      } else
        temp->roditelj->lijevo_podstablo = nullptr;
      delete temp;
    }
    // ako je cvor s oba djeteta
    else if (temp->lijevo_podstablo != nullptr &&
             temp->desno_podstablo != nullptr) {
      Cvor *pom = temp->lijevo_podstablo;
      while (pom->desno_podstablo != nullptr)
        pom = pom->desno_podstablo;
      pom->desno_podstablo = temp->desno_podstablo;
      if (temp->desno_podstablo != nullptr)
        temp->desno_podstablo->roditelj = pom;
      pom->roditelj->desno_podstablo = pom->lijevo_podstablo;
      if (pom->lijevo_podstablo != nullptr)
        pom->lijevo_podstablo->roditelj = pom->roditelj;
      if (temp == stablo)
        stablo = pom;
      else if (temp == temp->roditelj->lijevo_podstablo) {
        temp->roditelj->lijevo_podstablo = pom;
        pom->roditelj = temp->roditelj;
      } else {
        temp->roditelj->desno_podstablo = pom;
        pom->roditelj = temp->roditelj;
      }
      if (temp == stablo)
        pom->lijevo_podstablo = nullptr;
      else {
        pom->lijevo_podstablo = temp->lijevo_podstablo;
        temp->lijevo_podstablo->roditelj = pom;
      }
      delete temp;
    } // ako je jedno dijete
    else {
      Cvor *dijete = temp->lijevo_podstablo;
      if (dijete == nullptr)
        dijete = temp->desno_podstablo;
      if (temp == stablo)
        stablo = dijete;
      else if (temp == temp->roditelj->lijevo_podstablo)
        temp->roditelj->lijevo_podstablo = dijete;
      else
        temp->roditelj->desno_podstablo = dijete;
      dijete->roditelj = temp->roditelj;
      delete temp;
    }
    br_el--;
  }
  void popuni_vektor(vector<Tip2> &v, Cvor *cvor, const Tip1 &a,
                     const Tip1 &b) {
    if (cvor == nullptr)
      return;
    popuni_vektor(v, cvor->desno_podstablo, a, b);
    popuni_vektor(v, cvor->lijevo_podstablo, a, b);
    if (cvor->kljuc > a && cvor->kljuc < b)
      v.push_back(cvor->vrijednost);
  }
  vector<Tip2> od_do(const Tip1 &a, const Tip1 &b) {
    std::vector<Tip2> vektor;
    popuni_vektor(vektor, stablo, a, b);
    return vektor;
  }
};

int main() {
  BinStabloMapa<int, string> mapa1;
  BinStabloMapa<int, string> mapa2;
  mapa1[2] = "Amina";
  mapa1[1] = "Sedin";
  mapa1[4] = "Sabina";
  mapa1[8] = "Amra";
  mapa1[6] = "Kanita";
  mapa1[10] = "Dexter";
  std::vector<string> v = mapa1.od_do(3, 11);
  for (int i = 0; i < v.size(); i++) {
    std::cout << v.at(i) << " ";
  }
  return 0;
}