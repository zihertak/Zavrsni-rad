//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "RazvojPomoc.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
// Ocjena razvoja (1-5) -> boja za prikaz zvjezdica. Ranije je ista funkcija
// postojala odvojeno i identično u RazvojForm.cpp i RazvojDetaljiForm.cpp.
TColor TRazvojPomoc::BojaZaOcjenu(int ocjena)
{
	switch (ocjena)
	{
		case 1:
			return clRed;                                  // Crvena

		case 2:
			return static_cast<TColor>(RGB(255, 128, 0));   // Narančasta

		case 3:
			return static_cast<TColor>(RGB(255, 215, 0));   // Zlatno žuta

		case 4:
			return static_cast<TColor>(RGB(50, 205, 50));   // Lime zelena

		case 5:
			return static_cast<TColor>(RGB(0, 128, 0));     // Tamno zelena

		default:
			return clSilver;
	}
}
//---------------------------------------------------------------------------
// Ocjena razvoja (1-5) -> kratki tekstualni opis, npr. za izvještaj.
String TRazvojPomoc::OpisOcjene(int ocjena)
{
	switch (ocjena)
	{
		case 1:
			return L"slabo";

		case 2:
			return L"ispod prosjeka";

		case 3:
			return L"prosječno";

		case 4:
			return L"dobro";

		case 5:
			return L"odlično";

		default:
			return L"nepoznato";
	}
}
//---------------------------------------------------------------------------
// Dob u godinama iz datuma rođenja, uzimajući u obzir je li rođendan ove
// godine već prošao. Ista logika je ranije bila zasebno napisana u
// DjecaForm.cpp (OnCalcFields) i IzvjestajForm.cpp.
int TRazvojPomoc::IzracunajDob(TDate datumRodjenja)
{
	unsigned short godinaRodjenja, mjesecRodjenja, danRodjenja;
	unsigned short trenutnaGodina, trenutniMjesec, trenutniDan;

	datumRodjenja.DecodeDate(&godinaRodjenja, &mjesecRodjenja, &danRodjenja);
	Date().DecodeDate(&trenutnaGodina, &trenutniMjesec, &trenutniDan);

	int dob = trenutnaGodina - godinaRodjenja;

	if (trenutniMjesec < mjesecRodjenja ||
		(trenutniMjesec == mjesecRodjenja && trenutniDan < danRodjenja))
	{
		dob--;
	}

	return dob;
}
