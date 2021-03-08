#pragma once

// ------------------------------------------------------------------------------------------------
#include "Library/IO/Buffer.hpp"

// ------------------------------------------------------------------------------------------------
#include <ios>
#include <sstream>
#include <fstream>
#include <iostream>
#include <streambuf>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Implements specialized storage for the specified stream type.
*/
template < class T > struct SqStreamStorage
{
    using Type = T;
    /* --------------------------------------------------------------------------------------------
     * Move constructor (disabled).
    */
    SqStreamStorage(SqStreamStorage && o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Copy constructor (disabled).
    */
    SqStreamStorage(const SqStreamStorage & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator (disabled).
    */
    SqStreamStorage & operator = (SqStreamStorage && o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator (disabled).
    */
    SqStreamStorage & operator = (const SqStreamStorage & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Retrieve a reference to the managed stream.
    */
    inline Type & Stream()
    {
        return m_Stream;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve a constant reference to the managed stream.
    */
    inline const Type & Stream() const
    {
        return m_Stream;
    }

protected:

    /* --------------------------------------------------------------------------------------------
     * Wrapped stream object.
    */
    Type m_Stream;

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    template < class... Args > SqStreamStorage(Args &&... args) // NOLINT(google-explicit-constructor)
        : m_Stream(std::forward< Args >(args)...)
    {
    }
};

/* ------------------------------------------------------------------------------------------------
 * Stream type disambiguation tags.
*/
struct CInTag {
    explicit CInTag() = default;
};
struct COutTag {
    explicit COutTag() = default;
};
struct CErrTag {
    explicit CErrTag() = default;
};
struct CLogTag {
    explicit CLogTag() = default;
};

/* ------------------------------------------------------------------------------------------------
 * Helper used to select the appropriate global stream.
*/
template < class > struct SqGlobalStream;

/* ------------------------------------------------------------------------------------------------
 * Specialization of SqGlobalStream for std::cin.
*/
template < > struct SqGlobalStream< CInTag >
{
    /* --------------------------------------------------------------------------------------------
     * Type of stream used internally.
    */
    using Type = decltype(std::cin);
    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated global stream object.
    */
    static Type & Get() { return std::cin; }
};

/* ------------------------------------------------------------------------------------------------
 * Specialization of SqGlobalStream for std::cout.
*/
template < > struct SqGlobalStream< COutTag >
{
    /* --------------------------------------------------------------------------------------------
     * Type of stream used internally.
    */
    using Type = decltype(std::cout);
    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated global stream object.
    */
    static Type & Get() { return std::cout; }
};

/* ------------------------------------------------------------------------------------------------
 * Specialization of SqGlobalStream for std::cerr.
*/
template < > struct SqGlobalStream< CErrTag >
{
    /* --------------------------------------------------------------------------------------------
     * Type of stream used internally.
    */
    using Type = decltype(std::cerr);
    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated global stream object.
    */
    static Type & Get() { return std::cerr; }
};

/* ------------------------------------------------------------------------------------------------
 * Specialization of SqGlobalStream for std::clog.
*/
template < > struct SqGlobalStream< CLogTag >
{
    /* --------------------------------------------------------------------------------------------
     * Type of stream used internally.
    */
    using Type = decltype(std::clog);
    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated global stream object.
    */
    static Type & Get() { return std::clog; }
};

/* ------------------------------------------------------------------------------------------------
 * Implements specialized storage for stream reference.
*/
template < class T > struct SqStreamStorage< SqGlobalStream< T > >
{
    using Type = typename SqGlobalStream< T >::Type;
    /* --------------------------------------------------------------------------------------------
     * Move constructor (disabled).
    */
    SqStreamStorage(SqStreamStorage && o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Copy constructor (disabled).
    */
    SqStreamStorage(const SqStreamStorage & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator (disabled).
    */
    SqStreamStorage & operator = (SqStreamStorage && o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator (disabled).
    */
    SqStreamStorage & operator = (const SqStreamStorage & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Retrieve a reference to the managed stream.
    */
    inline Type & Stream()
    {
        return m_Stream.get();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve a constant reference to the managed stream.
    */
    inline const Type & Stream() const
    {
        return m_Stream.get();
    }

protected:

    /* --------------------------------------------------------------------------------------------
     * Wrapped stream object.
    */
    std::reference_wrapper< Type > m_Stream;

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    SqStreamStorage()
        : m_Stream(SqGlobalStream< T >::Get())
    {
    }
};

/* ------------------------------------------------------------------------------------------------
 * Stream-based input/output class.
*/
template < class T > struct SqStream : public SqStreamStorage< T >
{
    using SqStreamStorage< T >::Stream;
    // --------------------------------------------------------------------------------------------
    using Type = SqStreamStorage< T >::Type;
    /* --------------------------------------------------------------------------------------------
     * Default string constructor.
    */
    SqStream()
        : SqStreamStorage< T >(), m_Buffer()
    {
    }

    /* --------------------------------------------------------------------------------------------
     * Explicit string constructor.
    */
    explicit SqStream(SQInteger m)
        : SqStreamStorage< T >(static_cast< std::ios_base::openmode >(m)), m_Buffer()
    {
    }

    /* --------------------------------------------------------------------------------------------
     * Base file constructor.
    */
    explicit SqStream(StackStrF & name)
        : SqStreamStorage< T >(name.mPtr), m_Buffer()
    {
    }

    /* --------------------------------------------------------------------------------------------
     * Explicit file constructor.
    */
    SqStream(SQInteger m, StackStrF & name)
        : SqStreamStorage< T >(name.mPtr, static_cast< std::ios_base::openmode >(m)), m_Buffer()
    {
    }

    /* --------------------------------------------------------------------------------------------
     * Manually pre-allocate the size of the internal buffer.
    */
    void AdjustBuffer(SQInteger len)
    {
        m_Buffer.Adjust(ClampL< SQInteger, Buffer::SzType >(len));
    }

    /* --------------------------------------------------------------------------------------------
     * Reclaim memory from the memory buffer.
    */
    void ReclaimBuffer()
    {
        m_Buffer.Reset();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve current formatting setting.
    */
    SQMOD_NODISCARD SQInteger GetFlags() const
    {
        return static_cast< SQInteger >(Stream().flags());
    }

    /* --------------------------------------------------------------------------------------------
     * Modify current settings with given ones.
    */
    void SetFlags(SQInteger f)
    {
        Stream().flags(static_cast< std::ios_base::fmtflags >(f));
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve current field width.
    */
    SQMOD_NODISCARD SQInteger GetWidth() const
    {
        return static_cast< SQInteger >(Stream().width());
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the field width to the given one. Returns the previous field width.
    */
    SQInteger SetWidth(SQInteger w)
    {
        return static_cast< SQInteger >(Stream().width(static_cast< std::streamsize >(w)));
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve current precision.
    */
    SQMOD_NODISCARD SQInteger GetPrecision() const
    {
        return static_cast< SQInteger >(Stream().precision());
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the precision to the given one. Returns the previous precision.
    */
    SQInteger SetPrecision(SQInteger p)
    {
        return static_cast< SQInteger >(Stream().precision(static_cast< std::streamsize >(p)));
    }

    /* --------------------------------------------------------------------------------------------
     * Sets the formatting flags identified by `f`.
    */
    SqStream & SetF(SQInteger f)
    {
        Stream().setf(static_cast< std::ios_base::fmtflags >(f));
        return *this; // Allow chaining
    }

    /* --------------------------------------------------------------------------------------------
     * Clears the formatting flags under `m`, and sets the cleared flags to those specified by `f`.
    */
    SqStream & SetF_(SQInteger f, SQInteger m)
    {
        Stream().setf(static_cast< std::ios_base::fmtflags >(f), static_cast< std::ios_base::fmtflags >(m));
        return *this; // Allow chaining
    }

    /* --------------------------------------------------------------------------------------------
     * Unsets the formatting flags identified by `f`.
    */
    SqStream & UnSetF(SQInteger f)
    {
        Stream().unsetf(static_cast< std::ios_base::fmtflags >(f));
        return *this; // Allow chaining
    }

    /* --------------------------------------------------------------------------------------------
     * Returns true if the most recent I/O operation on the stream completed successfully, false otherwise.
    */
    SQMOD_NODISCARD bool Good() const
    {
        return Stream().good();
    }

    /* --------------------------------------------------------------------------------------------
     * Returns true if the associated stream has reached end-of-file, false otherwise.
    */
    SQMOD_NODISCARD bool EOF_() const
    {
        return Stream().eof();
    }

    /* --------------------------------------------------------------------------------------------
     * Returns true if an error has occurred on the associated stream, false otherwise.
    */
    SQMOD_NODISCARD bool Fail() const
    {
        return Stream().fail();
    }

    /* --------------------------------------------------------------------------------------------
     * Returns true if non-recoverable error has occurred on the associated stream.
    */
    SQMOD_NODISCARD bool Bad() const
    {
        return Stream().bad();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the current fill character.
    */
    SQMOD_NODISCARD SQChar GetFill() const
    {
        return Stream().fill();
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the fill character to `c`, returns previous value of the fill character.
    */
    SQChar SetFill(SQChar c)
    {
        return Stream().fill(c);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the current stream error state.
    */
    SQMOD_NODISCARD SQInteger GetState() const
    {
        return static_cast< SQInteger >(Stream().rdstate());
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the stream error flags state in addition to currently set flags.
    */
    void SetState(SQInteger f)
    {
        return Stream().setstate(static_cast< std::ios_base::iostate >(f));
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the exception mask.
    */
    SQMOD_NODISCARD SQInteger GetExceptions() const
    {
        return static_cast< SQInteger >(Stream().exceptions());
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the exception mask to except.
    */
    void SetExceptions(SQInteger f)
    {
        Stream().exceptions(static_cast< std::ios_base::iostate >(f));
    }

    /* --------------------------------------------------------------------------------------------
     * Sets the stream error state flags by assigning them the value of `SqIoState.GoodBit`.
    */
    SqStream & Clear()
    {
        Stream().clear();
        return *this; // Allow chaining
    }

    /* --------------------------------------------------------------------------------------------
     * Sets the stream error state flags by assigning them the value of `f`.
    */
    SqStream & ClearEx(SQInteger f)
    {
        Stream().clear(static_cast< std::ios_base::iostate >(f));
        return *this; // Allow chaining
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the output position indicator of the current associated `streambuf` object.
    */
    SQMOD_NODISCARD SQInteger TellP()
    {
        return static_cast< SQInteger >(Stream().tellp());
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the output position indicator of the current associated `streambuf` object.
     * Sets the output position indicator to absolute (relative to the beginning of the file) value `p`.
    */
    SqStream & SeekP(SQInteger p)
    {
        Stream().seekp(static_cast< typename Type::pos_type >(p));
        return *this; // Allow chaining
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the output position indicator of the current associated `streambuf` object.
     * Sets the output position indicator to position `o`, relative to position, defined by `d`.
    */
    SqStream & SeekP_(SQInteger o, SQInteger d)
    {
        Stream().seekp(static_cast< typename Type::off_type >(o), static_cast< typename std::ios_base::seekdir >(d));
        return *this; // Allow chaining
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve input position indicator of the current associated `streambuf` object.
    */
    SQMOD_NODISCARD SQInteger TellG()
    {
        return static_cast< SQInteger >(Stream().tellg());
    }

    /* --------------------------------------------------------------------------------------------
     * Modify input position indicator of the current associated `streambuf` object.
     * Sets the input position indicator to absolute (relative to the beginning of the file) value `p`.
    */
    SqStream & SeekG(SQInteger p)
    {
        Stream().seekg(static_cast< typename Type::pos_type >(p));
        return *this; // Allow chaining
    }

    /* --------------------------------------------------------------------------------------------
     * Modify input position indicator of the current associated `streambuf` object.
     * Sets the input position indicator to position `o`, relative to position, defined by `d`.
    */
    SqStream & SeekG_(SQInteger o, SQInteger d)
    {
        Stream().seekg(static_cast< typename Type::off_type >(o), static_cast< typename std::ios_base::seekdir >(d));
        return *this; // Allow chaining
    }

    /* --------------------------------------------------------------------------------------------
     * Write the character `c` to the output stream.
    */
    SqStream & Put(SQInteger c)
    {
        Stream().put(static_cast< typename Type::char_type >(c));
        return *this; // Allow chaining
    }

    /* --------------------------------------------------------------------------------------------
     * Write a string to the output stream.
    */
    SqStream & WriteString(StackStrF & str)
    {
        Stream().write(str.mPtr, static_cast< std::streamsize >(str.mLen));
        return *this; // Allow chaining
    }

    /* --------------------------------------------------------------------------------------------
     * Write a buffer to the output stream.
    */
    SqStream & WriteBuffer(SqBuffer & buf)
    {
        Stream().write(buf.GetInst().Data(), static_cast< std::streamsize >(buf.GetInst().Position()));
        return *this; // Allow chaining
    }

    /* --------------------------------------------------------------------------------------------
     * Write uncommitted changes to the underlying output sequence.
    */
    SqStream & Flush()
    {
        Stream().flush();
        return *this; // Allow chaining
    }

    /* --------------------------------------------------------------------------------------------
     * Reads at most num-1 characters and returns them as character string, until `delim` is found.
    */
    SQMOD_NODISCARD std::streamsize Get(SQInteger num, SQChar delim)
    {
        // Prevent negative lengths
        if (num < 0)
        {
            STHROWF("Invalid length: {} < 0", num);
        }
        // Prevent overflow of Buffer::SzType
        else
        {
            num = static_cast< SQInteger >(ClampL< SQInteger, Buffer::SzType >(num));
        }
        // Allocate a sufficiently large memory buffer
        m_Buffer.Adjust(static_cast< Buffer::SzType >(num));
        // Read data from the stream
        Stream().get(m_Buffer.Get(), static_cast< std::streamsize >(num), static_cast< typename Type::char_type >(delim));
        // Return the number of characters read
        return Stream().gcount();
    }

    /* --------------------------------------------------------------------------------------------
     * Read one character and return it if available.
    */
    SQMOD_NODISCARD SQInteger GetString0()
    {
        return static_cast< SQInteger >(Stream().get());
    }

    /* --------------------------------------------------------------------------------------------
     * Reads at most num-1 characters and returns them as character string, until '\n' is found.
    */
    SQMOD_NODISCARD LightObj GetString1(SQInteger num)
    {
        const std::streamsize len = Get(num, Stream().widen('\n'));
        // Create a string object and return it
        return LightObj(m_Buffer.Get(), static_cast< SQInteger >(len));
    }

    /* --------------------------------------------------------------------------------------------
     * Reads at most num-1 characters and returns them as character string, until `delim` is found.
    */
    SQMOD_NODISCARD LightObj GetString2(SQInteger num, SQChar delim)
    {
        const std::streamsize len = Get(num, delim);
        // Create a string object and return it
        return LightObj(m_Buffer.Get(), static_cast< SQInteger >(len));
    }

    /* --------------------------------------------------------------------------------------------
     * Reads at most num-1 characters and returns them as character string, until '\n' is found.
    */
    SQMOD_NODISCARD LightObj GetBuffer1(SQInteger num)
    {
        const std::streamsize len = Get(num, Stream().widen('\n'));
        // Create a buffer object and return it
        return LightObj(SqTypeIdentity< SqBuffer >{}, SqVM(), m_Buffer.Get(), static_cast< Buffer::SzType >(len));
    }

    /* --------------------------------------------------------------------------------------------
     * Reads at most num-1 characters and returns them as character string, until `delim` is found.
    */
    SQMOD_NODISCARD LightObj GetBuffer2(SQInteger num, SQChar delim)
    {
        const std::streamsize len = Get(num, delim);
        // Create a buffer object and return it
        return LightObj(SqTypeIdentity< SqBuffer >{}, SqVM(), m_Buffer.Get(), static_cast< Buffer::SzType >(len));
    }

    /* --------------------------------------------------------------------------------------------
     * Read the next character from the input stream without extracting it.
    */
    SQMOD_NODISCARD SQInteger Peek()
    {
        return static_cast< SQInteger >(Stream().peek());
    }

    /* --------------------------------------------------------------------------------------------
     * Makes the most recently extracted character available again.
    */
    SqStream & UnGet()
    {
        Stream().unget();
        return *this; // Allow chaining
    }

    /* --------------------------------------------------------------------------------------------
     * Puts the character `c` back to the input stream so the next extracted character will be `c`.
    */
    SqStream & PutBack(SQInteger c)
    {
        Stream().putback(static_cast< typename Type::char_type >(c));
        return *this; // Allow chaining
    }

    /* --------------------------------------------------------------------------------------------
     * Extracts characters from stream until end of line or the specified delimiter `delim`.
    */
    SQMOD_NODISCARD std::streamsize GetLine(SQInteger num, SQChar delim)
    {
        // Prevent negative lengths
        if (num < 0)
        {
            STHROWF("Invalid length: {} < 0", num);
        }
        // Prevent overflow of Buffer::SzType
        else
        {
            num = static_cast< SQInteger >(ClampL< SQInteger, Buffer::SzType >(num));
        }
        // Allocate a sufficiently large memory buffer
        m_Buffer.Adjust(static_cast< Buffer::SzType >(num));
        // Read data from the stream
        Stream().getline(m_Buffer.Get(), static_cast< std::streamsize >(num), static_cast< typename Type::char_type >(delim));
        // What actually stopped the reading?
        if (Stream().rdstate() & (std::ios_base::eofbit | std::ios_base::failbit))
        {
            // Either end of file or end of count stopped the reading
            return Stream().gcount();
        }
        else
        {
            // The delimiter stopped the reading but is only counted towards
            // the number of elements. Not actually read and stored in the buffer
            return Stream().gcount() > 0 ? (Stream().gcount() - 1) : std::streamsize(0);
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Extracts characters from stream until end of line or the specified delimiter '\n'.
    */
    SQMOD_NODISCARD LightObj GetLineString1(SQInteger num)
    {
        const std::streamsize len = GetLine(num, Stream().widen('\n'));
        // Create a string object and return it
        return LightObj(m_Buffer.Get(), static_cast< SQInteger >(len));
    }

    /* --------------------------------------------------------------------------------------------
     * Extracts characters from stream until end of line or the specified delimiter `delim`.
    */
    SQMOD_NODISCARD LightObj GetLineString2(SQInteger num, SQChar delim)
    {
        const std::streamsize len = GetLine(num, delim);
        // Create a string object and return it
        return LightObj(m_Buffer.Get(), static_cast< SQInteger >(len));
    }

    /* --------------------------------------------------------------------------------------------
     * Extracts characters from stream until end of line or the specified delimiter '\n'.
    */
    SQMOD_NODISCARD LightObj GetLineBuffer1(SQInteger num)
    {
        const std::streamsize len = GetLine(num, Stream().widen('\n'));
        // Create a buffer object and return it
        return LightObj(SqTypeIdentity< SqBuffer >{}, SqVM(), m_Buffer.Get(), static_cast< Buffer::SzType >(len));
    }

    /* --------------------------------------------------------------------------------------------
     * Extracts characters from stream until end of line or the specified delimiter `delim`.
    */
    SQMOD_NODISCARD LightObj GetLineBuffer2(SQInteger num, SQChar delim)
    {
        const std::streamsize len = GetLine(num, delim);
        // Create a buffer object and return it
        return LightObj(SqTypeIdentity< SqBuffer >{}, SqVM(), m_Buffer.Get(), static_cast< Buffer::SzType >(len));
    }

    /* --------------------------------------------------------------------------------------------
     * Extracts and discards one character from the input stream until end of file.
    */
    SqStream & Ignore0()
    {
        Stream().ignore();
        return *this; // Allow chaining
    }

    /* --------------------------------------------------------------------------------------------
     * Extracts and discards `num` characters from the input stream until end of file.
    */
    SqStream & Ignore1(SQInteger num)
    {
        Stream().ignore(static_cast< std::streamsize >(num));
        return *this; // Allow chaining
    }

    /* --------------------------------------------------------------------------------------------
     * Extracts and discards characters from the input stream until and including `delim`.
    */
    SqStream & Ignore2(SQInteger num, SQChar delim)
    {
        Stream().ignore(static_cast< std::streamsize >(num), static_cast< typename Type::char_type >(delim));
        return *this; // Allow chaining
    }

    /* --------------------------------------------------------------------------------------------
     * Extracts characters from stream.
    */
    SQMOD_NODISCARD std::streamsize Read(SQInteger num)
    {
        // Prevent negative lengths
        if (num < 0)
        {
            STHROWF("Invalid length: {} < 0", num);
        }
        // Prevent overflow of Buffer::SzType
        else
        {
            num = static_cast< SQInteger >(ClampL< SQInteger, Buffer::SzType >(num));
        }
        // Allocate a sufficiently large memory buffer
        m_Buffer.Adjust(static_cast< Buffer::SzType >(num));
        // Read data from the stream
        Stream().read(m_Buffer.Get(), static_cast< std::streamsize >(num));
        // Return the number of characters read
        return Stream().gcount();
    }

    /* --------------------------------------------------------------------------------------------
     * Extracts characters from stream.
    */
    SQMOD_NODISCARD LightObj ReadString(SQInteger num)
    {
        const std::streamsize len = Read(num);
        // Create a string object and return it
        return LightObj(m_Buffer.Get(), static_cast< SQInteger >(len));
    }

    /* --------------------------------------------------------------------------------------------
     * Extracts characters from stream.
    */
    SQMOD_NODISCARD LightObj ReadBuffer(SQInteger num)
    {
        const std::streamsize len = Read(num);
        // Create a buffer object and return it
        return LightObj(SqTypeIdentity< SqBuffer >{}, SqVM(), m_Buffer.Get(), static_cast< Buffer::SzType >(len));
    }

    /* --------------------------------------------------------------------------------------------
     * Extracts up to `num` immediately available characters from the input stream.
    */
    SQMOD_NODISCARD std::streamsize ReadSome(SQInteger num)
    {
        // Prevent negative lengths
        if (num < 0)
        {
            STHROWF("Invalid length: {} < 0", num);
        }
        // Prevent overflow of Buffer::SzType
        else
        {
            num = static_cast< SQInteger >(ClampL< SQInteger, Buffer::SzType >(num));
        }
        // Allocate a sufficiently large memory buffer
        m_Buffer.Adjust(static_cast< Buffer::SzType >(num));
        // Read data from the stream and return the number of characters read
        return Stream().readsome(m_Buffer.Get(), static_cast< std::streamsize >(num));
    }

    /* --------------------------------------------------------------------------------------------
     * Extracts up to `num` immediately available characters from the input stream.
    */
    SQMOD_NODISCARD LightObj ReadSomeString(SQInteger num)
    {
        const std::streamsize len = ReadSome(num);
        // Create a string object and return it
        return LightObj(m_Buffer.Get(), static_cast< SQInteger >(len));
    }

    /* --------------------------------------------------------------------------------------------
     * Extracts up to `num` immediately available characters from the input stream.
    */
    SQMOD_NODISCARD LightObj ReadSomeBuffer(SQInteger num)
    {
        const std::streamsize len = ReadSome(num);
        // Create a buffer object and return it
        return LightObj(SqTypeIdentity< SqBuffer >{}, SqVM(), m_Buffer.Get(), static_cast< Buffer::SzType >(len));
    }

    /* --------------------------------------------------------------------------------------------
     * Returns the number of characters extracted by the last unformatted input operation.
    */
    SQMOD_NODISCARD SQInteger GCount() const
    {
        return static_cast< SQInteger >(Stream().gcount());
    }

    /* --------------------------------------------------------------------------------------------
     * Returns a copy of the underlying string.
    */
    SQMOD_NODISCARD String GetStr() const
    {
        return Stream().str();
    }

    /* --------------------------------------------------------------------------------------------
     * Replaces the contents of the underlying string.
    */
    void SetStr(StackStrF & s)
    {
        Stream().str(s.ToStr());
    }

    /* --------------------------------------------------------------------------------------------
     * Check if the file stream has an associated file.
    */
    SQMOD_NODISCARD bool IsOpen() const
    {
        return Stream().is_open();
    }

    /* --------------------------------------------------------------------------------------------
     * Open and associate the file with name `name` with the file stream.
    */
    SqStream & Open(StackStrF & name)
    {
        Stream().open(name.mPtr);
        return *this; // Allow chaining
    }

    /* --------------------------------------------------------------------------------------------
     * Open and associate the file with name `name` with the file stream.
    */
    SqStream & Open_(SQInteger m, StackStrF & name)
    {
        Stream().open(name.mPtr, static_cast< std::ios_base::openmode >(m));
        return *this; // Allow chaining
    }

    /* --------------------------------------------------------------------------------------------
     * Close the associated file.
    */
    SqStream & Close()
    {
        Stream().close();
        return *this; // Allow chaining
    }

protected:

    /* --------------------------------------------------------------------------------------------
     * Reusable memory buffer used internally to avoid having to allocate one repeatedly.
    */
    Buffer m_Buffer;
};

} // Namespace:: SqMod
