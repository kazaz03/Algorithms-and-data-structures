#include <iostream>
#include <list>
#include <stdexcept>
#include <string.h>

using namespace std;

template <typename Tip1, typename Tip2> class Iterator;

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
private:
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
  friend class Iterator<Tip1, Tip2>;
};

template <typename Tip1, typename Tip2> class Iterator {
private:
  const NizMapa<Tip1, Tip2> *niz;
  std::pair<Tip1, Tip2> **el;
  int indeks;

public:
  Iterator(const NizMapa<Tip1, Tip2> &mapa) {
    niz = &mapa;
    el = mapa.niz;
    indeks = 0;
  }
  Tip2 trenutni() {
    if (niz->brojElemenata() == 0)
      throw std::range_error("Nema elemenata");
    return el[indeks]->second;
  }
  Tip1 kljuc() {
    if (niz->brojElemenata() == 0)
      throw std::range_error("Nema elemenata");
    return el[indeks]->first;
  }
  bool prethodni() {
    if (indeks == 0)
      return false;
    indeks--;
    return true;
  }
  bool sljedeci() {
    if (indeks == niz->brojElemenata() - 1)
      return false;
    indeks++;
    return true;
  }
  void pocetak() { indeks = 0; }
  void kraj() { indeks = niz->brojElemenata() - 1; }
};

int main() {
  NizMapa<string, string> i;
  NizMapa<string, string> i1;
  i["Amina"] = "19364";
  i["Sedin"] = "12903";
  i1 = i;
  i["Amina"] = "19365";
  std::cout << "Broj elemenata: " << i.brojElemenata() << std::endl;
  Iterator<string, string> it(i);
  do 
  {
      std::cout<<it.trenutni()<<" i "<<it.kljuc()<<std::endl;
  }while(it.sljedeci());
  return 0;
}