// ------------------------------------------------------------------------------------------------
#include "Parameter.hpp"

// ------------------------------------------------------------------------------------------------
#include <ctime>
#include <cerrno>
#include <cstdlib>
#include <cstring>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
// Error message when failed to bind value to parameter index.
#define SQMOD_BINDFAILED "Unable to bind (%s) parameter (%d) because [%s]"

// ------------------------------------------------------------------------------------------------
SQMODE_DECL_TYPENAME(Typename, _SC("SqLiteParameter"))

// ------------------------------------------------------------------------------------------------
static inline bool IsDigitsOnly(CSStr str)
{
    while (std::isdigit(*str) || std::isspace(*str))
    {
        ++str;
    }
    // Return whether we reached the end while searching
    return *str == '\0';
}

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
void Parameter::Validate(CCStr file, Int32 line) const
{
    // Are we pointing to a valid index?
    if (m_Index < 0)
    {
        SqThrowF("Invalid column index: %d < 0 =>[%s:%d]", m_Index, file, line);
    }
    // Do we have a valid statement handle?
    else if (!m_Handle)
    {
        SqThrowF("Invalid SQLite statement reference =>[%s:%d]", file, line);
    }
}
#else
void Parameter::Validate() const
{
    // Are we pointing to a valid index?
    if (m_Index < 0)
    {
        SqThrowF("Invalid column index: %d < 0", m_Index);
    }
    // Do we have a valid statement handle?
    else if (!m_Handle)
    {
        SqThrowF("Invalid SQLite statement reference");
    }
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
void Parameter::ValidateCreated(CCStr file, Int32 line) const
{
    // Are we pointing to a valid index?
    if (m_Index < 0)
    {
        SqThrowF("Invalid column index: %d < 0 =>[%s:%d]", m_Index, file, line);
    }
    else if (!m_Handle)
    {
        SqThrowF("Invalid SQLite statement reference =>[%s:%d]", file, line);
    }
    else if (m_Handle->mPtr == nullptr)
    {
        SqThrowF("Invalid SQLite statement =>[%s:%d]", file, line);
    }
}
#else
void Parameter::ValidateCreated() const
{
    // Are we pointing to a valid index?
    if (m_Index < 0)
    {
        SqThrowF("Invalid column index: %d < 0", m_Index);
    }
    else if (!m_Handle)
    {
        SqThrowF("Invalid SQLite statement reference");
    }
    else if (m_Handle->mPtr == nullptr)
    {
        SqThrowF("Invalid SQLite statement");
    }
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
const StmtRef & Parameter::GetValid(CCStr file, Int32 line) const
{
    Validate(file, line);
    return m_Handle;
}
#else
const StmtRef & Parameter::GetValid() const
{
    Validate();
    return m_Handle;
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
const StmtRef & Parameter::GetCreated(CCStr file, Int32 line) const
{
    ValidateCreated(file, line);
    return m_Handle;
}
#else
const StmtRef & Parameter::GetCreated() const
{
    ValidateCreated();
    return m_Handle;
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
void Parameter::ValidateParam(Int32 idx, CCStr file, Int32 line) const
{
    ValidateCreated(file, line);
    // Is the specified index in range?
    if (!m_Handle->CheckParameter(idx))
    {
        SqThrowF("Parameter index is out of range (%d:%d) =>[%s:%d]", idx, m_Handle->mParameters,
                    file, line);
    }
}
#else
void Parameter::ValidateParam(Int32 idx) const
{
    ValidateCreated();
    // Is the specified index in range?
    if (!m_Handle->CheckParameter(idx))
    {
        SqThrowF("Parameter index is out of range (%d:%d)", idx, m_Handle->mParameters);
    }
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
void Parameter::SetIndex(const Object & param)
{
    // Where the index will be extracted
    Int32 idx = 0;
    // Grab the object virtual machine
    HSQUIRRELVM vm = param.GetVM();
    // Remember the current stack size
    const StackGuard sg(vm);
    // Push the specified object onto the stack
    Var< const Object & >::push(vm, param);
    // Identify the type of parameter was given
    switch  (param.GetType())
    {
        // Is this a string value?
        case OT_STRING:
        {
            // Obtain the object from the stack as a string
            const StackStrF val(vm, -1, false);
            // Validate the result
            if (SQ_FAILED(val.mRes))
            {
                STHROWF("%s", LastErrorString(vm).c_str());
            }
            // Is the obtained string empty?
            else if (val.mLen <= 0)
            {
                STHROWF("Cannot use an empty parameter name");
            }
            // Attempt to find a parameter with the specified name
            idx = sqlite3_bind_parameter_index(SQMOD_GET_CREATED(*this)->mPtr, val.mPtr);
        } break;
        // Is this an integer value? (or at least can be easily converted to one)
        case OT_INTEGER:
        case OT_FLOAT:
        case OT_BOOL:
        {
            idx = ConvTo< Int32 >::From(SqMod_PopStackInteger(vm, -1));
        } break;
        // Is this an instance that we can extract either a string or integer from it?
        case OT_INSTANCE:
        {
            // Obtain the object from the stack as a string
            const StackStrF val(vm, -1, false);
            // Validate the result
            if (SQ_FAILED(val.mRes))
            {
                STHROWF("%s", LastErrorString(vm).c_str());
            }
            // Is the obtained string empty?
            else if (val.mLen <= 0)
            {
                STHROWF("Cannot use an empty parameter name");
            }
            // Check if this value is made only of digits
            else if (IsDigitsOnly(val.mPtr))
            {
                idx = ConvNum< Int32 >::FromStr(val.mPtr);
            }
            // Attempt to find a parameter with the specified name
            else
            {
                idx = sqlite3_bind_parameter_index(SQMOD_GET_CREATED(*this)->mPtr, val.mPtr);
            }
        } break;
        // We don't recognize this kind of value!
        default: STHROWF("Unknown parameter index of type (%s)", SqTypeName(param.GetType()));
    }
    // Assign the index with a failsafe to invalid on error
    AutoAssign< Int32 > aa(m_Index, 0, idx);
    // Validate the obtained parameter index
    SQMOD_VALIDATE_PARAM(*this, idx);
    // Don't fall back to the invalid index anymore
    aa.Set(idx);
}

// ------------------------------------------------------------------------------------------------
Object Parameter::GetStatement() const
{
    return GetStatementObj(m_Handle);
}

// ------------------------------------------------------------------------------------------------
Object Parameter::GetConnection() const
{
    return GetConnectionObj(SQMOD_GET_VALID(*this)->mConn);
}

// ------------------------------------------------------------------------------------------------
void Parameter::SetValue(const Object & value)
{
    switch (value.GetType())
    {
        case OT_NULL:
        {
            SetNull();
        } break;
        case OT_INTEGER:
        {
            SetInteger(value.Cast< SQInteger >());
        } break;
        case OT_FLOAT:
        {
            SetFloat(value.Cast< SQFloat >());
        } break;
        case OT_BOOL:
        {
            SetBool(value.Cast< bool >());
        } break;
        case OT_STRING:
        {
            SQMOD_VALIDATE_CREATED(*this);
            // Remember the current stack size
            const StackGuard sg;
            // Push the object onto the stack
            Var< Object >::push(DefaultVM::Get(), value);
            // Pop the object from the stack as a string
            const Var< CSStr > str(DefaultVM::Get(), -1);
            // Attempt to bind the specified value
            SetStringRaw(str.value, ConvTo< SQInteger >::From(str.size));
        } break;
        default: STHROWF("No known conversion for the specified value type");
    }
}

// ------------------------------------------------------------------------------------------------
void Parameter::SetBool(bool value)
{
    SQMOD_VALIDATE_CREATED(*this);
    // Attempt to bind the specified value
    m_Handle->mStatus = sqlite3_bind_int(m_Handle->mPtr, m_Index, value);
    // Validate the result
    if (m_Handle->mStatus != SQLITE_OK)
    {
        STHROWF(SQMOD_BINDFAILED, "bool", m_Index, m_Handle->ErrMsg());
    }
}

// ------------------------------------------------------------------------------------------------
void Parameter::SetChar(SQInteger value)
{
    SQMOD_VALIDATE_CREATED(*this);
    // Attempt to bind the specified value
    m_Handle->mStatus = sqlite3_bind_int(m_Handle->mPtr, m_Index, ConvTo< SQChar >::From(value));
    // Validate the result
    if (m_Handle->mStatus != SQLITE_OK)
    {
        STHROWF(SQMOD_BINDFAILED, "char", m_Index, m_Handle->ErrMsg());
    }
}

// ------------------------------------------------------------------------------------------------
void Parameter::SetInteger(SQInteger value)
{
    SQMOD_VALIDATE_CREATED(*this);
    // Attempt to bind the specified value
    m_Handle->mStatus = sqlite3_bind_integer(m_Handle->mPtr, m_Index, value);
    // Validate the result
    if (m_Handle->mStatus != SQLITE_OK)
    {
        STHROWF(SQMOD_BINDFAILED, "integer", m_Index, m_Handle->ErrMsg());
    }
}

// ------------------------------------------------------------------------------------------------
void Parameter::SetInt8(SQInteger value)
{
    SQMOD_VALIDATE_CREATED(*this);
    // Attempt to bind the specified value
    m_Handle->mStatus = sqlite3_bind_int(m_Handle->mPtr, m_Index, ConvTo< Int8 >::From(value));
    // Validate the result
    if (m_Handle->mStatus != SQLITE_OK)
    {
        STHROWF(SQMOD_BINDFAILED, "int8", m_Index, m_Handle->ErrMsg());
    }
}

// ------------------------------------------------------------------------------------------------
void Parameter::SetUint8(SQInteger value)
{
    SQMOD_VALIDATE_CREATED(*this);
    // Attempt to bind the specified value
    m_Handle->mStatus = sqlite3_bind_int(m_Handle->mPtr, m_Index, ConvTo< Uint8 >::From(value));
    // Validate the result
    if (m_Handle->mStatus != SQLITE_OK)
    {
        STHROWF(SQMOD_BINDFAILED, "uint8", m_Index, m_Handle->ErrMsg());
    }
}

// ------------------------------------------------------------------------------------------------
void Parameter::SetInt16(SQInteger value)
{
    SQMOD_VALIDATE_CREATED(*this);
    // Attempt to bind the specified value
    m_Handle->mStatus = sqlite3_bind_int(m_Handle->mPtr, m_Index, ConvTo< Int16 >::From(value));
    // Validate the result
    if (m_Handle->mStatus != SQLITE_OK)
    {
        STHROWF(SQMOD_BINDFAILED, "int16", m_Index, m_Handle->ErrMsg());
    }
}

// ------------------------------------------------------------------------------------------------
void Parameter::SetUint16(SQInteger value)
{
    SQMOD_VALIDATE_CREATED(*this);
    // Attempt to bind the specified value
    m_Handle->mStatus = sqlite3_bind_int(m_Handle->mPtr, m_Index, ConvTo< Uint16 >::From(value));
    // Validate the result
    if (m_Handle->mStatus != SQLITE_OK)
    {
        STHROWF(SQMOD_BINDFAILED, "uint16", m_Index, m_Handle->ErrMsg());
    }
}

// ------------------------------------------------------------------------------------------------
void Parameter::SetInt32(SQInteger value)
{
    SQMOD_VALIDATE_CREATED(*this);
    // Attempt to bind the specified value
    m_Handle->mStatus = sqlite3_bind_int(m_Handle->mPtr, m_Index, ConvTo< Int32 >::From(value));
    // Validate the result
    if (m_Handle->mStatus != SQLITE_OK)
    {
        STHROWF(SQMOD_BINDFAILED, "int32", m_Index, m_Handle->ErrMsg());
    }
}

// ------------------------------------------------------------------------------------------------
void Parameter::SetUint32(SQInteger value)
{
    SQMOD_VALIDATE_CREATED(*this);
    // Attempt to bind the specified value
    m_Handle->mStatus = sqlite3_bind_int(m_Handle->mPtr, m_Index, ConvTo< Uint32 >::From(value));
    // Validate the result
    if (m_Handle->mStatus != SQLITE_OK)
    {
        STHROWF(SQMOD_BINDFAILED, "uint32", m_Index, m_Handle->ErrMsg());
    }
}

// ------------------------------------------------------------------------------------------------
void Parameter::SetInt64(const Object & value)
{
    SQMOD_VALIDATE_CREATED(*this);
    // Attempt to bind the specified value
    m_Handle->mStatus = sqlite3_bind_int64(m_Handle->mPtr, m_Index, FetchSLongObjVal(value));
    // Validate the result
    if (m_Handle->mStatus != SQLITE_OK)
    {
        STHROWF(SQMOD_BINDFAILED, "int64", m_Index, m_Handle->ErrMsg());
    }
}

// ------------------------------------------------------------------------------------------------
void Parameter::SetUint64(const Object & value)
{
    SQMOD_VALIDATE_CREATED(*this);
    // Attempt to bind the specified value
    m_Handle->mStatus = sqlite3_bind_int64(m_Handle->mPtr, m_Index,
                                            static_cast< Int64 >(FetchULongObjVal(value)));
    // Validate the result
    if (m_Handle->mStatus != SQLITE_OK)
    {
        STHROWF(SQMOD_BINDFAILED, "uint64", m_Index, m_Handle->ErrMsg());
    }
}

// ------------------------------------------------------------------------------------------------
void Parameter::SetFloat(SQFloat value)
{
    SQMOD_VALIDATE_CREATED(*this);
    // Attempt to bind the specified value
    m_Handle->mStatus = sqlite3_bind_double(m_Handle->mPtr, m_Index, value);
    // Validate the result
    if (m_Handle->mStatus != SQLITE_OK)
    {
        STHROWF(SQMOD_BINDFAILED, "float", m_Index, m_Handle->ErrMsg());
    }
}

// ------------------------------------------------------------------------------------------------
void Parameter::SetFloat32(SQFloat value)
{
    SQMOD_VALIDATE_CREATED(*this);
    // Attempt to bind the specified value
    m_Handle->mStatus = sqlite3_bind_double(m_Handle->mPtr, m_Index, ConvTo< Float32 >::From(value));
    // Validate the result
    if (m_Handle->mStatus != SQLITE_OK)
    {
        STHROWF(SQMOD_BINDFAILED, "float32", m_Index, m_Handle->ErrMsg());
    }
}

// ------------------------------------------------------------------------------------------------
void Parameter::SetFloat64(SQFloat value)
{
    SQMOD_VALIDATE_CREATED(*this);
    // Attempt to bind the specified value
    m_Handle->mStatus = sqlite3_bind_double(m_Handle->mPtr, m_Index, value);
    // Validate the result
    if (m_Handle->mStatus != SQLITE_OK)
    {
        STHROWF(SQMOD_BINDFAILED, "float64", m_Index, m_Handle->ErrMsg());
    }
}

// ------------------------------------------------------------------------------------------------
void Parameter::SetString(const StackStrF & value)
{
    SQMOD_VALIDATE_CREATED(*this);
    // Attempt to bind the specified value
    m_Handle->mStatus = sqlite3_bind_text(m_Handle->mPtr, m_Index, value.mPtr, value.mLen, SQLITE_TRANSIENT);
    // Validate the result
    if (m_Handle->mStatus != SQLITE_OK)
    {
        STHROWF(SQMOD_BINDFAILED, "string", m_Index, m_Handle->ErrMsg());
    }
}

// ------------------------------------------------------------------------------------------------
void Parameter::SetStringRaw(CSStr value, SQInteger length)
{
    SQMOD_VALIDATE_CREATED(*this);
    // Attempt to bind the specified value
    m_Handle->mStatus = sqlite3_bind_text(m_Handle->mPtr, m_Index, value, length, SQLITE_TRANSIENT);
    // Validate the result
    if (m_Handle->mStatus != SQLITE_OK)
    {
        STHROWF(SQMOD_BINDFAILED, "string", m_Index, m_Handle->ErrMsg());
    }
}

// ------------------------------------------------------------------------------------------------
void Parameter::SetZeroBlob(SQInteger size)
{
    SQMOD_VALIDATE_CREATED(*this);
    // Attempt to bind the specified value
    m_Handle->mStatus = sqlite3_bind_zeroblob(m_Handle->mPtr, m_Index, ConvTo< Int32 >::From(size));
    // Validate the result
    if (m_Handle->mStatus != SQLITE_OK)
    {
        STHROWF(SQMOD_BINDFAILED, "blob", m_Index, m_Handle->ErrMsg());
    }
}

// ------------------------------------------------------------------------------------------------
void Parameter::SetBlob(const Object & value)
{
    SQMOD_VALIDATE_CREATED(*this);
    // The blob data pointer and size
    SQUserPointer ptr = 0;
    SQInteger len = 0;
    // Grab the associated object virtual machine
    HSQUIRRELVM vm = value.GetVM();
    // Extract the blob data from the specified object
    {
        // Remember the current stack size
        const StackGuard sg(vm);
        // Push the specified object onto the stack
        Var< const Object & >::push(vm, value);
        // Grab the blob data pointer
        if (SQ_FAILED(sqstd_getblob(vm, -1, &ptr)))
        {
            STHROWF("Unable to obtain the blob data");
        }
        // Grab the blob data size
        len = sqstd_getblobsize(vm, -1);
    }
    // Attempt to bind the specified value
    m_Handle->mStatus = sqlite3_bind_blob(m_Handle->mPtr, m_Index, ptr, len, SQLITE_TRANSIENT);
    // Validate the result
    if (m_Handle->mStatus != SQLITE_OK)
    {
        STHROWF(SQMOD_BINDFAILED, "blob", m_Index, m_Handle->ErrMsg());
    }
}

// ------------------------------------------------------------------------------------------------
void Parameter::SetData(const Object & value)
{
    SQMOD_VALIDATE_CREATED(*this);
    // The buffer data pointer and size
    CCStr ptr = 0;
    SQInteger len = 0;
    // Grab the associated object virtual machine
    HSQUIRRELVM vm = value.GetVM();
    // Extract the buffer data from the specified object
    {
        // Remember the current stack size
        const StackGuard sg(vm);
        // Push the specified object onto the stack
        Var< const Object & >::push(vm, value);
        // Grab the buffer data pointer and size
        if (SQ_FAILED(SqMod_GetBufferInfo(vm, -1, &ptr, &len, nullptr)))
        {
            STHROWF("Unable to obtain the buffer data");
        }
    }
    // Attempt to bind the specified value
    m_Handle->mStatus = sqlite3_bind_blob(m_Handle->mPtr, m_Index, ptr, len, SQLITE_TRANSIENT);
    // Validate the result
    if (m_Handle->mStatus != SQLITE_OK)
    {
        STHROWF(SQMOD_BINDFAILED, "buffer", m_Index, m_Handle->ErrMsg());
    }
}

// ------------------------------------------------------------------------------------------------
void Parameter::SetDate(const Object & value)
{
    SQMOD_VALIDATE_CREATED(*this);
    // Attempt to generate the specified date string
    CSStr ptr = FmtStr(_SC("%s 00:00:00"), FetchDateObjStr(value));
    // Attempt to bind the specified value
    m_Handle->mStatus = sqlite3_bind_text(m_Handle->mPtr, m_Index, ptr, -1, SQLITE_TRANSIENT);
    // Validate the result
    if (m_Handle->mStatus != SQLITE_OK)
    {
        STHROWF(SQMOD_BINDFAILED, "date", m_Index, m_Handle->ErrMsg());
    }
}

// ------------------------------------------------------------------------------------------------
void Parameter::SetDateEx(SQInteger year, SQInteger month, SQInteger day)
{
    SQMOD_VALIDATE_CREATED(*this);
    // Convert the specified values within the proper ranges
    const uint16_t y = ConvTo< uint16_t >::From(year);
    const uint8_t m = ConvTo< uint8_t >::From(month), d = ConvTo< uint8_t >::From(day);
    // Validate the specified date
    if (!SqMod_ValidDate(y, m, d))
    {
        STHROWF("Invalid date (%u-%u-%u)", y, m, d);
    }
    // Attempt to generate the specified date string
    CSStr ptr = FmtStr(_SC("%u-%u-%u 00:00:00"), y, m, d);
    // Attempt to bind the specified value
    m_Handle->mStatus = sqlite3_bind_text(m_Handle->mPtr, m_Index, ptr, -1, SQLITE_TRANSIENT);
    // Validate the result
    if (m_Handle->mStatus != SQLITE_OK)
    {
        STHROWF(SQMOD_BINDFAILED, "date", m_Index, m_Handle->ErrMsg());
    }
}

// ------------------------------------------------------------------------------------------------
void Parameter::SetTime(const Object & value)
{
    SQMOD_VALIDATE_CREATED(*this);
    // Attempt to bind the specified value
    m_Handle->mStatus = sqlite3_bind_int(m_Handle->mPtr, m_Index, FetchTimeObjSeconds(value));
    // Validate the result
    if (m_Handle->mStatus != SQLITE_OK)
    {
        STHROWF(SQMOD_BINDFAILED, "time", m_Index, m_Handle->ErrMsg());
    }
}

// ------------------------------------------------------------------------------------------------
void Parameter::SetTimeEx(SQInteger hour, SQInteger minute, SQInteger second)
{
    SQMOD_VALIDATE_CREATED(*this);
    // Convert the specified values within the proper ranges
    const uint8_t h = ConvTo< uint8_t >::From(hour)
                , m = ConvTo< uint8_t >::From(minute)
                , s = ConvTo< uint8_t >::From(second);
    // Is the specified hour within range?
    if (h >= 24)
    {
        STHROWF("Hour value is out of range: %u >= 24", h);
    }
    // Is the specified minute within range?
    else if (m >= 60)
    {
        STHROWF("Minute value is out of range: %u >= 60", m);
    }
    // Is the specified second within range?
    else if (s >= 60)
    {
        STHROWF("Second value is out of range: %u >= 60", s);
    }
    // Calculate the number of seconds in the specified time and bind the resulted value
    m_Handle->mStatus = sqlite3_bind_int(m_Handle->mPtr, m_Index, (h * (60 * 60)) + (m * 60) + s);
    // Validate the result
    if (m_Handle->mStatus != SQLITE_OK)
    {
        STHROWF(SQMOD_BINDFAILED, "time", m_Index, m_Handle->ErrMsg());
    }
}

// ------------------------------------------------------------------------------------------------
void Parameter::SetDatetime(const Object & value)
{
    SQMOD_VALIDATE_CREATED(*this);
    // Attempt to generate the specified date string and bind the resulted value
    m_Handle->mStatus = sqlite3_bind_text(m_Handle->mPtr, m_Index, FetchDatetimeObjStr(value),
                                            -1, SQLITE_TRANSIENT);
    // Validate the result
    if (m_Handle->mStatus != SQLITE_OK)
    {
        STHROWF(SQMOD_BINDFAILED, "date-time", m_Index, m_Handle->ErrMsg());
    }
}

// ------------------------------------------------------------------------------------------------
void Parameter::SetDatetimeEx(SQInteger year, SQInteger month, SQInteger day, SQInteger hour, SQInteger minute, SQInteger second)
{
    SQMOD_VALIDATE_CREATED(*this);
    // Convert the specified values within the proper ranges
    const uint16_t y = ConvTo< uint16_t >::From(year);
    const uint8_t mo = ConvTo< uint8_t >::From(month)
                , d = ConvTo< uint8_t >::From(day)
                , h = ConvTo< uint8_t >::From(hour)
                , mi = ConvTo< uint8_t >::From(minute)
                , s = ConvTo< uint8_t >::From(second);
    // Validate the specified date
    if (!SqMod_ValidDate(y, mo, d))
    {
        STHROWF("Invalid date (%u-%u-%u)", y, mo, d);
    }
    // Is the specified hour within range?
    else if (h >= 24)
    {
        STHROWF("Hour value is out of range: %u >= 24", h);
    }
    // Is the specified minute within range?
    else if (mi >= 60)
    {
        STHROWF("Minute value is out of range: %u >= 60", mi);
    }
    // Is the specified second within range?
    else if (s >= 60)
    {
        STHROWF("Second value is out of range: %u >= 60", s);
    }
    // Attempt to generate the specified date string
    CSStr ptr = FmtStr(_SC("%04u-%02u-%02u %02u:%02u:%02u"), y, mo, d, h, mi, s);
    // Attempt to bind the specified value
    m_Handle->mStatus = sqlite3_bind_text(m_Handle->mPtr, m_Index, ptr, -1, SQLITE_TRANSIENT);
    // Validate the result
    if (m_Handle->mStatus != SQLITE_OK)
    {
        STHROWF(SQMOD_BINDFAILED, "date-time", m_Index, m_Handle->ErrMsg());
    }
}

// ------------------------------------------------------------------------------------------------
void Parameter::SetNow()
{
    SQMOD_VALIDATE_CREATED(*this);
    // Attempt to bind the specified value
    m_Handle->mStatus = sqlite3_bind_int(m_Handle->mPtr, m_Index,
                                            static_cast< Int32 >(std::time(nullptr)));
    // Validate the result
    if (m_Handle->mStatus != SQLITE_OK)
    {
        STHROWF(SQMOD_BINDFAILED, "time-stamp", m_Index, m_Handle->ErrMsg());
    }
}

// ------------------------------------------------------------------------------------------------
void Parameter::SetNull()
{
    SQMOD_VALIDATE_CREATED(*this);
    // Attempt to bind the specified value
    m_Handle->mStatus = sqlite3_bind_null(m_Handle->mPtr, m_Index);
    // Validate the result
    if (m_Handle->mStatus != SQLITE_OK)
    {
        STHROWF(SQMOD_BINDFAILED, "null", m_Index, m_Handle->ErrMsg());
    }
}

// ================================================================================================
void Register_Parameter(Table & sqlns)
{
    sqlns.Bind(_SC("Parameter"),
        Class< Parameter >(sqlns.GetVM(), Typename::Str)
        // Constructors
        .Ctor()
        .Ctor< const Parameter & >()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &Typename::Fn)
        .Func(_SC("_tostring"), &Parameter::ToString)
        // Properties
        .Prop(_SC("IsValid"), &Parameter::IsValid)
        .Prop(_SC("References"), &Parameter::GetRefCount)
        .Prop(_SC("Index"), &Parameter::GetIndex)
        .Prop(_SC("Statement"), &Parameter::GetStatement)
        .Prop(_SC("Connection"), &Parameter::GetConnection)
        .Prop(_SC("References"), &Parameter::GetRefCount)
        .Prop(_SC("Name"), &Parameter::GetName)
        // Member Methods
        .Func(_SC("Release"), &Parameter::Release)
        .Func(_SC("SetValue"), &Parameter::SetValue)
        .Func(_SC("SetBool"), &Parameter::SetBool)
        .Func(_SC("SetChar"), &Parameter::SetChar)
        .Func(_SC("SetInteger"), &Parameter::SetInteger)
        .Func(_SC("SetInt8"), &Parameter::SetInt8)
        .Func(_SC("SetUint8"), &Parameter::SetUint8)
        .Func(_SC("SetInt16"), &Parameter::SetInt16)
        .Func(_SC("SetUint16"), &Parameter::SetUint16)
        .Func(_SC("SetInt32"), &Parameter::SetInt32)
        .Func(_SC("SetUint32"), &Parameter::SetUint32)
        .Func(_SC("SetInt64"), &Parameter::SetInt64)
        .Func(_SC("SetUint64"), &Parameter::SetUint64)
        .Func(_SC("SetFloat"), &Parameter::SetFloat)
        .Func(_SC("SetFloat32"), &Parameter::SetFloat32)
        .Func(_SC("SetFloat64"), &Parameter::SetFloat64)
        .FmtFunc(_SC("SetString"), &Parameter::SetString)
        .Func(_SC("SetZeroBlob"), &Parameter::SetZeroBlob)
        .Func(_SC("SetBlob"), &Parameter::SetBlob)
        .Func(_SC("SetData"), &Parameter::SetData)
        .Func(_SC("SetDate"), &Parameter::SetDate)
        .Func(_SC("SetDateEx"), &Parameter::SetDateEx)
        .Func(_SC("SetTime"), &Parameter::SetTime)
        .Func(_SC("SetTimeEx"), &Parameter::SetTimeEx)
        .Func(_SC("SetDatetime"), &Parameter::SetDatetime)
        .Func(_SC("SetDatetimeEx"), &Parameter::SetDatetimeEx)
        .Func(_SC("SetNow"), &Parameter::SetNow)
        .Func(_SC("SetNull"), &Parameter::SetNull)
    );
}

} // Namespace:: SqMod
