#include <iostream>
#include <stdexcept>

using namespace std;

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

template <typename Tip> class DvostrukaLista : public Lista<Tip> {
  struct Cvor {
    Tip el;
    Cvor *sljedeci;
    Cvor *prethodni;
  };
  Cvor *prvi;
  Cvor *posljednji;
  Cvor *trenutni_cvor;
  int br_el;
  void brisanje() {
    if (!br_el)
      return;
    auto it = prvi;
    while (it != nullptr) {
      Cvor *s = it->sljedeci;
      it->sljedeci = nullptr;
      delete it;
      it = s;
    }
    prvi = nullptr;
    posljednji = nullptr;
    trenutni_cvor = nullptr;
    br_el = 0;
  }

public:
template <typename Tip1>
  friend class Iterator;
  DvostrukaLista()
      : prvi(nullptr), posljednji(nullptr), trenutni_cvor(nullptr), br_el(0){};
  ~DvostrukaLista() { brisanje(); };
  DvostrukaLista(DvostrukaLista &&DL) {
    prvi = DL.prvi;
    posljednji = DL.posljednji;
    trenutni_cvor = DL.trenutni_cvor;
    br_el = DL.br_el;
  }
  DvostrukaLista(const DvostrukaLista &DL) {
    auto it = DL.prvi;
    bool p = true;
    Cvor *prosli = nullptr;
    while (it != nullptr) {
      Cvor *novi = new Cvor{it->el, nullptr, nullptr};
      if (DL.trenutni_cvor == it) {
        trenutni_cvor = novi;
      }
      if (p) {
        prvi = novi;
        prosli = prvi;
      } else {
        prosli->sljedeci = novi;
        novi->prethodni = prosli;
        prosli = novi;
      }
      p = false;
      it = it->sljedeci;
    }
    posljednji = prosli;
    br_el = DL.br_el;
  }
  DvostrukaLista &operator=(const DvostrukaLista &DL) {
    if (this == &DL)
      return *this;
    brisanje();
    auto it = DL.prvi;
    bool p = true;
    Cvor *prosli = nullptr;
    while (it != nullptr) {
      Cvor *novi = new Cvor{it->el, nullptr, nullptr};
      if (DL.trenutni_cvor == it) {
        trenutni_cvor = novi;
      }
      if (p) {
        prvi = novi;
        prosli = prvi;
      } else {
        prosli->sljedeci = novi;
        novi->prethodni = prosli;
        prosli = novi;
      }
      p = false;
      it = it->sljedeci;
    }
    posljednji = prosli;
    br_el = DL.br_el;
    return *this;
  }
  DvostrukaLista &operator=(DvostrukaLista &&DL) {
    if (this == &DL)
      return *this;
    std::swap(prvi, DL.prvi);
    std::swap(posljednji, DL.posljednji);
    std::swap(br_el, DL.br_el);
    return *this;
  }
  int brojElemenata() const override { return br_el; }
  Tip &trenutni() override {
    if (br_el == 0)
      throw std::logic_error("Lista nema elemenata");
    return trenutni_cvor->el;
  }
  const Tip &trenutni() const override {
    if (br_el == 0)
      throw std::logic_error("Lista nema elemenata");
    return trenutni_cvor->el;
  }
  bool prethodni() override {
    if (br_el == 0)
      throw std::logic_error("Lista nema elemenata");
    if (trenutni_cvor == prvi)
      return false;
    Cvor *p = trenutni_cvor->prethodni;
    trenutni_cvor = p;
    return true;
  }
  bool sljedeci() override {
    if (br_el == 0)
      throw std::logic_error("Lista nema elemenata");
    if (trenutni_cvor == posljednji)
      return false;
    Cvor *p = trenutni_cvor->sljedeci;
    trenutni_cvor = p;
    return true;
  }
  void pocetak() override {
    if (br_el == 0)
      throw std::logic_error("Lista nema elemenata");
    trenutni_cvor = prvi;
  }
  void kraj() override {
    if (br_el == 0)
      throw std::logic_error("Lista nema elemenata");
    trenutni_cvor = posljednji;
  }
  void obrisi() override {
    if (br_el == 0)
      throw std::logic_error("Lista nema elemenata");
    if (br_el == 1) {
      delete trenutni_cvor;
      trenutni_cvor = nullptr;
      posljednji = nullptr;
      prvi = nullptr;
    } else if (trenutni_cvor == posljednji) {
      Cvor *prosli = trenutni_cvor->prethodni;
      trenutni_cvor->prethodni = nullptr;
      delete trenutni_cvor;
      prosli->sljedeci = nullptr;
      posljednji = prosli;
      trenutni_cvor = prosli;
    } else if (trenutni_cvor == prvi) {
      Cvor *slj = trenutni_cvor->sljedeci;
      slj->prethodni = nullptr;
      trenutni_cvor->sljedeci = nullptr;
      delete trenutni_cvor;
      prvi = slj;
      trenutni_cvor = slj;
    } else {
      Cvor *slj = trenutni_cvor->sljedeci;
      Cvor *prosli = trenutni_cvor->prethodni;
      trenutni_cvor->sljedeci = nullptr;
      trenutni_cvor->prethodni = nullptr;
      delete trenutni_cvor;
      prosli->sljedeci = slj;
      slj->prethodni = prosli;
      trenutni_cvor = slj;
    }
    br_el--;
  }
  void dodajIspred(const Tip &el) override {
    Cvor *novi = new Cvor{el, nullptr, nullptr};
    if (br_el == 0) {
      prvi = novi;
      posljednji = novi;
      trenutni_cvor = novi;
    } else {
      if (trenutni_cvor == prvi) {
        novi->sljedeci = trenutni_cvor;
        trenutni_cvor->prethodni = novi;
        prvi = novi;
      } else {
        Cvor *prije = trenutni_cvor->prethodni;
        trenutni_cvor->prethodni = novi;
        novi->sljedeci = trenutni_cvor;
        prije->sljedeci = novi;
        novi->prethodni = prije;
      }
    }
    br_el++;
  }
  void dodajIza(const Tip &el) override {
    Cvor *novi = new Cvor{el, nullptr, nullptr};
    if (br_el == 0) {
      prvi = novi;
      posljednji = novi;
      trenutni_cvor = novi;
    } else {
      if (trenutni_cvor == posljednji) {
        trenutni_cvor->sljedeci = novi;
        novi->prethodni = trenutni_cvor;
        posljednji = novi;
      } else {
        Cvor *slj = trenutni_cvor->sljedeci;
        trenutni_cvor->sljedeci = novi;
        novi->prethodni = trenutni_cvor;
        novi->sljedeci = slj;
        slj->prethodni = novi;
      }
    }
    br_el++;
  }
  Tip &operator[](int indeks) override {
    if (indeks < 0 || indeks >= br_el)
      throw std::out_of_range("Neispravan indeks");
    Cvor *it=prvi;
    for (int i = 0; i < indeks; i++)
      it = it->sljedeci;
    return it->el;
  }
  Tip operator[](int indeks) const override {
    if (indeks < 0 || indeks >= br_el)
      throw std::out_of_range("Neispravan indeks");
    Cvor *it=prvi;
    for (int i = 0; i < indeks; i++)
      it = it->sljedeci;
    return it->el;
  }
};

template<typename Tip1>
class Iterator  
{
    const DvostrukaLista<Tip1> *lista;
    typename DvostrukaLista<Tip1>::Cvor *trenutni_cvor;
    public:
    Iterator(const DvostrukaLista<Tip1> &DL)
    {
        lista=&DL;
        trenutni_cvor=DL.prvi;
    }
    Tip1 dajTrenutni()
    {
        if(lista->brojElemenata()==0) throw std::logic_error("Lista nema elemenata");
        return trenutni_cvor->el;
    }
    bool sljedeci()
    {
        if(lista->brojElemenata()==0) throw std::logic_error("Lista nema elemenata");
        if(trenutni_cvor->sljedeci==nullptr) return false;
        auto slj=trenutni_cvor->sljedeci;
        trenutni_cvor=slj;
        return true;
    }
};

template <typename Tip> Tip dajMaksimum(const Lista<Tip> &n) {
    if(n.brojElemenata()==0) throw std::logic_error("Lista nema elemenata");
    Iterator<Tip> i=dynamic_cast<const DvostrukaLista<Tip>&>(n);;
    Tip najveci=i.dajTrenutni();
    while(i.sljedeci())
    {
        if(i.dajTrenutni()>najveci) najveci=i.dajTrenutni();
    }
    return najveci;
}

int main() {
  DvostrukaLista<int> l;

  for (int i = 0; i < 5; i++)
    l.dodajIspred(i);

  for (int i = 5; i < 8; i++)
    l.dodajIza(i);

  l.prethodni();
  l.sljedeci();
  l.sljedeci();

  std::cout << "Broj elemenata je: " << l.brojElemenata() << std::endl;
  std::cout << "Trenutni element je: " << l.trenutni() << std::endl;
  std::cout<<"Najveci element je "<<dajMaksimum(l)<<std::endl;
  std::cout<<"Svi elementi su: ";
  l.pocetak();
  for(int i=0; i<l.brojElemenata(); i++)
  {
      std::cout<<l.trenutni()<<" ";
      l.sljedeci();
  }
  for (int i = 8; i > 0; i--)
    l.obrisi();
  return 0;
}