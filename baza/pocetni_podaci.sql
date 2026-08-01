-- Početni podaci za bazu mydb
-- Pokrenuti nakon kreiranja prazne strukture baze.
-- Skripta je namijenjena praznoj bazi.

USE mydb;

START TRANSACTION;

INSERT INTO uloga (id_uloga, naziv, opis) VALUES
(1, 'Administrator', 'Puni pristup sustavu'),
(2, 'Odgojitelj', 'Radi s djecom'),
(3, 'Ravnatelj', 'Upravlja vrtićem'),
(4, 'Stručni suradnik', 'Prati razvoj djece');

INSERT INTO podrucje_razvoja
(id_podrucje_razvoja, naziv, opis) VALUES
(1, 'Tjelesni i psihomotorni razvoj',
 'Razvoj motorike, koordinacije i tjelesnih sposobnosti.'),
(2, 'Socio-emocionalni razvoj',
 'Razvoj emocija, suradnje i odnosa s drugima.'),
(3, 'Govorni razvoj',
 'Razvoj govora, jezika i komunikacijskih vještina.'),
(4, 'Spoznajni razvoj',
 'Razvoj mišljenja, pamćenja, pažnje i zaključivanja.');

INSERT INTO skupina
(id_skupina, naziv, pedagoska_godina, dob_od, dob_do) VALUES
(1, 'Leptirići', '2026./2027.', 3, 4),
(2, 'Pčelice', '2026./2027.', 4, 5),
(3, 'Sovice', '2026./2027.', 5, 7);

INSERT INTO korisnik
(id_korisnik, ime, prezime, username, lozinka, id_uloga, id_skupina) VALUES
(1, 'Admin', 'Administrator', 'admin', 'admin123', 1, NULL),
(2, 'Ivan', 'Horvat', 'ivan1', 'ravnatelj123', 3, NULL),
(3, 'Ana', 'Anić', 'ana1', 'odgojitelj123', 2, 1),
(4, 'Marija', 'Marić', 'marija1', 'strucni123', 4, NULL);

INSERT INTO aktivnost
(id_aktivnost, naziv, opis, trajanje_minuta, dob_od, dob_do,
 potrebne_stavke, upute) VALUES
(1, 'Poligon spretnosti',
 'Razvoj koordinacije, ravnoteže i osnovnih motoričkih vještina.',
 30, 3, 7, 'Čunjevi, obruči, strunjače',
 'Djeca prolaze poligon prema uputama odgojitelja.'),

(2, 'Likovna radionica',
 'Razvoj fine motorike i kreativnog izražavanja.',
 30, 3, 7, 'Papir, bojice, tempere',
 'Djeca crtaju ili izrađuju radove prema zadanoj temi.'),

(3, 'Čitanje i razgovor o priči',
 'Razvoj govora, slušanja i razumijevanja pročitanog sadržaja.',
 20, 3, 7, 'Slikovnica',
 'Nakon čitanja odgojitelj razgovara s djecom o priči.'),

(4, 'Slagalice i puzzle',
 'Razvoj logičkog razmišljanja i koncentracije.',
 25, 3, 7, 'Puzzle i didaktičke slagalice',
 'Djeca samostalno ili u paru slažu zadane elemente.'),

(5, 'Glazbeno-ritmička igra',
 'Razvoj ritma, koordinacije pokreta i suradnje.',
 20, 3, 7, 'Glazba i ritmički instrumenti',
 'Djeca prate ritam glazbe pokretom ili instrumentima.'),

(6, 'Igra uloga',
 'Razvoj komunikacije, mašte i socijalnih vještina.',
 30, 4, 7, 'Kostimi i rekviziti',
 'Djeca preuzimaju različite uloge i međusobno komuniciraju.'),

(7, 'Razvrstavanje predmeta',
 'Razvoj opažanja, uspoređivanja i logičkog zaključivanja.',
 20, 3, 7, 'Kockice i predmeti različitih boja i oblika',
 'Djeca razvrstavaju predmete prema zadanom kriteriju.'),

(8, 'Timska igra',
 'Razvoj suradnje, poštivanja pravila i zajedničkog rješavanja zadataka.',
 30, 4, 7, 'Različiti rekviziti',
 'Djeca zajedno izvršavaju zadatak uz dogovor i suradnju.');

INSERT INTO aktivnost_podrucje
(id_aktivnost, id_podrucje_razvoja) VALUES
(1, 1),
(2, 1),
(5, 1),
(6, 2),
(8, 2),
(3, 3),
(4, 4),
(7, 4);

COMMIT;
