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
class PvUnit;
class PvClass;
class PvEntry;
class PvManager;

// ------------------------------------------------------------------------------------------------
// Type of container used to store privilege entries.
typedef std::vector< std::pair< SQInteger, PvEntry * > >    PvEntryList;
// Type of container used to store privilege classes.
typedef std::vector< std::pair< SQInteger, PvClass * > >    PvClassList;
// Type of container used to store privilege units.
typedef std::vector< std::pair< SQInteger, PvUnit * > >     PvUnitList;

// ------------------------------------------------------------------------------------------------
typedef std::vector< std::pair< SQInteger, SQInteger > >    PvStatusList;
// ------------------------------------------------------------------------------------------------
typedef std::vector< PvManager * >                          PvManagers;

/* ------------------------------------------------------------------------------------------------
 * An individual unit/entity that inherits the privileges of a class/group and can
 * change their status without affecting other units and/or their associated class.
 * A unit cannot extend it's privileges to other entities.
 * Units cost more to query if they differ from their class but save memory in large numbers.
 * Each unit must have a unique numerical identifier within their associated manager.
*/
class PvUnit
{
    friend class PvManager;
    friend class PvClass;

private:

    /* --------------------------------------------------------------------------------------------
     * User identifier associated with this unit instance.
    */
    SQInteger       m_ID;

    /* --------------------------------------------------------------------------------------------
     * Authority level associated with a particular unit.
    */
    SQInteger       m_Authority;

    /* --------------------------------------------------------------------------------------------
     * A container with unique privilege status values associated with this unit.
    */
    PvStatusList    m_Privileges;

    /* --------------------------------------------------------------------------------------------
     * Dedicated callback for privilege query event.
    */
    Function        m_OnQuery;

    /* --------------------------------------------------------------------------------------------
     * Dedicated callback for privilege gained event.
    */
    Function        m_OnGained;

    /* --------------------------------------------------------------------------------------------
     * Dedicated callback for privilege lost event.
    */
    Function        m_OnLost;

    /* --------------------------------------------------------------------------------------------
     * User tag associated with this instance.
    */
    StackStrF       m_Tag;

    /* --------------------------------------------------------------------------------------------
     * User data associated with this instance.
    */
    LightObj        m_Data;

    /* --------------------------------------------------------------------------------------------
     * Pointer to the associated class.
    */
    PvClass *       m_Class;

public:

    /* -------------------------------------------------------------------------------------------
     * Default constructor.
    */
    PvUnit(SQInteger id, PvClass * cls)
        : m_ID(id)
        , m_Authority(0)
        , m_Privileges()
        , m_OnQuery(), m_OnGained(), m_OnLost()
        , m_Tag(), m_Data()
        , m_Class(cls)
    {
    }

    /* -------------------------------------------------------------------------------------------
     * Default constructor.
    */
    PvUnit(SQInteger id, StackStrF && tag, PvClass * cls)
        : m_ID(id)
        , m_Authority(0)
        , m_Privileges()
        , m_OnQuery(), m_OnGained(), m_OnLost()
        , m_Tag(std::forward< StackStrF >(tag)), m_Data()
        , m_Class(cls)
    {
    }

    /* -------------------------------------------------------------------------------------------
     * Copy constructor (disabled).
    */
    PvUnit(const PvUnit & o) = delete;

    /* -------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~PvUnit()
    {
    }

    /* -------------------------------------------------------------------------------------------
     * Copy assignment operator (disabled).
    */
    PvUnit & operator = (const PvUnit & o) = delete;

    /* -------------------------------------------------------------------------------------------
     * Comparison operators. Uses the identifier internally as that is guaranteed to be unique.
    */
    bool operator == (const PvUnit & o) const { return m_ID == o.m_ID; }
    bool operator != (const PvUnit & o) const { return m_ID != o.m_ID; }
    bool operator < (const PvUnit & o) const { return m_ID < o.m_ID; }
    bool operator > (const PvUnit & o) const { return m_ID > o.m_ID; }
    bool operator <= (const PvUnit & o) const { return m_ID <= o.m_ID; }
    bool operator >= (const PvUnit & o) const { return m_ID >= o.m_ID; }

    /* --------------------------------------------------------------------------------------------
     * Release all script resources. Recursively forward request.
    */
    void Release()
    {
        m_OnQuery.Release();
        m_OnGained.Release();
        m_OnLost.Release();
        m_Tag.Release();
        m_Data.Release();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the identifier associated with this unit.
    */
    SQInteger GetID() const
    {
        return m_ID;
    }

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
    PvUnit & ApplyTag(StackStrF & tag)
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
     * Retrieve the authority level associated with this unit.
    */
    SQInteger GetAuthority() const
    {
        return m_Authority;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the authority level associated with this unit.
    */
    void SetAuthority(SQInteger level)
    {
        m_Authority = level;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the manager associated with this unit.
    */
    PvManager & GetManager() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the class associated with this unit.
    */
    PvClass & GetClass() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the class associated with this unit.
    */
    void SetClass(PvClass & cls);

    /* --------------------------------------------------------------------------------------------
     * Query if a unit has a certain privilege over itself.
    */
    bool Can(const PvEntry & entry) const;

    /* --------------------------------------------------------------------------------------------
     * Query if a unit has a certain privilege over another unit.
    */
    bool Can(const PvEntry & entry, const PvUnit & unit) const;

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
};

/* ------------------------------------------------------------------------------------------------
 * An individual class/group that can optionally inherit the privileges of another and can
 * change their status to affect their children but not their parent class.
 * A class can extend it's privileges to units and/or other classes.
 * These cost less to query but use more memory because they're fewer and they preallocate it.
 * Each class must have a unique numerical identifier within their associated manager.
*/
class PvClass
{
    friend class PvManager;

private:

    /* --------------------------------------------------------------------------------------------
     * User identifier associated with this class instance.
    */
    SQInteger       m_ID;

    /* --------------------------------------------------------------------------------------------
     * The parent class from which we are inheriting privileges, if any.
    */
    PvClass *     	m_Parent;

    /* --------------------------------------------------------------------------------------------
     * A container with unique privilege status values associated with this class.
    */
    PvStatusList    m_Privileges;

    /* --------------------------------------------------------------------------------------------
     * Dedicated callback for privilege query event.
    */
    Function        m_OnQuery;

    /* --------------------------------------------------------------------------------------------
     * Dedicated callback for privilege gained event.
    */
    Function        m_OnGained;

    /* --------------------------------------------------------------------------------------------
     * Dedicated callback for privilege lost event.
    */
    Function        m_OnLost;

    /* --------------------------------------------------------------------------------------------
     * Container that stores all the associated units.
    */
    PvUnitList      m_Units;

    /* --------------------------------------------------------------------------------------------
     * User tag associated with this instance.
    */
    StackStrF       m_Tag;

    /* --------------------------------------------------------------------------------------------
     * User data associated with this instance.
    */
    LightObj        m_Data;

    /* --------------------------------------------------------------------------------------------
     * Pointer to the associated manager.
    */
    PvManager *     m_Manager;

public:

    /* -------------------------------------------------------------------------------------------
     * Default constructor.
    */
    PvClass(SQInteger id, PvManager * mgr)
        : m_ID(id)
        , m_Parent(nullptr)
        , m_Privileges()
        , m_OnQuery(), m_OnGained(), m_OnLost()
        , m_Units()
        , m_Tag(), m_Data()
        , m_Manager(mgr)
    {
    }

    /* -------------------------------------------------------------------------------------------
     * Default constructor.
    */
    PvClass(SQInteger id, StackStrF && tag, PvManager * mgr)
        : m_ID(id)
        , m_Parent(nullptr)
        , m_Privileges()
        , m_OnQuery(), m_OnGained(), m_OnLost()
        , m_Units()
        , m_Tag(std::forward< StackStrF >(tag)), m_Data()
        , m_Manager(mgr)
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
    ~PvClass()
    {
    }

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
    bool operator == (const PvClass & o) const { return m_ID == o.m_ID; }
    bool operator != (const PvClass & o) const { return m_ID != o.m_ID; }
    bool operator < (const PvClass & o) const { return m_ID < o.m_ID; }
    bool operator > (const PvClass & o) const { return m_ID > o.m_ID; }
    bool operator <= (const PvClass & o) const { return m_ID <= o.m_ID; }
    bool operator >= (const PvClass & o) const { return m_ID >= o.m_ID; }

    /* --------------------------------------------------------------------------------------------
     * Release all script resources. Recursively forward request.
    */
    void Release()
    {
        m_OnQuery.Release();
        m_OnGained.Release();
        m_OnLost.Release();
        m_Tag.Release();
        m_Data.Release();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the identifier associated with this manager.
    */
    SQInteger GetID() const
    {
        return m_ID;
    }

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
    PvClass & ApplyTag(StackStrF & tag)
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
     * Retrieve the manager associated with this unit.
    */
    LightObj GetParent() const
    {
        return m_Parent ? LightObj(m_Parent) : LightObj{};
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the manager associated with this unit.
    */
    void SetParent(LightObj & obj)
    {
    	m_Parent = obj.IsNull() ? nullptr : obj.Cast< PvClass * >();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the manager associated with this unit.
    */
    PvManager & GetManager() const
    {
        return *m_Manager; // Classes must always have a manager!
    }

    /* --------------------------------------------------------------------------------------------
     * Find the instance of the unit associated with the given identifier.
    */
    PvUnit * GetUnitByID(SQInteger id) const;

    /* --------------------------------------------------------------------------------------------
     * Check for existence of an unit associated with the given identifier.
    */
    bool HaveUnitWithID(SQInteger id) const
    {
        return GetUnitByID(id) != nullptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Find the instance of the unit associated with the given name tag.
    */
    PvUnit * GetUnitByTag(StackStrF & name) const;

    /* --------------------------------------------------------------------------------------------
     * Check for existence of an unit associated with the given name tag.
    */
    bool HaveUnitWithTag(StackStrF & name) const
    {
        return GetUnitByTag(name) != nullptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Create a new privilege unit instance and manage it.
    */
    LightObj CreateUnit(SQInteger id, StackStrF & name);

    /* --------------------------------------------------------------------------------------------
     * Add a unit to this privilege class.
    */
    void AddUnit(PvUnit & unit);

    /* --------------------------------------------------------------------------------------------
     * Add a unit to this class container (internal, assumes validation beforehand!).
    */
    bool EnlistUnit(PvUnit & unit);

    /* --------------------------------------------------------------------------------------------
     * Remove a unit from this class container (internal, assumes validation beforehand!).
    */
    bool UnlistUnit(PvUnit & unit);

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
};

/* ------------------------------------------------------------------------------------------------
 * An entry represents a privilege that a class or unit can can have.
 * Each entry must have a unique numerical identifier within their associated manager.
*/
class PvEntry
{
    friend class PvManager;

private:

    /* --------------------------------------------------------------------------------------------
     * User identifier associated with this instance.
    */
    SQInteger       m_ID;

    /* --------------------------------------------------------------------------------------------
     * User tag associated with this instance.
    */
    StackStrF       m_Tag;

    /* --------------------------------------------------------------------------------------------
     * Dedicated callback for privilege query event.
    */
    Function        m_OnQuery;

    /* --------------------------------------------------------------------------------------------
     * Dedicated callback for privilege gained event.
    */
    Function        m_OnGained;

    /* --------------------------------------------------------------------------------------------
     * Dedicated callback for privilege lost event.
    */
    Function        m_OnLost;

    /* --------------------------------------------------------------------------------------------
     * User data associated with this instance.
    */
    LightObj        m_Data;

    /* --------------------------------------------------------------------------------------------
     * Brief information about the privilege.
    */
    StackStrF       m_Brief;

    /* --------------------------------------------------------------------------------------------
     * Detailed information about the privilege.
    */
    StackStrF       m_Info;

    /* --------------------------------------------------------------------------------------------
     * Implicit privilege status value.
    */
    SQInteger       m_Default;

    /* --------------------------------------------------------------------------------------------
     * Pointer to the associated manager.
    */
    PvManager *     m_Manager;

public:

    /* -------------------------------------------------------------------------------------------
     * Default constructor.
    */
    PvEntry(SQInteger id, PvManager * mgr)
        : m_ID(id), m_Tag()
        , m_OnQuery(), m_OnGained(), m_OnLost()
        , m_Data(), m_Brief(), m_Info(), m_Default()
        , m_Manager(mgr)
    {
    }

    /* -------------------------------------------------------------------------------------------
     * Default constructor.
    */
    PvEntry(SQInteger id, StackStrF && tag, PvManager * mgr)
        : m_ID(id), m_Tag(std::forward< StackStrF >(tag))
        , m_OnQuery(), m_OnGained(), m_OnLost()
        , m_Data(), m_Brief(), m_Info(), m_Default()
        , m_Manager(mgr)

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
    ~PvEntry()
    {
    }

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
    bool operator == (const PvEntry & o) const { return m_ID == o.m_ID; }
    bool operator != (const PvEntry & o) const { return m_ID != o.m_ID; }
    bool operator < (const PvEntry & o) const { return m_ID < o.m_ID; }
    bool operator > (const PvEntry & o) const { return m_ID > o.m_ID; }
    bool operator <= (const PvEntry & o) const { return m_ID <= o.m_ID; }
    bool operator >= (const PvEntry & o) const { return m_ID >= o.m_ID; }

    /* --------------------------------------------------------------------------------------------
     * Release all script resources. Recursively forward request.
    */
    void Release()
    {
        m_Tag.Release();
        m_OnQuery.Release();
        m_OnGained.Release();
        m_OnLost.Release();
        m_Data.Release();
        m_Brief.Release();
        m_Info.Release();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the manager associated with this unit.
    */
    PvManager & GetManager() const
    {
        return *m_Manager; // Entries must always have a manager!
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the identifier associated with this manager.
    */
    SQInteger GetID() const
    {
        return m_ID;
    }

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
    PvEntry & ApplyTag(StackStrF & tag)
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
     * Retrieve the associated user brief.
    */
    auto GetBrief() const
    {
        return m_Brief.mObj;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the associated user brief.
    */
    void SetBrief(StackStrF & brief)
    {
        m_Brief = std::move(brief);
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the associated user brief.
    */
    PvEntry & ApplyBrief(StackStrF & brief)
    {
        SetBrief(brief);
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated user info.
    */
    auto GetInfo() const
    {
        return m_Info.mObj;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the associated user info.
    */
    void SetInfo(StackStrF & info)
    {
        m_Info = std::move(info);
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the associated user info.
    */
    PvEntry & ApplyInfo(StackStrF & info)
    {
        SetInfo(info);
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the implicit status value of this entry.
    */
    SQInteger GetDefault() const
    {
        return m_Default;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the implicit status value of this entry.
    */
    void SetDefault(SQInteger status)
    {
        m_Default = status;
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
    PvEntryList     m_Entries;

    /* --------------------------------------------------------------------------------------------
     * Container that stores the managed classes.
    */
    PvClassList     m_Classes;

    /* --------------------------------------------------------------------------------------------
     * Container that stores all the managed units.
    */
    PvUnitList      m_Units;

    /* --------------------------------------------------------------------------------------------
     * Dedicated callback for privilege query event.
    */
    Function        m_OnQuery;

    /* --------------------------------------------------------------------------------------------
     * Dedicated callback for privilege gained event.
    */
    Function        m_OnGained;

    /* --------------------------------------------------------------------------------------------
     * Dedicated callback for privilege lost event.
    */
    Function        m_OnLost;

    /* --------------------------------------------------------------------------------------------
     * User tag associated with this instance.
    */
    StackStrF       m_Tag;

    /* --------------------------------------------------------------------------------------------
     * User data associated with this instance.
    */
    LightObj        m_Data;

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
     * Find the instance of the entry associated with the given identifier.
    */
    PvEntry * GetEntryByID(SQInteger id) const;

    /* --------------------------------------------------------------------------------------------
     * Check for existence of an entry associated with the given identifier.
    */
    bool HaveEntryWithID(SQInteger id) const
    {
        return GetEntryByID(id) != nullptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Find the instance of the entry associated with the given name tag.
    */
    PvEntry * GetEntryByTag(StackStrF & name) const;

    /* --------------------------------------------------------------------------------------------
     * Check for existence of an entry associated with the given name tag.
    */
    bool HaveEntryWithTag(StackStrF & name) const
    {
        return GetEntryByTag(name) != nullptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Create a new privilege entry instance and manage it.
    */
    LightObj CreateEntry(SQInteger id, StackStrF & name);

    /* --------------------------------------------------------------------------------------------
     * Find the instance of the class associated with the given identifier.
    */
    PvClass * GetClassByID(SQInteger id) const;

    /* --------------------------------------------------------------------------------------------
     * Check for existence of an class associated with the given identifier.
    */
    bool HaveClassWithID(SQInteger id) const
    {
        return GetClassByID(id) != nullptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Find the instance of the class associated with the given name tag.
    */
    PvClass * GetClassByTag(StackStrF & name) const;

    /* --------------------------------------------------------------------------------------------
     * Check for existence of an class associated with the given name tag.
    */
    bool HaveClassWithTag(StackStrF & name) const
    {
        return GetClassByTag(name) != nullptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Create a new privilege class instance and manage it.
    */
    LightObj CreateClass(SQInteger id, StackStrF & name);

    /* --------------------------------------------------------------------------------------------
     * Find the instance of the unit associated with the given identifier.
    */
    PvUnit * GetUnitByID(SQInteger id) const;

    /* --------------------------------------------------------------------------------------------
     * Check for existence of an unit associated with the given identifier.
    */
    bool HaveUnitWithID(SQInteger id) const
    {
        return GetUnitByID(id) != nullptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Find the instance of the unit associated with the given name tag.
    */
    PvUnit * GetUnitByTag(StackStrF & name) const;

    /* --------------------------------------------------------------------------------------------
     * Check for existence of an unit associated with the given name tag.
    */
    bool HaveUnitWithTag(StackStrF & name) const
    {
        return GetUnitByTag(name) != nullptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Create a new privilege unit instance and manage it.
    */
    LightObj CreateUnit(SQInteger id, PvClass & cls, StackStrF & name);

    /* --------------------------------------------------------------------------------------------
     * Create a new privilege unit instance and manage it.
    */
    std::pair< PvUnit *, LightObj > CreateUnitImpl(SQInteger id, PvClass & cls, StackStrF & name);

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
        // Release objects from entries
        for (auto & pve : m_Entries)
        {
            pve.second->Release();
        }
        // Release objects from classes
        for (auto & pvc : m_Classes)
        {
            pvc.second->Release();
        }
        // Release objects from units
        for (auto & pvu : m_Units)
        {
            pvu.second->Release();
        }
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
