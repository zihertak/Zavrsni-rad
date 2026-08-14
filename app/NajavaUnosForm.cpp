//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "NajavaUnosForm.h"
#include "DatumHelper.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
Tform_najava_unos *form_najava_unos;
//---------------------------------------------------------------------------
__fastcall Tform_najava_unos::Tform_najava_unos(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void Tform_najava_unos::pripremiZaDodavanje()
{
	Caption = L"Dodaj najavu";
	date_datum->Date = Date();
	edit_naziv->Clear();
	memo_opis->Clear();
	combo_vrsta->ItemIndex = 0;
}
//---------------------------------------------------------------------------
void Tform_najava_unos::pripremiZaUredjivanje(String datum, String naziv, String opis, String vrsta)
{
	Caption = L"Uredi najavu";

	date_datum->Date = ParsirajDatum(datum);

	edit_naziv->Text = naziv;
	memo_opis->Text = opis;

	combo_vrsta->ItemIndex = combo_vrsta->Items->IndexOf(vrsta);

	if (combo_vrsta->ItemIndex < 0)
	{
		combo_vrsta->ItemIndex = 0;
	}
}
//---------------------------------------------------------------------------
void __fastcall Tform_najava_unos::button_spremiClick(TObject *Sender)
{
	String naziv = edit_naziv->Text.Trim();

	if (naziv.IsEmpty())
	{
		ShowMessage(L"Unesite naziv najave.");
		return;
	}

	if (combo_vrsta->ItemIndex < 0)
	{
		ShowMessage(L"Odaberite vrstu najave.");
		return;
	}

	String vrsta = combo_vrsta->Text;

	// "Zatvaranje vrtića" tip najave baš i jest namijenjen vikendu/prazniku,
	// za ostale vrste vrijedi upozoriti korisnika da provjeri datum.
	if (kalendarPomoc.JeVikend(DayOfWeek(date_datum->Date)) && vrsta != L"Zatvaranje vrtića")
	{
		int odgovor = MessageDlg(
			L"Odabrani datum je vikend - vrtić tada ne radi. Nastaviti?",
			mtWarning,
			TMsgDlgButtons() << mbYes << mbNo,
			0);

		if (odgovor != mrYes)
		{
			return;
		}
	}

	rezultatDatum = FormatirajDatum(date_datum->Date);
	rezultatNaziv = naziv;
	rezultatOpis = memo_opis->Text.Trim();
	rezultatVrsta = vrsta;

	ModalResult = mrOk;
}
//---------------------------------------------------------------------------
