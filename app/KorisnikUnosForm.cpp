//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "KorisnikUnosForm.h"
#include "DatabaseModule.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
Tform_korisnik_unos *form_korisnik_unos;
//---------------------------------------------------------------------------
__fastcall Tform_korisnik_unos::Tform_korisnik_unos(TComponent* Owner)
	: TForm(Owner),
	idKorisnik(0)
{
}
//---------------------------------------------------------------------------
void __fastcall Tform_korisnik_unos::button_odustaniClick(TObject *Sender)
{
    ModalResult = mrCancel;
}
//---------------------------------------------------------------------------
void __fastcall Tform_korisnik_unos::combo_uloga_unosChange(TObject *Sender)
{
	postaviDostupnostSkupine();
}
//---------------------------------------------------------------------------
void Tform_korisnik_unos::ucitajUloge()
{
    combo_uloga_unos->Items->Clear();

    query_uloge->Close();
    query_uloge->SQL->Clear();

    query_uloge->SQL->Add(
        L"SELECT id_uloga, naziv "
        L"FROM uloga "
        L"ORDER BY naziv"
    );

    query_uloge->Open();

    while (!query_uloge->Eof)
    {
        String naziv =
            query_uloge
                ->FieldByName(L"naziv")
                ->AsWideString;

        int idUloga =
            query_uloge
                ->FieldByName(L"id_uloga")
                ->AsInteger;

        combo_uloga_unos->Items->AddObject(
            naziv,
            reinterpret_cast<TObject*>(
                static_cast<NativeInt>(idUloga)
            )
        );

        query_uloge->Next();
    }

    query_uloge->Close();
}

void Tform_korisnik_unos::ucitajSkupine()
{
    combo_skupina->Items->Clear();

    query_skupine->Close();
    query_skupine->SQL->Clear();

    query_skupine->SQL->Add(
        L"SELECT id_skupina, naziv "
        L"FROM skupina "
        L"ORDER BY naziv"
    );

    query_skupine->Open();

    while (!query_skupine->Eof)
    {
        String naziv =
            query_skupine
                ->FieldByName(L"naziv")
                ->AsWideString;

        int idSkupina =
            query_skupine
                ->FieldByName(L"id_skupina")
                ->AsInteger;

        combo_skupina->Items->AddObject(
            naziv,
            reinterpret_cast<TObject*>(
                static_cast<NativeInt>(idSkupina)
            )
        );

        query_skupine->Next();
    }

    query_skupine->Close();
}

void Tform_korisnik_unos::postaviDostupnostSkupine()
{
    bool odgojitelj = false;

    if (combo_uloga_unos->ItemIndex >= 0)
    {
        odgojitelj =
            SameText(
                combo_uloga_unos->Text,
                L"Odgojitelj"
            );
    }

    label_skupina->Enabled = odgojitelj;
    combo_skupina->Enabled = odgojitelj;

    if (!odgojitelj)
    {
        combo_skupina->ItemIndex = -1;
    }
}

void Tform_korisnik_unos::pripremiZaDodavanje()
{
    idKorisnik = 0;

    Caption = L"Dodavanje korisnika";

    edit_ime->Clear();
    edit_prezime->Clear();
    edit_username->Clear();
    edit_lozinka->Clear();
    edit_ponovi_lozinku->Clear();

    ucitajUloge();
    ucitajSkupine();

    combo_uloga_unos->ItemIndex = -1;
    combo_skupina->ItemIndex = -1;

    label_lozinka->Caption = L"Lozinka";
    label_ponovi_lozinku->Caption = L"Ponovite lozinku";

    postaviDostupnostSkupine();
}

void Tform_korisnik_unos::pripremiZaUredjivanje(
    int odabraniIdKorisnik
)
{
    idKorisnik = odabraniIdKorisnik;

    Caption = L"Uređivanje korisnika";

    edit_ime->Clear();
    edit_prezime->Clear();
    edit_username->Clear();
    edit_lozinka->Clear();
    edit_ponovi_lozinku->Clear();

    ucitajUloge();
    ucitajSkupine();

    query_korisnik->Close();
    query_korisnik->SQL->Clear();

    query_korisnik->SQL->Add(
        L"SELECT "
        L"    id_korisnik, "
        L"    ime, "
        L"    prezime, "
        L"    username, "
        L"    id_uloga, "
        L"    id_skupina "
        L"FROM korisnik "
        L"WHERE id_korisnik = :id_korisnik"
    );

    query_korisnik
        ->ParamByName(L"id_korisnik")
        ->AsInteger = idKorisnik;

    query_korisnik->Open();

    if (query_korisnik->IsEmpty())
    {
        query_korisnik->Close();

        ShowMessage(
            L"Odabrani korisnik više ne postoji."
        );

        return;
    }

    edit_ime->Text =
        query_korisnik
            ->FieldByName(L"ime")
            ->AsWideString;

    edit_prezime->Text =
        query_korisnik
            ->FieldByName(L"prezime")
            ->AsWideString;

    edit_username->Text =
        query_korisnik
            ->FieldByName(L"username")
            ->AsWideString;

    int idUloga =
        query_korisnik
            ->FieldByName(L"id_uloga")
            ->AsInteger;

    combo_uloga_unos->ItemIndex =
        pronadiStavkuPoID(
            combo_uloga_unos,
            idUloga
        );

    if (
        query_korisnik
            ->FieldByName(L"id_skupina")
            ->IsNull
    )
    {
        combo_skupina->ItemIndex = -1;
    }
    else
    {
        int idSkupina =
            query_korisnik
                ->FieldByName(L"id_skupina")
                ->AsInteger;

        combo_skupina->ItemIndex =
            pronadiStavkuPoID(
                combo_skupina,
                idSkupina
            );
    }

    query_korisnik->Close();

    label_lozinka->Caption =
        L"Nova lozinka";

    label_ponovi_lozinku->Caption =
        L"Ponovite novu lozinku";

	postaviDostupnostSkupine();
}

int Tform_korisnik_unos::pronadiStavkuPoID(
	TComboBox *combo,
	int trazeniID
)
{
    for (int i = 0; i < combo->Items->Count; i++)
    {
        NativeInt spremljeniID =
            reinterpret_cast<NativeInt>(
                combo->Items->Objects[i]
            );

        if (spremljeniID == trazeniID)
        {
            return i;
        }
    }

    return -1;
}
void __fastcall Tform_korisnik_unos::button_spremiClick(TObject *Sender)
{
    String ime = edit_ime->Text.Trim();
    String prezime = edit_prezime->Text.Trim();
    String username = edit_username->Text.Trim();
    String lozinka = edit_lozinka->Text;
    String ponovljenaLozinka = edit_ponovi_lozinku->Text;

    if (ime.IsEmpty())
    {
        ShowMessage(L"Unesite ime korisnika.");
        edit_ime->SetFocus();
        return;
    }

    if (prezime.IsEmpty())
    {
        ShowMessage(L"Unesite prezime korisnika.");
        edit_prezime->SetFocus();
        return;
    }

    if (username.IsEmpty())
    {
        ShowMessage(L"Unesite korisničko ime.");
        edit_username->SetFocus();
        return;
    }

    if (combo_uloga_unos->ItemIndex < 0)
    {
        ShowMessage(L"Odaberite ulogu korisnika.");
        combo_uloga_unos->SetFocus();
        return;
    }

    bool dodavanje = idKorisnik == 0;

    if (dodavanje && lozinka.IsEmpty())
    {
        ShowMessage(L"Unesite lozinku.");
        edit_lozinka->SetFocus();
        return;
    }

    if (!lozinka.IsEmpty() && lozinka.Length() < 6)
    {
        ShowMessage(
            L"Lozinka mora sadržavati najmanje 6 znakova."
        );
        edit_lozinka->SetFocus();
        return;
    }

    if (lozinka != ponovljenaLozinka)
    {
        ShowMessage(L"Lozinke nisu jednake.");
        edit_ponovi_lozinku->SetFocus();
        return;
    }

    bool odgojitelj = SameText(
        combo_uloga_unos->Text,
        L"Odgojitelj"
    );

    if (
        odgojitelj &&
        combo_skupina->ItemIndex < 0
    )
    {
        ShowMessage(
            L"Odgojitelju morate odabrati skupinu."
        );
        combo_skupina->SetFocus();
        return;
	}
	query_provjera->Close();
	query_provjera->SQL->Clear();

	query_provjera->SQL->Add(
		L"SELECT COUNT(*) AS broj "
		L"FROM korisnik "
		L"WHERE LOWER(username) = LOWER(:username) "
		L"AND id_korisnik <> :id_korisnik"
	);

	query_provjera
		->ParamByName(L"username")
		->AsWideString = username;

	query_provjera
		->ParamByName(L"id_korisnik")
		->AsInteger = idKorisnik;

	query_provjera->Open();

	int brojPostojecih =
		query_provjera
			->FieldByName(L"broj")
			->AsInteger;

	query_provjera->Close();

	if (brojPostojecih > 0)
	{
		ShowMessage(
			L"Korisničko ime već postoji. "
			L"Odaberite drugo korisničko ime."
		);

		edit_username->SetFocus();
		edit_username->SelectAll();
		return;
	}
    NativeInt idUloga =
    reinterpret_cast<NativeInt>(
        combo_uloga_unos->Items->Objects[
            combo_uloga_unos->ItemIndex
        ]
    );

	NativeInt idSkupina = 0;

	if (
		odgojitelj &&
		combo_skupina->ItemIndex >= 0
	)
	{
		idSkupina =
			reinterpret_cast<NativeInt>(
				combo_skupina->Items->Objects[
					combo_skupina->ItemIndex
				]
			);
	}
    try
{
    query_spremanje->Close();
    query_spremanje->SQL->Clear();

    if (dodavanje)
    {
        query_spremanje->SQL->Add(
            L"INSERT INTO korisnik ("
            L"    ime, "
            L"    prezime, "
            L"    username, "
            L"    lozinka, "
            L"    id_uloga, "
            L"    id_skupina"
            L") VALUES ("
            L"    :ime, "
            L"    :prezime, "
            L"    :username, "
            L"    :lozinka, "
            L"    :id_uloga, "
            L"    :id_skupina"
            L")"
        );

        query_spremanje
            ->ParamByName(L"lozinka")
            ->AsWideString = lozinka;
	}
        else
    {
        if (lozinka.IsEmpty())
        {
            query_spremanje->SQL->Add(
                L"UPDATE korisnik SET "
                L"    ime = :ime, "
                L"    prezime = :prezime, "
                L"    username = :username, "
                L"    id_uloga = :id_uloga, "
                L"    id_skupina = :id_skupina "
                L"WHERE id_korisnik = :id_korisnik"
            );
        }
        else
        {
            query_spremanje->SQL->Add(
                L"UPDATE korisnik SET "
                L"    ime = :ime, "
                L"    prezime = :prezime, "
                L"    username = :username, "
                L"    lozinka = :lozinka, "
                L"    id_uloga = :id_uloga, "
                L"    id_skupina = :id_skupina "
                L"WHERE id_korisnik = :id_korisnik"
            );

            query_spremanje
                ->ParamByName(L"lozinka")
                ->AsWideString = lozinka;
        }

        query_spremanje
            ->ParamByName(L"id_korisnik")
            ->AsInteger = idKorisnik;
	}
	query_spremanje
        ->ParamByName(L"ime")
        ->AsWideString = ime;

	query_spremanje
        ->ParamByName(L"prezime")
        ->AsWideString = prezime;

    query_spremanje
        ->ParamByName(L"username")
        ->AsWideString = username;

    query_spremanje
        ->ParamByName(L"id_uloga")
        ->AsInteger =
		static_cast<int>(idUloga);

	if (odgojitelj)
    {
        query_spremanje
            ->ParamByName(L"id_skupina")
            ->AsInteger =
            static_cast<int>(idSkupina);
    }
    else
    {
        TFDParam *paramSkupina =
        query_spremanje->ParamByName(L"id_skupina");
		paramSkupina->DataType = ftInteger;
		paramSkupina->Clear();
	}

        query_spremanje->ExecSQL();

    if (dodavanje)
    {
        ShowMessage(
            L"Korisnik je uspješno dodan."
        );
    }
    else
    {
        ShowMessage(
            L"Podaci korisnika uspješno su spremljeni."
        );
    }

    ModalResult = mrOk;
	}
	catch (const Exception &e)
	{
		ShowMessage(
			L"Došlo je do pogreške pri spremanju korisnika:\n" +
			e.Message
		);
	}
}
//---------------------------------------------------------------------------
