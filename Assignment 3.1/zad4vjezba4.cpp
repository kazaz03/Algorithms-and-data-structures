#include <iostream>
#include <string.h>

using namespace std;

template <typename Tip> class Red {
  struct Cvor {
    Tip el;
    Cvor *sljedeci;
  };
  Cvor *pocetak;
  Cvor *kraj;
  int br_el;
  void brisanje_el() {
    if (br_el == 0)
      return;
    auto it = pocetak;
    while (it != nullptr) {
      Cvor *sljedeci = it->sljedeci;
      it->sljedeci = nullptr;
      delete it;
      it = sljedeci;
    }
    pocetak = nullptr;
    kraj = nullptr;
    br_el = 0;
  }

public:
  Red() : br_el(0), kraj(nullptr), pocetak(nullptr){};
  ~Red() { brisanje_el(); }
  Red(const Red &R);
  Red(Red &&R) {
    pocetak = R.pocetak;
    kraj = R.kraj;
    br_el = R.br_el;
  }
  Red &operator=(const Red &R);
  Red &operator=(Red &&R);
  void brisi() { brisanje_el(); };
  void stavi(const Tip &el);
  Tip skini();
  Tip &celo() {
    if (!br_el)
      throw std::logic_error("Red je prazan");
    return pocetak->el;
  }
  int brojElemenata() const { return br_el; };
  void veci_od_asr();
  friend class StampacServer;
};

template <typename Tip> Red<Tip> &Red<Tip>::operator=(const Red &R) {
  if (this == &R)
    return *this;
  brisanje_el();
  auto it = R.pocetak;
  bool prvi = true;
  Cvor *prosli = nullptr;
  while (it != nullptr) {
    Cvor *novi = new Cvor{it->el, nullptr};
    if (prvi) {
      pocetak = novi;
      prosli = pocetak;
    } else {
      prosli->sljedeci = novi;
      prosli = novi;
    }
    it = it->sljedeci;
    prvi = false;
  }
  kraj = prosli;
  prosli = nullptr;
  br_el = R.br_el;
  return *this;
}

template <typename Tip> Red<Tip> &Red<Tip>::operator=(Red &&R) {
  if (this == &R)
    return *this;
  std::swap(pocetak, R.pocetak);
  std::swap(kraj, R.kraj);
  std::swap(br_el, R.br_el);
  return *this;
}

template <typename Tip> Red<Tip>::Red(const Red &R) {
  auto it = R.pocetak;
  bool prvi = true;
  Cvor *prosli = nullptr;
  while (it != nullptr) {
    Cvor *novi = new Cvor{it->el, nullptr};
    if (prvi) {
      pocetak = novi;
      prosli = pocetak;
    } else {
      prosli->sljedeci = novi;
      prosli = novi;
    }
    it = it->sljedeci;
    prvi = false;
  }
  kraj = prosli;
  prosli = nullptr;
  br_el = R.br_el;
}

template <typename Tip> void Red<Tip>::stavi(const Tip &el) {
  Cvor *novi = new Cvor{el, nullptr};
  if (br_el == 0) {
    pocetak = novi;
    kraj = novi;
    br_el++;
  } else {
    kraj->sljedeci = novi;
    kraj = novi;
    br_el++;
  }
}

template <typename Tip> Tip Red<Tip>::skini() {
  if (!br_el)
    throw std::logic_error("Red je prazan");
  Tip el = pocetak->el;
  Cvor *sljed = pocetak->sljedeci;
  pocetak->sljedeci = nullptr;
  delete pocetak;
  pocetak = sljed;
  br_el--;
  if (br_el == 0)
    kraj = nullptr;
  return el;
}

/*
Ova funkcija treba da prihvati dokument sa odgovarajućim prioritetom i pohrani
ga za kasnije Osim toga, ova klasa treba da implementira funkciju string
dajDokument() koja od dokumenata koji su poslati za štampanje vraća neodštampani
dokument sa najvišim prioritetom, a ako ima više dokumenata sa istim
prioritetom, onda najstariji. Logično, najviši prioriet je Sef, a zatim Radnik i
najniži Pripravnik. Ako ne postoji niti jedan neodštampani dokument, treba
vratiti prazan string.
Klasa bi trebala biti korištena tako što će driver za štampače na računarima
uposlenika pozivati funkciju posaljiDokuement, a štampači čim završe sa
prethodnim dokumentom, pozivati funkciju dajDokument. Ovakvo ponašanje treba
simulirati u main programu
*/

enum Prioritet { Sef, Radnik, Pripravnik };

struct Dokument {
  Prioritet prioritet;
  string sadrzaj = " ";
  Dokument(Prioritet p, string s) {
    prioritet = p;
    sadrzaj = s;
  }
};

class StampacServer {
private:
  Red<Dokument> dokumenti;

public:
  void posaljiDokument(Prioritet p, string sadrzaj);
  string dajDokument();
};

void StampacServer::posaljiDokument(Prioritet p, string sadrzaj) {
  Dokument d(p, sadrzaj);
  if (dokumenti.br_el == 0) {
    dokumenti.stavi(d);
    return;
  }
  int duzina = dokumenti.br_el;
  int brojac = 0;
  auto it = dokumenti.pocetak;
  Dokument d1 = it->el;
  int broj = d1.prioritet;
  //pomjeramo sve ove koji su ispred njega (veci prioritet) na kraj i ako 
  // su svi pomjereni onda njega samo na kraj
  //a ako nisu svi pomjereni onda njega dodamo pa ovi sto su ostali nepomjereni
  //sa manjim prioritetom dodamo poslije njega
  while (broj <= p) {
    if (brojac == duzina)
      break;
    dokumenti.stavi(it->el);
    dokumenti.skini();
    it = dokumenti.pocetak;
    d1 = it->el;
    broj = d1.prioritet;
    brojac++;
  }
  if (brojac == duzina)
    dokumenti.stavi(d);
  else {
    dokumenti.stavi(d);
    int a = 0;
    it = dokumenti.pocetak;
    while (a < duzina-brojac) {
      dokumenti.stavi(it->el);
      dokumenti.skini();
      it = dokumenti.pocetak;
      a++;
    }
  }
}

string StampacServer::dajDokument() {
  if (dokumenti.br_el == 0)
    return " ";
  auto it = dokumenti.pocetak;
  Dokument d = it->el;
  dokumenti.skini();
  return d.sadrzaj + "\n";
}

int main() {
  StampacServer s;
  s.posaljiDokument(Sef, "Dobar dan kakoste");
  s.posaljiDokument(Pripravnik,"Pa cao");
  s.posaljiDokument(Sef, "Stavi vodu sad ovako");
  s.posaljiDokument(Radnik, "Auh bokte");
  s.posaljiDokument(Sef, "Kako tako");
  s.posaljiDokument(Pripravnik, "Treci");
  s.posaljiDokument(Radnik, "nez koji");
  s.posaljiDokument(Sef,"Vidi vidi");
  auto it = s;
  string k=it.dajDokument();
  while(k!=" ")
  {
      std::cout<<k;
      k=it.dajDokument();
  }
  return 0;
}