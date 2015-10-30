#include "Library/Hashing.hpp"
#include "Register.hpp"

// ------------------------------------------------------------------------------------------------
#include <crc32.h>
#include <keccak.h>
#include <md5.h>
#include <sha1.h>
#include <sha256.h>
#include <sha3.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
static CRC32        g_EncodeCRC32;
static Keccak       g_EncodeKeccak;
static MD5          g_EncodeMD5;
static SHA1         g_EncodeSHA1;
static SHA256       g_EncodeSHA256;
static SHA3         g_EncodeSHA3;

// ------------------------------------------------------------------------------------------------
String HashCRC32(const String & str) noexcept
{
    return g_EncodeCRC32(str);
}

// ------------------------------------------------------------------------------------------------
String HashKeccak(const String & str) noexcept
{
    return g_EncodeKeccak(str);
}

// ------------------------------------------------------------------------------------------------
String HashMD5(const String & str) noexcept
{
    return g_EncodeMD5(str);
}

// ------------------------------------------------------------------------------------------------
String HashSHA1(const String & str) noexcept
{
    return g_EncodeSHA1(str);
}

// ------------------------------------------------------------------------------------------------
String HashSHA256(const String & str) noexcept
{
    return g_EncodeSHA256(str);
}

// ------------------------------------------------------------------------------------------------
String HashSHA3(const String & str) noexcept
{
    return g_EncodeSHA3(str);
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
    .Func(_SC("HashCRC32"), &HashCRC32)
    .Func(_SC("HashKeccak"), &HashKeccak)
    .Func(_SC("HashMD5"), &HashMD5)
    .Func(_SC("HashSHA1"), &HashSHA1)
    .Func(_SC("HashSHA256"), &HashSHA256)
    .Func(_SC("HashSHA3"), &HashSHA3);
    // Output debugging information
    LogDbg("Registration of <Hashing functions> type was successful");
    // Registration succeeded
    return true;
}

} // Namespace:: SqMod
