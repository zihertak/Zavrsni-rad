//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "DashboardForm.h"
#include "DatabaseModule.h"
#include "LoginForm.h"
#include "DjecaForm.h"
#include "KorisnickaPrava.h"
#include "SkupineForm.h"
#include "AktivnostiForm.h"
#include "RazvojForm.h"
#include "KorisniciForm.h"
#include "UputeForm.h"
#include "BolestiForm.h"
#include "NajaveForm.h"
#include <System.JSON.hpp>
#include <System.Net.HttpClient.hpp>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
Tform_dashboard *form_dashboard;
//---------------------------------------------------------------------------
__fastcall Tform_dashboard::Tform_dashboard(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall Tform_dashboard::FormClose(TObject *Sender, TCloseAction &Action)
{
    Application->Terminate();
}
//---------------------------------------------------------------------------
void __fastcall Tform_dashboard::FormShow(TObject *Sender)
{
	label_korisnik->Caption = data_module->trenutniKorisnik.PunoIme();
	label_uloga->Caption = data_module->currentUserUloga;

	TKorisnickaPrava prava(data_module->currentUserUloga);
    button_korisnici->Visible = prava.korisnici();

	dohvatiVrijeme();
}
//---------------------------------------------------------------------------
void Tform_dashboard::dohvatiVrijeme()
{
	label_vrijeme->Caption = L"Dohvaćam vrijeme...";
	label_prikladnost->Caption = L"";

	THTTPClient *http = THTTPClient::Create();

	try
	{
		try
		{
			_di_IHTTPResponse odgovor = http->Get(
				L"https://api.open-meteo.com/v1/forecast?latitude=45.815&longitude=15.9819&current_weather=true");

			String sadrzaj = odgovor->ContentAsString();

			TJSONValue *korijen = TJSONObject::ParseJSONValue(sadrzaj);
			TJSONObject *korijenObjekt = dynamic_cast<TJSONObject*>(korijen);

			if (korijenObjekt == NULL)
			{
				label_vrijeme->Caption = L"Vrijeme trenutno nije dostupno.";
				delete korijen;
				return;
			}

			TJSONObject *trenutno =
				dynamic_cast<TJSONObject*>(korijenObjekt->GetValue("current_weather"));

			if (trenutno == NULL)
			{
				label_vrijeme->Caption = L"Vrijeme trenutno nije dostupno.";
				delete korijenObjekt;
				return;
			}

			TJSONNumber *temperaturaJson =
				dynamic_cast<TJSONNumber*>(trenutno->GetValue("temperature"));
			TJSONNumber *kodJson =
				dynamic_cast<TJSONNumber*>(trenutno->GetValue("weathercode"));

			double temperatura = (temperaturaJson != NULL) ? temperaturaJson->AsDouble : 0;
			int weathercode = (kodJson != NULL) ? kodJson->AsInt : -1;

			label_vrijeme->Caption =
				FormatFloat(L"0.0", temperatura) + L"°C · Zagreb · " + opisVremena(weathercode);

			if (prikladnoZaVani(temperatura, weathercode))
			{
				label_prikladnost->Caption = L"Prikladno za aktivnosti vani";
				label_prikladnost->Font->Color = (TColor)0x173404;
			}
			else
			{
				label_prikladnost->Caption = L"Nije prikladno za aktivnosti vani";
				label_prikladnost->Font->Color = (TColor)0x791F1F;
			}

			delete korijenObjekt;
		}
		__finally
		{
			delete http;
		}
	}
	catch (Exception &e)
	{
		label_vrijeme->Caption = L"Vrijeme trenutno nije dostupno.";
		label_prikladnost->Caption = L"";
	}
}
//---------------------------------------------------------------------------
String Tform_dashboard::opisVremena(int weathercode)
{
	if (weathercode == 0)
	{
		return L"vedro";
	}

	if (weathercode >= 1 && weathercode <= 3)
	{
		return L"djelomično oblačno";
	}

	if (weathercode == 45 || weathercode == 48)
	{
		return L"magla";
	}

	if (weathercode >= 51 && weathercode <= 67)
	{
		return L"kiša";
	}

	if (weathercode >= 71 && weathercode <= 77)
	{
		return L"snijeg";
	}

	if (weathercode >= 80 && weathercode <= 82)
	{
		return L"pljuskovi";
	}

	if (weathercode >= 95)
	{
		return L"grmljavina";
	}

	return L"nepoznato";
}
//---------------------------------------------------------------------------
bool Tform_dashboard::prikladnoZaVani(double temperatura, int weathercode)
{
	if (temperatura < 5)
	{
		return false;
	}

	bool losoVrijeme =
		(weathercode >= 51 && weathercode <= 82) || weathercode >= 95;

	return !losoVrijeme;
}
//---------------------------------------------------------------------------
void __fastcall Tform_dashboard::button_odjavaClick(TObject *Sender)
{
	data_module->currentUserID = 0;
	data_module->currentUserIme = "";
	data_module->currentUserPrezime = "";
	data_module->currentUserUloga = "";
    data_module->currentUserSkupinaID = 0;
    data_module->trenutniKorisnik.Odjava();
	form_Login->edit_username->Clear();
	form_Login->edit_password->Clear();
	form_Login->Show();
	form_Login->edit_username->SetFocus();
	this->Hide();
}
//---------------------------------------------------------------------------
void __fastcall Tform_dashboard::button_djecaClick(TObject *Sender)
{
	 form_djeca->Show();
     this->Hide();
}
//---------------------------------------------------------------------------
void __fastcall Tform_dashboard::button_skupineClick(TObject *Sender)
{
     form_skupine->Show();
	 this->Hide();
}
//---------------------------------------------------------------------------

void __fastcall Tform_dashboard::button_aktivnostiClick(TObject *Sender)
{
	form_aktivnosti->Show();
    this->Hide();
}
//---------------------------------------------------------------------------

void __fastcall Tform_dashboard::button_razvojClick(TObject *Sender)
{
    form_razvoj->Show();
    this->Hide();
}
//---------------------------------------------------------------------------

void __fastcall Tform_dashboard::button_korisniciClick(TObject *Sender)
{
    form_korisnici->Show();
    this->Hide();
}
//---------------------------------------------------------------------------

void __fastcall Tform_dashboard::button_uputeClick(TObject *Sender)
{
    form_upute->Show();
    this->Hide();
}
//---------------------------------------------------------------------------
void __fastcall Tform_dashboard::button_zdravljeClick(TObject *Sender)
{
    form_bolesti->Show();
    this->Hide();
}
//---------------------------------------------------------------------------
void __fastcall Tform_dashboard::button_najaveClick(TObject *Sender)
{
    form_najave->Show();
    this->Hide();
}
//---------------------------------------------------------------------------

