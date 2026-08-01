//---------------------------------------------------------------------------

#ifndef SkupinaUnosFormH
#define SkupinaUnosFormH
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
#include <Vcl.Samples.Spin.hpp>
//---------------------------------------------------------------------------
class Tform_skupina_unos : public TForm
{
__published:	// IDE-managed Components
	TLabel *label_naziv;
	TLabel *label_pedagoska_godina;
	TLabel *label_dob_od;
	TLabel *label_dob_do;
	TEdit *edit_naziv;
	TButton *button_spremi;
	TButton *button_odustani;
	TFDQuery *query_spremanje;
	TComboBox *combo_pedagoska_godina;
	TSpinEdit *spin_dob_od;
	TSpinEdit *spin_dob_do;
	void __fastcall button_spremiClick(TObject *Sender);
	void __fastcall button_odustaniClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
private:	// User declarations
    bool uredjivanje;
	int idSkupine;
    void ucitajPedagoskeGodine();
public:		// User declarations
	__fastcall Tform_skupina_unos(TComponent* Owner);
	void pripremiZaDodavanje();
	void pripremiZaUredjivanje(int id);
};
//---------------------------------------------------------------------------
extern PACKAGE Tform_skupina_unos *form_skupina_unos;
//---------------------------------------------------------------------------
#endif
