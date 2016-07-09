// ------------------------------------------------------------------------------------------------
#include "Base/Buffer.hpp"
#include "sqrat/sqratUtil.h"

// ------------------------------------------------------------------------------------------------
#include <cstdlib>
#include <cstring>
#include <exception>
#include <stdexcept>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Compute the next power of two for the specified number.
*/
inline unsigned int NextPow2(unsigned int num)
{
    --num;
    num |= num >> 1;
    num |= num >> 2;
    num |= num >> 4;
    num |= num >> 8;
    num |= num >> 16;
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
        throw Sqrat::Exception(_SC("Unknown memory error"));
    }
    // Throw the actual exception
    throw Sqrat::Exception(buffer);
}

/* ------------------------------------------------------------------------------------------------
 * Allocate a memory buffer and return it.
*/
static Buffer::Pointer AllocMem(Buffer::SzType size)
{
    // Attempt to allocate memory directly
    Buffer::Pointer ptr = reinterpret_cast< Buffer::Pointer >(std::malloc(size));
    // Validate the allocated memory
    if (!ptr)
    {
        ThrowMemExcept("Unable to allocate (%u) bytes of memory", size);
    }
    // Return the allocated memory
    return ptr;
}

/* ------------------------------------------------------------------------------------------------
 * ...
*/
class MemCat
{
    // --------------------------------------------------------------------------------------------
    friend class Memory;
    friend class Buffer;

public:

    // --------------------------------------------------------------------------------------------
    typedef Buffer::Value       Value; // The type of value used to represent a byte.

    // --------------------------------------------------------------------------------------------
    typedef Buffer::Reference   Reference; // A reference to the stored value type.
    typedef Buffer::ConstRef    ConstRef; // A const reference to the stored value type.

    // --------------------------------------------------------------------------------------------
    typedef Buffer::Pointer     Pointer; // A pointer to the stored value type.
    typedef Buffer::ConstPtr    ConstPtr; // A const pointer to the stored value type.

    // --------------------------------------------------------------------------------------------
    typedef Buffer::SzType      SzType; // The type used to represent size in general.

private:

    /* --------------------------------------------------------------------------------------------
     * Structure used to store a memory chunk in the linked list.
    */
    struct Node
    {
        // ----------------------------------------------------------------------------------------
        SzType      mCap; // The size of the memory chunk.
        Pointer     mPtr; // Pointer to the memory chunk.
        Node*       mNext; // The next node in the list.

        /* ----------------------------------------------------------------------------------------
         * Base constructor.
        */
        Node(Node * next)
            : mCap(0)
            , mPtr(nullptr)
            , mNext(next)
        {
            /* ... */
        }
    };

    // --------------------------------------------------------------------------------------------
    static Node *   s_Nodes; /* List of unused node instances. */

    // --------------------------------------------------------------------------------------------
    Node*   m_Head; /* The head memory node. */

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    MemCat()
        : m_Head(nullptr)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~MemCat()
    {
        for (Node * node = m_Head, * next = nullptr; node; node = next)
        {
            // Free the memory (if any)
            if (node->mPtr)
            {
                std::free(node->mPtr);
            }
            // Save the next node
            next = node->mNext;
            // Release the node instance
            delete node;
        }
        // Explicitly set the head node to null
        m_Head = nullptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Clear all memory buffers from the pool.
    */
    void Clear()
    {
        for (Node * node = m_Head, * next = nullptr; node; node = next)
        {
            // Free the memory (if any)
            if (node->mPtr)
            {
                free(node->mPtr);
            }
            // Save the next node
            next = node->mNext;
            // Release the node instance
            Push(node);
        }
        // Explicitly set the head node to null
        m_Head = nullptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Grab a memory buffer from the pool.
    */
    void Grab(Pointer & ptr, SzType & size)
    {
        // NOTE: Function assumes (size > 0)
        // Find a buffer large enough to satisfy the requested size
        for (Node * node = m_Head, * prev = nullptr; node; prev = node, node = node->mNext)
        {
            // Is this buffer large enough?
            if (node->mCap >= size)
            {
                // Was there a previous node?
                if (prev)
                {
                    prev->mNext = node->mNext;
                }
                // Probably this was the head
                else
                {
                    m_Head = node->mNext;
                }
                // Assign the memory
                ptr = node->mPtr;
                // Assign the size
                size = node->mCap;
                // Release the node instance
                Push(node);
                // Exit the function
                return;
            }
        }
        // Round up the size to a power of two number
        size = (size & (size - 1)) ? NextPow2(size) : size;
        // Allocate the memory directly
        ptr = AllocMem(size);
        // See if the memory could be allocated
        // (shouldn't reach this point if allocation failed)
        if (!ptr)
        {
            // Revert the size
            size = 0;
            // Throw the exception
            ThrowMemExcept("Unable to allocate (%u) bytes of memory", size);
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Return a memory buffer to the pool.
    */
    void Drop(Pointer & ptr, SzType & size)
    {
        if (!ptr)
        {
            ThrowMemExcept("Cannot store invalid memory buffer");
        }
        // Request a node instance
        Node * node = Pull();
        // Assign the specified memory
        node->mPtr = ptr;
        // Assign the specified size
        node->mCap = size;
        // Demote the current head node
        node->mNext = m_Head;
        // Promote as the head node
        m_Head = node;
    }

    /* --------------------------------------------------------------------------------------------
     * Allocate a group of nodes and pool them for later use.
    */
    static void Make()
    {
        for (SzType n = 16; n; --n)
        {
            // Create a new node instance
            s_Nodes = new Node(s_Nodes);
            // Validate the head node
            if (!s_Nodes)
            {
                ThrowMemExcept("Unable to allocate memory nodes");
            }
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve an unused node from the free list.
    */
    static Node * Pull()
    {
        // Are there any nodes available?
        if (!s_Nodes)
        {
            Make(); // Make some!
        }
        // Grab the head node
        Node * node = s_Nodes;
        // Promote the next node as the head
        s_Nodes = node->mNext;
        // Return the node
        return node;
    }

    /* --------------------------------------------------------------------------------------------
     * Return a node to the free list.
    */
    static void Push(Node * node)
    {
        // See if the node is even valid
        if (!node)
        {
            ThrowMemExcept("Attempting to push invalid node");
        }
        // Demote the current head node
        node->mNext = s_Nodes;
        // Promote as the head node
        s_Nodes = node;
    }
};

// ------------------------------------------------------------------------------------------------
MemCat::Node *  MemCat::s_Nodes = nullptr;

/* ------------------------------------------------------------------------------------------------
 * Lightweight memory allocator to reduce the overhead of small allocations.
*/
class Memory
{
    // --------------------------------------------------------------------------------------------
    friend class Buffer; // Allow the buffer type to access the memory categories.
    friend class MemRef; // Allow the memory manager reference to create new instances.

private:

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    Memory()
        : m_Small()
        , m_Medium()
        , m_Large()
    {
        // Allocate several nodes for when memory starts pooling
        MemCat::Make();
    }

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~Memory()
    {
        for (MemCat::Node * node = MemCat::s_Nodes, * next = nullptr; node; node = next)
        {
            // Save the next node
            next = node->mNext;
            // Release the node instance
            delete node;
        }
        // Explicitly set the head node to null
        MemCat::s_Nodes = nullptr;
    }

private:

    // --------------------------------------------------------------------------------------------
    MemCat m_Small; // Small memory allocations of <= 1024 bytes.
    MemCat m_Medium; // Medium memory allocations of <= 4096 bytes.
    MemCat m_Large; // Large memory allocations of <= 4096 bytes.
};

// ------------------------------------------------------------------------------------------------
MemRef MemRef::s_Mem;

// ------------------------------------------------------------------------------------------------
void MemRef::Grab()
{
    if (m_Ptr)
    {
        ++(*m_Ref);
    }
}

// ------------------------------------------------------------------------------------------------
void MemRef::Drop()
{
    if (m_Ptr && --(*m_Ref) == 0)
    {
        delete m_Ptr;
        delete m_Ref;
        m_Ptr = nullptr;
        m_Ref = nullptr;
    }
}

// ------------------------------------------------------------------------------------------------
const MemRef & MemRef::Get()
{
    if (!s_Mem.m_Ptr)
    {
        s_Mem.m_Ptr = new Memory();
        s_Mem.m_Ref = new Counter(1);
    }

    return s_Mem;
}

// ------------------------------------------------------------------------------------------------
Buffer::Buffer(const Buffer & o)
    : m_Ptr(nullptr)
    , m_Cap(o.m_Cap)
    , m_Cur(o.m_Cur)
    , m_Mem(o.m_Mem)
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
Buffer & Buffer::operator = (const Buffer & o)
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
    Buffer bkp(m_Ptr, m_Cap, m_Cur, m_Mem);
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
    // Is there a memory manager available?
    if (!m_Mem)
    {
        // Round up the size to a power of two number
        n = (n & (n - 1)) ? NextPow2(n) : n;
        // Allocate the memory directly
        m_Ptr = AllocMem(n);
    }
    // Find out in which category does this buffer reside
    else if (n <= 1024)
    {
        m_Mem->m_Small.Grab(m_Ptr, n);
    }
    else if (n <= 4096)
    {
        m_Mem->m_Medium.Grab(m_Ptr, n);
    }
    else
    {
        m_Mem->m_Large.Grab(m_Ptr, n);
    }
    // If no errors occurred then we can set the size
    m_Cap = n;
}

// ------------------------------------------------------------------------------------------------
void Buffer::Release()
{
    // TODO: Implement a limit on how much memory can actually be pooled.
    // Is there a memory manager available?
    if (!m_Mem)
    {
        std::free(m_Ptr); // Deallocate the memory directly
    }
    // Find out to which category does this buffer belong
    else if (m_Cap <= 1024)
    {
        m_Mem->m_Small.Drop(m_Ptr, m_Cap);
    }
    else if (m_Cap <= 4096)
    {
        m_Mem->m_Medium.Drop(m_Ptr, m_Cap);
    }
    else
    {
        m_Mem->m_Large.Drop(m_Ptr, m_Cap);
    }
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
