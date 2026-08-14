//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "IzvjestajForm.h"
#include "DatabaseModule.h"
#include <Vcl.Printers.hpp>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
Tform_izvjestaj *form_izvjestaj;
//---------------------------------------------------------------------------
__fastcall Tform_izvjestaj::Tform_izvjestaj(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall Tform_izvjestaj::FormShow(TObject *Sender)
{
	generirajIzvjestaj();
}
//---------------------------------------------------------------------------
String Tform_izvjestaj::odgojiteljSkupine(int idSkupina)
{
	query_izv_odgojitelj->Close();
	query_izv_odgojitelj->SQL->Clear();

	query_izv_odgojitelj->SQL->Add(
		L"SELECT k.ime, k.prezime "
		L"FROM korisnik k "
		L"INNER JOIN uloga u ON u.id_uloga = k.id_uloga "
		L"WHERE k.id_skupina = :id_skupina AND u.naziv = 'Odgojitelj' "
		L"LIMIT 1"
	);

	query_izv_odgojitelj->ParamByName(L"id_skupina")->AsInteger = idSkupina;
	query_izv_odgojitelj->Open();

	String rezultat = L"nije dodijeljen";

	if (!query_izv_odgojitelj->IsEmpty())
	{
		rezultat =
			query_izv_odgojitelj->FieldByName(L"ime")->AsString + L" " +
			query_izv_odgojitelj->FieldByName(L"prezime")->AsString;
	}

	query_izv_odgojitelj->Close();
	return rezultat;
}
//---------------------------------------------------------------------------
// Vraća true ako dijete ima barem jedan razvojni pregled. "trenutna" je
// prosjek razina (sva 4 područja) zadnjeg pregleda, "prethodna" prosjek
// pregleda prije njega (ako postoji) - za usporedbu napretka.
bool Tform_izvjestaj::prosjecnaRazinaDjeteta(
	int idDijete,
	double &trenutna,
	double &prethodna,
	bool &postojiPrethodna)
{
	trenutna = 0;
	prethodna = 0;
	postojiPrethodna = false;

	query_izv_procjene->Close();
	query_izv_procjene->SQL->Clear();

	query_izv_procjene->SQL->Add(
		L"SELECT "
		L"    rp.id_razvojni_pregled, "
		L"    rp.datum, "
		L"    (SELECT AVG(pp.razina) "
		L"     FROM procjena_podrucja pp "
		L"     WHERE pp.id_razvojni_pregled = rp.id_razvojni_pregled) AS prosjek "
		L"FROM razvojni_pregled rp "
		L"WHERE rp.id_dijete = :id_dijete "
		L"ORDER BY rp.datum DESC, rp.id_razvojni_pregled DESC "
		L"LIMIT 2"
	);

	query_izv_procjene->ParamByName(L"id_dijete")->AsInteger = idDijete;
	query_izv_procjene->Open();

	if (query_izv_procjene->IsEmpty())
	{
		query_izv_procjene->Close();
		return false;
	}

	trenutna = query_izv_procjene->FieldByName(L"prosjek")->AsFloat;
	query_izv_procjene->Next();

	if (!query_izv_procjene->Eof)
	{
		prethodna = query_izv_procjene->FieldByName(L"prosjek")->AsFloat;
		postojiPrethodna = true;
	}

	query_izv_procjene->Close();
	return true;
}
//---------------------------------------------------------------------------
String Tform_izvjestaj::odrediTrendIzvjestaj(
	double trenutna,
	double prethodna,
	bool postojiPrethodna)
{
	if (!postojiPrethodna)
	{
		return L"(prvi pregled)";
	}

	if (trenutna > prethodna + 0.05)
	{
		return L"↑ napreduje";
	}

	if (trenutna < prethodna - 0.05)
	{
		return L"↓ nazaduje";
	}

	return L"= stabilno";
}
//---------------------------------------------------------------------------
void Tform_izvjestaj::generirajIzvjestaj()
{
	memo_izvjestaj->Lines->Clear();
	podaciGrafova.clear();

	query_izv_skupine->Close();
	query_izv_skupine->Open();

	int minBrojDjece = -1;
	String nazivNajmanje = L"";

	while (!query_izv_skupine->Eof)
	{
		int idSkupina = query_izv_skupine->FieldByName(L"id_skupina")->AsInteger;
		String nazivSkupine = query_izv_skupine->FieldByName(L"naziv")->AsString;

		TStringList *redoviDjece = new TStringList();

		try
		{
			double zbrojDobi = 0;
			double zbrojRazina = 0;
			int brojacRazina = 0;

			query_izv_djeca->Close();
			query_izv_djeca->SQL->Clear();

			query_izv_djeca->SQL->Add(
				L"SELECT "
				L"    id_dijete, "
				L"    ime, "
				L"    prezime, "
				L"    datum_rodjenja, "
				L"    spol, "
				L"    id_skupina "
				L"FROM dijete "
				L"WHERE id_skupina = :id_skupina "
				L"ORDER BY prezime ASC"
			);

			query_izv_djeca->ParamByName(L"id_skupina")->AsInteger = idSkupina;
			query_izv_djeca->Open();

			while (!query_izv_djeca->Eof)
			{
				int idDijete = query_izv_djeca->FieldByName(L"id_dijete")->AsInteger;
				String imePrezime =
					query_izv_djeca->FieldByName(L"prezime")->AsString + L" " +
					query_izv_djeca->FieldByName(L"ime")->AsString;
				TDate datumRodjenja =
					query_izv_djeca->FieldByName(L"datum_rodjenja")->AsDateTime;

				int dob = razvojPomoc.IzracunajDob(datumRodjenja);
				zbrojDobi += dob;

				double trenutna = 0;
				double prethodna = 0;
				bool postojiPrethodna = false;
				bool imaPregled = prosjecnaRazinaDjeteta(
					idDijete, trenutna, prethodna, postojiPrethodna);

				String redak = L"  - " + imePrezime + L", " + IntToStr(dob) + L" god.";

				if (imaPregled)
				{
					zbrojRazina += trenutna;
					brojacRazina++;

					redak = redak +
						L"   razina " + FormatFloat(L"0.0", trenutna) +
						L"  " + odrediTrendIzvjestaj(trenutna, prethodna, postojiPrethodna);
				}
				else
				{
					redak = redak + L"   nema razvojnih pregleda";
				}

				redoviDjece->Add(redak);
				query_izv_djeca->Next();
			}

			int brojDjece = redoviDjece->Count;
			double prosjecnaDob = (brojDjece > 0) ? (zbrojDobi / brojDjece) : 0;
			double prosjecnaRazina = (brojacRazina > 0) ? (zbrojRazina / brojacRazina) : 0;

			memo_izvjestaj->Lines->Add(
				L"Skupina: " + nazivSkupine +
				L" (odgojitelj: " + odgojiteljSkupine(idSkupina) + L")");

			String redakStatistike =
				L"  Djece: " + IntToStr(brojDjece) +
				L"   Prosj. dob: " + FormatFloat(L"0.0", prosjecnaDob) + L" god.";

			if (brojacRazina > 0)
			{
				redakStatistike = redakStatistike +
					L"   Prosj. razina razvoja: " + FormatFloat(L"0.0", prosjecnaRazina);
			}

			memo_izvjestaj->Lines->Add(redakStatistike);
			memo_izvjestaj->Lines->AddStrings(redoviDjece);
			memo_izvjestaj->Lines->Add(L"");

			TPodaciSkupineGraf podaci;
			podaci.naziv = nazivSkupine;
			podaci.brojDjece = brojDjece;
			podaci.prosjecnaRazina = prosjecnaRazina;
			podaci.imaRazinu = (brojacRazina > 0);
			podaciGrafova.push_back(podaci);

			if (minBrojDjece == -1 || brojDjece < minBrojDjece)
			{
				minBrojDjece = brojDjece;
				nazivNajmanje = nazivSkupine;
			}
		}
		__finally
		{
			delete redoviDjece;
		}

		query_izv_skupine->Next();
	}

	query_izv_djeca->Close();
	query_izv_skupine->Close();

	if (!nazivNajmanje.IsEmpty())
	{
		memo_izvjestaj->Lines->Add(
			L"Napomena: skupina \"" + nazivNajmanje + L"\" ima najmanje djece (" +
			IntToStr(minBrojDjece) + L") u odnosu na ostale skupine.");
	}

	paint_broj_djece->Repaint();
	paint_razina_razvoja->Repaint();
}
//---------------------------------------------------------------------------
// Ručno crtanje jednostavnog stupčastog grafa unutar zadanog pravokutnika -
// visina svakog stupca je proporcionalna vrijednosti u odnosu na najveću u
// nizu. Radi i na ekranu (TPaintBox->Canvas) i na printeru (Printer()->Canvas)
// jer prima gotov Canvas i područje, ne konkretnu komponentu.
void Tform_izvjestaj::crtajStupcastiGraf(TCanvas *canvas, TRect podrucje, bool crtajBrojDjece)
{
	int sirina = podrucje.Right - podrucje.Left;
	int visina = podrucje.Bottom - podrucje.Top;

	canvas->Brush->Color = clWhite;
	canvas->FillRect(podrucje);

	if (podaciGrafova.empty())
	{
		return;
	}

	double maxVrijednost = 0;

	for (size_t i = 0; i < podaciGrafova.size(); i++)
	{
		double vrijednost = crtajBrojDjece
			? podaciGrafova[i].brojDjece
			: podaciGrafova[i].prosjecnaRazina;

		if (vrijednost > maxVrijednost)
		{
			maxVrijednost = vrijednost;
		}
	}

	if (maxVrijednost <= 0)
	{
		maxVrijednost = 1;
	}

	int brojStupaca = podaciGrafova.size();
	int donjaLinija = podrucje.Top + visina - 34;
	int visinaPodrucja = donjaLinija - podrucje.Top - 20;
	int sirinaStupca = (sirina - 20) / (brojStupaca * 2);

	canvas->Font->Size = 8;
	canvas->Brush->Style = bsClear;

	for (int i = 0; i < brojStupaca; i++)
	{
		bool imaPodatak = crtajBrojDjece || podaciGrafova[i].imaRazinu;

		double vrijednost = crtajBrojDjece
			? podaciGrafova[i].brojDjece
			: podaciGrafova[i].prosjecnaRazina;

		int mjestoStupca = (sirina - 20) / brojStupaca;
		int x = podrucje.Left + 10 + i * mjestoStupca + (mjestoStupca - sirinaStupca) / 2;

		if (imaPodatak)
		{
			int visinaStupca = (int)((vrijednost / maxVrijednost) * visinaPodrucja);
			int yGore = donjaLinija - visinaStupca;

			canvas->Brush->Style = bsSolid;
			canvas->Brush->Color = crtajBrojDjece
				? (TColor)0x5A9C55
				: (TColor)0xDD8A37;
			canvas->Rectangle(x, yGore, x + sirinaStupca, donjaLinija);
			canvas->Brush->Style = bsClear;

			String vrijednostTekst = crtajBrojDjece
				? IntToStr((int)vrijednost)
				: FormatFloat(L"0.0", vrijednost);

			int vrijSirina = canvas->TextWidth(vrijednostTekst);
			canvas->TextOut(
				x + (sirinaStupca - vrijSirina) / 2, yGore - 16, vrijednostTekst);
		}

		String oznaka = podaciGrafova[i].naziv;
		int tekstSirina = canvas->TextWidth(oznaka);
		canvas->TextOut(
			x + (sirinaStupca - tekstSirina) / 2, donjaLinija + 4, oznaka);
	}
}
//---------------------------------------------------------------------------
void __fastcall Tform_izvjestaj::paint_broj_djecePaint(TObject *Sender)
{
	crtajStupcastiGraf(paint_broj_djece->Canvas, paint_broj_djece->ClientRect, true);
}
//---------------------------------------------------------------------------
void __fastcall Tform_izvjestaj::paint_razina_razvojaPaint(TObject *Sender)
{
	crtajStupcastiGraf(paint_razina_razvoja->Canvas, paint_razina_razvoja->ClientRect, false);
}
//---------------------------------------------------------------------------
void __fastcall Tform_izvjestaj::button_pdfClick(TObject *Sender)
{
	// Umjesto da pitamo korisnika koji printer želi (TPrintDialog), sami
	// pronađemo Windowsov ugrađeni "Microsoft Print to PDF" i postavimo ga
	// kao aktivni - on sam otvori standardni "Spremi kao" dijalog.
	int indeksPdfPrintera = Printer()->Printers->IndexOf(L"Microsoft Print to PDF");

	if (indeksPdfPrintera < 0)
	{
		ShowMessage(
			L"Nije pronađen 'Microsoft Print to PDF' printer na ovom računalu. "
			L"Provjeri Windows postavke printera.");
		return;
	}

	Printer()->PrinterIndex = indeksPdfPrintera;

	// Margina i visine se računaju proporcionalno prema stvarnoj veličini
	// stranice na printeru (a ne fiksnim pikselima) - "Microsoft Print to
	// PDF" zna raditi u puno većoj rezoluciji (DPI) nego ekran, pa fiksni
	// broj piksela zna ispasti prevelik ili premalen.
	int margin = Printer()->PageWidth / 20;

	Printer()->BeginDoc();

	try
	{
		Printer()->Canvas->Font->Name = L"Segoe UI";
		Printer()->Canvas->Font->Size = 14;

		int visinaNaslova = Printer()->Canvas->TextHeight(L"Ay");
		int grafTop = margin + visinaNaslova + margin / 2;
		int sirinaGrafa = (Printer()->PageWidth - margin * 3) / 2;
		int visinaGrafa = Printer()->PageHeight / 3;

		TRect podrucjeDjece(margin, grafTop, margin + sirinaGrafa, grafTop + visinaGrafa);
		TRect podrucjeRazine(
			margin * 2 + sirinaGrafa, grafTop,
			margin * 2 + sirinaGrafa * 2, grafTop + visinaGrafa);

		Printer()->Canvas->TextOut(margin, margin, L"Broj djece po skupini");
		Printer()->Canvas->TextOut(
			margin * 2 + sirinaGrafa, margin, L"Prosječna razina razvoja");

		crtajStupcastiGraf(Printer()->Canvas, podrucjeDjece, true);
		crtajStupcastiGraf(Printer()->Canvas, podrucjeRazine, false);

		Printer()->NewPage();

		Printer()->Canvas->Font->Name = L"Consolas";
		Printer()->Canvas->Font->Size = 10;

		int lineHeight = Printer()->Canvas->TextHeight(L"Ay") + 6;
		int y = margin;

		for (int i = 0; i < memo_izvjestaj->Lines->Count; i++)
		{
			if (y + lineHeight > Printer()->PageHeight - margin)
			{
				Printer()->NewPage();
				y = margin;
			}

			Printer()->Canvas->TextOut(margin, y, memo_izvjestaj->Lines->Strings[i]);
			y += lineHeight;
		}
	}
	__finally
	{
		Printer()->EndDoc();
	}

	ShowMessage(L"Izvještaj je spremljen u PDF.");
}
//---------------------------------------------------------------------------
void __fastcall Tform_izvjestaj::button_zatvoriClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
