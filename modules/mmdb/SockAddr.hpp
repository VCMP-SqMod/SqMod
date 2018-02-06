#ifndef _SQMMDB_SOCKADDR_HPP_
#define _SQMMDB_SOCKADDR_HPP_

// ------------------------------------------------------------------------------------------------
#include "Common.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Class that can obtain information from string addresses and be used repeatedly thereafter.
*/
class SockAddr
{
public:

    // --------------------------------------------------------------------------------------------
    typedef struct addrinfo Type; // The managed type.

    // --------------------------------------------------------------------------------------------
    typedef Type*           Pointer; // Pointer to the managed type.
    typedef const Type*     ConstPtr; // Constant pointer to the managed type.

    // --------------------------------------------------------------------------------------------
    typedef Type&           Reference; // Reference to the managed type.
    typedef const Type&     ConstRef; // Constant reference to the managed type.

protected:

    /* --------------------------------------------------------------------------------------------
     * Validate the managed sockaddr pointer and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    void Validate(CCStr file, Int32 line) const;
#else
    void Validate() const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Validate the managed sockaddr pointer and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    Pointer GetValid(CCStr file, Int32 line) const;
#else
    Pointer GetValid() const;
#endif // _DEBUG

private:

    // ---------------------------------------------------------------------------------------------
    Pointer m_Handle; // The managed sockaddr structure.
    String  m_Addres; // The address that was queried for information.

public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    SockAddr()
        : m_Handle(nullptr), m_Addres(_SC(""))
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    SockAddr(CSStr addr);

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    SockAddr(const SockAddr & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    SockAddr(SockAddr && o)
        : m_Handle(o.m_Handle)
        , m_Addres(o.m_Addres)
    {
        o.m_Handle = nullptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~SockAddr();

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator. (disabled)
    */
    SockAddr & operator = (const SockAddr & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    SockAddr & operator = (SockAddr && o)
    {
        if (m_Handle != o.m_Handle)
        {
            m_Handle = o.m_Handle;
            m_Addres = o.m_Addres;
            o.m_Handle = nullptr;
        }
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the internal addrinfo structure pointer.
    */
    Pointer GetHandle()
    {
        return m_Handle;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the internal addrinfo structure pointer.
    */
    Pointer GetHandle() const
    {
        return m_Handle;
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to convert an instance of this type to a string.
    */
    CSStr ToString() const
    {
        return m_Addres.c_str();
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to retrieve the name from instances of this type.
    */
    static SQInteger Typename(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * See whether this instance references a valid addrinfo structure.
    */
    bool IsValid() const
    {
        return m_Handle;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated string address.
    */
    CSStr GetAddress() const
    {
        return m_Addres.c_str();
    }
};

} // Namespace:: SqMod

#endif // _SQMMDB_SOCKADDR_HPP_
