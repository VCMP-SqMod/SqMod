#pragma once

// ------------------------------------------------------------------------------------------------
#include "Core/Buffer.hpp"
#include "Core/Utility.hpp"

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
     * Default constructor.
    */
    SqBuffer()
        : m_Buffer(new Buffer())
    {
    }

    /* --------------------------------------------------------------------------------------------
     * Allocate constructor.
    */
    explicit SqBuffer(SQInteger n)
        : m_Buffer(new Buffer(ConvTo< SzType >::From(n)))
    {
    }

    /* --------------------------------------------------------------------------------------------
     * Allocate constructor.
    */
    SqBuffer(SQInteger n, SQInteger c)
        : m_Buffer(new Buffer(ConvTo< SzType >::From(n), ConvTo< SzType >::From(c)))
    {
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    SqBuffer(ConstPtr p, SQInteger n)
        : m_Buffer(new Buffer(p, ConvTo< SzType >::From(n)))
    {
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    SqBuffer(ConstPtr p, SQInteger n, SQInteger c)
        : m_Buffer(new Buffer(p, ConvTo< SzType >::From(n), ConvTo< SzType >::From(c)))
    {
    }

    /* --------------------------------------------------------------------------------------------
     * Reference constructor.
    */
    explicit SqBuffer(const SRef & ref) // NOLINT(modernize-pass-by-value)
        : m_Buffer(ref)
    {
    }

    /* --------------------------------------------------------------------------------------------
     * Buffer constructor.
    */
    explicit SqBuffer(const Buffer & b)
        : m_Buffer(new Buffer(b))
    {
    }

    /* --------------------------------------------------------------------------------------------
     * Buffer constructor.
    */
    explicit SqBuffer(Buffer && b)
        : m_Buffer(new Buffer(std::move(b)))
    {
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
    SQMOD_NODISCARD const SRef & GetRef() const
    {
        return m_Buffer;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve a reference to the managed memory buffer.
    */
    SQMOD_NODISCARD Buffer & GetInst() const
    {
        return *m_Buffer;
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
     * Validate the managed memory buffer reference.
    */
    SQMOD_NODISCARD Buffer & Valid() const
    {
        Validate();
        // Return the buffer
        return *m_Buffer;
    }

    /* --------------------------------------------------------------------------------------------
     * Validate the managed memory buffer reference and the buffer itself.
    */
    SQMOD_NODISCARD Buffer & ValidDeeper() const
    {
        ValidateDeeper();
        // Return the buffer
        return *m_Buffer;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve a certain element type at the specified position.
    */
    SQMOD_NODISCARD Value Get(SQInteger n) const
    {
        return Valid().At(ConvTo< SzType >::From(n));
    }

    /* --------------------------------------------------------------------------------------------
     * Modify a certain element type at the specified position.
    */
    void Set(SQInteger n, SQInteger v) const
    {
        Valid().At(ConvTo< SzType >::From(n)) = ConvTo< Value >::From(v);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the element at the front of the buffer.
    */
    SQMOD_NODISCARD Value GetFront() const
    {
        return Valid().Front();
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the element at the front of the buffer.
    */
    void SetFront(SQInteger v) const
    {
        Valid().Front() = ConvTo< Value >::From(v);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the element after the first element in the buffer.
    */
    SQMOD_NODISCARD Value GetNext() const
    {
        return Valid().Next();
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the element after the first element in the buffer.
    */
    void SetNext(SQInteger v) const
    {
        Valid().Next() = ConvTo< Value >::From(v);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the element at the back of the buffer.
    */
    SQMOD_NODISCARD Value GetBack() const
    {
        return Valid().Back();
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the element at the back of the buffer.
    */
    void SetBack(SQInteger v) const
    {
        Valid().Back() = ConvTo< Value >::From(v);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the element before the last element in the buffer.
    */
    SQMOD_NODISCARD Value GetPrev() const
    {
        return Valid().Prev();
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the element before the last element in the buffer.
    */
    void SetPrev(SQInteger v) const
    {
        Valid().Prev() = ConvTo< Value >::From(v);
    }

    /* --------------------------------------------------------------------------------------------
     * Reposition the edit cursor to the specified number of elements ahead.
    */
    void Advance(SQInteger n) const
    {
        Valid().Advance(ConvTo< SzType >::From(n));
    }

    /* --------------------------------------------------------------------------------------------
     * Reposition the edit cursor to the specified number of elements behind.
    */
    void Retreat(SQInteger n) const
    {
        Valid().Retreat(ConvTo< SzType >::From(n));
    }

    /* --------------------------------------------------------------------------------------------
     * Reposition the edit cursor to a fixed position within the buffer.
    */
    void Move(SQInteger n) const
    {
        Valid().Move(ConvTo< SzType >::From(n));
    }

    /* --------------------------------------------------------------------------------------------
     * Append a value to the current cursor location and advance the cursor.
    */
    void Push(SQInteger v) const
    {
        Valid().Push(ConvTo< Value >::From(v));
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the element at the cursor position.
    */
    SQMOD_NODISCARD Value GetCursor() const
    {
        return Valid().Cursor();
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the element at the cursor position.
    */
    void SetCursor(SQInteger v) const
    {
        Valid().Cursor() = ConvTo< Value >::From(v);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the element before the cursor position.
    */
    SQMOD_NODISCARD Value GetBefore() const
    {
        return Valid().Before();
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the element before the cursor position.
    */
    void SetBefore(SQInteger v) const
    {
        Valid().Before() = ConvTo< Value >::From(v);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the element after the cursor position.
    */
    SQMOD_NODISCARD Value GetAfter() const
    {
        return Valid().After();
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the element after the cursor position.
    */
    void SetAfter(SQInteger v) const
    {
        Valid().After() = ConvTo< Value >::From(v);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve maximum elements it can hold for a certain type.
    */
    SQMOD_NODISCARD SzType GetMax() const // NOLINT(readability-convert-member-functions-to-static)
    {
        return Buffer::Max();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the current buffer capacity in element count.
    */
    SQMOD_NODISCARD SzType GetSize() const
    {
        return Valid().CapacityAs< Value >();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the current buffer capacity in byte count.
    */
    SQMOD_NODISCARD SzType GetCapacity() const
    {
        return Valid().Capacity();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the current position of the cursor in the buffer.
    */
    SQMOD_NODISCARD SzType GetPosition() const
    {
        return Valid().Position();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the amount of unused buffer after the edit cursor.
    */
    SQMOD_NODISCARD SzType GetRemaining() const
    {
        return Valid().Remaining();
    }

    /* --------------------------------------------------------------------------------------------
     * Grow the size of the internal buffer by the specified amount of bytes.
    */
    void Grow(SQInteger n) const
    {
        return Valid().Grow(ConvTo< SzType >::From(n) * sizeof(Value));
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
            STHROWF("{}", e.what()); // Re-package
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Write a signed 8 bit integer to the buffer.
    */
    void WriteInt8(SQInteger val) const
    {
        Valid().Push< int8_t >(static_cast< int8_t >(val));
    }

    /* --------------------------------------------------------------------------------------------
     * Write an unsigned 8 bit integer to the buffer.
    */
    void WriteUint8(SQInteger val) const
    {
        Valid().Push< uint8_t >(static_cast< uint8_t >(val));
    }

    /* --------------------------------------------------------------------------------------------
     * Write a signed 16 bit integer to the buffer.
    */
    void WriteInt16(SQInteger val) const
    {
        Valid().Push< int16_t >(static_cast< int16_t >(val));
    }

    /* --------------------------------------------------------------------------------------------
     * Write an unsigned 16 bit integer to the buffer.
    */
    void WriteUint16(SQInteger val) const
    {
        Valid().Push< uint16_t >(static_cast< uint16_t >(val));
    }

    /* --------------------------------------------------------------------------------------------
     * Write a signed 32 bit integer to the buffer.
    */
    void WriteInt32(SQInteger val) const
    {
        Valid().Push< int32_t >(static_cast< int32_t >(val));
    }

    /* --------------------------------------------------------------------------------------------
     * Write an unsigned 32 bit integer to the buffer.
    */
    void WriteUint32(SQInteger val) const
    {
        Valid().Push< uint32_t >(static_cast< uint32_t >(val));
    }

    /* --------------------------------------------------------------------------------------------
     * Write a signed 64 bit integer to the buffer.
    */
    void WriteInt64(SQInteger val) const
    {
        Valid().Push< int64_t >(static_cast< int64_t >(val));
    }

    /* --------------------------------------------------------------------------------------------
     * Write an unsigned 64 bit integer to the buffer.
    */
    void WriteUint64(SQInteger val) const
    {
        Valid().Push< uint64_t >(static_cast< uint64_t >(val));
    }

    /* --------------------------------------------------------------------------------------------
     * Write a 32 bit float to the buffer.
    */
    void WriteFloat32(SQFloat val) const
    {
        Valid().Push< float >(ConvTo< float >::From(val));
    }

    /* --------------------------------------------------------------------------------------------
     * Write a 64 bit float to the buffer.
    */
    void WriteFloat64(SQFloat val) const
    {
        Valid().Push< double >(ConvTo< double >::From(val));
    }

    /* --------------------------------------------------------------------------------------------
     * Write a raw string to the buffer.
    */
    SQInteger WriteRawString(StackStrF & val) const;

    /* --------------------------------------------------------------------------------------------
     * Write a client encoded string to the buffer.
    */
    SQInteger WriteClientString(StackStrF & val) const;

    /* --------------------------------------------------------------------------------------------
     * Write a AABB to the buffer.
    */
    void WriteAABB(const AABB & val) const;

    /* --------------------------------------------------------------------------------------------
     * Write a Circle to the buffer.
    */
    void WriteCircle(const Circle & val) const;

    /* --------------------------------------------------------------------------------------------
     * Write a Color3 to the buffer.
    */
    void WriteColor3(const Color3 & val) const;

    /* --------------------------------------------------------------------------------------------
     * Write a Color4 to the buffer.
    */
    void WriteColor4(const Color4 & val) const;

    /* --------------------------------------------------------------------------------------------
     * Write a Quaternion to the buffer.
    */
    void WriteQuaternion(const Quaternion & val) const;

    /* --------------------------------------------------------------------------------------------
     * Write a Sphere to the buffer.
    */
    void WriteSphere(const Sphere &val) const;

    /* --------------------------------------------------------------------------------------------
     * Write a Vector2 to the buffer.
    */
    void WriteVector2(const Vector2 & val) const;

    /* --------------------------------------------------------------------------------------------
     * Write a Vector2i to the buffer.
    */
    void WriteVector2i(const Vector2i & val) const;

    /* --------------------------------------------------------------------------------------------
     * Write a Vector3 to the buffer.
    */
    void WriteVector3(const Vector3 & val) const;

    /* --------------------------------------------------------------------------------------------
     * Write a Vector4 to the buffer.
    */
    void WriteVector4(const Vector4 & val) const;

    /* --------------------------------------------------------------------------------------------
     * Write a signed 8 bit integer from the buffer.
    */
    SQMOD_NODISCARD SQInteger ReadInt8() const
    {
        // Validate the managed buffer reference
        ValidateDeeper();
        // Read one element from the buffer
        const int8_t value = m_Buffer->Cursor< int8_t >();
        // Advance the buffer cursor
        m_Buffer->Advance< int8_t >(1);
        // Return the requested information
        return ConvTo< SQInteger >::From(value);
    }

    /* --------------------------------------------------------------------------------------------
     * Read an unsigned 8 bit integer from the buffer.
    */
    SQMOD_NODISCARD SQInteger ReadUint8() const
    {
        // Validate the managed buffer reference
        ValidateDeeper();
        // Read one element from the buffer
        const uint8_t value = m_Buffer->Cursor< uint8_t >();
        // Advance the buffer cursor
        m_Buffer->Advance< uint8_t >(1);
        // Return the requested information
        return ConvTo< SQInteger >::From(value);
    }

    /* --------------------------------------------------------------------------------------------
     * Read a signed 16 bit integer from the buffer.
    */
    SQMOD_NODISCARD SQInteger ReadInt16() const
    {
        // Validate the managed buffer reference
        ValidateDeeper();
        // Read one element from the buffer
        const int16_t value = m_Buffer->Cursor< int16_t >();
        // Advance the buffer cursor
        m_Buffer->Advance< int16_t >(1);
        // Return the requested information
        return ConvTo< SQInteger >::From(value);
    }

    /* --------------------------------------------------------------------------------------------
     * Read an unsigned 16 bit integer from the buffer.
    */
    SQMOD_NODISCARD SQInteger ReadUint16() const
    {
        // Validate the managed buffer reference
        ValidateDeeper();
        // Read one element from the buffer
        const uint16_t value = m_Buffer->Cursor< uint16_t >();
        // Advance the buffer cursor
        m_Buffer->Advance< uint16_t >(1);
        // Return the requested information
        return ConvTo< SQInteger >::From(value);
    }

    /* --------------------------------------------------------------------------------------------
     * Read a signed 32 bit integer from the buffer.
    */
    SQMOD_NODISCARD SQInteger ReadInt32() const
    {
        // Validate the managed buffer reference
        ValidateDeeper();
        // Read one element from the buffer
        const int32_t value = m_Buffer->Cursor< int32_t >();
        // Advance the buffer cursor
        m_Buffer->Advance< int32_t >(1);
        // Return the requested information
        return ConvTo< SQInteger >::From(value);
    }

    /* --------------------------------------------------------------------------------------------
     * Read an unsigned 32 bit integer from the buffer.
    */
    SQMOD_NODISCARD SQInteger ReadUint32() const
    {
        // Validate the managed buffer reference
        ValidateDeeper();
        // Read one element from the buffer
        const uint32_t value = m_Buffer->Cursor< uint32_t >();
        // Advance the buffer cursor
        m_Buffer->Advance< uint32_t >(1);
        // Return the requested information
        return ConvTo< SQInteger >::From(value);
    }

    /* --------------------------------------------------------------------------------------------
     * Read a signed 64 bit integer from the buffer.
    */
    SQMOD_NODISCARD SQInteger ReadInt64() const;

    /* --------------------------------------------------------------------------------------------
     * Read an unsigned 64 bit integer from the buffer.
    */
    SQMOD_NODISCARD SQInteger ReadUint64() const;

    /* --------------------------------------------------------------------------------------------
     * Read a 32 bit float from the buffer.
    */
    SQMOD_NODISCARD SQFloat ReadFloat32() const
    {
        // Validate the managed buffer reference
        ValidateDeeper();
        // Read one element from the buffer
        const float value = m_Buffer->Cursor< float >();
        // Advance the buffer cursor
        m_Buffer->Advance< float >(1);
        // Return the requested information
        return ConvTo< SQFloat >::From(value);
    }

    /* --------------------------------------------------------------------------------------------
     * Read a 64 bit float from the buffer.
    */
    SQMOD_NODISCARD SQFloat ReadFloat64() const
    {
        // Validate the managed buffer reference
        ValidateDeeper();
        // Read one element from the buffer
        const double value = m_Buffer->Cursor< double >();
        // Advance the buffer cursor
        m_Buffer->Advance< double >(1);
        // Return the requested information
        return ConvTo< SQFloat >::From(value);
    }

    /* --------------------------------------------------------------------------------------------
     * Read a raw string from the buffer.
    */
    SQMOD_NODISCARD LightObj ReadRawString(SQInteger length) const;

    /* --------------------------------------------------------------------------------------------
     * Read a string from the buffer.
    */
    SQMOD_NODISCARD LightObj ReadClientString() const;

    /* --------------------------------------------------------------------------------------------
     * Read a AABB from the buffer.
    */
    SQMOD_NODISCARD AABB ReadAABB() const;

    /* --------------------------------------------------------------------------------------------
     * Read a Circle from the buffer.
    */
    SQMOD_NODISCARD Circle ReadCircle() const;

    /* --------------------------------------------------------------------------------------------
     * Read a Color3 from the buffer.
    */
    SQMOD_NODISCARD Color3 ReadColor3() const;

    /* --------------------------------------------------------------------------------------------
     * Read a Color4 from the buffer.
    */
    SQMOD_NODISCARD Color4 ReadColor4() const;

    /* --------------------------------------------------------------------------------------------
     * Read a Quaternion from the buffer.
    */
    SQMOD_NODISCARD Quaternion ReadQuaternion() const;

    /* --------------------------------------------------------------------------------------------
     * Read a Sphere from the buffer.
    */
    SQMOD_NODISCARD Sphere ReadSphere() const;

    /* --------------------------------------------------------------------------------------------
     * Read a Vector2 from the buffer.
    */
    SQMOD_NODISCARD Vector2 ReadVector2() const;

    /* --------------------------------------------------------------------------------------------
     * Read a Vector2i from the buffer.
    */
    SQMOD_NODISCARD Vector2i ReadVector2i() const;

    /* --------------------------------------------------------------------------------------------
     * Read a Vector3 from the buffer.
    */
    SQMOD_NODISCARD Vector3 ReadVector3() const;

    /* --------------------------------------------------------------------------------------------
     * Read a Vector4 from the buffer.
    */
    SQMOD_NODISCARD Vector4 ReadVector4() const;

    /* --------------------------------------------------------------------------------------------
     * Compute the CRC-32 checksums on the data in the buffer.
    */
    SQMOD_NODISCARD SQInteger GetCRC32(SQInteger n) const;

    /* --------------------------------------------------------------------------------------------
     * Compute the Adler-32 checksums on the data in the buffer.
    */
    SQMOD_NODISCARD SQInteger GetADLER32(SQInteger n) const;
};

} // Namespace:: SqMod
