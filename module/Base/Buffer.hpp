#ifndef _BASE_BUFFER_HPP_
#define _BASE_BUFFER_HPP_

// ------------------------------------------------------------------------------------------------
#include <cmath>
#include <cassert>
#include <cstdarg>

// ------------------------------------------------------------------------------------------------
#include <utility>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
class Memory;
class Buffer;

/* ------------------------------------------------------------------------------------------------
 * A counted reference to a memory manager instance.
*/
class MemRef
{
private:

    // --------------------------------------------------------------------------------------------
    static MemRef s_Mem;

    // --------------------------------------------------------------------------------------------
    typedef unsigned int Counter;

    // --------------------------------------------------------------------------------------------
    Memory*     m_Ptr; // The memory manager instance.
    Counter*    m_Ref; // Reference count to the managed instance.

    /* --------------------------------------------------------------------------------------------
     * Grab a strong reference to a memory manager.
    */
    void Grab();

    /* --------------------------------------------------------------------------------------------
     * Drop a strong reference to a memory manager.
    */
    void Drop();

public:

    /* --------------------------------------------------------------------------------------------
     * Get a reference to the global memory manager instance.
    */
    static const MemRef & Get();

    /* --------------------------------------------------------------------------------------------
     * Default constructor (null).
    */
    MemRef()
        : m_Ptr(s_Mem.m_Ptr)
        , m_Ref(s_Mem.m_Ref)
    {
        Grab();
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    MemRef(const MemRef & o)
        : m_Ptr(o.m_Ptr)
        , m_Ref(o.m_Ref)

    {
        Grab();
    }

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    MemRef(MemRef && o)
        : m_Ptr(o.m_Ptr), m_Ref(o.m_Ref)

    {
        o.m_Ptr = nullptr;
        o.m_Ref = nullptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~MemRef()
    {
        Drop();
    }

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    MemRef & operator = (const MemRef & o)
    {
        if (m_Ptr != o.m_Ptr)
        {
            Drop();
            m_Ptr = o.m_Ptr;
            m_Ref = o.m_Ref;
            Grab();
        }
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    MemRef & operator = (MemRef && o)
    {
        if (m_Ptr != o.m_Ptr)
        {
            Drop();
            m_Ptr = o.m_Ptr;
            m_Ref = o.m_Ref;
            o.m_Ptr = nullptr;
            o.m_Ref = nullptr;
        }
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Perform an equality comparison between two memory managers.
    */
    bool operator == (const MemRef & o) const
    {
        return (m_Ptr == o.m_Ptr);
    }

    /* --------------------------------------------------------------------------------------------
     * Perform an inequality comparison between two memory managers.
    */
    bool operator != (const MemRef & o) const
    {
        return (m_Ptr != o.m_Ptr);
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to boolean for use in boolean operations.
    */
    operator bool () const
    {
        return m_Ptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Member operator for dereferencing the managed pointer.
    */
    Memory * operator -> () const
    {
        assert(m_Ptr);
        return m_Ptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Indirection operator for obtaining a reference of the managed pointer.
    */
    Memory & operator * () const
    {
        assert(m_Ptr);
        return *m_Ptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Release the reference to the managed instance.
    */
    void Reset()
    {
        if (m_Ptr)
        {
            Drop();
        }
    }
};

// ------------------------------------------------------------------------------------------------
void ThrowMemExcept(const char * msg, ...);

/* ------------------------------------------------------------------------------------------------
 * Reusable and re-scalable buffer memory for quick memory allocations.
*/
class Buffer
{
public:

    // --------------------------------------------------------------------------------------------
    typedef char            Value; // The type of value used to represent a byte.

    // --------------------------------------------------------------------------------------------
    typedef Value &         Reference; // A reference to the stored value type.
    typedef const Value &   ConstRef; // A const reference to the stored value type.

    // --------------------------------------------------------------------------------------------
    typedef Value *         Pointer; // A pointer to the stored value type.
    typedef const Value *   ConstPtr; // A const pointer to the stored value type.

    // --------------------------------------------------------------------------------------------
    typedef unsigned int    SzType; // The type used to represent size in general.

private:

    /* --------------------------------------------------------------------------------------------
     * Construct and take ownership of the specified buffer.
    */
    Buffer(Pointer & ptr, SzType & cap, SzType & cur, const MemRef & mem)
        : m_Ptr(ptr)
        , m_Cap(cap)
        , m_Cur(cur)
        , m_Mem(mem)
    {
        ptr = nullptr;
        cap = 0;
        cur = 0;
    }

    /* --------------------------------------------------------------------------------------------
     * Round a size value.
    */
    template < typename T > static inline SzType RoundSz(T n)
    {
        return static_cast< SzType >(floor(n));
    }

public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor. (null)
    */
    Buffer()
        : m_Ptr(nullptr)
        , m_Cap(0)
        , m_Cur(0)
        , m_Mem(MemRef::Get())
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Explicit size constructor.
    */
    Buffer(SzType size)
        : m_Ptr(nullptr)
        , m_Cap(0)
        , m_Cur(0)
        , m_Mem(MemRef::Get())
    {
        Request(size < 8 ? 8 : size);
    }

    /* --------------------------------------------------------------------------------------------
     * Explicit size and cursor position constructor.
    */
    Buffer(SzType size, SzType pos)
        : m_Ptr(nullptr)
        , m_Cap(0)
        , m_Cur(0)
        , m_Mem(MemRef::Get())
    {
        Request(size < 8 ? 8 : size);
        Move(pos);
    }

    /* --------------------------------------------------------------------------------------------
     * Explicit size and buffer constructor.
    */
    Buffer(ConstPtr data, SzType size)
        : m_Ptr(nullptr)
        , m_Cap(0)
        , m_Cur(0)
        , m_Mem(MemRef::Get())
    {
        Request(size < 8 ? 8 : size);
        m_Cur += Write(m_Cur, data, size);
    }

    /* --------------------------------------------------------------------------------------------
     * Explicit size, data and cursor position constructor.
    */
    Buffer(ConstPtr data, SzType size, SzType pos)
        : m_Ptr(nullptr)
        , m_Cap(0)
        , m_Cur(0)
        , m_Mem(MemRef::Get())
    {
        Request(size < 8 ? 8 : size);
        m_Cur += Write(m_Cur, data, size);
        Move(pos);
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    Buffer(const Buffer & o);

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    Buffer(Buffer && o)
        : m_Ptr(o.m_Ptr)
        , m_Cap(o.m_Cap)
        , m_Cur(o.m_Cur)
        , m_Mem(o.m_Mem)
    {
        o.m_Ptr = nullptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~Buffer();

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    Buffer & operator = (const Buffer & o);

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    Buffer & operator = (Buffer && o)
    {
        if (m_Ptr != o.m_Ptr)
        {
            if (m_Ptr)
            {
                Release();
            }
            m_Ptr = o.m_Ptr;
            m_Cap = o.m_Cap;
            m_Cur = o.m_Cur;
            m_Mem = o.m_Mem;
            o.m_Ptr = nullptr;
        }
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Equality comparison operator.
    */
    bool operator == (const Buffer & o) const
    {
        return (m_Cap == o.m_Cap);
    }

    /* --------------------------------------------------------------------------------------------
     * Inequality comparison operator.
    */
    bool operator != (const Buffer & o) const
    {
        return (m_Cap != o.m_Cap);
    }

    /* --------------------------------------------------------------------------------------------
     * Less than comparison operator.
    */
    bool operator < (const Buffer & o) const
    {
        return (m_Cap < o.m_Cap);
    }

    /* --------------------------------------------------------------------------------------------
     * Greater than comparison operator.
    */
    bool operator > (const Buffer & o) const
    {
        return (m_Cap > o.m_Cap);
    }

    /* --------------------------------------------------------------------------------------------
     * Less than or equal comparison operator.
    */
    bool operator <= (const Buffer & o) const
    {
        return (m_Cap <= o.m_Cap);
    }

    /* --------------------------------------------------------------------------------------------
     * Greater than or equal comparison operator.
    */
    bool operator >= (const Buffer & o) const
    {
        return (m_Cap >= o.m_Cap);
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to boolean.
    */
    operator bool () const
    {
        return (m_Ptr != nullptr);
    }

    /* --------------------------------------------------------------------------------------------
     * Negation operator.
    */
    bool operator ! () const
    {
        return (!m_Ptr);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the internal buffer.
    */
    Pointer Data()
    {
        return m_Ptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the internal buffer.
    */
    ConstPtr Data() const
    {
        return m_Ptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the internal buffer casted as a different type.
    */
    template < typename T = Value > T * Get()
    {
        return reinterpret_cast< T * >(m_Ptr);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the internal buffer casted as a different type.
    */
    template < typename T = Value > const T * Get() const
    {
        return reinterpret_cast< const T * >(m_Ptr);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve a certain element type at the specified position.
    */
    template < typename T = Value > T & At(SzType n)
    {
        // Make sure that the buffer can host at least one element of this type
        if (m_Cap < sizeof(T))
        {
            ThrowMemExcept("Buffer capacity of (%u) is unable to host an element of size (%u)",
                            m_Cap, sizeof(T));
        }
        // Make sure that the specified element is withing buffer range
        else if (n > (m_Cap - sizeof(T)))
        {
            ThrowMemExcept("Element of size (%d) at index (%u) is out of buffer capacity (%u)",
                            sizeof(T), n, m_Cap);
        }
        // Return the requested element
        return *reinterpret_cast< T * >(m_Ptr + n);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve a certain element type at the specified position.
    */
    template < typename T = Value > const T & At(SzType n) const
    {
        // Make sure that the buffer can host at least one element of this type
        if (m_Cap < sizeof(T))
        {
            ThrowMemExcept("Buffer capacity of (%u) is unable to host an element of size (%u)",
                            m_Cap, sizeof(T));
        }
        // Make sure that the specified element is withing buffer range
        else if (n > (m_Cap - sizeof(T)))
        {
            ThrowMemExcept("Element of size (%d) at index (%u) is out of buffer capacity (%u)",
                            sizeof(T), n, m_Cap);
        }
        // Return the requested element
        return *reinterpret_cast< const T * >(m_Ptr + n);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the internal buffer casted as a different type.
    */
    template < typename T = Value > T * Begin()
    {
        return reinterpret_cast< T * >(m_Ptr);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the internal buffer casted as a different type.
    */
    template < typename T = Value > const T * Begin() const
    {
        return reinterpret_cast< const T * >(m_Ptr);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the internal buffer casted as a different type.
    */
    template < typename T = Value > T * End()
    {
        return reinterpret_cast< T * >(m_Ptr) + (m_Cap / sizeof(T));
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the internal buffer casted as a different type.
    */
    template < typename T = Value > const T * End() const
    {
        return reinterpret_cast< const T * >(m_Ptr) + (m_Cap / sizeof(T));
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the element at the front of the buffer.
    */
    template < typename T = Value > T & Front()
    {
        // Make sure that the buffer can host at least one element of this type
        if (m_Cap < sizeof(T))
        {
            ThrowMemExcept("Buffer capacity of (%u) is unable to host an element of size (%u)",
                            m_Cap, sizeof(T));
        }
        // Return the requested element
        return *reinterpret_cast< T * >(m_Ptr);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the element at the front of the buffer.
    */
    template < typename T = Value > const T & Front() const
    {
        // Make sure that the buffer can host at least one element of this type
        if (m_Cap < sizeof(T))
        {
            ThrowMemExcept("Buffer capacity of (%u) is unable to host an element of size (%u)",
                            m_Cap, sizeof(T));
        }
        // Return the requested element
        return *reinterpret_cast< T * >(m_Ptr);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the element after the first element in the buffer.
    */
    template < typename T = Value > T & Next()
    {
        // Make sure that the buffer can host at least two elements of this type
        if (m_Cap < (sizeof(T) * 2))
        {
            ThrowMemExcept("Buffer capacity of (%u) is unable to host two elements of size (%u)",
                            m_Cap, sizeof(T));
        }
        // Return the requested element
        return *reinterpret_cast< T * >(m_Ptr + sizeof(T));
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the element after the first element in the buffer.
    */
    template < typename T = Value > const T & Next() const
    {
        // Make sure that the buffer can host at least two elements of this type
        if (m_Cap < (sizeof(T) * 2))
        {
            ThrowMemExcept("Buffer capacity of (%u) is unable to host two elements of size (%u)",
                            m_Cap, sizeof(T));
        }
        // Return the requested element
        return *reinterpret_cast< const T * >(m_Ptr + sizeof(T));
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the element at the back of the buffer.
    */
    template < typename T = Value > T & Back()
    {
        // Make sure that the buffer can host at least one element of this type
        if (m_Cap < sizeof(T))
        {
            ThrowMemExcept("Buffer capacity of (%u) is unable to host an element of size (%u)",
                            m_Cap, sizeof(T));
        }
        // Return the requested element
        return *reinterpret_cast< T * >(m_Ptr + (m_Cap - sizeof(T)));
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the element at the back of the buffer.
    */
    template < typename T = Value > const T & Back() const
    {
        // Make sure that the buffer can host at least one element of this type
        if (m_Cap < sizeof(T))
        {
            ThrowMemExcept("Buffer capacity of (%u) is unable to host an element of size (%u)",
                            m_Cap, sizeof(T));
        }
        // Return the requested element
        return *reinterpret_cast< const T * >(m_Ptr + (m_Cap - sizeof(T)));
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the element before the last element in the buffer.
    */
    template < typename T = Value > T & Prev()
    {
        // Make sure that the buffer can host at least two elements of this type
        if (m_Cap < (sizeof(T) * 2))
        {
            ThrowMemExcept("Buffer capacity of (%u) is unable to host two elements of size (%u)",
                            m_Cap, sizeof(T));
        }
        // Return the requested element
        return *reinterpret_cast< T * >(m_Ptr + (m_Cap - (sizeof(T) * 2)));
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the element before the last element in the buffer.
    */
    template < typename T = Value > const T & Prev() const
    {
        // Make sure that the buffer can host at least two elements of this type
        if (m_Cap < (sizeof(T) * 2))
        {
            ThrowMemExcept("Buffer capacity of (%u) is unable to host two elements of size (%u)",
                            m_Cap, sizeof(T));
        }
        // Return the requested element
        return *reinterpret_cast< const T * >(m_Ptr + (m_Cap - (sizeof(T) * 2)));
    }

    /* --------------------------------------------------------------------------------------------
     * Reposition the edit cursor to the specified number of elements ahead.
    */
    template < typename T = Value > void Advance(SzType n)
    {
        // Do we need to scale the buffer?
        if ((m_Cur + (n * sizeof(T))) > m_Cap)
        {
            Grow(m_Cur + (n * sizeof(T)));
        }
        // Advance to the specified position
        m_Cur += (n * sizeof(T));
    }

    /* --------------------------------------------------------------------------------------------
     * Reposition the edit cursor to the specified number of elements behind.
    */
    template < typename T = Value > void Retreat(SzType n)
    {
        // Can we move that much backward?
        if ((n * sizeof(T)) <= m_Cur)
        {
            m_Cur -= (n * sizeof(T));
        }
        // Just got to the beginning
        else
        {
            m_Cur = 0;
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Reposition the edit cursor to a fixed position within the buffer.
    */
    template < typename T = Value > void Move(SzType n)
    {
        // Do we need to scale the buffer?
        if ((n * sizeof(T)) > m_Cap)
        {
            Grow(n * sizeof(T));
        }
        // Move to the specified position
        m_Cur = (n * sizeof(T));
    }

    /* --------------------------------------------------------------------------------------------
     * Append a value to the current cursor location and advance the cursor.
    */
    template < typename T = Value > void Push(T v)
    {
        // Do we need to scale the buffer?
        if ((m_Cur + sizeof(T)) > m_Cap)
        {
            Grow(m_Cap + sizeof(T));
        }
        // Assign the specified value
        *reinterpret_cast< T * >(m_Ptr + m_Cur) = v;
        // Move to the next element
        m_Cur += sizeof(T);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the element at the cursor position.
    */
    template < typename T = Value > T & Cursor()
    {
        // Make sure that at least one element of this type exists after the cursor
        if ((m_Cur + sizeof(T)) > m_Cap)
        {
            ThrowMemExcept("Element of size (%u) starting at (%u) exceeds buffer capacity (%u)",
                            sizeof(T), m_Cur, m_Cap);
        }
        // Return the requested element
        return *reinterpret_cast< T * >(m_Ptr + m_Cur);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the element at the cursor position.
    */
    template < typename T = Value > const T & Cursor() const
    {
        // Make sure that at least one element of this type exists after the cursor
        if ((m_Cur + sizeof(T)) > m_Cap)
        {
            ThrowMemExcept("Element of size (%u) starting at (%u) exceeds buffer capacity (%u)",
                            sizeof(T), m_Cur, m_Cap);
        }
        // Return the requested element
        return *reinterpret_cast< const T * >(m_Ptr + m_Cur);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the element before the cursor position.
    */
    template < typename T = Value > T & Before()
    {
        // The cursor must have at least one element of this type behind
        if (m_Cur < sizeof(T))
        {
            ThrowMemExcept("Cannot read an element of size (%u) before the cursor at (%u)",
                            sizeof(T), m_Cur);
        }
        // Return the requested element
        return *reinterpret_cast< T * >(m_Ptr + (m_Cur - sizeof(T)));
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the element before the cursor position.
    */
    template < typename T = Value > const T & Before() const
    {
        // The cursor must have at least one element of this type behind
        if (m_Cur < sizeof(T))
        {
            ThrowMemExcept("Cannot read an element of size (%u) before the cursor at (%u)",
                            sizeof(T), m_Cur);
        }
        // Return the requested element
        return *reinterpret_cast< const T * >(m_Ptr + (m_Cur - sizeof(T)));
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the element after the cursor position.
    */
    template < typename T = Value > T & After()
    {
        // Make sure that the buffer can host at least one element of this type
        if (m_Cap < sizeof(T))
        {
            ThrowMemExcept("Buffer capacity of (%u) is unable to host an element of size (%u)",
                            m_Cap, sizeof(T));
        }
        // There must be buffer left for at least two elements of this type after the cursor
        else if ((m_Cur + (sizeof(T) * 2)) > m_Cap)
        {
            ThrowMemExcept("Element of size (%u) starting at (%u) exceeds buffer capacity (%u)",
                            sizeof(T), m_Cur + sizeof(T), m_Cap);
        }
        // Return the requested element
        return *reinterpret_cast< T * >(m_Ptr + m_Cur + sizeof(T));
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the element after the cursor position.
    */
    template < typename T = Value > const T & After() const
    {
        // Make sure that the buffer can host at least one element of this type
        if (m_Cap < sizeof(T))
        {
            ThrowMemExcept("Buffer capacity of (%u) is unable to host an element of size (%u)",
                            m_Cap, sizeof(T));
        }
        // There must be buffer left for at least two elements of this type after the cursor
        else if ((m_Cur + (sizeof(T) * 2)) > m_Cap)
        {
            ThrowMemExcept("Element of size (%u) starting at (%u) exceeds buffer capacity (%u)",
                            sizeof(T), m_Cur + sizeof(T), m_Cap);
        }
        // Return the requested element
        return *reinterpret_cast< const T * >(m_Ptr + m_Cur + sizeof(T));
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve maximum elements it can hold for a certain type.
    */
    template < typename T = Value > static SzType Max()
    {
        return static_cast< SzType >(0xFFFFFFFF / sizeof(T));
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the current buffer capacity in element count.
    */
    template < typename T = Value > SzType Size() const
    {
        return static_cast< SzType >(m_Cap / sizeof(T));
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the current buffer capacity in byte count.
    */
    SzType Capacity() const
    {
        return m_Cap;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the current buffer capacity in byte count.
    */
    template < typename T = Value > SzType CapacityAs() const
    {
        return static_cast< SzType >(m_Cap / sizeof(T));
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the current position of the cursor in the buffer.
    */
    SzType Position() const
    {
        return m_Cur;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the current position of the cursor in the buffer.
    */
    template < typename T = Value > SzType PositionAs() const
    {
        return static_cast< SzType >(m_Cur / sizeof(T));
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the amount of unused buffer after the edit cursor.
    */
    SzType Remaining() const
    {
        return m_Cap - m_Cur;
    }

    /* --------------------------------------------------------------------------------------------
     * Grow the size of the internal buffer by the specified amount of bytes.
    */
    void Grow(SzType n);

    /* --------------------------------------------------------------------------------------------
     * Makes sure there is enough capacity to hold the specified element count.
    */
    template < typename T = Value > Buffer Adjust(SzType n)
    {
        // Do we meet the minimum size?
        if (n < 8)
        {
            n = 8; // Adjust to minimum size
        }
        // See if the requested capacity doesn't exceed the limit
        if (n > Max< T >())
        {
            ThrowMemExcept("Requested buffer of (%u) elements exceeds the (%u) limit", n, Max< T >());
        }
        // Is there an existing buffer?
        else if (n && !m_Cap)
        {
            Request(n * sizeof(T)); // Request the memory
        }
        // Should the size be increased?
        else if (n > m_Cap)
        {
            // Backup the current memory
            Buffer bkp(m_Ptr, m_Cap, m_Cur, m_Mem);
            // Request the memory
            Request(n * sizeof(T));
            // Return the backup
            return std::move(bkp);
        }
        // Return an empty buffer
        return Buffer();
    }

    /* --------------------------------------------------------------------------------------------
     * Release the managed memory.
    */
    void Reset()
    {
        if (m_Ptr)
        {
            Release();
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Release the managed memory and manager.
    */
    void ResetAll()
    {
        if (m_Ptr)
        {
            Release();
        }
        m_Mem.Reset();
    }

    /* --------------------------------------------------------------------------------------------
     * Swap the contents of two buffers.
    */
    void Swap(Buffer & o)
    {
        Pointer p = m_Ptr;
        SzType n = m_Cap;
        m_Ptr = o.m_Ptr;
        m_Cap = o.m_Cap;
        o.m_Ptr = p;
        o.m_Cap = n;
    }

    /* --------------------------------------------------------------------------------------------
     * Write a portion of a buffer to the internal buffer.
    */
    SzType Write(SzType pos, ConstPtr data, SzType size);

    /* --------------------------------------------------------------------------------------------
     * Write another buffer to the internal buffer.
    */
    SzType Write(SzType pos, const Buffer & b)
    {
        return Write(pos, b.m_Ptr, b.m_Cur);
    }

    /* --------------------------------------------------------------------------------------------
     * Write a formatted string to the internal buffer.
    */
    SzType WriteF(SzType pos, const char * fmt, ...);

    /* --------------------------------------------------------------------------------------------
     * Write a formatted string to the internal buffer.
    */
    SzType WriteF(SzType pos, const char * fmt, va_list args);

    /* --------------------------------------------------------------------------------------------
     * Write a string to the internal buffer.
    */
    SzType WriteS(SzType pos, const char * str);

    /* --------------------------------------------------------------------------------------------
     * Write a portion of a string to the internal buffer.
    */
    SzType WriteS(SzType pos, const char * str, SzType size)
    {
        return Write(pos, str, size);
    }

    /* --------------------------------------------------------------------------------------------
     * Append a portion of a buffer to the internal buffer.
    */
    void Append(ConstPtr data, SzType size)
    {
        m_Cur += Write(m_Cur, data, size);
    }

    /* --------------------------------------------------------------------------------------------
     * Append another buffer to the internal buffer.
    */
    void Append(const Buffer & b)
    {
        m_Cur += Write(m_Cur, b.m_Ptr, b.m_Cur);
    }

    /* --------------------------------------------------------------------------------------------
     * Append a formatted string to the internal buffer.
    */
    void AppendF(const char * fmt, ...);

    /* --------------------------------------------------------------------------------------------
     * Append a formatted string to the internal buffer.
    */
    void AppendF(const char * fmt, va_list args)
    {
        m_Cur += WriteF(m_Cur, fmt, args);
    }

    /* --------------------------------------------------------------------------------------------
     * Append a string to the internal buffer.
    */
    void AppendS(const char * str);

    /* --------------------------------------------------------------------------------------------
     * Append a portion of a string to the internal buffer.
    */
    void AppendS(const char * str, SzType size)
    {
        m_Cur += Write(m_Cur, str, size);
    }

protected:

    /* --------------------------------------------------------------------------------------------
     * Request the memory specified in the capacity.
    */
    void Request(SzType n);

    /* --------------------------------------------------------------------------------------------
     * Release the managed memory buffer.
    */
    void Release();

private:

    // --------------------------------------------------------------------------------------------
    Pointer     m_Ptr; /* Pointer to the memory buffer. */
    SzType      m_Cap; /* The total size of the buffer. */
    SzType      m_Cur; /* The buffer edit cursor. */

    // --------------------------------------------------------------------------------------------
    MemRef      m_Mem; /* Reference to the associated memory manager. */
};

} // Namespace:: SqMod

#endif // _BASE_BUFFER_HPP_
