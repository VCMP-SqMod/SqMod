#pragma once

// ------------------------------------------------------------------------------------------------
#include "Core/Privilege/Base.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * An entry represents a privilege that a class or unit can can have.
 * Each entry must have a unique numerical identifier within their associated manager.
*/
struct PvEntry
{
    /* --------------------------------------------------------------------------------------------
     * Strong and weak reference types.
    */
    typedef std::shared_ptr< PvEntry > Ref;
    typedef std::weak_ptr< PvEntry > Ptr;

    /* --------------------------------------------------------------------------------------------
     * Type of container used to store privilege entries.
    */
    typedef VecMap< PvIdentity, Ref, PvIdPred > List;

    /* --------------------------------------------------------------------------------------------
     * User identifier associated with this entry instance. Always unique in the same manager!
    */
    const SQInteger     mID;

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
     * Dedicated callback for privilege modify event.
    */
    Function            mOnModify;

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
     * Pointer to the associated manager. Should always be present.
    */
    PvManager *         mManager;

    /* -------------------------------------------------------------------------------------------
     * Default constructor.
    */
    PvEntry(SQInteger id, PvManager * mgr)
        : mID(id), mTag()
        , mOnQuery(), mOnModify(), mOnGained(), mOnLost()
        , mData(), mBrief(), mInfo(), mDefault()
        , mManager(mgr)
    {
    }

    /* -------------------------------------------------------------------------------------------
     * Default constructor.
    */
    PvEntry(SQInteger id, StackStrF && tag, PvManager * mgr)
        : mID(id), mTag(std::move(tag))
        , mOnQuery(), mOnModify(), mOnGained(), mOnLost()
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
    ~PvEntry() = default;

    /* -------------------------------------------------------------------------------------------
     * Copy assignment operator (disabled).
    */
    PvEntry & operator = (const PvEntry & o) = delete;

    /* -------------------------------------------------------------------------------------------
     * Move assignment operator (disabled).
    */
    PvEntry & operator = (PvEntry && o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Modify the associated user tag.
    */
    void SetTag(StackStrF & tag);

    /* --------------------------------------------------------------------------------------------
     * Release all script resources.
    */
    void Release();
};

/* ------------------------------------------------------------------------------------------------
 * Privilege entry wrapper. Mostly calls functions of the actual implementation.
 * Used to avoid having to involve hard script references that can persist.
 * Breaks away from usual formatting.
*/
struct SqPvEntry
{
    PvEntry::Ptr mI;
    // --------------------------------------------------------------------------------------------
    explicit SqPvEntry(PvEntry::Ptr i) : mI(std::move(i)) { } // NOLINT(modernize-pass-by-value)
    explicit SqPvEntry(const PvEntry::Ref & i) : mI(i) { }
    SqPvEntry(const SqPvEntry & o) = default;
    SqPvEntry(SqPvEntry && o) noexcept = default;
    ~SqPvEntry() = default;
    SqPvEntry & operator = (const SqPvEntry & o) = default;
    SqPvEntry & operator = (SqPvEntry && o) noexcept = default;
    // --------------------------------------------------------------------------------------------
    void Validate() const { if (mI.expired()) STHROWF("Invalid unit reference"); }
    SQMOD_NODISCARD PvEntry & Valid() const { Validate(); return *(mI.lock().get()); }
    // --------------------------------------------------------------------------------------------
    SQMOD_NODISCARD SQInteger GetID() const { return Valid().mID; }
    // --------------------------------------------------------------------------------------------
    SQMOD_NODISCARD auto ToString() const { return Valid().mTag.mObj; }
    SQMOD_NODISCARD auto GetTag() const { return Valid().mTag.mObj; }
    void SetTag(StackStrF & tag) const { Valid().SetTag(tag); }
    SQMOD_NODISCARD SqPvEntry & ApplyTag(StackStrF & tag) { SetTag(tag); return *this; }
    SQMOD_NODISCARD LightObj & GetData() const { return Valid().mData; }
    void SetData(LightObj & data) const { Valid().mData = data; }
    // --------------------------------------------------------------------------------------------
    void SetOnQuery(Function & func) const { Valid().mOnQuery = std::move(func); }
    void SetOnModify(Function & func) const { Valid().mOnModify = std::move(func); }
    void SetOnLost(Function & func) const { Valid().mOnLost = std::move(func); }
    void SetOnGained(Function & func) const { Valid().mOnGained = std::move(func); }
    // --------------------------------------------------------------------------------------------
    SQMOD_NODISCARD auto GetBrief() const { return Valid().mBrief.mObj; }
    void SetBrief(StackStrF & str) const { Valid().mBrief = std::move(str); }
    SQMOD_NODISCARD SqPvEntry & ApplyBrief(StackStrF & str) { SetBrief(str); return *this; }
    // --------------------------------------------------------------------------------------------
    SQMOD_NODISCARD auto GetInfo() const { return Valid().mInfo.mObj; }
    void SetInfo(StackStrF & str) const { Valid().mInfo = std::move(str); }
    SQMOD_NODISCARD SqPvEntry & ApplyInfo(StackStrF & str) { SetInfo(str); return *this; }
    // --------------------------------------------------------------------------------------------
    SQMOD_NODISCARD SQInteger GetDefault() const { return Valid().mDefault; }
    void SetDefault(SQInteger value) const { Valid().mDefault = value; }
    // --------------------------------------------------------------------------------------------
    SQMOD_NODISCARD LightObj GetManager() const { return LightObj(Valid().mManager); }
};

} // Namespace:: SqMod
