// ------------------------------------------------------------------------------------------------
#include "Metadata.hpp"
#include "Description.hpp"

// ------------------------------------------------------------------------------------------------
#include <cstdlib>
#include <cstring>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQInteger Metadata::Typename(HSQUIRRELVM vm)
{
    static const SQChar name[] = _SC("SqMMMetadata");
    sq_pushstring(vm, name, sizeof(name));
    return 1;
}

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
void Metadata::Validate(CCStr file, Int32 line) const
{
    if (!m_Handle)
    {
        SqThrowF("Invalid Maxmind database reference =>[%s:%d]", file, line);
    }
}
#else
void Metadata::Validate() const
{
    if (!m_Handle)
    {
        SqThrowF("Invalid Maxmind database reference");
    }
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
Metadata::Pointer Metadata::GetValid(CCStr file, Int32 line) const
{
    Validate(file, line);
    // Validate the referenced meta-data
    if (!m_Metadata)
    {
        SqThrowF("Invalid Maxmind meta-data reference =>[%s:%d]", file, line);
    }
    // Return the meta-data pointer
    return m_Metadata;
}
#else
Metadata::Pointer Metadata::GetValid() const
{
    Validate();
    // Validate the referenced meta-data
    if (!m_Metadata)
    {
        SqThrowF("Invalid Maxmind meta-data reference");
    }
    // Return the meta-data pointer
    return m_Metadata;
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
Description Metadata::GetDescriptionHandle(Uint32 idx) const
{
    // Validate the specified index
    if (idx > SQMOD_GET_VALID(*this)->description.count)
    {
        STHROWF("The specified description index is out of range: %u > %u", idx, m_Metadata->description.count);
    }
    // Return the requested description
    return Description(m_Handle, m_Metadata->description.descriptions[idx]);
}

// ================================================================================================
void Register_Metadata(Table & mmns)
{
    mmns.Bind(_SC("Metadata"),
        Class< Metadata >(mmns.GetVM(), _SC("SqMMMetadata"))
        // Constructors
        .Ctor()
        .Ctor< const Metadata & >()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &Metadata::Typename)
        .Func(_SC("_tostring"), &Metadata::ToString)
        // Properties
        .Prop(_SC("IsValid"), &Metadata::IsValid)
        .Prop(_SC("NodeCount"), &Metadata::GetNodeCount)
        .Prop(_SC("RecordSize"), &Metadata::GetRecordSize)
        .Prop(_SC("IpVersion"), &Metadata::GetIpVersion)
        .Prop(_SC("DatabaseType"), &Metadata::GetDatabaseType)
        .Prop(_SC("LanguageCount"), &Metadata::GetLanguageCount)
        .Prop(_SC("BinaryFormatMajorVersion"), &Metadata::GetBinaryFormatMajorVersion)
        .Prop(_SC("BinaryFormatMinorVersion"), &Metadata::GetBinaryFormatMinorVersion)
        .Prop(_SC("BuildEpoch"), &Metadata::GetBuildEpoch)
        .Prop(_SC("DescriptionCount"), &Metadata::GetDescriptionCount)
        // Member methods
        .Func(_SC("GetLanguageName"), &Metadata::GetLanguageName)
        .Func(_SC("GetDescriptionHandle"), &Metadata::GetDescriptionHandle)
        .Func(_SC("GetDescriptionValue"), &Metadata::GetDescriptionValue)
        .Func(_SC("GetDescriptionLanguage"), &Metadata::GetDescriptionLanguage)
    );
}

} // Namespace:: SqMod
