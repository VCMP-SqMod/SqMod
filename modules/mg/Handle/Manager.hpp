#ifndef _SQMG_HANDLE_MANAGER_HPP_
#define _SQMG_HANDLE_MANAGER_HPP_

// ------------------------------------------------------------------------------------------------
#include "Common.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Manages a reference counted manager and its associated resources.
*/
class ManagerHnd
{
    // --------------------------------------------------------------------------------------------
    friend class Manager;
    friend class Connection;

public:

    // --------------------------------------------------------------------------------------------
    typedef mg_mgr                  Type; // The managed type.

    // --------------------------------------------------------------------------------------------
    typedef Type*                   Pointer; // Pointer to the managed type.
    typedef const Type*             ConstPtr; // Constant pointer to the managed type.

    // --------------------------------------------------------------------------------------------
    typedef Type&                   Reference; // Reference to the managed type.
    typedef const Type&             ConstRef; // Constant reference to the managed type.

    // --------------------------------------------------------------------------------------------
    typedef unsigned int            Counter; // Reference counter type.

    /* --------------------------------------------------------------------------------------------
     * Validate the manager handle and throw an error if invalid.
    */
    void Validate() const;

    /* --------------------------------------------------------------------------------------------
     * Validate the manager handle and throw an error if invalid.
    */
    void Validate(CSStr act) const;

protected:

    /* --------------------------------------------------------------------------------------------
     * The structure that holds the data associated with a physically simulated world.
    */
    struct Handle
    {
        // ----------------------------------------------------------------------------------------
        Type                    mMgr; // The managed manager structure.
        Counter                 mRef; // Reference count to the managed handles.

        /* ----------------------------------------------------------------------------------------
         * Base constructor.
        */
        Handle();

        /* ----------------------------------------------------------------------------------------
         * Destructor.
        */
        ~Handle();

        /* ----------------------------------------------------------------------------------------
         * Pool the managed connections for events.
        */
        Int64 Pool(Int32 milli);
    };

    // --------------------------------------------------------------------------------------------
    typedef std::vector< Handle * > Managers; // List of created managers.

    // --------------------------------------------------------------------------------------------
    static Managers s_Managers;

private:

    // --------------------------------------------------------------------------------------------
    Handle * m_Hnd; // The managed handle instance.

    /* --------------------------------------------------------------------------------------------
     * Grab a strong reference to a connection handle.
    */
    void Grab()
    {
        if (m_Hnd)
        {
            ++(m_Hnd->mRef);
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Drop a strong reference to a connection handle.
    */
    void Drop()
    {
        if (m_Hnd && --(m_Hnd->mRef) == 0)
        {
            delete m_Hnd; // Let the destructor take care of cleaning up (if necessary)
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    ManagerHnd(HndInit init)
        : m_Hnd(init == HndInit::InitHnd ? new Handle() : nullptr)
    {
        /* ... */
    }

public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor (null).
    */
    ManagerHnd()
        : m_Hnd(nullptr)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Handle constructor.
    */
    ManagerHnd(Handle * hnd)
        : m_Hnd(hnd)
    {
        Grab();
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    ManagerHnd(const ManagerHnd & o)
        : m_Hnd(o.m_Hnd)
    {
        Grab();
    }

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    ManagerHnd(ManagerHnd && o)
        : m_Hnd(o.m_Hnd)
    {
        o.m_Hnd = nullptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~ManagerHnd()
    {
        Drop();
    }

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    ManagerHnd & operator = (const ManagerHnd & o)
    {
        if (m_Hnd != o.m_Hnd)
        {
            Drop();
            m_Hnd = o.m_Hnd;
            Grab();
        }
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    ManagerHnd & operator = (ManagerHnd && o)
    {
        if (m_Hnd != o.m_Hnd)
        {
            m_Hnd = o.m_Hnd;
            o.m_Hnd = nullptr;
        }

        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Perform an equality comparison between two connection handles.
    */
    bool operator == (const ManagerHnd & o) const
    {
        return (m_Hnd == o.m_Hnd);
    }

    /* --------------------------------------------------------------------------------------------
     * Perform an inequality comparison between two connection handles.
    */
    bool operator != (const ManagerHnd & o) const
    {
        return (m_Hnd != o.m_Hnd);
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to boolean for use in boolean operations.
    */
    operator bool () const
    {
        return (m_Hnd != nullptr);
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to the managed instance.
    */
    operator Pointer ()
    {
        return (m_Hnd != nullptr) ? &(m_Hnd->mMgr) : nullptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to the managed instance.
    */
    operator Pointer () const
    {
        return (m_Hnd != nullptr) ? &(m_Hnd->mMgr) : nullptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to the managed instance.
    */
    operator Reference ()
    {
        assert((m_Hnd != nullptr));
        return (m_Hnd->mMgr);
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to the managed instance.
    */
    operator ConstRef () const
    {
        assert((m_Hnd != nullptr));
        return (m_Hnd->mMgr);
    }

    /* --------------------------------------------------------------------------------------------
     * Member operator for dereferencing the managed pointer.
    */
    Handle * operator -> () const
    {
        assert(m_Hnd != nullptr);
        return m_Hnd;
    }

    /* --------------------------------------------------------------------------------------------
     * Indirection operator for obtaining a reference of the managed pointer.
    */
    Handle & operator * () const
    {
        assert(m_Hnd != nullptr);
        return *m_Hnd;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw handle structure pointer.
    */
    Handle * HndPtr()
    {
        return m_Hnd;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw handle structure pointer.
    */
    Handle * HndPtr() const
    {
        return m_Hnd;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the number of active references to the managed instance.
    */
    Counter Count() const
    {
        return (m_Hnd != nullptr) ? m_Hnd->mRef : 0;
    }
};

} // Namespace:: SqMod

#endif // _SQMG_HANDLE_MANAGER_HPP_
