#pragma once

// ------------------------------------------------------------------------------------------------
#include "Core/Utility.hpp"

// ------------------------------------------------------------------------------------------------
#include <algorithm>

// ------------------------------------------------------------------------------------------------
#include <Poco/RegularExpression.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * 
*/
struct PcRegExMatch : public Poco::RegularExpression::Match
{
    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    PcRegExMatch() noexcept
        : Poco::RegularExpression::Match{}
    {
    }

    /* --------------------------------------------------------------------------------------------
     * Offset constructor.
    */
    explicit PcRegExMatch(SQInteger offset) noexcept
        : Poco::RegularExpression::Match{static_cast< std::string::size_type >(offset), 0}
    {
    }

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    PcRegExMatch(SQInteger offset, SQInteger length) noexcept
        : Poco::RegularExpression::Match{
            static_cast< std::string::size_type >(offset),
            static_cast< std::string::size_type >(length)
        }
    {
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    PcRegExMatch(const PcRegExMatch & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    PcRegExMatch(PcRegExMatch && o) noexcept = default;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    PcRegExMatch & operator = (const PcRegExMatch & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    PcRegExMatch & operator = (PcRegExMatch && o) noexcept = default;

    /* --------------------------------------------------------------------------------------------
     * Retrieve offset.
    */
    SQMOD_NODISCARD SQInteger GetOffset() const noexcept
    {
        return static_cast< SQInteger >(Poco::RegularExpression::Match::offset);
    }

    /* --------------------------------------------------------------------------------------------
     * Modify offset.
    */
    void SetOffset(SQInteger value) noexcept
    {
        Poco::RegularExpression::Match::offset = static_cast< std::string::size_type >(value);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve length.
    */
    SQMOD_NODISCARD SQInteger GetLength() const noexcept
    {
        return static_cast< SQInteger >(Poco::RegularExpression::Match::length);
    }

    /* --------------------------------------------------------------------------------------------
     * Modify length.
    */
    void SetLength(SQInteger value) noexcept
    {
        Poco::RegularExpression::Match::length = static_cast< std::string::size_type >(value);
    }
};

/* ------------------------------------------------------------------------------------------------
 * 
*/
struct PcRegExMatches
{
    using List = Poco::RegularExpression::MatchVec;

    /* --------------------------------------------------------------------------------------------
     * Internal RegularExpression instance.
    */
    List m_List;

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    PcRegExMatches() = default;

    /* --------------------------------------------------------------------------------------------
     * Copy list constructor.
    */
    explicit PcRegExMatches(const List & l) // NOLINT(modernize-pass-by-value)
        : m_List{l}
    {
    }

    /* --------------------------------------------------------------------------------------------
     * Move list constructor.
    */
    explicit PcRegExMatches(List && m) noexcept
        : m_List{std::move(m)}
    {
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    PcRegExMatches(const PcRegExMatches & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    PcRegExMatches(PcRegExMatches && o) noexcept = default;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    PcRegExMatches & operator = (const PcRegExMatches & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    PcRegExMatches & operator = (PcRegExMatches && o) noexcept = default;

    /* --------------------------------------------------------------------------------------------
     * Make sure an index is within range and return the container. Container must exist.
    */
    List & ValidIdx(SQInteger i)
    {
        if (static_cast< size_t >(i) >= m_List.size())
        {
            STHROWF("Invalid RegEx match list index({})", i);
        }
        return m_List;
    }

    /* --------------------------------------------------------------------------------------------
     * Make sure an index is within range and return the container. Container must exist.
    */
    SQMOD_NODISCARD const List & ValidIdx(SQInteger i) const
    {
        if (static_cast< size_t >(i) >= m_List.size())
        {
            STHROWF("Invalid RegEx match list index({})", i);
        }
        return m_List;
    }

    /* --------------------------------------------------------------------------------------------
     * Make sure a container instance is populated, then return it.
    */
    SQMOD_NODISCARD List & ValidPop()
    {
        if (m_List.empty())
        {
            STHROWF("RegEx match list container is empty");
        }
        return m_List;
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
        return m_List.back();
    }

    /* --------------------------------------------------------------------------------------------
     * Check if the container has no elements.
    */
    SQMOD_NODISCARD bool Empty() const
    {
        return m_List.empty();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the number of elements in the container.
    */
    SQMOD_NODISCARD SQInteger Size() const
    {
        return static_cast< SQInteger >(m_List.size());
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the number of elements that the container has currently allocated space for.
    */
    SQMOD_NODISCARD SQInteger Capacity() const
    {
        return static_cast< SQInteger >(m_List.capacity());
    }

    /* --------------------------------------------------------------------------------------------
     * Increase the capacity of the container to a value that's greater or equal to the one specified.
    */
    PcRegExMatches & Reserve(SQInteger n)
    {
        m_List.reserve(ClampL< SQInteger, size_t >(n));
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Request the removal of unused capacity.
    */
    void Compact()
    {
        m_List.shrink_to_fit();
    }

    /* --------------------------------------------------------------------------------------------
     * Erase all elements from the container.
    */
    void Clear()
    {
        m_List.clear();
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
        m_List.erase(ValidIdx(i).begin() + static_cast< size_t >(i));
    }

    /* --------------------------------------------------------------------------------------------
     * Erase a certain amount of elements starting from a specific position.
    */
    void EraseFrom(SQInteger i, SQInteger n)
    {
        m_List.erase(ValidIdx(i).begin() + static_cast< size_t >(i),
                        ValidIdx(i + n).begin() + static_cast< size_t >(i + n));
    }

    /* --------------------------------------------------------------------------------------------
     * Iterate all values through a functor.
    */
    void Each(Function & fn) const
    {
        for (const auto & e : m_List)
        {
            fn.Execute(static_cast< SQInteger >(e.offset), static_cast< SQInteger >(e.length));
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Iterate values in range through a functor.
    */
    void EachRange(SQInteger p, SQInteger n, Function & fn) const
    {
        std::for_each(ValidIdx(p).begin() + static_cast< size_t >(p),
                      ValidIdx(p + n).begin() + static_cast< size_t >(p + n),
        [&](List::const_reference & e) {
            fn.Execute(static_cast< SQInteger >(e.offset), static_cast< SQInteger >(e.length));
        });
    }

    /* --------------------------------------------------------------------------------------------
     * Iterate all values through a functor until stopped (i.e false is returned).
    */
    void While(Function & fn) const
    {
        for (const auto & e : m_List)
        {
            auto ret = fn.Eval(static_cast< SQInteger >(e.offset), static_cast< SQInteger >(e.length));
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
            auto ret = fn.Eval(static_cast< SQInteger >(itr->offset), static_cast< SQInteger >(itr->length));
            // (null || true) == continue & false == break
            if (!ret.IsNull() || !ret.template Cast< bool >())
            {
                break;
            }
        }
    }
};

/* ------------------------------------------------------------------------------------------------
 * A class for working with regular expressions.
 * Implemented using PCRE, the Perl Compatible Regular Expressions library. (see http://www.pcre.org)
*/
struct PcRegEx
{
    /* --------------------------------------------------------------------------------------------
     * Internal RegularExpression instance.
    */
    Poco::RegularExpression m_Rx;

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    explicit PcRegEx(StackStrF & str)
        : m_Rx(str.ToStr())
    {
    }

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    PcRegEx(int options, StackStrF & str)
        : m_Rx(str.ToStr(), options)
    {
    }

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    PcRegEx(int options, bool study, StackStrF & str)
        : m_Rx(str.ToStr(), options, study)
    {
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    PcRegEx(const PcRegEx & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move constructor. (disabled)
    */
    PcRegEx(PcRegEx && o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator. (disabled)
    */
    PcRegEx & operator = (const PcRegEx & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator. (disabled)
    */
    PcRegEx & operator = (PcRegEx && o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Matches the given subject string against the pattern.
     * Returns the position of the first captured sub-string in m.
     * If no part of the subject matches the pattern, m.offset is std::string::npos and m.length is 0.
     * Returns the number of matches. Throws a exception in case of an error.
    */
    SQMOD_NODISCARD int MatchFirst(PcRegExMatch & m, StackStrF & s) const
    {
        return m_Rx.match(s.ToStr(), m);
    }
    SQMOD_NODISCARD int MatchFirst_(int f, PcRegExMatch & m, StackStrF & s) const
    {
        return m_Rx.match(s.ToStr(), m, f);
    }

    /* --------------------------------------------------------------------------------------------
     * Matches the given subject string against the pattern.
     * Returns the position of the first captured sub-string in m.
     * If no part of the subject matches the pattern, m.offset is std::string::npos and m.length is 0.
     * Returns the number of matches. Throws a exception in case of an error.
    */
    SQMOD_NODISCARD int MatchFirstFrom(SQInteger o, PcRegExMatch & m, StackStrF & s) const
    {
        return m_Rx.match(s.ToStr(), static_cast< std::string::size_type >(o), m);
    }
    SQMOD_NODISCARD int MatchFirstFrom_(int f, SQInteger o, PcRegExMatch & m, StackStrF & s) const
    {
        return m_Rx.match(s.ToStr(), static_cast< std::string::size_type >(o), m, f);
    }

    /* --------------------------------------------------------------------------------------------
     * Matches the given subject string against the pattern.
     * The first entry in m contains the position of the captured sub-string.
     * The following entries identify matching subpatterns. See the PCRE documentation for a more detailed explanation.
     * If no part of the subject matches the pattern, m is empty.
     * Returns the number of matches. Throws a exception in case of an error.
    */
    SQMOD_NODISCARD int Match(PcRegExMatches & m, StackStrF & s) const
    {
        return m_Rx.match(s.ToStr(), 0, m.m_List);
    }
    SQMOD_NODISCARD int Match_(int f, PcRegExMatches & m, StackStrF & s) const
    {
        return m_Rx.match(s.ToStr(), 0, m.m_List, f);
    }

    /* --------------------------------------------------------------------------------------------
     * Matches the given subject string against the pattern.
     * The first entry in m contains the position of the captured sub-string.
     * The following entries identify matching subpatterns. See the PCRE documentation for a more detailed explanation.
     * If no part of the subject matches the pattern, m is empty.
     * Returns the number of matches. Throws a exception in case of an error.
    */
    SQMOD_NODISCARD int MatchFrom(SQInteger o, PcRegExMatches & m, StackStrF & s) const
    {
        return m_Rx.match(s.ToStr(), static_cast< std::string::size_type >(o), m.m_List);
    }
    SQMOD_NODISCARD int MatchFrom_(int f, SQInteger o, PcRegExMatches & m, StackStrF & s) const
    {
        return m_Rx.match(s.ToStr(), static_cast< std::string::size_type >(o), m.m_List, f);
    }

    /* --------------------------------------------------------------------------------------------
     * Returns true if and only if the subject matches the regular expression.
     * Internally, this method sets the RE_ANCHORED and RE_NOTEMPTY options for matching,
     * which means that the empty string will never match and the pattern is treated as if it starts with a ^.
    */
    SQMOD_NODISCARD bool Matches(StackStrF & s) const
    {
        return m_Rx.match(s.ToStr());
    }
    SQMOD_NODISCARD bool Matches_(SQInteger o, StackStrF & s) const
    {
        return m_Rx.match(s.ToStr(), static_cast< std::string::size_type >(o));
    }
    SQMOD_NODISCARD bool MatchesEx(int f, SQInteger o, StackStrF & s) const
    {
        return m_Rx.match(s.ToStr(), static_cast< std::string::size_type >(o), f);
    }
};

} // Namespace:: SqMod
