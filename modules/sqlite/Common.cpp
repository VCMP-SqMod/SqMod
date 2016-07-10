// ------------------------------------------------------------------------------------------------
#include "Common.hpp"
#include "Connection.hpp"
#include "Statement.hpp"

// ------------------------------------------------------------------------------------------------
#include <cstdio>
#include <cstring>
#include <cstdarg>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
Object GetConnectionObj(const ConnRef & conn)
{
    return Object(new Connection(conn));
}

// ------------------------------------------------------------------------------------------------
Object GetStatementObj(const StmtRef & stmt)
{
    return Object(new Statement(stmt));
}

// ------------------------------------------------------------------------------------------------
CSStr QFmtStr(CSStr str, ...)
{
    // Initialize the argument list
    va_list args;
    va_start (args, str);
    // Write the requested contents
    sqlite3_vsnprintf(GetTempBuffSize(), GetTempBuff(), str, args);
    // Release the argument list
    va_end(args);
    // Return the data from the buffer
    return GetTempBuff();
}

// ------------------------------------------------------------------------------------------------
bool IsQueryEmpty(CSStr str)
{
    // Is the pointer valid?
    if (!str)
    {
        return true;
    }
    // Currently processed character
    SQChar c = 0;
    // See if the query contains any alpha numeric characters
    while ((c = *str) != 0)
    {
        if (std::isalnum(c) != 0)
        {
            return false;
        }
        ++str;
    }
    // At this point we consider the query empty
    return true;
}

// ------------------------------------------------------------------------------------------------
CSStr GetErrStr(Int32 status)
{
    return sqlite3_errstr(status);
}

// ------------------------------------------------------------------------------------------------
void SetSoftHeapLimit(Int32 limit)
{
    sqlite3_soft_heap_limit(limit);
}

// ------------------------------------------------------------------------------------------------
Int32 ReleaseMemory(Int32 bytes)
{
    return sqlite3_release_memory(bytes);
}

// ------------------------------------------------------------------------------------------------
Object GetMemoryUsage()
{
    // Obtain the initial stack size
    const StackGuard sg(_SqVM);
    // Push a long integer instance with the requested value on the stack
    SqMod_PushSLongObject(_SqVM, sqlite3_memory_used());
    // Obtain the object from the stack and return it
    return Var< Object >(_SqVM, -1).value;
}

// ------------------------------------------------------------------------------------------------
Object GetMemoryHighwaterMark(bool reset)
{
    // Obtain the initial stack size
    const StackGuard sg(_SqVM);
    // Push a long integer instance with the requested value on the stack
    SqMod_PushSLongObject(_SqVM, sqlite3_memory_highwater(reset));
    // Obtain the object from the stack and return it
    return Var< Object >(_SqVM, -1).value;
}

// ------------------------------------------------------------------------------------------------
CSStr EscapeString(CSStr str)
{
    // Is there even a string to escape?
    if (!str)
    {
        return _SC(""); // Default to empty string
    }
    // Attempt to escape the specified string
    sqlite3_snprintf(GetTempBuffSize(), GetTempBuff(), "%q", str);
    // Return the resulted string
    return GetTempBuff();
}

// ------------------------------------------------------------------------------------------------
CCStr EscapeStringEx(SQChar spec, CCStr str)
{
    // Utility that allows changing the format specifier temporarily
    static SQChar fs[] = _SC("%q");
    // Validate the specified format specifier
    if ((spec != 'q') && (spec != 'Q') && (spec != 'w') && (spec != 's'))
    {
        STHROWF("Unknown format specifier: '%c'", spec);
    }
    // Is there even a string to escape?
    else if (!str)
    {
        return _SC(""); // Default to empty string
    }
    // Apply the format specifier
    fs[1] = spec;
    // Attempt to escape the specified string
    sqlite3_snprintf(GetTempBuffSize(), GetTempBuff(), fs, str);
    // Restore the format specifier
    fs[1] = 'q';
    // Return the resulted string
    return GetTempBuff();
}

// ------------------------------------------------------------------------------------------------
CCStr ArrayToQueryColumns(Array & arr)
{
    // Do we even have any elements to process?
    if (arr.Length() <= 0)
    {
        return _SC(""); // Default to empty string
    }
    // Allocate a vector with the required amount of column names
    std::vector< String > values(arr.Length());
    // Attempt to extract the array elements as strings
    arr.GetArray< String >(&values[0], values.size());
    // Used to know the position of the next column name
    Uint32 offset = 0;
    // Obtain the start of the array
    std::vector< String >::iterator itr = values.begin();
    // Process all elements within range
    for (; itr != values.end() && offset < GetTempBuffSize(); ++itr)
    {
        // Is the name valid?
        if (itr->empty())
        {
            STHROWF("Invalid column name");
        }
        // Attempt to append the column name to the buffer
        sqlite3_snprintf(GetTempBuffSize() - offset, GetTempBuff() + offset, "[%q], ", itr->c_str());
        // Add the column name size to the offset
        offset += itr->size();
        // Also include the comma and space in the offset
        offset += 2;
    }
    // Trim the last coma and space
    if (offset >= 2)
    {
        GetTempBuff()[offset-2] = '\0';
    }
    else
    {
        GetTempBuff()[0] = '\0';
    }
    // Return the resulted string
    return GetTempBuff();
}

// ------------------------------------------------------------------------------------------------
CCStr TableToQueryColumns(Table & tbl)
{
    // Used to know the position of the next column name
    Uint32 offset = 0;
    // Used to obtain the column name temporarily
    String name;
    // Obtain the start of the table
    Table::iterator itr;
    // Process all elements within range
    while (tbl.Next(itr))
    {
        // Use the element key as the column name
        name.assign(itr.getName());
        // Is the name valid?
        if (name.empty())
        {
            STHROWF("Invalid or empty column name");
        }
        // Attempt to append the column name to the buffer
        sqlite3_snprintf(GetTempBuffSize() - offset, GetTempBuff() + offset, "[%q], ", name.c_str());
        // Add the column name size to the offset
        offset += name.size();
        // Also include the comma and space in the offset
        offset += 2;
    }
    // Trim the last coma and space
    if (offset >= 2)
    {
        GetTempBuff()[offset-2] = '\0';
    }
    else
    {
        GetTempBuff()[0] = '\0';
    }
    // Return the resulted string
    return GetTempBuff();
}

// ================================================================================================
void Register_Common(Table & sqlns)
{
    sqlns.Func(_SC("IsQueryEmpty"), &IsQueryEmpty)
        .Func(_SC("GetErrStr"), &GetErrStr)
        .Func(_SC("SetSoftHeapLimit"), &SetSoftHeapLimit)
        .Func(_SC("ReleaseMemory"), &ReleaseMemory)
        .Func(_SC("MemoryUsage"), &GetMemoryUsage)
        .Func(_SC("EscapeString"), &EscapeString)
        .Func(_SC("EscapeStringEx"), &EscapeStringEx)
        .Func(_SC("Escape"), &EscapeString)
        .Func(_SC("EscapeEx"), &EscapeStringEx)
        .Func(_SC("ArrayToQueryColumns"), &ArrayToQueryColumns)
        .Func(_SC("TableToQueryColumns"), &TableToQueryColumns);
}

} // Namespace:: SqMod
