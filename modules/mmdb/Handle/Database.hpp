#ifndef _SQMMDB_HANDLE_DATABASE_HPP_
#define _SQMMDB_HANDLE_DATABASE_HPP_

// ------------------------------------------------------------------------------------------------
#include "Common.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Manages a reference counted INI document instance.
*/
class DbHnd
{
    // --------------------------------------------------------------------------------------------
    friend class Database;

public:

    // --------------------------------------------------------------------------------------------
    typedef MMDB_s          Type; // The managed type.

    // --------------------------------------------------------------------------------------------
    typedef Type*           Pointer; // Pointer to the managed type.
    typedef const Type*     ConstPtr; // Constant pointer to the managed type.

    // --------------------------------------------------------------------------------------------
    typedef Type&           Reference; // Reference to the managed type.
    typedef const Type&     ConstRef; // Constant reference to the managed type.

public:

    // --------------------------------------------------------------------------------------------
    MMDB_s     mDb; // The managed database handle.

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    DbHnd(CSStr filepath, Uint32 flags);

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    DbHnd(const DbHnd & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move constructor. (disabled)
    */
    DbHnd(DbHnd && o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~DbHnd();

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator. (disabled)
    */
    DbHnd & operator = (const DbHnd & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator. (disabled)
    */
    DbHnd & operator = (DbHnd && o) = delete;
};

} // Namespace:: SqMod

#endif // _SQMMDB_HANDLE_DATABASE_HPP_
