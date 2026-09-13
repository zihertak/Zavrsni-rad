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
#include "BolestiForm.h"
#include "NajaveForm.h"
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
void Tform_upute::DodajNaslov(const String &tekst)
{
	int pocetak = richedit_upute->GetTextLen();

	richedit_upute->Lines->Add(tekst);

	richedit_upute->SelStart = pocetak;
	richedit_upute->SelLength = tekst.Length();
	richedit_upute->SelAttributes->Style = TFontStyles() << fsBold;
}
//---------------------------------------------------------------------------
void Tform_upute::DodajRedak(const String &tekst)
{
	int pocetak = richedit_upute->GetTextLen();

	richedit_upute->Lines->Add(tekst);

	richedit_upute->SelStart = pocetak;
	richedit_upute->SelLength = tekst.Length();
	richedit_upute->SelAttributes->Style = TFontStyles();
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

	richedit_upute->Lines->Clear();

	DodajNaslov(L"UPUTE ZA KORIŠTENJE");
	DodajRedak();
	DodajRedak(L"Ova aplikacija namijenjena je upravljanju podacima dječjeg vrtića.");
	DodajRedak(L"Pristup pojedinim funkcijama ovisi o ulozi prijavljenog korisnika.");
	DodajRedak();

	DodajNaslov(L"1. PRIJAVA");
	DodajRedak();
	DodajRedak(L"Za prijavu unesite korisničko ime i lozinku.");
	DodajRedak(L"Nakon uspješne prijave otvara se početni zaslon aplikacije.");
	DodajRedak();

	DodajNaslov(L"2. POČETNI ZASLON");
	DodajRedak();
	DodajRedak(L"Početni zaslon omogućuje pristup svim modulima aplikacije te");
	DodajRedak(L"prikazuje kratku dobrodošlicu i trenutnu vremensku prognozu za");
	DodajRedak(L"Zagreb (procjena prikladnosti za aktivnosti vani).");
	DodajRedak();

	DodajNaslov(L"3. DJECA");
	DodajRedak();
	DodajRedak(L"Pregled i upravljanje podacima o djeci - osnovni podaci,");
	DodajRedak(L"fotografija i zdravstveni podaci. Moguće je pretraživanje,");
	DodajRedak(L"filtriranje po skupini i sortiranje. Gumb \"Preporuke za sve\"");
	DodajRedak(L"generira razvojne preporuke za svu trenutno prikazanu djecu");
	DodajRedak(L"odjednom, a gumb \"Razvoj\" otvara razvojnu karticu odabranog");
	DodajRedak(L"djeteta.");
	DodajRedak();

	DodajNaslov(L"4. AKTIVNOSTI");
	DodajRedak();
	DodajRedak(L"Evidencija aktivnosti namijenjenih djeci - dodavanje, pregled,");
	DodajRedak(L"uređivanje i povezivanje s razvojnim područjima na koja pojedina");
	DodajRedak(L"aktivnost utječe.");
	DodajRedak();

	DodajNaslov(L"5. SKUPINE");
	DodajRedak();
	DodajRedak(L"Upravljanje odgojnim skupinama - dodavanje, uređivanje i pregled");
	DodajRedak(L"skupina, te izrada izvještaja po skupini (moguć izvoz u PDF).");
	DodajRedak();

	DodajNaslov(L"6. RAZVOJ");
	DodajRedak();
	DodajRedak(L"Unos razvojnih pregleda djece (ocjene po četiri razvojna");
	DodajRedak(L"područja) te izrada preporuka za daljnji rad - pojedinačno za");
	DodajRedak(L"jedno dijete, ili grupno za svu prikazanu djecu odjednom (radi");
	DodajRedak(L"se paralelno, u pozadini). Dostupan je i izvoz/uvoz cjelokupnog");
	DodajRedak(L"razvojnog profila djeteta, npr. pri prelasku u drugi vrtić.");
	DodajRedak();

	DodajNaslov(L"7. KORISNICI");
	DodajRedak();
	DodajRedak(L"Modul je namijenjen administratoru. Omogućuje upravljanje");
	DodajRedak(L"korisničkim računima te njihovim ulogama i statusom");
	DodajRedak(L"(aktivan/neaktivan).");
	DodajRedak();

	DodajNaslov(L"8. ZDRAVLJE");
	DodajRedak();
	DodajRedak(L"Evidencija bolesti i izostanaka djece. Aplikacija upozorava");
	DodajRedak(L"ako je dijete bolesno tri ili više puta u zadnjih 30 dana, ili");
	DodajRedak(L"ako je u istoj skupini u zadnjih 14 dana bolesno troje ili više");
	DodajRedak(L"djece (moguća epidemija).");
	DodajRedak();

	DodajNaslov(L"9. NAJAVE");
	DodajRedak();
	DodajRedak(L"Nadolazeći događaji i obavijesti vrtića - prikazuje broj dana");
	DodajRedak(L"preostalih do događaja i upozorava ako događaj pada na vikend.");
	DodajRedak();

	DodajNaslov(L"10. ZAVRŠETAK RADA");
	DodajRedak();
	DodajRedak(L"Po završetku rada zatvorite aplikaciju ili se odjavite ako je ta");
	DodajRedak(L"mogućnost dostupna.");
	DodajRedak();

	DodajNaslov(L"NAPOMENA");
	DodajRedak();
	DodajRedak(L"Dostupnost pojedinih funkcija ovisi o ulozi prijavljenog");
	DodajRedak(L"korisnika i dodijeljenim ovlastima.");

	richedit_upute->SelStart = 0;
	richedit_upute->SelLength = 0;
	}
//---------------------------------------------------------------------------
void __fastcall Tform_upute::FormClose(TObject *Sender, TCloseAction &Action)
{
    Application->Terminate();
}
//---------------------------------------------------------------------------
void __fastcall Tform_upute::button_pocetnaClick(TObject *Sender)
{
	form_dashboard->Show();
    this->Hide();
}
//---------------------------------------------------------------------------
void __fastcall Tform_upute::button_djecaClick(TObject *Sender)
{
	form_djeca->Show();
    this->Hide();
}
//---------------------------------------------------------------------------
void __fastcall Tform_upute::button_aktivnostiClick(TObject *Sender)
{
	form_aktivnosti->Show();
    this->Hide();
}
//---------------------------------------------------------------------------
void __fastcall Tform_upute::button_skupineClick(TObject *Sender)
{
	form_skupine->Show();
    this->Hide();
}
//---------------------------------------------------------------------------
void __fastcall Tform_upute::button_razvojClick(TObject *Sender)
{
	form_razvoj->Show();
    this->Hide();
}
//---------------------------------------------------------------------------
void __fastcall Tform_upute::button_korisniciClick(TObject *Sender)
{
	form_korisnici->Show();
    this->Hide();
}
//---------------------------------------------------------------------------
void __fastcall Tform_upute::button_zdravljeClick(TObject *Sender)
{
    form_bolesti->Show();
    this->Hide();
}
//---------------------------------------------------------------------------
void __fastcall Tform_upute::button_najaveClick(TObject *Sender)
{
    form_najave->Show();
    this->Hide();
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

    form_Login->Show();
    form_Login->BringToFront();

    this->Hide();
    form_dashboard->Hide();
	form_Login->edit_username->SetFocus();
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
