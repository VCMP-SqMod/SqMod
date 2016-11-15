// ------------------------------------------------------------------------------------------------
#include "Library/Crypt/Hash.hpp"
#include "Base/Shared.hpp"

// ------------------------------------------------------------------------------------------------
#include <crc32.h>
#include <keccak.h>
#include <md5.h>
#include <sha1.h>
#include <sha256.h>
#include <sha3.h>

// ------------------------------------------------------------------------------------------------
#include <cstdlib>
#include <cstring>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Utility to avoid creating encoder instances for each call.
*/
template < class T > struct BaseHash
{
    static T Algo;
};

// ------------------------------------------------------------------------------------------------
template < class T > T BaseHash< T >::Algo;

/* ------------------------------------------------------------------------------------------------
 * Hash the specified value or the result of a formatted string.
*/
template < class T > static SQInteger HashF(HSQUIRRELVM vm)
{
    // Attempt to retrieve the value from the stack as a string
    StackStrF val(vm, 2);
    // Have we failed to retrieve the string?
    if (SQ_FAILED(val.mRes))
    {
        return val.mRes; // Propagate the error!
    }
    // Forward the call to the actual implementation and store the string
    String str(BaseHash< T >::Algo(val.mPtr));
    // Push the string on the stack
    sq_pushstring(vm, str.data(), str.size());
    // At this point we have a valid string on the stack
    return 1;
}

// ================================================================================================
template < class T > static void RegisterWrapper(Table & hashns, CCStr cname)
{
    typedef HashWrapper< T > Hash;
    hashns.Bind(cname,
        Class< Hash >(hashns.GetVM(), cname)
        // Constructors
        .Ctor()
        // Meta-methods
        .Func(_SC("_tostring"), &Hash::ToString)
        // Properties
        .Prop(_SC("Hash"), &Hash::GetHash)
        // Functions
        .Func(_SC("Reset"), &Hash::Reset)
        .Func(_SC("Compute"), &Hash::Compute)
        .Func(_SC("GetHash"), &Hash::GetHash)
        .Func(_SC("Add"), &Hash::AddStr)
        .Func(_SC("AddStr"), &Hash::AddStr)
    );
}

// ================================================================================================
void Register_Hash(HSQUIRRELVM vm)
{
    Table hashns(vm);

    RegisterWrapper< CRC32 >(hashns, _SC("CRC32"));
    RegisterWrapper< Keccak >(hashns, _SC("Keccak"));
    RegisterWrapper< MD5 >(hashns, _SC("MD5"));
    RegisterWrapper< SHA1 >(hashns, _SC("SHA1"));
    RegisterWrapper< SHA256 >(hashns, _SC("SHA256"));
    RegisterWrapper< SHA3 >(hashns, _SC("SHA3"));

    hashns.SquirrelFunc(_SC("GetCRC32"), &HashF< CRC32 >);
    hashns.SquirrelFunc(_SC("GetKeccak"), &HashF< Keccak >);
    hashns.SquirrelFunc(_SC("GetMD5"), &HashF< MD5 >);
    hashns.SquirrelFunc(_SC("GetSHA1"), &HashF< SHA1 >);
    hashns.SquirrelFunc(_SC("GetSHA256"), &HashF< SHA256 >);
    hashns.SquirrelFunc(_SC("GetSHA3"), &HashF< SHA3 >);

    RootTable(vm).Bind(_SC("SqHash"), hashns);
}

} // Namespace:: SqMod
