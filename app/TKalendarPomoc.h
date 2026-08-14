//---------------------------------------------------------------------------

#ifndef TKalendarPomocH
#define TKalendarPomocH
//---------------------------------------------------------------------------
#include <System.hpp>
#include <System.SysUtils.hpp>
//---------------------------------------------------------------------------
#ifdef VRTICKALENDARMODUL_EXPORTS
#define KALENDAR_API __declspec(dllexport)
#else
#define KALENDAR_API __declspec(dllimport)
#endif
//---------------------------------------------------------------------------
class KALENDAR_API TKalendarPomoc
{
public:
	// danUTjednu: 1 = nedjelja, 7 = subota (VCL DayOfWeek konvencija) -
	// poziva se DayOfWeek(datum) u pozivajućoj formi, ne unutar DLL-a.
	bool JeVikend(int danUTjednu);

	// danas se šalje iz pozivajuće forme (Date()) da DLL ne mora sam
	// zvati RTL funkciju Date().
	int PreostaloDana(TDate datum, TDate danas);
};
//---------------------------------------------------------------------------
#endif
