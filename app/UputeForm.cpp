//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UputeForm.h"
#include "DatabaseModule.h"
#include "LoginForm.h"
#include "DashboardForm.h"
#include "DjecaForm.h"
#include "SkupineForm.h"
#include "AktivnostiForm.h"
#include "RazvojForm.h"
#include "KorisniciForm.h"
#include "KorisnickaPrava.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
Tform_upute *form_upute;
//---------------------------------------------------------------------------
__fastcall Tform_upute::Tform_upute(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall Tform_upute::FormShow(TObject *Sender)
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

	memo_upute->Lines->Clear();

	memo_upute->Lines->Add(L"UPUTE ZA KORIŠTENJE");
	memo_upute->Lines->Add(L"");
	memo_upute->Lines->Add(L"Ova aplikacija namijenjena je upravljanju podacima dječjeg vrtića.");
	memo_upute->Lines->Add(L"Pristup pojedinim funkcijama ovisi o ulozi prijavljenog korisnika.");
	memo_upute->Lines->Add(L"");

	memo_upute->Lines->Add(L"1. PRIJAVA");
	memo_upute->Lines->Add(L"");
	memo_upute->Lines->Add(L"Za prijavu unesite korisničko ime i lozinku.");
	memo_upute->Lines->Add(L"Nakon uspješne prijave otvara se početni zaslon aplikacije.");
	memo_upute->Lines->Add(L"");

	memo_upute->Lines->Add(L"2. POČETNI ZASLON");
	memo_upute->Lines->Add(L"");
	memo_upute->Lines->Add(L"Početni zaslon omogućuje pristup svim modulima aplikacije.");
	memo_upute->Lines->Add(L"Odaberite željeni modul putem izbornika s lijeve strane.");
	memo_upute->Lines->Add(L"");

	memo_upute->Lines->Add(L"3. DJECA");
	memo_upute->Lines->Add(L"");
	memo_upute->Lines->Add(L"Modul Djeca služi za pregled i upravljanje podacima o djeci.");
	memo_upute->Lines->Add(L"Moguće je pretraživanje, filtriranje te dodavanje, uređivanje");
	memo_upute->Lines->Add(L"i brisanje podataka, ovisno o korisničkim ovlastima.");
	memo_upute->Lines->Add(L"");

	memo_upute->Lines->Add(L"4. SKUPINE");
	memo_upute->Lines->Add(L"");
	memo_upute->Lines->Add(L"U ovom modulu upravlja se odgojnim skupinama.");
	memo_upute->Lines->Add(L"Omogućeno je dodavanje, uređivanje i pregled skupina.");
	memo_upute->Lines->Add(L"");

	memo_upute->Lines->Add(L"5. AKTIVNOSTI");
	memo_upute->Lines->Add(L"");
	memo_upute->Lines->Add(L"Modul Aktivnosti služi za evidenciju aktivnosti provedenih");
	memo_upute->Lines->Add(L"s djecom te njihov pregled i uređivanje.");
	memo_upute->Lines->Add(L"");

	memo_upute->Lines->Add(L"6. RAZVOJ");
	memo_upute->Lines->Add(L"");
	memo_upute->Lines->Add(L"Omogućuje vođenje razvojnih pregleda djece te izradu");
	memo_upute->Lines->Add(L"preporuka za daljnji rad.");
	memo_upute->Lines->Add(L"");

	memo_upute->Lines->Add(L"7. KORISNICI");
	memo_upute->Lines->Add(L"");
	memo_upute->Lines->Add(L"Modul je namijenjen administratoru.");
	memo_upute->Lines->Add(L"Omogućuje upravljanje korisničkim računima te njihovim");
	memo_upute->Lines->Add(L"ovlastima i statusom.");
	memo_upute->Lines->Add(L"");

	memo_upute->Lines->Add(L"8. ZAVRŠETAK RADA");
	memo_upute->Lines->Add(L"");
	memo_upute->Lines->Add(L"Po završetku rada zatvorite aplikaciju ili se odjavite");
	memo_upute->Lines->Add(L"ako je ta mogućnost dostupna.");
	memo_upute->Lines->Add(L"");

	memo_upute->Lines->Add(L"NAPOMENA");
	memo_upute->Lines->Add(L"");
	memo_upute->Lines->Add(L"Dostupnost pojedinih funkcija ovisi o ulozi prijavljenog");
	memo_upute->Lines->Add(L"korisnika i dodijeljenim ovlastima.");
	}
//---------------------------------------------------------------------------
void __fastcall Tform_upute::FormClose(TObject *Sender, TCloseAction &Action)
{
    Application->Terminate();
}
//---------------------------------------------------------------------------
void __fastcall Tform_upute::button_pocetnaClick(TObject *Sender)
{
    this->Hide();
	form_dashboard->Show();
}
//---------------------------------------------------------------------------
void __fastcall Tform_upute::button_djecaClick(TObject *Sender)
{
    this->Hide();
	form_djeca->Show();
}
//---------------------------------------------------------------------------
void __fastcall Tform_upute::button_aktivnostiClick(TObject *Sender)
{
    this->Hide();
	form_aktivnosti->Show();
}
//---------------------------------------------------------------------------
void __fastcall Tform_upute::button_skupineClick(TObject *Sender)
{
    this->Hide();
	form_skupine->Show();
}
//---------------------------------------------------------------------------
void __fastcall Tform_upute::button_razvojClick(TObject *Sender)
{
    this->Hide();
	form_razvoj->Show();
}
//---------------------------------------------------------------------------
void __fastcall Tform_upute::button_korisniciClick(TObject *Sender)
{
    this->Hide();
	form_korisnici->Show();
}
//---------------------------------------------------------------------------
void __fastcall Tform_upute::button_odjavaClick(TObject *Sender)
{
    data_module->currentUserID = 0;
    data_module->currentUserIme = L"";
    data_module->currentUserPrezime = L"";
    data_module->currentUserUloga = L"";
    data_module->currentUserSkupinaID = 0;

    form_Login->edit_username->Clear();
    form_Login->edit_password->Clear();

    this->Hide();
    form_dashboard->Hide();

    form_Login->Show();
    form_Login->BringToFront();
	form_Login->edit_username->SetFocus();
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
