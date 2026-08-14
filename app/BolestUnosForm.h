//---------------------------------------------------------------------------

#ifndef BolestUnosFormH
#define BolestUnosFormH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
#include <FireDAC.Comp.Client.hpp>
#include <Data.DB.hpp>
//---------------------------------------------------------------------------
class Tform_bolest_unos : public TForm
{
__published:	// IDE-managed Components
	TGroupBox *group_podaci;
	TLabel *label_dijete;
	TComboBox *combo_dijete;
	TLabel *label_od;
	TDateTimePicker *date_od;
	TLabel *label_do;
	TDateTimePicker *date_do;
	TLabel *label_razlog;
	TComboBox *combo_razlog;
	TLabel *label_napomena;
	TEdit *edit_napomena;
	TButton *button_spremi;
	TButton *button_odustani;
	TFDQuery *query_djeca;
	void __fastcall button_spremiClick(TObject *Sender);
private:	// User declarations
	void ucitajDjecu();
public:		// User declarations
	__fastcall Tform_bolest_unos(TComponent* Owner);
	int rezultatIdDijete;
	String rezultatOd;
	String rezultatDo;
	String rezultatRazlog;
	String rezultatNapomena;
	void pripremiZaDodavanje();
	void pripremiZaUredjivanje(
		int idDijete,
		String od,
		String doDatuma,
		String razlog,
		String napomena);
};
//---------------------------------------------------------------------------
extern PACKAGE Tform_bolest_unos *form_bolest_unos;
//---------------------------------------------------------------------------
#endif
