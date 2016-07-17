// ------------------------------------------------------------------------------------------------
#include "Core.hpp"
#include "Base/Buffer.hpp"

// ------------------------------------------------------------------------------------------------
#include "Library/Numeric/LongInt.hpp"
#include "Library/Chrono/Date.hpp"
#include "Library/Chrono/Time.hpp"
#include "Library/Chrono/Datetime.hpp"
#include "Library/Chrono/Timestamp.hpp"
#include "Library/Utils/Buffer.hpp"

// ------------------------------------------------------------------------------------------------
#include <cmath>
#include <cstdlib>
#include <cstring>

// ------------------------------------------------------------------------------------------------
#include <sqstdio.h>
#include <sqstdblob.h>
#include <sqstdstring.h>
#include <SqMod.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
static HSQUIRRELVM GetSquirrelVM()
{
    return Core::Get().GetVM();
}

// ------------------------------------------------------------------------------------------------
static SQRESULT SqModImpl_LoadScript(const SQChar * filepath, SQBool delay)
{
    // Attempt to add the specified script to the load queue
    if (Core::Get().LoadScript(filepath, delay))
    {
        return SQ_OK; // The script as added or already existed
    }
    // The path was invalid or was unable to pool the script
    return SQ_ERROR;
}

// ------------------------------------------------------------------------------------------------
static SQRESULT SqModImpl_GetSLongValue(HSQUIRRELVM vm, SQInteger idx, Int64 * num)
{
    // Validate the specified number pointer and value type
    if (!num)
    {
        return SQ_ERROR; // Nowhere to save!
    }
    // Is this an instance that we can treat as a SLongInt type?
    else if (sq_gettype(vm, idx) == OT_INSTANCE)
    {
        // Attempt to obtain the long instance and it's value from the stack
        try
        {
            *num =  static_cast< Int64 >(Var< const SLongInt & >(vm, idx).value.GetNum());
        }
        catch (...)
        {
            return SQ_ERROR; // Unable to obtain the value!
        }
    }
    // Is this a pure integer value?
    else if(sq_gettype(vm, idx) == OT_INTEGER)
    {
        SQInteger val = 0;
        // Attempt to get the value from the stack
        sq_getinteger(vm, idx, &val);
        // Save it into the specified memory location
        *num = static_cast< Int64 >(val);
    }
    // Is this a pure floating point value?
    else if(sq_gettype(vm, idx) == OT_FLOAT)
    {
        SQFloat val = 0.0;
        // Attempt to get the value from the stack
        sq_getfloat(vm, idx, &val);
        // Save it into the specified memory location
        *num = static_cast< Int64 >(std::llround(val));
    }
    // Is this a pure boolean value?
    else if(sq_gettype(vm, idx) == OT_BOOL)
    {
        SQBool val = SQFalse;
        // Attempt to get the value from the stack
        sq_getbool(vm, idx, &val);
        // Save it into the specified memory location
        *num = static_cast< Int64 >(val);
    }
    // Unrecognized value
    else
    {
        return SQ_ERROR;
    }
    // Value retrieved
    return SQ_OK;
}

// ------------------------------------------------------------------------------------------------
static SQRESULT SqModImpl_PushSLongObject(HSQUIRRELVM vm, Int64 num)
{
    // Attempt to push the requested instance
    try
    {
        Var< const SLongInt & >::push(vm, SLongInt(num));
    }
    catch (...)
    {
        // Specify that we failed
        return SQ_ERROR;
    }
    // Specify that we succeeded
    return SQ_OK;
}

// ------------------------------------------------------------------------------------------------
static SQRESULT SqModImpl_GetULongValue(HSQUIRRELVM vm, SQInteger idx, Uint64 * num)
{
    // Validate the specified number pointer and value type
    if (!num)
    {
        return SQ_ERROR; // Nowhere to save
    }
    // Is this an instance that we can treat as a ULongInt type?
    else if (sq_gettype(vm, idx) == OT_INSTANCE)
    {
        // Attempt to obtain the long instance and it's value from the stack
        try
        {
            *num =  static_cast< Uint64 >(Var< const ULongInt & >(vm, idx).value.GetNum());
        }
        catch (...)
        {
            return SQ_ERROR; // Unable to obtain the value!
        }
    }
    // Is this a pure integer value?
    else if(sq_gettype(vm, idx) == OT_INTEGER)
    {
        SQInteger val = 0;
        // Attempt to get the value from the stack
        sq_getinteger(vm, idx, &val);
        // Save it into the specified memory location
        *num = val ? static_cast< Uint64 >(val) : 0L;
    }
    // Is this a pure floating point value?
    else if(sq_gettype(vm, idx) == OT_FLOAT)
    {
        SQFloat val = 0.0;
        // Attempt to get the value from the stack
        sq_getfloat(vm, idx, &val);
        // Save it into the specified memory location
        *num = EpsLt(val, SQFloat(0.0)) ? 0L : static_cast< Uint64 >(std::llround(val));
    }
    // Is this a pure boolean value?
    else if(sq_gettype(vm, idx) == OT_BOOL)
    {
        SQBool val = SQFalse;
        // Attempt to get the value from the stack
        sq_getbool(vm, idx, &val);
        // Save it into the specified memory location
        *num = static_cast< Uint64 >(val);
    }
    // Unrecognized value
    else
    {
        return SQ_ERROR;
    }
    // Value retrieved
    return SQ_OK;
}

// ------------------------------------------------------------------------------------------------
static SQRESULT SqModImpl_PushULongObject(HSQUIRRELVM vm, Uint64 num)
{
    // Attempt to push the requested instance
    try
    {
        Var< const ULongInt & >::push(vm, ULongInt(num));
    }
    catch (...)
    {
        // Specify that we failed
        return SQ_ERROR;
    }
    // Specify that we succeeded
    return SQ_OK;
}

// ------------------------------------------------------------------------------------------------
SQBool SqModImpl_ValidDate(uint16_t year, uint8_t month, uint8_t day)
{
    return Chrono::ValidDate(year, month, day);
}

// ------------------------------------------------------------------------------------------------
SQBool SqModImpl_IsLeapYear(uint16_t year)
{
    return Chrono::IsLeapYear(year);
}

// ------------------------------------------------------------------------------------------------
static SQRESULT SqModImpl_GetTimestamp(HSQUIRRELVM vm, SQInteger idx, Int64 * num)
{
    // Validate the specified number pointer and value type
    if (!num)
    {
        return SQ_ERROR; // Nowhere to save
    }
    // Is this an instance that we can treat as a Time-stamp type?
    else if (sq_gettype(vm, idx) == OT_INSTANCE)
    {
        // Attempt to obtain the time-stamp and it's value from the stack
        try
        {
            *num =  static_cast< Int64 >(Var< const Timestamp & >(vm, idx).value.GetNum());
        }
        catch (...)
        {
            return SQ_ERROR; // Unable to obtain the value!
        }
    }
    // Is this a pure integer value?
    else if(sq_gettype(vm, idx) == OT_INTEGER)
    {
        SQInteger val = 0;
        // Attempt to get the value from the stack
        sq_getinteger(vm, idx, &val);
        // Save it into the specified memory location
        *num = static_cast< Int64 >(val);
    }
    // Is this a pure floating point value?
    else if(sq_gettype(vm, idx) == OT_FLOAT)
    {
        SQFloat val = 0.0;
        // Attempt to get the value from the stack
        sq_getfloat(vm, idx, &val);
        // Save it into the specified memory location
        *num = static_cast< Int64 >(std::llround(val));
    }
    // Is this a pure boolean value?
    else if(sq_gettype(vm, idx) == OT_BOOL)
    {
        SQBool val = SQFalse;
        // Attempt to get the value from the stack
        sq_getbool(vm, idx, &val);
        // Save it into the specified memory location
        *num = static_cast< Int64 >(val);
    }
    // Unrecognized value
    else
    {
        return SQ_ERROR;
    }
    // Value retrieved
    return SQ_OK;
}

// ------------------------------------------------------------------------------------------------
static SQRESULT SqModImpl_PushTimestamp(HSQUIRRELVM vm, Int64 num)
{
    // Attempt to push the requested instance
    try
    {
        Var< const Timestamp & >::push(vm, Timestamp(num));
    }
    catch (...)
    {
        // Specify that we failed
        return SQ_ERROR;
    }
    // Specify that we succeeded
    return SQ_OK;
}

// ------------------------------------------------------------------------------------------------
SQRESULT SqModImpl_GetDate(HSQUIRRELVM vm, SQInteger idx, uint16_t * year, uint8_t * month, uint8_t * day)
{
    // Is this an instance that we can treat as a Date type?
    if (sq_gettype(vm, idx) == OT_INSTANCE)
    {
        // Attempt to obtain the time-stamp and it's value from the stack
        try
        {
            // Attempt to retrieve the instance
            Var< Date * > var(vm, idx);
            // Assign the year
            if (year != nullptr)
            {
                *year = var.value->GetYear();
            }
            // Assign the month
            if (month != nullptr)
            {
                *month = var.value->GetMonth();
            }
            // Assign the day
            if (day != nullptr)
            {
                *day = var.value->GetDay();
            }
        }
        catch (...)
        {
            return SQ_ERROR; // Unable to obtain the value!
        }
    }
    // Unrecognized value
    else
    {
        return SQ_ERROR;
    }
    // Value retrieved
    return SQ_OK;
}

// ------------------------------------------------------------------------------------------------
SQRESULT SqModImpl_PushDate(HSQUIRRELVM vm, uint16_t year, uint8_t month, uint8_t day)
{
    // Attempt to push the requested instance
    try
    {
        Var< const Date & >::push(vm, Date(year, month, day));
    }
    catch (...)
    {
        // Specify that we failed
        return SQ_ERROR;
    }
    // Specify that we succeeded
    return SQ_OK;
}

// ------------------------------------------------------------------------------------------------
SQRESULT SqModImpl_GetTime(HSQUIRRELVM vm, SQInteger idx, uint8_t * hour, uint8_t * minute,
                        uint8_t * second, uint16_t * millisecond)
{
    // Is this an instance that we can treat as a Time type?
    if (sq_gettype(vm, idx) == OT_INSTANCE)
    {
        // Attempt to obtain the time-stamp and it's value from the stack
        try
        {
            // Attempt to retrieve the instance
            Var< Time * > var(vm, idx);
            // Assign the hour
            if (hour != nullptr)
            {
                *hour = var.value->GetHour();
            }
            // Assign the minute
            if (minute != nullptr)
            {
                *minute = var.value->GetMinute();
            }
            // Assign the second
            if (second != nullptr)
            {
                *second = var.value->GetSecond();
            }
            // Assign the millisecond
            if (millisecond != nullptr)
            {
                *millisecond = var.value->GetMillisecond();
            }
        }
        catch (...)
        {
            return SQ_ERROR; // Unable to obtain the value!
        }
    }
    // Unrecognized value
    else
    {
        return SQ_ERROR;
    }
    // Value retrieved
    return SQ_OK;
}

// ------------------------------------------------------------------------------------------------
SQRESULT SqModImpl_PushTime(HSQUIRRELVM vm, uint8_t hour, uint8_t minute, uint8_t second,
                        uint16_t millisecond)
{
    // Attempt to push the requested instance
    try
    {
        Var< const Time & >::push(vm, Time(hour, minute, second, millisecond));
    }
    catch (...)
    {
        // Specify that we failed
        return SQ_ERROR;
    }
    // Specify that we succeeded
    return SQ_OK;
}

// ------------------------------------------------------------------------------------------------
SQRESULT SqModImpl_GetDatetime(HSQUIRRELVM vm, SQInteger idx, uint16_t * year, uint8_t * month, uint8_t * day,
                            uint8_t * hour, uint8_t * minute, uint8_t * second, uint16_t * millisecond)
{
    // Is this an instance that we can treat as a Date-time type?
    if (sq_gettype(vm, idx) == OT_INSTANCE)
    {
        // Attempt to obtain the time-stamp and it's value from the stack
        try
        {
            // Attempt to retrieve the instance
            Var< Datetime * > var(vm, idx);
            // Assign the year
            if (year != nullptr)
            {
                *year = var.value->GetYear();
            }
            // Assign the month
            if (month != nullptr)
            {
                *month = var.value->GetMonth();
            }
            // Assign the day
            if (day != nullptr)
            {
                *day = var.value->GetDay();
            }
            // Assign the hour
            if (hour != nullptr)
            {
                *hour = var.value->GetHour();
            }
            // Assign the minute
            if (minute != nullptr)
            {
                *minute = var.value->GetMinute();
            }
            // Assign the second
            if (second != nullptr)
            {
                *second = var.value->GetSecond();
            }
            // Assign the millisecond
            if (millisecond != nullptr)
            {
                *millisecond = var.value->GetMillisecond();
            }
        }
        catch (...)
        {
            return SQ_ERROR; // Unable to obtain the value!
        }
    }
    // Unrecognized value
    else
    {
        return SQ_ERROR;
    }
    // Value retrieved
    return SQ_OK;
}

// ------------------------------------------------------------------------------------------------
SQRESULT SqModImpl_PushDatetime(HSQUIRRELVM vm, uint16_t year, uint8_t month, uint8_t day,
                            uint8_t hour, uint8_t minute, uint8_t second, uint16_t millisecond)
{
    // Attempt to push the requested instance
    try
    {
        Var< const Datetime & >::push(vm, Datetime(year, month, day, hour, minute, second, millisecond));
    }
    catch (...)
    {
        // Specify that we failed
        return SQ_ERROR;
    }
    // Specify that we succeeded
    return SQ_OK;
}

// ------------------------------------------------------------------------------------------------
SQRESULT SqModImpl_PushBuffer(HSQUIRRELVM vm, SQInteger size, SQInteger cursor)
{
    // Attempt to push the requested instance
    try
    {
        Var< const SqBuffer & >::push(vm, SqBuffer(size, cursor));
    }
    catch (...)
    {
        // Specify that we failed
        return SQ_ERROR;
    }
    // Specify that we succeeded
    return SQ_OK;
}

// ------------------------------------------------------------------------------------------------
SQRESULT SqModImpl_PushBufferData(HSQUIRRELVM vm, const char * data, SQInteger size, SQInteger cursor)
{
    // Attempt to push the requested instance
    try
    {
        Var< const SqBuffer & >::push(vm, SqBuffer(data, size, cursor));
    }
    catch (...)
    {
        // Specify that we failed
        return SQ_ERROR;
    }
    // Specify that we succeeded
    return SQ_OK;
}

// ------------------------------------------------------------------------------------------------
SQRESULT SqModImpl_GetBufferInfo(HSQUIRRELVM vm, SQInteger idx, const char ** ptr, SQInteger * size, SQInteger * cursor)
{
    // Attempt to obtain the requested information
    try
    {
        // Attempt to retrieve the instance
        Var< SqBuffer * > var(vm, idx);
        // Validate the obtained buffer
        if (!(var.value) || !(var.value->GetRef()) || !(*var.value->GetRef()))
        {
            // Should we obtain the buffer contents?
            if (ptr)
            {
                *ptr = nullptr; // Default to null data
            }
            // Should we obtain the buffer length?
            if (size)
            {
                *size = 0; // Default to 0 length
            }
            // Should we obtain the cursor position?
            if (cursor)
            {
                *cursor = 0; // Default to position 0
            }
        }
        // Grab the internal buffer
        const Buffer & b = *var.value->GetRef();
        // Should we obtain the buffer contents?
        if (ptr)
        {
            *ptr = b.Data();
        }
        // Should we obtain the buffer length?
        if (size)
        {
            *size = ConvTo< SQInteger >::From(b.Capacity());
        }
        // Should we obtain the cursor position?
        if (cursor)
        {
            *cursor = ConvTo< SQInteger >::From(b.Position());
        }
    }
    catch (...)
    {
        // Specify that we failed
        return SQ_ERROR;
    }
    // Specify that we succeeded
    return SQ_OK;
}

// ------------------------------------------------------------------------------------------------
const char * SqModImpl_GetBufferData(HSQUIRRELVM vm, SQInteger idx)
{
    // Attempt to obtain the requested information
    try
    {
        // Attempt to retrieve the instance
        Var< SqBuffer * > var(vm, idx);
        // Validate the obtained buffer and return the requested information
        if ((var.value) && (var.value->GetRef()) && (*var.value->GetRef()))
        {
            return var.value->GetRef()->Data();
        }
    }
    catch (...)
    {
        // Just ignore it...
    }
    // Specify that we failed
    return nullptr;
}

// ------------------------------------------------------------------------------------------------
SQInteger SqModImpl_GetBufferSize(HSQUIRRELVM vm, SQInteger idx)
{
    // Attempt to obtain the requested information
    try
    {
        // Attempt to retrieve the instance
        Var< SqBuffer * > var(vm, idx);
        // Validate the obtained buffer and return the requested information
        if ((var.value) && (var.value->GetRef()) && (*var.value->GetRef()))
        {
            return ConvTo< SQInteger >::From(var.value->GetRef()->Capacity());
        }
    }
    catch (...)
    {
        // Just ignore it...
    }
    // Specify that we failed
    return -1;
}

// ------------------------------------------------------------------------------------------------
SQInteger SqModImpl_GetBufferCursor(HSQUIRRELVM vm, SQInteger idx)
{
    // Attempt to obtain the requested information
    try
    {
        // Attempt to retrieve the instance
        Var< SqBuffer * > var(vm, idx);
        // Validate the obtained buffer and return the requested information
        if ((var.value) && (var.value->GetRef()) && (*var.value->GetRef()))
        {
            return ConvTo< SQInteger >::From(var.value->GetRef()->Position());
        }
    }
    catch (...)
    {
        // Just ignore it...
    }
    // Specify that we failed
    return -1;
}

// ------------------------------------------------------------------------------------------------
static int32_t SqExport_PopulateModuleAPI(HSQMODAPI api, size_t size)
{
    if (!api)
    {
        return 0; // Nothing to populate!
    }
    else if (size != sizeof(SQMODAPI))
    {
        return -1; // Incompatible API!
    }

    //primitive functions
    api->GetSquirrelVM              = GetSquirrelVM;

    //logging utilities
    api->LogDbg                     = LogDbg;
    api->LogUsr                     = LogUsr;
    api->LogScs                     = LogScs;
    api->LogInf                     = LogInf;
    api->LogWrn                     = LogWrn;
    api->LogErr                     = LogErr;
    api->LogFtl                     = LogFtl;
    api->LogSDbg                    = LogSDbg;
    api->LogSUsr                    = LogSUsr;
    api->LogSScs                    = LogSScs;
    api->LogSInf                    = LogSInf;
    api->LogSWrn                    = LogSWrn;
    api->LogSErr                    = LogSErr;
    api->LogSFtl                    = LogSFtl;

    //script loading
    api->LoadScript                 = SqModImpl_LoadScript;

    //numeric utilities
    api->GetSLongValue              = SqModImpl_GetSLongValue;
    api->PushSLongObject            = SqModImpl_PushSLongObject;
    api->GetULongValue              = SqModImpl_GetULongValue;
    api->PushULongObject            = SqModImpl_PushULongObject;

    //time utilities
    api->GetCurrentSysTime          = Chrono::GetCurrentSysTime;
    api->GetEpochTimeMicro          = Chrono::GetEpochTimeMicro;
    api->GetEpochTimeMilli          = Chrono::GetEpochTimeMilli;
    api->ValidDate                  = SqModImpl_ValidDate;
    api->IsLeapYear                 = SqModImpl_IsLeapYear;
    api->DaysInYear                 = Chrono::DaysInYear;
    api->DaysInMonth                = Chrono::DaysInMonth;
    api->DayOfYear                  = Chrono::DayOfYear;
    api->DateRangeToSeconds         = Chrono::DateRangeToSeconds;
    api->GetTimestamp               = SqModImpl_GetTimestamp;
    api->PushTimestamp              = SqModImpl_PushTimestamp;
    api->GetDate                    = SqModImpl_GetDate;
    api->PushDate                   = SqModImpl_PushDate;
    api->GetTime                    = SqModImpl_GetTime;
    api->PushTime                   = SqModImpl_PushTime;
    api->GetDatetime                = SqModImpl_GetDatetime;
    api->PushDatetime               = SqModImpl_PushDatetime;

    //stack utilities
    api->PopStackInteger            = PopStackInteger;
    api->PopStackFloat              = PopStackFloat;
    api->PopStackSLong              = PopStackSLong;
    api->PopStackULong              = PopStackULong;

    //buffer utilities
    api->PushBuffer                 = SqModImpl_PushBuffer;
    api->PushBufferData             = SqModImpl_PushBufferData;
    api->GetBufferInfo              = SqModImpl_GetBufferInfo;
    api->GetBufferData              = SqModImpl_GetBufferData;
    api->GetBufferSize              = SqModImpl_GetBufferSize;
    api->GetBufferCursor            = SqModImpl_GetBufferCursor;

    return 1; // Successfully populated!
}

// ------------------------------------------------------------------------------------------------
static int32_t SqExport_PopulateSquirrelAPI(HSQLIBAPI api, size_t size)
{
    if (!api)
    {
        return 0; // Nothing to populate!
    }
    else if (size != sizeof(SQLIBAPI))
    {
        return -1; // Incompatible API!
    }

    //vm
    api->open                           = sq_open;
    api->newthread                      = sq_newthread;
    api->seterrorhandler                = sq_seterrorhandler;
    api->close                          = sq_close;
    api->setforeignptr                  = sq_setforeignptr;
    api->getforeignptr                  = sq_getforeignptr;
    api->setsharedforeignptr            = sq_setsharedforeignptr;
    api->getsharedforeignptr            = sq_getsharedforeignptr;
    api->setvmreleasehook               = sq_setvmreleasehook;
    api->getvmreleasehook               = sq_getvmreleasehook;
    api->setsharedreleasehook           = sq_setsharedreleasehook;
    api->getsharedreleasehook           = sq_getsharedreleasehook;
    api->setprintfunc                   = sq_setprintfunc;
    api->getprintfunc                   = sq_getprintfunc;
    api->geterrorfunc                   = sq_geterrorfunc;
    api->suspendvm                      = sq_suspendvm;
    api->wakeupvm                       = sq_wakeupvm;
    api->getvmstate                     = sq_getvmstate;
    api->getversion                     = sq_getversion;

    //compiler
    api->compile                        = sq_compile;
    api->compilebuffer                  = sq_compilebuffer;
    api->enabledebuginfo                = sq_enabledebuginfo;
    api->notifyallexceptions            = sq_notifyallexceptions;
    api->setcompilererrorhandler        = sq_setcompilererrorhandler;

    //stack operations
    api->push                           = sq_push;
    api->pop                            = sq_pop;
    api->poptop                         = sq_poptop;
    api->remove                         = sq_remove;
    api->gettop                         = sq_gettop;
    api->settop                         = sq_settop;
    api->reservestack                   = sq_reservestack;
    api->cmp                            = sq_cmp;
    api->move                           = sq_move;

    //object creation handling
    api->newuserdata                    = sq_newuserdata;
    api->newtable                       = sq_newtable;
    api->newtableex                     = sq_newtableex;
    api->newarray                       = sq_newarray;
    api->newclosure                     = sq_newclosure;
    api->setparamscheck                 = sq_setparamscheck;
    api->bindenv                        = sq_bindenv;
    api->setclosureroot                 = sq_setclosureroot;
    api->getclosureroot                 = sq_getclosureroot;
    api->pushstring                     = sq_pushstring;
    api->pushfloat                      = sq_pushfloat;
    api->pushinteger                    = sq_pushinteger;
    api->pushbool                       = sq_pushbool;
    api->pushuserpointer                = sq_pushuserpointer;
    api->pushnull                       = sq_pushnull;
    api->pushthread                     = sq_pushthread;
    api->gettype                        = sq_gettype;
    api->typeof_                        = sq_typeof;
    api->getsize                        = sq_getsize;
    api->gethash                        = sq_gethash;
    api->getbase                        = sq_getbase;
    api->instanceof                     = sq_instanceof;
    api->tostring                       = sq_tostring;
    api->tobool                         = sq_tobool;
    api->getstringandsize               = sq_getstringandsize;
    api->getstring                      = sq_getstring;
    api->getinteger                     = sq_getinteger;
    api->getfloat                       = sq_getfloat;
    api->getbool                        = sq_getbool;
    api->getthread                      = sq_getthread;
    api->getuserpointer                 = sq_getuserpointer;
    api->getuserdata                    = sq_getuserdata;
    api->settypetag                     = sq_settypetag;
    api->gettypetag                     = sq_gettypetag;
    api->setreleasehook                 = sq_setreleasehook;
    api->getreleasehook                 = sq_getreleasehook;
    api->getscratchpad                  = sq_getscratchpad;
    api->getfunctioninfo                = sq_getfunctioninfo;
    api->getclosureinfo                 = sq_getclosureinfo;
    api->getclosurename                 = sq_getclosurename;
    api->setnativeclosurename           = sq_setnativeclosurename;
    api->setinstanceup                  = sq_setinstanceup;
    api->getinstanceup                  = sq_getinstanceup;
    api->setclassudsize                 = sq_setclassudsize;
    api->newclass                       = sq_newclass;
    api->createinstance                 = sq_createinstance;
    api->setattributes                  = sq_setattributes;
    api->getattributes                  = sq_getattributes;
    api->getclass                       = sq_getclass;
    api->weakref                        = sq_weakref;
    api->getdefaultdelegate             = sq_getdefaultdelegate;
    api->getmemberhandle                = sq_getmemberhandle;
    api->getbyhandle                    = sq_getbyhandle;
    api->setbyhandle                    = sq_setbyhandle;

    //object manipulation
    api->pushroottable                  = sq_pushroottable;
    api->pushregistrytable              = sq_pushregistrytable;
    api->pushconsttable                 = sq_pushconsttable;
    api->setroottable                   = sq_setroottable;
    api->setconsttable                  = sq_setconsttable;
    api->newslot                        = sq_newslot;
    api->deleteslot                     = sq_deleteslot;
    api->set                            = sq_set;
    api->get                            = sq_get;
    api->rawget                         = sq_rawget;
    api->rawset                         = sq_rawset;
    api->rawdeleteslot                  = sq_rawdeleteslot;
    api->newmember                      = sq_newmember;
    api->rawnewmember                   = sq_rawnewmember;
    api->arrayappend                    = sq_arrayappend;
    api->arraypop                       = sq_arraypop;
    api->arrayresize                    = sq_arrayresize;
    api->arrayreverse                   = sq_arrayreverse;
    api->arrayremove                    = sq_arrayremove;
    api->arrayinsert                    = sq_arrayinsert;
    api->setdelegate                    = sq_setdelegate;
    api->getdelegate                    = sq_getdelegate;
    api->clone                          = sq_clone;
    api->setfreevariable                = sq_setfreevariable;
    api->next                           = sq_next;
    api->getweakrefval                  = sq_getweakrefval;
    api->clear                          = sq_clear;

    //calls
    api->call                           = sq_call;
    api->resume                         = sq_resume;
    api->getlocal                       = sq_getlocal;
    api->getcallee                      = sq_getcallee;
    api->getfreevariable                = sq_getfreevariable;
    api->throwerror                     = sq_throwerror;
    api->throwobject                    = sq_throwobject;
    api->reseterror                     = sq_reseterror;
    api->getlasterror                   = sq_getlasterror;

    //raw object handling
    api->getstackobj                    = sq_getstackobj;
    api->pushobject                     = sq_pushobject;
    api->addref                         = sq_addref;
    api->release                        = sq_release;
    api->getrefcount                    = sq_getrefcount;
    api->resetobject                    = sq_resetobject;
    api->objtostring                    = sq_objtostring;
    api->objtobool                      = sq_objtobool;
    api->objtointeger                   = sq_objtointeger;
    api->objtofloat                     = sq_objtofloat;
    api->objtouserpointer               = sq_objtouserpointer;
    api->getobjtypetag                  = sq_getobjtypetag;
    api->getvmrefcount                  = sq_getvmrefcount;

    //GC
    api->collectgarbage                 = sq_collectgarbage;
    api->resurrectunreachable           = sq_resurrectunreachable;

    //serialization
    api->writeclosure                   = sq_writeclosure;
    api->readclosure                    = sq_readclosure;

    //mem allocation
    api->malloc                         = sq_malloc;
    api->realloc                        = sq_realloc;
    api->free                           = sq_free;

    //debug
    api->stackinfos                     = sq_stackinfos;
    api->setdebughook                   = sq_setdebughook;
    api->setnativedebughook             = sq_setnativedebughook;

    //compiler helpers
    api->loadfile                       = sqstd_loadfile;
    api->dofile                         = sqstd_dofile;
    api->writeclosuretofile             = sqstd_writeclosuretofile;

    //blob
    api->createblob                     = sqstd_createblob;
    api->getblob                        = sqstd_getblob;
    api->getblobsize                    = sqstd_getblobsize;

    //string
    api->format                         = sqstd_format;

    return 1; // Successfully populated!
}

// ------------------------------------------------------------------------------------------------
static const SQMODEXPORTS g_SqModExports{
    sizeof(SQMODEXPORTS),
    SqExport_PopulateModuleAPI,
    SqExport_PopulateSquirrelAPI
};

// ------------------------------------------------------------------------------------------------
void InitExports()
{
    // The server needs a pointer to a pointer, and a persistent one
    static const SQMODEXPORTS * sqmodexports = &g_SqModExports;

    // Tell the server about the pointer to the exports structure
    _Func->ExportFunctions(_Info->pluginId, reinterpret_cast< const void ** >(&sqmodexports),
                            sizeof(HSQMODEXPORTS));
}

} // Namespace:: SqMod
