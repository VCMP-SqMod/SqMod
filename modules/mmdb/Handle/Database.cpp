// ------------------------------------------------------------------------------------------------
#include "Handle/Database.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
DbHnd::DbHnd(CSStr filepath, Uint32 flags)
    : mDb()
{
    // Validate the specified file path
    if (!filepath || *filepath == '\0')
    {
        STHROWF("Invalid database file path");
    }
    // Let's attempt to open the specified database
    const Int32 status = MMDB_open(filepath, flags, &mDb);
    // Validate the result of the operation
    if (status != MMDB_SUCCESS)
    {
        STHROWF("Unable to open the specified database [%s]", MMDB_strerror(status));
    }
}

// ------------------------------------------------------------------------------------------------
DbHnd::~DbHnd()
{
    // We don't need the database handle anymore
    MMDB_close(&mDb);
}

} // Namespace:: SqMod