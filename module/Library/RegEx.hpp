#pragma once

// ------------------------------------------------------------------------------------------------
#include "Core/Utility.hpp"

// ------------------------------------------------------------------------------------------------
#ifdef POCO_UNBUNDLED
	#include <pcre.h>
#else
	#include "pcre_config.h"
	#include "pcre.h"
#endif

// ------------------------------------------------------------------------------------------------
#include <utility>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * 
*/
struct RxMatch
{
    /* --------------------------------------------------------------------------------------------
     * 
    */
	SQInteger mOffset{0};

    /* --------------------------------------------------------------------------------------------
     * 
    */
	SQInteger mLength{0};
    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    RxMatch() noexcept = default;

    /* --------------------------------------------------------------------------------------------
     * Offset constructor.
    */
    explicit RxMatch(SQInteger offset) noexcept
        : mOffset{offset}
    {
    }

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    RxMatch(SQInteger offset, SQInteger length) noexcept
        : mOffset{offset}, mLength{length}
    {
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    RxMatch(const RxMatch & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    RxMatch(RxMatch && o) noexcept = default;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    RxMatch & operator = (const RxMatch & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    RxMatch & operator = (RxMatch && o) noexcept = default;

    /* --------------------------------------------------------------------------------------------
     * Retrieve offset.
    */
    SQMOD_NODISCARD SQInteger GetOffset() const noexcept
    {
        return mOffset;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify offset.
    */
    void SetOffset(SQInteger value) noexcept
    {
        mOffset = value;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve length.
    */
    SQMOD_NODISCARD SQInteger GetLength() const noexcept
    {
        return mLength;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify length.
    */
    void SetLength(SQInteger value) noexcept
    {
        mLength = value;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve match end.
    */
    SQMOD_NODISCARD SQInteger GetEnd() const noexcept
    {
        return mOffset + mLength;
    }

    /* --------------------------------------------------------------------------------------------
     * Extract a sub-string.
    */
    [[nodiscard]] LightObj SubStr(StackStrF & str) const
    {
        if ((mOffset + mLength) > str.mLen)
        {
            STHROWF("Rx: Match is outside the range of the specified string.");
        }
        // Return the sub-string
        return LightObj{str.mPtr + mOffset, mLength};
    }
};

/* ------------------------------------------------------------------------------------------------
 * 
*/
struct RxMatches
{
    using List = std::vector< RxMatch >;

    /* --------------------------------------------------------------------------------------------
     * Internal RegularExpression instance.
    */
    List mList;

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    RxMatches() = default;

    /* --------------------------------------------------------------------------------------------
     * Copy list constructor.
    */
    explicit RxMatches(const List & l) // NOLINT(modernize-pass-by-value)
            : mList{l}
    {
    }

    /* --------------------------------------------------------------------------------------------
     * Move list constructor.
    */
    explicit RxMatches(List && m) noexcept
            : mList{std::move(m)}
    {
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    RxMatches(const RxMatches & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    RxMatches(RxMatches && o) noexcept = default;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    RxMatches & operator = (const RxMatches & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    RxMatches & operator = (RxMatches && o) noexcept = default;

    /* --------------------------------------------------------------------------------------------
     * Make sure an index is within range and return the container. Container must exist.
    */
    List & ValidIdx(SQInteger i)
    {
        if (static_cast< size_t >(i) >= mList.size())
        {
            STHROWF("Invalid Regular Expression match list index({})", i);
        }
        return mList;
    }

    /* --------------------------------------------------------------------------------------------
     * Make sure an index is within range and return the container. Container must exist.
    */
    SQMOD_NODISCARD const List & ValidIdx(SQInteger i) const
    {
        if (static_cast< size_t >(i) >= mList.size())
        {
            STHROWF("Invalid Regular Expression match list index({})", i);
        }
        return mList;
    }

    /* --------------------------------------------------------------------------------------------
     * Make sure a container instance is populated, then return it.
    */
    SQMOD_NODISCARD List & ValidPop()
    {
        if (mList.empty())
        {
            STHROWF("Regular Expression match list container is empty");
        }
        return mList;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve a value from the container.
    */
    SQMOD_NODISCARD List::reference Get(SQInteger i)
    {
        return ValidIdx(i).at(ClampL< SQInteger, size_t >(i));
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the first element in the container.
    */
    SQMOD_NODISCARD List::reference Front()
    {
        return ValidPop().front();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the last element in the container.
    */
    SQMOD_NODISCARD List::reference Back()
    {
        return mList.back();
    }

    /* --------------------------------------------------------------------------------------------
     * Check if the container has no elements.
    */
    SQMOD_NODISCARD bool Empty() const
    {
        return mList.empty();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the number of elements in the container.
    */
    SQMOD_NODISCARD SQInteger Size() const
    {
        return static_cast< SQInteger >(mList.size());
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the number of elements that the container has currently allocated space for.
    */
    SQMOD_NODISCARD SQInteger Capacity() const
    {
        return static_cast< SQInteger >(mList.capacity());
    }

    /* --------------------------------------------------------------------------------------------
     * Increase the capacity of the container to a value that's greater or equal to the one specified.
    */
    RxMatches & Reserve(SQInteger n)
    {
        mList.reserve(ClampL< SQInteger, size_t >(n));
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Request the removal of unused capacity.
    */
    void Compact()
    {
        mList.shrink_to_fit();
    }

    /* --------------------------------------------------------------------------------------------
     * Erase all elements from the container.
    */
    void Clear()
    {
        mList.clear();
    }

    /* --------------------------------------------------------------------------------------------
     * Pop the last element in the container.
    */
    void Pop()
    {
        ValidPop().pop_back();
    }

    /* --------------------------------------------------------------------------------------------
     * Erase the element at a certain position.
    */
    void EraseAt(SQInteger i)
    {
        mList.erase(ValidIdx(i).begin() + static_cast< size_t >(i)); // NOLINT(cppcoreguidelines-narrowing-conversions)
    }

    /* --------------------------------------------------------------------------------------------
     * Erase a certain amount of elements starting from a specific position.
    */
    void EraseFrom(SQInteger i, SQInteger n)
    {
        mList.erase(ValidIdx(i).begin() + static_cast< size_t >(i), // NOLINT(cppcoreguidelines-narrowing-conversions)
                     ValidIdx(i + n).begin() + static_cast< size_t >(i + n)); // NOLINT(cppcoreguidelines-narrowing-conversions)
    }

    /* --------------------------------------------------------------------------------------------
     * Iterate all values through a functor.
    */
    void Each(Function & fn) const
    {
        for (const auto & e : mList)
        {
            fn.Execute(static_cast< SQInteger >(e.mOffset), static_cast< SQInteger >(e.mLength));
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Iterate values in range through a functor.
    */
    void EachRange(SQInteger p, SQInteger n, Function & fn) const
    {
        std::for_each(ValidIdx(p).begin() + static_cast< size_t >(p), // NOLINT(cppcoreguidelines-narrowing-conversions)
                      ValidIdx(p + n).begin() + static_cast< size_t >(p + n), // NOLINT(cppcoreguidelines-narrowing-conversions)
                      [&](List::const_reference & e) {
                          fn.Execute(static_cast< SQInteger >(e.mOffset), static_cast< SQInteger >(e.mLength));
                      });
    }

    /* --------------------------------------------------------------------------------------------
     * Iterate all values through a functor until stopped (i.e. false is returned).
    */
    void While(Function & fn) const
    {
        for (const auto & e : mList)
        {
            auto ret = fn.Eval(static_cast< SQInteger >(e.mOffset), static_cast< SQInteger >(e.mLength));
            // (null || true) == continue & false == break
            if (!ret.IsNull() || !ret.template Cast< bool >())
            {
                break;
            }
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Iterate values in range through a functor until stopped (i.e. false is returned).
    */
    void WhileRange(SQInteger p, SQInteger n, Function & fn) const
    {
        auto itr = ValidIdx(p).begin() + static_cast< size_t >(p); // NOLINT(cppcoreguidelines-narrowing-conversions)
        auto end = ValidIdx(p + n).begin() + static_cast< size_t >(p + n); // NOLINT(cppcoreguidelines-narrowing-conversions)
        for (; itr != end; ++itr)
        {
            auto ret = fn.Eval(static_cast< SQInteger >(itr->mOffset), static_cast< SQInteger >(itr->mLength));
            // (null || true) == continue & false == break
            if (!ret.IsNull() || !ret.template Cast< bool >())
            {
                break;
            }
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Extract a sub-string.
    */
    [[nodiscard]] LightObj SubStr(SQInteger i, StackStrF & str) const
    {
        const RxMatch & m = ValidIdx(i)[i];
        // Check if match is within range
        if ((m.mOffset + m.mLength) > str.mLen)
        {
            STHROWF("Rx: Match is outside the range of the specified string.");
        }
        // Return the sub-string
        return LightObj{str.mPtr + m.mOffset, m.mLength};
    }
};

/* ------------------------------------------------------------------------------------------------
 * 
*/
struct RxInstance
{
    /* --------------------------------------------------------------------------------------------
     * Whether to analyze and optimize the pattern by default for evey new instance (true).
    */
    static bool STUDY;

    /* --------------------------------------------------------------------------------------------
     * Default options for every new instance (0).
    */
    static int OPTIONS;

    /* --------------------------------------------------------------------------------------------
     * Default study options for every new instance (0).
    */
    static int STUDY_OPTIONS;

    /* --------------------------------------------------------------------------------------------
     * Default offset vector size (must be multiple of 3).
    */
    static constexpr int OVEC_SIZE = 63;

    /* --------------------------------------------------------------------------------------------
     * Internal vector type used for offsets buffer.
    */
    using OVEC_t = std::vector< int >;

    /* --------------------------------------------------------------------------------------------
     * Internal RegularExpression instance.
    */
	pcre * 			mPCRE{nullptr};

    /* --------------------------------------------------------------------------------------------
     * Internal RegularExpression instance.
    */
	pcre_extra * 	mExtra{nullptr};

    /* --------------------------------------------------------------------------------------------
     * Internal buffer used for offsets.
    */
    OVEC_t          mOVEC{};

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    RxInstance() noexcept = default;

    /* --------------------------------------------------------------------------------------------
     * Copy constructor (disabled).
    */
    RxInstance(const RxInstance &) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    RxInstance(RxInstance && o) noexcept
        : mPCRE(o.mPCRE), mExtra(o.mExtra), mOVEC(std::move(o.mOVEC)) // Replicate it
    {
        o.mPCRE = nullptr; // Take ownership
        o.mExtra = nullptr; // Take ownership
    }

    /* --------------------------------------------------------------------------------------------
     * Basic constructor.
    */
    explicit RxInstance(StackStrF & pattern)
        : RxInstance(OPTIONS, STUDY, pattern)
    {
    }

    /* --------------------------------------------------------------------------------------------
     * Basic constructor. With specific options.
    */
    explicit RxInstance(int options, StackStrF & pattern)
        : RxInstance(options, STUDY, pattern)
    {
    }

    /* --------------------------------------------------------------------------------------------
     * Basic constructor. With specific options.
    */
    explicit RxInstance(int options, bool study, StackStrF & pattern)
        : mPCRE(Compile_(pattern.mPtr, options)), mExtra(nullptr)
    {
        if (study)
        {
            Study0();
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Internal constructor.
    */
    RxInstance(const char * pattern, int options, bool study)
        : mPCRE(Compile_(pattern, options)), mExtra(nullptr)
    {
        if (study)
        {
            Study0();
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
	~RxInstance()
	{
        Destroy();
	}

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator (disabled).
    */
    RxInstance & operator = (const RxInstance &) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    RxInstance & operator = (RxInstance && o) noexcept
    {
        // Prevent self assignment
        if (this != &o)
        {
            // Release current instance, if any
            Destroy();
            // Replicate it
            mPCRE = o.mPCRE;
            mExtra = o.mExtra;
            mOVEC = std::move(o.mOVEC);
            // Take ownership
            o.mPCRE = nullptr;
            o.mExtra = nullptr;
        }
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Estimate the size necessary for the offsets vector buffer.
    */
    void EstimateOVEC(bool force = false)
    {
        if (mOVEC.empty() || force)
        {
            int size = 0;
            // Attempt to estimate the size of the offsets vector buffer
            const int r = pcre_fullinfo(ValidPCRE(), mExtra, PCRE_INFO_CAPTURECOUNT, &size);
            // Check for errors
            if (r != 0)
            {
                STHROWF("Rx: Offsets vector buffer estimation failed ({})", r);
            }
            // Attempt to scale the vector (must be multiple of 3)
            mOVEC.resize((size + 1) * 3);
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Return a valid `pcre` instance pointer or throw an exception.
    */
    SQMOD_NODISCARD pcre * ValidPCRE() const
    {
        // Do we manage a valid instance?
        if (mPCRE == nullptr)
        {
            STHROWF("Uninitialized Regular Expression instance.");
        }
        // Return it
        return mPCRE;
    }

    /* --------------------------------------------------------------------------------------------
     * Return a valid `pcre_extra` instance pointer or throw an exception.
    */
    SQMOD_NODISCARD pcre_extra * ValidExtra() const
    {
        // Do we manage a valid instance?
        if (mExtra == nullptr)
        {
            STHROWF("Regular Expression was not studied and optimized.");
        }
        // Return it
        return mExtra;
    }

    /* --------------------------------------------------------------------------------------------
     * Compile the specified pattern.
    */
    SQMOD_NODISCARD static pcre * Compile_(const char * pattern, int options = OPTIONS)
    {
        const char * error_msg = nullptr;
        int error_code, error_offset = 0;
        // Attempt to compile the specified pattern
        pcre * ptr = pcre_compile2(pattern, options, &error_code, &error_msg, &error_offset, nullptr);
        // Did the compilation failed?
        if (ptr == nullptr)
        {
            STHROWF("Rx: {s} (code {}) (at offset {})", error_msg, error_code, error_offset);
        }
        // Return the `pcre` instance
        return ptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to compile the specified pattern. Error information is returned instead of thrown.
    */
    SQMOD_NODISCARD static std::pair< pcre *, Table > TryCompile_(const char * pattern, int options = OPTIONS)
    {
        const char * error_msg = nullptr;
        int error_code, error_offset = 0;
        // Attempt to compile the specified pattern
        pcre * ptr = pcre_compile2(pattern, options, &error_code, &error_msg, &error_offset, nullptr);
        // Did the compilation failed?
        if (ptr == nullptr)
        {
            Table t;
            t.SetValue("message", error_msg);
            t.SetValue("code", error_code);
            t.SetValue("offset", error_offset);
            // Return the table with error information
            return std::make_pair(ptr, std::move(t));
        }
        // Return the `pcre` instance with no error information
        return std::make_pair(ptr, Table{});
    }

    /* --------------------------------------------------------------------------------------------
     * Compile the specified pattern.
    */
    RxInstance & Compile1(StackStrF & pattern)
    {
        return Compile2(OPTIONS, pattern);
    }

    /* --------------------------------------------------------------------------------------------
     * Compile the specified pattern. With specific options.
    */
    RxInstance & Compile2(int options, StackStrF & pattern)
    {
        // Release current instance, if any
        Destroy();
        // Attempt to compile
        mPCRE = Compile_(pattern.mPtr, options);
        // Allocate offsets vector buffer
        EstimateOVEC();
        // Allow chaining
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Compile the specified pattern.
    */
    Table TryCompile1(StackStrF & pattern)
    {
        return TryCompile2(OPTIONS, pattern);
    }

    /* --------------------------------------------------------------------------------------------
     * Compile the specified pattern. With specific options.
    */
    Table TryCompile2(int options, StackStrF & pattern)
    {
        // Release current instance, if any
        Destroy();
        // Attempt to compile
        auto p = TryCompile_(pattern.mPtr, options);
        // Were there any compilation errors?
        if (p.first != nullptr)
        {
            mPCRE = p.first;
        }
        // Return compilation info
        return p.second;
    }

    /* --------------------------------------------------------------------------------------------
     * Analyze the managed pattern and optimized it.
    */
    RxInstance & Study0()
    {
        return Study1(STUDY_OPTIONS);
    }

    /* --------------------------------------------------------------------------------------------
     * Analyze the managed pattern and optimized it. With specific options.
    */
    RxInstance & Study1(int options)
    {
        if (mExtra != nullptr)
        {
            STHROWF("Regular Expression was already analyzed and optimized");
        }
        const char * error = nullptr;
        // Study and optimize the expression
        mExtra = pcre_study(ValidPCRE(), options, &error);
        // If there was an error studying the expression then throw it
        if (mExtra == nullptr && error != nullptr)
        {
            STHROWF("Rx: {s}", error);
        }
        // Allow chaining
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Release managed resources and revert to uninitialized instance.
    */
    SQMOD_NODISCARD bool IsValid() const
    {
        return (mPCRE != nullptr);
    }

    /* --------------------------------------------------------------------------------------------
     * Release managed resources and revert to uninitialized instance.
    */
    SQMOD_NODISCARD bool IsStudied() const
    {
        return (mExtra != nullptr);
    }

    /* --------------------------------------------------------------------------------------------
     * Release managed resources and revert to uninitialized instance.
    */
    void Destroy()
    {
        // Do we manage any instance?
        if (mPCRE != nullptr)
        {
            pcre_free(mPCRE);
            mPCRE = nullptr;
        }
        // Is the expression optimized?
        if (mExtra != nullptr)
        {
            pcre_free(mExtra);
            mExtra = nullptr;
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Matches the given subject string against the pattern.
     * Returns the position of the first captured sub-string in m.
     * If no part of the subject matches the pattern, m.mOffset is -1 and m.mLength is 0.
     * Returns the number of matches. Throws a exception in case of an error.
    */
    SQMOD_NODISCARD int MatchFirstFrom(SQInteger o, RxMatch & m, StackStrF & s)
    {
        return MatchFirstFrom_(OPTIONS, o, m, s);
    }
    SQMOD_NODISCARD int MatchFirstFrom_(int f, SQInteger o, RxMatch & m, StackStrF & s)
    {
        if (o > s.mLen)
        {
            STHROWF("Rx: Offset is out of range");
        }
        EstimateOVEC();
        // Attempt to execute the expression on the specified subject
        const int rc = pcre_exec(ValidPCRE(), mExtra, s.mPtr, static_cast< int >(s.mLen), static_cast< int >(o), f & 0xFFFF, mOVEC.data(), static_cast< int >(mOVEC.size()));
        // Was there a match?
        if (rc == PCRE_ERROR_NOMATCH)
        {
            m.mOffset = -1;
            m.mLength = 0;
            // No match found
            return 0;
        }
        // Bad options/flags?
        else if (rc == PCRE_ERROR_BADOPTION)
        {
            STHROWF("Rx: Bad option");
        }
        // Overflow?
        else if (rc == 0)
        {
            STHROWF("Rx: too many captured sub-strings");
        }
        // Some other error?
        else if (rc < 0)
        {
            STHROWF("Rx: error {}", rc);
        }
        // Store match
        m.mOffset = mOVEC[0];
        m.mLength = mOVEC[1] - mOVEC[0];
        // Yield result back to script
        return rc;
    }

    /* --------------------------------------------------------------------------------------------
     * Matches the given subject string against the pattern.
     * Returns the position of the first captured sub-string in m.
     * If no part of the subject matches the pattern, m.mOffset is -1 and m.mLength is 0.
     * Returns the number of matches. Throws a exception in case of an error.
    */
    SQMOD_NODISCARD int MatchFirst(RxMatch & m, StackStrF & s)
    {
        return MatchFirstFrom_(OPTIONS, 0, m, s);
    }
    SQMOD_NODISCARD int MatchFirst_(int f, RxMatch & m, StackStrF & s)
    {
        return MatchFirstFrom_(f, 0, m, s);
    }

    /* --------------------------------------------------------------------------------------------
     * Matches the given subject string against the pattern.
     * The first entry in m contains the position of the captured sub-string.
     * The following entries identify matching sub-patterns. See the PCRE documentation for a more detailed explanation.
     * If no part of the subject matches the pattern, m is empty.
     * Returns the number of matches. Throws an exception in case of an error.
    */
    SQMOD_NODISCARD int MatchFrom(SQInteger o, RxMatches & m, StackStrF & s)
    {
        return MatchFrom_(OPTIONS, o, m, s);
    }
    SQMOD_NODISCARD int MatchFrom_(int f, SQInteger o, RxMatches & m, StackStrF & s)
    {
        if (o > s.mLen)
        {
            STHROWF("Rx: Offset is out of range");
        }
        EstimateOVEC();
        // Clear previous matches, if any
        m.mList.clear();
        // Attempt to execute the expression on the specified subject
        const int rc = pcre_exec(ValidPCRE(), mExtra, s.mPtr, static_cast< int >(s.mLen), static_cast< int >(o), f & 0xFFFF, mOVEC.data(), static_cast< int >(mOVEC.size()));
        // Was there a match?
        if (rc == PCRE_ERROR_NOMATCH)
        {
            return 0; // No match found
        }
        // Bad options/flags?
        else if (rc == PCRE_ERROR_BADOPTION)
        {
            STHROWF("Rx: Bad option");
        }
        // Overflow?
        else if (rc == 0)
        {
            STHROWF("Rx: too many captured sub-strings");
        }
        // Some other error?
        else if (rc < 0)
        {
            STHROWF("Rx: error {}", rc);
        }
        // Reserve space in advance
        m.mList.reserve(static_cast< size_t >(rc));
        // Transfer matches to match-list
        for (int i = 0; i < rc; ++i)
        {
            m.mList.emplace_back(mOVEC[i*2], mOVEC[i*2+1] - mOVEC[i*2]);
        }
        // Yield result back to script
        return rc;
    }

    /* --------------------------------------------------------------------------------------------
     * Matches the given subject string against the pattern.
     * The first entry in m contains the position of the captured sub-string.
     * The following entries identify matching sub-patterns. See the PCRE documentation for a more detailed explanation.
     * If no part of the subject matches the pattern, m is empty.
     * Returns the number of matches. Throws an exception in case of an error.
    */
    SQMOD_NODISCARD int Match(RxMatches & m, StackStrF & s)
    {
        return MatchFrom_(OPTIONS, 0, m, s);
    }
    SQMOD_NODISCARD int Match_(int f, RxMatches & m, StackStrF & s)
    {
        return MatchFrom_(f, 0, m, s);
    }

    /* --------------------------------------------------------------------------------------------
     * Returns true if and only if the subject matches the regular expression.
     * Internally, this method sets the RE_ANCHORED and RE_NOTEMPTY options for matching,
     * which means that the empty string will never match and the pattern is treated as if it starts with a ^.
    */
    SQMOD_NODISCARD bool Matches(StackStrF & s)
    {
        return Matches_(PCRE_ANCHORED | PCRE_NOTEMPTY, s);
    }
    SQMOD_NODISCARD bool Matches_(SQInteger o, StackStrF & s)
    {
        return MatchesEx(PCRE_ANCHORED | PCRE_NOTEMPTY, 0, s);
    }
    SQMOD_NODISCARD bool MatchesEx(int f, SQInteger o, StackStrF & s)
    {
        RxMatch m;
        const int rc = MatchFirstFrom_(f, o, m, s);
        return (rc > 0) && (m.mOffset == o) && (m.mLength == (s.mLen - o));
    }
};


} // Namespace:: SqMod
