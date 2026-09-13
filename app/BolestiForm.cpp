//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "BolestiForm.h"
#include "DatabaseModule.h"
#include "DashboardForm.h"
#include "LoginForm.h"
#include "DjecaForm.h"
#include "AktivnostiForm.h"
#include "SkupineForm.h"
#include "RazvojForm.h"
#include "KorisniciForm.h"
#include "UputeForm.h"
#include "NajaveForm.h"
#include "BolestUnosForm.h"
#include "DatumHelper.h"
#include "KorisnickaPrava.h"
#include <System.IOUtils.hpp>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
Tform_bolesti *form_bolesti;
//---------------------------------------------------------------------------
__fastcall Tform_bolesti::Tform_bolesti(TComponent* Owner)
	: TForm(Owner),
	odabraniRedak(-1),
	nizBolesti(NULL)
{
}
//---------------------------------------------------------------------------
String Tform_bolesti::putanjaDatoteke()
{
	return ExtractFilePath(Application->ExeName) + "bolesti.json";
}
//---------------------------------------------------------------------------
void __fastcall Tform_bolesti::FormShow(TObject *Sender)
{
	label_korisnik->Caption =
		data_module->currentUserIme +
		L" " +
		data_module->currentUserPrezime;

	label_uloga->Caption =
		data_module->currentUserUloga;

	TKorisnickaPrava prava(
		data_module->currentUserUloga
	);

	button_korisnici->Visible =
		prava.korisnici();

	ucitajBolesti();
	popuniGrid();
	izracunajUpozorenja();
	odabraniRedak = -1;
}
//---------------------------------------------------------------------------
void __fastcall Tform_bolesti::FormClose(TObject *Sender, TCloseAction &Action)
{
    Application->Terminate();
}
//---------------------------------------------------------------------------
String Tform_bolesti::imeDjeteta(int idDijete)
{
	query_djeca->Close();
	query_djeca->Open();

	String rezultat = L"(nepoznato dijete)";

	while (!query_djeca->Eof)
	{
		if (query_djeca->FieldByName("id_dijete")->AsInteger == idDijete)
		{
			rezultat =
				query_djeca->FieldByName("ime")->AsString + L" " +
				query_djeca->FieldByName("prezime")->AsString;
			break;
		}

		query_djeca->Next();
	}

	query_djeca->Close();
	return rezultat;
}
//---------------------------------------------------------------------------
int Tform_bolesti::skupinaDjeteta(int idDijete)
{
	query_djeca->Close();
	query_djeca->Open();

	int rezultat = 0;

	while (!query_djeca->Eof)
	{
		if (query_djeca->FieldByName("id_dijete")->AsInteger == idDijete)
		{
			rezultat = query_djeca->FieldByName("id_skupina")->AsInteger;
			break;
		}

		query_djeca->Next();
	}

	query_djeca->Close();
	return rezultat;
}
//---------------------------------------------------------------------------
String Tform_bolesti::spolDjeteta(int idDijete)
{
	query_djeca->Close();
	query_djeca->Open();

	String rezultat = L"M";

	while (!query_djeca->Eof)
	{
		if (query_djeca->FieldByName("id_dijete")->AsInteger == idDijete)
		{
			rezultat = query_djeca->FieldByName("spol")->AsString;
			break;
		}

		query_djeca->Next();
	}

	query_djeca->Close();
	return rezultat;
}
//---------------------------------------------------------------------------
void Tform_bolesti::ucitajBolesti()
{
	if (nizBolesti != NULL)
	{
		delete nizBolesti;
		nizBolesti = NULL;
	}

	String putanja = putanjaDatoteke();

	if (!FileExists(putanja))
	{
		nizBolesti = new TJSONArray();
		return;
	}

	String sadrzaj = TFile::ReadAllText(putanja, TEncoding::UTF8);
	TJSONValue *vrijednost = TJSONObject::ParseJSONValue(sadrzaj);

	if (vrijednost == NULL)
	{
		nizBolesti = new TJSONArray();
		return;
	}

	TJSONObject *objekt = dynamic_cast<TJSONObject*>(vrijednost);

	if (objekt == NULL)
	{
		delete vrijednost;
		nizBolesti = new TJSONArray();
		return;
	}

	TJSONValue *nizVrijednost = objekt->GetValue("bolesti");

	if (nizVrijednost == NULL)
	{
		nizBolesti = new TJSONArray();
	}
	else
	{
		nizBolesti = dynamic_cast<TJSONArray*>(nizVrijednost->Clone());

		if (nizBolesti == NULL)
		{
			nizBolesti = new TJSONArray();
		}
	}

	delete objekt;
}
//---------------------------------------------------------------------------
void Tform_bolesti::spremiBolesti()
{
	TJSONObject *korijen = new TJSONObject();
	korijen->AddPair(
		"bolesti",
		static_cast<TJSONArray*>(nizBolesti->Clone()));

	TFile::WriteAllText(putanjaDatoteke(), korijen->ToJSON(), TEncoding::UTF8);

	delete korijen;
}
//---------------------------------------------------------------------------
void Tform_bolesti::popuniGrid()
{
	grid_bolesti->RowCount = nizBolesti->Count + 1;

	grid_bolesti->Cells[0][0] = L"Dijete";
	grid_bolesti->Cells[1][0] = L"Od";
	grid_bolesti->Cells[2][0] = L"Do";
	grid_bolesti->Cells[3][0] = L"Razlog";
	grid_bolesti->Cells[4][0] = L"Napomena";

	for (int i = 0; i < nizBolesti->Count; i++)
	{
		TJSONObject *zapis = static_cast<TJSONObject*>(nizBolesti->Items[i]);

		int idDijete = StrToIntDef(zapis->GetValue("idDijete")->Value(), 0);

		grid_bolesti->Cells[0][i + 1] = imeDjeteta(idDijete);
		grid_bolesti->Cells[1][i + 1] = zapis->GetValue("od")->Value();
		grid_bolesti->Cells[2][i + 1] = zapis->GetValue("do")->Value();
		grid_bolesti->Cells[3][i + 1] = zapis->GetValue("razlog")->Value();
		grid_bolesti->Cells[4][i + 1] = zapis->GetValue("napomena")->Value();
	}
}
//---------------------------------------------------------------------------
bool Tform_bolesti::preklapanjeDatuma(int idDijete, String od, String doDatuma, int preskociIndeks)
{
	TDate noviOd = ParsirajDatum(od);
	TDate noviDo = ParsirajDatum(doDatuma);

	for (int i = 0; i < nizBolesti->Count; i++)
	{
		if (i == preskociIndeks)
		{
			continue;
		}

		TJSONObject *zapis = static_cast<TJSONObject*>(nizBolesti->Items[i]);
		int idDijeteZapisa = StrToIntDef(zapis->GetValue("idDijete")->Value(), 0);

		if (idDijeteZapisa != idDijete)
		{
			continue;
		}

		TDate postojeciOd = ParsirajDatum(zapis->GetValue("od")->Value());
		TDate postojeciDo = ParsirajDatum(zapis->GetValue("do")->Value());

		if (noviOd <= postojeciDo && postojeciOd <= noviDo)
		{
			return true;
		}
	}

	return false;
}
//---------------------------------------------------------------------------
void Tform_bolesti::izracunajUpozorenja()
{
	memo_upozorenja->Lines->Clear();

	TDate danas = Date();
	TDate granica30 = danas - 30;
	TDate granica14 = danas - 14;

	TStringList *obradjenaDjeca = new TStringList();
	TStringList *obradjeneSkupine = new TStringList();

	try
	{
		for (int i = 0; i < nizBolesti->Count; i++)
		{
			TJSONObject *zapis = static_cast<TJSONObject*>(nizBolesti->Items[i]);
			int idDijete = StrToIntDef(zapis->GetValue("idDijete")->Value(), 0);
			String kljuc = IntToStr(idDijete);

			if (obradjenaDjeca->IndexOf(kljuc) >= 0)
			{
				continue;
			}

			int brojac = 0;

			for (int j = 0; j < nizBolesti->Count; j++)
			{
				TJSONObject *zapisJ = static_cast<TJSONObject*>(nizBolesti->Items[j]);
				int idDijeteJ = StrToIntDef(zapisJ->GetValue("idDijete")->Value(), 0);
				TDate odJ = ParsirajDatum(zapisJ->GetValue("od")->Value());
				String razlogJ = zapisJ->GetValue("razlog")->Value();

				if (idDijeteJ == idDijete && odJ >= granica30 && razlogJ == L"Bolest")
				{
					brojac++;
				}
			}

			obradjenaDjeca->Add(kljuc);

			if (brojac >= 3)
			{
				String glagol =
					(spolDjeteta(idDijete) == L"Ž") ? L"bolovala" : L"bolovao";

				memo_upozorenja->Lines->Add(
					L"Upozorenje: " + imeDjeteta(idDijete) +
					L" je " + glagol + L" " + IntToStr(brojac) +
					L" puta u zadnjih 30 dana.");
			}
		}

		for (int i = 0; i < nizBolesti->Count; i++)
		{
			TJSONObject *zapis = static_cast<TJSONObject*>(nizBolesti->Items[i]);
			int idDijete = StrToIntDef(zapis->GetValue("idDijete")->Value(), 0);
			int idSkupine = skupinaDjeteta(idDijete);

			if (idSkupine == 0)
			{
				continue;
			}

			String kljucSkupine = IntToStr(idSkupine);

			if (obradjeneSkupine->IndexOf(kljucSkupine) >= 0)
			{
				continue;
			}

			TStringList *djecaUSkupini = new TStringList();
			djecaUSkupini->Sorted = true;
			djecaUSkupini->Duplicates = System::Classes::dupIgnore;

			try
			{
				for (int j = 0; j < nizBolesti->Count; j++)
				{
					TJSONObject *zapisJ = static_cast<TJSONObject*>(nizBolesti->Items[j]);
					int idDijeteJ = StrToIntDef(zapisJ->GetValue("idDijete")->Value(), 0);
					TDate odJ = ParsirajDatum(zapisJ->GetValue("od")->Value());
					String razlogJ = zapisJ->GetValue("razlog")->Value();

					if (skupinaDjeteta(idDijeteJ) == idSkupine &&
						odJ >= granica14 &&
						razlogJ == L"Bolest")
					{
						djecaUSkupini->Add(IntToStr(idDijeteJ));
					}
				}

				obradjeneSkupine->Add(kljucSkupine);

				if (djecaUSkupini->Count >= 3)
				{
					memo_upozorenja->Lines->Add(
						L"Upozorenje: moguća epidemija - " +
						IntToStr(djecaUSkupini->Count) +
						L" različite djece iz iste skupine bolesno u zadnjih 14 dana.");
				}
			}
			__finally
			{
				delete djecaUSkupini;
			}
		}

		if (memo_upozorenja->Lines->Count == 0)
		{
			memo_upozorenja->Lines->Add(L"Nema aktivnih upozorenja.");
		}
	}
	__finally
	{
		delete obradjenaDjeca;
		delete obradjeneSkupine;
	}

	memo_upozorenja->SelStart = 0;
	memo_upozorenja->SelLength = 0;
	SendMessage(memo_upozorenja->Handle, EM_SCROLLCARET, 0, 0);
}
//---------------------------------------------------------------------------
void __fastcall Tform_bolesti::grid_bolestiClick(TObject *Sender)
{
	odabraniRedak = grid_bolesti->Row - 1;

	if (odabraniRedak < 0 || odabraniRedak >= nizBolesti->Count)
	{
		odabraniRedak = -1;
	}
}
//---------------------------------------------------------------------------
void __fastcall Tform_bolesti::grid_bolestiDblClick(TObject *Sender)
{
	button_urediClick(Sender);
}
//---------------------------------------------------------------------------
void __fastcall Tform_bolesti::button_dodajClick(TObject *Sender)
{
	form_bolest_unos->pripremiZaDodavanje();

	if (form_bolest_unos->ShowModal() != mrOk)
	{
		return;
	}

	if (preklapanjeDatuma(
			form_bolest_unos->rezultatIdDijete,
			form_bolest_unos->rezultatOd,
			form_bolest_unos->rezultatDo,
			-1))
	{
		ShowMessage(L"Ovo dijete već ima zapis koji se vremenski preklapa s unesenim razdobljem.");
		return;
	}

	TJSONObject *noviZapis = new TJSONObject();
	noviZapis->AddPair("idDijete", new TJSONNumber(form_bolest_unos->rezultatIdDijete));
	noviZapis->AddPair("od", new TJSONString(form_bolest_unos->rezultatOd));
	noviZapis->AddPair("do", new TJSONString(form_bolest_unos->rezultatDo));
	noviZapis->AddPair("razlog", new TJSONString(form_bolest_unos->rezultatRazlog));
	noviZapis->AddPair("napomena", new TJSONString(form_bolest_unos->rezultatNapomena));

	nizBolesti->AddElement(noviZapis);

	spremiBolesti();
	popuniGrid();
	izracunajUpozorenja();
}
//---------------------------------------------------------------------------
void __fastcall Tform_bolesti::button_urediClick(TObject *Sender)
{
	if (odabraniRedak < 0 || odabraniRedak >= nizBolesti->Count)
	{
		ShowMessage(L"Odaberite zapis za uređivanje (klikom na redak u tablici).");
		return;
	}

	TJSONObject *zapis = static_cast<TJSONObject*>(nizBolesti->Items[odabraniRedak]);

	form_bolest_unos->pripremiZaUredjivanje(
		StrToIntDef(zapis->GetValue("idDijete")->Value(), 0),
		zapis->GetValue("od")->Value(),
		zapis->GetValue("do")->Value(),
		zapis->GetValue("razlog")->Value(),
		zapis->GetValue("napomena")->Value());

	if (form_bolest_unos->ShowModal() != mrOk)
	{
		return;
	}

	if (preklapanjeDatuma(
			form_bolest_unos->rezultatIdDijete,
			form_bolest_unos->rezultatOd,
			form_bolest_unos->rezultatDo,
			odabraniRedak))
	{
		ShowMessage(L"Ovo dijete već ima zapis koji se vremenski preklapa s unesenim razdobljem.");
		return;
	}

	TJSONObject *noviZapis = new TJSONObject();
	noviZapis->AddPair("idDijete", new TJSONNumber(form_bolest_unos->rezultatIdDijete));
	noviZapis->AddPair("od", new TJSONString(form_bolest_unos->rezultatOd));
	noviZapis->AddPair("do", new TJSONString(form_bolest_unos->rezultatDo));
	noviZapis->AddPair("razlog", new TJSONString(form_bolest_unos->rezultatRazlog));
	noviZapis->AddPair("napomena", new TJSONString(form_bolest_unos->rezultatNapomena));

	TJSONArray *noviNiz = new TJSONArray();

	for (int i = 0; i < nizBolesti->Count; i++)
	{
		if (i == odabraniRedak)
		{
			continue;
		}

		noviNiz->AddElement(static_cast<TJSONValue*>(nizBolesti->Items[i]->Clone()));
	}

	noviNiz->AddElement(noviZapis);

	delete nizBolesti;
	nizBolesti = noviNiz;

	spremiBolesti();
	popuniGrid();
	izracunajUpozorenja();
	odabraniRedak = -1;
}
//---------------------------------------------------------------------------
void __fastcall Tform_bolesti::button_obrisiClick(TObject *Sender)
{
	if (odabraniRedak < 0 || odabraniRedak >= nizBolesti->Count)
	{
		ShowMessage(L"Odaberite zapis za brisanje (klikom na redak u tablici).");
		return;
	}

	if (MessageDlg(
			L"Sigurno obrisati odabrani zapis?",
			mtConfirmation,
			TMsgDlgButtons() << mbYes << mbNo,
			0) != mrYes)
	{
		return;
	}

	TJSONArray *noviNiz = new TJSONArray();

	for (int i = 0; i < nizBolesti->Count; i++)
	{
		if (i == odabraniRedak)
		{
			continue;
		}

		noviNiz->AddElement(static_cast<TJSONValue*>(nizBolesti->Items[i]->Clone()));
	}

	delete nizBolesti;
	nizBolesti = noviNiz;

	spremiBolesti();
	popuniGrid();
	izracunajUpozorenja();
	odabraniRedak = -1;
}
//---------------------------------------------------------------------------
void __fastcall Tform_bolesti::button_pocetnaClick(TObject *Sender)
{
	form_dashboard->Show();
    this->Hide();
}
//---------------------------------------------------------------------------
void __fastcall Tform_bolesti::button_djecaClick(TObject *Sender)
{
	form_djeca->Show();
	this->Hide();
}
//---------------------------------------------------------------------------
void __fastcall Tform_bolesti::button_aktivnostiClick(TObject *Sender)
{
	form_aktivnosti->Show();
	this->Hide();
}
//---------------------------------------------------------------------------
void __fastcall Tform_bolesti::button_skupineClick(TObject *Sender)
{
	form_skupine->Show();
	this->Hide();
}
//---------------------------------------------------------------------------
void __fastcall Tform_bolesti::button_razvojClick(TObject *Sender)
{
	form_razvoj->Show();
	this->Hide();
}
//---------------------------------------------------------------------------
void __fastcall Tform_bolesti::button_korisniciClick(TObject *Sender)
{
	form_korisnici->Show();
	this->Hide();
}
//---------------------------------------------------------------------------
void __fastcall Tform_bolesti::button_uputeClick(TObject *Sender)
{
	form_upute->Show();
	this->Hide();
}
//---------------------------------------------------------------------------
void __fastcall Tform_bolesti::button_najaveClick(TObject *Sender)
{
	form_najave->Show();
	this->Hide();
}
//---------------------------------------------------------------------------
void __fastcall Tform_bolesti::button_odjavaClick(TObject *Sender)
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
