//---------------------------------------------------------------------------

#pragma hdrstop

#include "KorisnickaPrava.h"
#include <vcl.h>
#pragma package(smart_init)

TKorisnickaPrava::TKorisnickaPrava(const String &u, bool akt)
{
	uloga = u;
	aktivan = akt;
}
//---------------------------------------------------------------------------
bool TKorisnickaPrava::jeAktivan() const
{
	return aktivan;
}
//---------------------------------------------------------------------------
bool TKorisnickaPrava::admin() const
{
	return aktivan && uloga == L"Administrator";
}
//---------------------------------------------------------------------------
bool TKorisnickaPrava::ravnatelj() const
{
	return aktivan && uloga == L"Ravnatelj";
}
//---------------------------------------------------------------------------
bool TKorisnickaPrava::odgojitelj() const
{
	return aktivan && uloga == L"Odgojitelj";
}
//---------------------------------------------------------------------------
bool TKorisnickaPrava::strucni() const
{
	return aktivan && uloga == L"Strucni suradnik";
}
//---------------------------------------------------------------------------
bool TKorisnickaPrava::korisnici() const
{
	return admin();
}
//---------------------------------------------------------------------------
bool TKorisnickaPrava::dodajDijete() const
{
	return admin() || ravnatelj() || odgojitelj();
}
//---------------------------------------------------------------------------
bool TKorisnickaPrava::urediDijete() const
{
	return admin() || ravnatelj() || odgojitelj() || strucni();
}
//---------------------------------------------------------------------------
bool TKorisnickaPrava::obrisiDijete() const
{
	return admin() || ravnatelj() || odgojitelj();
}
//---------------------------------------------------------------------------
bool TKorisnickaPrava::zdravstvo() const
{
	return true;
}
//---------------------------------------------------------------------------
bool TKorisnickaPrava::samoSvojaSkupina() const
{
	return odgojitelj();
}
//---------------------------------------------------------------------------
bool TKorisnickaPrava::dodajSkupinu() const
{
	return admin() || ravnatelj();
}
//---------------------------------------------------------------------------
bool TKorisnickaPrava::urediSkupinu() const
{
	return admin() || ravnatelj();
}
//---------------------------------------------------------------------------
bool TKorisnickaPrava::obrisiSkupinu() const
{
	return admin() || ravnatelj();
}
//---------------------------------------------------------------------------
bool TKorisnickaPrava::dodajAktivnost() const
{
	return admin() || ravnatelj() || odgojitelj() || strucni();
}
//---------------------------------------------------------------------------
bool TKorisnickaPrava::urediAktivnost() const
{
	return admin() || ravnatelj() || odgojitelj() || strucni();
}
//---------------------------------------------------------------------------
bool TKorisnickaPrava::obrisiAktivnost() const
{
	return admin() || ravnatelj();
}
//---------------------------------------------------------------------------
bool TKorisnickaPrava::unesiRazvojniPregled() const
{
	return admin() || odgojitelj();
}
//---------------------------------------------------------------------------
bool TKorisnickaPrava::generirajPreporuku() const
{
	return admin() || ravnatelj() || odgojitelj() || strucni();
}
//---------------------------------------------------------------------------
bool TKorisnickaPrava::spremiPreporuku() const
{
	return admin() || odgojitelj() || strucni();
}
//---------------------------------------------------------------------------
bool TKorisnickaPrava::promijeniStatusPreporuke() const
{
	return admin() || odgojitelj() || strucni();
}
//---------------------------------------------------------------------------
