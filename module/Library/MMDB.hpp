#pragma once

// ------------------------------------------------------------------------------------------------
#include "Base/Shared.hpp"

// ------------------------------------------------------------------------------------------------
#include "Library/Numeric/LongInt.hpp"
#include "Library/Utils/Buffer.hpp"

// ------------------------------------------------------------------------------------------------
#include <vector>

// ------------------------------------------------------------------------------------------------
#include <maxminddb.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Handle validation.
*/
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    #define SQMOD_VALIDATE(x)               (x).Validate(__FILE__, __LINE__)
    #define SQMOD_GET_VALID(x)              (x).GetValid(__FILE__, __LINE__)
    #define SQMOD_GET_VALID_ELEM(x)         (x).GetValidElem(__FILE__, __LINE__)
    #define SQMOD_GET_VALID_DATA(x)         (x).GetValidData(__FILE__, __LINE__)
#else
    #define SQMOD_VALIDATE(x)               (x).Validate()
    #define SQMOD_GET_VALID(x)              (x).GetValid()
    #define SQMOD_GET_VALID_ELEM(x)         (x).GetValidElem()
    #define SQMOD_GET_VALID_DATA(x)         (x).GetValidData()
#endif // _DEBUG

/* ------------------------------------------------------------------------------------------------
 * Forward declarations.
*/
class Database;
class Metadata;
class Description;
class SockAddr;
class EntryData;
class EntryDataList;
class LookupResult;
class SearchNode;

/* ------------------------------------------------------------------------------------------------
 * Forward handle declarations.
*/
struct DbHnd;

/* ------------------------------------------------------------------------------------------------
 * Common typedefs.
*/
typedef SharedPtr< DbHnd > DbRef;

/* ------------------------------------------------------------------------------------------------
 * Used to retrieve the string representation of the specified type identifier.
*/
CSStr AsTypeStr(Uint32 id);

/* ------------------------------------------------------------------------------------------------
 * Retrieve the value from the specified entry data as a boolean.
*/
bool GetEntryAsBool(const MMDB_entry_data_s & ed);

/* ------------------------------------------------------------------------------------------------
 * Retrieve the value from the specified entry data as a native integer.
*/
SQInteger GetEntryAsInteger(const MMDB_entry_data_s & ed);

/* ------------------------------------------------------------------------------------------------
 * Retrieve the value from the specified entry data as a floating point.
*/
SQFloat GetEntryAsFloat(const MMDB_entry_data_s & ed);

/* ------------------------------------------------------------------------------------------------
 * Retrieve the value from the specified entry data as a long integer.
*/
LightObj GetEntryAsLong(const MMDB_entry_data_s & ed);

/* ------------------------------------------------------------------------------------------------
 * Retrieve the value from the specified entry data as a string.
*/
LightObj GetEntryAsString(const MMDB_entry_data_s & ed);

/* ------------------------------------------------------------------------------------------------
 * Retrieve the value from the specified entry data as a stream of bytes.
*/
LightObj GetEntryAsBytes(const MMDB_entry_data_s & ed);

/* ------------------------------------------------------------------------------------------------
 * Class that can obtain information from string addresses and be used repeatedly thereafter.
*/
class SockAddr
{
public:

    // --------------------------------------------------------------------------------------------
    typedef struct addrinfo Type; // The managed type.

    // --------------------------------------------------------------------------------------------
    typedef Type*           Pointer; // Pointer to the managed type.
    typedef const Type*     ConstPtr; // Constant pointer to the managed type.

    // --------------------------------------------------------------------------------------------
    typedef Type&           Reference; // Reference to the managed type.
    typedef const Type&     ConstRef; // Constant reference to the managed type.

protected:

    /* --------------------------------------------------------------------------------------------
     * Validate the managed sockaddr pointer and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    void Validate(CCStr file, Int32 line) const;
#else
    void Validate() const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Validate the managed sockaddr pointer and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    Pointer GetValid(CCStr file, Int32 line) const;
#else
    Pointer GetValid() const;
#endif // _DEBUG

private:

    // ---------------------------------------------------------------------------------------------
    Pointer m_Handle; // The managed sockaddr structure.
    String  m_Addres; // The address that was queried for information.

public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    SockAddr()
        : m_Handle(nullptr), m_Addres(_SC(""))
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    SockAddr(CSStr addr);

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    SockAddr(const SockAddr & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    SockAddr(SockAddr && o)
        : m_Handle(o.m_Handle)
        , m_Addres(o.m_Addres)
    {
        o.m_Handle = nullptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~SockAddr();

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator. (disabled)
    */
    SockAddr & operator = (const SockAddr & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    SockAddr & operator = (SockAddr && o)
    {
        if (m_Handle != o.m_Handle)
        {
            m_Handle = o.m_Handle;
            m_Addres = o.m_Addres;
            o.m_Handle = nullptr;
        }
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the internal addrinfo structure pointer.
    */
    Pointer GetHandle()
    {
        return m_Handle;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the internal addrinfo structure pointer.
    */
    Pointer GetHandle() const
    {
        return m_Handle;
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to convert an instance of this type to a string.
    */
    CSStr ToString() const
    {
        return m_Addres.c_str();
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to retrieve the name from instances of this type.
    */
    static SQInteger Typename(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * See whether this instance references a valid addrinfo structure.
    */
    bool IsValid() const
    {
        return m_Handle;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated string address.
    */
    CSStr GetAddress() const
    {
        return m_Addres.c_str();
    }
};

/* ------------------------------------------------------------------------------------------------
 * Manages a reference counted database instance.
*/
class DbHnd
{
    // --------------------------------------------------------------------------------------------
    friend class Database;

public:

    // --------------------------------------------------------------------------------------------
    typedef MMDB_s          Type; // The managed type.

    // --------------------------------------------------------------------------------------------
    typedef Type*           Pointer; // Pointer to the managed type.
    typedef const Type*     ConstPtr; // Constant pointer to the managed type.

    // --------------------------------------------------------------------------------------------
    typedef Type&           Reference; // Reference to the managed type.
    typedef const Type&     ConstRef; // Constant reference to the managed type.

public:

    // --------------------------------------------------------------------------------------------
    MMDB_s     mDb; // The managed database handle.

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    DbHnd(CSStr filepath, Uint32 flags);

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    DbHnd(const DbHnd & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move constructor. (disabled)
    */
    DbHnd(DbHnd && o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~DbHnd();

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator. (disabled)
    */
    DbHnd & operator = (const DbHnd & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator. (disabled)
    */
    DbHnd & operator = (DbHnd && o) = delete;
};

/* ------------------------------------------------------------------------------------------------
 * Class that can be used to open and query information from MaxMind database files.
*/
class Database
{
public:

    // --------------------------------------------------------------------------------------------
    typedef DbHnd::Type             Type; // The managed type.

    // --------------------------------------------------------------------------------------------
    typedef Type*                   Pointer; // Pointer to the managed type.
    typedef const Type*             ConstPtr; // Constant pointer to the managed type.

    // --------------------------------------------------------------------------------------------
    typedef Type&                   Reference; // Reference to the managed type.
    typedef const Type&             ConstRef; // Constant reference to the managed type.

protected:

    /* --------------------------------------------------------------------------------------------
     * Validate the managed database handle and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    void Validate(CCStr file, Int32 line) const;
#else
    void Validate() const;
#endif //

    /* --------------------------------------------------------------------------------------------
     * Validate the managed database handle and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    const DbRef & GetValid(CCStr file, Int32 line) const;
#else
    const DbRef & GetValid() const;
#endif // _DEBUG

private:

    // ---------------------------------------------------------------------------------------------
    DbRef m_Handle; // The managed database handle.

public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor. (null)
    */
    Database()
        : m_Handle()
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Base constructor. (default flags)
    */
    Database(CSStr filepath)
        : m_Handle(new DbHnd(filepath, 0))
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    Database(CSStr filepath, Uint32 flags)
        : m_Handle(new DbHnd(filepath, flags))
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Explicit handle constructor.
    */
    Database(const DbRef & db)
        : m_Handle(db)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    Database(const Database & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    Database(Database && o) = default;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    Database & operator = (const Database & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    Database & operator = (Database && o) = default;

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to convert an instance of this type to a string.
    */
    CSStr ToString() const
    {
        return m_Handle ? m_Handle->mDb.filename : _SC("");
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to retrieve the name from instances of this type.
    */
    static SQInteger Typename(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * See whether this instance references a valid database instance.
    */
    bool IsValid() const
    {
        return m_Handle;
    }

    /* --------------------------------------------------------------------------------------------
     * Release the manages handles/pointers and become a null instance.
    */
    void Release()
    {
        m_Handle.Reset();
    }

    /* --------------------------------------------------------------------------------------------
     * Return the number of active references to the managed database instance.
    */
    Uint32 GetRefCount() const
    {
        return m_Handle.Count();
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to open the specified database.
    */
    void Open(CSStr filepath)
    {
        Open(filepath, 0);
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to open the specified database.
    */
    void Open(CSStr filepath, Uint32 flags)
    {
        // Make sure there isn't another database handle
        if (!m_Handle)
        {
            m_Handle = DbRef(new DbHnd(filepath, flags));
        }
        else
        {
            STHROWF("Loading is disabled while database is referenced");
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the metadata associated with the managed database handle.
    */
    Metadata GetMetadata() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the metadata associated with the managed database handle as an entry data list.
    */
    Object GetMetadataAsEntryDataList() const;

    /* --------------------------------------------------------------------------------------------
     * Look up an IP address that is passed in as a null-terminated string.
    */
    LookupResult LookupString(CSStr addr);

    /* --------------------------------------------------------------------------------------------
     * Looks up an IP address that has already been resolved by getaddrinfo().
    */
    LookupResult LookupSockAddr(SockAddr & sockaddr);

    /* --------------------------------------------------------------------------------------------
     * Retrieve a speciffic node from the managed database.
    */
    SearchNode ReadNode(Uint32 node) const;
};

/* ------------------------------------------------------------------------------------------------
 * Class that can hold and be used inspect meta-data descriptions.
*/
class Description
{
public:

    // --------------------------------------------------------------------------------------------
    typedef MMDB_description_s      Type; // The managed type.

    // --------------------------------------------------------------------------------------------
    typedef Type*                   Pointer; // Pointer to the managed type.
    typedef const Type*             ConstPtr; // Constant pointer to the managed type.

    // --------------------------------------------------------------------------------------------
    typedef Type&                   Reference; // Reference to the managed type.
    typedef const Type&             ConstRef; // Constant reference to the managed type.

protected:

    /* --------------------------------------------------------------------------------------------
     * Validate the managed database handle and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    void Validate(CCStr file, Int32 line) const;
#else
    void Validate() const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Validate the managed database handle and description pointer and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    Pointer GetValid(CCStr file, Int32 line) const;
#else
    Pointer GetValid() const;
#endif // _DEBUG

private:

    // ---------------------------------------------------------------------------------------------
    DbRef   m_Handle; // The database associated with this meta-data description.
    Pointer m_Description; // The inspected meta-data description structure.

public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor. (null)
    */
    Description()
        : m_Handle(), m_Description(nullptr)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Construct and with a specific meta-data description.
    */
    Description(const DbRef & db, Pointer description)
        : m_Handle(db), m_Description(description)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    Description(const Description &) = default;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    Description(Description &&) = default;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    Description & operator = (const Description &) = default;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    Description & operator = (Description &&) = default;

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to convert an instance of this type to a string.
    */
    CSStr ToString() const
    {
        return m_Description ? m_Description->description : _SC("");
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to retrieve the name from instances of this type.
    */
    static SQInteger Typename(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * See whether this instance references a valid database and description structure.
    */
    bool IsValid() const
    {
        return m_Handle; // If there's a database handle then there's a description too
    }

    /* --------------------------------------------------------------------------------------------
     * Release the manages handles/pointers and become a null instance.
    */
    void Release()
    {
        m_Handle.Reset();
        m_Description = nullptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the database associated with the managed handle/pointer.
    */
    Database GetDatabase() const;

    /* --------------------------------------------------------------------------------------------
     * Return the number of active references to the managed database instance.
    */
    Uint32 GetRefCount() const
    {
        return m_Handle.Count();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value of the managed description handle.
    */
    CSStr GetDescriptionValue() const
    {
        return SQMOD_GET_VALID(*this)->description;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the language of the managed description handle.
    */
    CSStr GetDescriptionLanguage() const
    {
        return SQMOD_GET_VALID(*this)->language;
    }
};

/* ------------------------------------------------------------------------------------------------
 * Class that can hold and be used to inspect entry data values.
*/
class EntryData
{
public:

    // --------------------------------------------------------------------------------------------
    typedef MMDB_entry_data_s       Type; // The managed type.

    // --------------------------------------------------------------------------------------------
    typedef Type*                   Pointer; // Pointer to the managed type.
    typedef const Type*             ConstPtr; // Constant pointer to the managed type.

    // --------------------------------------------------------------------------------------------
    typedef Type&                   Reference; // Reference to the managed type.
    typedef const Type&             ConstRef; // Constant reference to the managed type.

protected:

    /* --------------------------------------------------------------------------------------------
     * Validate the managed database handle and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    void Validate(CCStr file, Int32 line) const;
#else
    void Validate() const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Validate the managed database handle and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    ConstRef GetValid(CCStr file, Int32 line) const;
#else
    ConstRef GetValid() const;
#endif // _DEBUG

private:

    // ---------------------------------------------------------------------------------------------
    DbRef   m_Handle; // The database from which this result comes from.
    Type    m_Entry; // The managed entry-data structure.

public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor. (null)
    */
    EntryData();

    /* --------------------------------------------------------------------------------------------
     * Construct and take ownership of a certain entry data.
    */
    EntryData(const DbRef & db, Reference entry)
        : m_Handle(db), m_Entry(entry)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    EntryData(const EntryData &) = default;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    EntryData(EntryData &&) = default;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    EntryData & operator = (const EntryData &) = default;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    EntryData & operator = (EntryData &&) = default;

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to convert an instance of this type to a string.
    */
    CSStr ToString() const
    {
        return AsTypeStr(m_Entry.type);
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to retrieve the name from instances of this type.
    */
    static SQInteger Typename(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * See whether this instance references a valid database and entry structure.
    */
    bool IsValid() const
    {
        return m_Handle && m_Entry.has_data;
    }

    /* --------------------------------------------------------------------------------------------
     * Release the manages handles/pointers and become a null instance.
    */
    void Release();

    /* --------------------------------------------------------------------------------------------
     * Retrieve the database associated with the managed handle/pointer.
    */
    Database GetDatabase() const;

    /* --------------------------------------------------------------------------------------------
     * Return the number of active references to the managed database instance.
    */
    Uint32 GetRefCount() const
    {
        return m_Handle.Count();
    }

    /* --------------------------------------------------------------------------------------------
     * Used to retrieve the type of the current element as a string.
    */
    CSStr TypeName() const
    {
        // Validate the handle
        SQMOD_VALIDATE(*this);
        // Return the requested information
        return AsTypeStr(m_Entry.type);
    }

    /* --------------------------------------------------------------------------------------------
     * See whether a valid element is currently processed.
    */
    bool HasData() const
    {
        return ConvTo< bool >::From(SQMOD_GET_VALID(*this).has_data);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the type identifier of the current element.
    */
    SQInteger GetType() const
    {
        return ConvTo< SQInteger >::From(SQMOD_GET_VALID(*this).type);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the offset of the current element.
    */
    SQInteger GetOffset() const
    {
        return ConvTo< SQInteger >::From(SQMOD_GET_VALID(*this).offset);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the offset of the next element.
    */
    SQInteger DataSize() const
    {
        return ConvTo< SQInteger >::From(SQMOD_GET_VALID(*this).data_size);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value from the current element as a boolean.
    */
    bool GetBool() const
    {
        return GetEntryAsBool(SQMOD_GET_VALID(*this));
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value from the current element as a native integer.
    */
    SQInteger GetInteger() const
    {
        return GetEntryAsInteger(SQMOD_GET_VALID(*this));
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value from the current element as a floating point.
    */
    SQFloat GetFloat() const
    {
        return GetEntryAsFloat(SQMOD_GET_VALID(*this));
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value from the current element as a long integer.
    */
    LightObj GetLong() const
    {
        return GetEntryAsLong(SQMOD_GET_VALID(*this));
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value from the current element as a string.
    */
    LightObj GetString() const
    {
        return GetEntryAsString(SQMOD_GET_VALID(*this));
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value from the current element as a stream of bytes.
    */
    LightObj GetBytes() const
    {
        return GetEntryAsBytes(SQMOD_GET_VALID(*this));
    }
};

/* ------------------------------------------------------------------------------------------------
 * Class that can hold and be used inspect database meta-data.
*/
class EntryDataList
{
public:

    // --------------------------------------------------------------------------------------------
    typedef MMDB_entry_data_list_s  Type; // The managed type.

    // --------------------------------------------------------------------------------------------
    typedef Type*                   Pointer; // Pointer to the managed type.
    typedef const Type*             ConstPtr; // Constant pointer to the managed type.

    // --------------------------------------------------------------------------------------------
    typedef Type&                   Reference; // Reference to the managed type.
    typedef const Type&             ConstRef; // Constant reference to the managed type.

protected:

    /* --------------------------------------------------------------------------------------------
     * Validate the managed database handle and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    void Validate(CCStr file, Int32 line) const;
#else
    void Validate() const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Validate the managed database handle and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    Pointer GetValid(CCStr file, Int32 line) const;
#else
    Pointer GetValid() const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Validate the managed database handle and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    Pointer GetValidElem(CCStr file, Int32 line) const;
#else
    Pointer GetValidElem() const;
#endif // _DEBUG

private:

    // ---------------------------------------------------------------------------------------------
    DbRef   m_Handle; // The database associated with this meta-data.
    Pointer m_List; // The managed entry data list.
    Pointer m_Elem; // The currently processed element from the list.

public:

    /* --------------------------------------------------------------------------------------------
     * Construct and with a specific entry list.
    */
    EntryDataList(const DbRef & db, Pointer list)
        : m_Handle(db), m_List(list), m_Elem(list)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Default constructor. (null)
    */
    EntryDataList()
        : m_Handle(), m_List(nullptr), m_Elem(nullptr)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    EntryDataList(const EntryDataList &) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    EntryDataList(EntryDataList && o)
        : m_Handle(o.m_Handle)
        , m_List(o.m_List)
        , m_Elem(o.m_Elem)
    {
        o.m_List = nullptr;
        o.m_Elem = nullptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~EntryDataList()
    {
        // Do we have to free any list?
        if (m_List)
        {
            MMDB_free_entry_data_list(m_List);
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator. (disabled)
    */
    EntryDataList & operator = (const EntryDataList &) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    EntryDataList & operator = (EntryDataList && o)
    {
        if (m_List != o.m_List)
        {
            m_Handle = o.m_Handle;
            m_List = o.m_List;
            m_Elem = o.m_Elem;
            o.m_List = nullptr;
            o.m_Elem = nullptr;
        }
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to convert an instance of this type to a string.
    */
    CSStr ToString() const
    {
        return m_Elem ? AsTypeStr(m_Elem->entry_data.type) : _SC("invalid");
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to retrieve the name from instances of this type.
    */
    static SQInteger Typename(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * See whether this instance references a valid database and element pointer.
    */
    bool IsValid() const
    {
        return m_Handle && m_Elem;
    }

    /* --------------------------------------------------------------------------------------------
     * Release the manages handles/pointers and become a null instance.
    */
    void Release()
    {
        m_Handle.Reset();
        // Do we have to free any list?
        if (m_List)
        {
            MMDB_free_entry_data_list(m_List);
        }
        // Finally, release those as well
        m_List = nullptr;
        m_Elem = nullptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the database associated with the managed handle/pointer.
    */
    Database GetDatabase() const;

    /* --------------------------------------------------------------------------------------------
     * Return the number of active references to the managed database instance.
    */
    Uint32 GetRefCount() const
    {
        return m_Handle.Count();
    }

    /* --------------------------------------------------------------------------------------------
     * See whether a valid element is currently processed.
    */
    bool HaveElement() const
    {
        return m_Elem;
    }

    /* --------------------------------------------------------------------------------------------
     * Used to retrieve the type of the current element as a string.
    */
    CSStr TypeName() const
    {
        return AsTypeStr(SQMOD_GET_VALID_ELEM(*this)->entry_data.type);
    }

    /* --------------------------------------------------------------------------------------------
     * Return the total entries in the list.
    */
    Uint32 GetCount() const;

    /* --------------------------------------------------------------------------------------------
     * Go to the next element.
    */
    bool Next();

    /* --------------------------------------------------------------------------------------------
     * Advance a certain number of elements.
    */
    bool Advance(SQInteger n);

    /* --------------------------------------------------------------------------------------------
     * Go back to the first element in the list.
    */
    void Reset()
    {
        m_Elem = SQMOD_GET_VALID_ELEM(*this);
    }

    /* --------------------------------------------------------------------------------------------
     * See whether a valid element is currently processed.
    */
    bool HasData() const
    {
        return ConvTo< bool >::From(SQMOD_GET_VALID_ELEM(*this)->entry_data.has_data);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the type identifier of the current element.
    */
    SQInteger GetType() const
    {
        return ConvTo< SQInteger >::From(SQMOD_GET_VALID_ELEM(*this)->entry_data.type);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the offset of the current element.
    */
    SQInteger GetOffset() const
    {
        return ConvTo< SQInteger >::From(SQMOD_GET_VALID_ELEM(*this)->entry_data.offset);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the offset of the next element.
    */
    SQInteger DataSize() const
    {
        return ConvTo< SQInteger >::From(SQMOD_GET_VALID_ELEM(*this)->entry_data.data_size);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value from the current element as a boolean.
    */
    bool GetBool() const
    {
        return GetEntryAsBool(SQMOD_GET_VALID_ELEM(*this)->entry_data);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value from the current element as a native integer.
    */
    SQInteger GetInteger() const
    {
        return GetEntryAsInteger(SQMOD_GET_VALID_ELEM(*this)->entry_data);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value from the current element as a floating point.
    */
    SQFloat GetFloat() const
    {
        return GetEntryAsFloat(SQMOD_GET_VALID_ELEM(*this)->entry_data);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value from the current element as a long integer.
    */
    LightObj GetLong() const
    {
        return GetEntryAsLong(SQMOD_GET_VALID_ELEM(*this)->entry_data);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value from the current element as a string.
    */
    LightObj GetString() const
    {
        return GetEntryAsString(SQMOD_GET_VALID_ELEM(*this)->entry_data);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value from the current element as a stream of bytes.
    */
    LightObj GetBytes() const
    {
        return GetEntryAsBytes(SQMOD_GET_VALID_ELEM(*this)->entry_data);
    }

    /* --------------------------------------------------------------------------------------------
     * Dumpt the contents of the list to the specified list.
    */
    void DumpTo(CSStr filepath) const
    {
        DumpTo(filepath, 0);
    }

    /* --------------------------------------------------------------------------------------------
     * Dumpt the contents of the list to the specified list.
    */
    void DumpTo(CSStr filepath, Int32 indent) const;
};

/* ------------------------------------------------------------------------------------------------
 * Class that can hold and be used to work with lookup results.
*/
class LookupResult
{
public:

    // --------------------------------------------------------------------------------------------
    typedef MMDB_lookup_result_s    Type; // The managed type.

    // --------------------------------------------------------------------------------------------
    typedef Type*                   Pointer; // Pointer to the managed type.
    typedef const Type*             ConstPtr; // Constant pointer to the managed type.

    // --------------------------------------------------------------------------------------------
    typedef Type&                   Reference; // Reference to the managed type.
    typedef const Type&             ConstRef; // Constant reference to the managed type.

protected:

    /* --------------------------------------------------------------------------------------------
     * Validate the managed database handle and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    void Validate(CCStr file, Int32 line) const;
#else
    void Validate() const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Validate the managed database handle and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    const DbRef & GetValid(CCStr file, Int32 line) const;
#else
    const DbRef & GetValid() const;
#endif // _DEBUG

private:

    // ---------------------------------------------------------------------------------------------
    DbRef   m_Handle; // The database from which this result comes from.
    Type    m_Result; // The managed result structure.

public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor. (null)
    */
    LookupResult();

    /* --------------------------------------------------------------------------------------------
     * Construct and take ownership of a certain result.
    */
    LookupResult(const DbRef & db, Reference result)
        : m_Handle(db), m_Result(result)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    LookupResult(const LookupResult &) = default;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    LookupResult(LookupResult &&) = default;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    LookupResult & operator = (const LookupResult &) = default;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    LookupResult & operator = (LookupResult &&) = default;

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to convert an instance of this type to a string.
    */
    CSStr ToString() const
    {
        return FmtStr("%u", m_Result.entry.offset);
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to retrieve the name from instances of this type.
    */
    static SQInteger Typename(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * See whether this instance references a valid database and result structure.
    */
    bool IsValid() const
    {
        return m_Handle && m_Result.found_entry;
    }

    /* --------------------------------------------------------------------------------------------
     * Release the manages handles/pointers and become a null instance.
    */
    void Release();

    /* --------------------------------------------------------------------------------------------
     * Retrieve the database associated with the managed handle/pointer.
    */
    Database GetDatabase() const;

    /* --------------------------------------------------------------------------------------------
     * Return the number of active references to the managed database instance.
    */
    Uint32 GetRefCount() const
    {
        return m_Handle.Count();
    }

    /* --------------------------------------------------------------------------------------------
     * See whether the result contains a valid entry in the associated database.
    */
    bool FoundEntry() const
    {
        // Validate the database handle
        SQMOD_VALIDATE(*this);
        // Return the requested information
        return m_Result.found_entry;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the net-mask from the result structure.
    */
    SQInteger GetNetMask() const
    {
        // Validate the database handle
        SQMOD_VALIDATE(*this);
        // Return the requested information
        return static_cast< SQInteger >(m_Result.netmask);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the entire entry data list.
    */
    Object GetEntryDataList();

    /* --------------------------------------------------------------------------------------------
     * Lookup data in the associated result using the specified path.
    */
    static SQInteger GetValue(HSQUIRRELVM vm);
};

/* ------------------------------------------------------------------------------------------------
 * Class that can hold and be used inspect database meta-data.
*/
class Metadata
{
public:

    // --------------------------------------------------------------------------------------------
    typedef MMDB_metadata_s         Type; // The managed type.

    // --------------------------------------------------------------------------------------------
    typedef Type*                   Pointer; // Pointer to the managed type.
    typedef const Type*             ConstPtr; // Constant pointer to the managed type.

    // --------------------------------------------------------------------------------------------
    typedef Type&                   Reference; // Reference to the managed type.
    typedef const Type&             ConstRef; // Constant reference to the managed type.

protected:

    /* --------------------------------------------------------------------------------------------
     * Validate the managed database handle and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    void Validate(CCStr file, Int32 line) const;
#else
    void Validate() const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Validate the managed database handle and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    Pointer GetValid(CCStr file, Int32 line) const;
#else
    Pointer GetValid() const;
#endif // _DEBUG

private:

    // ---------------------------------------------------------------------------------------------
    DbRef   m_Handle; // The database associated with this meta-data.
    Pointer m_Metadata; // The inspected meta-data structure.

public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor. (null)
    */
    Metadata()
        : m_Handle(), m_Metadata(nullptr)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Construct and with a specific meta-data.
    */
    Metadata(const DbRef & db, Pointer metadata)
        : m_Handle(db), m_Metadata(metadata)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    Metadata(const Metadata &) = default;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    Metadata(Metadata &&) = default;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    Metadata & operator = (const Metadata &) = default;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    Metadata & operator = (Metadata &&) = default;

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to convert an instance of this type to a string.
    */
    CSStr ToString() const
    {
        return m_Metadata ? m_Metadata->database_type : _SC("");
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to retrieve the name from instances of this type.
    */
    static SQInteger Typename(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * See whether this instance references a valid database and meta-data pointer.
    */
    bool IsValid() const
    {
        return m_Handle; // If there's a database handle then there's a meta-data too
    }

    /* --------------------------------------------------------------------------------------------
     * Release the manages handles/pointers and become a null instance.
    */
    void Release()
    {
        m_Handle.Reset();
        m_Metadata = nullptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the database associated with the managed handle/pointer.
    */
    Database GetDatabase() const;

    /* --------------------------------------------------------------------------------------------
     * Return the number of active references to the managed database instance.
    */
    Uint32 GetRefCount() const
    {
        return m_Handle.Count();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the node count.
    */
    SQInteger GetNodeCount() const
    {
        return ConvTo< SQInteger >::From(SQMOD_GET_VALID(*this)->node_count);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the record size.
    */
    SQInteger GetRecordSize() const
    {
        return ConvTo< SQInteger >::From(SQMOD_GET_VALID(*this)->record_size);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the IP version.
    */
    SQInteger GetIpVersion() const
    {
        return ConvTo< SQInteger >::From(SQMOD_GET_VALID(*this)->ip_version);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the database type.
    */
    CSStr GetDatabaseType() const
    {
        return SQMOD_GET_VALID(*this)->database_type;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the number of language names.
    */
    SQInteger GetLanguageCount() const
    {
        return ConvTo< SQInteger >::From(SQMOD_GET_VALID(*this)->languages.count);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the name of a certain language.
    */
    CSStr GetLanguageName(Uint32 idx) const
    {
        // Validate the specified index
        if (idx > SQMOD_GET_VALID(*this)->languages.count)
        {
            STHROWF("The specified language index is out of range: %u > %u", idx, m_Metadata->languages.count);
        }
        // Return the requested name
        return m_Metadata->languages.names[idx];
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the major version of the binary format.
    */
    SQInteger GetBinaryFormatMajorVersion() const
    {
        return ConvTo< SQInteger >::From(SQMOD_GET_VALID(*this)->binary_format_major_version);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the minor version of the binary format.
    */
    SQInteger GetBinaryFormatMinorVersion() const
    {
        return ConvTo< SQInteger >::From(SQMOD_GET_VALID(*this)->binary_format_minor_version);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the build epoch.
    */
    Object GetBuildEpoch() const
    {
        return Object(SqTypeIdentity< ULongInt >{}, SqVM(), ConvTo< Uint64 >::From(SQMOD_GET_VALID(*this)->build_epoch));
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the number of available description handles.
    */
    SQInteger GetDescriptionCount() const
    {
        return ConvTo< SQInteger >::From(SQMOD_GET_VALID(*this)->description.count);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the handle of a certain description.
    */
    Description GetDescriptionHandle(Uint32 idx) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the description of a certain description handle.
    */
    CSStr GetDescriptionValue(Uint32 idx) const
    {
        // Validate the specified index
        if (idx > SQMOD_GET_VALID(*this)->description.count)
        {
            STHROWF("The specified description index is out of range: %u > %u", idx, m_Metadata->description.count);
        }
        // Return the requested description value
        return m_Metadata->description.descriptions[idx]->description;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the language of a certain description handle.
    */
    CSStr GetDescriptionLanguage(Uint32 idx) const
    {
        // Validate the specified index
        if (idx > SQMOD_GET_VALID(*this)->description.count)
        {
            STHROWF("The specified description index is out of range: %u > %u", idx, m_Metadata->description.count);
        }
        // Return the requested description language
        return m_Metadata->description.descriptions[idx]->language;
    }
};

/* ------------------------------------------------------------------------------------------------
 * Class that can hold and be used to work with search nodes.
*/
class SearchNode
{
public:

    // --------------------------------------------------------------------------------------------
    typedef MMDB_search_node_s      Type; // The managed type.

    // --------------------------------------------------------------------------------------------
    typedef Type*                   Pointer; // Pointer to the managed type.
    typedef const Type*             ConstPtr; // Constant pointer to the managed type.

    // --------------------------------------------------------------------------------------------
    typedef Type&                   Reference; // Reference to the managed type.
    typedef const Type&             ConstRef; // Constant reference to the managed type.

protected:

    /* --------------------------------------------------------------------------------------------
     * Validate the managed database handle and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    void Validate(CCStr file, Int32 line) const;
#else
    void Validate() const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Validate the managed database handle and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    Reference GetValid(CCStr file, Int32 line);
#else
    Reference GetValid();
#endif // _DEBUG

private:

    // ---------------------------------------------------------------------------------------------
    DbRef   m_Handle; // The database from which this search node comes from.
    Type    m_Node; // The managed search node structure.

public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor. (null)
    */
    SearchNode();

    /* --------------------------------------------------------------------------------------------
     * Construct and take ownership of a certain search node.
    */
    SearchNode(const DbRef & db, Reference node)
        : m_Handle(db), m_Node(node)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    SearchNode(const SearchNode &) = default;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    SearchNode(SearchNode &&) = default;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    SearchNode & operator = (const SearchNode &) = default;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    SearchNode & operator = (SearchNode &&) = default;

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to convert an instance of this type to a string.
    */
    CSStr ToString() const
    {
        return FmtStr("<%llu:%s,%llu:s>", m_Node.left_record, AsTypeStr(m_Node.left_record_type)
                                        , m_Node.right_record, AsTypeStr(m_Node.right_record_type));
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to retrieve the name from instances of this type.
    */
    static SQInteger Typename(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * See whether this instance references a valid database and node structure.
    */
    bool IsValid() const
    {
        return m_Handle;
    }

    /* --------------------------------------------------------------------------------------------
     * Release the manages handles/pointers and become a null instance.
    */
    void Release();

    /* --------------------------------------------------------------------------------------------
     * Retrieve the database associated with the managed handle/pointer.
    */
    Database GetDatabase() const;

    /* --------------------------------------------------------------------------------------------
     * Return the number of active references to the managed database instance.
    */
    Uint32 GetRefCount() const
    {
        return m_Handle.Count();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the left record value.
    */
    Object GetLeftRecord()
    {
        return Object(SqTypeIdentity< ULongInt >{}, SqVM(), ConvTo< Uint64 >::From(SQMOD_GET_VALID(*this).left_record));
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the right record value.
    */
    Object GetRightRecord()
    {
      return Object(SqTypeIdentity< ULongInt >{}, SqVM(), ConvTo< Uint64 >::From(SQMOD_GET_VALID(*this).right_record));
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the left record value type.
    */
    SQInteger GetLeftRecordType()
    {
        return static_cast< SQInteger >(SQMOD_GET_VALID(*this).left_record_type);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the right record value type.
    */
    SQInteger GetRightRecordType()
    {
        return static_cast< SQInteger >(SQMOD_GET_VALID(*this).right_record_type);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the left record entry data list.
    */
    Object GetLeftRecordEntryDataList();

    /* --------------------------------------------------------------------------------------------
     * Retrieve the right record entry data list.
    */
    Object GetRightRecordEntryDataList();

    /* --------------------------------------------------------------------------------------------
     * Retrieve the left record entry data.
    */
    static SQInteger GetLeftRecordEntryData(HSQUIRRELVM vm)
    {
        return GetRecordEntryData(vm, false);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the left record entry data.
    */
    static SQInteger GetRightRecordEntryData(HSQUIRRELVM vm)
    {
        return GetRecordEntryData(vm, true);
    }

protected:

    /* --------------------------------------------------------------------------------------------
     * Shared function to retrieve the left/right record entry data.
    */
    static SQInteger GetRecordEntryData(HSQUIRRELVM vm, bool right);
};

} // Namespace:: SqMod
