//---------------------------------------------------------------------------

#ifndef KorisnikH
#define KorisnikH
//---------------------------------------------------------------------------
#include <System.hpp>
class TKorisnik{
private:
	int idKorisnik;
	String ime;
	String prezime;
	String uloga;
	int idSkupina;
	bool aktivan;
public:
	TKorisnik();
	void Postavi(int id, const String &i, const String &p, const String &u, int idSk, bool akt);
	void Odjava();
	String PunoIme() const;
	int IdKorisnik() const { return idKorisnik; }
	String Uloga() const { return uloga; }
	int IdSkupina() const { return idSkupina; }
	bool Aktivan() const { return aktivan; }
};
#endif
