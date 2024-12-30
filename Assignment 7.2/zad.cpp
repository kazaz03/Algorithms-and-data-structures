#include <iostream>
#include <time.h>

using namespace std;
int veci(int a, int b);

template <typename TipKljuca,typename TipVrijednosti> class Mapa {
public:
  Mapa<TipKljuca,TipVrijednosti>(){};
  virtual ~Mapa<TipKljuca,TipVrijednosti>(){};
  virtual int brojElemenata() const = 0;
  virtual void obrisi() = 0;
  virtual void obrisi(const TipKljuca &kljuc) = 0;
  virtual TipVrijednosti &operator[](TipKljuca k) = 0;
  virtual TipVrijednosti operator[](TipKljuca k) const = 0;
};

template <typename TipKljuca, typename TipVrijednosti>
class AVLStabloMapa : public Mapa<TipKljuca, TipVrijednosti> {

    struct Cvor {
    TipKljuca kljuc;
    TipVrijednosti vrijednost;
    Cvor *lijevo_podstablo;
    Cvor *desno_podstablo;
    Cvor *roditelj;
    int balans;
    };
    int getBalans(Cvor *t) const{
    if (t == nullptr)
      return 0;
    return t->balans;
    }
    Cvor *novi(TipKljuca i = TipKljuca(), TipVrijednosti vrijednost = TipVrijednosti()) {
    Cvor *temp = new Cvor();
    temp->kljuc = i;
    temp->vrijednost = vrijednost;
    temp->desno_podstablo = temp->lijevo_podstablo = temp->roditelj = nullptr;
    temp->balans = 0;
    return temp;
    }
    Cvor *nasljednik(Cvor *cvor) {
    auto temp = cvor;
    while (temp && temp->lijevo_podstablo != nullptr)
    temp = temp->lijevo_podstablo;
    return temp;
    }
    void rotiraj_desno(Cvor *cvor) {
    Cvor *temp = nullptr;
    int broj = 0;
    temp = cvor->lijevo_podstablo->desno_podstablo;
    cvor->balans = 0;
    Cvor *lijevo = cvor->lijevo_podstablo;
    Cvor *rod = cvor->roditelj;
    lijevo->roditelj = rod;
    lijevo->desno_podstablo = cvor;

    if (!lijevo->roditelj && !broj) {
      p = lijevo;
      broj = 1;
    } else if (rod->desno_podstablo == cvor && !broj) {
      lijevo->roditelj->desno_podstablo = lijevo;
      broj= 1;
    }

    if (!broj) {
      lijevo->roditelj->lijevo_podstablo = lijevo;
    }
    lijevo->balans = 0;

    if (temp)
      temp->roditelj = cvor;
    cvor->lijevo_podstablo = temp;
    cvor->roditelj = lijevo;
  }
  void rotiraj_lijevo(Cvor *cvor) {

    Cvor *temp = nullptr;
    int broj = 0;
    temp = cvor->desno_podstablo->lijevo_podstablo;
    cvor->balans = 0;
    Cvor *desno = cvor->desno_podstablo;
    Cvor *rod = cvor->roditelj;
    desno->roditelj = rod;
    desno->lijevo_podstablo = cvor;

    if (!desno->roditelj && !broj) {
      p = desno;
      broj= 1;
    } else if (rod->desno_podstablo == cvor && !broj) {
      desno->roditelj->desno_podstablo = desno;
      broj= 1;
    }

    if (!broj) {
      desno->roditelj->lijevo_podstablo = desno;
    }

    desno->balans = 0;

    if (temp)
      temp->roditelj = cvor;
    cvor->desno_podstablo = temp;
    cvor->roditelj = desno;
  }

  void azuriraj(Cvor *cvor, bool bul) {

    if (!cvor)
      return;
    else if (bul)
      cvor->balans--;
    else
      cvor->balans++;

    int blns = cvor->balans;

    if (cvor->roditelj) {

      if (cvor->roditelj->desno_podstablo == cvor)
        bul = true;
      else
        bul = false;
    }

    if (blns == 2) {

      if (getBalans(cvor->lijevo_podstablo) == 1)
        rotiraj_desno(cvor);
      else {
        rotiraj_lijevo(cvor->lijevo_podstablo);
        rotiraj_desno(cvor);
      }

    } else if (blns == -2) {
      if (getBalans(cvor->desno_podstablo) == -1)
        rotiraj_lijevo(cvor);

      else {
        rotiraj_desno(cvor->desno_podstablo);
        rotiraj_lijevo(cvor);
      }
    } else {

      if (blns == 0)
        return;
      azuriraj(cvor->roditelj, bul);
    }
  }
  Cvor *pretraga(Cvor *cvor, TipKljuca k) const{
    if (cvor == nullptr || p->kljuc == k)
      return cvor;
    if (cvor->kljuc < k)
      return pretraga(cvor->desno_podstablo, k);
    else if (cvor->kljuc > k)
      return pretraga(cvor->lijevo_podstablo, k);
    return cvor;
  }
  void konstr(Cvor *cvor1, Cvor *cvor2) {

    if (cvor2== nullptr)
      return;

    konstr(cvor1, cvor2->lijevo_podstablo);
    konstr(cvor1, cvor2->desno_podstablo);
    dodaj(cvor1, cvor2->kljuc, cvor2->vrijednost);
  }

  Cvor *dodaj(Cvor *p, TipKljuca k, TipVrijednosti vrij = TipVrijednosti(), Cvor *rod = nullptr) {

    while (p) {

      if (p->kljuc == k)
        return p;
      else if (k> p->kljuc)
        p = p->desno_podstablo;
      else
        p = p->lijevo_podstablo;
    }

    if (!p) {

      Cvor *t = AVLStabloMapa::p, *q = nullptr;
      p = novi(k, vrij);
      while (t) {
        q = t;
        if (t->kljuc > p->kljuc)
          t = t->lijevo_podstablo;
        else
          t = t->desno_podstablo;
      }

      if (!q)
        AVLStabloMapa::p = p;
      else if (q->kljuc > p->kljuc) {
        q->lijevo_podstablo = p;
        p->roditelj = q;
      } else {
        q->desno_podstablo = p;
        p->roditelj = q;
      }

      if (!q)
        return p;

      if (q->lijevo_podstablo && q->desno_podstablo)
        q->balans = 0;
      else {
        bool b = true;
        if (q->lijevo_podstablo)
          b = false;
        azuriraj(q, b);
      }
    }

    return p;
  }
  void obrisiCvor(Cvor *cvor, TipKljuca k) {

    Cvor *temp = nullptr;

    while (cvor != nullptr && cvor->kljuc != k) {

      temp = cvor;
      if (cvor->kljuc < k)
        cvor = cvor->desno_podstablo;
      else
        cvor = cvor->lijevo_podstablo;
    }
    if (!cvor)
      return;
    Cvor *cvor1, *cvor2, *cvor3;

    if (!cvor->lijevo_podstablo){
      cvor1 = cvor->desno_podstablo;
      cvor3 = temp;}
    else if (!cvor->desno_podstablo){
      cvor1 = cvor->lijevo_podstablo;
      cvor3 = temp;}
    else {
      cvor1 = cvor->lijevo_podstablo;
      cvor2 = cvor1->desno_podstablo;
      cvor3= cvor;
      while (cvor2) {
        cvor3 = cvor1;
        cvor1 = cvor2;
        cvor2 = cvor1->desno_podstablo;
      }
      if (cvor != cvor3) {
        cvor3->desno_podstablo = cvor1->lijevo_podstablo;
        if (cvor1->lijevo_podstablo)
          cvor1->lijevo_podstablo->roditelj = cvor3;
          cvor1->lijevo_podstablo = cvor->lijevo_podstablo;
        if (cvor->lijevo_podstablo)
          cvor->lijevo_podstablo->roditelj = cvor1;
      }
      cvor1->desno_podstablo = cvor->desno_podstablo;
      if (cvor->desno_podstablo)
        cvor->desno_podstablo->roditelj = cvor1;
    }

    if (!temp) {
      AVLStabloMapa::p = cvor1;
    } else {
      if (temp->lijevo_podstablo == cvor) {
        temp->lijevo_podstablo = cvor1;
        temp->balans--;
      } else {
        temp->desno_podstablo = cvor1;
        temp->balans++;
      }
    }

    if (cvor1!=nullptr)  cvor1->roditelj = temp;

      cvor->desno_podstablo = cvor->lijevo_podstablo = cvor->roditelj = nullptr;
      delete cvor;
    
  }

  void obrisi_stablo(Cvor *cvor) {

    if (!cvor)
      return;
    obrisi_stablo(cvor->lijevo_podstablo);
    obrisi_stablo(cvor->desno_podstablo);
    delete cvor;
  }
  int broj_el;
  Cvor *p;

public:
  AVLStabloMapa<TipKljuca, TipVrijednosti>() : broj_el(0), p(nullptr) {}

  ~AVLStabloMapa<TipKljuca, TipVrijednosti>() { if(p!=nullptr) obrisi_stablo(p); }
  
  int brojElemenata() const { return broj_el; }

  void obrisi() {
    obrisi_stablo(p);
    p = nullptr;
    broj_el = 0;
  }

  void obrisi(const TipKljuca &kljuc) {
    obrisiCvor(p, kljuc);
    broj_el--;
  }

  AVLStabloMapa<TipKljuca, TipVrijednosti>(const AVLStabloMapa<TipKljuca, TipVrijednosti> &stablo) {
    broj_el = stablo.broj_el;
    if (stablo.broj_el == 0)
      p = nullptr;
    else {
      p = novi(stablo.p->kljuc, stablo.p->vrijednost);
      konstr(p, stablo.p);
    }
  }

  AVLStabloMapa<TipKljuca, TipVrijednosti> &operator=(const AVLStabloMapa<TipKljuca, TipVrijednosti> &stablo) {

    if (this == &stablo)
      return *this;

    obrisi();
    broj_el = stablo.broj_el;
    if (stablo.broj_el == 0)
      p = nullptr;

    else {
      if(!p)p = novi(stablo.p->kljuc,stablo.p->vrijednost);
      p->kljuc = stablo.p->kljuc;
      p->vrijednost = stablo.p->vrijednost;
      konstr(p, stablo.p);
    }
    return *this;
  }

  TipVrijednosti &operator[](TipKljuca kljuc) {

    if (p == nullptr) {
      p = novi(kljuc);
      broj_el++;
      return p->vrijednost;
    }

    auto c = pretraga(p, kljuc);

    if (c == nullptr) {
      dodaj(p, kljuc);
      broj_el++;
      c = pretraga(p, kljuc);
    }
    return c->vrijednost;
  }

   TipVrijednosti operator[](TipKljuca kljuc) const {
    auto temp = pretraga(p, kljuc);
    if (temp)
      return temp->vrijednost;
    return TipVrijednosti();
  }
  void preorderIspis(Cvor *cvor)
{
    if(cvor==nullptr) return;
    std::cout<<cvor->vrijednost<<" ";
    preorderIspis(cvor->lijevo_podstablo);
    preorderIspis(cvor->desno_podstablo);
}
void Ispis()
{
    preorderIspis(this->p);
}
};

int veci(int a, int b)
{
    if(a>b) return a;
    return b;
}

int main() {
    AVLStabloMapa<string,string> mapa1;
  AVLStabloMapa<string,string> mapa2;
  mapa1["Amina"]="Kazazovic";
  mapa1["Sedin"]="Kazazovic";
  mapa1["Daris"]="Mujkic"; 
  mapa1["Sabina"]="Kazazovic";
  mapa1["Samra"]="Tufo";
  mapa1["Senida"]="Kezic";
  mapa2["Amina"]="Kazazovic";
  mapa2["Sedin"]="Kazazovic";
  mapa2["Daris"]="Mujkic"; 
  mapa2["Sabina"]="Kazazovic";
  mapa2["Samra"]="Tufo";
  mapa2["Senida"]="Kezic";
  mapa1.Ispis();
  std::cout<<std::endl;
  mapa2.Ispis();
  return 0;
}