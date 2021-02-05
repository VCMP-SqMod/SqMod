#pragma once

// ------------------------------------------------------------------------------------------------
#include "Core/Privilege/Base.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

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
    typedef std::shared_ptr< PvUnit > Ref;
    typedef std::weak_ptr< PvUnit > Ptr;

    /* --------------------------------------------------------------------------------------------
     * Type of container used to store privilege units.
    */
    typedef VecMap< PvIdentity, Ref, PvIdPred > List;

    /* --------------------------------------------------------------------------------------------
     * User identifier associated with this unit instance. Always unique in the same manager!
    */
    const SQInteger     mID;

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
     * Weak pointer to the associated class. Units always have an associated class!
    */
    std::weak_ptr< PvClass > mClass;

    /* -------------------------------------------------------------------------------------------
     * Default constructor.
    */
    PvUnit(SQInteger id, std::weak_ptr< PvClass > cls)
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
    PvUnit(SQInteger id, StackStrF && tag, std::weak_ptr< PvClass > cls)
        : mID(id)
        , mAuthority(0)
        , mPrivileges()
        , mOnQuery(), mOnGained(), mOnLost()
        , mTag(std::move(tag)), mData()
        , mClass(std::move(cls))
    {
    }

    /* -------------------------------------------------------------------------------------------
     * Copy constructor (disabled).
    */
    PvUnit(const PvUnit & o) = delete;

    /* -------------------------------------------------------------------------------------------
     * Move constructor (disabled).
    */
    PvUnit(PvUnit && o) = delete;

    /* -------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~PvUnit() = default;

    /* -------------------------------------------------------------------------------------------
     * Copy assignment operator (disabled).
    */
    PvUnit & operator = (const PvUnit & o) = delete;

    /* -------------------------------------------------------------------------------------------
     * Move assignment operator (disabled).
    */
    PvUnit & operator = (PvUnit && o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Modify the associated user tag.
    */
    void SetTag(StackStrF & tag);

    /* --------------------------------------------------------------------------------------------
     * Release all script resources.
    */
    void Release();

    /* --------------------------------------------------------------------------------------------
     * Make sure the referenced parent class is valid.
    */
    void ValidateClass() const
    {
        if (mClass.expired())
        {
            STHROWF("Unit ({} : {}) has invalid class reference", mID, mTag.mPtr);
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Make sure the referenced parent class is valid and return a reference to it.
    */
    SQMOD_NODISCARD PvClass & ValidClass() const
    {
        ValidateClass();
        // Acquire a reference and return it
        return *(mClass.lock().get());
    }

    /* --------------------------------------------------------------------------------------------
     * Make sure the referenced manger is valid.
    */
    void ValidateManager() const;

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
     * Retrieve the current value of an entry for this particular unit.
    */
    SQMOD_NODISCARD SQInteger GetEntryValue(SQInteger id) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the inherited value of an entry for this particular unit.
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
     * Remove all status values. Basically it reverts to the current class privileges.
    */
    void RemoveAllPrivileges();

    /* --------------------------------------------------------------------------------------------
     * Assign a new class.
    */
    void AssignClass(const std::shared_ptr< PvClass > & cls);

    /* --------------------------------------------------------------------------------------------
     * Check if this unit has a certain privilege.
    */
    SQMOD_NODISCARD bool Can(SQInteger id, SQInteger req) const;

    /* --------------------------------------------------------------------------------------------
     * Invoke a given callback with every owned entry identifier.
    */
    void EachEntryID(Object & ctx, Function & func);
};

/* ------------------------------------------------------------------------------------------------
 * Privilege unit wrapper. Mostly calls functions of the actual implementation.
 * Used to avoid having to involve hard script references that can persist.
 * Breaks away from usual formatting.
*/
struct SqPvUnit
{
    PvUnit::Ptr mI;
    // --------------------------------------------------------------------------------------------
    explicit SqPvUnit(PvUnit::Ptr i) : mI(std::move(i)) { } // NOLINT(modernize-pass-by-value)
    explicit SqPvUnit(const PvUnit::Ref & i) : mI(i) { }
    SqPvUnit(const SqPvUnit & o) = default;
    SqPvUnit(SqPvUnit && o) noexcept = default;
    ~SqPvUnit() =  default;
    SqPvUnit & operator = (const SqPvUnit & o) = default;
    SqPvUnit & operator = (SqPvUnit && o) noexcept = default;
    // --------------------------------------------------------------------------------------------
    void Validate() const { if (mI.expired()) STHROWF("Invalid unit reference"); }
    SQMOD_NODISCARD PvUnit & Valid() const { Validate(); return *(mI.lock().get()); }
    // --------------------------------------------------------------------------------------------
    void ValidateCls() const { Validate(); if (mI.lock()->mClass.expired()) STHROWF("Invalid class reference"); }
    SQMOD_NODISCARD PvClass & ValidCls() const { ValidateCls(); return *(mI.lock()->mClass.lock().get()); }
    // --------------------------------------------------------------------------------------------
    SQMOD_NODISCARD SQInteger GetID() const { return Valid().mID; }
    // --------------------------------------------------------------------------------------------
    SQMOD_NODISCARD auto ToString() const { return Valid().mTag.mObj; }
    SQMOD_NODISCARD auto GetTag() const { return Valid().mTag.mObj; }
    void SetTag(StackStrF & tag) const { Valid().SetTag(tag); }
    SQMOD_NODISCARD SqPvUnit & ApplyTag(StackStrF & tag) { SetTag(tag); return *this; }
    SQMOD_NODISCARD LightObj & GetData() const { return Valid().mData; }
    void SetData(LightObj & data) const { Valid().mData = data; }
    // --------------------------------------------------------------------------------------------
    void SetOnQuery(Function & func) const { Valid().mOnQuery = std::move(func); }
    void SetOnLost(Function & func) const { Valid().mOnLost = std::move(func); }
    void SetOnGained(Function & func) const { Valid().mOnGained = std::move(func); }
    // --------------------------------------------------------------------------------------------
    SQMOD_NODISCARD SQInteger GetAuthority() const { return Valid().mAuthority; }
    void SetAuthority(SQInteger a) const { Valid().mAuthority = a; }
    // --------------------------------------------------------------------------------------------
    SQMOD_NODISCARD LightObj GetClass() const;
    void SetClass(const SqPvClass & cls) const;
    // --------------------------------------------------------------------------------------------
    SQMOD_NODISCARD LightObj GetManager() const;
    // --------------------------------------------------------------------------------------------
    bool Can(LightObj & obj, SQInteger req) const;
    // --------------------------------------------------------------------------------------------
    void AssignPrivilegeWithID(SQInteger id, SQInteger value) { Valid().AssignPrivilege(id, value); }
    void AssignPrivilegeWithTag(StackStrF & tag, SQInteger value) { Valid().AssignPrivilege(tag, value); }
    void RemovePrivilegeWithID(SQInteger id) { Valid().RemovePrivilege(id); }
    void RemovePrivilegeWithTag(StackStrF & tag) { Valid().RemovePrivilege(tag); }
    void ModifyPrivilegeWithID(SQInteger id, SQInteger value) { Valid().ModifyPrivilege(id, value); }
    void ModifyPrivilegeWithTag(StackStrF & tag, SQInteger value) { Valid().ModifyPrivilege(tag, value); }
    void RemoveAllPrivileges() { Valid().RemoveAllPrivileges(); }
    // --------------------------------------------------------------------------------------------
    void EachEntryID(Object & ctx, Function & func) { return Valid().EachEntryID(ctx, func); }
};

} // Namespace:: SqMod
