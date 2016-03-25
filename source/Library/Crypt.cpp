// ------------------------------------------------------------------------------------------------
#include "Library/Crypt.hpp"
#include "Base/Shared.hpp"

// ------------------------------------------------------------------------------------------------
#include <cstdlib>
#include <cstring>

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
SQInteger AES256::Typename(HSQUIRRELVM vm)
{
    static SQChar name[] = _SC("SqAES");
    sq_pushstring(vm, name, sizeof(name));
    return 1;
}

// ------------------------------------------------------------------------------------------------
AES256::AES256()
    : m_Context(), m_Buffer()
{
    aes256_done(&m_Context);
}

// ------------------------------------------------------------------------------------------------
AES256::AES256(CSStr key)
    : m_Context(), m_Buffer()
{
    Init(key);
}

// ------------------------------------------------------------------------------------------------
Int32 AES256::Cmp(const AES256 & o) const
{
    return memcmp(m_Buffer, o.m_Buffer, sizeof(m_Buffer));
}

// ------------------------------------------------------------------------------------------------
CSStr AES256::ToString() const
{
    return ToStrF("%s", m_Buffer);
}

// ------------------------------------------------------------------------------------------------
CSStr AES256::GetKey() const
{
    return reinterpret_cast< CSStr >(m_Buffer);
}

// ------------------------------------------------------------------------------------------------
void AES256::Init(CSStr key)
{
    // Clear current key, if any
    aes256_done(&m_Context);
    // Is the specified key empty?
    if (!key || *key == 0)
    {
        return; // Leave the context with an empty key
    }
    // Obtain the specified key size
    const Uint32 size = (strlen(key) * sizeof(SQChar));
    // See if the key size is accepted
    if (size > sizeof(m_Buffer))
    {
        STHROWF("The specified key is out of bounds: %u > %u", size, sizeof(m_Buffer));
    }
    // Initialize the key buffer to 0
    memset(m_Buffer, 0, sizeof(m_Buffer));
    // Copy the key into the key buffer
    memcpy(m_Buffer, key, size);
    // Initialize the context with the specified key
    aes256_init(&m_Context, m_Buffer);
}

// ------------------------------------------------------------------------------------------------
void AES256::Done()
{
    aes256_done(&m_Context);
}

// ------------------------------------------------------------------------------------------------
String AES256::Encrypt(CSStr data)
{
    // Is there any data to encrypt?
    if (!data || *data == 0)
    {
        return String();
    }
    // Copy the data into an editable string
    String str(data);
    // Make sure that we have a size with a multiple of 16
    if ((str.size() % 16) != 0)
    {
        str.resize(str.size() - (str.size() % 16) + 16);
    }
    // Encrypt in chunks of 16 characters
    for (Uint32 n = 0; n < str.size(); n += 16)
    {
        aes256_encrypt_ecb(&m_Context, reinterpret_cast< Uint8 * >(&str[n]));
    }
    // Return ownership of the encrypted string
    return std::move(str);
}

// ------------------------------------------------------------------------------------------------
String AES256::Decrypt(CSStr data)
{
    // Is there any data to decrypt?
    if (!data || *data == 0)
    {
        return String();
    }
    // Copy the data into an editable string
    String str(data);
    // Make sure that we have a size with a multiple of 16
    if ((str.size() % 16) != 0)
    {
        str.resize(str.size() - (str.size() % 16) + 16);
    }
    // Decrypt inc chunks of 16 characters
    for (Uint32 n = 0; n < str.size(); n += 16)
    {
        aes256_decrypt_ecb(&m_Context, reinterpret_cast< Uint8 * >(&str[n]));
    }
    // Remove null characters in case the string was not a multiple of 16 when encrypted
    while (!str.empty() && str.back() == 0)
    {
        str.pop_back();
    }
    // Return ownership of the encrypted string
    return std::move(str);
}

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
void Register_Crypt(HSQUIRRELVM vm)
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

    RootTable(vm).Bind("SqAES256", Class< AES256 >(vm, "SqAES256")
        /* Constructors */
        .Ctor()
        .Ctor< CSStr >()
        /* Metamethods */
        .Func(_SC("_cmp"), &AES256::Cmp)
        .SquirrelFunc(_SC("_typename"), &AES256::Typename)
        .Func(_SC("_tostring"), &AES256::ToString)
        /* Properties */
        .Prop(_SC("Key"), &AES256::GetKey)
        /* Functions */
        .Func(_SC("Init"), &AES256::Init)
        .Func(_SC("Done"), &AES256::Done)
        .Func(_SC("Encrypt"), &AES256::Encrypt)
        .Func(_SC("Decrypt"), &AES256::Decrypt)
    );
}

} // Namespace:: SqMod
