//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "BolestUnosForm.h"
#include "DatabaseModule.h"
#include "DatumHelper.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
Tform_bolest_unos *form_bolest_unos;
//---------------------------------------------------------------------------
__fastcall Tform_bolest_unos::Tform_bolest_unos(TComponent* Owner)
	: TForm(Owner),
	rezultatIdDijete(0)
{
}
//---------------------------------------------------------------------------
void Tform_bolest_unos::ucitajDjecu()
{
	combo_dijete->Items->Clear();
	query_djeca->Close();
	query_djeca->Open();

	while (!query_djeca->Eof)
	{
		int id = query_djeca->FieldByName("id_dijete")->AsInteger;
		String naziv =
			query_djeca->FieldByName("ime")->AsString + L" " +
			query_djeca->FieldByName("prezime")->AsString;

		combo_dijete->Items->AddObject(
			naziv,
			reinterpret_cast<TObject*>(static_cast<NativeInt>(id)));

		query_djeca->Next();
	}

	query_djeca->Close();
}
//---------------------------------------------------------------------------
void Tform_bolest_unos::pripremiZaDodavanje()
{
	Caption = L"Dodaj zapis o bolesti";
	ucitajDjecu();
    combo_dijete->ItemIndex = -1;
	date_od->Date = Date();
	date_do->Date = Date();
	combo_razlog->ItemIndex = -1;
	edit_napomena->Clear();
}
//---------------------------------------------------------------------------
void Tform_bolest_unos::pripremiZaUredjivanje(
	int idDijete,
	String od,
	String doDatuma,
	String razlog,
	String napomena)
{
	Caption = L"Uredi zapis o bolesti";
	ucitajDjecu();

	combo_dijete->ItemIndex = -1;
	for (int i = 0; i < combo_dijete->Items->Count; i++)
	{
		int id = static_cast<int>(
			reinterpret_cast<NativeInt>(combo_dijete->Items->Objects[i]));

		if (id == idDijete)
		{
			combo_dijete->ItemIndex = i;
			break;
		}
	}

	date_od->Date = ParsirajDatum(od);
	date_do->Date = ParsirajDatum(doDatuma);

	combo_razlog->ItemIndex = combo_razlog->Items->IndexOf(razlog);
	edit_napomena->Text = napomena;
}
//---------------------------------------------------------------------------
void __fastcall Tform_bolest_unos::button_spremiClick(TObject *Sender)
{
	if (combo_dijete->ItemIndex < 0)
	{
	    ShowMessage(L"Odaberite dijete.");
		return;
	}

	if (combo_razlog->ItemIndex < 0)
	{
		ShowMessage(L"Odaberite razlog.");
		return;
	}

	if (date_od->Date > Date())
	{
		ShowMessage(L"Datum 'Od' ne može biti u budućnosti.");
		return;
	}

	if (date_do->Date > Date())
	{
		ShowMessage(L"Datum 'Do' ne može biti u budućnosti.");
		return;
	}

	if (date_do->Date <= date_od->Date)
	{
		ShowMessage(L"Datum 'Do' mora biti nakon datuma 'Od' (ne mogu biti isti).");
		return;
	}

	rezultatIdDijete = static_cast<int>(
		reinterpret_cast<NativeInt>(
			combo_dijete->Items->Objects[combo_dijete->ItemIndex]));

	rezultatOd = FormatirajDatum(date_od->Date);
	rezultatDo = FormatirajDatum(date_do->Date);

	rezultatRazlog = combo_razlog->Text;
	rezultatNapomena = edit_napomena->Text.Trim();

	ModalResult = mrOk;
}
//---------------------------------------------------------------------------
