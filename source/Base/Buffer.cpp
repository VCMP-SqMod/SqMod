#include "Base/Buffer.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
Buffer::Buffer()
    : m_Data(nullptr), m_Size(0)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
Buffer::Buffer(SzType sz)
    : m_Data(Alloc(sz)), m_Size(m_Data == nullptr ? 0 : sz)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
Buffer::Buffer(Buffer && o)
    : m_Data(o.m_Data), m_Size(o.m_Size)
{
    o.m_Data = nullptr;
}

// ------------------------------------------------------------------------------------------------
Buffer::~Buffer()
{
    Free(m_Data);
}

// ------------------------------------------------------------------------------------------------
Buffer & Buffer::operator = (Buffer && o)
{
    if (m_Data != o.m_Data)
    {
        m_Data = o.m_Data;
        m_Size = o.m_Size;

        o.m_Data = nullptr;
    }

    return *this;
}

// ------------------------------------------------------------------------------------------------
void Buffer::Resize(SzType sz)
{
    if (!sz)
    {
        Free(m_Data);
    }
    else if (sz != m_Size)
    {
        Pointer data = m_Data;
        m_Data = Alloc(sz);

        if (sz > m_Size)
        {
            Copy(data, m_Size);
        }
        else
        {
            Copy(data, sz);
        }

        m_Size = sz;
        Free(data);
    }
}

// ------------------------------------------------------------------------------------------------
void Buffer::Reserve(SzType sz)
{
    if (!sz)
    {
        return;
    }
    else if (sz > m_Size)
    {
        Pointer data = m_Data;
        m_Data = Alloc(sz);

        Copy(data, m_Size);

        m_Size = sz;
        Free(data);
    }
}

// ------------------------------------------------------------------------------------------------
void Buffer::Copy(ConstPtr buf, SzType sz)
{
    memcpy(m_Data, buf, sz * sizeof(Value));
}

// ------------------------------------------------------------------------------------------------
Buffer::Pointer Buffer::Alloc(SzType sz)
{
    Pointer mem = reinterpret_cast< Pointer >(malloc(sz * sizeof(Value)));

    if (!mem)
    {
        return nullptr;
    }

    return mem;
}

// ------------------------------------------------------------------------------------------------
void Buffer::Free(Pointer buf)
{
    if (buf != nullptr)
    {
        free(buf);
    }
}



} // Namespace:: SqMod
