#ifndef _LIBRARY_CRYPT_AES_HPP_
#define _LIBRARY_CRYPT_AES_HPP_

// ------------------------------------------------------------------------------------------------
#include "SqBase.hpp"

// ------------------------------------------------------------------------------------------------
#include <aes256.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Simple wrapper around a the AES encryption context.
*/
class AES256
{
public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    AES256();

    /* --------------------------------------------------------------------------------------------
     * Construct with an explicit key.
    */
    AES256(CSStr key);

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    AES256(const AES256 & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    AES256(AES256 && o) = default;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~AES256() = default;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    AES256 & operator = (const AES256 & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    AES256 & operator = (AES256 && o) = default;

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare two instances of this type.
    */
    Int32 Cmp(const AES256 & o) const;

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to convert an instance of this type to a string.
    */
    CSStr ToString() const;

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to retrieve the name from instances of this type.
    */
    static SQInteger Typename(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated key.
    */
    CSStr GetKey() const;

    /* --------------------------------------------------------------------------------------------
     * Initialize the context key.
    */
    bool Init(CSStr key);

    /* --------------------------------------------------------------------------------------------
     * Reset the associated context.
    */
    void Done();

    /* --------------------------------------------------------------------------------------------
     * Encrypt the specified string.
    */
    String Encrypt(CSStr data);

    /* --------------------------------------------------------------------------------------------
     * Decrypt the specified string.
    */
    String Decrypt(CSStr data);

private:

    /* --------------------------------------------------------------------------------------------
     * The managed encryption context.
    */
    aes256_context  m_Context;

    /* --------------------------------------------------------------------------------------------
     * The key used to encrypt data.
    */
    Uint8           m_Buffer[32]{0};
};

} // Namespace:: SqMod

#endif // _LIBRARY_CRYPT_AES_HPP_
