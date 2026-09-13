//---------------------------------------------------------------------------

#ifndef DijeteUnosFormH
#define DijeteUnosFormH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Dialogs.hpp>
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
class Tform_dijete_unos : public TForm
{
__published:	// IDE-managed Components
	TGroupBox *group_osnovni;
	TGroupBox *group_zdravstveni;
	TLabel *label_ime;
	TEdit *edit_ime;
	TLabel *label_prezime;
	TEdit *edit_prezime;
	TLabel *label_datum_rodjenja;
	TDateTimePicker *date_datum_rodjenja;
	TLabel *label_spol;
	TComboBox *combo_spol;
	TLabel *label_datum_upisa;
	TDateTimePicker *date_datum_upisa;
	TLabel *label_kontakt_roditelja;
	TEdit *edit_kontakt_roditelja;
	TLabel *label_skupina;
	TComboBox *combo_skupina;
	TLabel *label_alergije;
	TMemo *memo_alergije;
	TLabel *label_posebne_potrebe;
	TMemo *memo_posebne_potrebe;
	TLabel *label_zdravstvene_napomene;
	TMemo *memo_zdravstvene_napomene;
	TButton *button_spremi;
	TButton *button_odustani;
	TFDQuery *query_skupine;
	TFDQuery *query_spremanje;
	TFDQuery *query_dijete;
	TImage *image_slika;
	TButton *button_ucitajSliku;
	TButton *button_ukloniSliku;
	TOpenDialog *open_dialog_slika;
	TButton *button_uvezi_profil;
	TOpenDialog *open_dialog_profil;
	TFDQuery *query_uvoz_pregled;
	TFDQuery *query_uvoz_procjena;
	void __fastcall button_spremiClick(TObject *Sender);
	void __fastcall button_ucitajSlikuClick(TObject *Sender);
	void __fastcall button_ukloniSlikuClick(TObject *Sender);
	void __fastcall button_uvezi_profilClick(TObject *Sender);
private:	// User declarations
	int idDijete;
	bool samoZdravstveniPodaci;
	String odabranaSlikaPutanja;
	bool slikaPromijenjena;
	void ucitajSkupine();
    void primijeniPrava();
	bool SpremiPodatke();
public:		// User declarations
	__fastcall Tform_dijete_unos(TComponent* Owner);
	void pripremiZaDodavanje();
    void pripremiZaUredjivanje(int odabraniIdDijete);
};
//---------------------------------------------------------------------------
extern PACKAGE Tform_dijete_unos *form_dijete_unos;
//---------------------------------------------------------------------------
#endif
