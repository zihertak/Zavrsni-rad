//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "RazvojForm.h"
#include "DashboardForm.h"
#include "DjecaForm.h"
#include "SkupineForm.h"
#include "AktivnostiForm.h"
#include "LoginForm.h"
#include "DatabaseModule.h"
#include "RazvojDetaljiForm.h"
#include "AktivnostUnosForm.h"
#include "KorisnickaPrava.h"
#include "KorisniciForm.h"
#include "UputeForm.h"
#include "BolestiForm.h"
#include "NajaveForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
Tform_razvoj *form_razvoj;
//---------------------------------------------------------------------------
__fastcall Tform_razvoj::Tform_razvoj(TComponent* Owner)
	: TForm(Owner),
	odabranoDijeteID(0),
	ocjenaTjelesni(0),
	ocjenaSocio(0),
	ocjenaGovorni(0),
	ocjenaSpoznajni(0)
{
    generiraniPregledID = 0;
	preporukaGenerirana = false;
}
//---------------------------------------------------------------------------
void __fastcall Tform_razvoj::button_pocetnaClick(TObject *Sender)
{
	form_dashboard->Show();
	this->Hide();
}
//---------------------------------------------------------------------------
void __fastcall Tform_razvoj::button_djecaClick(TObject *Sender)
{
	form_djeca->Show();
	this->Hide();
}
//---------------------------------------------------------------------------
void __fastcall Tform_razvoj::button_aktivnostiClick(TObject *Sender)
{
    form_aktivnosti->Show();
    this->Hide();
}
//---------------------------------------------------------------------------
void __fastcall Tform_razvoj::button_skupineClick(TObject *Sender)
{
	form_skupine->Show();
	this->Hide();
}
//---------------------------------------------------------------------------
void __fastcall Tform_razvoj::FormShow(TObject *Sender)
{
	label_korisnik->Caption =
		data_module->currentUserIme +
		L" " +
		data_module->currentUserPrezime;

	label_uloga->Caption =
		data_module->currentUserUloga;

	ucitajDjecu();
    primijeniPrava();
}
//---------------------------------------------------------------------------
void __fastcall Tform_razvoj::button_odjavaClick(TObject *Sender)
{
    data_module->currentUserID = 0;
    data_module->currentUserIme = "";
    data_module->currentUserPrezime = "";
    data_module->currentUserUloga = "";
    data_module->currentUserSkupinaID = 0;

    form_Login->edit_username->Clear();
    form_Login->edit_password->Clear();

    form_Login->Show();
    form_Login->BringToFront();

    this->Hide();
    form_dashboard->Hide();
    form_Login->edit_username->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall Tform_razvoj::FormClose(TObject *Sender, TCloseAction &Action)
{
     Application->Terminate();
}
//---------------------------------------------------------------------------
void Tform_razvoj::ucitajDjecu()
{
    combo_dijete->Items->Clear();

    query_djeca->Close();
    query_djeca->SQL->Clear();

    TKorisnickaPrava prava(data_module->currentUserUloga);

    query_djeca->SQL->Add(
        "SELECT "
        "id_dijete, "
        "ime, "
        "prezime "
        "FROM dijete "
        "WHERE 1 = 1 "
    );

    if (prava.samoSvojaSkupina())
    {
        query_djeca->SQL->Add(
            "AND id_skupina = :id_skupina "
        );
    }

    query_djeca->SQL->Add(
        "ORDER BY prezime ASC, ime ASC"
    );

    if (prava.samoSvojaSkupina())
    {
        query_djeca
            ->ParamByName("id_skupina")
            ->AsInteger =
                data_module->currentUserSkupinaID;
    }

    query_djeca->Open();

    while (!query_djeca->Eof)
    {
        int idDijete =
            query_djeca
                ->FieldByName("id_dijete")
                ->AsInteger;

        String punoIme =
            query_djeca
                ->FieldByName("ime")
                ->AsString +
            L" " +
            query_djeca
                ->FieldByName("prezime")
                ->AsString;

        combo_dijete->Items->AddObject(
            punoIme,
            reinterpret_cast<TObject*>(
                static_cast<NativeInt>(idDijete)
            )
        );

        query_djeca->Next();
    }

    if (combo_dijete->Items->Count > 0)
    {
        combo_dijete->ItemIndex = 0;

        odabranoDijeteID =
            static_cast<int>(
                reinterpret_cast<NativeInt>(
                    combo_dijete->Items->Objects[0]
                )
            );

        ucitajPodatkeDjeteta();
		ucitajPovijest();
        ucitajSpremljenuPreporuku();
    }
    else
    {
        combo_dijete->ItemIndex = -1;
        odabranoDijeteID = 0;

        ocistiPodatkeDjeteta();

        query_povijest->Close();
    }
}

void Tform_razvoj::ocistiPodatkeDjeteta()
{
	odabranoDijeteID = 0;
    label_skupina->Caption = L"-";
    label_dob->Caption = L"-";
    label_zadnji_pregled->Caption = L"Nema pregleda";
}

void Tform_razvoj::ucitajPodatkeDjeteta()
{
    if (odabranoDijeteID == 0)
    {
        ocistiPodatkeDjeteta();
        return;
    }

    query_djeca->Close();
    query_djeca->SQL->Clear();

    query_djeca->SQL->Add(
        "SELECT "
        "d.datum_rodjenja, "
        "s.naziv AS naziv_skupine, "
        "("
        "SELECT MAX(rp.datum) "
        "FROM razvojni_pregled rp "
        "WHERE rp.id_dijete = d.id_dijete"
        ") AS zadnji_pregled "
        "FROM dijete d "
        "LEFT JOIN skupina s "
        "ON s.id_skupina = d.id_skupina "
        "WHERE d.id_dijete = :id_dijete"
    );

    query_djeca
        ->ParamByName("id_dijete")
        ->AsInteger = odabranoDijeteID;

    query_djeca->Open();

    if (query_djeca->IsEmpty())
    {
        ocistiPodatkeDjeteta();
        return;
    }

    if (query_djeca->FieldByName("naziv_skupine")->IsNull)
    {
        label_skupina->Caption = L"Nije dodijeljena";
    }
    else
    {
        label_skupina->Caption =
            query_djeca
                ->FieldByName("naziv_skupine")
                ->AsString;
    }

    if (query_djeca->FieldByName("datum_rodjenja")->IsNull)
    {
        label_dob->Caption = L"-";
    }
    else
    {
        TDateTime datumRodjenja =
            query_djeca
                ->FieldByName("datum_rodjenja")
                ->AsDateTime;

        unsigned short godinaRodjenja;
        unsigned short mjesecRodjenja;
        unsigned short danRodjenja;

        unsigned short trenutnaGodina;
        unsigned short trenutniMjesec;
        unsigned short trenutniDan;

        datumRodjenja.DecodeDate(
            &godinaRodjenja,
            &mjesecRodjenja,
            &danRodjenja
        );

        Date().DecodeDate(
            &trenutnaGodina,
            &trenutniMjesec,
            &trenutniDan
        );

        int dob = trenutnaGodina - godinaRodjenja;

        if (
            trenutniMjesec < mjesecRodjenja ||
            (
                trenutniMjesec == mjesecRodjenja &&
                trenutniDan < danRodjenja
            )
        )
        {
            dob--;
        }

        label_dob->Caption =
            IntToStr(dob) + L" godina";
    }

    if (query_djeca->FieldByName("zadnji_pregled")->IsNull)
    {
        label_zadnji_pregled->Caption =
            L"Nema pregleda";
    }
    else
    {
        label_zadnji_pregled->Caption =
            FormatDateTime(
                L"dd.mm.yyyy.",
                query_djeca
                    ->FieldByName("zadnji_pregled")
                    ->AsDateTime
            );
    }
}
void __fastcall Tform_razvoj::combo_dijeteChange(TObject *Sender)
{
	if (combo_dijete->ItemIndex < 0)
    {
        ocistiPodatkeDjeteta();
        return;
    }

    odabranoDijeteID =
        static_cast<int>(
            reinterpret_cast<NativeInt>(
                combo_dijete->Items->Objects[
                    combo_dijete->ItemIndex
                ]
            )
        );
	ocistiPrikazPreporuke();

	ucitajPodatkeDjeteta();
	ucitajPovijest();
    ucitajSpremljenuPreporuku();
}
//---------------------------------------------------------------------------
void Tform_razvoj::OdaberiDijeteID(int idDijete)
{
	for (int i = 0; i < combo_dijete->Items->Count; i++)
	{
		int trenutniID =
			static_cast<int>(
				reinterpret_cast<NativeInt>(
					combo_dijete->Items->Objects[i]
				)
			);

		if (trenutniID == idDijete)
		{
			combo_dijete->ItemIndex = i;

			odabranoDijeteID = idDijete;
			ocistiPrikazPreporuke();

			ucitajPodatkeDjeteta();
			ucitajPovijest();
			ucitajSpremljenuPreporuku();

			break;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall Tform_razvoj::button_izvezi_profilClick(TObject *Sender)
{
	if (odabranoDijeteID == 0)
	{
		ShowMessage(L"Odaberite dijete čiji profil želite izvesti.");
		return;
	}

	if (!save_dialog_profil->Execute())
	{
		return;
	}

	String punoIme = combo_dijete->Text;

	while (punoIme.Length() < 100)
	{
		punoIme = punoIme + L" ";
	}

	punoIme = punoIme.SubString(1, 100);

	query_izvoz_pregledi->Close();
	query_izvoz_pregledi->SQL->Clear();

	query_izvoz_pregledi->SQL->Add(
		"SELECT COUNT(*) AS broj FROM razvojni_pregled WHERE id_dijete = :id_dijete"
	);

	query_izvoz_pregledi->ParamByName("id_dijete")->AsInteger = odabranoDijeteID;
	query_izvoz_pregledi->Open();

	int brojPregleda = query_izvoz_pregledi->FieldByName("broj")->AsInteger;
	query_izvoz_pregledi->Close();

	query_izvoz_pregledi->SQL->Clear();

	query_izvoz_pregledi->SQL->Add(
		"SELECT id_razvojni_pregled, datum FROM razvojni_pregled "
		"WHERE id_dijete = :id_dijete ORDER BY datum ASC"
	);

	query_izvoz_pregledi->ParamByName("id_dijete")->AsInteger = odabranoDijeteID;
	query_izvoz_pregledi->Open();

	TFileStream *stream = new TFileStream(save_dialog_profil->FileName, fmCreate);

	try
	{
		stream->WriteBuffer(punoIme.c_str(), 100 * sizeof(wchar_t));
		stream->WriteBuffer(&brojPregleda, sizeof(int));

		while (!query_izvoz_pregledi->Eof)
		{
			int idPregled =
				query_izvoz_pregledi->FieldByName("id_razvojni_pregled")->AsInteger;

			double datum = query_izvoz_pregledi->FieldByName("datum")->AsDateTime;
			stream->WriteBuffer(&datum, sizeof(double));

			query_izvoz_ocjene->Close();
			query_izvoz_ocjene->SQL->Clear();

			query_izvoz_ocjene->SQL->Add(
				"SELECT id_podrucje_razvoja, razina FROM procjena_podrucja "
				"WHERE id_razvojni_pregled = :id_pregled"
			);

			query_izvoz_ocjene->ParamByName("id_pregled")->AsInteger = idPregled;
			query_izvoz_ocjene->Open();

			query_izvoz_ocjene->Last();
			int brojOcjena = query_izvoz_ocjene->RecordCount;
			query_izvoz_ocjene->First();

			stream->WriteBuffer(&brojOcjena, sizeof(int));

			while (!query_izvoz_ocjene->Eof)
			{
				int idPodrucje =
					query_izvoz_ocjene->FieldByName("id_podrucje_razvoja")->AsInteger;

				int razina = query_izvoz_ocjene->FieldByName("razina")->AsInteger;

				stream->WriteBuffer(&idPodrucje, sizeof(int));
				stream->WriteBuffer(&razina, sizeof(int));

				query_izvoz_ocjene->Next();
			}

			query_izvoz_ocjene->Close();
			query_izvoz_pregledi->Next();
		}

		ShowMessage(
			L"Profil je izvezen (" + IntToStr(brojPregleda) + L" pregleda).");
	}
	__finally
	{
		delete stream;
		query_izvoz_pregledi->Close();
	}
}
//---------------------------------------------------------------------------

void __fastcall Tform_razvoj::speed_tjelesniClick(TObject *Sender)
{
    TSpeedButton *gumb =
        dynamic_cast<TSpeedButton*>(Sender);

    if (gumb == nullptr)
    {
        return;
    }

    ocjenaTjelesni = gumb->Tag;

	osvjeziTjelesneZvjezdice();
}
//---------------------------------------------------------------------------
void Tform_razvoj::osvjeziTjelesneZvjezdice()
{
    TColor boja = razvojPomoc.BojaZaOcjenu(ocjenaTjelesni);

    speed_tjelesni_1->Caption =
        ocjenaTjelesni >= 1 ? L"★" : L"☆";

    speed_tjelesni_2->Caption =
        ocjenaTjelesni >= 2 ? L"★" : L"☆";

    speed_tjelesni_3->Caption =
        ocjenaTjelesni >= 3 ? L"★" : L"☆";

    speed_tjelesni_4->Caption =
        ocjenaTjelesni >= 4 ? L"★" : L"☆";

    speed_tjelesni_5->Caption =
        ocjenaTjelesni >= 5 ? L"★" : L"☆";

	speed_tjelesni_1->Font->Color =
		ocjenaTjelesni >= 1 ? boja : clGray;

	speed_tjelesni_2->Font->Color =
		ocjenaTjelesni >= 2 ? boja : clGray;

	speed_tjelesni_3->Font->Color =
		ocjenaTjelesni >= 3 ? boja : clGray;

	speed_tjelesni_4->Font->Color =
		ocjenaTjelesni >= 4 ? boja : clGray;

	speed_tjelesni_5->Font->Color =
		ocjenaTjelesni >= 5 ? boja : clGray;
}
void __fastcall Tform_razvoj::speed_socioClick(TObject *Sender)
{
     TSpeedButton *gumb =
        dynamic_cast<TSpeedButton*>(Sender);

    if (gumb == nullptr)
    {
        return;
    }

    ocjenaSocio = gumb->Tag;
    osvjeziSocioZvjezdice();
}
//---------------------------------------------------------------------------

void __fastcall Tform_razvoj::speed_govorniClick(TObject *Sender)
{
     TSpeedButton *gumb =
        dynamic_cast<TSpeedButton*>(Sender);

    if (gumb == nullptr)
    {
        return;
    }

    ocjenaGovorni = gumb->Tag;
	osvjeziGovorneZvjezdice();
}
//---------------------------------------------------------------------------

void __fastcall Tform_razvoj::speed_spoznajniClick(TObject *Sender)
{
    TSpeedButton *gumb =
        dynamic_cast<TSpeedButton*>(Sender);

    if (gumb == nullptr)
    {
        return;
    }

    ocjenaSpoznajni = gumb->Tag;
	osvjeziSpoznajneZvjezdice();
}
//---------------------------------------------------------------------------
void Tform_razvoj::osvjeziSocioZvjezdice()
{
	TColor boja = razvojPomoc.BojaZaOcjenu(ocjenaSocio);
    speed_socio_1->Caption =
        ocjenaSocio >= 1 ? L"★" : L"☆";

    speed_socio_2->Caption =
        ocjenaSocio >= 2 ? L"★" : L"☆";

    speed_socio_3->Caption =
        ocjenaSocio >= 3 ? L"★" : L"☆";

    speed_socio_4->Caption =
        ocjenaSocio >= 4 ? L"★" : L"☆";

    speed_socio_5->Caption =
		ocjenaSocio >= 5 ? L"★" : L"☆";

	speed_socio_1->Font->Color =
		ocjenaSocio >= 1 ? boja : clGray;

	speed_socio_2->Font->Color =
		ocjenaSocio >= 2 ? boja : clGray;

	speed_socio_3->Font->Color =
		ocjenaSocio >= 3 ? boja : clGray;

	speed_socio_4->Font->Color =
		ocjenaSocio >= 4 ? boja : clGray;

	speed_socio_5->Font->Color =
		ocjenaSocio >= 5 ? boja : clGray;
}

void Tform_razvoj::osvjeziGovorneZvjezdice()
{
	TColor boja = razvojPomoc.BojaZaOcjenu(ocjenaGovorni);
	speed_govorni_1->Caption =
        ocjenaGovorni >= 1 ? L"★" : L"☆";

    speed_govorni_2->Caption =
        ocjenaGovorni >= 2 ? L"★" : L"☆";

    speed_govorni_3->Caption =
        ocjenaGovorni >= 3 ? L"★" : L"☆";

    speed_govorni_4->Caption =
        ocjenaGovorni >= 4 ? L"★" : L"☆";

    speed_govorni_5->Caption =
		ocjenaGovorni >= 5 ? L"★" : L"☆";

	speed_govorni_1->Font->Color =
		ocjenaGovorni >= 1 ? boja : clGray;

	speed_govorni_2->Font->Color =
		ocjenaGovorni >= 2 ? boja : clGray;

	speed_govorni_3->Font->Color =
		ocjenaGovorni >= 3 ? boja : clGray;

	speed_govorni_4->Font->Color =
		ocjenaGovorni >= 4 ? boja : clGray;

	speed_govorni_5->Font->Color =
		ocjenaGovorni >= 5 ? boja : clGray;
}

void Tform_razvoj::osvjeziSpoznajneZvjezdice()
{
	TColor boja = razvojPomoc.BojaZaOcjenu(ocjenaSpoznajni);
    speed_spoznajni_1->Caption =
        ocjenaSpoznajni >= 1 ? L"★" : L"☆";

    speed_spoznajni_2->Caption =
        ocjenaSpoznajni >= 2 ? L"★" : L"☆";

    speed_spoznajni_3->Caption =
        ocjenaSpoznajni >= 3 ? L"★" : L"☆";

    speed_spoznajni_4->Caption =
        ocjenaSpoznajni >= 4 ? L"★" : L"☆";

    speed_spoznajni_5->Caption =
		ocjenaSpoznajni >= 5 ? L"★" : L"☆";

	speed_spoznajni_1->Font->Color =
		ocjenaSpoznajni >= 1 ? boja : clGray;

	speed_spoznajni_2->Font->Color =
		ocjenaSpoznajni >= 2 ? boja : clGray;

	speed_spoznajni_3->Font->Color =
		ocjenaSpoznajni >= 3 ? boja : clGray;

	speed_spoznajni_4->Font->Color =
		ocjenaSpoznajni >= 4 ? boja : clGray;

	speed_spoznajni_5->Font->Color =
		ocjenaSpoznajni >= 5 ? boja : clGray;
}

void __fastcall Tform_razvoj::button_ocistiClick(TObject *Sender)
{
    ocjenaTjelesni = 0;
    ocjenaSocio = 0;
    ocjenaGovorni = 0;
    ocjenaSpoznajni = 0;

    memo_napomena->Clear();

    osvjeziTjelesneZvjezdice();
	osvjeziSocioZvjezdice();
    osvjeziGovorneZvjezdice();
	osvjeziSpoznajneZvjezdice();
}
//---------------------------------------------------------------------------

void __fastcall Tform_razvoj::button_spremiClick(TObject *Sender)
{
    TKorisnickaPrava prava(data_module->currentUserUloga);

	if (!prava.unesiRazvojniPregled())
	{
		ShowMessage(L"Nemate pravo spremanja razvojnog pregleda.");
		return;
	}
		if (odabranoDijeteID == 0)
    {
        ShowMessage(
            L"Odaberite dijete."
        );

        return;
    }

    if (
        ocjenaTjelesni == 0 ||
        ocjenaSocio == 0 ||
        ocjenaGovorni == 0 ||
		ocjenaSpoznajni == 0
	)
	{
		ShowMessage(
			L"Potrebno je procijeniti sva područja razvoja."
		);

		return;
	}

	query_spremi_pregled->Close();
	query_spremi_pregled->SQL->Clear();

	query_spremi_pregled->SQL->Add(
		"SELECT COUNT(*) AS broj "
		"FROM razvojni_pregled "
		"WHERE id_dijete = :id_dijete "
		"AND datum = :datum"
	);

	query_spremi_pregled
		->ParamByName("id_dijete")
		->AsInteger =
			odabranoDijeteID;

	query_spremi_pregled
		->ParamByName("datum")
		->AsDate =
			Date();

	query_spremi_pregled->Open();

	if (
		query_spremi_pregled
			->FieldByName("broj")
			->AsInteger > 0
	)
	{
		ShowMessage(
			L"Za odabrano dijete već postoji razvojni pregled za današnji datum."
		);

		query_spremi_pregled->Close();
		return;
	}

	query_spremi_pregled->Close();

    try
    {
        data_module->connection->StartTransaction();

        query_spremi_pregled->Close();
        query_spremi_pregled->SQL->Clear();

        query_spremi_pregled->SQL->Add(
            "INSERT INTO razvojni_pregled "
            "("
                "datum, "
                "napomena, "
                "id_korisnik, "
                "id_dijete"
            ") "
            "VALUES "
            "("
                ":datum, "
                ":napomena, "
                ":id_korisnik, "
                ":id_dijete"
            ")"
        );

        query_spremi_pregled
            ->ParamByName("datum")
            ->AsDate = Date();

        query_spremi_pregled
            ->ParamByName("napomena")
            ->AsString =
                memo_napomena->Text.Trim();

        query_spremi_pregled
            ->ParamByName("id_korisnik")
            ->AsInteger =
                data_module->currentUserID;

        query_spremi_pregled
            ->ParamByName("id_dijete")
            ->AsInteger =
                odabranoDijeteID;

        query_spremi_pregled->ExecSQL();

        query_spremi_pregled->Close();
        query_spremi_pregled->SQL->Clear();

        query_spremi_pregled->SQL->Add(
            "SELECT LAST_INSERT_ID() AS id_razvojni_pregled"
        );

        query_spremi_pregled->Open();

        int noviPregledID =
            query_spremi_pregled
                ->FieldByName("id_razvojni_pregled")
                ->AsInteger;

        query_spremi_pregled->Close();

        query_spremi_procjenu->Close();
        query_spremi_procjenu->SQL->Clear();

        query_spremi_procjenu->SQL->Add(
            "INSERT INTO procjena_podrucja "
            "("
                "razina, "
                "napomena, "
                "id_podrucje_razvoja, "
                "id_razvojni_pregled"
            ") "
            "VALUES "
            "("
                ":razina, "
                ":napomena, "
                ":id_podrucje, "
                ":id_pregled"
            ")"
        );

		query_spremi_procjenu
			->ParamByName("napomena")
			->AsString = "";

        query_spremi_procjenu
            ->ParamByName("id_pregled")
            ->AsInteger =
                noviPregledID;

        query_spremi_procjenu
            ->ParamByName("razina")
            ->AsInteger =
                ocjenaTjelesni;

        query_spremi_procjenu
            ->ParamByName("id_podrucje")
            ->AsInteger = 1;

        query_spremi_procjenu->ExecSQL();

        query_spremi_procjenu
            ->ParamByName("razina")
            ->AsInteger =
                ocjenaSocio;

        query_spremi_procjenu
            ->ParamByName("id_podrucje")
            ->AsInteger = 2;

        query_spremi_procjenu->ExecSQL();

        query_spremi_procjenu
            ->ParamByName("razina")
            ->AsInteger =
                ocjenaGovorni;

        query_spremi_procjenu
            ->ParamByName("id_podrucje")
            ->AsInteger = 3;

        query_spremi_procjenu->ExecSQL();

        query_spremi_procjenu
            ->ParamByName("razina")
            ->AsInteger =
                ocjenaSpoznajni;

        query_spremi_procjenu
            ->ParamByName("id_podrucje")
            ->AsInteger = 4;

        query_spremi_procjenu->ExecSQL();

		data_module->connection->Commit();

        button_ocistiClick(nullptr);
		ucitajPodatkeDjeteta();
        ucitajPovijest();

        ShowMessage(
            L"Razvojni pregled uspješno je spremljen."
        );
    }
    catch (const Exception &e)
    {
        if (data_module->connection->InTransaction)
        {
            data_module->connection->Rollback();
        }

        ShowMessage(
            L"Greška prilikom spremanja razvojnog pregleda:\n" +
            e.Message
        );
    }
}
//---------------------------------------------------------------------------
void Tform_razvoj::ucitajPovijest()
{
    query_povijest->Close();
    query_povijest->SQL->Clear();

    if (odabranoDijeteID == 0)
    {
        return;
    }

    query_povijest->SQL->Add(
        "SELECT "
        "rp.id_razvojni_pregled, "
        "rp.datum, "
        "CONCAT(k.ime, ' ', k.prezime) AS odgojitelj, "
        "ROUND(AVG(pp.razina), 2) AS prosjek, "
		"CAST(rp.napomena AS CHAR(255)) AS napomena "
        "FROM razvojni_pregled rp "
        "INNER JOIN korisnik k "
        "ON k.id_korisnik = rp.id_korisnik "
        "INNER JOIN procjena_podrucja pp "
        "ON pp.id_razvojni_pregled = rp.id_razvojni_pregled "
        "WHERE rp.id_dijete = :id_dijete "
        "GROUP BY "
        "rp.id_razvojni_pregled, "
        "rp.datum, "
        "k.ime, "
        "k.prezime, "
        "rp.napomena "
        "ORDER BY rp.datum DESC, rp.id_razvojni_pregled DESC"
    );

    query_povijest
        ->ParamByName("id_dijete")
        ->AsInteger =
            odabranoDijeteID;

    query_povijest->Open();

    postaviIzgledPovijesti();
}

void Tform_razvoj::postaviIzgledPovijesti()
{
    query_povijest
        ->FieldByName("id_razvojni_pregled")
        ->Visible = false;

    query_povijest
        ->FieldByName("datum")
        ->DisplayLabel = L"Datum";

    query_povijest
        ->FieldByName("datum")
        ->DisplayWidth = 12;

    query_povijest
        ->FieldByName("odgojitelj")
        ->DisplayLabel = L"Odgojitelj";

    query_povijest
        ->FieldByName("odgojitelj")
        ->DisplayWidth = 24;

    query_povijest
        ->FieldByName("prosjek")
        ->DisplayLabel = L"Prosjek";

    query_povijest
        ->FieldByName("prosjek")
        ->DisplayWidth = 10;

    query_povijest
        ->FieldByName("napomena")
        ->DisplayLabel = L"Napomena";

    query_povijest
        ->FieldByName("napomena")
        ->DisplayWidth = 40;
}
void __fastcall Tform_razvoj::grid_povijestDblClick(TObject *Sender)
{
    if (query_povijest->IsEmpty())
    {
        return;
    }

    int idRazvojniPregled =
        query_povijest
            ->FieldByName("id_razvojni_pregled")
            ->AsInteger;

	form_razvoj_detalji
        ->ucitajPregled(idRazvojniPregled);

    form_razvoj_detalji->ShowModal();
}
//---------------------------------------------------------------------------
bool Tform_razvoj::ucitajPregledeZaPreporuke(
    TFDConnection *konekcija,
    int idDijete,
    int &zadnjiPregledID,
    int &prethodniPregledID,
    TDateTime &datumZadnjeg,
    bool &postojiPrethodni)
{
    zadnjiPregledID = 0;
    prethodniPregledID = 0;
    datumZadnjeg = 0;
    postojiPrethodni = false;

    TFDQuery *query = new TFDQuery(NULL);
    query->Connection = konekcija;

    try
    {
        query->SQL->Add(
            L"SELECT id_razvojni_pregled, datum "
            L"FROM razvojni_pregled "
            L"WHERE id_dijete = :id_dijete "
            L"ORDER BY datum DESC, id_razvojni_pregled DESC "
            L"LIMIT 2"
        );

        query->ParamByName(L"id_dijete")->AsInteger = idDijete;

        query->Open();

        if (query->IsEmpty())
        {
            return false;
        }

        zadnjiPregledID =
            query->FieldByName(L"id_razvojni_pregled")->AsInteger;

        datumZadnjeg =
            query->FieldByName(L"datum")->AsDateTime;

        query->Next();

        if (!query->Eof)
        {
            prethodniPregledID =
                query->FieldByName(L"id_razvojni_pregled")->AsInteger;

            postojiPrethodni = true;
        }

        return true;
    }
    __finally
    {
        delete query;
    }
}
//---------------------------------------------------------------------------
String Tform_razvoj::odrediTrend(
    int trenutnaRazina,
    int prethodnaRazina,
    bool postojiPrethodna)
{
    if (!postojiPrethodna)
    {
        return L"Prvi pregled";
    }

    if (trenutnaRazina > prethodnaRazina)
    {
        return L"Rast";
    }

    if (trenutnaRazina < prethodnaRazina)
    {
        return L"Pad";
    }

    return L"Stabilno";
}

std::vector<Tform_razvoj::TAnalizaPodrucja> Tform_razvoj::ucitajAnalizePodrucja(
    TFDConnection *konekcija,
    int zadnjiPregledID,
    int prethodniPregledID,
    bool postojiPrethodni)
{
    std::vector<TAnalizaPodrucja> analize;

    int trenutneRazine[5] = {0, 0, 0, 0, 0};
    int prethodneRazine[5] = {0, 0, 0, 0, 0};

    TFDQuery *query = new TFDQuery(NULL);
    query->Connection = konekcija;

    try
    {
        query->SQL->Add(
            L"SELECT "
            L"id_podrucje_razvoja, "
            L"razina "
            L"FROM procjena_podrucja "
            L"WHERE id_razvojni_pregled = :id_pregled"
        );

        query->ParamByName(L"id_pregled")->AsInteger = zadnjiPregledID;

        query->Open();

        while (!query->Eof)
        {
            int idPodrucje =
                query->FieldByName(L"id_podrucje_razvoja")->AsInteger;

            int razina =
                query->FieldByName(L"razina")->AsInteger;

            if (idPodrucje >= 1 && idPodrucje <= 4)
            {
                trenutneRazine[idPodrucje] = razina;
            }

            query->Next();
        }

        query->Close();

        if (postojiPrethodni)
        {
            query->ParamByName(L"id_pregled")->AsInteger = prethodniPregledID;

            query->Open();

            while (!query->Eof)
            {
                int idPodrucje =
                    query->FieldByName(L"id_podrucje_razvoja")->AsInteger;

                int razina =
                    query->FieldByName(L"razina")->AsInteger;

                if (idPodrucje >= 1 && idPodrucje <= 4)
                {
                    prethodneRazine[idPodrucje] = razina;
                }

                query->Next();
            }

            query->Close();
        }
    }
    __finally
    {
        delete query;
    }

    String naziviPodrucja[5];

    naziviPodrucja[1] = L"Tjelesni i psihomotorni razvoj";
    naziviPodrucja[2] = L"Socio-emocionalni razvoj";
    naziviPodrucja[3] = L"Govorni razvoj";
    naziviPodrucja[4] = L"Spoznajni razvoj";

    for (int idPodrucje = 1; idPodrucje <= 4; idPodrucje++)
    {
        TAnalizaPodrucja analiza;

        analiza.idPodrucje = idPodrucje;
        analiza.nazivPodrucja = naziviPodrucja[idPodrucje];

        analiza.trenutnaRazina =
            trenutneRazine[idPodrucje];

        analiza.prethodnaRazina =
            prethodneRazine[idPodrucje];

        analiza.postojiPrethodna =
            postojiPrethodni;

        analiza.trend =
            odrediTrend(
                analiza.trenutnaRazina,
                analiza.prethodnaRazina,
                analiza.postojiPrethodna
            );

        analiza.ciljanaTezina = 0;
        analiza.prioritet = 0;

        analize.push_back(analiza);
    }

    return analize;
}

int Tform_razvoj::odrediCiljanuTezinu(
    int trenutnaRazina,
    const String &trend)
{
    if (trenutnaRazina < 1)
    {
        return 1;
    }

    if (trenutnaRazina > 5)
    {
        return 5;
    }

    if (trend == L"Pad")
    {
        switch (trenutnaRazina)
        {
            case 1: return 1;
            case 2: return 2;
            case 3: return 2;
            case 4: return 3;
            case 5: return 4;
        }
    }

    if (trend == L"Rast")
    {
        switch (trenutnaRazina)
        {
            case 1: return 2;
            case 2: return 3;
            case 3: return 4;
            case 4: return 5;
            case 5: return 5;
        }
    }

    switch (trenutnaRazina)
    {
        case 1: return 2;
        case 2: return 2;
        case 3: return 3;
        case 4: return 4;
        case 5: return 5;
    }

    return trenutnaRazina;
}

int Tform_razvoj::izracunajPrioritet(
    int trenutnaRazina,
    const String &trend)
{
    int prioritet = 0;

    switch (trenutnaRazina)
    {
        case 1:
            prioritet = 100;
            break;

        case 2:
            prioritet = 80;
            break;

        case 3:
            prioritet = 60;
            break;

        case 4:
            prioritet = 40;
            break;

        case 5:
            prioritet = 20;
            break;

        default:
            prioritet = 0;
            break;
    }

    if (trend == L"Pad")
    {
        prioritet += 30;
    }
    else if (trend == L"Stabilno")
    {
        prioritet += 10;
    }
    else if (trend == L"Prvi pregled")
    {
        prioritet += 5;
    }
    else if (trend == L"Rast")
    {
        prioritet -= 10;
    }

    return prioritet;
}

void Tform_razvoj::izracunajTezineIPrioritete(
    std::vector<TAnalizaPodrucja> &analize)
{
    for (unsigned int i = 0; i < analize.size(); i++)
    {
        analize[i].ciljanaTezina =
            odrediCiljanuTezinu(
                analize[i].trenutnaRazina,
                analize[i].trend
            );

        analize[i].prioritet =
            izracunajPrioritet(
                analize[i].trenutnaRazina,
                analize[i].trend
            );
    }
}

bool Tform_razvoj::aktivnostJeVecOdabrana(
    const std::vector<TPreporucenaAktivnost> &aktivnosti,
    int idAktivnost)
{
    for (unsigned int i = 0;
         i < aktivnosti.size();
         i++)
    {
        if (aktivnosti[i].idAktivnost == idAktivnost)
        {
            return true;
        }
    }

    return false;
}

std::vector<Tform_razvoj::TPreporucenaAktivnost> Tform_razvoj::odaberiPreporuceneAktivnosti(
    TFDConnection *konekcija,
    int idDijete,
    const std::vector<TAnalizaPodrucja> &analize)
{
    std::vector<TPreporucenaAktivnost> preporuceneAktivnosti;

    if (analize.empty())
    {
        return preporuceneAktivnosti;
    }

    std::vector<TAnalizaPodrucja> sortiraneAnalize =
        analize;

    std::sort(
        sortiraneAnalize.begin(),
        sortiraneAnalize.end(),
        [](const TAnalizaPodrucja &a,
           const TAnalizaPodrucja &b)
        {
            return a.prioritet > b.prioritet;
        }
    );

    TFDQuery *query = new TFDQuery(NULL);
    query->Connection = konekcija;

    try
    {
        for (unsigned int i = 0;
             i < sortiraneAnalize.size();
             i++)
        {
            if (preporuceneAktivnosti.size() >= 3)
            {
                break;
            }

            const TAnalizaPodrucja &analiza =
                sortiraneAnalize[i];

            int maksimalnoIzPodrucja;

            if (i == 0)
            {
                maksimalnoIzPodrucja = 2;
            }
            else
            {
                maksimalnoIzPodrucja = 1;
            }

            int odabranoIzPodrucja = 0;

            query->Close();
            query->SQL->Clear();

            query->SQL->Add(
                L"SELECT DISTINCT "
                L"    a.id_aktivnost, "
                L"    a.naziv, "
                L"    a.razina_tezine, "
                L"    a.trajanje_minuta "
                L"FROM aktivnost a "
                L"INNER JOIN aktivnost_podrucje ap "
                L"    ON ap.id_aktivnost = a.id_aktivnost "
                L"WHERE ap.id_podrucje_razvoja = :id_podrucje "
                L"  AND a.aktivna = 1 "
                L"  AND TIMESTAMPDIFF( "
                L"          YEAR, "
                L"          (SELECT d.datum_rodjenja "
                L"           FROM dijete d "
                L"           WHERE d.id_dijete = :id_dijete), "
                L"          CURDATE() "
                L"      ) BETWEEN a.dob_od AND a.dob_do "
                L"ORDER BY "
                L"    ABS(a.razina_tezine - :ciljana_tezina), "
                L"    a.trajanje_minuta, "
                L"    a.naziv"
            );

            query->ParamByName(L"id_podrucje")->AsInteger = analiza.idPodrucje;
            query->ParamByName(L"id_dijete")->AsInteger = idDijete;
            query->ParamByName(L"ciljana_tezina")->AsInteger = analiza.ciljanaTezina;

            query->Open();

            while (!query->Eof &&
                   odabranoIzPodrucja < maksimalnoIzPodrucja &&
                   preporuceneAktivnosti.size() < 3)
            {
                int idAktivnost =
                    query->FieldByName(L"id_aktivnost")->AsInteger;

                if (!aktivnostJeVecOdabrana(preporuceneAktivnosti, idAktivnost))
                {
                    TPreporucenaAktivnost aktivnost;

                    aktivnost.idAktivnost = idAktivnost;
                    aktivnost.idPodrucje = analiza.idPodrucje;

                    aktivnost.nazivAktivnosti =
                        query->FieldByName(L"naziv")->AsString;

                    aktivnost.nazivPodrucja =
                        analiza.nazivPodrucja;

                    aktivnost.tezinaAktivnosti =
                        query->FieldByName(L"razina_tezine")->AsInteger;

                    preporuceneAktivnosti.push_back(aktivnost);

                    odabranoIzPodrucja++;
                }

                query->Next();
            }

            query->Close();
        }
    }
    __finally
    {
        delete query;
    }

    return preporuceneAktivnosti;
}

String Tform_razvoj::generirajOpciZakljucak(
    const std::vector<TAnalizaPodrucja> &analize)
{
    if (analize.empty())
    {
        return L"Nema dostupnih podataka za izradu zaključka.";
    }

    int brojPadova = 0;
    int brojRastova = 0;
    int brojStabilnih = 0;
    int brojPrvihPregleda = 0;

    int indeksNajvecegPrioriteta = 0;

    for (unsigned int i = 0; i < analize.size(); i++)
    {
        if (analize[i].trend == L"Pad")
        {
            brojPadova++;
        }
        else if (analize[i].trend == L"Rast")
        {
            brojRastova++;
        }
        else if (analize[i].trend == L"Stabilno")
        {
            brojStabilnih++;
        }
        else if (analize[i].trend == L"Prvi pregled")
        {
            brojPrvihPregleda++;
        }

        if (analize[i].prioritet >
            analize[indeksNajvecegPrioriteta].prioritet)
        {
            indeksNajvecegPrioriteta = i;
        }
    }

    const TAnalizaPodrucja &prioritetnoPodrucje =
        analize[indeksNajvecegPrioriteta];

    String zakljucak;

    if (brojPrvihPregleda == 4)
    {
        zakljucak =
            L"Ovo je prvi razvojni pregled djeteta. "
            L"Preporuka je izrađena prema trenutačnim razinama razvoja "
            L"i dobi djeteta.";
    }
    else if (brojPadova >= 2)
    {
        zakljucak =
            L"U odnosu na prethodni razvojni pregled uočeno je smanjenje "
            L"procjene u više razvojnih područja. Preporučuje se dodatno "
            L"strukturirano poticanje i praćenje razvoja.";
    }
    else if (brojPadova == 1)
    {
        zakljucak =
            L"Razvoj djeteta uglavnom je stabilan, ali je u jednom području "
            L"uočena niža procjena u odnosu na prethodni pregled.";
    }
    else if (brojRastova >= 2)
    {
        zakljucak =
            L"Dijete pokazuje pozitivan razvojni napredak u više područja. "
            L"Preporučuju se aktivnosti koje održavaju napredak i pružaju "
            L"primjeren dodatni izazov.";
    }
    else if (brojStabilnih >= 3)
    {
        zakljucak =
            L"Razvoj djeteta je uglavnom stabilan. Preporučuju se aktivnosti "
            L"za učvršćivanje postojećih vještina i postupno napredovanje.";
    }
    else
    {
        zakljucak =
            L"Razvojni pregled pokazuje različite trendove među područjima. "
            L"Preporuka je usmjerena na održavanje jakih strana i dodatno "
            L"poticanje područja kojima je potrebna veća podrška.";
    }

    zakljucak +=
        L"\r\n\r\nNajveći prioritet trenutno ima područje: " +
        prioritetnoPodrucje.nazivPodrucja + L".";

    return zakljucak;
}

String Tform_razvoj::generirajPopisAktivnosti(
    const std::vector<TPreporucenaAktivnost> &aktivnosti)
{
    if (aktivnosti.empty())
    {
        return L"Nisu pronađene prikladne aktivnosti.";
    }

    String rezultat;

    for (unsigned int i = 0;
         i < aktivnosti.size();
         i++)
    {
        rezultat +=
            IntToStr(static_cast<int>(i + 1)) +
            L". " +
            aktivnosti[i].nazivAktivnosti;

        rezultat +=
            L"\r\n   Područje: " +
            aktivnosti[i].nazivPodrucja;

        rezultat +=
            L"\r\n   Težina: " +
            IntToStr(aktivnosti[i].tezinaAktivnosti);

        if (i + 1 < aktivnosti.size())
        {
            rezultat += L"\r\n\r\n";
        }
    }

    return rezultat;
}

String Tform_razvoj::generirajOpciRazlog(
    const std::vector<TAnalizaPodrucja> &analize,
    const std::vector<TPreporucenaAktivnost> &aktivnosti)
{
    if (aktivnosti.empty())
    {
        return L"Nije moguće oblikovati razlog jer nisu pronađene prikladne aktivnosti.";
    }

    if (analize.empty())
    {
        return L"Nema dostupnih razvojnih podataka.";
    }

    int indeksNajvecegPrioriteta = 0;

    for (unsigned int i = 1; i < analize.size(); i++)
    {
        if (analize[i].prioritet >
            analize[indeksNajvecegPrioriteta].prioritet)
        {
            indeksNajvecegPrioriteta = i;
        }
    }

    const TAnalizaPodrucja &prioritetnoPodrucje =
        analize[indeksNajvecegPrioriteta];

    String razlog =
        L"Odabrane aktivnosti odgovaraju dobi djeteta, području razvoja "
        L"i procijenjenoj razini težine.";

    if (prioritetnoPodrucje.trend == L"Pad")
    {
        razlog +=
            L"\r\n\r\nPrednost je dana području \"" +
            prioritetnoPodrucje.nazivPodrucja +
            L"\" jer je u odnosu na prethodni pregled zabilježena niža "
            L"procjena. Preporučuje se dodatno strukturirano poticanje "
            L"uz podršku odgojitelja.";
    }
    else if (prioritetnoPodrucje.trend == L"Stabilno")
    {
        razlog +=
            L"\r\n\r\nPodručje \"" +
            prioritetnoPodrucje.nazivPodrucja +
            L"\" trenutno ima najveći prioritet. Razina je stabilna pa su "
            L"odabrane aktivnosti usmjerene na učvršćivanje postojećih "
            L"vještina i postupno napredovanje.";
    }
    else if (prioritetnoPodrucje.trend == L"Rast")
    {
        razlog +=
            L"\r\n\r\nU području \"" +
            prioritetnoPodrucje.nazivPodrucja +
            L"\" zabilježen je pozitivan trend. Odabrane aktivnosti pružaju "
            L"dodatni izazov primjeren trenutačnoj razini razvoja.";
    }
    else
    {
        razlog +=
            L"\r\n\r\nBudući da prethodni pregled ne postoji, aktivnosti su "
            L"odabrane prema trenutačnoj procjeni i dobi djeteta.";
    }

    if (aktivnosti.size() == 1)
    {
        razlog +=
            L"\r\n\r\nPronađena je jedna prikladna aktivnost.";
    }
    else
    {
        razlog +=
            L"\r\n\r\nOdabrano je " +
            IntToStr(static_cast<int>(aktivnosti.size())) +
            L" aktivnosti kako bi preporuka bila dovoljno konkretna, "
            L"ali i pregledna.";
    }

    return razlog;
}
void __fastcall Tform_razvoj::button_generiraj_preporukuClick(
    TObject *Sender)
{
    TKorisnickaPrava prava(data_module->currentUserUloga);

	if (!prava.generirajPreporuku())
	{
		ShowMessage(L"Nemate pravo generiranja preporuke.");
		return;
	}
    memo_zakljucak->Clear();
    memo_preporucene_aktivnosti->Clear();
    memo_razlog_preporuke->Clear();

    analizePodrucja.clear();
	preporuceneAktivnosti.clear();

	combo_preporucene_aktivnosti->Items->Clear();
	combo_preporucene_aktivnosti->ItemIndex = -1;
	combo_preporucene_aktivnosti->Enabled = false;
	button_otvori_aktivnost->Enabled = false;

    generiraniPregledID = 0;
    preporukaGenerirana = false;

    button_spremi_preporuku->Enabled = false;

    if (odabranoDijeteID <= 0)
    {
        ShowMessage(L"Prvo odaberite dijete.");
        return;
    }

    int zadnjiPregledID = 0;
    int prethodniPregledID = 0;
    TDateTime datumZadnjeg = 0;
    bool postojiPrethodni = false;

    try
    {
        bool postojiPregled =
            ucitajPregledeZaPreporuke(
                data_module->connection,
                odabranoDijeteID,
                zadnjiPregledID,
                prethodniPregledID,
                datumZadnjeg,
                postojiPrethodni
            );

        if (!postojiPregled)
        {
            memo_zakljucak->Text =
                L"Za odabrano dijete ne postoji razvojni pregled.";

            memo_preporucene_aktivnosti->Text =
                L"Preporuka se ne može generirati bez razvojne procjene.";

            memo_razlog_preporuke->Text =
                L"Najprije je potrebno spremiti barem jedan razvojni pregled.";

            return;
        }

        analizePodrucja =
            ucitajAnalizePodrucja(
                data_module->connection,
                zadnjiPregledID,
                prethodniPregledID,
                postojiPrethodni
            );

        if (analizePodrucja.empty())
        {
            memo_zakljucak->Text =
                L"Za zadnji pregled nisu pronađene razvojne procjene.";

            memo_preporucene_aktivnosti->Text =
                L"Nema dostupnih aktivnosti.";

            memo_razlog_preporuke->Text =
                L"Preporuka se ne može izraditi bez ocjena razvojnih područja.";

            return;
        }

        izracunajTezineIPrioritete(analizePodrucja);

        preporuceneAktivnosti =
            odaberiPreporuceneAktivnosti(
                data_module->connection,
                odabranoDijeteID,
                analizePodrucja
            );

        memo_zakljucak->Text =
            generirajOpciZakljucak(analizePodrucja);

        memo_preporucene_aktivnosti->Text =
            generirajPopisAktivnosti(preporuceneAktivnosti);

        memo_razlog_preporuke->Text =
			generirajOpciRazlog(analizePodrucja, preporuceneAktivnosti);

        popuniComboAktivnosti();

        combo_status_preporuke->ItemIndex = 0;

        generiraniPregledID = zadnjiPregledID;
        preporukaGenerirana = true;

		button_spremi_preporuku->Enabled =
			prava.spremiPreporuku();
		combo_status_preporuke->Enabled =
			prava.promijeniStatusPreporuke();
	}
    catch (const Exception &e)
    {
        memo_zakljucak->Clear();
        memo_preporucene_aktivnosti->Clear();
        memo_razlog_preporuke->Clear();

        generiraniPregledID = 0;
        preporukaGenerirana = false;

        button_spremi_preporuku->Enabled = false;

        ShowMessage(
            L"Došlo je do pogreške pri generiranju preporuke:\n" +
            e.Message
        );
    }
}
//---------------------------------------------------------------------------
int Tform_razvoj::spremiGlavnuPreporuku(
    TFDConnection *konekcija,
    int idPregled,
    const String &status,
    const String &razlog,
    const String &zakljucak)
{
    if (idPregled <= 0)
    {
        throw Exception(L"Preporuka nije generirana.");
    }

    if (status.IsEmpty())
    {
        throw Exception(L"Odaberite status preporuke.");
    }

    int preporukaID = 0;

    TFDQuery *query = new TFDQuery(NULL);
    query->Connection = konekcija;

    try
    {
        query->SQL->Add(
            L"SELECT id_preporuka "
            L"FROM preporuka "
            L"WHERE id_razvojni_pregled = :id_pregled "
            L"LIMIT 1"
        );

        query->ParamByName(L"id_pregled")->AsInteger = idPregled;

        query->Open();

        if (!query->IsEmpty())
        {
            preporukaID =
                query->FieldByName(L"id_preporuka")->AsInteger;
        }

        query->Close();
        query->SQL->Clear();

        if (preporukaID > 0)
        {
            query->SQL->Add(
                L"UPDATE preporuka SET "
                L"    razlog = :razlog, "
                L"    status = :status, "
                L"    datum = CURDATE(), "
                L"    napomena = :napomena "
                L"WHERE id_preporuka = :id_preporuka"
            );

            query->ParamByName(L"id_preporuka")->AsInteger = preporukaID;
        }
        else
        {
            query->SQL->Add(
                L"INSERT INTO preporuka "
                L"    (razlog, status, datum, napomena, id_razvojni_pregled) "
                L"VALUES "
                L"    (:razlog, :status, CURDATE(), :napomena, :id_pregled)"
            );

            query->ParamByName(L"id_pregled")->AsInteger = idPregled;
        }

        query->ParamByName(L"razlog")->AsString = razlog;
        query->ParamByName(L"status")->AsString = status;

        query->ParamByName(L"napomena")->AsString = zakljucak;

        query->ExecSQL();

        if (preporukaID == 0)
        {
            query->Close();
            query->SQL->Clear();
            query->SQL->Add(
                L"SELECT LAST_INSERT_ID() AS id_preporuka"
            );
            query->Open();

            preporukaID =
                query->FieldByName(L"id_preporuka")->AsInteger;

            query->Close();
        }
    }
    __finally
    {
        delete query;
    }

    return preporukaID;
}

void Tform_razvoj::spremiAktivnostiPreporuke(
    TFDConnection *konekcija,
    int preporukaID,
    const std::vector<TPreporucenaAktivnost> &aktivnosti)
{
    if (preporukaID <= 0)
    {
        throw Exception(L"Neispravan ID preporuke.");
    }

    TFDQuery *query = new TFDQuery(NULL);
    query->Connection = konekcija;

    try
    {
        query->SQL->Add(
            L"DELETE FROM preporuka_aktivnost "
            L"WHERE id_preporuka = :id_preporuka"
        );

        query->ParamByName(L"id_preporuka")->AsInteger = preporukaID;

        query->ExecSQL();

        for (unsigned int i = 0;
             i < aktivnosti.size();
             i++)
        {
            query->Close();
            query->SQL->Clear();
            query->SQL->Add(
                L"INSERT INTO preporuka_aktivnost "
                L"    (id_preporuka, id_aktivnost) "
                L"VALUES "
                L"    (:id_preporuka, :id_aktivnost)"
            );

            query->ParamByName(L"id_preporuka")->AsInteger = preporukaID;

            query->ParamByName(L"id_aktivnost")->AsInteger =
                aktivnosti[i].idAktivnost;

            query->ExecSQL();
        }
    }
    __finally
    {
        delete query;
    }
}
void __fastcall Tform_razvoj::button_spremi_preporukuClick(TObject *Sender)
{
    TKorisnickaPrava prava(data_module->currentUserUloga);

	if (!prava.spremiPreporuku())
	{
		ShowMessage(L"Nemate pravo spremanja preporuke.");
		return;
	}
    if (!prava.promijeniStatusPreporuke())
    {
        ShowMessage(L"Nemate pravo promjene statusa preporuke.");
        return;
    }

     if (!preporukaGenerirana || generiraniPregledID <= 0)
    {
        ShowMessage(L"Najprije generirajte preporuku.");
        return;
    }

    if (preporuceneAktivnosti.empty())
    {
        ShowMessage(
            L"Preporuka nema odabranih aktivnosti i ne može se spremiti."
        );
        return;
    }

    try
    {
        data_module->connection->StartTransaction();

        int preporukaID =
            spremiGlavnuPreporuku(
                data_module->connection,
                generiraniPregledID,
                combo_status_preporuke->Text,
                memo_razlog_preporuke->Text,
                memo_zakljucak->Text
            );

        spremiAktivnostiPreporuke(
            data_module->connection,
            preporukaID,
            preporuceneAktivnosti
        );

        data_module->connection->Commit();

        ShowMessage(L"Preporuka je uspješno spremljena.");

        button_spremi_preporuku->Enabled = false;
    }
    catch (const Exception &e)
    {
        if (data_module->connection->InTransaction)
        {
            data_module->connection->Rollback();
        }

        ShowMessage(
            L"Došlo je do pogreške pri spremanju preporuke:\n" +
            e.Message
        );
    }
}
//---------------------------------------------------------------------------
void Tform_razvoj::popuniComboAktivnosti()
{
    combo_preporucene_aktivnosti->Items->Clear();
    combo_preporucene_aktivnosti->ItemIndex = -1;

    combo_preporucene_aktivnosti->Enabled = false;
    button_otvori_aktivnost->Enabled = false;

    for (unsigned int i = 0;
         i < preporuceneAktivnosti.size();
         i++)
    {
        combo_preporucene_aktivnosti->Items->AddObject(
            preporuceneAktivnosti[i].nazivAktivnosti,
            reinterpret_cast<TObject*>(
                static_cast<NativeInt>(
                    preporuceneAktivnosti[i].idAktivnost
                )
            )
        );
    }

    if (combo_preporucene_aktivnosti->Items->Count > 0)
    {
        combo_preporucene_aktivnosti->ItemIndex = 0;
        combo_preporucene_aktivnosti->Enabled = true;
        button_otvori_aktivnost->Enabled = true;
    }
}
void __fastcall Tform_razvoj::button_otvori_aktivnostClick(TObject *Sender)
{
    if (combo_preporucene_aktivnosti->ItemIndex < 0)
    {
        ShowMessage(L"Odaberite aktivnost.");
        return;
    }

    int indeks =
        combo_preporucene_aktivnosti->ItemIndex;

    int idAktivnost =
        static_cast<int>(
            reinterpret_cast<NativeInt>(
                combo_preporucene_aktivnosti
                    ->Items
                    ->Objects[indeks]
            )
        );

    if (idAktivnost <= 0)
    {
        ShowMessage(L"Odabrana aktivnost nije ispravna.");
        return;
    }

    form_aktivnost_unos
        ->pripremiZaPregled(idAktivnost);

    form_aktivnost_unos->ShowModal();
}
//---------------------------------------------------------------------------
void Tform_razvoj::primijeniPrava()
{
    TKorisnickaPrava prava(data_module->currentUserUloga);

    bool smijeUnosPregleda =
        prava.unesiRazvojniPregled();

    speed_tjelesni_1->Enabled = smijeUnosPregleda;
    speed_tjelesni_2->Enabled = smijeUnosPregleda;
    speed_tjelesni_3->Enabled = smijeUnosPregleda;
    speed_tjelesni_4->Enabled = smijeUnosPregleda;
    speed_tjelesni_5->Enabled = smijeUnosPregleda;

    speed_socio_1->Enabled = smijeUnosPregleda;
    speed_socio_2->Enabled = smijeUnosPregleda;
    speed_socio_3->Enabled = smijeUnosPregleda;
    speed_socio_4->Enabled = smijeUnosPregleda;
    speed_socio_5->Enabled = smijeUnosPregleda;

    speed_govorni_1->Enabled = smijeUnosPregleda;
    speed_govorni_2->Enabled = smijeUnosPregleda;
    speed_govorni_3->Enabled = smijeUnosPregleda;
    speed_govorni_4->Enabled = smijeUnosPregleda;
    speed_govorni_5->Enabled = smijeUnosPregleda;

    speed_spoznajni_1->Enabled = smijeUnosPregleda;
    speed_spoznajni_2->Enabled = smijeUnosPregleda;
    speed_spoznajni_3->Enabled = smijeUnosPregleda;
    speed_spoznajni_4->Enabled = smijeUnosPregleda;
    speed_spoznajni_5->Enabled = smijeUnosPregleda;

    memo_napomena->ReadOnly = !smijeUnosPregleda;

    button_ocisti->Enabled = smijeUnosPregleda;
    button_spremi->Enabled = smijeUnosPregleda;

    button_generiraj_preporuku->Enabled =
        prava.generirajPreporuku();

    combo_status_preporuke->Enabled =
        prava.promijeniStatusPreporuke();

    button_spremi_preporuku->Enabled = false;

    button_otvori_aktivnost->Enabled = false;

    button_korisnici->Visible =
        prava.korisnici();
}

void Tform_razvoj::ocistiPrikazPreporuke()
{
    memo_zakljucak->Clear();
    memo_preporucene_aktivnosti->Clear();
    memo_razlog_preporuke->Clear();

    analizePodrucja.clear();
    preporuceneAktivnosti.clear();

    combo_preporucene_aktivnosti->Items->Clear();
    combo_preporucene_aktivnosti->ItemIndex = -1;
    combo_preporucene_aktivnosti->Enabled = false;

    button_otvori_aktivnost->Enabled = false;
    button_spremi_preporuku->Enabled = false;

    generiraniPregledID = 0;
    preporukaGenerirana = false;

    combo_status_preporuke->ItemIndex = 0;
}

void Tform_razvoj::ucitajSpremljenuPreporuku()
{
    ocistiPrikazPreporuke();

    if (odabranoDijeteID <= 0)
    {
        return;
    }

    query_ucitaj_preporuku->Close();
    query_ucitaj_preporuku->SQL->Clear();

    query_ucitaj_preporuku->SQL->Add(
        L"SELECT "
        L"    p.id_preporuka, "
        L"    p.id_razvojni_pregled, "
        L"    p.napomena, "
        L"    p.razlog, "
        L"    p.status "
        L"FROM preporuka p "
        L"INNER JOIN razvojni_pregled rp "
        L"    ON rp.id_razvojni_pregled = p.id_razvojni_pregled "
        L"WHERE rp.id_dijete = :id_dijete "
        L"ORDER BY rp.datum DESC, rp.id_razvojni_pregled DESC "
        L"LIMIT 1"
    );

    query_ucitaj_preporuku
        ->ParamByName(L"id_dijete")
        ->AsInteger = odabranoDijeteID;

    query_ucitaj_preporuku->Open();

    if (query_ucitaj_preporuku->IsEmpty())
    {
        query_ucitaj_preporuku->Close();
        return;
    }

    int preporukaID =
        query_ucitaj_preporuku
            ->FieldByName(L"id_preporuka")
            ->AsInteger;

    generiraniPregledID =
        query_ucitaj_preporuku
            ->FieldByName(L"id_razvojni_pregled")
            ->AsInteger;

    memo_zakljucak->Text =
        query_ucitaj_preporuku
            ->FieldByName(L"napomena")
            ->AsString;

    memo_razlog_preporuke->Text =
        query_ucitaj_preporuku
            ->FieldByName(L"razlog")
            ->AsString;

    String status =
        query_ucitaj_preporuku
            ->FieldByName(L"status")
            ->AsString;

    int statusIndex =
        combo_status_preporuke
            ->Items
            ->IndexOf(status);

    if (statusIndex >= 0)
    {
        combo_status_preporuke->ItemIndex = statusIndex;
    }
    else
    {
        combo_status_preporuke->ItemIndex = 0;
    }

    query_ucitaj_preporuku->Close();

    preporuceneAktivnosti.clear();

    query_ucitaj_preporuka_aktivnosti->Close();
    query_ucitaj_preporuka_aktivnosti->SQL->Clear();

    query_ucitaj_preporuka_aktivnosti->SQL->Add(
        L"SELECT "
        L"    a.id_aktivnost, "
        L"    a.naziv, "
        L"    a.razina_tezine, "
        L"    GROUP_CONCAT("
        L"        DISTINCT pr.naziv "
        L"        ORDER BY pr.naziv "
        L"        SEPARATOR ', '"
        L"    ) AS podrucja "
        L"FROM preporuka_aktivnost pa "
        L"INNER JOIN aktivnost a "
        L"    ON a.id_aktivnost = pa.id_aktivnost "
        L"LEFT JOIN aktivnost_podrucje ap "
        L"    ON ap.id_aktivnost = a.id_aktivnost "
        L"LEFT JOIN podrucje_razvoja pr "
        L"    ON pr.id_podrucje_razvoja = ap.id_podrucje_razvoja "
        L"WHERE pa.id_preporuka = :id_preporuka "
        L"GROUP BY "
        L"    a.id_aktivnost, "
        L"    a.naziv, "
        L"    a.razina_tezine "
        L"ORDER BY a.naziv"
    );

    query_ucitaj_preporuka_aktivnosti
        ->ParamByName(L"id_preporuka")
        ->AsInteger = preporukaID;

    query_ucitaj_preporuka_aktivnosti->Open();

    while (!query_ucitaj_preporuka_aktivnosti->Eof)
    {
        TPreporucenaAktivnost aktivnost;

        aktivnost.idAktivnost =
            query_ucitaj_preporuka_aktivnosti
                ->FieldByName(L"id_aktivnost")
                ->AsInteger;

        aktivnost.idPodrucje = 0;

        aktivnost.nazivAktivnosti =
            query_ucitaj_preporuka_aktivnosti
                ->FieldByName(L"naziv")
                ->AsString;

        aktivnost.nazivPodrucja =
            query_ucitaj_preporuka_aktivnosti
                ->FieldByName(L"podrucja")
                ->AsString;

        aktivnost.tezinaAktivnosti =
            query_ucitaj_preporuka_aktivnosti
                ->FieldByName(L"razina_tezine")
                ->AsInteger;

        preporuceneAktivnosti.push_back(aktivnost);

        query_ucitaj_preporuka_aktivnosti->Next();
    }

    query_ucitaj_preporuka_aktivnosti->Close();

    memo_preporucene_aktivnosti->Text =
        generirajPopisAktivnosti(preporuceneAktivnosti);

    popuniComboAktivnosti();

    preporukaGenerirana = true;

    TKorisnickaPrava prava(data_module->currentUserUloga);

    button_spremi_preporuku->Enabled =
        prava.spremiPreporuku();

    combo_status_preporuke->Enabled =
        prava.promijeniStatusPreporuke();
}
void __fastcall Tform_razvoj::button_korisniciClick(TObject *Sender)
{
    form_korisnici->Show();
    this->Hide();
}
//---------------------------------------------------------------------------

void __fastcall Tform_razvoj::button_uputeClick(TObject *Sender)
{
    form_upute->Show();
    this->Hide();
}
//---------------------------------------------------------------------------
void __fastcall Tform_razvoj::button_zdravljeClick(TObject *Sender)
{
    form_bolesti->Show();
    this->Hide();
}
//---------------------------------------------------------------------------
void __fastcall Tform_razvoj::button_najaveClick(TObject *Sender)
{
    form_najave->Show();
    this->Hide();
}
//---------------------------------------------------------------------------
