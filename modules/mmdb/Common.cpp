// ------------------------------------------------------------------------------------------------
#include "Common.hpp"

// ------------------------------------------------------------------------------------------------
#include <cstdio>
#include <cstdlib>
#include <cstring>


// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
// N[0] - contains least significant bits, N[3] - most significant
static SQChar * Bin128ToDec(const Uint32 N[4])
{
    // log10(x) = log2(x) / log2(10) ~= log2(x) / 3.322
    static SQChar s[128 / 3 + 1 + 1];
    Uint32 n[4];
    SQChar * p = s;
    int i;

    std::memset(s, '0', sizeof(s) - 1);
    s[sizeof(s) - 1] = '\0';

    std::memcpy(n, N, sizeof(n));

    for (i = 0; i < 128; i++)
    {
        int j, carry;

        carry = (n[3] >= 0x80000000);
        // Shift n[] left, doubling it
        n[3] = ((n[3] << 1) & 0xFFFFFFFF) + (n[2] >= 0x80000000);
        n[2] = ((n[2] << 1) & 0xFFFFFFFF) + (n[1] >= 0x80000000);
        n[1] = ((n[1] << 1) & 0xFFFFFFFF) + (n[0] >= 0x80000000);
        n[0] = ((n[0] << 1) & 0xFFFFFFFF);

        // Add s[] to itself in decimal, doubling it
        for (j = sizeof(s) - 2; j >= 0; j--)
        {
            s[j] += s[j] - '0' + carry;

            carry = (s[j] > '9');

            if (carry)
            {
                s[j] -= 10;
            }
        }
    }

    while ((p[0] == '0') && (p < &s[sizeof(s) - 2]))
    {
        p++;
    }

    return p;
}

// ------------------------------------------------------------------------------------------------
CSStr AsTypeStr(Uint32 id)
{
    switch (id)
    {
        case MMDB_DATA_TYPE_EXTENDED:       return _SC("extended");
        case MMDB_DATA_TYPE_POINTER:        return _SC("pointer");
        case MMDB_DATA_TYPE_UTF8_STRING:    return _SC("string");
        case MMDB_DATA_TYPE_DOUBLE:         return _SC("double");
        case MMDB_DATA_TYPE_BYTES:          return _SC("bytes");
        case MMDB_DATA_TYPE_UINT16:         return _SC("uint16");
        case MMDB_DATA_TYPE_UINT32:         return _SC("uint32");
        case MMDB_DATA_TYPE_MAP:            return _SC("map");
        case MMDB_DATA_TYPE_INT32:          return _SC("int32");
        case MMDB_DATA_TYPE_UINT64:         return _SC("uint64");
        case MMDB_DATA_TYPE_UINT128:        return _SC("uint128");
        case MMDB_DATA_TYPE_ARRAY:          return _SC("array");
        case MMDB_DATA_TYPE_CONTAINER:      return _SC("container");
        case MMDB_DATA_TYPE_END_MARKER:     return _SC("endmarker");
        case MMDB_DATA_TYPE_BOOLEAN:        return _SC("boolean");
        case MMDB_DATA_TYPE_FLOAT:          return _SC("float");
        default:                            return _SC("unknonw");
    }
}

// ------------------------------------------------------------------------------------------------
bool GetEntryAsBool(const MMDB_entry_data_s & ed)
{
    bool value = false;
    // Identify the type of entry data
    switch (ed.type)
    {
        case MMDB_DATA_TYPE_POINTER: {
            value = ed.pointer > 0;
        } break;
        case MMDB_DATA_TYPE_UTF8_STRING: {
            if (ed.data_size > 0)
            {
                value = ConvTo< bool >::From(reinterpret_cast< CSStr >(ed.utf8_string));
            }
        } break;
        case MMDB_DATA_TYPE_DOUBLE: {
            value = ConvTo< bool >::From(ed.double_value);
        } break;
        case MMDB_DATA_TYPE_BYTES: {
            for (Uint32 i = 0; i < ed.data_size; ++i)
            {
                if (ed.bytes[i] != 0)
                {
                    value = true;
                    // Found somethinf that isn't 0
                    break;
                }
            }
        } break;
        case MMDB_DATA_TYPE_UINT16: {
            value = ConvTo< bool >::From(ed.uint16);
        } break;
        case MMDB_DATA_TYPE_UINT32: {
            value = ConvTo< bool >::From(ed.uint16);
        } break;
        case MMDB_DATA_TYPE_INT32: {
            value = ConvTo< bool >::From(ed.uint16);
        } break;
        case MMDB_DATA_TYPE_UINT64: {
            value = ConvTo< bool >::From(ed.uint16);
        } break;
        case MMDB_DATA_TYPE_UINT128: {
#if defined(MMDB_UINT128_IS_BYTE_ARRAY) && (MMDB_UINT128_IS_BYTE_ARRAY == 1)
            for (Uint32 i = 0; i < sizeof(ed.uint128); ++i)
            {
                if (ed.uint128[i] != 0)
                {
                    value = true;
                    // Found somethinf that isn't 0
                    break;
                }
            }
#else
            value = ed.uint128 > 0;
#endif // MMDB_UINT128_IS_BYTE_ARRAY

        } break;
        case MMDB_DATA_TYPE_BOOLEAN: {
            value = ed.boolean ? true : false;
        } break;
        case MMDB_DATA_TYPE_FLOAT: {
            value = ConvTo< bool >::From(ed.float_value);
        } break;
        default:
            STHROWF("Unsupported conversion from (%s) to (boolean)", AsTypeStr(ed.type));
    }
    // Return the extracted value
    return value;
}

// ------------------------------------------------------------------------------------------------
SQInteger GetEntryAsInteger(const MMDB_entry_data_s & ed)
{
    SQInteger value = 0;
    // Identify the type of entry data
    switch (ed.type)
    {
        case MMDB_DATA_TYPE_POINTER: {
            value = static_cast< SQInteger >(ed.pointer);
        } break;
        case MMDB_DATA_TYPE_UTF8_STRING: {
            if (ed.data_size > 0)
            {
                value = ConvTo< SQInteger >::From(reinterpret_cast< CSStr >(ed.utf8_string));
            }
        } break;
        case MMDB_DATA_TYPE_DOUBLE: {
            value = ConvTo< SQInteger >::From(ed.double_value);
        } break;
        case MMDB_DATA_TYPE_BYTES: {
            std::memcpy(&value, ed.bytes, Clamp(ed.data_size, 0U, sizeof(value)));
        } break;
        case MMDB_DATA_TYPE_UINT16: {
            value = ConvTo< SQInteger >::From(ed.uint16);
        } break;
        case MMDB_DATA_TYPE_UINT32: {
            value = ConvTo< SQInteger >::From(ed.uint32);
        } break;
        case MMDB_DATA_TYPE_INT32: {
            value = ConvTo< SQInteger >::From(ed.int32);
        } break;
        case MMDB_DATA_TYPE_UINT64: {
            value = ConvTo< SQInteger >::From(ed.uint64);
        } break;
        case MMDB_DATA_TYPE_UINT128: {
#if defined(MMDB_UINT128_IS_BYTE_ARRAY) && (MMDB_UINT128_IS_BYTE_ARRAY == 1)
            std::memcpy(&value, ed.uint128, sizeof(value));
#else
            std::memcpy(&value, &ed.uint128, sizeof(value));
#endif // MMDB_UINT128_IS_BYTE_ARRAY
        } break;
        case MMDB_DATA_TYPE_BOOLEAN: {
            value = ed.boolean ? 1 : 0;
        } break;
        case MMDB_DATA_TYPE_FLOAT: {
            value = ConvTo< SQInteger >::From(ed.float_value);
        } break;
        default:
            STHROWF("Unsupported conversion from (%s) to (integer)", AsTypeStr(ed.type));
    }
    // Return the extracted value
    return value;
}

// ------------------------------------------------------------------------------------------------
SQFloat GetEntryAsFloat(const MMDB_entry_data_s & ed)
{
    SQFloat value = 0.0;
    // Identify the type of entry data
    switch (ed.type)
    {
        case MMDB_DATA_TYPE_POINTER: {
            value = ConvTo< SQFloat >::From(static_cast< SQInteger >(ed.pointer));
        } break;
        case MMDB_DATA_TYPE_UTF8_STRING: {
            if (ed.data_size > 0)
            {
                value = ConvTo< SQFloat >::From(reinterpret_cast< CSStr >(ed.utf8_string));
            }
        } break;
        case MMDB_DATA_TYPE_DOUBLE: {
            value = ConvTo< SQFloat >::From(ed.double_value);
        } break;
        case MMDB_DATA_TYPE_BYTES: {
            // Not our problem if the result will be junk!
            std::memcpy(&value, ed.bytes, Clamp(ed.data_size, 0U, sizeof(value)));
        } break;
        case MMDB_DATA_TYPE_UINT16: {
            value = ConvTo< SQFloat >::From(ed.uint16);
        } break;
        case MMDB_DATA_TYPE_UINT32: {
            value = ConvTo< SQFloat >::From(ed.uint32);
        } break;
        case MMDB_DATA_TYPE_INT32: {
            value = ConvTo< SQFloat >::From(ed.int32);
        } break;
        case MMDB_DATA_TYPE_UINT64: {
            value = ConvTo< SQFloat >::From(ed.uint64);
        } break;
        case MMDB_DATA_TYPE_UINT128: {
            SQInteger num;
            // Convert to integer first
#if defined(MMDB_UINT128_IS_BYTE_ARRAY) && (MMDB_UINT128_IS_BYTE_ARRAY == 1)
            std::memcpy(&num, ed.uint128, sizeof(num));
#else
            std::memcpy(&num, &ed.uint128, sizeof(num));
#endif // MMDB_UINT128_IS_BYTE_ARRAY
            // Now convert to float
            value = ConvTo< SQFloat >::From(num);
        } break;
        case MMDB_DATA_TYPE_BOOLEAN: {
            value = ed.boolean ? 1.0 : 0.0;
        } break;
        case MMDB_DATA_TYPE_FLOAT: {
            value = ConvTo< SQFloat >::From(ed.float_value);
        } break;
        default:
            STHROWF("Unsupported conversion from (%s) to (float)", AsTypeStr(ed.type));
    }
    // Return the extracted value
    return value;
}

// ------------------------------------------------------------------------------------------------
Object GetEntryAsLong(const MMDB_entry_data_s & ed)
{
    Uint64 value = 0;
    // Identify the type of entry data
    switch (ed.type)
    {
        case MMDB_DATA_TYPE_POINTER: {
            value = static_cast< Uint64 >(ed.pointer);
        } break;
        case MMDB_DATA_TYPE_UTF8_STRING: {
            if (ed.data_size > 0)
            {
                value = ConvTo< Uint64 >::From(reinterpret_cast< CSStr >(ed.utf8_string));
            }
        } break;
        case MMDB_DATA_TYPE_DOUBLE: {
            value = ConvTo< Uint64 >::From(ed.double_value);
        } break;
        case MMDB_DATA_TYPE_BYTES: {
            std::memcpy(&value, ed.bytes, Clamp(ed.data_size, 0U, sizeof(value)));
        } break;
        case MMDB_DATA_TYPE_UINT16: {
            value = ConvTo< Uint64 >::From(ed.uint16);
        } break;
        case MMDB_DATA_TYPE_UINT32: {
            value = ConvTo< Uint64 >::From(ed.uint32);
        } break;
        case MMDB_DATA_TYPE_INT32: {
            value = ConvTo< Uint64 >::From(ed.int32);
        } break;
        case MMDB_DATA_TYPE_UINT64: {
            value = ConvTo< Uint64 >::From(ed.uint64);
        } break;
        case MMDB_DATA_TYPE_UINT128: {
#if defined(MMDB_UINT128_IS_BYTE_ARRAY) && (MMDB_UINT128_IS_BYTE_ARRAY == 1)
            std::memcpy(&value, ed.uint128, sizeof(value));
#else
            std::memcpy(&value, &ed.uint128, sizeof(value));
#endif // MMDB_UINT128_IS_BYTE_ARRAY
        } break;
        case MMDB_DATA_TYPE_BOOLEAN: {
            value = ed.boolean ? 1 : 0;
        } break;
        case MMDB_DATA_TYPE_FLOAT: {
            value = ConvTo< Uint64 >::From(ed.float_value);
        } break;
        default:
            STHROWF("Unsupported conversion from (%s) to (long)", AsTypeStr(ed.type));
    }
    // Obtain the initial stack size
    const StackGuard sg;
    // Push a long integer instance with the requested value on the stack
    SqMod_PushULongObject(DefaultVM::Get(), value);
    // Obtain the object from the stack and return it
    return Var< Object >(DefaultVM::Get(), -1).value;
}

// ------------------------------------------------------------------------------------------------
Object GetEntryAsString(const MMDB_entry_data_s & ed)
{
    // Obtain the initial stack size
    const StackGuard sg;
    // The default vm
    HSQUIRRELVM vm = DefaultVM::Get();
    // Identify the type of entry data
    switch (ed.type)
    {
        case MMDB_DATA_TYPE_POINTER: {
            sq_pushstring(vm, ToStrF("%p", ed.pointer), -1);
        } break;
        case MMDB_DATA_TYPE_UTF8_STRING: {
            sq_pushstring(vm, ed.utf8_string, ed.data_size);
        } break;
        case MMDB_DATA_TYPE_DOUBLE: {
            sq_pushstring(vm, ToStrF("%f", ed.double_value), -1);
        } break;
        case MMDB_DATA_TYPE_BYTES: {
            sq_pushstring(vm, reinterpret_cast< CSStr >(ed.bytes), ed.data_size / sizeof(SQChar));
        } break;
        case MMDB_DATA_TYPE_UINT16: {
            sq_pushstring(vm, ToStrF("%u", ed.uint16), -1);
        } break;
        case MMDB_DATA_TYPE_UINT32: {
            sq_pushstring(vm, ToStrF("%u", ed.uint32), -1);
        } break;
        case MMDB_DATA_TYPE_INT32: {
            sq_pushstring(vm, ToStrF("%d", ed.int32), -1);
        } break;
        case MMDB_DATA_TYPE_UINT64: {
            sq_pushstring(vm, ToStrF("%llu", ed.uint64), -1);
        } break;
        case MMDB_DATA_TYPE_UINT128: {
#if defined(MMDB_UINT128_IS_BYTE_ARRAY) && (MMDB_UINT128_IS_BYTE_ARRAY == 1)
            sq_pushstring(vm, Bin128ToDec(reinterpret_cast< const Uint32 * >(ed.uint128)), -1);
#else
            sq_pushstring(vm, Bin128ToDec(reinterpret_cast< const Uint32 * >(&ed.uint128)), -1);
#endif // MMDB_UINT128_IS_BYTE_ARRAY
        } break;
        case MMDB_DATA_TYPE_BOOLEAN: {
            sq_pushstring(vm, ed.boolean ? _SC("true") : _SC("false"), -1);
        } break;
        case MMDB_DATA_TYPE_FLOAT: {
            sq_pushstring(vm, ToStrF("%f", ed.float_value), -1);
        } break;
        default:
            STHROWF("Unsupported conversion from (%s) to (string)", AsTypeStr(ed.type));
    }
    // Obtain the object from the stack and return it
    return Var< Object >(vm, -1).value;
}

// ------------------------------------------------------------------------------------------------
Object GetEntryAsBytes(const MMDB_entry_data_s & ed)
{
    // Obtain the initial stack size
    const StackGuard sg;
    // The result of operations
    SQRESULT res = SQ_OK;
    // The default vm
    HSQUIRRELVM vm = DefaultVM::Get();
    // Identify the type of entry data
    switch (ed.type)
    {
        case MMDB_DATA_TYPE_POINTER: {
            res = SqMod_PushBufferData(vm,
                    reinterpret_cast< const char * >(&ed.pointer), sizeof(ed.pointer), 0);
        } break;
        case MMDB_DATA_TYPE_UTF8_STRING: {
            res = SqMod_PushBufferData(vm,
                    reinterpret_cast< const char * >(ed.utf8_string), ed.data_size, 0);
        } break;
        case MMDB_DATA_TYPE_DOUBLE: {
            res = SqMod_PushBufferData(vm,
                    reinterpret_cast< const char * >(&ed.double_value), sizeof(ed.double_value), 0);
        } break;
        case MMDB_DATA_TYPE_BYTES: {
            res = SqMod_PushBufferData(vm,
                    reinterpret_cast< const char * >(ed.bytes), ed.data_size, 0);
        } break;
        case MMDB_DATA_TYPE_UINT16: {
            res = SqMod_PushBufferData(vm,
                    reinterpret_cast< const char * >(&ed.uint16), sizeof(ed.uint16), 0);
        } break;
        case MMDB_DATA_TYPE_UINT32: {
            res = SqMod_PushBufferData(vm,
                    reinterpret_cast< const char * >(&ed.uint32), sizeof(ed.uint32), 0);
        } break;
        case MMDB_DATA_TYPE_INT32: {
            res = SqMod_PushBufferData(vm,
                    reinterpret_cast< const char * >(&ed.int32), sizeof(ed.int32), 0);
        } break;
        case MMDB_DATA_TYPE_UINT64: {
            res = SqMod_PushBufferData(vm,
                    reinterpret_cast< const char * >(&ed.uint64), sizeof(ed.uint64), 0);
        } break;
        case MMDB_DATA_TYPE_UINT128: {
            res = SqMod_PushBufferData(vm,
                    reinterpret_cast< const char * >(&ed.uint128), sizeof(ed.uint128), 0);
        } break;
        case MMDB_DATA_TYPE_BOOLEAN: {
            res = SqMod_PushBufferData(vm,
                    reinterpret_cast< const char * >(&ed.boolean), sizeof(ed.boolean), 0);
        } break;
        case MMDB_DATA_TYPE_FLOAT: {
            res = SqMod_PushBufferData(vm,
                                 reinterpret_cast< const char * >(&ed.float_value),
                                 sizeof(ed.float_value), 0);
        } break;
        default:
            STHROWF("Unsupported conversion from (%s) to (buffer)", AsTypeStr(ed.type));
    }
    // Did we fail to push the buffer o the stack?
    if (SQ_FAILED(res))
    {
        STHROWF("Failed to convert the (%s) value to a buffer.", AsTypeStr(ed.type));
    }
    // Obtain the object from the stack and return it
    return Var< Object >(vm, -1).value;
}


} // Namespace:: SqMod
