//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TKalendarPomoc.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
bool TKalendarPomoc::JeVikend(int danUTjednu)
{
	return (danUTjednu == 1 || danUTjednu == 7);
}
//---------------------------------------------------------------------------
int TKalendarPomoc::PreostaloDana(TDate datum, TDate danas)
{
	return (int)(datum - danas);
}
