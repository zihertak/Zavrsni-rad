//---------------------------------------------------------------------------

#ifndef KorisniciFormH
#define KorisniciFormH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Data.DB.hpp>
#include <Vcl.DBGrids.hpp>
#include <Vcl.Grids.hpp>
#include <FireDAC.Comp.Client.hpp>
#include <FireDAC.Comp.DataSet.hpp>
#include <FireDAC.DApt.hpp>
#include <FireDAC.DApt.Intf.hpp>
#include <FireDAC.DatS.hpp>
#include <FireDAC.Phys.Intf.hpp>
#include <FireDAC.Stan.Async.hpp>
#include <FireDAC.Stan.Error.hpp>
#include <FireDAC.Stan.Intf.hpp>
#include <FireDAC.Stan.Option.hpp>
#include <FireDAC.Stan.Param.hpp>
#include <Vcl.Imaging.pngimage.hpp>
//---------------------------------------------------------------------------
class Tform_korisnici : public TForm
{
__published:	// IDE-managed Components
	TPanel *panel_header;
	TLabel *label_naslov;
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
	TLabel *label_naslov_korisnici;
	TLabel *label_opis;
	TPanel *panel_filter;
	TLabel *label_pretraga;
	TLabel *label_uloga_filter;
	TLabel *label_sortiranje;
	TEdit *edit_pretraga;
	TComboBox *combo_uloga;
	TComboBox *combo_sortiranje;
	TPanel *panel_gumbi;
	TButton *button_dodaj;
	TButton *button_uredi;
	TButton *button_status;
	TPanel *panel_grid;
	TDBGrid *grid_korisnici;
	TFDQuery *query_korisnici;
	TFDQuery *query_uloge;
	TDataSource *source_korisnici;
	TFDQuery *query_status;
	TFDQuery *query_provjera_admina;
	TPanel *panel_user;
	TLabel *label_korisnik;
	TLabel *label_uloga;
	TImage *imgKorisnik;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall button_pocetnaClick(TObject *Sender);
	void __fastcall button_djecaClick(TObject *Sender);
	void __fastcall button_aktivnostiClick(TObject *Sender);
	void __fastcall button_skupineClick(TObject *Sender);
	void __fastcall button_razvojClick(TObject *Sender);
	void __fastcall button_odjavaClick(TObject *Sender);
	void __fastcall edit_pretragaChange(TObject *Sender);
	void __fastcall combo_ulogaChange(TObject *Sender);
	void __fastcall combo_sortiranjeChange(TObject *Sender);
	void __fastcall button_dodajClick(TObject *Sender);
	void __fastcall button_urediClick(TObject *Sender);
	void __fastcall grid_korisniciDblClick(TObject *Sender);
	void __fastcall button_statusClick(TObject *Sender);
	void __fastcall grid_korisniciCellClick(TColumn *Column);
	void __fastcall button_uputeClick(TObject *Sender);
	void __fastcall button_zdravljeClick(TObject *Sender);
	void __fastcall button_najaveClick(TObject *Sender);
private:	// User declarations
    void ucitajUloge();
	void osvjeziPopisKorisnika();
	void postaviIzgledGrida();
    void osvjeziGumbStatusa();
public:		// User declarations
	__fastcall Tform_korisnici(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE Tform_korisnici *form_korisnici;
//---------------------------------------------------------------------------
#endif
