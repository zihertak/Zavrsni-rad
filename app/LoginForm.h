//---------------------------------------------------------------------------

#ifndef LoginFormH
#define LoginFormH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
//---------------------------------------------------------------------------
class Tform_Login : public TForm
{
__published:	// IDE-managed Components
	TPanel *panel_main;
	TPanel *panel_header;
	TLabel *label_naslov;
	TPanel *panel_login;
	TLabel *label_username;
	TEdit *edit_username;
	TLabel *label_password;
	TEdit *edit_password;
	TButton *button_login;
	TLabel *label_zadnja_prijava;
	void __fastcall button_loginClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall Tform_Login(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE Tform_Login *form_Login;
//---------------------------------------------------------------------------
#endif
