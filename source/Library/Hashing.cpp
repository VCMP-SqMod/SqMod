// ------------------------------------------------------------------------------------------------
#include "Library/Hashing.hpp"
#include "Base/Shared.hpp"

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
    const Int32 top = sq_gettop(vm);
    Object ret;
    if (top <= 1)
        SqThrow("Missing the hash string");
    else if (top == 2 && ((sq_gettype(vm, -1) == OT_STRING) || !SQ_FAILED(sq_tostring(vm, -1))))
    {
        CCStr str = 0;
        if (SQ_FAILED(sq_getstring(vm, -1, &str)))
            SqThrow("Unable to retrieve the string");
        else
            ret = MakeObject(vm, BaseHash< T >::Algo(str));
        sq_settop(vm, top);
    }
    else if (top > 2)
    {
        CStr str = NULL;
        SQInteger len = 0;
        if (SQ_FAILED(sqstd_format(vm, 2, &len, &str)))
            SqThrow("Unable to generate the string [%s]", Error::Message(vm).c_str());
        else
            ret = MakeObject(vm, BaseHash< T >::Algo(str));
    }
    else
        SqThrow("Unable to extract the hash string");
    Var< Object >::push(vm, ret);
    return 1;
}

// ================================================================================================
template < class T > static void RegisterWrapper(Table & hashns, CCStr cname)
{
    typedef HashWrapper< T > Hash;
    hashns.Bind(cname, Class< Hash >(hashns.GetVM(), cname)
        /* Constructors */
        .Ctor()
        /* Metamethods */
        .Func(_SC("_tostring"), &Hash::ToString)
        /* Properties */
        .Prop(_SC("Hash"), &Hash::GetHash)
        /* Functions */
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
