#pragma once

/* ------------------------------------------------------------------------------------------------
 * This could have been much more flexible and advanced than this (as initially planned).
 * However, several factors played an important role in its current simplicity:
 * 1) Time constraints:
 *      This had to be done over the course of a weekend (at worst). It might be extended
 *      over time but right now this will do.
 * 2) Performance cost:
 *      Privilege micro-management will become costly if scripting is involved. Therefore, several
 *      sacrifices had to be made. Such as type-agnostic status value, script controlled query etc.
*/

// ------------------------------------------------------------------------------------------------
#include "Base/Shared.hpp"

// ------------------------------------------------------------------------------------------------
#include <vector>
#include <utility>
#include <algorithm>
#include <unordered_map>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
struct PvUnit;
struct PvClass;
struct PvEntry;

// ------------------------------------------------------------------------------------------------
class PvUnit;
class PvClass;
class PvEntry;
class PvManager;

// ------------------------------------------------------------------------------------------------
typedef std::vector< PvManager * >                          PvManagers;
// ------------------------------------------------------------------------------------------------
typedef std::vector< std::pair< SQInteger, SQInteger > >    PvStatusList;

/* ------------------------------------------------------------------------------------------------
 * An individual unit/entity that inherits the privileges of a class/group and can
 * change their status without affecting other units and/or their associated class.
 * A unit cannot extend it's privileges to other entities.
 * Units cost more to query if they differ from their class but save memory in large numbers.
 * Each unit must have a unique numerical identifier within their associated manager.
*/
struct PvUnit
{
    /* --------------------------------------------------------------------------------------------
     * Strong and weak reference types.
    */
    typedef SharedPtr< PvUnit > Ref;
    typedef WeakPtr< PvUnit > Ptr;

    /* --------------------------------------------------------------------------------------------
     * Type of container used to store privilege units.
    */
    typedef std::vector< std::pair< SQInteger, Ref > >   List;

    /* --------------------------------------------------------------------------------------------
     * User identifier associated with this unit instance.
    */
    SQInteger           mID;

    /* --------------------------------------------------------------------------------------------
     * Authority level associated with a particular unit.
    */
    SQInteger           mAuthority;

    /* --------------------------------------------------------------------------------------------
     * A container with unique privilege status values associated with this unit.
    */
    PvStatusList        mPrivileges;

    /* --------------------------------------------------------------------------------------------
     * Dedicated callback for privilege query event.
    */
    Function            mOnQuery;

    /* --------------------------------------------------------------------------------------------
     * Dedicated callback for privilege modify event.
    */
    Function            mOnModify;

    /* --------------------------------------------------------------------------------------------
     * Dedicated callback for privilege gained event.
    */
    Function            mOnGained;

    /* --------------------------------------------------------------------------------------------
     * Dedicated callback for privilege lost event.
    */
    Function            mOnLost;

    /* --------------------------------------------------------------------------------------------
     * User tag associated with this instance.
    */
    StackStrF           mTag;

    /* --------------------------------------------------------------------------------------------
     * User data associated with this instance.
    */
    LightObj            mData;

    /* --------------------------------------------------------------------------------------------
     * Pointer to the associated class.
    */
    WeakPtr< PvClass >  mClass;

    /* -------------------------------------------------------------------------------------------
     * Default constructor.
    */
    PvUnit(SQInteger id, WeakPtr< PvClass > cls)
        : mID(id)
        , mAuthority(0)
        , mPrivileges()
        , mOnQuery(), mOnGained(), mOnLost()
        , mTag(), mData()
        , mClass(std::move(cls))
    {
    }

    /* -------------------------------------------------------------------------------------------
     * Default constructor.
    */
    PvUnit(SQInteger id, StackStrF && tag, WeakPtr< PvClass > cls)
        : mID(id)
        , mAuthority(0)
        , mPrivileges()
        , mOnQuery(), mOnGained(), mOnLost()
        , mTag(std::forward< StackStrF >(tag)), mData()
        , mClass(std::move(cls))
    {
    }

    /* -------------------------------------------------------------------------------------------
     * Copy constructor (disabled).
    */
    PvUnit(const PvUnit & o) = delete;

    /* -------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~PvUnit();

    /* -------------------------------------------------------------------------------------------
     * Copy assignment operator (disabled).
    */
    PvUnit & operator = (const PvUnit & o) = delete;

    /* -------------------------------------------------------------------------------------------
     * Comparison operators. Uses the identifier internally as that is guaranteed to be unique.
    */
    bool operator == (const PvUnit & o) const { return mID == o.mID; }
    bool operator != (const PvUnit & o) const { return mID != o.mID; }
    bool operator < (const PvUnit & o) const { return mID < o.mID; }
    bool operator > (const PvUnit & o) const { return mID > o.mID; }
    bool operator <= (const PvUnit & o) const { return mID <= o.mID; }
    bool operator >= (const PvUnit & o) const { return mID >= o.mID; }

    /* --------------------------------------------------------------------------------------------
     * Release all script resources. Recursively forward request.
    */
    void Release()
    {
        mOnQuery.Release();
        mOnGained.Release();
        mOnLost.Release();
        mTag.Release();
        mData.Release();
    }
};

/* ------------------------------------------------------------------------------------------------
 * An individual class/group that can optionally inherit the privileges of another and can
 * change their status to affect their children but not their parent class.
 * A class can extend it's privileges to units and/or other classes.
 * These cost less to query but use more memory because they're fewer and they preallocate it.
 * Each class must have a unique numerical identifier within their associated manager.
*/
struct PvClass
{
    /* --------------------------------------------------------------------------------------------
     * Strong and weak reference types.
    */
    typedef SharedPtr< PvClass > Ref;
    typedef WeakPtr< PvClass > Ptr;

    /* --------------------------------------------------------------------------------------------
     * Type of container used to store privilege classes.
    */
    typedef std::vector< std::pair< SQInteger, Ref > >   List;

    /* --------------------------------------------------------------------------------------------
     * User identifier associated with this class instance.
    */
    SQInteger           mID;

    /* --------------------------------------------------------------------------------------------
     * The parent class from which we are inheriting privileges, if any.
    */
    PvClass *           mParent;

    /* --------------------------------------------------------------------------------------------
     * A container with unique privilege status values associated with this class.
    */
    PvStatusList        mPrivileges;

    /* --------------------------------------------------------------------------------------------
     * Dedicated callback for privilege query event.
    */
    Function            mOnQuery;

    /* --------------------------------------------------------------------------------------------
     * Dedicated callback for privilege modify event.
    */
    Function            mOnModify;

    /* --------------------------------------------------------------------------------------------
     * Dedicated callback for privilege gained event.
    */
    Function            mOnGained;

    /* --------------------------------------------------------------------------------------------
     * Dedicated callback for privilege lost event.
    */
    Function            mOnLost;

    /* --------------------------------------------------------------------------------------------
     * Container that stores all the associated units.
    */
    PvUnit::List        mUnits;

    /* --------------------------------------------------------------------------------------------
     * User tag associated with this instance.
    */
    StackStrF           mTag;

    /* --------------------------------------------------------------------------------------------
     * User data associated with this instance.
    */
    LightObj            mData;

    /* --------------------------------------------------------------------------------------------
     * Pointer to the associated manager.
    */
    PvManager *         mManager;

    /* -------------------------------------------------------------------------------------------
     * Default constructor.
    */
    PvClass(SQInteger id, PvManager * mgr)
        : mID(id)
        , mParent(nullptr)
        , mPrivileges()
        , mOnQuery(), mOnGained(), mOnLost()
        , mUnits()
        , mTag(), mData()
        , mManager(mgr)
    {
    }

    /* -------------------------------------------------------------------------------------------
     * Default constructor.
    */
    PvClass(SQInteger id, StackStrF && tag, PvManager * mgr)
        : mID(id)
        , mParent(nullptr)
        , mPrivileges()
        , mOnQuery(), mOnGained(), mOnLost()
        , mUnits()
        , mTag(std::forward< StackStrF >(tag)), mData()
        , mManager(mgr)
    {
    }

    /* -------------------------------------------------------------------------------------------
     * Copy constructor (disabled).
    */
    PvClass(const PvClass & o) = delete;

    /* -------------------------------------------------------------------------------------------
     * Move constructor (disabled).
    */
    PvClass(PvClass && o) = delete;

    /* -------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~PvClass();

    /* -------------------------------------------------------------------------------------------
     * Copy assignment operator (disabled).
    */
    PvClass & operator = (const PvClass & o) = delete;

    /* -------------------------------------------------------------------------------------------
     * Move assignment operator (disabled).
    */
    PvClass & operator = (PvClass && o) = delete;

    /* -------------------------------------------------------------------------------------------
     * Comparison operators. Uses the identifier internally as that is guaranteed to be unique.
    */
    bool operator == (const PvClass & o) const { return mID == o.mID; }
    bool operator != (const PvClass & o) const { return mID != o.mID; }
    bool operator < (const PvClass & o) const { return mID < o.mID; }
    bool operator > (const PvClass & o) const { return mID > o.mID; }
    bool operator <= (const PvClass & o) const { return mID <= o.mID; }
    bool operator >= (const PvClass & o) const { return mID >= o.mID; }

    /* --------------------------------------------------------------------------------------------
     * Release all script resources. Recursively forward request.
    */
    void Release()
    {
        mOnQuery.Release();
        mOnGained.Release();
        mOnLost.Release();
        mTag.Release();
        mData.Release();
    }

};

/* ------------------------------------------------------------------------------------------------
 * An entry represents a privilege that a class or unit can can have.
 * Each entry must have a unique numerical identifier within their associated manager.
*/
struct PvEntry
{
    /* --------------------------------------------------------------------------------------------
     * Strong and weak reference types.
    */
    typedef SharedPtr< PvEntry > Ref;
    typedef WeakPtr< PvEntry > Ptr;

    /* --------------------------------------------------------------------------------------------
     * Type of container used to store privilege entries.
    */
    typedef std::vector< std::pair< SQInteger, Ref > >   List;

    /* --------------------------------------------------------------------------------------------
     * User identifier associated with this instance.
    */
    SQInteger           mID;

    /* --------------------------------------------------------------------------------------------
     * User tag associated with this instance.
    */
    StackStrF           mTag;

    /* --------------------------------------------------------------------------------------------
     * Dedicated callback for privilege query event.
    */
    Function            mOnQuery;

    /* --------------------------------------------------------------------------------------------
     * Dedicated callback for privilege gained event.
    */
    Function            mOnGained;

    /* --------------------------------------------------------------------------------------------
     * Dedicated callback for privilege lost event.
    */
    Function            mOnLost;

    /* --------------------------------------------------------------------------------------------
     * User data associated with this instance.
    */
    LightObj            mData;

    /* --------------------------------------------------------------------------------------------
     * Brief information about the privilege.
    */
    StackStrF           mBrief;

    /* --------------------------------------------------------------------------------------------
     * Detailed information about the privilege.
    */
    StackStrF           mInfo;

    /* --------------------------------------------------------------------------------------------
     * Implicit privilege status value.
    */
    SQInteger           mDefault;

    /* --------------------------------------------------------------------------------------------
     * Pointer to the associated manager.
    */
    PvManager *         mManager;

    /* -------------------------------------------------------------------------------------------
     * Default constructor.
    */
    PvEntry(SQInteger id, PvManager * mgr)
        : mID(id), mTag()
        , mOnQuery(), mOnGained(), mOnLost()
        , mData(), mBrief(), mInfo(), mDefault()
        , mManager(mgr)
    {
    }

    /* -------------------------------------------------------------------------------------------
     * Default constructor.
    */
    PvEntry(SQInteger id, StackStrF && tag, PvManager * mgr)
        : mID(id), mTag(std::forward< StackStrF >(tag))
        , mOnQuery(), mOnGained(), mOnLost()
        , mData(), mBrief(), mInfo(), mDefault()
        , mManager(mgr)

    {
    }

    /* -------------------------------------------------------------------------------------------
     * Copy constructor (disabled).
    */
    PvEntry(const PvEntry & o) = delete;

    /* -------------------------------------------------------------------------------------------
     * Move constructor (disabled).
    */
    PvEntry(PvEntry && o) = delete;

    /* -------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~PvEntry();

    /* -------------------------------------------------------------------------------------------
     * Copy assignment operator (disabled).
    */
    PvEntry & operator = (const PvEntry & o) = delete;

    /* -------------------------------------------------------------------------------------------
     * Move assignment operator (disabled).
    */
    PvEntry & operator = (PvEntry && o) = delete;

    /* -------------------------------------------------------------------------------------------
     * Comparison operators. Uses the identifier internally as that is guaranteed to be unique.
    */
    bool operator == (const PvEntry & o) const { return mID == o.mID; }
    bool operator != (const PvEntry & o) const { return mID != o.mID; }
    bool operator < (const PvEntry & o) const { return mID < o.mID; }
    bool operator > (const PvEntry & o) const { return mID > o.mID; }
    bool operator <= (const PvEntry & o) const { return mID <= o.mID; }
    bool operator >= (const PvEntry & o) const { return mID >= o.mID; }

    /* --------------------------------------------------------------------------------------------
     * Release all script resources. Recursively forward request.
    */
    void Release()
    {
        mTag.Release();
        mOnQuery.Release();
        mOnGained.Release();
        mOnLost.Release();
        mData.Release();
        mBrief.Release();
        mInfo.Release();
    }

};

/* ------------------------------------------------------------------------------------------------
 *
*/
class PvManager
{
private:

    /* --------------------------------------------------------------------------------------------
     * Container that stores privilege entries.
    */
    PvEntry::List   m_Entries;

    /* --------------------------------------------------------------------------------------------
     * Container that stores the managed classes.
    */
    PvClass::List   m_Classes;

    /* --------------------------------------------------------------------------------------------
     * Container that stores all the managed units.
    */
    PvUnit::List    m_Units;

    /* --------------------------------------------------------------------------------------------
     * Dedicated callback for privilege query event.
    */
    Function            m_OnQuery;

    /* --------------------------------------------------------------------------------------------
     * Dedicated callback for privilege gained event.
    */
    Function            m_OnGained;

    /* --------------------------------------------------------------------------------------------
     * Dedicated callback for privilege lost event.
    */
    Function            m_OnLost;

    /* --------------------------------------------------------------------------------------------
     * User tag associated with this instance.
    */
    StackStrF           m_Tag;

    /* --------------------------------------------------------------------------------------------
     * User data associated with this instance.
    */
    LightObj            m_Data;

    /* --------------------------------------------------------------------------------------------
     * List of active privilege managers.
    */
    static PvManagers   s_Managers;

public:

    /* -------------------------------------------------------------------------------------------
     * Default constructor.
    */
    PvManager()
        : m_Entries(), m_Classes(), m_Units()
        , m_OnQuery(), m_OnGained(), m_OnLost()
        , m_Tag(), m_Data()
    {
        s_Managers.push_back(this);
    }

    /* -------------------------------------------------------------------------------------------
     * Default constructor.
    */
    PvManager(StackStrF & tag)
        : m_Entries(), m_Classes(), m_Units()
        , m_OnQuery(), m_OnGained(), m_OnLost()
        , m_Tag(std::move(tag)), m_Data()
    {
        s_Managers.push_back(this);
    }

    /* -------------------------------------------------------------------------------------------
     * Copy constructor (disabled).
    */
    PvManager(const PvManager & o) = delete;

    /* -------------------------------------------------------------------------------------------
     * Move constructor (disabled).
    */
    PvManager(PvManager && o) = delete;

    /* -------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~PvManager()
    {
        s_Managers.erase(std::remove(s_Managers.begin(), s_Managers.end(), this), s_Managers.end());
    }

    /* -------------------------------------------------------------------------------------------
     * Copy assignment operator (disabled).
    */
    PvManager & operator = (const PvManager & o) = delete;

    /* -------------------------------------------------------------------------------------------
     * Move assignment operator (disabled).
    */
    PvManager & operator = (PvManager && o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to convert an instance of this type to a string.
    */
    auto ToString() const
    {
        return m_Tag.mObj;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated user tag wrapper.
    */
    const StackStrF & GetTagW() const
    {
        return m_Tag;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated user tag.
    */
    auto GetTag() const
    {
        return m_Tag.mObj;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the associated user tag.
    */
    void SetTag(StackStrF & tag)
    {
        m_Tag = std::move(tag);
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the associated user tag.
    */
    PvManager & ApplyTag(StackStrF & tag)
    {
        SetTag(tag);
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated user data.
    */
    LightObj & GetData()
    {
        return m_Data;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the associated user data.
    */
    void SetData(LightObj & data)
    {
        m_Data = data;
    }

    /* --------------------------------------------------------------------------------------------
     * Bind a script function to the status query callback.
    */
    void SetOnQuery(Function & func)
    {
        m_OnQuery = std::move(func);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the function bound to the status query callback.
    */
    Function & GetOnQuery()
    {
        return m_OnQuery;
    }

    /* --------------------------------------------------------------------------------------------
     * Bind a script function to the status lost callback.
    */
    void SetOnLost(Function & func)
    {
        m_OnLost = std::move(func);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the function bound to the status lost callback.
    */
    Function & GetOnLost()
    {
        return m_OnLost;
    }

    /* --------------------------------------------------------------------------------------------
     * Bind a script function to the status gained callback.
    */
    void SetOnGained(Function & func)
    {
        m_OnGained = std::move(func);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the function bound to the status gained callback.
    */
    Function & GetOnGained()
    {
        return m_OnGained;
    }

    /* --------------------------------------------------------------------------------------------
     * Release all script resources. Recursively forward request to all classes, units and entries.
    */
    void Release()
    {
        // Release objects from this instance
        m_OnQuery.Release();
        m_OnGained.Release();
        m_OnLost.Release();
        m_Tag.Release();
        m_Data.Release();
    }

    /* --------------------------------------------------------------------------------------------
     * Terminate the all managers by releasing their classes and units and callbacks.
    */
    static void Terminate()
    {
        for (auto & pvm : s_Managers)
        {
            pvm->Release();
        }
    }

};

} // Namespace:: SqMod
