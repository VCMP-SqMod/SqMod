#pragma once

// ------------------------------------------------------------------------------------------------
#include "Core/Utility.hpp"

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
     * Locate the position of a value.
    */
    SQMOD_NODISCARD SQInteger Locate(String::value_type v) const
    {
        return static_cast< SQInteger >(mS.find(v));
    }

    /* --------------------------------------------------------------------------------------------
     * Locate the position of a value starting from an offset.
    */
    SQMOD_NODISCARD SQInteger LocateFrom(SQInteger p, String::value_type v) const
    {
        return static_cast< SQInteger >(mS.find(v, static_cast< size_t >(p)));
    }

    /* --------------------------------------------------------------------------------------------
     * Find the position of a sub-string.
    */
    SQMOD_NODISCARD SQInteger Find(StackStrF & s) const
    {
        return static_cast< SQInteger >(mS.find(s.mPtr));
    }

    /* --------------------------------------------------------------------------------------------
     * Find the position of a sub-string starting from an offset.
    */
    SQMOD_NODISCARD SQInteger FindFrom(SQInteger p, StackStrF & s) const
    {
        return static_cast< SQInteger >(mS.find(s.mPtr, static_cast< size_t >(p), s.GetSize()));
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
};

} // Namespace:: SqMod
