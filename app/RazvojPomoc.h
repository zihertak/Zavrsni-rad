//---------------------------------------------------------------------------

#ifndef RazvojPomocH
#define RazvojPomocH
//---------------------------------------------------------------------------
#include <System.hpp>
#include <Vcl.Graphics.hpp>
//---------------------------------------------------------------------------
class TRazvojPomoc
{
public:
	TColor BojaZaOcjenu(int ocjena);
	String OpisOcjene(int ocjena);
	int IzracunajDob(TDate datumRodjenja);
};
//---------------------------------------------------------------------------
#endif
