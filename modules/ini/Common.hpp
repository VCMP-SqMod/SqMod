#ifndef _SQINI_COMMON_HPP_
#define _SQINI_COMMON_HPP_

// ------------------------------------------------------------------------------------------------
#include "ModBase.hpp"

// ------------------------------------------------------------------------------------------------
#include <assert.h>

// ------------------------------------------------------------------------------------------------
#include <SimpleIni.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * SOFTWARE INFORMATION
*/
#define SQINI_NAME "Squirrel INI Module"
#define SQINI_AUTHOR "Sandu Liviu Catalin (S.L.C)"
#define SQINI_COPYRIGHT "Copyright (C) 2016 Sandu Liviu Catalin"
#define SQINI_HOST_NAME "SqModINIHost"
#define SQINI_VERSION 001
#define SQINI_VERSION_STR "0.0.1"
#define SQINI_VERSION_MAJOR 0
#define SQINI_VERSION_MINOR 0
#define SQINI_VERSION_PATCH 1

/* ------------------------------------------------------------------------------------------------
 * Forward declarations.
*/
class Entries;
class Document;

/* ------------------------------------------------------------------------------------------------
 * Retrieve the temporary buffer.
*/
SStr GetTempBuff();

/* ------------------------------------------------------------------------------------------------
 * Retrieve the size of the temporary buffer.
*/
Uint32 GetTempBuffSize();

/* ------------------------------------------------------------------------------------------------
 * Throw a formatted exception.
*/
void SqThrowF(CSStr str, ...);

/* ------------------------------------------------------------------------------------------------
 * Generate a formatted string.
*/
CSStr FmtStr(CSStr str, ...);

/* ------------------------------------------------------------------------------------------------
 * Implements RAII to restore the VM stack to it's initial size on function exit.
*/
struct StackGuard
{
    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    StackGuard(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~StackGuard();

private:

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    StackGuard(const StackGuard &);

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    StackGuard(StackGuard &&);

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    StackGuard & operator = (const StackGuard &);

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    StackGuard & operator = (StackGuard &&);

private:

    // --------------------------------------------------------------------------------------------
    Int32       m_Top; /* The top of the stack when this instance was created. */
    HSQUIRRELVM m_VM; /* The VM where the stack should be restored. */
};

/* ------------------------------------------------------------------------------------------------
 * Manages a reference counted INI document instance.
*/
class DocumentRef
{
    // --------------------------------------------------------------------------------------------
    friend class Document;

public:

    // --------------------------------------------------------------------------------------------
    typedef CSimpleIniA     Type; // The managed type.

    // --------------------------------------------------------------------------------------------
    typedef Type*           Pointer; // Pointer to the managed type.
    typedef const Type*     ConstPtr; // Constant pointer to the managed type.

    // --------------------------------------------------------------------------------------------
    typedef Type&           Reference; // Reference to the managed type.
    typedef const Type&     ConstRef; // Constant reference to the managed type.

    // --------------------------------------------------------------------------------------------
    typedef unsigned int    Counter; // Reference counter type.

private:

    // --------------------------------------------------------------------------------------------
    Pointer     m_Ptr; // The document reader, writer and manager instance.
    Counter*    m_Ref; // Reference count to the managed instance.

    /* --------------------------------------------------------------------------------------------
     * Grab a strong reference to a document instance.
    */
    void Grab()
    {
        if (m_Ptr)
            ++(*m_Ref);
    }

    /* --------------------------------------------------------------------------------------------
     * Drop a strong reference to a document instance.
    */
    void Drop()
    {
        if (m_Ptr && --(*m_Ref) == 0)
        {
            delete m_Ptr;
            delete m_Ref;
            m_Ptr = NULL;
            m_Ref = NULL;
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    DocumentRef(bool utf8, bool multikey, bool multiline)
        : m_Ptr(new Type(utf8, multikey, multiline)), m_Ref(new Counter(1))
    {
        /* ... */
    }

public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor (null).
    */
    DocumentRef()
        : m_Ptr(NULL), m_Ref(NULL)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    DocumentRef(const DocumentRef & o)
        : m_Ptr(o.m_Ptr), m_Ref(o.m_Ref)

    {
        Grab();
    }

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    DocumentRef(DocumentRef && o)
        : m_Ptr(o.m_Ptr), m_Ref(o.m_Ref)

    {
        o.m_Ptr = NULL;
        o.m_Ref = NULL;
    }

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~DocumentRef()
    {
        Drop();
    }

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    DocumentRef & operator = (const DocumentRef & o)
    {
        if (m_Ptr != o.m_Ptr)
        {
            Drop();
            m_Ptr = o.m_Ptr;
            m_Ref = o.m_Ref;
            Grab();
        }
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    DocumentRef & operator = (DocumentRef && o)
    {
        if (m_Ptr != o.m_Ptr)
        {
            m_Ptr = o.m_Ptr;
            m_Ref = o.m_Ref;
            o.m_Ptr = NULL;
            o.m_Ref = NULL;
        }
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Perform an equality comparison between two document instances.
    */
    bool operator == (const DocumentRef & o) const
    {
        return (m_Ptr == o.m_Ptr);
    }

    /* --------------------------------------------------------------------------------------------
     * Perform an inequality comparison between two document instances.
    */
    bool operator != (const DocumentRef & o) const
    {
        return (m_Ptr != o.m_Ptr);
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to boolean for use in boolean operations.
    */
    operator bool () const
    {
        return m_Ptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to the managed instance pointer.
    */
    operator Pointer ()
    {
        return m_Ptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to the managed instance pointer.
    */
    operator ConstPtr () const
    {
        return m_Ptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to the managed instance reference.
    */
    operator Reference ()
    {
        assert(m_Ptr);
        return *m_Ptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to the managed instance reference.
    */
    operator ConstRef () const
    {
        assert(m_Ptr);
        return *m_Ptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Member operator for dereferencing the managed pointer.
    */
    Pointer operator -> () const
    {
        assert(m_Ptr);
        return m_Ptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Indirection operator for obtaining a reference of the managed pointer.
    */
    Reference operator * () const
    {
        assert(m_Ptr);
        return *m_Ptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the number of active references to the managed instance.
    */
    Counter Count() const
    {
        return (m_Ptr && m_Ref) ? (*m_Ref) : 0;
    }
};

/* ------------------------------------------------------------------------------------------------
 * Allows the user to inspect the result of certain operations and act accordingly.
*/
class IniResult
{
private:

    // --------------------------------------------------------------------------------------------
    String      m_Action; /* The action that was performed. */
    SQInteger   m_Result; /* The internal result code. */

public:

    /* --------------------------------------------------------------------------------------------
     * Construct with no specific action or result.
    */
    IniResult()
        : m_Action("unknown action"), m_Result(SI_OK)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Construct with no specific result.
    */
    explicit IniResult(CSStr action)
        : m_Action(!action ? _SC("") : action), m_Result(SI_OK)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Construct with no specific action.
    */
    explicit IniResult(SQInteger result)
        : m_Action("unknown action"), m_Result(result)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Construct with specific action and result.
    */
    IniResult(CSStr action, SQInteger result)
        : m_Action(!action ? _SC("") : action), m_Result(result)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    IniResult(const IniResult & o)
        : m_Action(o.m_Action), m_Result(o.m_Result)

    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~IniResult()
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    IniResult & operator = (const IniResult & o)
    {
        m_Action = o.m_Action;
        m_Result = o.m_Result;
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Perform an equality comparison between two results.
    */
    bool operator == (const IniResult & o) const
    {
        return (m_Result == o.m_Result);
    }

    /* --------------------------------------------------------------------------------------------
     * Perform an inequality comparison between two results.
    */
    bool operator != (const IniResult & o) const
    {
        return (m_Result != o.m_Result);
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to boolean for use in boolean operations.
    */
    operator bool () const
    {
        return (m_Result >= 0);
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare two instances of this type.
    */
    Int32 Cmp(const IniResult & o) const
    {
        if (m_Result == o.m_Result)
            return 0;
        else if (m_Result > o.m_Result)
            return 1;
        else
            return -1;
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to convert an instance of this type to a string.
    */
    CSStr ToString() const
    {
        return m_Action.c_str();
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to retrieve the name from instances of this type.
    */
    static SQInteger Typename(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * See whether this instance references a valid INI result.
    */
    bool IsValid() const
    {
        return (m_Result >= 0);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated action.
    */
    CSStr GetAction() const
    {
        return m_Action.c_str();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the resulted code.
    */
    SQInteger GetResult() const
    {
        return m_Result;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the resulted code.
    */
    void Check() const;
};

} // Namespace:: SqMod

#endif // _SQINI_COMMON_HPP_
