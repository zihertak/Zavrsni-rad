//---------------------------------------------------------------------------

#ifndef NajavaUnosFormH
#define NajavaUnosFormH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
#include "TKalendarPomoc.h"
//---------------------------------------------------------------------------
class Tform_najava_unos : public TForm
{
__published:	// IDE-managed Components
	TGroupBox *group_podaci;
	TLabel *label_datum;
	TDateTimePicker *date_datum;
	TLabel *label_naziv;
	TEdit *edit_naziv;
	TLabel *label_vrsta;
	TComboBox *combo_vrsta;
	TLabel *label_opis;
	TMemo *memo_opis;
	TButton *button_spremi;
	TButton *button_odustani;
	void __fastcall button_spremiClick(TObject *Sender);
private:	// User declarations
	TKalendarPomoc kalendarPomoc;
public:		// User declarations
	__fastcall Tform_najava_unos(TComponent* Owner);
	String rezultatDatum;
	String rezultatNaziv;
	String rezultatOpis;
	String rezultatVrsta;
	void pripremiZaDodavanje();
	void pripremiZaUredjivanje(String datum, String naziv, String opis, String vrsta);
};
//---------------------------------------------------------------------------
extern PACKAGE Tform_najava_unos *form_najava_unos;
//---------------------------------------------------------------------------
#endif
