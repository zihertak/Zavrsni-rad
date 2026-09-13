//---------------------------------------------------------------------------

#ifndef PreporukeGrupnoFormH
#define PreporukeGrupnoFormH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <vector>
//---------------------------------------------------------------------------
struct TDijeteZaPreporuku
{
	int idDijete;
	String imePrezime;
};
//---------------------------------------------------------------------------
class Tform_preporuke_grupno : public TForm
{
__published:	// IDE-managed Components
	TLabel *label_naslov;
	TListBox *list_status;
	TLabel *label_napredak;
	TButton *button_zatvori;
	void __fastcall button_zatvoriClick(TObject *Sender);
private:	// User declarations
	std::vector<TDijeteZaPreporuku> djeca;
	int brojGotovih;
public:		// User declarations
	__fastcall Tform_preporuke_grupno(TComponent* Owner);
	void PokreniZaDjecu(const std::vector<TDijeteZaPreporuku> &listaDjece);
	void AzurirajStatus(int indeks, const String &tekst);
};
//---------------------------------------------------------------------------
extern PACKAGE Tform_preporuke_grupno *form_preporuke_grupno;
//---------------------------------------------------------------------------
#endif
