//---------------------------------------------------------------------------

#ifndef NajaveFormH
#define NajaveFormH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Imaging.pngimage.hpp>
#include <Xml.XMLDoc.hpp>
#include <Xml.XMLIntf.hpp>
#include <Xml.xmldom.hpp>
#include "TKalendarPomoc.h"
//---------------------------------------------------------------------------
class Tform_najave : public TForm
{
__published:	// IDE-managed Components
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
	TLabel *label_naslov_najave;
	TLabel *label_opis;
	TPanel *panel_grid;
	TStringGrid *grid_najave;
	TPanel *panel_gumbi;
	TButton *button_dodaj;
	TButton *button_uredi;
	TButton *button_obrisi;
	TXMLDocument *xml_najave;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall button_pocetnaClick(TObject *Sender);
	void __fastcall button_djecaClick(TObject *Sender);
	void __fastcall button_aktivnostiClick(TObject *Sender);
	void __fastcall button_skupineClick(TObject *Sender);
	void __fastcall button_razvojClick(TObject *Sender);
	void __fastcall button_korisniciClick(TObject *Sender);
	void __fastcall button_uputeClick(TObject *Sender);
	void __fastcall button_zdravljeClick(TObject *Sender);
	void __fastcall button_odjavaClick(TObject *Sender);
	void __fastcall grid_najaveClick(TObject *Sender);
	void __fastcall grid_najaveDblClick(TObject *Sender);
	void __fastcall grid_najaveDrawCell(TObject *Sender, int ACol, int ARow, const TRect &Rect, TGridDrawState State);
	void __fastcall button_dodajClick(TObject *Sender);
	void __fastcall button_urediClick(TObject *Sender);
	void __fastcall button_obrisiClick(TObject *Sender);
private:	// User declarations
	int odabraniRedak;
	TKalendarPomoc kalendarPomoc;
	void ucitajNajave();
	void spremiNajave();
	void popuniGrid();
	String putanjaDatoteke();
	String tekstPreostalihDana(int preostalo);
	TColor bojaZaVrstu(String vrsta);
public:		// User declarations
	__fastcall Tform_najave(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE Tform_najave *form_najave;
//---------------------------------------------------------------------------
#endif
