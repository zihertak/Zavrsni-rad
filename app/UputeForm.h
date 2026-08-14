//---------------------------------------------------------------------------

#ifndef UputeFormH
#define UputeFormH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Imaging.pngimage.hpp>
//---------------------------------------------------------------------------
class Tform_upute : public TForm
{
__published:	// IDE-managed Components
	TPanel *panel_header;
	TLabel *label_naslov;
	TPanel *panel_user;
	TLabel *label_korisnik;
	TLabel *label_uloga;
	TPanel *panel_menu;
	TButton *button_pocetna;
	TButton *button_djeca;
	TButton *button_aktivnosti;
	TButton *button_skupine;
	TButton *button_razvoj;
	TButton *button_korisnici;
	TButton *button_upute;
	TButton *button_zdravlje;
	TButton *button_najave;
	TButton *button_odjava;
	TPanel *panel_content;
	TPanel *panel_alati;
	TLabel *label_naslov_upute;
	TLabel *label_opis;
	TMemo *memo_upute;
	TImage *imgKorisnik;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall button_pocetnaClick(TObject *Sender);
	void __fastcall button_djecaClick(TObject *Sender);
	void __fastcall button_aktivnostiClick(TObject *Sender);
	void __fastcall button_skupineClick(TObject *Sender);
	void __fastcall button_razvojClick(TObject *Sender);
	void __fastcall button_korisniciClick(TObject *Sender);
	void __fastcall button_odjavaClick(TObject *Sender);
	void __fastcall button_zdravljeClick(TObject *Sender);
	void __fastcall button_najaveClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall Tform_upute(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE Tform_upute *form_upute;
//---------------------------------------------------------------------------
#endif
