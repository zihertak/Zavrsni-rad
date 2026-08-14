//---------------------------------------------------------------------------

#ifndef IzvjestajFormH
#define IzvjestajFormH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.Graphics.hpp>
#include <System.Types.hpp>
#include <Data.DB.hpp>
#include "RazvojPomoc.h"
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
#include <vector>
//---------------------------------------------------------------------------
struct TPodaciSkupineGraf
{
	String naziv;
	int brojDjece;
	double prosjecnaRazina;
	bool imaRazinu;
};
//---------------------------------------------------------------------------
class Tform_izvjestaj : public TForm
{
__published:	// IDE-managed Components
	TLabel *label_naslov;
	TLabel *label_opis;
	TLabel *label_graf_djece;
	TLabel *label_graf_razine;
	TPaintBox *paint_broj_djece;
	TPaintBox *paint_razina_razvoja;
	TMemo *memo_izvjestaj;
	TButton *button_pdf;
	TButton *button_zatvori;
	TFDQuery *query_izv_skupine;
	TFDQuery *query_izv_djeca;
	TFDQuery *query_izv_procjene;
	TFDQuery *query_izv_odgojitelj;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall button_zatvoriClick(TObject *Sender);
	void __fastcall button_pdfClick(TObject *Sender);
	void __fastcall paint_broj_djecePaint(TObject *Sender);
	void __fastcall paint_razina_razvojaPaint(TObject *Sender);
private:	// User declarations
	std::vector<TPodaciSkupineGraf> podaciGrafova;
	TRazvojPomoc razvojPomoc;
	void generirajIzvjestaj();
	String odgojiteljSkupine(int idSkupina);
	bool prosjecnaRazinaDjeteta(int idDijete, double &trenutna, double &prethodna, bool &postojiPrethodna);
	String odrediTrendIzvjestaj(double trenutna, double prethodna, bool postojiPrethodna);
	void crtajStupcastiGraf(TCanvas *canvas, TRect podrucje, bool crtajBrojDjece);
public:		// User declarations
	__fastcall Tform_izvjestaj(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE Tform_izvjestaj *form_izvjestaj;
//---------------------------------------------------------------------------
#endif
