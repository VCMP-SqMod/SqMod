// ------------------------------------------------------------------------------------------------
#include "Base/Buffer.hpp"
#include "sqrat/sqratUtil.h"

// ------------------------------------------------------------------------------------------------
#include <cstdlib>
#include <cstring>
#include <stdexcept>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Compute the next power of two for the specified number.
*/
inline unsigned int NextPow2(unsigned int num)
{
    --num;
    num |= num >> 1u;
    num |= num >> 2u;
    num |= num >> 4u;
    num |= num >> 8u;
    num |= num >> 16u;
    return ++num;
}

/* ------------------------------------------------------------------------------------------------
 * Throw an memory exception.
*/
void ThrowMemExcept(const char * msg, ...)
{
    // Exception messages should be concise
    SQChar buffer[256];
    // Variable arguments structure
    va_list args;
    // Get the specified arguments
    va_start(args, msg);
    // Run the specified format
    int ret =  std::vsnprintf(buffer, sizeof(buffer), msg, args);
    // Check for formatting errors
    if (ret < 0)
    {
        throw Sqrat::Exception(_SC("Unknown memory error")); // NOLINT(hicpp-exception-baseclass,cert-err60-cpp)
    }
    // Throw the actual exception
    throw Sqrat::Exception(buffer); // NOLINT(hicpp-exception-baseclass,cert-err60-cpp)
}

// ------------------------------------------------------------------------------------------------
Buffer::Buffer(const Buffer & o)
    : m_Ptr(nullptr)
    , m_Cap(o.m_Cap)
    , m_Cur(o.m_Cur)
{
    if (m_Cap)
    {
        Request(o.m_Cap);
        std::memcpy(m_Ptr, o.m_Ptr, o.m_Cap);
    }
}

// ------------------------------------------------------------------------------------------------
Buffer::~Buffer()
{
    // Do we have a buffer?
    if (m_Ptr)
    {
        Release(); // Release it!
    }
}

// ------------------------------------------------------------------------------------------------
Buffer & Buffer::operator = (const Buffer & o) // NOLINT(cert-oop54-cpp)
{
    if (m_Ptr != o.m_Ptr)
    {
        // Can we work in the current buffer?
        if (m_Cap && o.m_Cap <= m_Cap)
        {
            // It's safe to copy the data
            std::memcpy(m_Ptr, o.m_Ptr, o.m_Cap);
        }
        // Do we even have data to copy?
        else if (!o.m_Cap)
        {
            // Do we have a buffer?
            if (m_Ptr)
            {
                Release(); // Release it!
            }
        }
        else
        {
            // Do we have a buffer?
            if (m_Ptr)
            {
                Release(); // Release it!
            }
            // Request a larger buffer
            Request(o.m_Cap);
            // Now it's safe to copy the data
            std::memcpy(m_Ptr, o.m_Ptr, o.m_Cap);
        }
        // Also copy the edit cursor
        m_Cur = o.m_Cur;
    }

    return *this;
}

// ------------------------------------------------------------------------------------------------
void Buffer::Grow(SzType n)
{
    // Backup the current memory
    Buffer bkp(m_Ptr, m_Cap, m_Cur);
    // Acquire a bigger buffer
    Request(bkp.m_Cap + n);
    // Copy the data from the old buffer
    std::memcpy(m_Ptr, bkp.m_Ptr, bkp.m_Cap);
    // Copy the previous edit cursor
    m_Cur = bkp.m_Cur;
}

// ------------------------------------------------------------------------------------------------
void Buffer::Request(SzType n)
{
    // NOTE: Function assumes (n > 0)
    assert(n > 0);
    // Round up the size to a power of two number
    n = (n & (n - 1)) ? NextPow2(n) : n;
    // Release previous memory if any
    delete[] m_Ptr; // Implicitly handles null!
    // Attempt to allocate memory
    m_Ptr = new Value[n];
    // If no errors occurred then we can set the size
    m_Cap = n;
}

// ------------------------------------------------------------------------------------------------
void Buffer::Release()
{
    // Deallocate the memory
    delete[] m_Ptr; // Implicitly handles null!
    // Explicitly reset the buffer
    m_Ptr = nullptr;
    m_Cap = 0;
    m_Cur = 0;
}

// ------------------------------------------------------------------------------------------------
Buffer::SzType Buffer::Write(SzType pos, ConstPtr data, SzType size)
{
    // Do we have what to write?
    if (!data || !size)
    {
        return 0;
    }
    // See if the buffer size must be adjusted
    else if ((pos + size) >= m_Cap)
    {
        // Acquire a larger buffer
        Grow((pos + size) - m_Cap + 32);
    }
    // Copy the data into the internal buffer
    std::memcpy(m_Ptr + pos, data, size);
    // Return the amount of data written to the buffer
    return size;
}

// ------------------------------------------------------------------------------------------------
Buffer::SzType Buffer::WriteF(SzType pos, const char * fmt, ...)
{
    // Initialize the variable argument list
    va_list args;
    va_start(args, fmt);
    // Call the function that takes the variable argument list
    const SzType ret = WriteF(pos, fmt, args);
    // Finalize the variable argument list
    va_end(args);
    // Return the result
    return ret;
}

// ------------------------------------------------------------------------------------------------
Buffer::SzType Buffer::WriteF(SzType pos, const char * fmt, va_list args)
{
    // Is the specified position within range?
    if (pos >= m_Cap)
    {
        // Acquire a larger buffer
        Grow(pos - m_Cap + 32);
    }
    // Backup the variable argument list
    va_list args_cpy;
    va_copy(args_cpy, args);
    // Attempt to write to the current buffer
    // (if empty, it should tell us the necessary size)
    int ret =  std::vsnprintf(m_Ptr + pos, m_Cap, fmt, args);
    // Do we need a bigger buffer?
    if ((pos + ret) >= m_Cap)
    {
        // Acquire a larger buffer
        Grow((pos + ret) - m_Cap + 32);
        // Retry writing the requested information
        ret =  std::vsnprintf(m_Ptr + pos, m_Cap, fmt, args_cpy);
    }
    // Return the value 0 if data could not be written
    if (ret < 0)
    {
        return 0;
    }
    // Return the number of written characters
    return static_cast< SzType >(ret);
}

// ------------------------------------------------------------------------------------------------
Buffer::SzType Buffer::WriteS(SzType pos, ConstPtr str)
{
    // Is there any string to write?
    if (str && *str != '\0')
    {
        // Forward this to the regular write function
        return Write(pos, str, std::strlen(str));
    }
    // Nothing to write
    return 0;
}

// ------------------------------------------------------------------------------------------------
void Buffer::AppendF(const char * fmt, ...)
{
    // Initialize the variable argument list
    va_list args;
    va_start(args, fmt);
    // Forward this to the regular write function
    m_Cur += WriteF(m_Cur, fmt, args);
    // Finalize the variable argument list
    va_end(args);
}

// ------------------------------------------------------------------------------------------------
void Buffer::AppendS(const char * str)
{
    // Is there any string to write?
    if (str)
    {
        m_Cur += Write(m_Cur, str, std::strlen(str));
    }
}

} // Namespace:: SqMod
