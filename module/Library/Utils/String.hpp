#pragma once

// ------------------------------------------------------------------------------------------------
#include "Core/Utility.hpp"

// ------------------------------------------------------------------------------------------------
#include <Poco/String.h>

// ------------------------------------------------------------------------------------------------
#include <string>
#include <random>
#include <iterator>
#include <algorithm>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Wrapper around a std::string.
*/
struct SqString
{
    /* --------------------------------------------------------------------------------------------
     * String instance.
    */
    String mS{};

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    SqString() = default;

    /* --------------------------------------------------------------------------------------------
     * Construct with initial capacity. No element is created.
    */
    explicit SqString(SQInteger n)
        : mS()
    {
        mS.reserve(ClampL< SQInteger, size_t >(n));
    }

    /* --------------------------------------------------------------------------------------------
     * Construct with initial string.
    */
    explicit SqString(StackStrF & s)
        : mS(s.mPtr, s.GetSize())
    {
    }

    /* --------------------------------------------------------------------------------------------
     * Construct with initial size. Filled with copies of specified element.
    */
    SqString(SQInteger n, String::value_type v)
        : mS()
    {
        mS.resize(ClampL< SQInteger, size_t >(n), v);
    }

    /* --------------------------------------------------------------------------------------------
     * Construct with forwarded native arguments.
    */
    template < class... Args > explicit SqString(SqInPlace SQ_UNUSED_ARG(x), Args&&... args)
        : mS(std::forward< Args >(args)...)
    {
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor from reference.
    */
    explicit SqString(const String & s) // NOLINT(modernize-pass-by-value)
        : mS(s)
    {
    }

    /* --------------------------------------------------------------------------------------------
     * Move constructor from reference.
    */
    explicit SqString(String && s) noexcept
        : mS(std::move(s))
    {
    }

    /* --------------------------------------------------------------------------------------------
     * Construct with character range.
    */
    SqString(String::iterator f, String::iterator l)
        : mS(f, l)
    {
    }

    /* --------------------------------------------------------------------------------------------
     * Construct with character range.
    */
    SqString(String::const_iterator f, String::const_iterator l)
        : mS(f, l)
    {
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    SqString(const SqString & s) = default;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    SqString(SqString &&) noexcept = default;

    /* --------------------------------------------------------------------------------------------
     * Destroys the Statement.
    */
    ~SqString() = default;

    /* --------------------------------------------------------------------------------------------
     * Assignment operator.
    */
    SqString & operator = (const SqString &) = default;

    /* --------------------------------------------------------------------------------------------
     * Move assignment.
    */
    SqString & operator = (SqString &&) noexcept = default;

    /* --------------------------------------------------------------------------------------------
     * Make sure an index is within range and return the container.
    */
    SQMOD_NODISCARD String & ValidIdx(SQInteger i)
    {
        if (static_cast< size_t >(i) >= mS.size())
        {
            STHROWF("Invalid string container index({})", i);
        }
        return mS;
    }

    /* --------------------------------------------------------------------------------------------
     * Make sure an index is within range and return the container.
    */
    SQMOD_NODISCARD const String & ValidIdx(SQInteger i) const
    {
        if (static_cast< size_t >(i) >= mS.size())
        {
            STHROWF("Invalid string container index({})", i);
        }
        return mS;
    }

    /* --------------------------------------------------------------------------------------------
     * Make sure a container instance is referenced and is populated, then return it.
    */
    SQMOD_NODISCARD String & ValidPop()
    {
        if (mS.empty())
        {
            STHROWF("String container is empty");
        }
        return mS;
    }

    /* --------------------------------------------------------------------------------------------
     * Make sure a container instance is referenced and is populated, then return it.
    */
    SQMOD_NODISCARD const String & ValidPop() const
    {
        if (mS.empty())
        {
            STHROWF("String container is empty");
        }
        return mS;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the internal string container.
    */
    SQMOD_NODISCARD String & ToString()
    {
        return mS;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the internal string container.
    */
    SQMOD_NODISCARD const String & ToString() const
    {
        return mS;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the internal string container.
    */
    SQMOD_NODISCARD const String & GetString() const
    {
        return mS;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the internal string container.
    */
    SQMOD_NODISCARD SqString & SetString(StackStrF & str)
    {
        mS.assign(str.mPtr, static_cast< size_t >(str.mLen));
        return *this; // Allow chaining
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve a value from the container.
    */
    SQMOD_NODISCARD SQInteger Get(SQInteger i) const
    {
        return ValidIdx(i)[ClampL< SQInteger, size_t >(i)];
    }

    /* --------------------------------------------------------------------------------------------
     * Modify a value from the container.
    */
    void Set(SQInteger i, String::value_type v)
    {
        ValidIdx(i)[i] = v;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the first element in the container.
    */
    SQMOD_NODISCARD SQInteger Front() const
    {
        return ValidPop().front();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the last element in the container.
    */
    SQMOD_NODISCARD SQInteger Back() const
    {
        return ValidPop().back();
    }

    /* --------------------------------------------------------------------------------------------
     * Check if the container has no elements.
    */
    SQMOD_NODISCARD bool Empty() const
    {
        return mS.empty();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the number of elements in the container.
    */
    SQMOD_NODISCARD SQInteger Size() const
    {
        return static_cast< SQInteger >(mS.size());
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the number of elements that the container has currently allocated space for.
    */
    SQMOD_NODISCARD SQInteger Capacity() const
    {
        return static_cast< SQInteger >(mS.capacity());
    }

    /* --------------------------------------------------------------------------------------------
     * Resize the container to contain a specific amount of elements.
    */
    void Resize(SQInteger n)
    {
        mS.resize(ClampL< SQInteger, size_t >(n));
    }

    /* --------------------------------------------------------------------------------------------
     * Resize the container to contain a specific amount of elements.
    */
    SqString & ResizeEx(SQInteger n, String::value_type v)
    {
        mS.resize(ClampL< SQInteger, size_t >(n), v);
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Increase the capacity of the container to a value that's greater or equal to the one specified.
    */
    SqString & Reserve(SQInteger n)
    {
        mS.reserve(ClampL< SQInteger, size_t >(n));
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Request the removal of unused capacity.
    */
    void Compact()
    {
        mS.shrink_to_fit();
    }

    /* --------------------------------------------------------------------------------------------
     * Erase all elements from the container.
    */
    void Clear()
    {
        mS.clear();
    }

    /* --------------------------------------------------------------------------------------------
     * Push a value at the back of the container.
    */
    void Push(String::value_type v)
    {
        mS.push_back(v);
    }

    /* --------------------------------------------------------------------------------------------
     * Push copies of a value at the back of the container.
    */
    void Append(SQInteger n, String::value_type v)
    {
        mS.append(static_cast< size_t >(n), v);
    }

    /* --------------------------------------------------------------------------------------------
     * Extends the Container by appending all the items in the given container.
    */
    void Extend(StackStrF & s)
    {
        mS.append(s.mPtr, s.GetSize());
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
        mS.erase(ValidIdx(i).begin() + static_cast< size_t >(i));
    }

    /* --------------------------------------------------------------------------------------------
     * Erase a certain amount of elements starting from a specific position.
    */
    void EraseFrom(SQInteger i, SQInteger n)
    {
        mS.erase(ValidIdx(i).begin() + static_cast< size_t >(i),
                  ValidIdx(i + n).begin() + static_cast< size_t >(i + n));
    }

    /* --------------------------------------------------------------------------------------------
     * Erase all occurrences of value from the container.
    */
    void EraseValue(String::value_type v)
    {
        mS.erase(std::remove(mS.begin(), mS.end(), v), mS.end());
    }

    /* --------------------------------------------------------------------------------------------
     * Insert a specific value starting from a certain position.
    */
    void InsertAt(SQInteger i, String::value_type v)
    {
        mS.insert(ValidIdx(i).begin() + static_cast< size_t >(i), v);
    }

    /* --------------------------------------------------------------------------------------------
     * Insert a certain amount of copies of a value starting from a specific position.
    */
    void Insert(SQInteger i, SQInteger n, String::value_type v)
    {
        mS.insert(ValidIdx(i).begin() + static_cast< size_t >(i), ClampL< SQInteger, size_t >(n), v);
    }

    /* --------------------------------------------------------------------------------------------
     * Count the occurrences of a value in the container.
    */
    SQMOD_NODISCARD SQInteger Count(String::value_type v) const
    {
        return static_cast< SQInteger >(std::count(mS.begin(), mS.end(), v));
    }

    /* --------------------------------------------------------------------------------------------
     * See if values are the same as another container.
    */
    SQMOD_NODISCARD bool Equal(SqString & o) const { return mS == o.mS; }

    /* --------------------------------------------------------------------------------------------
     * Retrieve a portion of this container.
    */
    SQMOD_NODISCARD LightObj Slice(SQInteger p, SQInteger n) const
    {
        return LightObj(SqTypeIdentity< SqString >{}, SqVM(),
                        ValidIdx(p).begin() + static_cast< size_t >(p),
                        ValidIdx(p + n).begin() + static_cast< size_t >(p + n));
    }

    /* --------------------------------------------------------------------------------------------
     * Iterate all values through a functor.
    */
    void Each(Function & fn) const
    {
        for (const auto & e : mS)
        {
            fn.Execute(e);
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Iterate values in range through a functor.
    */
    void EachRange(SQInteger p, SQInteger n, Function & fn) const
    {
        std::for_each(ValidIdx(p).begin() + static_cast< size_t >(p),
                      ValidIdx(p + n).begin() + static_cast< size_t >(p + n),
        [&](String::const_reference e) {
            fn.Execute(e);
        });
    }

    /* --------------------------------------------------------------------------------------------
     * Iterate all values through a functor until stopped (i.e false is returned).
    */
    void While(Function & fn) const
    {
        for (const auto & e : mS)
        {
            auto ret = fn.Eval(e);
            // (null || true) == continue & false == break
            if (!ret.IsNull() || !ret.template Cast< bool >())
            {
                break;
            }
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Iterate values in range through a functor until stopped (i.e false is returned).
    */
    void WhileRange(SQInteger p, SQInteger n, Function & fn) const
    {
        auto itr = ValidIdx(p).begin() + static_cast< size_t >(p);
        auto end = ValidIdx(p + n).begin() + static_cast< size_t >(p + n);
        for (; itr != end; ++itr)
        {
            auto ret = fn.Eval(*itr);
            // (null || true) == continue & false == break
            if (!ret.IsNull() || !ret.template Cast< bool >())
            {
                break;
            }
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Reverse the order of elements in the container.
    */
    SqString & Reverse()
    {
        std::reverse(mS.begin(), mS.end());
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Generate new elements at the back of the container.
    */
    SqString & Generate(LightObj & ctx, Function & fn)
    {
        for (;;)
        {
            auto ret = fn.Eval(ctx);
            // null == break
            if (ret.IsNull())
            {
                break;
            }
            // Extract the value from object
            mS.push_back(ret.Cast< String::value_type >());
        }
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Generate new elements at the back of the container.
    */
    SqString & GenerateSome(SQInteger n, LightObj & ctx, Function & fn)
    {
        while (n--)
        {
            auto ret = fn.Eval(ctx);
            // Extract the value from object
            mS.push_back(ret.Cast< String::value_type >());
        }
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Generate new elements at specified position.
    */
    SqString & GenerateFrom(SQInteger p, SQInteger n, LightObj & ctx, Function & fn)
    {
        if (static_cast< size_t >(p) >= mS.size())
        {
            STHROWF("Invalid container index ({} >= {})", p, mS.size());
        }
        for (auto i = static_cast< size_t >(p); n--; ++i)
        {
            auto ret = fn.Eval(ctx);
            // Extract the value from object and insert it
            mS.insert(mS.begin() + i, ret.Cast< String::value_type >());
        }
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Generate new elements at specified position.
    */
    SqString & GenerateBetween(SQInteger p, SQInteger n, LightObj & ctx, Function & fn)
    {
        if (static_cast< size_t >(p) >= mS.size())
        {
            STHROWF("Invalid container index ({} >= {})", p, mS.size());
        }
        else if (static_cast< size_t >(p + n) >= mS.size())
        {
            STHROWF("Invalid container index ({} >= {})", p + n, mS.size());
        }
        for (n = (p + n); p <= n; ++p)
        {
            auto ret = fn.Eval(ctx);
            // Extract the value from object and assign it
            mS.at(static_cast< size_t >(p)) = ret.Cast< String::value_type >();
        }
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Sort the elements from the container.
    */
    SqString & Sort()
    {
        std::sort(mS.begin(), mS.end());
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Check if the elements from the container are sorted.
    */
    SQMOD_NODISCARD bool IsSorted()
    {
        return std::is_sorted(mS.begin(), mS.end());
    }

    /* --------------------------------------------------------------------------------------------
     * Shuffle the elements from the container.
    */
    SqString & Shuffle()
    {
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(mS.begin(), mS.end(), g);
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Remove white-space at the start of the string.
    */
    SqString & TrimLeft()
    {
        mS.erase(mS.begin(), std::find_if(mS.begin(), mS.end(), [](auto c) { return !std::isspace(c); }));
        // Allow chaining
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Remove white-space at the end of the string.
    */
    SqString & TrimRight()
    {
        mS.erase(std::find_if(mS.rbegin(), mS.rend(), [](auto c) { return !std::isspace(c); }).base(), mS.end());
        // Allow chaining
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Remove white-space either side of the string.
    */
    SqString & Trim()
    {
        return TrimLeft().TrimRight();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the string with white space trimmed from either side of the string.
    */
    SQMOD_NODISCARD LightObj Trimmed()
    {
        const auto sb = mS.find_first_not_of(" \n\t\v\b\r\f\a");
        // Is it all white-space?
        if (sb != String::npos)
        {
            const auto se = mS.find_last_not_of(" \n\t\v\b\r\f\a");
            // Return the portion of the string that isn't surrounded by white-space
            return LightObj(mS.data() + sb, se - sb);
        }
        // Return an empty string
        return LightObj(_SC(""), SQInteger(0));
    }

    /* --------------------------------------------------------------------------------------------
     * Convert the string to lower-case.
    */
    SqString & ToLower()
    {
        std::transform(mS.begin(), mS.end(), mS.begin(), [](auto c) { return std::tolower(c); });
        // Allow chaining
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Convert the string to upper-case.
    */
    SqString & ToUpper()
    {
        std::transform(mS.begin(), mS.end(), mS.begin(), [](auto c) { return std::toupper(c); });
        // Allow chaining
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the string as lower-case.
    */
    SQMOD_NODISCARD LightObj GetLower() const
    {
        SqString s(mS);
        std::transform(s.mS.begin(), s.mS.end(), s.mS.begin(), [&](auto c) { return std::tolower(c); });
        return LightObj(s.mS.data(), static_cast< SQInteger >(s.mS.size()));
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the string as upper-case.
    */
    SQMOD_NODISCARD LightObj GetUpper() const
    {
        SqString s(mS);
        std::transform(s.mS.begin(), s.mS.end(), s.mS.begin(), [&](auto c) { return std::toupper(c); });
        return LightObj(s.mS.data(), static_cast< SQInteger >(s.mS.size()));
    }

    /* --------------------------------------------------------------------------------------------
     * Swap the letter case.
    */
    SqString & SwapCase()
    {
        std::transform(mS.begin(), mS.end(), mS.begin(), [](auto c) {
            return std::islower(c) ? std::toupper(c) : std::tolower(c);
        });
        // Allow chaining
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Perform a case-sensitive comparison against another string.
    */
    SQMOD_NODISCARD SQInteger Compare(SqString & o) const
    {
        return mS.compare(o.mS);
    }

    /* --------------------------------------------------------------------------------------------
     * Perform a case-insensitive comparison against another string.
    */
    SQMOD_NODISCARD SQInteger CompareI(SqString & o) const
    {
        return Poco::icompare(mS, mS);
    }

    /* --------------------------------------------------------------------------------------------
     * Check if string contains another sub-string.
    */
    SQMOD_NODISCARD bool Contains(StackStrF & s) const
    {
        return mS.find(s.mPtr, 0, s.GetSize()) != String::npos;
    }

    /* --------------------------------------------------------------------------------------------
     * Checks if the string begins with the given prefix.
    */
    SQMOD_NODISCARD bool StartsWith(StackStrF & s) const
    {
        return mS.size() >= s.GetSize() && mS.compare(0, s.GetSize(), s.mPtr) == 0;
    }

    /* --------------------------------------------------------------------------------------------
     * Checks if the string ends with the given suffix.
    */
    SQMOD_NODISCARD bool EndsWith(StackStrF & s) const
    {
        return mS.size() >= s.GetSize() && mS.compare(mS.size() - s.GetSize(), s.GetSize(), s.mPtr) == 0;
    }

    /* --------------------------------------------------------------------------------------------
     * Finds the first substring equal to s.
    */
    SQMOD_NODISCARD SQInteger Find(StackStrF & s) const { return FindFrom(0, s); }
    SQMOD_NODISCARD SQInteger FindFrom(SQInteger p, StackStrF & s) const
    {
        return static_cast< SQInteger >(mS.find(s.mPtr, static_cast< size_t >(p), s.GetSize()));
    }

    /* --------------------------------------------------------------------------------------------
     * Finds the first character equal c (treated as a single-character substring).
    */
    SQMOD_NODISCARD SQInteger Locate(String::value_type c) const { return LocateFrom(c, 0); }
    SQMOD_NODISCARD SQInteger LocateFrom(String::value_type c, SQInteger p) const
    {
        return static_cast< SQInteger >(mS.find(c, static_cast< size_t >(p)));
    }

    /* --------------------------------------------------------------------------------------------
     * Finds the last substring equal to s.
    */
    SQMOD_NODISCARD SQInteger RFind(StackStrF & s) const { return RFindFrom(0, s); }
    SQMOD_NODISCARD SQInteger RFindFrom(SQInteger p, StackStrF & s) const
    {
        return static_cast< SQInteger >(mS.rfind(s.mPtr, static_cast< size_t >(p), s.GetSize()));
    }

    /* --------------------------------------------------------------------------------------------
     * Finds the last character equal to c. (treated as a single-character substring).
    */
    SQMOD_NODISCARD SQInteger RLocate(String::value_type c) const { return RLocateFrom(c, 0); }
    SQMOD_NODISCARD SQInteger RLocateFrom(String::value_type c, SQInteger p) const
    {
        return static_cast< SQInteger >(mS.rfind(c, static_cast< size_t >(p)));
    }

    /* --------------------------------------------------------------------------------------------
     * Finds the first character equal to one of the characters in s.
    */
    SQMOD_NODISCARD SQInteger FindFirstOf(StackStrF & s) const { return FindFirstOfFrom(0, s); }
    SQMOD_NODISCARD SQInteger FindFirstOfFrom(SQInteger p, StackStrF & s) const
    {
        return static_cast< SQInteger >(mS.find_first_of(s.mPtr, static_cast< size_t >(p), s.GetSize()));
    }

    /* --------------------------------------------------------------------------------------------
     * Finds the first character equal to c.
    */
    SQMOD_NODISCARD SQInteger LocateFirstOf(String::value_type c) const { return LocateFirstOfFrom(c, 0); }
    SQMOD_NODISCARD SQInteger LocateFirstOfFrom(String::value_type c, SQInteger p) const
    {
        return static_cast< SQInteger >(mS.find_first_of(c, static_cast< size_t >(p)));
    }

    /* --------------------------------------------------------------------------------------------
     * Finds the first character equal to none of characters in s.
    */
    SQMOD_NODISCARD SQInteger FindFirstNotOf(StackStrF & s) const { return FindFirstNotOfFrom(0, s); }
    SQMOD_NODISCARD SQInteger FindFirstNotOfFrom(SQInteger p, StackStrF & s) const
    {
        return static_cast< SQInteger >(mS.find_first_not_of(s.mPtr, static_cast< size_t >(p), s.GetSize()));
    }

    /* --------------------------------------------------------------------------------------------
     * Finds the first character not equal to c.
    */
    SQMOD_NODISCARD SQInteger LocateFirstNotOf(String::value_type c) const { return LocateFirstNotOfFrom(c, 0); }
    SQMOD_NODISCARD SQInteger LocateFirstNotOfFrom(String::value_type c, SQInteger p) const
    {
        return static_cast< SQInteger >(mS.find_first_not_of(c, static_cast< size_t >(p)));
    }

    /* --------------------------------------------------------------------------------------------
     * Finds the last character equal to one of characters in s.
    */
    SQMOD_NODISCARD SQInteger FindLastOf(StackStrF & s) const { return FindLastOfFrom(0, s); }
    SQMOD_NODISCARD SQInteger FindLastOfFrom(SQInteger p, StackStrF & s) const
    {
        return static_cast< SQInteger >(mS.find_last_of(s.mPtr, static_cast< size_t >(p), s.GetSize()));
    }

    /* --------------------------------------------------------------------------------------------
     * Finds the last character equal to c.
    */
    SQMOD_NODISCARD SQInteger LocateLastOf(String::value_type c) const { return LocateLastOfFrom(c, 0); }
    SQMOD_NODISCARD SQInteger LocateLastOfFrom(String::value_type c, SQInteger p) const
    {
        return static_cast< SQInteger >(mS.find_last_of(c, static_cast< size_t >(p)));
    }

    /* --------------------------------------------------------------------------------------------
     * Finds the last character equal to none of characters in s.
    */
    SQMOD_NODISCARD SQInteger FindLastNotOf(StackStrF & s) const { return FindLastNotOfFrom(0, s); }
    SQMOD_NODISCARD SQInteger FindLastNotOfFrom(SQInteger p, StackStrF & s) const
    {
        return static_cast< SQInteger >(mS.find_last_not_of(s.mPtr, static_cast< size_t >(p), s.GetSize()));
    }

    /* --------------------------------------------------------------------------------------------
     * Finds the last character not equal to c.
    */
    SQMOD_NODISCARD SQInteger LocateLastNotOf(String::value_type c) const { return LocateLastNotOfFrom(c, 0); }
    SQMOD_NODISCARD SQInteger LocateLastNotOfFrom(String::value_type c, SQInteger p) const
    {
        return static_cast< SQInteger >(mS.find_last_not_of(c, static_cast< size_t >(p)));
    }

    /* --------------------------------------------------------------------------------------------
     * Duplicate the string a certain amount of times.
    */
    SqString & Repeat(SQInteger n)
    {
        if (!mS.empty())
        {
            const size_t len = mS.size();
            // Replicate the same string n times
            while (n--)
            {
                mS.append(mS.data(), len);
            }
        }
        return *this; // Allow chaining
    }

    /* --------------------------------------------------------------------------------------------
     * Duplicate the string a certain amount of times with a specific delimiter.
    */
    SqString & Repeat_(SQInteger n, StackStrF & d)
    {
        const size_t len = mS.size();
        // Replicate the same string n times
        while (n--)
        {
            mS.append(mS.data(), len);
            // If there's no string then the delimiter will be repeated
            mS.append(d.mPtr, static_cast< size_t >(d.mLen));
        }
        return *this; // Allow chaining
    }

    /* --------------------------------------------------------------------------------------------
     * Replace all occurrences of `from` with `to`, starting at position p.
    */
    SqString & Replace(StackStrF & f, StackStrF & t) { return Replace_(0, f, t); }
    SqString & Replace_(SQInteger p, StackStrF & f, StackStrF & t)
    {
        if (!f.mLen)
        {
            STHROWF("Cannot replace empty string");
        }
        else if (!mS.empty())
        {
            Poco::replaceInPlace(mS, f.mPtr, t.mPtr, static_cast< size_t >(p));
        }
        return *this; // Allow chaining
    }


    /* --------------------------------------------------------------------------------------------
     * Replace all occurrences of `from` with `to`, starting at position p.
    */
    SqString & Change(String::value_type f, String::value_type t) { return Change_(0, f, t); }
    SqString & Change_(SQInteger p, String::value_type f, String::value_type t)
    {
        Poco::replaceInPlace(mS, f, t, static_cast< size_t >(p));
        return *this; // Allow chaining
    }

    /* --------------------------------------------------------------------------------------------
     * Remove all occurrences of characters from s.
    */
    SqString & Remove(StackStrF & s)
    {
        if (!s.mLen)
        {
            STHROWF("Cannot remove empty string");
        }
        for (SQInteger i = 0; !mS.empty() && i < s.mLen; ++i)
        {
            mS.erase(std::remove(mS.begin(), mS.end(), s.mPtr[i]), mS.end());
        }
        return *this; // Allow chaining
    }

    /* --------------------------------------------------------------------------------------------
     * Remove all occurrences of character c.
    */
    SqString & Eliminate(String::value_type c)
    {
        if (!mS.empty() )
        {
            mS.erase(std::remove(mS.begin(), mS.end(), c), mS.end());
        }
        return *this; // Allow chaining
    }

    /* --------------------------------------------------------------------------------------------
     * Generate a hash of the string using the 32-bit using the Fnv1a algorithm.
    */
    SQMOD_NODISCARD SQInteger GetFnv1a32() const
    {
        return static_cast< SQInteger >(FnvHash32(reinterpret_cast< FnvHashData >(mS.data()), mS.size()));
    }

    /* --------------------------------------------------------------------------------------------
     * Generate a hash of the string using the 64-bit using the Fnv1a algorithm.
    */
    SQMOD_NODISCARD SQInteger GetFnv1a64() const
    {
        return static_cast< SQInteger >(FnvHash64(reinterpret_cast< FnvHashData >(mS.data()), mS.size()));
    }

    /* --------------------------------------------------------------------------------------------
     * Compute the Levenshtein distance between two strings.
    */
    SQMOD_NODISCARD SQInteger GetLevenshtein(SqString & o) const;
};

} // Namespace:: SqMod
