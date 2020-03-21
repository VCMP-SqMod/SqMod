#ifndef _LIBRARY_CRYPT_HASH_HPP_
#define _LIBRARY_CRYPT_HASH_HPP_

// ------------------------------------------------------------------------------------------------
#include "SqBase.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Simple class to maintain the state of an encoder.
*/
template < class T > class HashWrapper
{
public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    HashWrapper()
        : m_Encoder()
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Copy operator.
    */
    HashWrapper(const HashWrapper & o)
        : m_Encoder(o.m_Encoder)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~HashWrapper()
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    HashWrapper & operator = (const HashWrapper & o)
    {
        m_Encoder = o.m_Encoder;
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to convert an instance of this type to a string.
    */
    String ToString()
    {
        return m_Encoder.getHash();
    }

    /* --------------------------------------------------------------------------------------------
     * Reset the encoder state.
    */
    void Reset()
    {
        m_Encoder.reset();
    }

    /* --------------------------------------------------------------------------------------------
     * Compute the hash of the specified string.
    */
    String Compute(const String & str)
    {
        return m_Encoder(str);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the hash value of the data hashed so far.
    */
    String GetHash()
    {
        return m_Encoder.getHash();
    }

    /* --------------------------------------------------------------------------------------------
     * Add a string value to be hashed.
    */
    void AddStr(const String & str)
    {
        m_Encoder.add(str.data(), str.length() * sizeof(String::value_type));
    }

private:

    /* --------------------------------------------------------------------------------------------
     * The managed encoder state.
    */
    T m_Encoder;
};

} // Namespace:: SqMod

#endif // _LIBRARY_CRYPT_HASH_HPP_
