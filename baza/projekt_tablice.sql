-- MySQL dump 10.13  Distrib 8.0.42, for Win64 (x86_64)
--
-- Host: localhost    Database: mydb
-- ------------------------------------------------------
-- Server version	8.0.42

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!50503 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `aktivnost`
--

DROP TABLE IF EXISTS `aktivnost`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `aktivnost` (
  `id_aktivnost` int NOT NULL AUTO_INCREMENT,
  `naziv` varchar(45) NOT NULL,
  `opis` text,
  `trajanje_minuta` int DEFAULT NULL,
  `dob_od` int DEFAULT NULL,
  `dob_do` int DEFAULT NULL,
  `potrebne_stavke` text,
  `upute` text,
  PRIMARY KEY (`id_aktivnost`)
) ENGINE=InnoDB AUTO_INCREMENT=9 DEFAULT CHARSET=utf8mb3;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `aktivnost_podrucje`
--

DROP TABLE IF EXISTS `aktivnost_podrucje`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `aktivnost_podrucje` (
  `id_aktivnost` int NOT NULL,
  `id_podrucje_razvoja` int NOT NULL,
  PRIMARY KEY (`id_aktivnost`,`id_podrucje_razvoja`),
  KEY `idx_aktivnost_podrucje_podrucje_razvoja` (`id_podrucje_razvoja`),
  CONSTRAINT `fk_aktivnost_podrucje_aktivnost` FOREIGN KEY (`id_aktivnost`) REFERENCES `aktivnost` (`id_aktivnost`),
  CONSTRAINT `fk_aktivnost_podrucje_podrucje_razvoja1` FOREIGN KEY (`id_podrucje_razvoja`) REFERENCES `podrucje_razvoja` (`id_podrucje_razvoja`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `dijete`
--

DROP TABLE IF EXISTS `dijete`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `dijete` (
  `id_dijete` int NOT NULL AUTO_INCREMENT,
  `ime` varchar(45) NOT NULL,
  `prezime` varchar(45) NOT NULL,
  `datum_rodjenja` date NOT NULL,
  `spol` char(1) NOT NULL,
  `datum_upisa` date NOT NULL,
  `kontakt_roditelj` varchar(100) DEFAULT NULL,
  `id_skupina` int NOT NULL,
  PRIMARY KEY (`id_dijete`),
  KEY `idx_dijete_skupina` (`id_skupina`),
  CONSTRAINT `fk_dijete_skupina` FOREIGN KEY (`id_skupina`) REFERENCES `skupina` (`id_skupina`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `korisnik`
--

DROP TABLE IF EXISTS `korisnik`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `korisnik` (
  `id_korisnik` int NOT NULL AUTO_INCREMENT,
  `ime` varchar(45) NOT NULL,
  `prezime` varchar(45) NOT NULL,
  `username` varchar(45) NOT NULL,
  `lozinka` varchar(255) NOT NULL,
  `id_uloga` int NOT NULL,
  `id_skupina` int DEFAULT NULL,
  PRIMARY KEY (`id_korisnik`),
  UNIQUE KEY `username_UNIQUE` (`username`),
  KEY `fk_korisnik_uloga_idx` (`id_uloga`),
  KEY `fk_korisnik_skupina_idx` (`id_skupina`),
  CONSTRAINT `fk_korisnik_skupina` FOREIGN KEY (`id_skupina`) REFERENCES `skupina` (`id_skupina`),
  CONSTRAINT `fk_korisnik_uloga` FOREIGN KEY (`id_uloga`) REFERENCES `uloga` (`id_uloga`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8mb3;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `podrucje_razvoja`
--

DROP TABLE IF EXISTS `podrucje_razvoja`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `podrucje_razvoja` (
  `id_podrucje_razvoja` int NOT NULL AUTO_INCREMENT,
  `naziv` varchar(45) NOT NULL,
  `opis` text,
  PRIMARY KEY (`id_podrucje_razvoja`),
  UNIQUE KEY `uq_podrucje_razvoja_naziv` (`naziv`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8mb3;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `preporuka`
--

DROP TABLE IF EXISTS `preporuka`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `preporuka` (
  `id_preporuka` int NOT NULL AUTO_INCREMENT,
  `razlog` text,
  `status` varchar(45) NOT NULL,
  `datum` date NOT NULL,
  `napomena` text,
  `id_razvojni_pregled` int NOT NULL,
  `id_aktivnost` int NOT NULL,
  PRIMARY KEY (`id_preporuka`),
  KEY `idx_preporuka_razvojni_pregled` (`id_razvojni_pregled`),
  KEY `idx_preporuka_aktivnost` (`id_aktivnost`),
  CONSTRAINT `fk_preporuka_aktivnost` FOREIGN KEY (`id_aktivnost`) REFERENCES `aktivnost` (`id_aktivnost`),
  CONSTRAINT `fk_preporuka_razvojni_pregled` FOREIGN KEY (`id_razvojni_pregled`) REFERENCES `razvojni_pregled` (`id_razvojni_pregled`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `procjena_podrucja`
--

DROP TABLE IF EXISTS `procjena_podrucja`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `procjena_podrucja` (
  `id_procjena_podrucja` int NOT NULL AUTO_INCREMENT,
  `razina` tinyint DEFAULT NULL,
  `napomena` text,
  `id_podrucje_razvoja` int NOT NULL,
  `id_razvojni_pregled` int NOT NULL,
  PRIMARY KEY (`id_procjena_podrucja`),
  UNIQUE KEY `unique_pregled_podrucje` (`id_podrucje_razvoja`,`id_razvojni_pregled`),
  KEY `idx_procjena_podrucje` (`id_podrucje_razvoja`),
  KEY `idx_procjena_pregled` (`id_razvojni_pregled`),
  CONSTRAINT `fk_procjena_podrucja_podrucje` FOREIGN KEY (`id_podrucje_razvoja`) REFERENCES `podrucje_razvoja` (`id_podrucje_razvoja`),
  CONSTRAINT `fk_procjena_podrucja_pregled` FOREIGN KEY (`id_razvojni_pregled`) REFERENCES `razvojni_pregled` (`id_razvojni_pregled`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `razvojni_pregled`
--

DROP TABLE IF EXISTS `razvojni_pregled`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `razvojni_pregled` (
  `id_razvojni_pregled` int NOT NULL AUTO_INCREMENT,
  `datum` date NOT NULL,
  `napomena` text,
  `id_korisnik` int NOT NULL,
  `id_dijete` int NOT NULL,
  PRIMARY KEY (`id_razvojni_pregled`),
  UNIQUE KEY `uq_razvojni_pregled_dijete_datum` (`id_dijete`,`datum`),
  KEY `idx_razvojni_pregled_korisnik` (`id_korisnik`),
  KEY `idx_razvojni_pregled_dijete` (`id_dijete`),
  CONSTRAINT `fk_razvojni_pregled_dijete` FOREIGN KEY (`id_dijete`) REFERENCES `dijete` (`id_dijete`),
  CONSTRAINT `fk_razvojni_pregled_korisnik` FOREIGN KEY (`id_korisnik`) REFERENCES `korisnik` (`id_korisnik`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `skupina`
--

DROP TABLE IF EXISTS `skupina`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `skupina` (
  `id_skupina` int NOT NULL AUTO_INCREMENT,
  `naziv` varchar(45) NOT NULL,
  `pedagoska_godina` varchar(45) NOT NULL,
  `dob_od` int NOT NULL,
  `dob_do` int NOT NULL,
  PRIMARY KEY (`id_skupina`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8mb3;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `uloga`
--

DROP TABLE IF EXISTS `uloga`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `uloga` (
  `id_uloga` int NOT NULL AUTO_INCREMENT,
  `naziv` varchar(45) NOT NULL,
  `opis` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`id_uloga`),
  UNIQUE KEY `naziv_UNIQUE` (`naziv`)
) ENGINE=InnoDB AUTO_INCREMENT=9 DEFAULT CHARSET=utf8mb3;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `zdravstveni_podaci`
--

DROP TABLE IF EXISTS `zdravstveni_podaci`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `zdravstveni_podaci` (
  `id_zdravstveni_podaci` int NOT NULL AUTO_INCREMENT,
  `alergije` text,
  `posebne_potrebe` text,
  `zdravstvene_napomene` text,
  `id_dijete` int NOT NULL,
  PRIMARY KEY (`id_zdravstveni_podaci`),
  UNIQUE KEY `uq_zdravstveni_podaci_dijete` (`id_dijete`),
  CONSTRAINT `fk_zdravstveni_podaci_dijete` FOREIGN KEY (`id_dijete`) REFERENCES `dijete` (`id_dijete`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2026-07-18 14:39:53
