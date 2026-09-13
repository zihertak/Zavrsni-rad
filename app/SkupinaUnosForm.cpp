//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "SkupinaUnosForm.h"
#include "DatabaseModule.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
Tform_skupina_unos *form_skupina_unos;
//---------------------------------------------------------------------------
__fastcall Tform_skupina_unos::Tform_skupina_unos(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void Tform_skupina_unos::pripremiZaDodavanje()
{
	uredjivanje = false;
	idSkupine = 0;
	edit_naziv->Clear();
    ucitajPedagoskeGodine();
	combo_pedagoska_godina->ItemIndex = -1;
	spin_dob_od->Value = 2;
	spin_dob_do->Value = 2;
}
//---------------------------------------------------------------------------
void __fastcall Tform_skupina_unos::button_spremiClick(
	TObject *Sender)
{
	String naziv = edit_naziv->Text.Trim();

	String pedagoskaGodina =
		combo_pedagoska_godina->Text;

	int dobOd = spin_dob_od->Value;
	int dobDo = spin_dob_do->Value;

	if (naziv.IsEmpty())
	{
		ShowMessage(L"Unesite naziv skupine.");
		edit_naziv->SetFocus();
		return;
	}

	if (combo_pedagoska_godina->ItemIndex == -1)
	{
		ShowMessage(L"Odaberite pedagošku godinu.");
		combo_pedagoska_godina->SetFocus();
		return;
	}

	if (dobOd > dobDo)
	{
		ShowMessage(
			L"Vrijednost 'Dob od' ne može biti veća od vrijednosti 'Dob do'."
		);

		spin_dob_od->SetFocus();
		return;
	}

	try
	{
		if (!uredjivanje)
		{
			query_spremanje->Close();
			query_spremanje->SQL->Clear();

			query_spremanje->SQL->Add(
				"INSERT INTO skupina "
				"(naziv, pedagoska_godina, dob_od, dob_do) "
				"VALUES "
				"(:naziv, :pedagoska_godina, :dob_od, :dob_do)"
			);

			query_spremanje
				->ParamByName("naziv")
				->AsString = naziv;

			query_spremanje
				->ParamByName("pedagoska_godina")
				->AsString = pedagoskaGodina;

			query_spremanje
				->ParamByName("dob_od")
				->AsInteger = dobOd;

			query_spremanje
				->ParamByName("dob_do")
				->AsInteger = dobDo;

			query_spremanje->ExecSQL();

			ModalResult = mrOk;
		}
		else
		{
			query_spremanje->Close();
			query_spremanje->SQL->Clear();

			query_spremanje->SQL->Add(
				"UPDATE skupina SET "
				"naziv = :naziv, "
				"pedagoska_godina = :pedagoska_godina, "
				"dob_od = :dob_od, "
				"dob_do = :dob_do "
				"WHERE id_skupina = :id"
			);

			query_spremanje
				->ParamByName("naziv")
				->AsString = naziv;

			query_spremanje
				->ParamByName("pedagoska_godina")
				->AsString = pedagoskaGodina;

			query_spremanje
				->ParamByName("dob_od")
				->AsInteger = dobOd;

			query_spremanje
				->ParamByName("dob_do")
				->AsInteger = dobDo;

			query_spremanje
				->ParamByName("id")
				->AsInteger = idSkupine;

			query_spremanje->ExecSQL();

			ModalResult = mrOk;
		}
	}
	catch (Exception &e)
	{
		ShowMessage(
			L"Greška prilikom spremanja skupine:\n" +
			e.Message
		);
	}
}
//---------------------------------------------------------------------------

void __fastcall Tform_skupina_unos::button_odustaniClick(TObject *Sender)
{
	ModalResult = mrCancel;
}
//---------------------------------------------------------------------------

void __fastcall Tform_skupina_unos::FormShow(TObject *Sender)
{
    edit_naziv->SetFocus();
}
//---------------------------------------------------------------------------
void Tform_skupina_unos::ucitajPedagoskeGodine()
{
	combo_pedagoska_godina->Items->Clear();

	unsigned short godina;
	unsigned short mjesec;
	unsigned short dan;

	DecodeDate(
		Date(),
		godina,
		mjesec,
		dan
	);

	int pocetnaGodina;

	if (mjesec >= 9)
	{
		pocetnaGodina = godina;
	}
	else
	{
		pocetnaGodina = godina - 1;
	}

	for (int i = -1; i <= 2; i++)
	{
		int prvaGodina = pocetnaGodina + i;
		int drugaGodina = prvaGodina + 1;

		String pedagoskaGodina =
			IntToStr(prvaGodina) +
			L"./" +
			IntToStr(drugaGodina) +
			L".";

		combo_pedagoska_godina
			->Items
			->Add(pedagoskaGodina);
	}

	combo_pedagoska_godina->ItemIndex = -1;
}

void Tform_skupina_unos::pripremiZaUredjivanje(int id)
{
	uredjivanje = true;
	idSkupine = id;

	ucitajPedagoskeGodine();

	query_spremanje->Close();
	query_spremanje->SQL->Clear();

	query_spremanje->SQL->Add(
		"SELECT naziv, pedagoska_godina, dob_od, dob_do "
		"FROM skupina "
		"WHERE id_skupina = :id"
	);

	query_spremanje
		->ParamByName("id")
		->AsInteger = idSkupine;

	query_spremanje->Open();

	if (!query_spremanje->IsEmpty())
	{
		edit_naziv->Text =
			query_spremanje->FieldByName("naziv")->AsString;

		combo_pedagoska_godina->ItemIndex =
			combo_pedagoska_godina->Items->IndexOf(
				query_spremanje
					->FieldByName("pedagoska_godina")
					->AsString
			);

		spin_dob_od->Value =
			query_spremanje->FieldByName("dob_od")->AsInteger;

		spin_dob_do->Value =
			query_spremanje->FieldByName("dob_do")->AsInteger;
	}

	query_spremanje->Close();
}
