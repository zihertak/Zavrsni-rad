//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "RazvojDetaljiForm.h"
#include "RazvojForm.h"
#include "DatabaseModule.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
Tform_razvoj_detalji *form_razvoj_detalji;
//---------------------------------------------------------------------------
__fastcall Tform_razvoj_detalji::Tform_razvoj_detalji(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
String Tform_razvoj_detalji::napraviZvjezdice(int ocjena)
{
    String rezultat = "";

    for (int i = 1; i <= 5; i++)
    {
        rezultat +=
            i <= ocjena
                ? L"★"
                : L"☆";
    }

    return rezultat;
}

void Tform_razvoj_detalji::ucitajPregled(int idRazvojniPregled)
{
    query_detalji->Close();
    query_detalji->SQL->Clear();

    query_detalji->SQL->Add(
        "SELECT "
        "rp.datum, "
        "CAST(rp.napomena AS CHAR(255)) AS napomena, "
        "CONCAT(d.ime, ' ', d.prezime) AS dijete, "
        "CONCAT(k.ime, ' ', k.prezime) AS odgojitelj, "
        "pp.id_podrucje_razvoja, "
        "pp.razina "
        "FROM razvojni_pregled rp "
        "INNER JOIN dijete d "
        "ON d.id_dijete = rp.id_dijete "
        "INNER JOIN korisnik k "
        "ON k.id_korisnik = rp.id_korisnik "
        "INNER JOIN procjena_podrucja pp "
        "ON pp.id_razvojni_pregled = rp.id_razvojni_pregled "
        "WHERE rp.id_razvojni_pregled = :id_pregled "
        "ORDER BY pp.id_podrucje_razvoja"
    );

    query_detalji
        ->ParamByName("id_pregled")
        ->AsInteger =
            idRazvojniPregled;

    query_detalji->Open();

    if (query_detalji->IsEmpty())
    {
        ShowMessage(
            L"Podaci o odabranom pregledu nisu pronađeni."
        );

        return;
    }

    label_dijete->Caption =
        query_detalji
            ->FieldByName("dijete")
            ->AsString;

    label_datum->Caption =
        FormatDateTime(
            L"dd.mm.yyyy.",
            query_detalji
                ->FieldByName("datum")
                ->AsDateTime
        );

    label_odgojitelj->Caption =
        query_detalji
            ->FieldByName("odgojitelj")
            ->AsString;

    memo_napomena->Text =
        query_detalji
            ->FieldByName("napomena")
            ->AsString;

    label_tjelesni_zvjezdice->Caption = L"☆☆☆☆☆";
    label_socio_zvjezdice->Caption = L"☆☆☆☆☆";
    label_govorni_zvjezdice->Caption = L"☆☆☆☆☆";
	label_spoznajni_zvjezdice->Caption = L"☆☆☆☆☆";
    label_tjelesni_zvjezdice->Font->Color = clGray;
	label_socio_zvjezdice->Font->Color = clGray;
	label_govorni_zvjezdice->Font->Color = clGray;
	label_spoznajni_zvjezdice->Font->Color = clGray;

    while (!query_detalji->Eof)
    {
        int idPodrucje =
            query_detalji
                ->FieldByName("id_podrucje_razvoja")
                ->AsInteger;

        int razina =
            query_detalji
                ->FieldByName("razina")
                ->AsInteger;

        switch (idPodrucje)
        {
			case 1:
				postaviZvjezdice(
					label_tjelesni_zvjezdice,
					razina
				);
				break;

			case 2:
				postaviZvjezdice(
					label_socio_zvjezdice,
					razina
				);
				break;

			case 3:
				postaviZvjezdice(
					label_govorni_zvjezdice,
					razina
				);
				break;

			case 4:
	postaviZvjezdice(
		label_spoznajni_zvjezdice,
        razina
    );
	break;
        }

        query_detalji->Next();
    }
}

TColor Tform_razvoj_detalji::bojaZaOcjenu(int ocjena)
{
    switch (ocjena)
    {
        case 1:
            return clRed;

        case 2:
            return static_cast<TColor>(
                RGB(255, 128, 0)
            );

        case 3:
            return static_cast<TColor>(
                RGB(255, 215, 0)
            );

        case 4:
            return static_cast<TColor>(
                RGB(50, 205, 50)
            );

        case 5:
            return static_cast<TColor>(
                RGB(0, 128, 0)
            );

        default:
            return clGray;
    }
}

void Tform_razvoj_detalji::postaviZvjezdice(
    TLabel *labela,
    int ocjena
)
{
    labela->Caption =
        napraviZvjezdice(ocjena);

    labela->Font->Color =
        bojaZaOcjenu(ocjena);
}