//---------------------------------------------------------------------------

#ifndef RazvojDetaljiFormH
#define RazvojDetaljiFormH
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
class Tform_razvoj_detalji : public TForm
{
__published:	// IDE-managed Components
	TGroupBox *group_pregled;
	TLabel *label_dijete_naslov;
	TLabel *label_dijete;
	TLabel *label_datum_naslov;
	TLabel *label_datum;
	TLabel *label_odgojitelj_naslov;
	TLabel *label_odgojitelj;
	TLabel *label_tjelesni;
	TLabel *label_tjelesni_zvjezdice;
	TLabel *label_socio;
	TLabel *label_socio_zvjezdice;
	TLabel *label_govorni;
	TLabel *label_govorni_zvjezdice;
	TLabel *label_spoznajni;
	TLabel *label_spoznajni_zvjezdice;
	TLabel *label_napomena;
	TMemo *memo_napomena;
	TButton *button_zatvori;
	TFDQuery *query_detalji;
private:	// User declarations
	String napraviZvjezdice(int ocjena);
    TColor bojaZaOcjenu(int ocjena);
	void postaviZvjezdice(TLabel *labela, int ocjena);
public:		// User declarations
	__fastcall Tform_razvoj_detalji(TComponent* Owner);
	void ucitajPregled(int idRazvojniPregled);
};
//---------------------------------------------------------------------------
extern PACKAGE Tform_razvoj_detalji *form_razvoj_detalji;
//---------------------------------------------------------------------------
#endif
