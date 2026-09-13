//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "LoginForm.h"
#include "DashboardForm.h"
#include "DatabaseModule.h"
#include "Sigurnost.h"
#include <Registry.hpp>
//---------------------------------------------------------------------------
static const String REGISTRY_KLJUC = "\\Software\\VrticApp";
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
Tform_Login *form_Login;
//---------------------------------------------------------------------------
__fastcall Tform_Login::Tform_Login(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall Tform_Login::button_loginClick(TObject *Sender)
{
    if (edit_username->Text.Trim().IsEmpty() ||
    edit_password->Text.IsEmpty())
	{
    ShowMessage("Unesite korisničko ime i lozinku.");
    return;
	}
	try
	{
		data_module->connection ->Connected = true;
		data_module->query_login->Close();
		data_module->query_login->SQL->Text =
			"SELECT korisnik.*, "
			"uloga.naziv AS naziv_uloge "
			"FROM korisnik "
			"JOIN uloga "
			"ON korisnik.id_uloga = uloga.id_uloga "
			"WHERE korisnik.username = :username "
			"AND korisnik.aktivan = 1";
		data_module->query_login
			->ParamByName("username")
			->AsString = edit_username->Text.Trim();
		data_module->query_login->Open();

		bool lozinkaIspravna = false;
		if (!data_module->query_login->IsEmpty())
		{
			String pohranjeniHash =
				data_module->query_login
					->FieldByName("lozinka")
					->AsString;
			String sol = GenerirajSol(edit_username->Text.Trim());
			String unesenaLozinkaHash =
				Hashiraj(edit_password->Text, sol);
			lozinkaIspravna = (pohranjeniHash == unesenaLozinkaHash);
		}

		if (lozinkaIspravna)
		{
			data_module->currentUserID =
				data_module->query_login
					->FieldByName("id_korisnik")
					->AsInteger;
			data_module->currentUserIme	 =
				data_module->query_login
					->FieldByName("ime")
					->AsString;
			data_module->currentUserPrezime	 =
				data_module->query_login
					->FieldByName("prezime")
					->AsString;
			data_module->currentUserUloga		 =
				data_module->query_login
					->FieldByName("naziv_uloge")
					->AsString;
            if (data_module->query_login
				->FieldByName("id_skupina")
				->IsNull)
			{
				data_module->currentUserSkupinaID = 0;
			}
			else
			{
				data_module->currentUserSkupinaID =
					data_module->query_login
						->FieldByName("id_skupina")
						->AsInteger;
			}
			data_module->trenutniKorisnik.Postavi(
				data_module->currentUserID,
				data_module->currentUserIme,
				data_module->currentUserPrezime,
				data_module->currentUserUloga,
				data_module->currentUserSkupinaID,
				true);

			TRegistry *registar = new TRegistry();
			try
			{
				registar->RootKey = HKEY_CURRENT_USER;
				if (registar->OpenKey(REGISTRY_KLJUC, true))
				{
					registar->WriteString(
						"ZadnjiUsername",
						edit_username->Text.Trim());

					registar->WriteString(
						"ZadnjaPrijava",
						DateTimeToStr(Now()));
				}
			}
			__finally
			{
				delete registar;
			}

			form_dashboard->Show();
			this->Hide();
		}
		else
		{
			data_module->query_login->Close();
			ShowMessage("Neispravno korisničko ime ili lozinka.");
			edit_password->Clear();
			edit_password->SetFocus();
		}
	}
	catch (const Exception &e)
	{
		ShowMessage("Greška pri prijavi." + e.Message);
	}
}
//---------------------------------------------------------------------------
void __fastcall Tform_Login::FormShow(TObject *Sender)
{
	TRegistry *registar = new TRegistry();
	try
	{
		registar->RootKey = HKEY_CURRENT_USER;
		if (registar->OpenKey(REGISTRY_KLJUC, true))
		{
			if (registar->ValueExists("ZadnjiUsername"))
			{
				edit_username->Text = registar->ReadString("ZadnjiUsername");
			}

			if (registar->ValueExists("ZadnjaPrijava"))
			{
				label_zadnja_prijava->Caption =
					L"Zadnja prijava: " + registar->ReadString("ZadnjaPrijava");
			}
			else
			{
				label_zadnja_prijava->Caption = "";
			}
		}
	}
	__finally
	{
		delete registar;
	}

	edit_username->SetFocus();
}
//---------------------------------------------------------------------------
