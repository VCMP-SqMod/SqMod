#include "Library/Hashing.hpp"
#include "Register.hpp"

// ------------------------------------------------------------------------------------------------
#include <sqstdstring.h>

// ------------------------------------------------------------------------------------------------
#include <crc32.h>
#include <keccak.h>
#include <md5.h>
#include <sha1.h>
#include <sha256.h>
#include <sha3.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * ...
*/
template < class T > struct BaseHash
{
    static T Algo;
};

// ------------------------------------------------------------------------------------------------
template < class T > T BaseHash< T >::Algo;

// ------------------------------------------------------------------------------------------------
template < class T > static SQInteger HashF(HSQUIRRELVM vm)
{
    const SQInteger top = sq_gettop(vm);
    // Are there any arguments on the stack?
    if (top <= 1)
    {
        LogErr("Attempting to <hash string> without specifying a value");
        // Push a null value on the stack
        sq_pushnull(vm);
    }
    // Is there a single string or at least something that can convert to a string on the stack?
    else if (top == 2 && ((sq_gettype(vm, -1) == OT_STRING) || !SQ_FAILED(sq_tostring(vm, -1))))
    {
        // Variable where the resulted string will be retrieved
        const SQChar * msg = 0;
        // Attempt to retrieve the specified string from the stack
        if (SQ_FAILED(sq_getstring(vm, -1, &msg)))
        {
            LogErr("Unable to <hash string> because : the string cannot be retrieved from the stack");
            // Pop any pushed values pushed to the stack
            sq_settop(vm, top);
            // Push a null value on the stack
            sq_pushnull(vm);
        }
        else
        {
            // Pop any pushed values pushed to the stack
            sq_settop(vm, top);
            // Hash the specified string
            sq_pushstring(vm, BaseHash< T >::Algo(msg).c_str(), -1);
        }
    }
    else if (top > 2)
    {
        // Variables containing the resulted string
        SQChar * msg = NULL;
        SQInteger len = 0;
        // Attempt to call the format function with the passed arguments
        if (SQ_FAILED(sqstd_format(vm, 2, &len, &msg)))
        {
            LogErr("Unable to <hash string> because : %s", Error::Message(vm).c_str());
            // Push a null value on the stack
            sq_pushnull(vm);
        }
        else
        {
            // Pop any pushed values pushed to the stack
            sq_settop(vm, top);
            // Hash the specified string
            sq_pushstring(vm, BaseHash< T >::Algo(msg).c_str(), -1);
        }
    }
    else
    {
        LogErr("Unable to <extract the log message> from the specified value");
        // Push a null value on the stack
        sq_pushnull(vm);
    }
    // At this point everything went correctly (probably)
    return 1;
}

// ================================================================================================
template < class T > static bool RegisterWrapper(HSQUIRRELVM vm, const SQChar * cname)
{
    // Typedef the reference type to simplify code
    typedef HashWrapper< T > Hash;
    // Output debugging information
    LogDbg("Beginning registration of <%s> type", cname);
    // Attempt to register the specified type
    Sqrat::RootTable(vm).Bind(cname, Sqrat::Class< Hash >(vm, cname)
        /* Constructors */
        .Ctor()
        /* Metamethods */
        .Func(_SC("_tostring"), &Hash::ToString)
        /* Properties */
        .Prop(_SC("hash"), &Hash::GetHash)
        /* Functions */
        .Func(_SC("reset"), &Hash::Reset)
        .Func(_SC("compute"), &Hash::Compute)
        .Func(_SC("get_hash"), &Hash::GetHash)
        .Func(_SC("add"), &Hash::AddStr)
        .Func(_SC("add_str"), &Hash::AddStr)

    );
    // Output debugging information
    LogDbg("Registration of <%s> type was successful", cname);
    // Registration succeeded
    return true;
}

// ------------------------------------------------------------------------------------------------
bool Register_Hash(HSQUIRRELVM vm)
{
    // Attempt to register the hash wrapers
    if (!RegisterWrapper< CRC32 >(vm, _SC("CHashCRC32")) ||
        !RegisterWrapper< Keccak >(vm, _SC("CHashKeccak")) ||
        !RegisterWrapper< MD5 >(vm, _SC("CHashMD5")) ||
        !RegisterWrapper< SHA1 >(vm, _SC("CHashSHA1")) ||
        !RegisterWrapper< SHA256 >(vm, _SC("CHashSHA256")) ||
        !RegisterWrapper< SHA3 >(vm, _SC("CHashSHA3")))
    {
        return false;
    }
    // Output debugging information
    LogDbg("Beginning registration of <Hashing functions> type");
    // Attempt to register the free functions
    Sqrat::RootTable(vm)
    .SquirrelFunc(_SC("HashCRC32"), &HashF< CRC32 >)
    .SquirrelFunc(_SC("HashKeccak"), &HashF< Keccak >)
    .SquirrelFunc(_SC("HashMD5"), &HashF< MD5 >)
    .SquirrelFunc(_SC("HashSHA1"), &HashF< SHA1 >)
    .SquirrelFunc(_SC("HashSHA256"), &HashF< SHA256 >)
    .SquirrelFunc(_SC("HashSHA3"), &HashF< SHA3 >);
    // Output debugging information
    LogDbg("Registration of <Hashing functions> type was successful");
    // Registration succeeded
    return true;
}

} // Namespace:: SqMod
