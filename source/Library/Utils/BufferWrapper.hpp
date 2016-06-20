#ifndef _LIBRARY_UTILS_BUFFERWRAPPER_HPP_
#define _LIBRARY_UTILS_BUFFERWRAPPER_HPP_

// ------------------------------------------------------------------------------------------------
#include "Base/Shared.hpp"
#include "Base/Buffer.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
template < typename T > class BufferInterpreter;

/* ------------------------------------------------------------------------------------------------
 * Squirrel wrapper for the shared buffer class.
*/
class BufferWrapper
{
private:

    // --------------------------------------------------------------------------------------------
    typedef SharedPtr< Buffer > SRef; // Strong reference type to the managed memory buffer.
    typedef WeakPtr< Buffer >   WRef; // Weak reference type to the managed memory buffer.

    // --------------------------------------------------------------------------------------------
    SRef m_Buffer; // The managed memory buffer.

public:

    // --------------------------------------------------------------------------------------------
    typedef Buffer::Value   Value; // The type of value used to represent a byte.

    // --------------------------------------------------------------------------------------------
    typedef Value &         Reference; // A reference to the stored value type.
    typedef const Value &   ConstRef; // A const reference to the stored value type.

    // --------------------------------------------------------------------------------------------
    typedef Value *         Pointer; // A pointer to the stored value type.
    typedef const Value *   ConstPtr; // A const pointer to the stored value type.

    // --------------------------------------------------------------------------------------------
    typedef Buffer::SzType  SzType; // The type used to represent size in general.

public:

    /* --------------------------------------------------------------------------------------------
     * Create a memory buffer with the requested size.
    */
    static Object Create(SzType n);

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    BufferWrapper(const SRef & ref)
        : m_Buffer(ref)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Buffer constructor.
    */
    BufferWrapper(const Buffer & b)
        : m_Buffer(new Buffer(b))
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Buffer constructor.
    */
    BufferWrapper(Buffer && b)
        : m_Buffer(new Buffer(std::move(b)))
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    BufferWrapper(const BufferWrapper & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    BufferWrapper(BufferWrapper && o) = default;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~BufferWrapper() = default;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    BufferWrapper & operator = (const BufferWrapper & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    BufferWrapper & operator = (BufferWrapper && o) = default;

    /* --------------------------------------------------------------------------------------------
     * Retrieve a reference to the managed memory buffer.
    */
    const SRef & GetRef() const
    {
        return m_Buffer;
    }

    /* --------------------------------------------------------------------------------------------
     * Validate the managed memory buffer reference.
    */
    void Validate() const
    {
        // Do we even point to a valid buffer?
        if (!m_Buffer)
        {
            STHROWF("Invalid memory buffer reference");
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Validate the managed memory buffer reference and the buffer itself.
    */
    void ValidateDeeper() const
    {
        // Do we even point to a valid buffer?
        if (!m_Buffer)
        {
            STHROWF("Invalid memory buffer reference");
        }
        // Validate the buffer itself
        else if (!(*m_Buffer))
        {
            STHROWF("Invalid memory buffer");
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve a certain element type at the specified position.
    */
    Value Get(SzType n) const
    {
        // Validate the managed buffer reference
        Validate();
        // Are we out of the memory buffer range?
        if (n >= m_Buffer->Size())
        {
            STHROWF("Index (%u) is out of bounds (%u)", n, m_Buffer->Size());
        }
        // Return the requested element
        return m_Buffer->At(n);
    }

    /* --------------------------------------------------------------------------------------------
     * Modify a certain element type at the specified position.
    */
    void Set(SzType n, Value v)
    {
        // Validate the managed buffer reference
        Validate();
        // Are we out of the memory buffer range?
        if (n >= m_Buffer->Size())
        {
            STHROWF("Index (%u) is out of bounds (%u)", n, m_Buffer->Size());
        }
        // Return the requested element
        m_Buffer->At(n) = v;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the element at the front of the buffer.
    */
    Value GetFront() const
    {
        // Validate the managed buffer reference
        Validate();
        // Are we out of the memory buffer range?
        if (m_Buffer->Capacity() < sizeof(Value))
        {
            STHROWF("Value size (%u starting at 0) is out of bounds (%u)",
                        sizeof(Value), m_Buffer->Capacity());
        }
        // Return the requested element
        return m_Buffer->Front();
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the element at the front of the buffer.
    */
    void SetFront(Value v)
    {
        // Validate the managed buffer reference
        Validate();
        // Are we out of the memory buffer range?
        if (m_Buffer->Capacity() < sizeof(Value))
        {
            STHROWF("Value size (%u starting at 0) is out of bounds (%u)",
                        sizeof(Value), m_Buffer->Capacity());
        }
        // Return the requested element
        m_Buffer->Front() = v;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the element after the first element in the buffer.
    */
    Value GetNext() const
    {
        // Validate the managed buffer reference
        Validate();
        // Are we out of the memory buffer range?
        if (m_Buffer->Capacity() < (sizeof(Value) * 2))
        {
            STHROWF("Value size (%u starting at %u) is out of bounds (%u)",
                        sizeof(Value), sizeof(Value), m_Buffer->Capacity());
        }
        // Return the requested element
        return m_Buffer->Next();
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the element after the first element in the buffer.
    */
    void SetNext(Value v)
    {
        // Validate the managed buffer reference
        Validate();
        // Are we out of the memory buffer range?
        if (m_Buffer->Capacity() < (sizeof(Value) * 2))
        {
            STHROWF("Value size (%u starting at %u) is out of bounds (%u)",
                        sizeof(Value), sizeof(Value), m_Buffer->Capacity());
        }
        // Return the requested element
        m_Buffer->Next() = v;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the element at the back of the buffer.
    */
    Value GetBack() const
    {
        // Validate the managed buffer reference
        Validate();
        // Are we out of the memory buffer range?
        if (m_Buffer->Capacity() < sizeof(Value))
        {
            STHROWF("Value size (%u starting at 0) is out of bounds (%u)",
                        sizeof(Value), m_Buffer->Capacity());
        }
        // Return the requested element
        return m_Buffer->Back();
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the element at the back of the buffer.
    */
    void SetBack(Value v)
    {
        // Validate the managed buffer reference
        Validate();
        // Are we out of the memory buffer range?
        if (m_Buffer->Capacity() < sizeof(Value))
        {
            STHROWF("Value size (%u starting at 0) is out of bounds (%u)",
                        sizeof(Value), m_Buffer->Capacity());
        }
        // Return the requested element
        m_Buffer->Back() = v;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the element before the last element in the buffer.
    */
    Value GetPrev() const
    {
        // Validate the managed buffer reference
        Validate();
        // Are we out of the memory buffer range?
        if (m_Buffer->Capacity() < (sizeof(Value) * 2))
        {
            STHROWF("Value size (%u starting at %u) is out of bounds (%u)",
                        sizeof(Value), sizeof(Value), m_Buffer->Capacity());
        }
        // Return the requested element
        return m_Buffer->Prev();
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the element before the last element in the buffer.
    */
    void SetPrev(Value v)
    {
        // Validate the managed buffer reference
        Validate();
        // Are we out of the memory buffer range?
        if (m_Buffer->Capacity() < (sizeof(Value) * 2))
        {
            STHROWF("Value size (%u starting at %u) is out of bounds (%u)",
                        sizeof(Value), sizeof(Value), m_Buffer->Capacity());
        }
        // Return the requested element
        m_Buffer->Prev() = v;
    }

    /* --------------------------------------------------------------------------------------------
     * Reposition the edit cursor to the specified number of elements ahead.
    */
    void Advance(SzType n)
    {
        // Validate the managed buffer reference
        Validate();
        // Perform the requested operation
        m_Buffer->Advance(n);
    }

    /* --------------------------------------------------------------------------------------------
     * Reposition the edit cursor to the specified number of elements behind.
    */
    void Retreat(SzType n)
    {
        // Validate the managed buffer reference
        Validate();
        // Perform the requested operation
        m_Buffer->Retreat(n);
    }

    /* --------------------------------------------------------------------------------------------
     * Reposition the edit cursor to a fixed position within the buffer.
    */
    void Move(SzType n)
    {
        // Validate the managed buffer reference
        Validate();
        // Perform the requested operation
        m_Buffer->Move(n);
    }

    /* --------------------------------------------------------------------------------------------
     * Append a value to the current cursor location and advance the cursor.
    */
    void Push(Value v)
    {
        // Validate the managed buffer reference
        Validate();
        // Perform the requested operation
        m_Buffer->Push(v);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the element at the cursor position.
    */
    Value GetCursor() const
    {
        // Validate the managed buffer reference
        Validate();
        // Are we out of the memory buffer range?
        if (m_Buffer->Position() >= m_Buffer->Size())
        {
            STHROWF("Value size (%u starting at %u) is out of bounds (%u)",
                        sizeof(Value), m_Buffer->Position(), m_Buffer->Capacity());
        }
        // Return the requested element
        return m_Buffer->Cursor();
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the element at the cursor position.
    */
    void SetCursor(Value v)
    {
        // Validate the managed buffer reference
        Validate();
        // Are we out of the memory buffer range?
        if (m_Buffer->Position() >= m_Buffer->Size())
        {
            STHROWF("Value size (%u starting at %u) is out of bounds (%u)",
                        sizeof(Value), m_Buffer->Position(), m_Buffer->Capacity());
        }
        // Return the requested element
        m_Buffer->Cursor() = v;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the element before the cursor position.
    */
    Value GetBefore() const
    {
        // Validate the managed buffer reference
        Validate();
        // Are we out of the memory buffer range?
        if (m_Buffer->Position() < sizeof(Value))
        {
            STHROWF("Value size (%u starting at 0) is out of bounds (%u)",
                        sizeof(Value), m_Buffer->Capacity());
        }
        // Return the requested element
        return m_Buffer->Before();
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the element before the cursor position.
    */
    void SetBefore(Value v)
    {
        // Validate the managed buffer reference
        Validate();
        // Are we out of the memory buffer range?
        if (m_Buffer->Position() < sizeof(Value))
        {
            STHROWF("Value size (%u starting at 0) is out of bounds (%u)",
                        sizeof(Value), m_Buffer->Capacity());
        }
        // Return the requested element
        m_Buffer->Before() = v;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the element after the cursor position.
    */
    Value GetAfter() const
    {
        // Validate the managed buffer reference
        Validate();
        // Are we out of the memory buffer range?
        if (m_Buffer->Capacity() < sizeof(Value) ||
            (m_Buffer->Position() + sizeof(Value)) > (m_Buffer->Capacity() - sizeof(Value)))
        {
            STHROWF("Value size (%u starting at %u) is out of bounds (%u)",
                        sizeof(Value), m_Buffer->Position(), m_Buffer->Capacity());
        }
        // Return the requested element
        return m_Buffer->After();
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the element after the cursor position.
    */
    void SetAfter(Value v)
    {
        // Validate the managed buffer reference
        Validate();
        // Are we out of the memory buffer range?
        if (m_Buffer->Capacity() < sizeof(Value) ||
            (m_Buffer->Position() + sizeof(Value)) > (m_Buffer->Capacity() - sizeof(Value)))
        {
            STHROWF("Value size (%u starting at %u) is out of bounds (%u)",
                        sizeof(Value), m_Buffer->Position(), m_Buffer->Capacity());
        }
        // Return the requested element
        m_Buffer->After() = v;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve maximum elements it can hold for a certain type.
    */
    SzType GetMax() const
    {
        return Buffer::Max();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the current buffer capacity in element count.
    */
    SzType GetSize() const
    {
        // Validate the managed buffer reference
        Validate();
        // Return the requested information
        return m_Buffer->Size();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the current buffer capacity in byte count.
    */
    SzType GetCapacity() const
    {
        // Validate the managed buffer reference
        Validate();
        // Return the requested information
        return m_Buffer->Capacity();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the current position of the cursor in the buffer.
    */
    SzType GetPosition() const
    {
        // Validate the managed buffer reference
        Validate();
        // Return the requested information
        return m_Buffer->Position();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the amount of unused buffer after the edit cursor.
    */
    SzType GetRemaining() const
    {
        // Validate the managed buffer reference
        Validate();
        // Return the requested information
        return m_Buffer->Remaining();
    }

    /* --------------------------------------------------------------------------------------------
     * Grow the size of the internal buffer by the specified amount of bytes.
    */
    void Grow(SzType n)
    {
        // Validate the managed buffer reference
        Validate();
        // Perform the requested operation
        return m_Buffer->Grow(n * sizeof(Value));
    }

    /* --------------------------------------------------------------------------------------------
     * Makes sure there is enough capacity to hold the specified element count.
    */
    void Adjust(SzType n)
    {
        // Validate the managed buffer reference
        Validate();
        // Attempt to perform the requested operation
        try
        {
            Buffer bkp(m_Buffer->Adjust(n * sizeof(Value)));
            // Copy the data into the new buffer
            m_Buffer->Write(0, bkp.Data(), bkp.Capacity());
            m_Buffer->Move(bkp.Position());
        }
        catch (const std::exception & e)
        {
            STHROWF("%s", e.what()); // Re-package
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Write a 8 bit byte to the stream buffer.
    */
    void WriteByte(SQInteger val);

    /* --------------------------------------------------------------------------------------------
     * Write a 16 bit short to the stream buffer.
    */
    void WriteShort(SQInteger val);

    /* --------------------------------------------------------------------------------------------
     * Write a 32 bit integer to the stream buffer.
    */
    void WriteInt(SQInteger val);

    /* --------------------------------------------------------------------------------------------
     * Write a 32 bit float to the stream buffer.
    */
    void WriteFloat(SQFloat val);

    /* --------------------------------------------------------------------------------------------
     * Write a string to the stream buffer.
    */
    void WriteString(CSStr val);

    /* --------------------------------------------------------------------------------------------
     * Write a raw string to the stream buffer.
    */
    void WriteRawString(CSStr val);

    /* --------------------------------------------------------------------------------------------
     * Read a 8 bit byte to the stream buffer.
    */
    SQInteger ReadByte();

    /* --------------------------------------------------------------------------------------------
     * Read a 16 bit short to the stream buffer.
    */
    SQInteger ReadShort();

    /* --------------------------------------------------------------------------------------------
     * Read a 32 bit integer to the stream buffer.
    */
    SQInteger ReadInt();

    /* --------------------------------------------------------------------------------------------
     * Read a 32 bit float to the stream buffer.
    */
    SQFloat ReadFloat();

    /* --------------------------------------------------------------------------------------------
     * Read a string to the stream buffer.
    */
    Object ReadString();

    /* --------------------------------------------------------------------------------------------
     * Read a raw string to the stream buffer.
    */
    Object ReadRawString(Uint32 len);

    /* --------------------------------------------------------------------------------------------
     * Retrieve a signed 8 bit interpreter to this buffer.
    */
    BufferInterpreter< Int8 > GetInt8Interpreter() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve an unsigned 8 bit interpreter to this buffer.
    */
    BufferInterpreter< Uint8 > GetUint8Interpreter() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve a signed 16 bit interpreter to this buffer.
    */
    BufferInterpreter< Int16 > GetInt16Interpreter() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve an unsigned 16 bit interpreter to this buffer.
    */
    BufferInterpreter< Uint16 > GetUint16Interpreter() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve a signed 32 bit interpreter to this buffer.
    */
    BufferInterpreter< Int32 > GetInt32Interpreter() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve an unsigned 32 bit interpreter to this buffer.
    */
    BufferInterpreter< Uint32 > GetUint32Interpreter() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve a signed 64 bit interpreter to this buffer.
    */
    BufferInterpreter< Int64 > GetInt64Interpreter() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve an unsigned 64 bit interpreter to this buffer.
    */
    BufferInterpreter< Uint64 > GetUint64Interpreter() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve a 32 bit floating point interpreter to this buffer.
    */
    BufferInterpreter< Float32 > GetFloat32Interpreter() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve a 64 bit floating point interpreter to this buffer.
    */
    BufferInterpreter< Float64 > GetFloat64Interpreter() const;
};

} // Namespace:: SqMod

#endif // _LIBRARY_UTILS_BUFFERWRAPPER_HPP_
