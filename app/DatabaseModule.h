//---------------------------------------------------------------------------

#ifndef DatabaseModuleH
#define DatabaseModuleH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Data.DB.hpp>
#include <FireDAC.Comp.Client.hpp>
#include <FireDAC.Phys.hpp>
#include <FireDAC.Phys.Intf.hpp>
#include <FireDAC.Phys.MySQL.hpp>
#include <FireDAC.Phys.MySQLDef.hpp>
#include <FireDAC.Stan.Async.hpp>
#include <FireDAC.Stan.Def.hpp>
#include <FireDAC.Stan.Error.hpp>
#include <FireDAC.Stan.Intf.hpp>
#include <FireDAC.Stan.Option.hpp>
#include <FireDAC.Stan.Pool.hpp>
#include <FireDAC.UI.Intf.hpp>
#include <FireDAC.VCLUI.Wait.hpp>
#include <FireDAC.Comp.DataSet.hpp>
#include <FireDAC.DApt.hpp>
#include <FireDAC.DApt.Intf.hpp>
#include <FireDAC.DatS.hpp>
#include <FireDAC.Stan.Param.hpp>
#include "Korisnik.h"
//---------------------------------------------------------------------------
class Tdata_module : public TDataModule
{
__published:	// IDE-managed Components
	TFDConnection *connection;
	TFDPhysMySQLDriverLink *mysql_driver;
	TFDQuery *query_login;
private:	// User declarations
public:		// User declarations
	__fastcall Tdata_module(TComponent* Owner);
	int currentUserID;
	String currentUserIme;
	String currentUserPrezime;
	String currentUserUloga;
    int currentUserSkupinaID;
    TKorisnik trenutniKorisnik;
};
//---------------------------------------------------------------------------
extern PACKAGE Tdata_module *data_module;
//---------------------------------------------------------------------------
#endif
