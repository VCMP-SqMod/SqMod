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
#include "Core/Privilege/Class.hpp"
#include "Core/Privilege/Entry.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Privilege manager responsible for keeping track of each resource and handling requests.
*/
struct PvManager : SqChainedInstances< PvManager >
{
    /* --------------------------------------------------------------------------------------------
     * Container that stores privilege entries.
    */
    PvEntry::List		m_Entries;

    /* --------------------------------------------------------------------------------------------
     * Container that stores the managed classes.
    */
    PvClass::List   	m_Classes;

    /* --------------------------------------------------------------------------------------------
     * Container that stores all the managed units.
    */
    PvUnit::List    	m_Units;

    /* --------------------------------------------------------------------------------------------
     * Dedicated callback for privilege query event.
    */
    Function            m_OnQuery;

    /* --------------------------------------------------------------------------------------------
     * Dedicated callback for privilege modify event.
    */
    Function            m_OnModify;

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
     * Used to prevent modification of entries, classes or units.
    */
    bool                m_LockEntries;
    bool                m_LockClasses;
    bool                m_LockUnits;

public:

    /* -------------------------------------------------------------------------------------------
     * Default constructor.
    */
    PvManager()
        : m_Entries(), m_Classes(), m_Units()
        , m_OnQuery(),  m_OnModify(),  m_OnGained(), m_OnLost()
        , m_Tag(), m_Data()
        , m_LockEntries(false), m_LockClasses(false), m_LockUnits(false)
    {
        // Remember this instance
        ChainInstance();
    }

    /* -------------------------------------------------------------------------------------------
     * Default constructor.
    */
    explicit PvManager(StackStrF & tag)
        : m_Entries(), m_Classes(), m_Units()
        , m_OnQuery(), m_OnModify(), m_OnGained(), m_OnLost()
        , m_Tag(std::move(tag)), m_Data()
        , m_LockEntries(false), m_LockClasses(false), m_LockUnits(false)
    {
        // Remember this instance
        ChainInstance();
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
        // Forget about this instance
        UnchainInstance();
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
    SQMOD_NODISCARD auto ToString() const
    {
        return m_Tag.mObj;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated user tag.
    */
    SQMOD_NODISCARD auto GetTag() const
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
    SQMOD_NODISCARD PvManager & ApplyTag(StackStrF & tag)
    {
        SetTag(tag);
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated user data.
    */
    SQMOD_NODISCARD LightObj & GetData()
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
     * Release all script resources. Recursively forward request to all classes, units and entries.
    */
    void Release();

    /* --------------------------------------------------------------------------------------------
     * Terminate the all managers by releasing their classes and units and callbacks.
    */
    void Terminate();

    /* --------------------------------------------------------------------------------------------
     * Makes sure you can modify current entries.
    */
    void ModifyEntries()
    {
        if (m_LockEntries)
        {
            STHROWF("Entries cannot be modified");
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Makes sure you can modify current entries.
    */
    void ModifyClasses()
    {
        if (m_LockClasses)
        {
            STHROWF("Classes cannot be modified");
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Makes sure you can modify current entries.
    */
    void ModifyUnits()
    {
        if (m_LockUnits)
        {
            STHROWF("Units cannot be modified");
        }
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
    SQMOD_NODISCARD const Function & GetOnQuery() const
    {
        return m_OnQuery;
    }

    /* --------------------------------------------------------------------------------------------
     * Find out the callback that must be invoked to handle query events for a certain entry. 
    */
    SQMOD_NODISCARD const Function & GetOnQuery(SQInteger id) const
    {
        // Look for a the specified entry
        auto itr = m_Entries.find(PvIdentity(id));
        // Does the specified entry exist?
        if (itr != m_Entries.end())
        {
            return itr->second->mOnQuery;
        }
        // Default to the global one
        return m_OnQuery;
    }

    /* --------------------------------------------------------------------------------------------
     * Bind a script function to the status modify callback.
    */
    void SetOnModify(Function & func)
    {
        m_OnModify = std::move(func);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the function bound to the status modify callback.
    */
    SQMOD_NODISCARD const Function & GetOnModify() const
    {
        return m_OnModify;
    }

    /* --------------------------------------------------------------------------------------------
     * Find out the callback that must be invoked to handle modify events for a certain entry. 
    */
    SQMOD_NODISCARD const Function & GetOnModify(SQInteger id) const
    {
        // Look for a the specified entry
        auto itr = m_Entries.find(PvIdentity(id));
        // Does the specified entry exist?
        if (itr != m_Entries.end())
        {
            return itr->second->mOnModify;
        }
        // Default to the global one
        return m_OnModify;
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
    SQMOD_NODISCARD const Function & GetOnGained() const
    {
        return m_OnGained;
    }

    /* --------------------------------------------------------------------------------------------
     * Find out the callback that must be invoked to handle gained events for a certain entry. 
    */
    SQMOD_NODISCARD const Function & GetOnGained(SQInteger id) const
    {
        // Look for a the specified entry
        auto itr = m_Entries.find(PvIdentity(id));
        // Does the specified entry exist?
        if (itr != m_Entries.end())
        {
            return itr->second->mOnGained;
        }
        // Default to the global one
        return m_OnGained;
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
    SQMOD_NODISCARD Function & GetOnLost()
    {
        return m_OnLost;
    }

    /* --------------------------------------------------------------------------------------------
     * Find out the callback that must be invoked to handle lost events for a certain entry. 
    */
    SQMOD_NODISCARD Function & GetOnLost(SQInteger id)
    {
        // Look for a the specified entry
        auto itr = m_Entries.find(PvIdentity(id));
        // Does the specified entry exist?
        if (itr != m_Entries.end())
        {
            return itr->second->mOnLost;
        }
        // Default to the global one
        return m_OnLost;
    }

    /* --------------------------------------------------------------------------------------------
     * Make sure a specific entry exists and return a smart reference to it.
    */
    SQMOD_NODISCARD const PvEntry::Ref & GetValidEntry(SQInteger id)
    {
        // Look for a the specified entry
        auto itr = m_Entries.find(PvIdentity(id));
        // Does this entry exist?
        if (itr == m_Entries.end())
        {
            STHROWF("Entry ({}) doesn't exists", id);
        }
        // Return the reference
        return itr->second;
    }

    /* --------------------------------------------------------------------------------------------
     * Make sure a specific class exists and return a smart reference to it.
    */
    SQMOD_NODISCARD const PvClass::Ref & GetValidClass(SQInteger id)
    {
        // Look for a the specified class
        auto itr = m_Classes.find(PvIdentity(id));
        // Does this class exist?
        if (itr == m_Classes.end())
        {
            STHROWF("Class ({}) doesn't exists", id);
        }
        // Return the reference
        return itr->second;
    }

    /* --------------------------------------------------------------------------------------------
     * Make sure a specific unit exists and return a smart reference to it.
    */
    SQMOD_NODISCARD const PvUnit::Ref & GetValidUnit(SQInteger id)
    {
        // Look for a the specified unit
        auto itr = m_Units.find(PvIdentity(id));
        // Does this unit exist?
        if (itr == m_Units.end())
        {
            STHROWF("Unit ({}) doesn't exists", id);
        }
        // Return the reference
        return itr->second;
    }

    /* --------------------------------------------------------------------------------------------
     * Make sure a specific entry exists and return a reference to it.
    */
    SQMOD_NODISCARD PvEntry & ValidEntry(SQInteger id) { return *GetValidEntry(id); }

    /* --------------------------------------------------------------------------------------------
     * Make sure a specific class exists and return a reference to it.
    */
    SQMOD_NODISCARD PvClass & ValidClass(SQInteger id) { return *GetValidClass(id); }

    /* --------------------------------------------------------------------------------------------
     * Make sure a specific unit exists and return a reference to it.
    */
    SQMOD_NODISCARD PvUnit & ValidUnit(SQInteger id) { return *GetValidUnit(id); }

    /* --------------------------------------------------------------------------------------------
     * Make sure a specific entry exists and return a smart reference to it.
    */
    SQMOD_NODISCARD const PvEntry::Ref & GetValidEntryWithTag(StackStrF & tag)
    {
        const size_t h = tag.GetHash();
        for (const auto & e : m_Entries)
        {
            if (e.first.mHash == h)
            {
                return e.second;
            }
        }
        // Not found
        STHROWF("Entry ({}) doesn't exists", tag.mPtr);
        SQ_UNREACHABLE
    }

    /* --------------------------------------------------------------------------------------------
     * Make sure a specific class exists and return a smart reference to it.
    */
    SQMOD_NODISCARD const PvClass::Ref & GetValidClassWithTag(StackStrF & tag)
    {
        const size_t h = tag.GetHash();
        for (const auto & c : m_Classes)
        {
            if (c.first.mHash == h)
            {
                return c.second;
            }
        }
        // Not found
        STHROWF("Class ({}) doesn't exists", tag.mPtr);
        SQ_UNREACHABLE
    }

    /* --------------------------------------------------------------------------------------------
     * Make sure a specific unit exists and return a smart reference to it.
    */
    SQMOD_NODISCARD const PvUnit::Ref & GetValidUnitWithTag(StackStrF & tag)
    {
        const size_t h = tag.GetHash();
        for (const auto & u : m_Units)
        {
            if (u.first.mHash == h)
            {
                return u.second;
            }
        }
        // Not found
        STHROWF("Unit ({}) doesn't exists", tag.mPtr);
        SQ_UNREACHABLE
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the default value of an entry.
    */
    SQMOD_NODISCARD SQInteger GetEntryValue(SQInteger id) const
    {
        // Look for a the specified entry
        auto itr = m_Entries.find(PvIdentity(id));
        // Does this entry exist?
        if (itr == m_Entries.end())
        {
            STHROWF("Entry ({}) doesn't exists", id);
        }
        // Return the associated callback
        return itr->second->mDefault;
    }

    /* --------------------------------------------------------------------------------------------
     * Update the hash of the specified entry.
    */
    void UpdateEntryHash(SQInteger id, size_t hash)
    {
        auto itr = m_Entries.find(PvIdentity(id));
        // Only update if it exists
        if (itr != m_Entries.end())
        {
            itr->first.mHash = hash;
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Update the hash of the specified class.
    */
    void UpdateClassHash(SQInteger id, size_t hash)
    {
        auto itr = m_Classes.find(PvIdentity(id));
        // Only update if it exists
        if (itr != m_Classes.end())
        {
            itr->first.mHash = hash;
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Update the hash of the specified unit.
    */
    void UpdateUnitHash(SQInteger id, size_t hash)
    {
        auto itr = m_Units.find(PvIdentity(id));
        // Only update if it exists
        if (itr != m_Units.end())
        {
            itr->first.mHash = hash;
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Create a entry unit. It throws an error if it already exists.
    */
    LightObj CreateEntry(SQInteger id, StackStrF & name);

    /* --------------------------------------------------------------------------------------------
     * Create a class unit. It throws an error if it already exists.
    */
    LightObj CreateClass(SQInteger id, StackStrF & name);

    /* --------------------------------------------------------------------------------------------
     * Create a unit unit. It throws an error if it already exists.
    */
    LightObj CreateUnit(SQInteger id, const SqPvClass & cls, StackStrF & name);

    /* --------------------------------------------------------------------------------------------
     * Performs the necessary checks to see if privileges were gained or lost when changing classes.
    */
    void PropagateParentAssign(const PvClass & cls, const PvClass::Ref & parent);
    void PropagateParentAssign(const PvClass::Ref & cls, const PvClass::Ref & parent) { PropagateParentAssign(*cls, parent); }

    /* --------------------------------------------------------------------------------------------
     * Performs the necessary checks to see if privileges were gained or lost when changing classes.
    */
    void PropagateParentChange(const PvClass & cls, const PvClass::Ref & parent);
    void PropagateParentChange(const PvClass::Ref & cls, const PvClass::Ref & parent) { PropagateParentChange(*cls, parent); }

    /* --------------------------------------------------------------------------------------------
     * Performs the necessary checks to see if privileges were gained or lost when changing classes.
    */
    void PropagateClassChange(const PvUnit & unit, const PvClass::Ref & cls);
    void PropagateClassChange(const PvUnit::Ref & unit, const PvClass::Ref & cls) { PropagateClassChange(*unit, cls); }

    /* --------------------------------------------------------------------------------------------
     * Retrieve a entry with a certain identifier from this manager.
    */
    SQMOD_NODISCARD LightObj GetEntryWithID(SQInteger id);

    /* --------------------------------------------------------------------------------------------
     * Retrieve a entry with a certain tag from this manager.
    */
    SQMOD_NODISCARD LightObj GetEntryWithTag(StackStrF & tag);

    /* --------------------------------------------------------------------------------------------
     * See if a entry with a certain identifier exists in this manager.
    */
    bool HaveEntryWithID(SQInteger id);

    /* --------------------------------------------------------------------------------------------
     * See if a entry with a certain tag exists in this manager.
    */
    bool HaveEntryWithTag(StackStrF & tag);

    /* --------------------------------------------------------------------------------------------
     * Retrieve a class with a certain identifier from this manager.
    */
    SQMOD_NODISCARD LightObj GetClassWithID(SQInteger id);

    /* --------------------------------------------------------------------------------------------
     * Retrieve a class with a certain tag from this manager.
    */
    SQMOD_NODISCARD LightObj GetClassWithTag(StackStrF & tag);

    /* --------------------------------------------------------------------------------------------
     * See if a class with a certain identifier exists in this manager.
    */
    bool HaveClassWithID(SQInteger id);

    /* --------------------------------------------------------------------------------------------
     * See if a class with a certain tag exists in this manager.
    */
    bool HaveClassWithTag(StackStrF & tag);

    /* --------------------------------------------------------------------------------------------
     * Retrieve a unit with a certain identifier from this manager.
    */
    SQMOD_NODISCARD LightObj GetUnitWithID(SQInteger id);

    /* --------------------------------------------------------------------------------------------
     * Retrieve a unit with a certain tag from this manager.
    */
    SQMOD_NODISCARD LightObj GetUnitWithTag(StackStrF & tag);

    /* --------------------------------------------------------------------------------------------
     * See if a unit with a certain identifier exists in this manager.
    */
    bool HaveUnitWithID(SQInteger id);

    /* --------------------------------------------------------------------------------------------
     * See if a unit with a certain tag exists in this manager.
    */
    bool HaveUnitWithTag(StackStrF & tag);

    /* --------------------------------------------------------------------------------------------
     * Remove a entry with a certain identifier from this manager.
    */
    void RemoveEntryWithID(SQInteger id);

    /* --------------------------------------------------------------------------------------------
     * Remove a entry with a certain tag from this manager.
    */
    void RemoveEntryWithTag(StackStrF & tag);

    /* --------------------------------------------------------------------------------------------
     * Remove a class with a certain identifier from this manager.
    */
    void RemoveClassWithID(SqPvClass & sub, SQInteger id);

    /* --------------------------------------------------------------------------------------------
     * Remove a class with a certain tag from this manager.
    */
    void RemoveClassWithTag(SqPvClass & sub, StackStrF & tag);

    /* --------------------------------------------------------------------------------------------
     * Remove a unit with a certain identifier from this manager.
    */
    void RemoveUnitWithID(SQInteger id);

    /* --------------------------------------------------------------------------------------------
     * Remove a unit with a certain tag from this manager.
    */
    void RemoveUnitWithTag(StackStrF & tag);
};

} // Namespace:: SqMod
