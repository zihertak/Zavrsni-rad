# Projekt: Aplikacija za praćenje razvoja djece u vrtiću

Kolegij: Napredne tehnike programiranja (NTP). C++ Builder (VCL), FireDAC + MySQL.
Rok: 2-3 tjedna do obrane. Student radi projekt sam, uz AI pomoć korak po korak (ne slijepi copy-paste) — cilj je da SVAKI dio koda može objasniti na obrani.

## Struktura projekta
- `app/` — sav C++ Builder kod (forme, DatabaseModule, glavna app)
- `baza/` — SQL dump (`projekt_tablice.sql`, `pocetni_podaci.sql`) — NAPOMENA: dump je zastario, stvarna baza ima dodatne stupce (`korisnik.aktivan`, `aktivnost.aktivna`, `aktivnost.razina_tezine`) koji nisu u dump fajlu. Treba re-exportati prije predaje.

## Forme u aplikaciji (13, sve povezane preko `data_module`)
Login → Dashboard → Djeca / Skupine / Aktivnosti / Razvoj / Korisnici / Upute
Svaka forma prima podatke o trenutnom korisniku (ime, prezime, uloga, id_skupina) preko `data_module` (Tdata_module, DatabaseModule.h/cpp) — global singleton s FireDAC konekcijom.

## Bodovanje — pravila kolegija (iz Uputa za izradu i obranu projekta)
- Max 131 bod ukupno, cilj: ~60 bodova
- Kriteriji po funkcionalnosti: 100% ako student razumije I teoriju I kod, 50% djelomično, 0% ne razumije, -4 boda ako je kopirano
- Funkcionalnost mora imati stvarnu svrhu u kontekstu aplikacije (vrtić), ne smije biti identična primjerima s predavanja

## Trenutno stanje (zadnja procjena: ~13 bodova)
| # | Funkcionalnost | Status | Bodovi |
|---|---|---|---|
| 2 | Više dijaloga + komunikacija | GOTOVO | 4/4 |
| 7 | Baza — CRUD 3+ tablica | GOTOVO | 6/6 |
| 8 | Sort/filter/calculated/lookup polje | DJELOMIČNO — sort ✅, filter ✅, calculated polje `Dob` ✅ (pravi OnCalcFields u DjecaForm), NEDOSTAJE pravo lookup polje (fkLookup TField) | 3/5 |
| 1 | Vlastite klase (min 2, po 2 atributa/metode) | NIJE GOTOVO — postoji samo `TKorisnickaPrava` (KorisnickaPrava.h/cpp) u projektu. Napravljena je i `TKorisnik` klasa u razgovoru s Claude ali NIJE dodana u projekt niti integrirana u LoginForm (login trenutno radi direktno preko SQL-a) | 0/3 |

## Plan do ~60 bodova (dogovoreni prioritet)
1. **Fn.1 — Klase**: dodati TKorisnik u projekt, koristiti ga u LoginForm umjesto direktnog SQL-a u data_module poljima → 3 boda
2. **Fn.4 — INI + Registry**: npr. zadnji filter/sortiranje u INI, zadnji username u registru → 5 bodova
3. **Fn.8 — dovršiti**: pravi lookup field (npr. skupina u gridu djece) → +1-2 boda
4. **Fn.25 — SHA-256 hash lozinki** (+fiksna sol): trenutno se lozinka sprema/uspoređuje kao čisti tekst u SQL WHERE — SIGURNOSNI PROPUST koji ovo popravlja → 5 bodova
5. **Fn.5 — XML + JSON CRUD**: JSON za predloške aktivnosti (odvojeno od baze podataka — različit dataset po pravilima kolegija) → 8 bodova
6. **Fn.20 — REST klijent** na javni servis: vremenska prognoza za planiranje aktivnosti vani → 3 boda
7. **Fn.27 — Statička biblioteka**: izdvojiti npr. računanje dobi, boje za ocjenu, validacije u .lib → 3-4 boda
8. **Fn.10 — Izvještaj + PDF + master-detail**: izvještaj skupine (skupina + djeca) → 5 bodova
9. Stretch ako ostane vremena: fn.6 (binarni format), fn.17 (HTTP download), fn.28 (DLL)
   - NAPOMENA: izbjegavati dretve za "3 paralelna dohvaćanja Dashboard podataka" — I/O-bound lokalni SQL upit, slabo opravdano, profesor će pitati zašto. Ako ide fn.11-13, bolji kandidat je paralelna obrada (npr. generiranje preporuka za više djece odjednom).

## Poznati bugovi za popraviti
- `AktivnostiForm.cpp`: `button_urediClick` i `button_obrisiClick` provjeravaju `prava.dodajAktivnost()` umjesto `urediAktivnost()`/`obrisiAktivnost()` — copy-paste greška

## Stil koda (bitno za "izgleda kao da je pisao student")
- Tabovi za indentaciju, `//---------------------------------------------------------------------------` razdjelnici između metoda (C++ Builder konvencija)
- `#pragma hdrstop` i `#pragma package(smart_init)` na vrhu .cpp, odmah nakon uključivanja headera — NE usred fajla
- Izbjegavati previše "AI-ish" savršen kod — komentari umjereni, ne enciklopedijski
- Svaka nova/izmijenjena funkcionalnost mora biti nešto što student stvarno razumije i može objasniti — to se provjerava na obrani

## Radni ritam
- Svaki dan ~1h prolazak kroz POSTOJEĆI kod (ne nova funkcionalnost) — objašnjavanje, pojednostavljivanje, "posvajanje" koda
- Kod svake veće funkcionalnosti: kratka dokumentacija za završni rad (opis, zašto, koje klase/biblioteke, što screenshotati) — usput, ne na kraju
