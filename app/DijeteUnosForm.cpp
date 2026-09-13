//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "DijeteUnosForm.h"
#include "DatabaseModule.h"
#include "KorisnickaPrava.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
Tform_dijete_unos *form_dijete_unos;
//---------------------------------------------------------------------------
__fastcall Tform_dijete_unos::Tform_dijete_unos(TComponent* Owner)
	: TForm(Owner),
	idDijete(0),
	samoZdravstveniPodaci(false),
	slikaPromijenjena(false)
{
}
//---------------------------------------------------------------------------
void Tform_dijete_unos::primijeniPrava()
{
	TKorisnickaPrava prava(data_module->currentUserUloga);
	samoZdravstveniPodaci =
		prava.strucni() && idDijete != 0;
	edit_ime->ReadOnly =
		samoZdravstveniPodaci;
	edit_prezime->ReadOnly =
		samoZdravstveniPodaci;
	date_datum_rodjenja->Enabled =
		!samoZdravstveniPodaci;
	combo_spol->Enabled =
		!samoZdravstveniPodaci;
	date_datum_upisa->Enabled =
		!samoZdravstveniPodaci;
	edit_kontakt_roditelja->ReadOnly =
		samoZdravstveniPodaci;
	if (prava.odgojitelj())
	{
		combo_skupina->Enabled = false;

		for (int i = 0; i < combo_skupina->Items->Count; i++)
		{
			NativeInt trenutniId =
				reinterpret_cast<NativeInt>(
					combo_skupina->Items->Objects[i]
				);

			if (trenutniId ==
				data_module->currentUserSkupinaID)
			{
				combo_skupina->ItemIndex = i;
				break;
			}
		}
	}
	else
	{
		combo_skupina->Enabled =
			!samoZdravstveniPodaci;
	}
	memo_alergije->ReadOnly = false;
	memo_posebne_potrebe->ReadOnly = false;
	memo_zdravstvene_napomene->ReadOnly = false;
}

void Tform_dijete_unos::ucitajSkupine()
{
	combo_skupina->Items->Clear();
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
	combo_skupina->ItemIndex = -1;
}
void Tform_dijete_unos::pripremiZaDodavanje()
{
	idDijete = 0;
	Caption ="Dodaj dijete";
	edit_ime->Clear();
	edit_prezime->Clear();
	edit_kontakt_roditelja->Clear();
	memo_alergije->Clear();
	memo_posebne_potrebe->Clear();
	memo_zdravstvene_napomene->Clear();
	combo_spol->Items->Clear();
	combo_spol->Items->Add("M");
	combo_spol->Items->Add(L"Ž");
	combo_spol->ItemIndex = -1;
	ucitajSkupine();
	date_datum_rodjenja->Date = Date() - 365 * 3;
	date_datum_upisa->Date = Date();
	odabranaSlikaPutanja = "";
	slikaPromijenjena = false;
	image_slika->Picture->Assign(NULL);
	button_uvezi_profil->Visible = false;
	primijeniPrava();
}
void __fastcall Tform_dijete_unos::button_spremiClick(TObject *Sender)
{
	if (SpremiPodatke())
	{
		ModalResult = mrOk;
	}
}
//---------------------------------------------------------------------------
bool Tform_dijete_unos::SpremiPodatke()
{
	TKorisnickaPrava prava(data_module->currentUserUloga);
	if (prava.strucni())
	{
		if (idDijete == 0)
		{
			ShowMessage(
				L"Nemate pravo dodavanja djece."
			);
			return false;
		}
		try
		{
			data_module->connection->StartTransaction();
			query_spremanje->Close();
			query_spremanje->SQL->Clear();
			query_spremanje->SQL->Add(
				"UPDATE zdravstveni_podaci SET "
				"alergije = :alergije, "
				"posebne_potrebe = :posebne_potrebe, "
				"zdravstvene_napomene = :zdravstvene_napomene "
				"WHERE id_dijete = :id_dijete"
			);
			query_spremanje
				->ParamByName("alergije")
				->AsString =
				memo_alergije->Text.Trim();
			query_spremanje
				->ParamByName("posebne_potrebe")
				->AsString =
				memo_posebne_potrebe->Text.Trim();
			query_spremanje
				->ParamByName("zdravstvene_napomene")
				->AsString =
				memo_zdravstvene_napomene->Text.Trim();
			query_spremanje
				->ParamByName("id_dijete")
				->AsInteger =
				idDijete;
			query_spremanje->ExecSQL();
			data_module->connection->Commit();
			ShowMessage(
				L"Zdravstveni podaci uspješno su izmijenjeni."
			);
			return true;
		}
		catch (const Exception &e)
		{
			if (data_module->connection->InTransaction)
			{
				data_module->connection->Rollback();
			}
			ShowMessage(
				L"Došlo je do pogreške pri spremanju "
				L"zdravstvenih podataka:\n" +
				e.Message
			);
			return false;
		}
	}

	String ime = edit_ime->Text.Trim();
	String prezime = edit_prezime->Text.Trim();
	String kontaktRoditelja = edit_kontakt_roditelja->Text.Trim();
	if (ime.IsEmpty())
	{
		ShowMessage("Unesite ime djeteta.");
		edit_ime->SetFocus();
		return false;
	}
	if (prezime.IsEmpty())
	{
		ShowMessage("Unesite prezime djeteta.");
		edit_prezime->SetFocus();
		return false;
	}
	if (combo_spol->ItemIndex == -1)
	{
		ShowMessage("Odaberite spol djeteta.");
		combo_spol->SetFocus();
		return false;
	}
	if (kontaktRoditelja.IsEmpty())
	{
		ShowMessage("Unesite kontakt roditelja.");
		edit_kontakt_roditelja->SetFocus();
		return false;
	}
	if (combo_skupina->ItemIndex == -1)
	{
		ShowMessage("Odaberite skupinu.");
		combo_skupina->SetFocus();
		return false;
	}
	if (date_datum_rodjenja->Date > Date())
	{
		ShowMessage(L"Datum rođenja ne može biti u budućnosti.");
		date_datum_rodjenja->SetFocus();
		return false;
	}
	if (date_datum_upisa->Date <= date_datum_rodjenja->Date)
	{
		ShowMessage(L"Datum upisa mora biti nakon datuma rođenja.");
		date_datum_upisa->SetFocus();
		return false;
	}
	NativeInt idSkupina = reinterpret_cast<NativeInt>(
		combo_skupina->Items->Objects[
			combo_skupina->ItemIndex
		]
	);
	if (idDijete != 0)
	{
		try
		{
			data_module->connection->StartTransaction();

			query_spremanje->Close();
			query_spremanje->SQL->Clear();

			String sqlUpdateDijete =
				"UPDATE dijete SET "
				"ime = :ime, "
				"prezime = :prezime, "
				"datum_rodjenja = :datum_rodjenja, "
				"spol = :spol, "
				"datum_upisa = :datum_upisa, "
				"kontakt_roditelj = :kontakt_roditelj, "
				"id_skupina = :id_skupina";

			if (slikaPromijenjena)
			{
				sqlUpdateDijete = sqlUpdateDijete + ", slika = :slika";
			}

			sqlUpdateDijete = sqlUpdateDijete + " WHERE id_dijete = :id_dijete";

			query_spremanje->SQL->Add(sqlUpdateDijete);

			query_spremanje->ParamByName("ime")->AsString =
				edit_ime->Text.Trim();

			query_spremanje->ParamByName("prezime")->AsString =
				edit_prezime->Text.Trim();

			query_spremanje->ParamByName("datum_rodjenja")->AsDateTime =
				date_datum_rodjenja->Date;

			query_spremanje->ParamByName("spol")->AsString =
				combo_spol->Text;

			query_spremanje->ParamByName("datum_upisa")->AsDateTime =
				date_datum_upisa->Date;

			query_spremanje->ParamByName("kontakt_roditelj")->AsString =
				edit_kontakt_roditelja->Text.Trim();

			query_spremanje->ParamByName("id_skupina")->AsInteger =
				static_cast<int>(idSkupina);

			if (slikaPromijenjena)
			{
				if (odabranaSlikaPutanja.IsEmpty())
				{
					query_spremanje->ParamByName("slika")->DataType = ftBlob;
					query_spremanje->ParamByName("slika")->Clear();
				}
				else
				{
					query_spremanje
						->ParamByName("slika")
						->LoadFromFile(odabranaSlikaPutanja, ftBlob);
				}
			}

			query_spremanje->ParamByName("id_dijete")->AsInteger =
				idDijete;

			query_spremanje->ExecSQL();

			query_spremanje->Close();
			query_spremanje->SQL->Clear();

			query_spremanje->SQL->Add(
				"UPDATE zdravstveni_podaci SET "
				"alergije = :alergije, "
				"posebne_potrebe = :posebne_potrebe, "
				"zdravstvene_napomene = :zdravstvene_napomene "
				"WHERE id_dijete = :id_dijete"
			);

			query_spremanje->ParamByName("alergije")->AsString =
				memo_alergije->Text.Trim();

			query_spremanje->ParamByName("posebne_potrebe")->AsString =
				memo_posebne_potrebe->Text.Trim();

			query_spremanje->ParamByName("zdravstvene_napomene")->AsString =
				memo_zdravstvene_napomene->Text.Trim();

			query_spremanje->ParamByName("id_dijete")->AsInteger =
				idDijete;

			query_spremanje->ExecSQL();

			data_module->connection->Commit();

			ShowMessage(
				L"Podaci o djetetu uspješno su izmijenjeni."
			);

			return true;
		}
		catch (const Exception &e)
		{
			if (data_module->connection->InTransaction)
			{
				data_module->connection->Rollback();
			}

			ShowMessage(
				L"Došlo je do pogreške pri izmjeni podataka:\n" +
				e.Message
			);

			return false;
		}
	}
	try
	{
		data_module->connection->StartTransaction();

		query_spremanje->Close();
		query_spremanje->SQL->Clear();

		query_spremanje->SQL->Add(
			"INSERT INTO dijete "
			"(ime, prezime, datum_rodjenja, spol, "
			"datum_upisa, kontakt_roditelj, id_skupina, slika) "
			"VALUES "
			"(:ime, :prezime, :datum_rodjenja, :spol, "
			":datum_upisa, :kontakt_roditelj, :id_skupina, :slika)"
		);

		query_spremanje->ParamByName("ime")->AsString =
			edit_ime->Text.Trim();

		query_spremanje->ParamByName("prezime")->AsString =
			edit_prezime->Text.Trim();

		query_spremanje->ParamByName("datum_rodjenja")->AsDateTime =
			date_datum_rodjenja->Date;

		query_spremanje->ParamByName("spol")->AsString =
			combo_spol->Text;

		query_spremanje->ParamByName("datum_upisa")->AsDateTime =
			date_datum_upisa->Date;

		query_spremanje->ParamByName("kontakt_roditelj")->AsString =
			edit_kontakt_roditelja->Text.Trim();

		query_spremanje->ParamByName("id_skupina")->AsInteger =
			static_cast<int>(idSkupina);

		if (slikaPromijenjena && !odabranaSlikaPutanja.IsEmpty())
		{
			query_spremanje
				->ParamByName("slika")
				->LoadFromFile(odabranaSlikaPutanja, ftBlob);
		}
		else
		{
			query_spremanje->ParamByName("slika")->DataType = ftBlob;
			query_spremanje->ParamByName("slika")->Clear();
		}

		query_spremanje->ExecSQL();

		query_spremanje->Close();
		query_spremanje->SQL->Clear();

		query_spremanje->SQL->Add(
			"SELECT LAST_INSERT_ID() AS novi_id"
		);

		query_spremanje->Open();

		int noviIdDijete =
			query_spremanje->FieldByName("novi_id")->AsInteger;

		query_spremanje->Close();
		query_spremanje->SQL->Clear();

		query_spremanje->SQL->Add(
			"INSERT INTO zdravstveni_podaci "
			"(alergije, posebne_potrebe, "
			"zdravstvene_napomene, id_dijete) "
			"VALUES "
			"(:alergije, :posebne_potrebe, "
			":zdravstvene_napomene, :id_dijete)"
		);

		query_spremanje->ParamByName("alergije")->AsString =
			memo_alergije->Text.Trim();

		query_spremanje->ParamByName("posebne_potrebe")->AsString =
			memo_posebne_potrebe->Text.Trim();

		query_spremanje->ParamByName("zdravstvene_napomene")->AsString =
			memo_zdravstvene_napomene->Text.Trim();

		query_spremanje->ParamByName("id_dijete")->AsInteger =
			noviIdDijete;

		query_spremanje->ExecSQL();

		data_module->connection->Commit();

		idDijete = noviIdDijete;

		ShowMessage(L"Dijete je uspješno spremljeno.");

		return true;
	}
	catch (const Exception &e)
	{
		if (data_module->connection->InTransaction)
		{
			data_module->connection->Rollback();
		}

		ShowMessage(
			L"Došlo je do pogreške pri spremanju podataka:\n" +
			e.Message
		);

		return false;
	}
}
//---------------------------------------------------------------------------
void __fastcall Tform_dijete_unos::button_ucitajSlikuClick(TObject *Sender)
{
	if (open_dialog_slika->Execute())
	{
		odabranaSlikaPutanja = open_dialog_slika->FileName;
		slikaPromijenjena = true;
		image_slika->Picture->LoadFromFile(odabranaSlikaPutanja);
	}
}
//---------------------------------------------------------------------------
void __fastcall Tform_dijete_unos::button_ukloniSlikuClick(TObject *Sender)
{
	odabranaSlikaPutanja = "";
	slikaPromijenjena = true;
	image_slika->Picture->Assign(NULL);
}
//---------------------------------------------------------------------------
void __fastcall Tform_dijete_unos::button_uvezi_profilClick(TObject *Sender)
{
	if (idDijete == 0)
	{
		ShowMessage(L"Prvo spremite dijete prije uvoza razvojnog profila.");
		return;
	}

	if (!open_dialog_profil->Execute())
	{
		return;
	}

	TFileStream *stream = new TFileStream(open_dialog_profil->FileName, fmOpenRead);

	try
	{
		wchar_t imeBuffer[101];
		stream->ReadBuffer(imeBuffer, 100 * sizeof(wchar_t));
		imeBuffer[100] = 0;
		String punoIme = String(imeBuffer).Trim();

		int brojPregleda = 0;
		stream->ReadBuffer(&brojPregleda, sizeof(int));

		int odgovor = MessageDlg(
			L"Uvoziš razvojni profil za: " + punoIme + L" (" +
			IntToStr(brojPregleda) + L" pregleda). Nastaviti?",
			mtConfirmation,
			TMsgDlgButtons() << mbYes << mbNo,
			0);

		if (odgovor != mrYes)
		{
			return;
		}

		try
		{
			data_module->connection->StartTransaction();

			for (int i = 0; i < brojPregleda; i++)
			{
				double datum;
				stream->ReadBuffer(&datum, sizeof(double));

				int brojOcjena;
				stream->ReadBuffer(&brojOcjena, sizeof(int));

				query_uvoz_pregled->Close();
				query_uvoz_pregled->SQL->Clear();

				query_uvoz_pregled->SQL->Add(
					"INSERT INTO razvojni_pregled "
					"(datum, napomena, id_korisnik, id_dijete) "
					"VALUES (:datum, :napomena, :id_korisnik, :id_dijete)"
				);

				query_uvoz_pregled->ParamByName("datum")->AsDateTime = (TDateTime)datum;
				query_uvoz_pregled->ParamByName("napomena")->AsString =
					L"[Uvezeno iz vanjske datoteke]";
				query_uvoz_pregled->ParamByName("id_korisnik")->AsInteger =
					data_module->currentUserID;
				query_uvoz_pregled->ParamByName("id_dijete")->AsInteger = idDijete;

				query_uvoz_pregled->ExecSQL();

				query_uvoz_pregled->Close();
				query_uvoz_pregled->SQL->Clear();
				query_uvoz_pregled->SQL->Add(L"SELECT LAST_INSERT_ID() AS novi_id");
				query_uvoz_pregled->Open();

				int noviPregledID = query_uvoz_pregled->FieldByName("novi_id")->AsInteger;
				query_uvoz_pregled->Close();

				for (int j = 0; j < brojOcjena; j++)
				{
					int idPodrucje;
					int razina;
					stream->ReadBuffer(&idPodrucje, sizeof(int));
					stream->ReadBuffer(&razina, sizeof(int));

					query_uvoz_procjena->Close();
					query_uvoz_procjena->SQL->Clear();

					query_uvoz_procjena->SQL->Add(
						"INSERT INTO procjena_podrucja "
						"(razina, napomena, id_podrucje_razvoja, id_razvojni_pregled) "
						"VALUES (:razina, '', :id_podrucje, :id_pregled)"
					);

					query_uvoz_procjena->ParamByName("razina")->AsInteger = razina;
					query_uvoz_procjena->ParamByName("id_podrucje")->AsInteger = idPodrucje;
					query_uvoz_procjena->ParamByName("id_pregled")->AsInteger = noviPregledID;

					query_uvoz_procjena->ExecSQL();
				}
			}

			data_module->connection->Commit();

			ShowMessage(
				L"Uvezeno " + IntToStr(brojPregleda) + L" razvojnih pregleda.");
		}
		catch (Exception &e)
		{
			data_module->connection->Rollback();
			ShowMessage(L"Greška prilikom uvoza profila:\n" + e.Message);
		}
	}
	__finally
	{
		delete stream;
	}
}
//---------------------------------------------------------------------------
void Tform_dijete_unos::pripremiZaUredjivanje(int odabraniIdDijete)
{
	idDijete = odabraniIdDijete;
	Caption = "Uredi dijete";
	odabranaSlikaPutanja = "";
	slikaPromijenjena = false;
	button_uvezi_profil->Visible = true;
	combo_spol->Items->Clear();
	combo_spol->Items->Add(L"M");
	combo_spol->Items->Add(L"Ž");
	ucitajSkupine();
	query_dijete->Close();
	query_dijete->SQL->Clear();
	query_dijete->SQL->Add(
		"SELECT "
		"d.ime, "
		"d.prezime, "
		"d.datum_rodjenja, "
		"d.spol, "
		"d.datum_upisa, "
		"d.kontakt_roditelj, "
		"d.id_skupina, "
		"d.slika, "
		"z.alergije, "
		"z.posebne_potrebe, "
		"z.zdravstvene_napomene "
		"FROM dijete d "
		"LEFT JOIN zdravstveni_podaci z "
		"ON d.id_dijete = z.id_dijete "
		"WHERE d.id_dijete = :id_dijete"
	);
	query_dijete->ParamByName("id_dijete")->AsInteger =
		idDijete;
	query_dijete->Open();

	if (query_dijete->IsEmpty())
	{
		ShowMessage(L"Dijete nije pronađeno.");
		return;
	}
	edit_ime->Text =
		query_dijete->FieldByName("ime")->AsString;
	edit_prezime->Text =
		query_dijete->FieldByName("prezime")->AsString;
	date_datum_rodjenja->Date =
		query_dijete->FieldByName("datum_rodjenja")->AsDateTime;

	combo_spol->ItemIndex =
		combo_spol->Items->IndexOf(
			query_dijete->FieldByName("spol")->AsString
		);

	date_datum_upisa->Date =
		query_dijete->FieldByName("datum_upisa")->AsDateTime;

	edit_kontakt_roditelja->Text =
		query_dijete->FieldByName("kontakt_roditelj")->AsString;

	int idSkupina =
		query_dijete->FieldByName("id_skupina")->AsInteger;

	combo_skupina->ItemIndex = -1;

	for (int i = 0; i < combo_skupina->Items->Count; i++)
	{
		NativeInt trenutniId =
			reinterpret_cast<NativeInt>(
				combo_skupina->Items->Objects[i]
			);

		if (trenutniId == idSkupina)
		{
			combo_skupina->ItemIndex = i;
			break;
		}
	}

	if (query_dijete->FieldByName("slika")->IsNull)
	{
		image_slika->Picture->Assign(NULL);
	}
	else
	{
		TMemoryStream *tok = new TMemoryStream();
		try
		{
			static_cast<TBlobField*>(
				query_dijete->FieldByName("slika")
			)->SaveToStream(tok);
			tok->Position = 0;
			image_slika->Picture->LoadFromStream(tok);
		}
		__finally
		{
			delete tok;
		}
	}

	memo_alergije->Text =
		query_dijete->FieldByName("alergije")->AsString;

	memo_posebne_potrebe->Text =
		query_dijete->FieldByName("posebne_potrebe")->AsString;

	memo_zdravstvene_napomene->Text =
		query_dijete->FieldByName("zdravstvene_napomene")->AsString;

	primijeniPrava();
}
