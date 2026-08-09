//---------------------------------------------------------------------------

#ifndef RazvojFormH
#define RazvojFormH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ComCtrls.hpp>
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
#include <Vcl.Buttons.hpp>
#include <Vcl.DBGrids.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.Imaging.pngimage.hpp>
#include <vector>
#include <algorithm>
//---------------------------------------------------------------------------
class Tform_razvoj : public TForm
{
__published:	// IDE-managed Components
	TPanel *panel_header;
	TLabel *label_naslov;
	TPanel *panel_menu;
	TButton *button_pocetna;
	TButton *button_djeca;
	TButton *button_aktivnosti;
	TButton *button_skupine;
	TButton *button_razvoj;
	TButton *button_korisnici;
	TButton *button_upute;
	TButton *button_odjava;
	TPanel *panel_content;
	TPanel *panel_alati;
	TLabel *label_naslov_razvoj;
	TLabel *label_opis;
	TPanel *panel_odabir_djeteta;
	TLabel *label_dijete;
	TComboBox *combo_dijete;
	TLabel *label_skupina_naslov;
	TLabel *label_skupina;
	TLabel *label_dob_naslov;
	TLabel *label_dob;
	TLabel *label_zadnji_pregled_naslov;
	TLabel *label_zadnji_pregled;
	TPageControl *page_razvoj;
	TTabSheet *tab_procjena;
	TTabSheet *tab_povijest;
	TTabSheet *tab_preporuke;
	TFDQuery *query_djeca;
	TPanel *panel_procjena;
	TLabel *label_tjelesni;
	TLabel *label_socio;
	TLabel *label_govorni;
	TLabel *label_spoznajni;
	TSpeedButton *speed_tjelesni_1;
	TSpeedButton *speed_tjelesni_2;
	TSpeedButton *speed_tjelesni_3;
	TSpeedButton *speed_tjelesni_4;
	TSpeedButton *speed_tjelesni_5;
	TSpeedButton *speed_socio_1;
	TSpeedButton *speed_socio_2;
	TSpeedButton *speed_socio_3;
	TSpeedButton *speed_socio_4;
	TSpeedButton *speed_socio_5;
	TSpeedButton *speed_govorni_1;
	TSpeedButton *speed_govorni_2;
	TSpeedButton *speed_govorni_3;
	TSpeedButton *speed_govorni_4;
	TSpeedButton *speed_govorni_5;
	TSpeedButton *speed_spoznajni_1;
	TSpeedButton *speed_spoznajni_2;
	TSpeedButton *speed_spoznajni_3;
	TSpeedButton *speed_spoznajni_4;
	TSpeedButton *speed_spoznajni_5;
	TLabel *label_napomena;
	TMemo *memo_napomena;
	TButton *button_ocisti;
	TButton *button_spremi;
	TFDQuery *query_spremi_pregled;
	TFDQuery *query_spremi_procjenu;
	TDBGrid *grid_povijest;
	TFDQuery *query_povijest;
	TDataSource *data_povijest;
	TLabel *label_preporuke_naslov;
	TLabel *label_preporuke_opis;
	TButton *button_generiraj_preporuku;
	TPanel *panel_opca_preporuka;
	TLabel *label_zakljucak_naslov;
	TMemo *memo_zakljucak;
	TLabel *label_aktivnosti_naslov;
	TMemo *memo_preporucene_aktivnosti;
	TLabel *label_razlog_naslov;
	TMemo *memo_razlog_preporuke;
	TButton *button_detalji_preporuke;
	TFDQuery *query_pregledi_preporuke;
	TFDQuery *query_ocjene_preporuke;
	TFDQuery *query_aktivnosti_preporuke;
	TLabel *label_status_preporuke_naslov;
	TComboBox *combo_status_preporuke;
	TButton *button_spremi_preporuku;
	TFDQuery *query_spremi_preporuku;
	TFDQuery *query_spremi_preporuka_aktivnost;
	TFDQuery *query_provjeri_preporuku;
	TLabel *label_otvori_aktivnost;
	TComboBox *combo_preporucene_aktivnosti;
	TButton *button_otvori_aktivnost;
	TFDQuery *query_ucitaj_preporuku;
	TFDQuery *query_ucitaj_preporuka_aktivnosti;
	TPanel *panel_user;
	TLabel *label_korisnik;
	TLabel *label_uloga;
	TImage *imgKorisnik;
	void __fastcall button_pocetnaClick(TObject *Sender);
	void __fastcall button_djecaClick(TObject *Sender);
	void __fastcall button_aktivnostiClick(TObject *Sender);
	void __fastcall button_skupineClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall button_odjavaClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall combo_dijeteChange(TObject *Sender);
	void __fastcall speed_tjelesniClick(TObject *Sender);
	void __fastcall speed_socioClick(TObject *Sender);
	void __fastcall speed_govorniClick(TObject *Sender);
	void __fastcall speed_spoznajniClick(TObject *Sender);
	void __fastcall button_ocistiClick(TObject *Sender);
	void __fastcall button_spremiClick(TObject *Sender);
	void __fastcall grid_povijestDblClick(TObject *Sender);
	void __fastcall button_generiraj_preporukuClick(TObject *Sender);
	void __fastcall button_spremi_preporukuClick(TObject *Sender);
	void __fastcall button_otvori_aktivnostClick(TObject *Sender);
	void __fastcall button_korisniciClick(TObject *Sender);
	void __fastcall button_uputeClick(TObject *Sender);
private:	// User declarations
    void ucitajDjecu();
	void ocistiPodatkeDjeteta();
	int odabranoDijeteID;
	void ucitajPodatkeDjeteta();
	int ocjenaTjelesni;
	void osvjeziTjelesneZvjezdice();
	int ocjenaSocio;
	void osvjeziSocioZvjezdice();
	int ocjenaGovorni;
	void osvjeziGovorneZvjezdice();
	int ocjenaSpoznajni;
	void osvjeziSpoznajneZvjezdice();
	TColor bojaZaOcjenu(int ocjena);
	void ucitajPovijest();
	void postaviIzgledPovijesti();
    bool ucitajPregledeZaPreporuke(
    int &zadnjiPregledID,
    int &prethodniPregledID,
    TDateTime &datumZadnjeg,
    bool &postojiPrethodni
	);
    String odrediTrend(
    int trenutnaRazina,
    int prethodnaRazina,
    bool postojiPrethodna
	);

	void ucitajAnalizePodrucja(
		int zadnjiPregledID,
		int prethodniPregledID,
		bool postojiPrethodni
	);
    int odrediCiljanuTezinu(
    int trenutnaRazina,
    const String &trend
	);

	int izracunajPrioritet(
		int trenutnaRazina,
		const String &trend
	);
	void izracunajTezineIPrioritete();
    bool aktivnostJeVecOdabrana(int idAktivnost);
	void odaberiPreporuceneAktivnosti();
	struct TAnalizaPodrucja
	{
		int idPodrucje;
		String nazivPodrucja;

		int trenutnaRazina;
		int prethodnaRazina;

		bool postojiPrethodna;
		String trend;

		int ciljanaTezina;
		int prioritet;
	};
    struct TPreporucenaAktivnost
	{
		int idAktivnost;
		int idPodrucje;

		String nazivAktivnosti;
		String nazivPodrucja;

		int tezinaAktivnosti;
	};
	std::vector<TAnalizaPodrucja> analizePodrucja;
	std::vector<TPreporucenaAktivnost> preporuceneAktivnosti;
	String generirajOpciZakljucak();
	String generirajPopisAktivnosti();
	String generirajOpciRazlog();
    int generiraniPregledID;
	bool preporukaGenerirana;
	int spremiGlavnuPreporuku();
	void spremiAktivnostiPreporuke(int preporukaID);
	void popuniComboAktivnosti();
	void primijeniPrava();
	void ocistiPrikazPreporuke();
	void ucitajSpremljenuPreporuku();
public:		// User declarations
	__fastcall Tform_razvoj(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE Tform_razvoj *form_razvoj;
//---------------------------------------------------------------------------
#endif
