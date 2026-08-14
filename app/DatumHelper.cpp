//---------------------------------------------------------------------------

#pragma hdrstop

#include "DatumHelper.h"
#include <vcl.h>
//---------------------------------------------------------------------------
static String Dvoznamenkasto(int broj)
{
	if (broj < 10)
	{
		return L"0" + IntToStr(broj);
	}

	return IntToStr(broj);
}
//---------------------------------------------------------------------------
// Format() s '%02d' zna ispuniti razmakom umjesto nulom u ovoj verziji,
// zato se broj ručno oblikuje - da bi datum uvijek imao točno 10 znakova
// (GGGG-MM-DD) i mogao se pouzdano parsirati natrag preko fiksnih pozicija.
String FormatirajDatum(TDate datum)
{
	unsigned short godina, mjesec, dan;
	datum.DecodeDate(&godina, &mjesec, &dan);

	return IntToStr(godina) + L"-" + Dvoznamenkasto(mjesec) + L"-" + Dvoznamenkasto(dan);
}
//---------------------------------------------------------------------------
TDate ParsirajDatum(const String &tekst)
{
	if (tekst.Length() < 10)
	{
		return Date();
	}

	int godina = StrToIntDef(tekst.SubString(1, 4).Trim(), 2026);
	int mjesec = StrToIntDef(tekst.SubString(6, 2).Trim(), 1);
	int dan = StrToIntDef(tekst.SubString(9, 2).Trim(), 1);

	try
	{
		return EncodeDate(godina, mjesec, dan);
	}
	catch (...)
	{
		return Date();
	}
}
//---------------------------------------------------------------------------
#pragma package(smart_init)
