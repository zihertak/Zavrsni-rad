//---------------------------------------------------------------------------

#pragma hdrstop

#include "Sigurnost.h"
#include <vcl.h>
#include <System.Hash.hpp>

// Sol se generira po pravilu iz korisničkog imena (obrnuto, velika slova) -
// ne sprema se nigdje, svaki put se izračuna iznova.
String GenerirajSol(const String &username)
{
	String obrnuto = "";
	for (int i = username.Length(); i >= 1; i--)
	{
		obrnuto = obrnuto + username[i];
	}
	return obrnuto.UpperCase();
}
//---------------------------------------------------------------------------
String Hashiraj(const String &lozinka, const String &sol)
{
	System::Hash::THashSHA2 hash;
	return hash.GetHashString(lozinka + sol);
}
//---------------------------------------------------------------------------
#pragma package(smart_init)
