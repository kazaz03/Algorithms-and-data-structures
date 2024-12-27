#include <iostream>
#include <vector>

using namespace std;

template <typename Tip> class Stek {
  struct Cvor {
    Tip el;
    Cvor *sljedeci;
  };
  Cvor *top;
  int velicina;
  void jelprazan() {
    if (velicina == 0)
      throw std::logic_error("Stek je prazan");
  }
  void obrisiel() {
    while (top != nullptr) {
      Cvor *a = top->sljedeci;
      top->sljedeci = nullptr;
      delete top;
      top = a;
    }
    velicina = 0;
  }

public:
  Stek() : velicina(0), top(nullptr){};
  ~Stek() { obrisiel(); }
  Stek(const Stek &S);
  Stek(Stek &&S) {
    top = S.top;
    velicina = S.velicina;
  }
  Stek &operator=(const Stek &S);
  Stek &operator=(Stek &&S) {
    std::swap(top, S.top);
    std::swap(velicina, S.velicina);
    return *this;
  }
  void brisi() { obrisiel(); } // done
  void stavi(const Tip &el);   // done
  Tip skini();                 // done
  Tip &vrh() {
    jelprazan();
    return top->el;
  }
  Tip vrh() const {
    jelprazan();
    return top->el;
  }
  int brojElemenata() const { return velicina; }
};

template <typename Tip>
Stek<Tip> &Stek<Tip>::operator=(const Stek<Tip> &S) { // popravi
  if (this == &S)
    return *this;
  obrisiel();
  auto it = S.top;
  int vel = S.brojElemenata();
  while (vel > 0) {
    it = S.top;
    for (int i = 0; i < vel - 1; i++) {
      it = it->sljedeci;
    }
    vel--;
    stavi(it->el);
  }
  return *this;
}

template <typename Tip> Stek<Tip>::Stek(const Stek<Tip> &S) {
  velicina = 0;
  top = nullptr;
  if (this == &S)
    return;
  auto it = S.top;
  int vel = S.brojElemenata();
  while (vel > 0) {
    it = S.top;
    for (int i = 0; i < vel - 1; i++) {
      it = it->sljedeci;
    }
    vel--;
    stavi(it->el);
  }
}

template <typename Tip> Tip Stek<Tip>::skini() {
  jelprazan();
  Tip element = top->el;
  Cvor *it = top->sljedeci;
  top->sljedeci = nullptr;
  delete top;
  top = it;
  velicina--;
  return element;
}

template <typename Tip> void Stek<Tip>::stavi(const Tip &el) {
  if (top == nullptr) {
    top = new Cvor{el, nullptr};
    velicina++;
  } else {
    Cvor *novi = new Cvor{el, nullptr};
    novi->sljedeci = top;
    top = novi;
    velicina++;
    novi = nullptr;
  }
}

void pretraga(Stek<vector<int>> &s, int trazeni) {
  static int brojac = 0;
  static int vel = s.brojElemenata();
  if(!vel)
  {
      std::cout<<"Nema elementa";
      return;
  }
  if (s.brojElemenata() == 0) {
    return;
  }
  std::vector<int> el = s.skini();
  pretraga(s, trazeni);
  s.stavi(el);
  int dno = 0;
  int kraj = el.size() - 1;
  int sredina = 0;
  while (dno <= kraj) {
    sredina = dno+(kraj - dno) / 2;
    if (el.at(sredina) == trazeni) {
      {
        std::cout << sredina << " " << brojac;
        return;
      }
    }
    if (el.at(sredina) < trazeni) {
      dno = sredina + 1;
    } else if (el.at(sredina) > trazeni) {
      kraj = sredina - 1;
    }
  }
  brojac++;
  if (brojac == vel) {
    std::cout << "Nema elementa";
    return;
  }
}

int main() {
  Stek<vector<int>> s;
  vector<int> a;
  for (int i(1); i < 10; i++)
    a.push_back(i);
  vector<int> b;
  b.push_back(6);
  b.push_back(8);
  b.push_back(12);
  vector<int> c;
  c.push_back(9);
  c.push_back(10);
  c.push_back(130);
  s.stavi(a);
  s.stavi(b);
  s.stavi(c);
  pretraga(s, 130);
  return 0;
}