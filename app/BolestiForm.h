//---------------------------------------------------------------------------

#ifndef BolestiFormH
#define BolestiFormH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Imaging.pngimage.hpp>
#include <FireDAC.Comp.Client.hpp>
#include <System.JSON.hpp>
#include <Data.DB.hpp>
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
//---------------------------------------------------------------------------
class Tform_bolesti : public TForm
{
__published:	// IDE-managed Components
	TFDQuery *query_djeca;
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
	TPanel *panel_header;
	TLabel *label_naslov;
	TPanel *panel_user;
	TLabel *label_korisnik;
	TLabel *label_uloga;
	TImage *imgKorisnik;
	TPanel *panel_content;
	TPanel *panel_alati;
	TLabel *label_naslov_bolesti;
	TLabel *label_opis;
	TPanel *panel_upozorenja;
	TMemo *memo_upozorenja;
	TPanel *panel_grid;
	TStringGrid *grid_bolesti;
	TPanel *panel_gumbi;
	TButton *button_dodaj;
	TButton *button_uredi;
	TButton *button_obrisi;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall button_pocetnaClick(TObject *Sender);
	void __fastcall button_djecaClick(TObject *Sender);
	void __fastcall button_aktivnostiClick(TObject *Sender);
	void __fastcall button_skupineClick(TObject *Sender);
	void __fastcall button_razvojClick(TObject *Sender);
	void __fastcall button_korisniciClick(TObject *Sender);
	void __fastcall button_uputeClick(TObject *Sender);
	void __fastcall button_najaveClick(TObject *Sender);
	void __fastcall button_odjavaClick(TObject *Sender);
	void __fastcall grid_bolestiClick(TObject *Sender);
	void __fastcall grid_bolestiDblClick(TObject *Sender);
	void __fastcall button_dodajClick(TObject *Sender);
	void __fastcall button_urediClick(TObject *Sender);
	void __fastcall button_obrisiClick(TObject *Sender);
private:	// User declarations
	int odabraniRedak;
	TJSONArray *nizBolesti;
	void ucitajDjecu();
	void ucitajBolesti();
	void popuniGrid();
	void spremiBolesti();
	void izracunajUpozorenja();
	String putanjaDatoteke();
	String imeDjeteta(int idDijete);
	int skupinaDjeteta(int idDijete);
	String spolDjeteta(int idDijete);
	bool preklapanjeDatuma(int idDijete, String od, String doDatuma, int preskociIndeks);
public:		// User declarations
	__fastcall Tform_bolesti(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE Tform_bolesti *form_bolesti;
//---------------------------------------------------------------------------
#endif
