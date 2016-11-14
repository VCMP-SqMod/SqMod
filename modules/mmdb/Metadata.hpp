#ifndef _SQMMDB_METADATA_HPP_
#define _SQMMDB_METADATA_HPP_

// ------------------------------------------------------------------------------------------------
#include "Handle/Database.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

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
        // Obtain the initial stack size
        const StackGuard sg;
        // Push a long integer instance with the requested value on the stack
        SqMod_PushULongObject(DefaultVM::Get(), ConvTo< Uint64 >::From(SQMOD_GET_VALID(*this)->build_epoch));
        // Obtain the object from the stack and return it
        return Var< Object >(DefaultVM::Get(), -1).value;
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

} // Namespace:: SqMod

#endif // _SQMMDB_METADATA_HPP_
