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
#include <b64.h>
#include <whirlpool.h>

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
    if (SQ_FAILED(val.Proc(true)))
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

/* ------------------------------------------------------------------------------------------------
 * Hash the specified value or the result of a formatted string with whirlpool algorithm.
*/
static SQInteger WhirlpoolF(HSQUIRRELVM vm)
{
    // Attempt to retrieve the value from the stack as a string
    StackStrF val(vm, 2);
    // Have we failed to retrieve the string?
    if (SQ_FAILED(val.Proc(true)))
    {
        return val.mRes; // Propagate the error!
    }
    // Prepare a whirlpool hashing context
    whirlpool_ctx ctx;
    // Initialize the hashing context
    rhash_whirlpool_init(&ctx);
    // Update the context with the given string
    rhash_whirlpool_update(&ctx, reinterpret_cast< const unsigned char * >(val.mPtr),
                                    val.mLen < 0 ? 0 : static_cast< size_t >(val.mLen));
    // Reserve space for the result in binary form
    unsigned char raw_hash[whirlpool_block_size];
    // Finalize hashing and obtain the result
    rhash_whirlpool_final(&ctx, raw_hash);
    // Reserve space for the hexadecimal string
    char hex_hash[whirlpool_block_size * 2];
    // Convert from binary form to hex string
    for (int i = 0, p = 0; i < whirlpool_block_size; ++i)
    {
        static const char dec2hex[16+1] = "0123456789abcdef";
        hex_hash[p++] = dec2hex[(raw_hash[i] >> 4) & 15];
        hex_hash[p++] = dec2hex[ raw_hash[i]       & 15];
    }
    // Push the string on the stack
    sq_pushstring(vm, hex_hash, whirlpool_block_size * 2);
    // At this point we have a valid string on the stack
    return 1;
}

/* ------------------------------------------------------------------------------------------------
 * Encode the specified value or the result of a formatted string with base64 algorithm.
*/
static SQInteger EncodeBase64F(HSQUIRRELVM vm)
{
    // Attempt to retrieve the value from the stack as a string
    StackStrF val(vm, 2);
    // Have we failed to retrieve the string?
    if (SQ_FAILED(val.Proc(true)))
    {
        return val.mRes; // Propagate the error!
    }
    // Size of the encoded string
    size_t enclen = 0;
    // Attempt to encode the resulted string
    char * result = b64_encode_ex(reinterpret_cast< const unsigned char * >(val.mPtr),
                                val.mLen < 0 ? 0 : static_cast< size_t >(val.mLen), &enclen);
    // Did we fail to allocate memory for it?
    if (!result)
    {
        return sq_throwerror(vm, _SC("Unable to allocate memory for output"));
    }
    // Push the string on the stack
    sq_pushstring(vm, result, ConvTo< SQInteger >::From(enclen));
    // At this point we have a valid string on the stack
    return 1;
}

/* ------------------------------------------------------------------------------------------------
 * Decode the specified value or the result of a formatted string with base64 algorithm.
*/
static SQInteger DecodeBase64F(HSQUIRRELVM vm)
{
    // Attempt to retrieve the value from the stack as a string
    StackStrF val(vm, 2);
    // Have we failed to retrieve the string?
    if (SQ_FAILED(val.Proc(true)))
    {
        return val.mRes; // Propagate the error!
    }
    // Size of the decoded string
    size_t declen = 0;
    // Attempt to decode the resulted string
    unsigned char * result = b64_decode_ex(val.mPtr, val.mLen < 0 ? 0 : static_cast< size_t >(val.mLen), &declen);
    // Did we fail to allocate memory for it?
    if (!result)
    {
        return sq_throwerror(vm, _SC("Unable to allocate memory for output"));
    }
    // Push the string on the stack
    sq_pushstring(vm, reinterpret_cast< CSStr >(result), ConvTo< SQInteger >::From(declen));
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
    hashns.SquirrelFunc(_SC("GetWhirlpool"), &WhirlpoolF);
    hashns.SquirrelFunc(_SC("EncodeBase64"), &EncodeBase64F);
    hashns.SquirrelFunc(_SC("DecodeBase64"), &DecodeBase64F);

    RootTable(vm).Bind(_SC("SqHash"), hashns);
}

} // Namespace:: SqMod
