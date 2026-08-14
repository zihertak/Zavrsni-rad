//---------------------------------------------------------------------------

#pragma hdrstop

#include "Korisnik.h"
#include <vcl.h>

TKorisnik::TKorisnik()
{
	idKorisnik = 0;
	ime = "";
	prezime = "";
	uloga = "";
	idSkupina = 0;
	aktivan = false;
}
//---------------------------------------------------------------------------
void TKorisnik::Postavi(int id, const String &i, const String &p, const String &u, int idSk, bool akt)
{
	idKorisnik = id;
	ime = i;
	prezime = p;
	uloga = u;
	idSkupina = idSk;
	aktivan = akt;
}
//---------------------------------------------------------------------------
void TKorisnik::Odjava()
{
	idKorisnik = 0;
	ime = "";
	prezime = "";
	uloga = "";
	idSkupina = 0;
	aktivan = false;
}
//---------------------------------------------------------------------------
String TKorisnik::PunoIme() const
{
	return ime + L" " + prezime;
}
//---------------------------------------------------------------------------
#pragma package(smart_init)
