#include <iostream>

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

template<typename Tip>
Stek<Tip> spojiSortirano(Stek<Tip> s1, Stek<Tip> s2)
{
    Stek<Tip> rezultat;
    dodajEl(s1,s2,rezultat);
    return rezultat;
}

template<typename Tip>
void dodajEl(Stek<Tip> s1, Stek<Tip> s2,Stek<Tip> &R)
{
    if(s1.brojElemenata()==0 && s2.brojElemenata()==0)
    {
        return;
    }
    Tip broj;
    if(s1.brojElemenata()==0) broj=s2.skini();
    else if(s2.brojElemenata()==0) broj=s1.skini();
    else if(s1.vrh()<s2.vrh()) 
    {
        broj=s1.skini();
    }
    else if(s1.vrh()>s2.vrh())
    {
        broj=s2.skini();
    }
    else {
        broj=s1.skini();
        s2.skini();
    }
    dodajEl(s1,s2,R);
    R.stavi(broj);
}

int main()
{
    Stek<int> s1; Stek<int> s2;
    s1.stavi(8);
    s1.stavi(7);
    s1.stavi(6);
    s1.stavi(5);
    s1.stavi(4);
    s2.stavi(9);
    s2.stavi(5);
    s2.stavi(3);
    Stek<int> r=spojiSortirano(s1,s2);
    int k=r.brojElemenata();
    for(int i=0; i<k; i++)
    {
        std::cout<<r.vrh()<<" ";
        r.skini();
    }
    int n=4;
    int s=16;
    for(int i=0; i<n*n; i++) s++;
    std::cout<<"-"<<s;
    return 0;
}