#include <iostream>

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
class BinStabloMapa : public Mapa<Tip1, Tip2> {
private:
  struct Cvor {
    Tip1 kljuc;
    Tip2 vrijednost;
    Cvor *desno_podstablo = nullptr;
    Cvor *lijevo_podstablo = nullptr;
    Cvor *roditelj = nullptr;
    Cvor(){
        kljuc=Tip1();
        vrijednost=Tip2();
    }
    Cvor(Tip1 kljuc, Tip2 vrijednost, Cvor *d, Cvor *l, Cvor *r)
    {
        this->kljuc=kljuc; this->vrijednost=vrijednost;
        desno_podstablo=d;
        lijevo_podstablo=l;
        roditelj=r;
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
  /*Tip2 pronadjiel(Tip1 kljuc, Cvor *cvor) const
  {
      static Tip1 trazeni=kljuc;
      static Cvor *temp=nullptr;
      Tip2 vrijednost1={};
      Tip2 vrijednost2={};
      if(cvor==nullptr) return Tip2();
      if(cvor->kljuc==trazeni) return cvor->vrijednost;
      vrijednost1=pronadjiel(trazeni,cvor->desno_podstablo);
      vrijednost2=pronadjiel(trazeni,cvor->lijevo_podstablo);
      if(vrijednost1!=Tip2())
      return vrijednost1;
      if(vrijednost2!=Tip2())
      return vrijednost2;
      return Tip2();
  }*/
  Cvor *pronadjicvor(Tip1 kljuc, Cvor *cvor) const {
    if (cvor == nullptr || cvor->kljuc==kljuc)
      return cvor;
    if (kljuc > cvor->kljuc) {
      return pronadjicvor(kljuc, cvor->desno_podstablo);
    } else if (kljuc < cvor->kljuc) {
      return pronadjicvor(kljuc, cvor->lijevo_podstablo);
    }
    return cvor;
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
  void obrisi(const Tip1 &kljuc) override { return; }
};

int main() {
  BinStabloMapa<string, string> mapa;
  mapa["Amina"] = "Kazazovic";
  mapa["Sedin"] = "Kaz";
  std::cout << mapa["Sedin"];
  return 0;
}