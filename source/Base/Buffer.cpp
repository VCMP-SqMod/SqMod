// ------------------------------------------------------------------------------------------------
#include "Base/Buffer.hpp"

// ------------------------------------------------------------------------------------------------
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <exception>
#include <stdexcept>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Compute the next power of two for the specified number.
*/
static inline unsigned int NPow2(unsigned int num)
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
    char buffer[128];
    // Variable arguments structure
    va_list args;
    // Get the specified arguments
    va_start (args, msg);
    // Run the specified format
    int ret =  vsnprintf(buffer, sizeof(buffer), msg, args);
    // Check for formatting errors
    if (ret < 0)
    {
        throw std::runtime_error("Unknown memory error");
    }
    // Throw the actual exception
    throw std::runtime_error(buffer);
}

/* ------------------------------------------------------------------------------------------------
 * Allocate a memory buffer and return it.
*/
static Buffer::Pointer AllocMem(Buffer::SzType size)
{
    // Attempt to allocate memory directly
    Buffer::Pointer ptr = (Buffer::Pointer)malloc(size);
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
    typedef Buffer::Value       Value;

    // --------------------------------------------------------------------------------------------
    typedef Buffer::Reference   Reference;
    typedef Buffer::ConstRef    ConstRef;

    // --------------------------------------------------------------------------------------------
    typedef Buffer::Pointer     Pointer;
    typedef Buffer::ConstPtr    ConstPtr;

    // --------------------------------------------------------------------------------------------
    typedef Buffer::SzType      SzType;

private:

    /* --------------------------------------------------------------------------------------------
     *
    */
    struct Node
    {
        // ----------------------------------------------------------------------------------------
        SzType      mCap;
        Pointer     mPtr;
        Node*       mNext;

        /* ----------------------------------------------------------------------------------------
         * Base constructor.
        */
        Node(Node * next)
            : mCap(0), mPtr(NULL), mNext(next)
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
        : m_Head(NULL)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~MemCat()
    {
        for (Node * node = m_Head, * next = NULL; node; node = next)
        {
            // Free the memory (if any)
            if (node->mPtr)
            {
                free(node->mPtr);
            }
            // Save the next node
            next = node->mNext;
            // Release the node instance
            delete node;
        }
        // Explicitly set the head node to null
        m_Head = NULL;
    }

    /* --------------------------------------------------------------------------------------------
     * Clear all memory buffers from the pool.
    */
    void Clear()
    {
        for (Node * node = m_Head, * next = NULL; node; node = next)
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
        m_Head = NULL;
    }

    /* --------------------------------------------------------------------------------------------
     * Grab a memory buffer from the pool.
    */
    void Grab(Pointer & ptr, SzType & size)
    {
        // NOTE: Function assumes (size > 0)
        // Find a buffer large enough to satisfy the requested size
        for (Node * node = m_Head, * prev = NULL; node; prev = node, node = node->mNext)
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
        size = (size & (size - 1)) ? NPow2(size) : size;
        // Allocate the memory directly
        ptr = AllocMem(size);
        // See if the memory could be allocated
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
        if (!s_Nodes)
        {
            Make();
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
MemCat::Node *  MemCat::s_Nodes = NULL;

/* ------------------------------------------------------------------------------------------------
 * Lightweight memory allocator to reduce the overhead of small allocations.
*/
class Memory
{
    // --------------------------------------------------------------------------------------------
    friend class Buffer;
    friend class MemRef;

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
        for (MemCat::Node * node = MemCat::s_Nodes, * next = NULL; node; node = next)
        {
            // Save the next node
            next = node->mNext;
            // Release the node instance
            delete node;
        }
        // Explicitly set the head node to null
        MemCat::s_Nodes = NULL;
    }

private:

    // --------------------------------------------------------------------------------------------
    MemCat m_Small, m_Medium, m_Large;
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
        m_Ptr = NULL;
        m_Ref = NULL;
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
Buffer::Pointer Buffer::s_Ptr = NULL;
Buffer::SzType  Buffer::s_Cap = 0;

// ------------------------------------------------------------------------------------------------
Buffer::Buffer(const Buffer & o)
    : m_Ptr(NULL)
    , m_Cap(0)
    , m_Mem(o.m_Mem)
{
    if (m_Cap)
    {
        Request(o.m_Cap);
        memcpy(m_Ptr, o.m_Ptr, o.m_Cap);
    }
}

// ------------------------------------------------------------------------------------------------
Buffer::~Buffer()
{
    if (m_Ptr)
    {
        Release();
    }
}

// ------------------------------------------------------------------------------------------------
Buffer & Buffer::operator = (const Buffer & o)
{
    if (m_Ptr != o.m_Ptr)
    {
        if (m_Cap && o.m_Cap <= m_Cap)
        {
            memcpy(m_Ptr, o.m_Ptr, m_Cap);
        }
        else if (!o.m_Cap)
        {
            if (m_Ptr)
            {
                Release();
            }
        }
        else
        {
            if (m_Ptr)
            {
                Release();
            }
            Request(o.m_Cap);
            memcpy(m_Ptr, o.m_Ptr, o.m_Cap);
        }
    }

    return *this;
}

// ------------------------------------------------------------------------------------------------
void Buffer::Request(SzType n)
{
    // NOTE: Function assumes (n > 0)
    // Is there a memory manager available?
    if (!m_Mem)
    {
        // Round up the size to a power of two number
        n = (n & (n - 1)) ? NPow2(n) : n;
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
    // If no errors occured then we can set the size
    m_Cap= n;
}

// ------------------------------------------------------------------------------------------------
void Buffer::Release()
{
    // Is there a memory manager available?
    if (!m_Mem)
    {
        // Deallocate the memory directly
        free(m_Ptr);
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
    m_Ptr = NULL;
    m_Cap = 0;
}

// ------------------------------------------------------------------------------------------------
Buffer::SzType Buffer::Write(SzType pos, ConstPtr data, SzType size)
{
    // Make sure the pos is not out of bounds
    if (pos > m_Cap || !data || !size)
    {
        return 0;
    }
    // See if the buffer size must be adjusted
    else if ((pos + size) >= m_Cap)
    {
        // Backup current data
        Buffer bkp = Adjust< Value >(pos + size);
        // Copy data back from the old buffer
        memcpy(m_Ptr, bkp.m_Ptr, bkp.m_Cap);
    }
    // Copy the data into the internal buffer
    memcpy(m_Ptr + pos, data, size);
    // Return the amount of data written to the buffer
    return size;
}

// ------------------------------------------------------------------------------------------------
Buffer::SzType Buffer::WriteF(SzType pos, const char * fmt, ...)
{
    // Make sure the pos is not out of bounds
    if (pos > m_Cap)
    {
        return 0;
    }
    // Initialize the arguments list
    va_list args;
    va_start(args, fmt);
    // Initial attempt to write to the current buffer
    // (if empty, it should tell us the necessary size)
    int ret =  vsnprintf(m_Ptr + pos, m_Cap - pos, fmt, args);
    // Do we need a bigger buffer?
    if ((pos + ret) >= m_Cap)
    {
        // Backup current data
        Buffer bkp = Adjust< Value >(pos + ret);
        // Copy data back from the old buffer
        memcpy(m_Ptr, bkp.m_Ptr, bkp.m_Cap);
        // Argument list was modified during the initial format
        va_end(args);
        va_start(args, fmt);
        // Resume writting the requested information
        ret = vsnprintf(m_Ptr + pos, m_Cap - pos, fmt, args);
    }
    // Finalize the arguments list
    va_end(args);
    // Return the size of the written data in bytes
    return (ret < 0) ? 0 : (SzType)ret;
}

// ------------------------------------------------------------------------------------------------
Buffer::SzType Buffer::WriteF(SzType pos, const char * fmt, va_list args)
{
    // Make sure the pos is not out of bounds
    if (pos > m_Cap)
    {
        return 0;
    }
    // Attempt to write to the current buffer
    int ret =  vsnprintf(m_Ptr + pos, m_Cap - pos, fmt, args);
    // Return the size of the written data in bytes
    return (ret < 0) ? 0 : (SzType)ret;
}

} // Namespace:: SQMod
