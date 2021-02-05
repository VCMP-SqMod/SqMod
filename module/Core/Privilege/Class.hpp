#pragma once

// ------------------------------------------------------------------------------------------------
#include "Core/Privilege/Unit.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

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
    typedef std::shared_ptr< PvClass > Ref;
    typedef std::weak_ptr< PvClass > Ptr;

    /* --------------------------------------------------------------------------------------------
     * Type of container used to store privilege classes.
    */
    typedef VecMap< PvIdentity, Ref, PvIdPred > List;

    /* --------------------------------------------------------------------------------------------
     * User identifier associated with this class instance. Always unique in the same manager!
    */
    const SQInteger     mID;

    /* --------------------------------------------------------------------------------------------
     * The parent class from which we are inheriting privileges, if any.
    */
    Ptr                 mParent;

    /* --------------------------------------------------------------------------------------------
     * A container with unique privilege status values associated with this class.
    */
    PvStatusList        mPrivileges;

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
     * Pointer to the associated manager. Should always be present.
     * 
    */
    PvManager *         mManager;

    /* -------------------------------------------------------------------------------------------
     * Default constructor.
    */
    PvClass(SQInteger id, PvManager * mgr)
        : mID(id)
        , mParent()
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
        , mParent()
        , mPrivileges()
        , mOnQuery(), mOnGained(), mOnLost()
        , mUnits()
        , mTag(std::move(tag)), mData()
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
    ~PvClass() = default;

    /* -------------------------------------------------------------------------------------------
     * Copy assignment operator (disabled).
    */
    PvClass & operator = (const PvClass & o) = delete;

    /* -------------------------------------------------------------------------------------------
     * Move assignment operator (disabled).
    */
    PvClass & operator = (PvClass && o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Update the hash of the specified unit.
    */
    void UpdateUnitHash(SQInteger id, size_t hash)
    {
        auto itr = mUnits.find(PvIdentity(id));
        // Only update if it exists
        if (itr != mUnits.end())
        {
            itr->first.mHash = hash;
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the associated user tag.
    */
    void SetTag(StackStrF & tag);

    /* --------------------------------------------------------------------------------------------
     * Release all script resources. Recursively forward request.
    */
    void Release();

    /* --------------------------------------------------------------------------------------------
     * Make sure the referenced parent class is valid.
    */
    void ValidateParent() const
    {
        if (mParent.expired())
        {
            STHROWF("Class ({} : {}) has invalid parent class reference", mID, mTag.mPtr);
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Make sure the referenced parent class is valid and return a reference to it.
    */
    SQMOD_NODISCARD PvClass & ValidParent() const
    {
        ValidateParent();
        // Acquire a reference and return it
        return *(mParent.lock().get());
    }

    /* --------------------------------------------------------------------------------------------
     * Make sure the referenced manager is valid.
    */
    void ValidateManager() const
    {
        if (!mManager)
        {
            STHROWF("Class ({} : {}) has invalid manager reference", mID, mTag.mPtr);
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Make sure the referenced manger is valid and return a reference to it.
    */
    SQMOD_NODISCARD PvManager & ValidManager() const;

    /* --------------------------------------------------------------------------------------------
     * Find out the callback that must be invoked to handle query events for a certain entry. 
    */
    SQMOD_NODISCARD const Function & GetOnQuery(SQInteger id) const;

    /* --------------------------------------------------------------------------------------------
     * Find out the callback that must be invoked to handle gained events for a certain entry. 
    */
    SQMOD_NODISCARD const Function & GetOnGained(SQInteger id) const;

    /* --------------------------------------------------------------------------------------------
     * Find out the callback that must be invoked to handle lost events for a certain entry. 
    */
    SQMOD_NODISCARD const Function & GetOnLost(SQInteger id) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the current value of an entry for this particular class.
    */
    SQMOD_NODISCARD SQInteger GetEntryValue(SQInteger id) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the inherited value of an entry for this particular class.
    */
    SQMOD_NODISCARD SQInteger GetInheritedEntryValue(SQInteger id) const;

    /* --------------------------------------------------------------------------------------------
     * Perform the actions necessary to handle a privilege gain event.
    */
    void DoGained(SQInteger id, SQInteger value) const;

    /* --------------------------------------------------------------------------------------------
     * Perform the actions necessary to handle a privilege lost event.
    */
    void DoLost(SQInteger id, SQInteger value) const;

    /* --------------------------------------------------------------------------------------------
     * Perform the actions necessary to handle a privilege change event.
    */
    void DoChanged(SQInteger id, bool status, SQInteger value) const;

    /* --------------------------------------------------------------------------------------------
     * Assign a status value. Does not care if a parent (class or global) has the same status.
     * Later if the parent changes this status, we will keep having this status value.
    */
    void AssignPrivilege(SQInteger id, SQInteger value);

    /* --------------------------------------------------------------------------------------------
     * Remove a status value. If the specified status value is not assigned, nothing happens.
    */
    void RemovePrivilege(SQInteger id);

    /* --------------------------------------------------------------------------------------------
     * Assign a status value. If a parent (class or global) has the same value, nothing changes.
     * Same as AssignStatus but the status will not be enforced if we have it (inherited or not).
    */
    void ModifyPrivilege(SQInteger id, SQInteger value);

    /* --------------------------------------------------------------------------------------------
     * See AssignPrivilege().
    */
    void AssignPrivilege(StackStrF & tag, SQInteger value);

    /* --------------------------------------------------------------------------------------------
     * See RemovePrivilege().
    */
    void RemovePrivilege(StackStrF & tag);

    /* --------------------------------------------------------------------------------------------
     * See ModifyPrivilege().
    */
    void ModifyPrivilege(StackStrF & tag, SQInteger value);

    /* --------------------------------------------------------------------------------------------
     * Remove all status values. Basically it reverts to the parent class privileges.
    */
    void RemoveAllPrivileges();

    /* --------------------------------------------------------------------------------------------
     * Change the parent class.
    */
    void AssignParent(const Ref & parent);

    /* --------------------------------------------------------------------------------------------
     * Check if this class has a certain privilege.
    */
    SQMOD_NODISCARD bool Can(SQInteger id, SQInteger req) const;

    /* --------------------------------------------------------------------------------------------
     * See if a unit with a certain identifier inherits this class.
    */
    SQMOD_NODISCARD LightObj GetUnitWithID(SQInteger id);

    /* --------------------------------------------------------------------------------------------
     * See if a unit with a certain tag inherits this class.
    */
    SQMOD_NODISCARD LightObj GetUnitWithTag(StackStrF & tag);

    /* --------------------------------------------------------------------------------------------
     * See if a unit with a certain identifier inherits this class.
    */
    bool HaveUnitWithID(SQInteger id);

    /* --------------------------------------------------------------------------------------------
     * See if a unit with a certain tag inherits this class.
    */
    bool HaveUnitWithTag(StackStrF & tag);

    /* --------------------------------------------------------------------------------------------
     * Invoke a given callback with every owned entry identifier.
    */
    void EachEntryID(Object & ctx, Function & func) const;

    /* --------------------------------------------------------------------------------------------
     * Invoke a given callback with every parented unit identifier.
    */
    void EachUnitID(Object & ctx, Function & func) const;
};

/* ------------------------------------------------------------------------------------------------
 * Privilege class wrapper. Mostly calls functions of the actual implementation.
 * Used to avoid having to involve hard script references that can persist.
 * Breaks away from usual formatting.
*/
struct SqPvClass
{
    PvClass::Ptr mI;
    // --------------------------------------------------------------------------------------------
    explicit SqPvClass(PvClass::Ptr i) : mI(std::move(i)) { } // NOLINT(modernize-pass-by-value)
    explicit SqPvClass(const PvClass::Ref & i) : mI(i) { }
    SqPvClass(const SqPvClass & o) = default;
    SqPvClass(SqPvClass && o) noexcept = default;
    ~SqPvClass() = default;
    SqPvClass & operator = (const SqPvClass & o) = default;
    SqPvClass & operator = (SqPvClass && o) noexcept = default;
    // --------------------------------------------------------------------------------------------
    void Validate() const { if (mI.expired()) STHROWF("Invalid unit reference"); }
    SQMOD_NODISCARD PvClass & Valid() const { Validate(); return *(mI.lock().get()); }
    // --------------------------------------------------------------------------------------------
    SQMOD_NODISCARD SQInteger GetID() const { return Valid().mID; }
    // --------------------------------------------------------------------------------------------
    SQMOD_NODISCARD auto ToString() const { return Valid().mTag.mObj; }
    SQMOD_NODISCARD auto GetTag() const { return Valid().mTag.mObj; }
    void SetTag(StackStrF & tag) const { Valid().SetTag(tag); }
    SQMOD_NODISCARD SqPvClass & ApplyTag(StackStrF & tag) { SetTag(tag); return *this; }
    SQMOD_NODISCARD LightObj & GetData() const { return Valid().mData; }
    void SetData(LightObj & data) const { Valid().mData = data; }
    // --------------------------------------------------------------------------------------------
    void SetOnQuery(Function & func) const { Valid().mOnQuery = std::move(func); }
    void SetOnLost(Function & func) const { Valid().mOnLost = std::move(func); }
    void SetOnGained(Function & func) const { Valid().mOnGained = std::move(func); }
    // --------------------------------------------------------------------------------------------
    SQMOD_NODISCARD LightObj GetManager() const { return LightObj(Valid().mManager); }
    // --------------------------------------------------------------------------------------------
    SQMOD_NODISCARD LightObj GetParent() const
    {
        return LightObj(SqTypeIdentity< SqPvClass >{}, SqVM(), Valid().mParent);
    }
    void SetParent(const SqPvClass & cls) const
    {
        Validate();
        cls.Validate();
        mI.lock()->AssignParent(cls.mI.lock());
    }
    // --------------------------------------------------------------------------------------------
    bool Can(LightObj & obj, SQInteger req) const;
    // --------------------------------------------------------------------------------------------
    void AssignPrivilegeWithID(SQInteger id, SQInteger value) const { Valid().AssignPrivilege(id, value); }
    void AssignPrivilegeWithTag(StackStrF & tag, SQInteger value) const { Valid().AssignPrivilege(tag, value); }
    void RemovePrivilegeWithID(SQInteger id) const { Valid().RemovePrivilege(id); }
    void RemovePrivilegeWithTag(StackStrF & tag) const { Valid().RemovePrivilege(tag); }
    void ModifyPrivilegeWithID(SQInteger id, SQInteger value) const { Valid().ModifyPrivilege(id, value); }
    void ModifyPrivilegeWithTag(StackStrF & tag, SQInteger value) const { Valid().ModifyPrivilege(tag, value); }
    void RemoveAllPrivileges() const { Valid().RemoveAllPrivileges(); }
    // --------------------------------------------------------------------------------------------
    SQMOD_NODISCARD LightObj GetUnitWithID(SQInteger id) const { return Valid().GetUnitWithID(id); }
    SQMOD_NODISCARD LightObj GetUnitWithTag(StackStrF & tag) const { return Valid().GetUnitWithTag(tag); }
    SQMOD_NODISCARD bool HaveUnitWithID(SQInteger id) const { return Valid().HaveUnitWithID(id); }
    SQMOD_NODISCARD bool HaveUnitWithTag(StackStrF & tag) const { return Valid().HaveUnitWithTag(tag); }
    // --------------------------------------------------------------------------------------------
    void EachEntryID(Object & ctx, Function & func) const { return Valid().EachEntryID(ctx, func); }
    void EachUnitID(Object & ctx, Function & func) const { return Valid().EachUnitID(ctx, func); }
};

} // Namespace:: SqMod
