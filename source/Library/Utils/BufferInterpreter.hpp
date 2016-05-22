#ifndef _LIBRARY_UTILS_BUFFERINTERPRETER_HPP_
#define _LIBRARY_UTILS_BUFFERINTERPRETER_HPP_

// ------------------------------------------------------------------------------------------------
#include "Base/Shared.hpp"
#include "Base/Buffer.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
class BufferWrapper;

/* ------------------------------------------------------------------------------------------------
 * Used internally to obtain a reference to the memory buffer without including the wrapper header.
*/
const SharedPtr< Buffer > & GetBufferBufferRef(const BufferWrapper & buffer);

/* ------------------------------------------------------------------------------------------------
 * Utility class used to interpret a memory buffer in different ways.
*/
template < typename T > class BufferInterpreter
{
private:

    // --------------------------------------------------------------------------------------------
    typedef SharedPtr< Buffer > SRef; // Strong reference type to the interpreted memory buffer.
    typedef WeakPtr< Buffer >   WRef; // Weak reference type to the interpreted memory buffer.

    // --------------------------------------------------------------------------------------------
    WRef m_Buffer; // The interpreted memory buffer.

public:

    // --------------------------------------------------------------------------------------------
    typedef T               Value; // The type of value used to represent a byte.

    // --------------------------------------------------------------------------------------------
    typedef Value &         Reference; // A reference to the stored value type.
    typedef const Value &   ConstRef; // A const reference to the stored value type.

    // --------------------------------------------------------------------------------------------
    typedef Value *         Pointer; // A pointer to the stored value type.
    typedef const Value *   ConstPtr; // A const pointer to the stored value type.

    // --------------------------------------------------------------------------------------------
    typedef Buffer::SzType  SzType; // The type used to represent size in general.

protected:

    /* --------------------------------------------------------------------------------------------
     * Attempt to obtain a strong reference to the memory buffer at all costs.
    */
    SRef Validate() const
    {
        // Did the buffer that we reference expired?
        if (m_Buffer.Expired())
        {
            STHROWF("Invalid memory buffer reference");
        }
        // Obtain a strong reference to it
        return m_Buffer.Lock();
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to obtain a strong reference to a valid memory buffer at all costs.
    */
    SRef ValidateDeeper() const
    {
        // Did the buffer that we reference expired?
        if (m_Buffer.Expired())
        {
            STHROWF("Invalid memory buffer reference");
        }
        // Obtain a strong reference to it
        SRef ref = m_Buffer.Lock();
        // Validate the buffer itself
        if (!(*ref))
        {
            STHROWF("Invalid memory buffer");
        }
        // Return the reference
        return ref;
    }

public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor. (null)
    */
    BufferInterpreter()
        : m_Buffer()
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    BufferInterpreter(const WRef & ref)
        : m_Buffer(ref)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    BufferInterpreter(const SRef & ref)
        : m_Buffer(ref)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    BufferInterpreter(const BufferInterpreter & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    BufferInterpreter(BufferInterpreter && o) = default;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~BufferInterpreter() = default;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    BufferInterpreter & operator = (const BufferInterpreter & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    BufferInterpreter & operator = (BufferInterpreter && o) = default;

    /* --------------------------------------------------------------------------------------------
     * Retrieve a reference to the managed memory buffer.
    */
    const WRef & GetRef() const
    {
        return m_Buffer;
    }

    /* --------------------------------------------------------------------------------------------
     * Assign a different memory buffer to interpret from.
    */
    void UseBuffer(const BufferWrapper & buffer)
    {
        m_Buffer = GetBufferBufferRef(buffer);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve a certain element type at the specified position.
    */
    T Get(SzType n) const
    {
        // Acquire a reference to the memory buffer
        SRef b(Validate());
        // Are we out of the memory buffer range?
        if (n >= b->Size< T >())
        {
            STHROWF("Index (%u) is out of bounds (%u)", n, b->Size< T >());
        }
        // Return the requested element
        return b->At< T >(n);
    }

    /* --------------------------------------------------------------------------------------------
     * Modify a certain element type at the specified position.
    */
    void Set(SzType n, T v)
    {
        // Acquire a reference to the memory buffer
        SRef b(Validate());
        // Are we out of the memory buffer range?
        if (n >= b->Size< T >())
        {
            STHROWF("Index (%u) is out of bounds (%u)", n, b->Size< T >());
        }
        // Return the requested element
        b->At< T >(n) = v;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the element at the front of the buffer.
    */
    T GetFront() const
    {
        // Acquire a reference to the memory buffer
        SRef b(Validate());
        // Are we out of the memory buffer range?
        if (b->Capacity() < sizeof(T))
        {
            STHROWF("Value size (%u starting at 0) is out of bounds (%u)",
                        sizeof(T), b->Capacity());
        }
        // Return the requested element
        return b->Front< T >();
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the element at the front of the buffer.
    */
    void SetFront(T v)
    {
        // Acquire a reference to the memory buffer
        SRef b(Validate());
        // Are we out of the memory buffer range?
        if (b->Capacity() < sizeof(T))
        {
            STHROWF("Value size (%u starting at 0) is out of bounds (%u)",
                        sizeof(T), b->Capacity());
        }
        // Return the requested element
        b->Front< T >() = v;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the element after the first element in the buffer.
    */
    T GetNext() const
    {
        // Acquire a reference to the memory buffer
        SRef b(Validate());
        // Are we out of the memory buffer range?
        if (b->Capacity() < (sizeof(T) * 2))
        {
            STHROWF("Value size (%u starting at %u) is out of bounds (%u)",
                        sizeof(T), sizeof(T), b->Capacity());
        }
        // Return the requested element
        return b->Next< T >();
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the element after the first element in the buffer.
    */
    void SetNext(T v)
    {
        // Acquire a reference to the memory buffer
        SRef b(Validate());
        // Are we out of the memory buffer range?
        if (b->Capacity() < (sizeof(T) * 2))
        {
            STHROWF("Value size (%u starting at %u) is out of bounds (%u)",
                        sizeof(T), sizeof(T), b->Capacity());
        }
        // Return the requested element
        b->Next< T >() = v;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the element at the back of the buffer.
    */
    T GetBack() const
    {
        // Acquire a reference to the memory buffer
        SRef b(Validate());
        // Are we out of the memory buffer range?
        if (b->Capacity() < sizeof(T))
        {
            STHROWF("Value size (%u starting at 0) is out of bounds (%u)",
                        sizeof(T), b->Capacity());
        }
        // Return the requested element
        return b->Back< T >();
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the element at the back of the buffer.
    */
    void SetBack(T v)
    {
        // Acquire a reference to the memory buffer
        SRef b(Validate());
        // Are we out of the memory buffer range?
        if (b->Capacity() < sizeof(T))
        {
            STHROWF("Value size (%u starting at 0) is out of bounds (%u)",
                        sizeof(T), b->Capacity());
        }
        // Return the requested element
        b->Back< T >() = v;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the element before the last element in the buffer.
    */
    T GetPrev() const
    {
        // Acquire a reference to the memory buffer
        SRef b(Validate());
        // Are we out of the memory buffer range?
        if (b->Capacity() < (sizeof(T) * 2))
        {
            STHROWF("Value size (%u starting at %u) is out of bounds (%u)",
                        sizeof(T), sizeof(T), b->Capacity());
        }
        // Return the requested element
        return b->Prev< T >();
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the element before the last element in the buffer.
    */
    void SetPrev(T v)
    {
        // Acquire a reference to the memory buffer
        SRef b(Validate());
        // Are we out of the memory buffer range?
        if (b->Capacity() < (sizeof(T) * 2))
        {
            STHROWF("Value size (%u starting at %u) is out of bounds (%u)",
                        sizeof(T), sizeof(T), b->Capacity());
        }
        // Return the requested element
        b->Prev< T >() = v;
    }

    /* --------------------------------------------------------------------------------------------
     * Reposition the edit cursor to the specified number of elements ahead.
    */
    void Advance(SzType n)
    {
        // Acquire a reference to the memory buffer
        SRef b(Validate());
        // Perform the requested operation
        b->Advance< T >(n);
    }

    /* --------------------------------------------------------------------------------------------
     * Reposition the edit cursor to the specified number of elements behind.
    */
    void Retreat(SzType n)
    {
        // Acquire a reference to the memory buffer
        SRef b(Validate());
        // Perform the requested operation
        b->Retreat< T >(n);
    }

    /* --------------------------------------------------------------------------------------------
     * Reposition the edit cursor to a fixed position within the buffer.
    */
    void Move(SzType n)
    {
        // Acquire a reference to the memory buffer
        SRef b(Validate());
        // Perform the requested operation
        b->Move< T >(n);
    }

    /* --------------------------------------------------------------------------------------------
     * Append a value to the current cursor location and advance the cursor.
    */
    void Push(T v)
    {
        // Acquire a reference to the memory buffer
        SRef b(Validate());
        // Perform the requested operation
        b->Push< T >(v);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the element at the cursor position.
    */
    T GetCursor() const
    {
        // Acquire a reference to the memory buffer
        SRef b(Validate());
        // Are we out of the memory buffer range?
        if (b->Position< T >() >= b->Size< T >())
        {
            STHROWF("Value size (%u starting at %u) is out of bounds (%u)",
                        sizeof(T), b->Position(), b->Capacity());
        }
        // Return the requested element
        return b->Cursor< T >();
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the element at the cursor position.
    */
    void SetCursor(T v)
    {
        // Acquire a reference to the memory buffer
        SRef b(Validate());
        // Are we out of the memory buffer range?
        if (b->Position< T >() >= b->Size< T >())
        {
            STHROWF("Value size (%u starting at %u) is out of bounds (%u)",
                        sizeof(T), b->Position(), b->Capacity());
        }
        // Return the requested element
        b->Cursor< T >() = v;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the element before the cursor position.
    */
    T GetBefore() const
    {
        // Acquire a reference to the memory buffer
        SRef b(Validate());
        // Are we out of the memory buffer range?
        if (b->Position() < sizeof(T))
        {
            STHROWF("Value size (%u starting at 0) is out of bounds (%u)",
                        sizeof(T), b->Capacity());
        }
        // Return the requested element
        return b->Before< T >();
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the element before the cursor position.
    */
    void SetBefore(T v)
    {
        // Acquire a reference to the memory buffer
        SRef b(Validate());
        // Are we out of the memory buffer range?
        if (b->Position() < sizeof(T))
        {
            STHROWF("Value size (%u starting at 0) is out of bounds (%u)",
                        sizeof(T), b->Capacity());
        }
        // Return the requested element
        b->Before< T >() = v;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the element after the cursor position.
    */
    T GetAfter() const
    {
        // Acquire a reference to the memory buffer
        SRef b(Validate());
        // Are we out of the memory buffer range?
        if (b->Capacity() < sizeof(T) || (b->Position() + sizeof(T)) > (b->Capacity() - sizeof(T)))
        {
            STHROWF("Value size (%u starting at %u) is out of bounds (%u)",
                        sizeof(T), b->Position(), b->Capacity());
        }
        // Return the requested element
        return b->After< T >();
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the element after the cursor position.
    */
    void SetAfter(T v)
    {
        // Acquire a reference to the memory buffer
        SRef b(Validate());
        // Are we out of the memory buffer range?
        if (b->Capacity() < sizeof(T) || (b->Position() + sizeof(T)) > (b->Capacity() - sizeof(T)))
        {
            STHROWF("Value size (%u starting at %u) is out of bounds (%u)",
                        sizeof(T), b->Position(), b->Capacity());
        }
        // Return the requested element
        b->After< T >() = v;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve maximum elements it can hold for a certain type.
    */
    SzType GetMax() const
    {
        return Buffer::Max< T >();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the current buffer capacity in element count.
    */
    SzType GetSize() const
    {
        return Validate()->template Size< T >();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the current buffer capacity in byte count.
    */
    SzType GetCapacity() const
    {
        return Validate()->Capacity();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the current position of the cursor in the buffer.
    */
    SzType GetPosition() const
    {
        return Validate()->template Position< T >();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the amount of unused buffer after the edit cursor.
    */
    SzType GetRemaining() const
    {
        return Validate()->template Remaining< T >();
    }

    /* --------------------------------------------------------------------------------------------
     * Grow the size of the internal buffer by the specified amount of bytes.
    */
    void Grow(SzType n)
    {
        return Validate()->Grow(n * sizeof(T));
    }

    /* --------------------------------------------------------------------------------------------
     * Makes sure there is enough capacity to hold the specified element count.
    */
    void Adjust(SzType n)
    {
        // Acquire a reference to the memory buffer
        SRef b(Validate());
        // Attempt to perform the requested operation
        try
        {
            Buffer bkp(b->Adjust(n * sizeof(T)));
            // Copy the data into the new buffer
            b->Write(0, bkp.Data(), bkp.Capacity());
            b->Move(bkp.Position());
        }
        catch (const std::exception & e)
        {
            STHROWF("%s", e.what()); // Re-package
        }
    }
};

} // Namespace:: SqMod

#endif // _LIBRARY_UTILS_BUFFERINTERPRETER_HPP_
