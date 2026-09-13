//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "RazvojPomoc.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
TColor TRazvojPomoc::BojaZaOcjenu(int ocjena)
{
	switch (ocjena)
	{
		case 1:
			return clRed;

		case 2:
			return static_cast<TColor>(RGB(255, 128, 0));

		case 3:
			return static_cast<TColor>(RGB(255, 215, 0));

		case 4:
			return static_cast<TColor>(RGB(50, 205, 50));

		case 5:
			return static_cast<TColor>(RGB(0, 128, 0));

		default:
			return clSilver;
	}
}
//---------------------------------------------------------------------------
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
