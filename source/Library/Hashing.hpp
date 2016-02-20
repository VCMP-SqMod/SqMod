#ifndef _LIBRARY_HASHING_HPP_
#define _LIBRARY_HASHING_HPP_

// ------------------------------------------------------------------------------------------------
#include "SqBase.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
template < class T > class HashWrapper
{
public:

    // --------------------------------------------------------------------------------------------
    HashWrapper()
        : m_Encoder()
    {

    }

    // --------------------------------------------------------------------------------------------
    HashWrapper(const HashWrapper & o)
        : m_Encoder(o.m_Encoder)
    {

    }

    // --------------------------------------------------------------------------------------------
    ~HashWrapper()
    {

    }

    // --------------------------------------------------------------------------------------------
    HashWrapper & operator = (const HashWrapper & o)
    {
        m_Encoder = o.m_Encoder;
        return *this;
    }

    // --------------------------------------------------------------------------------------------
    String ToString() { return m_Encoder.getHash(); }
    void Reset() { m_Encoder.reset(); }
    String Compute(const String & str) { return m_Encoder(str); }
    String GetHash() { return m_Encoder.getHash(); }
    void AddStr(const String & str)
    {  m_Encoder.add(str.data(), str.length() * sizeof(String::value_type)); }

private:

    // --------------------------------------------------------------------------------------------
    T   m_Encoder;
};

} // Namespace:: SqMod

#endif // _LIBRARY_HASHING_HPP_
