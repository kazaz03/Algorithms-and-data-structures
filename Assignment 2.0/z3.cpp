#include <iostream>

using namespace std;

template <typename Tip> class Stek {
  struct Cvor {
    Tip el;
    Cvor *sljedeci;
  };
  Cvor *top;
  int velicina;
  void jelprazan() const{
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

template<typename tip>
void brisitest(const Stek<tip> &s){
    Stek<tip> s2(s);
    s2.brisi();
    std::cout<<"Duzina steka: "<<s2.brojElemenata()<<"(treba biti 0)"<<std::endl;
}
void stavitest(const Stek<int> &s)
{
    Stek<int> s2(s);
    s2.stavi(3); s2.stavi(40);
    std::cout<<"Duzina steka je (treba biti 7) "<<s2.brojElemenata()<<endl;
}
void brojeltest(const Stek<int> &s){
    cout<<"Ova funkcija ispisuje broj elemenata steka: "<<s.brojElemenata()<<endl;
}
void skinitest(const Stek<int> &s){
    Stek<int> s2(s);
    s2.skini(); s2.skini();
    cout<<"Duzina steka je "<<s2.brojElemenata()<<" (treba biti manja za 2,tj iznosi 3)"<<endl;
}
void testvrh(const Stek<int> &s){
    cout<<"Ova funkcija ispisuje vrh steka: "<<s.vrh()<<endl;
}
int main(){
    Stek<int> stek;
    for(int i=0;i<10;i+=2) stek.stavi(i);
    cout<<"Duzina datog steka je "<<stek.brojElemenata()<<" ";
    brisitest(stek);
    stavitest(stek);
    brojeltest(stek);
    skinitest(stek);
    testvrh(stek);
    return 0;
}