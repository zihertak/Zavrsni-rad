	//---------------------------------------------------------------------------

#pragma hdrstop

#include "KorisnickaPrava.h"
#include <vcl.h>

TKorisnickaPrava::TKorisnickaPrava(const String &u)
{
    uloga = u;
}
bool TKorisnickaPrava::admin() const
{
    return uloga == L"Administrator";
}
bool TKorisnickaPrava::ravnatelj() const
{
    return uloga == L"Ravnatelj";
}
bool TKorisnickaPrava::odgojitelj() const
{
    return uloga == L"Odgojitelj";
}
bool TKorisnickaPrava::strucni() const
{
    return uloga == L"Strucni suradnik";
}
bool TKorisnickaPrava::korisnici() const
{
    return admin();
}
bool TKorisnickaPrava::dodajDijete() const
{
    return admin() || ravnatelj() || odgojitelj();
}
bool TKorisnickaPrava::urediDijete() const
{
    return admin() || ravnatelj() || odgojitelj() || strucni();
}
bool TKorisnickaPrava::obrisiDijete() const
{
    return admin() || ravnatelj() || odgojitelj();
}
bool TKorisnickaPrava::zdravstvo() const
{
    return true;
}
bool TKorisnickaPrava::samoSvojaSkupina() const
{
    return odgojitelj();
}
bool TKorisnickaPrava::dodajSkupinu() const
{
	return admin() || ravnatelj();
}

bool TKorisnickaPrava::urediSkupinu() const
{
	return admin() || ravnatelj();
}

bool TKorisnickaPrava::obrisiSkupinu() const
{
	return admin() || ravnatelj();
}

bool TKorisnickaPrava::dodajAktivnost() const
{
	return admin() ||
		   ravnatelj() ||
		   odgojitelj() ||
		   strucni();
}

bool TKorisnickaPrava::urediAktivnost() const
{
	return admin() ||
		   ravnatelj() ||
		   odgojitelj() ||
		   strucni();
}

bool TKorisnickaPrava::obrisiAktivnost() const
{
	return admin() ||
		   ravnatelj();
}
//---------------------------------------------------------------------------
#pragma package(smart_init)

bool TKorisnickaPrava::unesiRazvojniPregled() const
{
    return admin() || odgojitelj();
}

bool TKorisnickaPrava::generirajPreporuku() const
{
    return admin() ||
           ravnatelj() ||
           odgojitelj() ||
           strucni();
}

bool TKorisnickaPrava::spremiPreporuku() const
{
    return admin() ||
           odgojitelj() ||
           strucni();
}

bool TKorisnickaPrava::promijeniStatusPreporuke() const
{
    return admin() ||
           odgojitelj() ||
           strucni();
}