//---------------------------------------------------------------------------

#ifndef KorisnickaPravaH
#define KorisnickaPravaH
//---------------------------------------------------------------------------
#include <System.hpp>
class TKorisnickaPrava{
private:
	String uloga;
public:
	TKorisnickaPrava(const String &u);
    bool admin() const;
    bool ravnatelj() const;
    bool odgojitelj() const;
	bool strucni() const;
	bool korisnici() const;
    bool dodajDijete() const;
    bool urediDijete() const;
	bool obrisiDijete() const;
    bool dodajSkupinu() const;
	bool urediSkupinu() const;
	bool obrisiSkupinu() const;
    bool dodajAktivnost() const;
	bool urediAktivnost() const;
	bool obrisiAktivnost() const;
	bool zdravstvo() const;
	bool samoSvojaSkupina() const;
    bool unesiRazvojniPregled() const;
	bool generirajPreporuku() const;
	bool spremiPreporuku() const;
	bool promijeniStatusPreporuke() const;
};
#endif
