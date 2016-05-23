// ------------------------------------------------------------------------------------------------
#include "Library/Crypt/AES.hpp"
#include "Base/Shared.hpp"
#include "Base/Stack.hpp"

// ------------------------------------------------------------------------------------------------
#include <cstdlib>
#include <cstring>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQInteger AES256::Typename(HSQUIRRELVM vm)
{
    static const SQChar name[] = _SC("SqAES256");
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
    return std::memcmp(m_Buffer, o.m_Buffer, sizeof(m_Buffer));
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
bool AES256::Init(CSStr key)
{
    // Clear current key, if any
    aes256_done(&m_Context);
    // Is the specified key empty?
    if (!key || *key == '\0')
    {
        return false; // Leave the context with an empty key
    }
    // Obtain the specified key size
    const Uint32 size = (std::strlen(key) * sizeof(SQChar));
    // See if the key size is accepted
    if (size > sizeof(m_Buffer))
    {
        STHROWF("The specified key is out of bounds: %u > %u", size, sizeof(m_Buffer));
    }
    // Initialize the key buffer to 0
    std::memset(m_Buffer, 0, sizeof(m_Buffer));
    // Copy the key into the key buffer
    std::memcpy(m_Buffer, key, size);
    // Initialize the context with the specified key
    aes256_init(&m_Context, m_Buffer);
    // This context was successfully initialized
    return true;
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

// ================================================================================================
void Register_AES(HSQUIRRELVM vm)
{
    RootTable(vm).Bind("SqAES256", Class< AES256 >(vm, "SqAES256")
        // Constructors
        .Ctor()
        .Ctor< CSStr >()
        // Metamethods
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
