#ifndef _SQMMDB_DESCRIPTION_HPP_
#define _SQMMDB_DESCRIPTION_HPP_

// ------------------------------------------------------------------------------------------------
#include "Handle/Database.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Class that can hold and be used inspect meta-data descriptions.
*/
class Description
{
    // --------------------------------------------------------------------------------------------
    friend class Metadata; // Only a valid meta-data instance can construct this type.

protected:

    // --------------------------------------------------------------------------------------------
    typedef MMDB_description_s      Type; // The managed type.

    // --------------------------------------------------------------------------------------------
    typedef Type*                   Pointer; // Pointer to the managed type.
    typedef const Type*             ConstPtr; // Constant pointer to the managed type.

    // --------------------------------------------------------------------------------------------
    typedef Type&                   Reference; // Reference to the managed type.
    typedef const Type&             ConstRef; // Constant reference to the managed type.

    /* --------------------------------------------------------------------------------------------
     * Validate the managed database handle and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    void Validate(CCStr file, Int32 line) const;
#else
    void Validate() const;
#endif // _DEBUG

private:

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

    /* --------------------------------------------------------------------------------------------
     * Construct and with a specific meta-data description.
    */
    Description(const DbRef & db, Pointer description)
        : m_Handle(db), m_Description(description)
    {
        /* ... */
    }

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

} // Namespace:: SqMod

#endif // _SQMMDB_DESCRIPTION_HPP_
