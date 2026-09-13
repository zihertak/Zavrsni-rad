//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "PreporukeGrupnoForm.h"
#include "DatabaseModule.h"
#include "RazvojForm.h"
#include <System.Threading.hpp>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
Tform_preporuke_grupno *form_preporuke_grupno;
//---------------------------------------------------------------------------
using namespace System::Threading;
//---------------------------------------------------------------------------
static String ObradiPreporukuZaDijete(int idDijete)
{
	// Svaki zadatak iz pool-a dobiva vlastitu konekciju na bazu -
	// FireDAC konekcija nije sigurna za istovremeno koristenje
	// iz vise dretvi odjednom.
	TFDConnection *konekcija = new TFDConnection(NULL);
    String rezultatStatus;

	try
	{
		try
		{
			konekcija->Params->Assign(data_module->connection->Params);
			konekcija->Connected = true;

			int zadnjiPregledID = 0;
			int prethodniPregledID = 0;
			TDateTime datumZadnjeg = 0;
			bool postojiPrethodni = false;

			bool postojiPregled =
				form_razvoj->ucitajPregledeZaPreporuke(
					konekcija,
					idDijete,
					zadnjiPregledID,
					prethodniPregledID,
					datumZadnjeg,
					postojiPrethodni
				);

			if (!postojiPregled)
			{
				rezultatStatus = L"preskočeno, nema razvojnog pregleda";
			}
			else
			{
				std::vector<Tform_razvoj::TAnalizaPodrucja> analize =
					form_razvoj->ucitajAnalizePodrucja(
						konekcija,
						zadnjiPregledID,
						prethodniPregledID,
						postojiPrethodni
					);

				if (analize.empty())
				{
					rezultatStatus = L"preskočeno, nema ocjena razvoja";
				}
				else
				{
					form_razvoj->izracunajTezineIPrioritete(analize);

					std::vector<Tform_razvoj::TPreporucenaAktivnost> aktivnosti =
						form_razvoj->odaberiPreporuceneAktivnosti(
							konekcija,
							idDijete,
							analize
						);

					String zakljucak =
						form_razvoj->generirajOpciZakljucak(analize);

					String razlog =
						form_razvoj->generirajOpciRazlog(analize, aktivnosti);

					// Kad vise zadataka istovremeno upisuje retke koji dijele
					// isti strani kljuc (npr. istu preporucenu aktivnost),
					// MySQL to moze prijaviti kao deadlock. To je ocekivano
					// kod istovremenih transakcija, pa se transakcija u tom
					// slucaju jednostavno ponovi.
					const int MAX_POKUSAJA = 3;
					bool gotovo = false;

					konekcija->StartTransaction();

					for (int pokusaj = 1;
						 pokusaj <= MAX_POKUSAJA && !gotovo;
						 pokusaj++)
					{
						try
						{
							int preporukaID =
								form_razvoj->spremiGlavnuPreporuku(
									konekcija,
									zadnjiPregledID,
									L"Predložena",
									razlog,
									zakljucak
								);

							form_razvoj->spremiAktivnostiPreporuke(
								konekcija,
								preporukaID,
								aktivnosti
							);

							konekcija->Commit();

							rezultatStatus =
								L"gotovo, " +
								IntToStr(static_cast<int>(aktivnosti.size())) +
								L" aktivnosti";

							gotovo = true;
						}
						catch (const Exception &e)
						{
							if (konekcija->InTransaction)
							{
								konekcija->Rollback();
							}

							bool jeDeadlock =
								e.Message.Pos(L"Deadlock") > 0;

							if (jeDeadlock && pokusaj < MAX_POKUSAJA)
							{
								TThread::Sleep(50 * pokusaj);
								konekcija->StartTransaction();
							}
							else
							{
								rezultatStatus = L"greška: " + e.Message;
								gotovo = true;
							}
						}
					}
				}
			}
		}
		catch (const Exception &e)
		{
			rezultatStatus = L"greška: " + e.Message;
		}
	}
	__finally
	{
		delete konekcija;
	}

	return rezultatStatus;
}
//---------------------------------------------------------------------------
__fastcall Tform_preporuke_grupno::Tform_preporuke_grupno(TComponent* Owner)
	: TForm(Owner)
{
	brojGotovih = 0;
}
//---------------------------------------------------------------------------
void Tform_preporuke_grupno::PokreniZaDjecu(
	const std::vector<TDijeteZaPreporuku> &listaDjece)
{
	djeca = listaDjece;
	brojGotovih = 0;

    list_status->Items->Clear();

	for (unsigned int i = 0; i < djeca.size(); i++)
	{
		list_status->Items->Add(djeca[i].imePrezime + L" - čeka na obradu...");
	}

	label_napredak->Caption =
		L"Gotovo 0 / " + IntToStr(static_cast<int>(djeca.size()));

	button_zatvori->Enabled = false;

	// Svako dijete je jedan zadatak poslan u zajednicki thread pool
	// (System.Threading / PPL). Pool sam ogranicava koliko se zadataka
	// izvodi istovremeno i raspodjeljuje ih po ogranicenom broju dretvi -
	// ne otvara se posebna dretva za svako dijete.
	for (unsigned int i = 0; i < djeca.size(); i++)
	{
		int idDijete = djeca[i].idDijete;
		String imePrezime = djeca[i].imePrezime;
		int indeks = static_cast<int>(i);
		Tform_preporuke_grupno *forma = this;

		TTask::Run(
			[idDijete, imePrezime, indeks, forma]()
			{
				String rezultat = ObradiPreporukuZaDijete(idDijete);

				// Sucelje (VCL) smije se mijenjati samo iz glavne dretve,
				// pa se azuriranje statusa vraca na nju preko Synchronize.
				TThread::Synchronize(
					NULL,
					[forma, indeks, imePrezime, rezultat]()
					{
						forma->AzurirajStatus(
							indeks,
							imePrezime + L" - " + rezultat
						);
					}
				);
			}
		);
	}
}
//---------------------------------------------------------------------------
void Tform_preporuke_grupno::AzurirajStatus(int indeks, const String &tekst)
{
	if (indeks >= 0 && indeks < list_status->Items->Count)
	{
		list_status->Items->Strings[indeks] = tekst;
	}

	brojGotovih++;

	label_napredak->Caption =
		L"Gotovo " +
		IntToStr(brojGotovih) +
		L" / " +
		IntToStr(static_cast<int>(djeca.size()));

	if (brojGotovih >= static_cast<int>(djeca.size()))
	{
		button_zatvori->Enabled = true;
	}
}
//---------------------------------------------------------------------------
void __fastcall Tform_preporuke_grupno::button_zatvoriClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
