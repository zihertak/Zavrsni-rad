//---------------------------------------------------------------------------

#ifndef KorisnikUnosFormH
#define KorisnikUnosFormH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Data.DB.hpp>
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
//---------------------------------------------------------------------------
class Tform_korisnik_unos : public TForm
{
__published:	// IDE-managed Components
	TGroupBox *group_osnovni;
	TLabel *label_ime;
	TEdit *edit_ime;
	TLabel *label_prezime;
	TEdit *edit_prezime;
	TLabel *label_username;
	TEdit *edit_username;
	TLabel *label_lozinka;
	TEdit *edit_lozinka;
	TLabel *label_ponovi_lozinku;
	TEdit *edit_ponovi_lozinku;
	TLabel *label_uloga_unos;
	TComboBox *combo_uloga_unos;
	TLabel *label_skupina;
	TComboBox *combo_skupina;
	TButton *button_spremi;
	TButton *button_odustani;
	TFDQuery *query_spremanje;
	TFDQuery *query_korisnik;
	TFDQuery *query_uloge;
	TFDQuery *query_skupine;
	TFDQuery *query_provjera;
	void __fastcall button_odustaniClick(TObject *Sender);
	void __fastcall combo_uloga_unosChange(TObject *Sender);
	void __fastcall button_spremiClick(TObject *Sender);
private:	// User declarations
     int idKorisnik;

    void ucitajUloge();
    void ucitajSkupine();
	void postaviDostupnostSkupine();
	int pronadiStavkuPoID(
		TComboBox *combo,
		int trazeniID
	);
public:		// User declarations
	__fastcall Tform_korisnik_unos(TComponent* Owner);
	void pripremiZaDodavanje();
	void pripremiZaUredjivanje(int odabraniIdKorisnik);
};
//---------------------------------------------------------------------------
extern PACKAGE Tform_korisnik_unos *form_korisnik_unos;
//---------------------------------------------------------------------------
#endif
