#pragma once

// ------------------------------------------------------------------------------------------------
#include "Base/Shared.hpp"
#include "Base/Buffer.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Squirrel wrapper for the shared buffer class.
*/
class SqBuffer
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
     * Default constructor.
    */
    SqBuffer()
        : m_Buffer(new Buffer())
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Allocate constructor.
    */
    SqBuffer(SQInteger n)
        : m_Buffer(new Buffer(ConvTo< SzType >::From(n)))
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Allocate constructor.
    */
    SqBuffer(SQInteger n, SQInteger c)
        : m_Buffer(new Buffer(ConvTo< SzType >::From(n), ConvTo< SzType >::From(c)))
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    SqBuffer(ConstPtr p, SQInteger n)
        : m_Buffer(new Buffer(p, ConvTo< SzType >::From(n)))
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    SqBuffer(ConstPtr p, SQInteger n, SQInteger c)
        : m_Buffer(new Buffer(p, ConvTo< SzType >::From(n), ConvTo< SzType >::From(c)))
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Reference constructor.
    */
    SqBuffer(const SRef & ref)
        : m_Buffer(ref)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Buffer constructor.
    */
    SqBuffer(const Buffer & b)
        : m_Buffer(new Buffer(b))
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Buffer constructor.
    */
    SqBuffer(Buffer && b)
        : m_Buffer(new Buffer(std::move(b)))
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    SqBuffer(const SqBuffer & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    SqBuffer(SqBuffer && o) = default;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~SqBuffer() = default;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    SqBuffer & operator = (const SqBuffer & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    SqBuffer & operator = (SqBuffer && o) = default;

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
    Value Get(SQInteger n) const
    {
        // Validate the managed buffer reference
        Validate();
        // Return the requested element
        return m_Buffer->At(ConvTo< SzType >::From(n));
    }

    /* --------------------------------------------------------------------------------------------
     * Modify a certain element type at the specified position.
    */
    void Set(SQInteger n, SQInteger v)
    {
        // Validate the managed buffer reference
        Validate();
        // Return the requested element
        m_Buffer->At(ConvTo< SzType >::From(n)) = ConvTo< Value >::From(v);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the element at the front of the buffer.
    */
    Value GetFront() const
    {
        // Validate the managed buffer reference
        Validate();
        // Return the requested element
        return m_Buffer->Front();
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the element at the front of the buffer.
    */
    void SetFront(SQInteger v)
    {
        // Validate the managed buffer reference
        Validate();
        // Return the requested element
        m_Buffer->Front() = ConvTo< Value >::From(v);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the element after the first element in the buffer.
    */
    Value GetNext() const
    {
        // Validate the managed buffer reference
        Validate();
        // Return the requested element
        return m_Buffer->Next();
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the element after the first element in the buffer.
    */
    void SetNext(SQInteger v)
    {
        // Validate the managed buffer reference
        Validate();
        // Return the requested element
        m_Buffer->Next() = ConvTo< Value >::From(v);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the element at the back of the buffer.
    */
    Value GetBack() const
    {
        // Validate the managed buffer reference
        Validate();
        // Return the requested element
        return m_Buffer->Back();
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the element at the back of the buffer.
    */
    void SetBack(SQInteger v)
    {
        // Validate the managed buffer reference
        Validate();
        // Return the requested element
        m_Buffer->Back() = ConvTo< Value >::From(v);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the element before the last element in the buffer.
    */
    Value GetPrev() const
    {
        // Validate the managed buffer reference
        Validate();
        // Return the requested element
        return m_Buffer->Prev();
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the element before the last element in the buffer.
    */
    void SetPrev(SQInteger v)
    {
        // Validate the managed buffer reference
        Validate();
        // Return the requested element
        m_Buffer->Prev() = ConvTo< Value >::From(v);
    }

    /* --------------------------------------------------------------------------------------------
     * Reposition the edit cursor to the specified number of elements ahead.
    */
    void Advance(SQInteger n)
    {
        // Validate the managed buffer reference
        Validate();
        // Perform the requested operation
        m_Buffer->Advance(ConvTo< SzType >::From(n));
    }

    /* --------------------------------------------------------------------------------------------
     * Reposition the edit cursor to the specified number of elements behind.
    */
    void Retreat(SQInteger n)
    {
        // Validate the managed buffer reference
        Validate();
        // Perform the requested operation
        m_Buffer->Retreat(ConvTo< SzType >::From(n));
    }

    /* --------------------------------------------------------------------------------------------
     * Reposition the edit cursor to a fixed position within the buffer.
    */
    void Move(SQInteger n)
    {
        // Validate the managed buffer reference
        Validate();
        // Perform the requested operation
        m_Buffer->Move(ConvTo< SzType >::From(n));
    }

    /* --------------------------------------------------------------------------------------------
     * Append a value to the current cursor location and advance the cursor.
    */
    void Push(SQInteger v)
    {
        // Validate the managed buffer reference
        Validate();
        // Perform the requested operation
        m_Buffer->Push(ConvTo< Value >::From(v));
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the element at the cursor position.
    */
    Value GetCursor() const
    {
        // Validate the managed buffer reference
        Validate();
        // Return the requested element
        return m_Buffer->Cursor();
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the element at the cursor position.
    */
    void SetCursor(SQInteger v)
    {
        // Validate the managed buffer reference
        Validate();
        // Return the requested element
        m_Buffer->Cursor() = ConvTo< Value >::From(v);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the element before the cursor position.
    */
    Value GetBefore() const
    {
        // Validate the managed buffer reference
        Validate();
        // Return the requested element
        return m_Buffer->Before();
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the element before the cursor position.
    */
    void SetBefore(SQInteger v)
    {
        // Validate the managed buffer reference
        Validate();
        // Return the requested element
        m_Buffer->Before() = ConvTo< Value >::From(v);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the element after the cursor position.
    */
    Value GetAfter() const
    {
        // Validate the managed buffer reference
        Validate();
        // Return the requested element
        return m_Buffer->After();
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the element after the cursor position.
    */
    void SetAfter(SQInteger v)
    {
        // Validate the managed buffer reference
        Validate();
        // Return the requested element
        m_Buffer->After() = ConvTo< Value >::From(v);
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
        return m_Buffer->CapacityAs< Value >();
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
    void Grow(SQInteger n)
    {
        // Validate the managed buffer reference
        Validate();
        // Perform the requested operation
        return m_Buffer->Grow(ConvTo< SzType >::From(n) * sizeof(Value));
    }

    /* --------------------------------------------------------------------------------------------
     * Makes sure there is enough capacity to hold the specified element count.
    */
    void Adjust(SQInteger n)
    {
        // Validate the managed buffer reference
        Validate();
        // Attempt to perform the requested operation
        try
        {
            Buffer bkp(m_Buffer->Adjust(ConvTo< SzType >::From(n) * sizeof(Value)));
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
     * Write a signed 8 bit integer to the buffer.
    */
    void WriteInt8(SQInteger val)
    {
        // Validate the managed buffer reference
        Validate();
        // Perform the requested operation
        m_Buffer->Push< Int8 >(ConvTo< Int8 >::From(val));
    }

    /* --------------------------------------------------------------------------------------------
     * Write an unsigned 8 bit integer to the buffer.
    */
    void WriteUint8(SQInteger val)
    {
        // Validate the managed buffer reference
        Validate();
        // Perform the requested operation
        m_Buffer->Push< Uint8 >(ConvTo< Uint8 >::From(val));
    }

    /* --------------------------------------------------------------------------------------------
     * Write a signed 16 bit integer to the buffer.
    */
    void WriteInt16(SQInteger val)
    {
        // Validate the managed buffer reference
        Validate();
        // Perform the requested operation
        m_Buffer->Push< Int16 >(ConvTo< Int16 >::From(val));
    }

    /* --------------------------------------------------------------------------------------------
     * Write an unsigned 16 bit integer to the buffer.
    */
    void WriteUint16(SQInteger val)
    {
        // Validate the managed buffer reference
        Validate();
        // Perform the requested operation
        m_Buffer->Push< Uint16 >(ConvTo< Uint16 >::From(val));
    }

    /* --------------------------------------------------------------------------------------------
     * Write a signed 32 bit integer to the buffer.
    */
    void WriteInt32(SQInteger val)
    {
        // Validate the managed buffer reference
        Validate();
        // Perform the requested operation
        m_Buffer->Push< Int32 >(ConvTo< Int32 >::From(val));
    }

    /* --------------------------------------------------------------------------------------------
     * Write an unsigned 32 bit integer to the buffer.
    */
    void WriteUint32(SQInteger val)
    {
        // Validate the managed buffer reference
        Validate();
        // Perform the requested operation
        m_Buffer->Push< Uint32 >(ConvTo< Uint32 >::From(val));
    }

    /* --------------------------------------------------------------------------------------------
     * Write a signed 64 bit integer to the buffer.
    */
    void WriteInt64(const SLongInt & val);

    /* --------------------------------------------------------------------------------------------
     * Write an unsigned 64 bit integer to the buffer.
    */
    void WriteUint64(const ULongInt & val);

    /* --------------------------------------------------------------------------------------------
     * Write a 32 bit float to the buffer.
    */
    void WriteFloat32(SQFloat val)
    {
        // Validate the managed buffer reference
        Validate();
        // Perform the requested operation
        m_Buffer->Push< Float32 >(ConvTo< Float32 >::From(val));
    }

    /* --------------------------------------------------------------------------------------------
     * Write a 64 bit float to the buffer.
    */
    void WriteFloat64(SQFloat val)
    {
        // Validate the managed buffer reference
        Validate();
        // Perform the requested operation
        m_Buffer->Push< Float64 >(ConvTo< Float64 >::From(val));
    }

    /* --------------------------------------------------------------------------------------------
     * Write a raw string to the buffer.
    */
    SQInteger WriteRawString(StackStrF & val);

    /* --------------------------------------------------------------------------------------------
     * Write a client encoded string to the buffer.
    */
    SQInteger WriteClientString(StackStrF & val);

    /* --------------------------------------------------------------------------------------------
     * Write a AABB to the buffer.
    */
    void WriteAABB(const AABB & val);

    /* --------------------------------------------------------------------------------------------
     * Write a Circle to the buffer.
    */
    void WriteCircle(const Circle & val);

    /* --------------------------------------------------------------------------------------------
     * Write a Color3 to the buffer.
    */
    void WriteColor3(const Color3 & val);

    /* --------------------------------------------------------------------------------------------
     * Write a Color4 to the buffer.
    */
    void WriteColor4(const Color4 & val);

    /* --------------------------------------------------------------------------------------------
     * Write a Quaternion to the buffer.
    */
    void WriteQuaternion(const Quaternion & val);

    /* --------------------------------------------------------------------------------------------
     * Write a Sphere to the buffer.
    */
    void WriteSphere(const Sphere &val);

    /* --------------------------------------------------------------------------------------------
     * Write a Vector2 to the buffer.
    */
    void WriteVector2(const Vector2 & val);

    /* --------------------------------------------------------------------------------------------
     * Write a Vector2i to the buffer.
    */
    void WriteVector2i(const Vector2i & val);

    /* --------------------------------------------------------------------------------------------
     * Write a Vector3 to the buffer.
    */
    void WriteVector3(const Vector3 & val);

    /* --------------------------------------------------------------------------------------------
     * Write a Vector4 to the buffer.
    */
    void WriteVector4(const Vector4 & val);

    /* --------------------------------------------------------------------------------------------
     * Write a signed 8 bit integer from the buffer.
    */
    SQInteger ReadInt8()
    {
        // Validate the managed buffer reference
        ValidateDeeper();
        // Read one element from the buffer
        const Int8 value = m_Buffer->Cursor< Int8 >();
        // Advance the buffer cursor
        m_Buffer->Advance< Int8 >(1);
        // Return the requested information
        return ConvTo< SQInteger >::From(value);
    }

    /* --------------------------------------------------------------------------------------------
     * Read an unsigned 8 bit integer from the buffer.
    */
    SQInteger ReadUint8()
    {
        // Validate the managed buffer reference
        ValidateDeeper();
        // Read one element from the buffer
        const Uint8 value = m_Buffer->Cursor< Uint8 >();
        // Advance the buffer cursor
        m_Buffer->Advance< Uint8 >(1);
        // Return the requested information
        return ConvTo< SQInteger >::From(value);
    }

    /* --------------------------------------------------------------------------------------------
     * Read a signed 16 bit integer from the buffer.
    */
    SQInteger ReadInt16()
    {
        // Validate the managed buffer reference
        ValidateDeeper();
        // Read one element from the buffer
        const Int16 value = m_Buffer->Cursor< Int16 >();
        // Advance the buffer cursor
        m_Buffer->Advance< Int16 >(1);
        // Return the requested information
        return ConvTo< SQInteger >::From(value);
    }

    /* --------------------------------------------------------------------------------------------
     * Read an unsigned 16 bit integer from the buffer.
    */
    SQInteger ReadUint16()
    {
        // Validate the managed buffer reference
        ValidateDeeper();
        // Read one element from the buffer
        const Uint16 value = m_Buffer->Cursor< Uint16 >();
        // Advance the buffer cursor
        m_Buffer->Advance< Uint16 >(1);
        // Return the requested information
        return ConvTo< SQInteger >::From(value);
    }

    /* --------------------------------------------------------------------------------------------
     * Read a signed 32 bit integer from the buffer.
    */
    SQInteger ReadInt32()
    {
        // Validate the managed buffer reference
        ValidateDeeper();
        // Read one element from the buffer
        const Int32 value = m_Buffer->Cursor< Int32 >();
        // Advance the buffer cursor
        m_Buffer->Advance< Int32 >(1);
        // Return the requested information
        return ConvTo< SQInteger >::From(value);
    }

    /* --------------------------------------------------------------------------------------------
     * Read an unsigned 32 bit integer from the buffer.
    */
    SQInteger ReadUint32()
    {
        // Validate the managed buffer reference
        ValidateDeeper();
        // Read one element from the buffer
        const Uint32 value = m_Buffer->Cursor< Uint32 >();
        // Advance the buffer cursor
        m_Buffer->Advance< Uint32 >(1);
        // Return the requested information
        return ConvTo< SQInteger >::From(value);
    }

    /* --------------------------------------------------------------------------------------------
     * Read a signed 64 bit integer from the buffer.
    */
    SLongInt ReadInt64();

    /* --------------------------------------------------------------------------------------------
     * Read an unsigned 64 bit integer from the buffer.
    */
    ULongInt ReadUint64();

    /* --------------------------------------------------------------------------------------------
     * Read a 32 bit float from the buffer.
    */
    SQFloat ReadFloat32()
    {
        // Validate the managed buffer reference
        ValidateDeeper();
        // Read one element from the buffer
        const Float32 value = m_Buffer->Cursor< Float32 >();
        // Advance the buffer cursor
        m_Buffer->Advance< Float32 >(1);
        // Return the requested information
        return ConvTo< SQFloat >::From(value);
    }

    /* --------------------------------------------------------------------------------------------
     * Read a 64 bit float from the buffer.
    */
    SQFloat ReadFloat64()
    {
        // Validate the managed buffer reference
        ValidateDeeper();
        // Read one element from the buffer
        const Float64 value = m_Buffer->Cursor< Float64 >();
        // Advance the buffer cursor
        m_Buffer->Advance< Float64 >(1);
        // Return the requested information
        return ConvTo< SQFloat >::From(value);
    }

    /* --------------------------------------------------------------------------------------------
     * Read a raw string from the buffer.
    */
    LightObj ReadRawString(SQInteger length);

    /* --------------------------------------------------------------------------------------------
     * Read a string from the buffer.
    */
    LightObj ReadClientString();

    /* --------------------------------------------------------------------------------------------
     * Read a AABB from the buffer.
    */
    AABB ReadAABB();

    /* --------------------------------------------------------------------------------------------
     * Read a Circle from the buffer.
    */
    Circle ReadCircle();

    /* --------------------------------------------------------------------------------------------
     * Read a Color3 from the buffer.
    */
    Color3 ReadColor3();

    /* --------------------------------------------------------------------------------------------
     * Read a Color4 from the buffer.
    */
    Color4 ReadColor4();

    /* --------------------------------------------------------------------------------------------
     * Read a Quaternion from the buffer.
    */
    Quaternion ReadQuaternion();

    /* --------------------------------------------------------------------------------------------
     * Read a Sphere from the buffer.
    */
    Sphere ReadSphere();

    /* --------------------------------------------------------------------------------------------
     * Read a Vector2 from the buffer.
    */
    Vector2 ReadVector2();

    /* --------------------------------------------------------------------------------------------
     * Read a Vector2i from the buffer.
    */
    Vector2i ReadVector2i();

    /* --------------------------------------------------------------------------------------------
     * Read a Vector3 from the buffer.
    */
    Vector3 ReadVector3();

    /* --------------------------------------------------------------------------------------------
     * Read a Vector4 from the buffer.
    */
    Vector4 ReadVector4();
};

} // Namespace:: SqMod
