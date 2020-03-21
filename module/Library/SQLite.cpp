// ------------------------------------------------------------------------------------------------
#include "Library/SQLite.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
extern void Register_Constants(Table & sqlns);
extern void Register_Common(Table & sqlns);
extern void Register_Connection(Table & sqlns);
extern void Register_Statement(Table & sqlns);
extern void Register_Parameter(Table & sqlns);
extern void Register_Column(Table & sqlns);
extern void Register_Transaction(Table & sqlns);

// ================================================================================================
void Register_SQLite(HSQUIRRELVM vm)
{
    Table sqlns(vm);

    Register_Constants(sqlns);
    Register_Common(sqlns);
    Register_Connection(sqlns);
    Register_Statement(sqlns);
    Register_Parameter(sqlns);
    Register_Column(sqlns);
    Register_Transaction(sqlns);

    RootTable(vm).Bind(_SC("SQLite"), sqlns);
}

} // Namespace:: SqMod
