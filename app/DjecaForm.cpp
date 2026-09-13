//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "DjecaForm.h"
#include "DatabaseModule.h"
#include "DijeteUnosForm.h"
#include "LoginForm.h"
#include "DashboardForm.h"
#include "KorisnickaPrava.h"
#include "SkupineForm.h"
#include "AktivnostiForm.h"
#include "RazvojForm.h"
#include "KorisniciForm.h"
#include "UputeForm.h"
#include "BolestiForm.h"
#include "NajaveForm.h"
#include "PreporukeGrupnoForm.h"
#include <IniFiles.hpp>
//---------------------------------------------------------------------------
static String PutanjaPostavkiIni()
{
	return ExtractFilePath(Application->ExeName) + "postavke.ini";
}
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
Tform_djeca *form_djeca;
//---------------------------------------------------------------------------
__fastcall Tform_djeca::Tform_djeca(TComponent* Owner)
	: TForm(Owner)
{
    originalSortLabelLeft = label_sortiranje->Left;
	originalSortComboLeft = combo_sortiranje->Left;
}
//---------------------------------------------------------------------------
void __fastcall Tform_djeca::button_pocetnaClick(TObject *Sender)
{
	form_dashboard->Show();
    this->Hide();
}
//---------------------------------------------------------------------------
void __fastcall Tform_djeca::FormShow(TObject *Sender)
{
	label_korisnik->Caption =
		data_module->currentUserIme +
		L" " +
		data_module->currentUserPrezime;

	label_uloga->Caption =
		data_module->currentUserUloga;

	ucitajSkupine();

    combo_sortiranje->Items->Clear();

	combo_sortiranje->Items->Add(L"Prezime A-Ž");
	combo_sortiranje->Items->Add(L"Prezime Ž-A");
	combo_sortiranje->Items->Add(L"Ime A-Ž");
	combo_sortiranje->Items->Add(L"Ime Ž-A");
	combo_sortiranje->Items->Add(L"Najmlađi prvo");
	combo_sortiranje->Items->Add(L"Najstariji prvo");

	TIniFile *iniSort = new TIniFile(PutanjaPostavkiIni());
	int zadnjeSortiranje;
	try
	{
		zadnjeSortiranje = iniSort->ReadInteger("Djeca", "ZadnjeSortiranje", 0);
	}
	__finally
	{
		delete iniSort;
	}

	if (zadnjeSortiranje < 0 || zadnjeSortiranje >= combo_sortiranje->Items->Count)
	{
		zadnjeSortiranje = 0;
	}

	combo_sortiranje->ItemIndex = zadnjeSortiranje;

	osvjeziPopisDjece();
    primijeniPrava();
}
//---------------------------------------------------------------------------
void Tform_djeca::ucitajSkupine()
{
	combo_skupina->Items->Clear();

    combo_skupina->Items->AddObject(
        L"Sve skupine",
        reinterpret_cast<TObject*>(0)
	);
    query_skupine->Close();
	query_skupine->Open();
    while (!query_skupine->Eof)
    {
        String naziv =
			query_skupine->FieldByName("naziv")->AsString;
		int idSkupina =
			query_skupine->FieldByName("id_skupina")->AsInteger;
		combo_skupina->Items->AddObject(
			naziv,
			reinterpret_cast<TObject*>(
				static_cast<NativeInt>(idSkupina)
			)
		);
        query_skupine->Next();
	}

	TIniFile *ini = new TIniFile(PutanjaPostavkiIni());
	int zadnjaSkupina;
	try
	{
		zadnjaSkupina = ini->ReadInteger("Djeca", "ZadnjaSkupina", 0);
	}
	__finally
	{
		delete ini;
	}

	combo_skupina->ItemIndex = 0;
	for (int i = 0; i < combo_skupina->Items->Count; i++)
	{
		if (reinterpret_cast<NativeInt>(combo_skupina->Items->Objects[i]) == zadnjaSkupina)
		{
			combo_skupina->ItemIndex = i;
			break;
		}
	}
}
void Tform_djeca::osvjeziPopisDjece()
{
	String pretraga = edit_pretraga->Text.Trim();
	TKorisnickaPrava prava(data_module->currentUserUloga);

    NativeInt idSkupina = 0;

    if (combo_skupina->ItemIndex >= 0)
    {
        idSkupina = reinterpret_cast<NativeInt>(
            combo_skupina->Items->Objects[
                combo_skupina->ItemIndex
            ]
        );
    }

    query_djeca->Close();
    query_djeca->SQL->Clear();

	query_djeca->SQL->Add(
		"SELECT "
		"id_dijete, "
		"ime, "
		"prezime, "
		"datum_rodjenja, "
		"id_skupina "
		"FROM dijete "
		"WHERE 1 = 1 "
	);

    if (!pretraga.IsEmpty())
    {
        query_djeca->SQL->Add(
			"AND (ime LIKE :pretraga "
			"OR prezime LIKE :pretraga) "
        );

        query_djeca->ParamByName("pretraga")->AsString =
            "%" + pretraga + "%";
    }

	if (prava.samoSvojaSkupina())
	{
		if (data_module->currentUserSkupinaID == 0)
		{
			query_djeca->SQL->Add(
				"AND 1 = 0 "
			);
		}
		else
		{
			query_djeca->SQL->Add(
				"AND id_skupina = :id_skupina_odgojitelja "
			);

			query_djeca
				->ParamByName("id_skupina_odgojitelja")
				->AsInteger =
				data_module->currentUserSkupinaID;
		}
	}
	else if (idSkupina != 0)
	{
		query_djeca->SQL->Add(
			"AND id_skupina = :id_skupina "
		);

		query_djeca
			->ParamByName("id_skupina")
			->AsInteger =
			static_cast<int>(idSkupina);
	}

    switch (combo_sortiranje->ItemIndex)
    {
        case 0:
            query_djeca->SQL->Add(
				"ORDER BY prezime ASC, ime ASC"
			);
            break;

        case 1:
            query_djeca->SQL->Add(
				"ORDER BY prezime DESC, ime DESC"
            );
            break;

        case 2:
            query_djeca->SQL->Add(
				"ORDER BY ime ASC, prezime ASC"
            );
            break;

        case 3:
            query_djeca->SQL->Add(
				"ORDER BY ime DESC, prezime DESC"
            );
            break;

        case 4:
            query_djeca->SQL->Add(
				"ORDER BY datum_rodjenja DESC"
            );
            break;

        case 5:
            query_djeca->SQL->Add(
				"ORDER BY datum_rodjenja ASC"
            );
            break;

        default:
            query_djeca->SQL->Add(
                "ORDER BY prezime ASC, ime ASC"
            );
            break;
    }

	query_djeca->Open();
	postaviIzgledGrida();
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void __fastcall Tform_djeca::edit_pretragaChange(TObject *Sender)
{
	osvjeziPopisDjece();
}
//---------------------------------------------------------------------------
void __fastcall Tform_djeca::combo_skupinaChange(TObject *Sender)
{
	NativeInt idSkupina = 0;
	if (combo_skupina->ItemIndex >= 0)
	{
		idSkupina = reinterpret_cast<NativeInt>(
			combo_skupina->Items->Objects[combo_skupina->ItemIndex]);
	}

	TIniFile *ini = new TIniFile(PutanjaPostavkiIni());
	try
	{
		ini->WriteInteger("Djeca", "ZadnjaSkupina", (int)idSkupina);
	}
	__finally
	{
		delete ini;
	}

    osvjeziPopisDjece();
}
//---------------------------------------------------------------------------
void __fastcall Tform_djeca::combo_sortiranjeChange(TObject *Sender)
{
	TIniFile *ini = new TIniFile(PutanjaPostavkiIni());
	try
	{
		ini->WriteInteger("Djeca", "ZadnjeSortiranje", combo_sortiranje->ItemIndex);
	}
	__finally
	{
		delete ini;
	}

    osvjeziPopisDjece();
}
//---------------------------------------------------------------------------
void Tform_djeca::postaviIzgledGrida()
{
	query_djeca->FieldByName("id_dijete")->Visible = false;
	query_djeca->FieldByName("id_skupina")->Visible = false;
	query_djeca->FieldByName("ime")->DisplayLabel = L"Ime";
	query_djeca->FieldByName("ime")->DisplayWidth = 20;
	query_djeca->FieldByName("prezime")->DisplayLabel = L"Prezime";
	query_djeca->FieldByName("prezime")->DisplayWidth = 25;
	query_djeca->FieldByName("datum_rodjenja")->Visible = false;
	query_djeca->FieldByName("Dob")->DisplayLabel = L"Dob";
	query_djeca->FieldByName("Dob")->DisplayWidth = 20;
	query_djeca->FieldByName("Dob")->Alignment = taCenter;
	query_djeca->FieldByName("NazivSkupine")->DisplayLabel = L"Skupina";
	query_djeca->FieldByName("NazivSkupine")->DisplayWidth = 20;
}
//---------------------------------------------------------------------------
void __fastcall Tform_djeca::button_dodajClick(TObject *Sender)
{
	TKorisnickaPrava prava(data_module->currentUserUloga);
	if (!prava.dodajDijete())
    {
        ShowMessage(L"Nemate pravo dodavanja djece.");
        return;
	}
    form_dijete_unos->pripremiZaDodavanje();
    if (form_dijete_unos->ShowModal() == mrOk)
    {
        osvjeziPopisDjece();
    }
}
//---------------------------------------------------------------------------

void __fastcall Tform_djeca::button_urediClick(TObject *Sender)
{
	TKorisnickaPrava prava(data_module->currentUserUloga);
    if (!prava.urediDijete())
    {
        ShowMessage(L"Nemate pravo uređivanja djece.");
        return;
	}
    if (query_djeca->IsEmpty())
    {
        ShowMessage(
            L"Odaberite dijete koje želite urediti."
        );
        return;
    }

    int idDijete =
		query_djeca
			->FieldByName("id_dijete")
			->AsInteger;
	form_dijete_unos->pripremiZaUredjivanje(idDijete);
    if (form_dijete_unos->ShowModal() == mrOk)
	{
		osvjeziPopisDjece();
	}
}
//---------------------------------------------------------------------------

void __fastcall Tform_djeca::button_obrisiClick(TObject *Sender)
{
	TKorisnickaPrava prava(data_module->currentUserUloga);
    if (!prava.obrisiDijete())
    {
        ShowMessage(L"Nemate pravo brisanja djece.");
        return;
	}

    if (query_djeca->IsEmpty())
	{
        ShowMessage(
            L"Odaberite dijete koje želite obrisati."
        );
        return;
	}

	if (prava.odgojitelj())
	{
		int idSkupineDjeteta =
			query_djeca
				->FieldByName("id_skupina")
				->AsInteger;

		if (idSkupineDjeteta !=
			data_module->currentUserSkupinaID)
		{
			ShowMessage(
				"Možete brisati samo djecu iz svoje skupine."
			);
			return;
		}
	}

    int idDijete =
        query_djeca
            ->FieldByName("id_dijete")
            ->AsInteger;

    String ime =
        query_djeca
            ->FieldByName("ime")
            ->AsString;

    String prezime =
        query_djeca
            ->FieldByName("prezime")
            ->AsString;

    int odgovor = MessageDlg(
        L"Želite li zaista obrisati dijete:\n\n" +
        ime + L" " + prezime + L"?",
        mtConfirmation,
        TMsgDlgButtons() << mbYes << mbNo,
        0
    );

    if (odgovor != mrYes)
    {
        return;
    }

    try
    {
        data_module->connection->StartTransaction();

        query_brisanje->Close();
        query_brisanje->SQL->Clear();

        query_brisanje->SQL->Add(
            "DELETE FROM zdravstveni_podaci "
            "WHERE id_dijete = :id_dijete"
        );

        query_brisanje
            ->ParamByName("id_dijete")
            ->AsInteger = idDijete;

        query_brisanje->ExecSQL();

        query_brisanje->Close();
        query_brisanje->SQL->Clear();

        query_brisanje->SQL->Add(
            "DELETE FROM dijete "
            "WHERE id_dijete = :id_dijete"
        );

        query_brisanje
            ->ParamByName("id_dijete")
            ->AsInteger = idDijete;

        query_brisanje->ExecSQL();

        data_module->connection->Commit();

        ShowMessage(
            L"Dijete je uspješno obrisano."
        );

        osvjeziPopisDjece();
    }
    catch (const Exception &e)
    {
        if (data_module->connection->InTransaction)
        {
            data_module->connection->Rollback();
        }

        ShowMessage(
            L"Došlo je do pogreške pri brisanju djeteta:\n" +
            e.Message
        );
    }
}
//---------------------------------------------------------------------------

void __fastcall Tform_djeca::button_generiraj_sveClick(TObject *Sender)
{
	TKorisnickaPrava prava(data_module->currentUserUloga);

	if (!prava.generirajPreporuku() || !prava.spremiPreporuku())
	{
		ShowMessage(L"Nemate pravo generiranja preporuka.");
		return;
	}

	if (query_djeca->IsEmpty())
	{
		ShowMessage(L"Nema djece u trenutnom prikazu.");
		return;
	}

	std::vector<TDijeteZaPreporuku> listaDjece;

	query_djeca->DisableControls();

	try
	{
		query_djeca->First();

		while (!query_djeca->Eof)
		{
			TDijeteZaPreporuku dijete;

			dijete.idDijete =
				query_djeca->FieldByName("id_dijete")->AsInteger;

			dijete.imePrezime =
				query_djeca->FieldByName("ime")->AsString +
				L" " +
				query_djeca->FieldByName("prezime")->AsString;

			listaDjece.push_back(dijete);

			query_djeca->Next();
		}
	}
	__finally
	{
		query_djeca->EnableControls();
	}

	int odgovor = MessageDlg(
		L"Generirati i spremiti preporuke za " +
		IntToStr(static_cast<int>(listaDjece.size())) +
		L" djece iz trenutnog prikaza?\n\n"
		L"Za djecu koja nemaju razvojni pregled preporuka će biti "
		L"preskočena.",
		mtConfirmation,
		TMsgDlgButtons() << mbYes << mbNo,
		0
	);

	if (odgovor != mrYes)
	{
		return;
	}

	form_preporuke_grupno->PokreniZaDjecu(listaDjece);
	form_preporuke_grupno->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall Tform_djeca::button_razvoj_djetetaClick(TObject *Sender)
{
	int idDijete = 0;

	if (!query_djeca->IsEmpty())
	{
		idDijete =
			query_djeca
				->FieldByName("id_dijete")
				->AsInteger;
	}

	form_razvoj->Show();
	this->Hide();

	if (idDijete != 0)
	{
		form_razvoj->OdaberiDijeteID(idDijete);
	}
}
//---------------------------------------------------------------------------

void __fastcall Tform_djeca::button_odjavaClick(TObject *Sender)
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

void __fastcall Tform_djeca::query_djecaCalcFields(TDataSet *DataSet)
{
    if (query_djeca->FieldByName("datum_rodjenja")->IsNull)
    {
        query_djeca->FieldByName("Dob")->Clear();
        return;
    }

    TDateTime datumRodjenja =
        query_djeca->FieldByName("datum_rodjenja")->AsDateTime;

    query_djeca->FieldByName("Dob")->AsInteger =
        razvojPomoc.IzracunajDob(datumRodjenja);
}
//---------------------------------------------------------------------------

void __fastcall Tform_djeca::FormClose(TObject *Sender, TCloseAction &Action)
{
    Application->Terminate();
}
//---------------------------------------------------------------------------

void __fastcall Tform_djeca::grid_djecaDblClick(TObject *Sender)
{
    button_urediClick(Sender);
}
//---------------------------------------------------------------------------
void Tform_djeca::primijeniPrava()
{
	TKorisnickaPrava prava(data_module->currentUserUloga);
    button_dodaj->Enabled =
		prava.dodajDijete();
    button_uredi->Enabled =
		prava.urediDijete();
    button_obrisi->Enabled =
		prava.obrisiDijete();
    button_generiraj_sve->Enabled =
        prava.generirajPreporuku() && prava.spremiPreporuku();
    button_korisnici->Visible =
        prava.korisnici();
    combo_skupina->Visible =
		!prava.samoSvojaSkupina();
    label_skupina->Visible =
        !prava.samoSvojaSkupina();
    if (prava.odgojitelj())
    {
        label_sortiranje->Left =
            label_skupina->Left;

        combo_sortiranje->Left =
            combo_skupina->Left;
    }
    else
    {
        label_sortiranje->Left =
            originalSortLabelLeft;

        combo_sortiranje->Left =
            originalSortComboLeft;
    }
}
void __fastcall Tform_djeca::button_skupineClick(TObject *Sender)
{
	form_skupine->Show();
	this->Hide();
}
//---------------------------------------------------------------------------

void __fastcall Tform_djeca::button_aktivnostiClick(TObject *Sender)
{
	form_aktivnosti->Show();
    this->Hide();
}
//---------------------------------------------------------------------------

void __fastcall Tform_djeca::button_razvojClick(TObject *Sender)
{
    form_razvoj->Show();
    this->Hide();
}
//---------------------------------------------------------------------------

void __fastcall Tform_djeca::button_korisniciClick(TObject *Sender)
{
    form_korisnici->Show();
    this->Hide();
}
//---------------------------------------------------------------------------

void __fastcall Tform_djeca::button_uputeClick(TObject *Sender)
{
    form_upute->Show();
    this->Hide();
}
//---------------------------------------------------------------------------
void __fastcall Tform_djeca::button_zdravljeClick(TObject *Sender)
{
    form_bolesti->Show();
    this->Hide();
}
//---------------------------------------------------------------------------
void __fastcall Tform_djeca::button_najaveClick(TObject *Sender)
{
    form_najave->Show();
    this->Hide();
}
//---------------------------------------------------------------------------

