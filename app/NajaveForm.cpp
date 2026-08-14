//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "NajaveForm.h"
#include "DatabaseModule.h"
#include "DashboardForm.h"
#include "LoginForm.h"
#include "DjecaForm.h"
#include "AktivnostiForm.h"
#include "SkupineForm.h"
#include "RazvojForm.h"
#include "KorisniciForm.h"
#include "UputeForm.h"
#include "BolestiForm.h"
#include "NajavaUnosForm.h"
#include "DatumHelper.h"
#include "KorisnickaPrava.h"
#include <System.IOUtils.hpp>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
Tform_najave *form_najave;
//---------------------------------------------------------------------------
__fastcall Tform_najave::Tform_najave(TComponent* Owner)
	: TForm(Owner),
	odabraniRedak(-1)
{
}
//---------------------------------------------------------------------------
String Tform_najave::putanjaDatoteke()
{
	return ExtractFilePath(Application->ExeName) + "najave.xml";
}
//---------------------------------------------------------------------------
void __fastcall Tform_najave::FormShow(TObject *Sender)
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

	ucitajNajave();
	popuniGrid();
	odabraniRedak = -1;
}
//---------------------------------------------------------------------------
void __fastcall Tform_najave::FormClose(TObject *Sender, TCloseAction &Action)
{
    Application->Terminate();
}
//---------------------------------------------------------------------------
void Tform_najave::ucitajNajave()
{
	String putanja = putanjaDatoteke();

	if (!FileExists(putanja))
	{
		TFile::WriteAllText(
			putanja,
			L"<?xml version=\"1.0\" encoding=\"UTF-8\"?><najave></najave>",
			TEncoding::UTF8);
	}

	xml_najave->Active = false;
	xml_najave->LoadFromFile(putanja);
	xml_najave->Active = true;
}
//---------------------------------------------------------------------------
void Tform_najave::spremiNajave()
{
	xml_najave->SaveToFile(putanjaDatoteke());
}
//---------------------------------------------------------------------------
void Tform_najave::popuniGrid()
{
	_di_IXMLNode korijen = xml_najave->DocumentElement;

	grid_najave->RowCount = korijen->ChildNodes->Count + 1;

	grid_najave->Cells[0][0] = L"Datum";
	grid_najave->Cells[1][0] = L"Vrsta";
	grid_najave->Cells[2][0] = L"Naziv";
	grid_najave->Cells[3][0] = L"Za koliko dana";
	grid_najave->Cells[4][0] = L"Opis";

	for (int i = 0; i < korijen->ChildNodes->Count; i++)
	{
		_di_IXMLNode cvor = korijen->ChildNodes->Get(i);
		String datum = cvor->ChildNodes->FindNode(L"datum")->Text;

		_di_IXMLNode cvorVrsta = cvor->ChildNodes->FindNode(L"vrsta");
		String vrsta = (cvorVrsta != NULL) ? cvorVrsta->Text : L"Događaj";

		int preostalo = kalendarPomoc.PreostaloDana(ParsirajDatum(datum), Date());

		grid_najave->Cells[0][i + 1] = datum;
		grid_najave->Cells[1][i + 1] = vrsta;
		grid_najave->Cells[2][i + 1] = cvor->ChildNodes->FindNode(L"naziv")->Text;
		grid_najave->Cells[3][i + 1] = tekstPreostalihDana(preostalo);
		grid_najave->Cells[4][i + 1] = cvor->ChildNodes->FindNode(L"opis")->Text;
	}
}
//---------------------------------------------------------------------------
String Tform_najave::tekstPreostalihDana(int preostalo)
{
	if (preostalo < 0)
	{
		return L"Prošlo";
	}

	if (preostalo == 0)
	{
		return L"Danas";
	}

	if (preostalo == 1)
	{
		return L"Sutra";
	}

	return L"Za " + IntToStr(preostalo) + L" dana";
}
//---------------------------------------------------------------------------
TColor Tform_najave::bojaZaVrstu(String vrsta)
{
	if (vrsta == L"Zatvaranje vrtića")
	{
		return static_cast<TColor>(RGB(180, 0, 0));
	}

	if (vrsta == L"Roditeljski sastanak")
	{
		return static_cast<TColor>(RGB(30, 90, 160));
	}

	if (vrsta == L"Podsjetnik")
	{
		return static_cast<TColor>(RGB(180, 120, 0));
	}

	return clWindowText;
}
//---------------------------------------------------------------------------
void __fastcall Tform_najave::grid_najaveDrawCell(
	TObject *Sender, int ACol, int ARow, const TRect &Rect, TGridDrawState State)
{
	if (ARow == 0)
	{
		return;
	}

	_di_IXMLNode korijen = xml_najave->DocumentElement;
	int indeksPodatka = ARow - 1;

	if (indeksPodatka < 0 || indeksPodatka >= korijen->ChildNodes->Count)
	{
		return;
	}

	_di_IXMLNode cvor = korijen->ChildNodes->Get(indeksPodatka);
	TDate datumNajave = ParsirajDatum(cvor->ChildNodes->FindNode(L"datum")->Text);
	int preostalo = kalendarPomoc.PreostaloDana(datumNajave, Date());

	TColor bojaPozadine = (preostalo >= 0 && preostalo <= 3)
		? static_cast<TColor>(RGB(255, 248, 220))
		: clWhite;

	grid_najave->Canvas->Brush->Color = bojaPozadine;
	grid_najave->Canvas->FillRect(Rect);

	grid_najave->Canvas->Font->Color = (ACol == 1)
		? bojaZaVrstu(grid_najave->Cells[ACol][ARow])
		: clWindowText;

	grid_najave->Canvas->TextOut(
		Rect.Left + 4, Rect.Top + 2, grid_najave->Cells[ACol][ARow]);
}
//---------------------------------------------------------------------------
void __fastcall Tform_najave::grid_najaveClick(TObject *Sender)
{
	_di_IXMLNode korijen = xml_najave->DocumentElement;

	odabraniRedak = grid_najave->Row - 1;

	if (odabraniRedak < 0 || odabraniRedak >= korijen->ChildNodes->Count)
	{
		odabraniRedak = -1;
	}
}
//---------------------------------------------------------------------------
void __fastcall Tform_najave::grid_najaveDblClick(TObject *Sender)
{
	button_urediClick(Sender);
}
//---------------------------------------------------------------------------
void __fastcall Tform_najave::button_dodajClick(TObject *Sender)
{
	form_najava_unos->pripremiZaDodavanje();

	if (form_najava_unos->ShowModal() != mrOk)
	{
		return;
	}

	_di_IXMLNode korijen = xml_najave->DocumentElement;
	_di_IXMLNode cvor = korijen->AddChild(L"dogadjaj");

	cvor->AddChild(L"datum")->Text = form_najava_unos->rezultatDatum;
	cvor->AddChild(L"naziv")->Text = form_najava_unos->rezultatNaziv;
	cvor->AddChild(L"opis")->Text = form_najava_unos->rezultatOpis;
	cvor->AddChild(L"vrsta")->Text = form_najava_unos->rezultatVrsta;

	spremiNajave();
	popuniGrid();
}
//---------------------------------------------------------------------------
void __fastcall Tform_najave::button_urediClick(TObject *Sender)
{
	_di_IXMLNode korijen = xml_najave->DocumentElement;

	if (odabraniRedak < 0 || odabraniRedak >= korijen->ChildNodes->Count)
	{
		ShowMessage(L"Odaberite najavu za uređivanje (klikom na redak u tablici).");
		return;
	}

	_di_IXMLNode cvor = korijen->ChildNodes->Get(odabraniRedak);
	_di_IXMLNode cvorVrsta = cvor->ChildNodes->FindNode(L"vrsta");
	String postojecaVrsta = (cvorVrsta != NULL) ? cvorVrsta->Text : L"Događaj";

	form_najava_unos->pripremiZaUredjivanje(
		cvor->ChildNodes->FindNode(L"datum")->Text,
		cvor->ChildNodes->FindNode(L"naziv")->Text,
		cvor->ChildNodes->FindNode(L"opis")->Text,
		postojecaVrsta);

	if (form_najava_unos->ShowModal() != mrOk)
	{
		return;
	}

	cvor->ChildNodes->FindNode(L"datum")->Text = form_najava_unos->rezultatDatum;
	cvor->ChildNodes->FindNode(L"naziv")->Text = form_najava_unos->rezultatNaziv;
	cvor->ChildNodes->FindNode(L"opis")->Text = form_najava_unos->rezultatOpis;

	cvorVrsta = cvor->ChildNodes->FindNode(L"vrsta");

	if (cvorVrsta != NULL)
	{
		cvorVrsta->Text = form_najava_unos->rezultatVrsta;
	}
	else
	{
		cvor->AddChild(L"vrsta")->Text = form_najava_unos->rezultatVrsta;
	}

	spremiNajave();
	popuniGrid();
	odabraniRedak = -1;
}
//---------------------------------------------------------------------------
void __fastcall Tform_najave::button_obrisiClick(TObject *Sender)
{
	_di_IXMLNode korijen = xml_najave->DocumentElement;

	if (odabraniRedak < 0 || odabraniRedak >= korijen->ChildNodes->Count)
	{
		ShowMessage(L"Odaberite najavu za brisanje (klikom na redak u tablici).");
		return;
	}

	if (MessageDlg(
			L"Sigurno obrisati odabranu najavu?",
			mtConfirmation,
			TMsgDlgButtons() << mbYes << mbNo,
			0) != mrYes)
	{
		return;
	}

	korijen->ChildNodes->Delete(odabraniRedak);

	spremiNajave();
	popuniGrid();
	odabraniRedak = -1;
}
//---------------------------------------------------------------------------
void __fastcall Tform_najave::button_pocetnaClick(TObject *Sender)
{
	form_dashboard->Show();
    this->Hide();
}
//---------------------------------------------------------------------------
void __fastcall Tform_najave::button_djecaClick(TObject *Sender)
{
	form_djeca->Show();
	this->Hide();
}
//---------------------------------------------------------------------------
void __fastcall Tform_najave::button_aktivnostiClick(TObject *Sender)
{
	form_aktivnosti->Show();
	this->Hide();
}
//---------------------------------------------------------------------------
void __fastcall Tform_najave::button_skupineClick(TObject *Sender)
{
	form_skupine->Show();
	this->Hide();
}
//---------------------------------------------------------------------------
void __fastcall Tform_najave::button_razvojClick(TObject *Sender)
{
	form_razvoj->Show();
	this->Hide();
}
//---------------------------------------------------------------------------
void __fastcall Tform_najave::button_korisniciClick(TObject *Sender)
{
	form_korisnici->Show();
	this->Hide();
}
//---------------------------------------------------------------------------
void __fastcall Tform_najave::button_uputeClick(TObject *Sender)
{
	form_upute->Show();
	this->Hide();
}
//---------------------------------------------------------------------------
void __fastcall Tform_najave::button_zdravljeClick(TObject *Sender)
{
	form_bolesti->Show();
	this->Hide();
}
//---------------------------------------------------------------------------
void __fastcall Tform_najave::button_odjavaClick(TObject *Sender)
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
