#pragma once

// ------------------------------------------------------------------------------------------------
#include "SqBase.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Attempt to generate a moderately unique number to be used as a seed for random numbers.
*/
Uint32 GenerateSeed();

// ------------------------------------------------------------------------------------------------
void ReseedRandom();
void ReseedRandom(Uint32 n);

// ------------------------------------------------------------------------------------------------
Int8 GetRandomInt8();
Int8 GetRandomInt8(Int8 n);
Int8 GetRandomInt8(Int8 m, Int8 n);

// ------------------------------------------------------------------------------------------------
Uint8 GetRandomUint8();
Uint8 GetRandomUint8(Uint8 n);
Uint8 GetRandomUint8(Uint8 m, Uint8 n);

// ------------------------------------------------------------------------------------------------
Int16 GetRandomInt16();
Int16 GetRandomInt16(Int16 n);
Int16 GetRandomInt16(Int16 m, Int16 n);

// ------------------------------------------------------------------------------------------------
Uint16 GetRandomUint16();
Uint16 GetRandomUint16(Uint16 n);
Uint16 GetRandomUint16(Uint16 m, Uint16 n);

// ------------------------------------------------------------------------------------------------
Int32 GetRandomInt32();
Int32 GetRandomInt32(Int32 n);
Int32 GetRandomInt32(Int32 m, Int32 n);

// ------------------------------------------------------------------------------------------------
Uint32 GetRandomUint32();
Uint32 GetRandomUint32(Uint32 n);
Uint32 GetRandomUint32(Uint32 m, Uint32 n);

// ------------------------------------------------------------------------------------------------
Int64 GetRandomInt64();
Int64 GetRandomInt64(Int64 n);
Int64 GetRandomInt64(Int64 m, Int64 n);

// ------------------------------------------------------------------------------------------------
Uint64 GetRandomUint64();
Uint64 GetRandomUint64(Uint64 n);
Uint64 GetRandomUint64(Uint64 m, Uint64 n);

// ------------------------------------------------------------------------------------------------
Float32 GetRandomFloat32();
Float32 GetRandomFloat32(Float32 n);
Float32 GetRandomFloat32(Float32 m, Float32 n);

// ------------------------------------------------------------------------------------------------
Float64 GetRandomFloat64();
Float64 GetRandomFloat64(Float64 n);
Float64 GetRandomFloat64(Float64 m, Float64 n);

// ------------------------------------------------------------------------------------------------
void GetRandomString(String & str, String::size_type len);
void GetRandomString(String & str, String::size_type len, String::value_type n);
void GetRandomString(String & str, String::size_type len, String::value_type m, String::value_type n);

// ------------------------------------------------------------------------------------------------
bool GetRandomBool();

// ------------------------------------------------------------------------------------------------
template <typename T> struct RandomVal
{ /* ... */ };

// ------------------------------------------------------------------------------------------------
template <> struct RandomVal< Int8 >
{
    static inline Int8 Get() { return GetRandomInt8(); }
    static inline Int8 Get(Int8 n) { return GetRandomInt8(n); }
    static inline Int8 Get(Int8 m, Int8 n) { return GetRandomInt8(m, n); }
};

template <> struct RandomVal< Uint8 >
{
    static inline Uint8 Get() { return GetRandomUint8(); }
    static inline Uint8 Get(Uint8 n) { return GetRandomUint8(n); }
    static inline Uint8 Get(Uint8 m, Uint8 n) { return GetRandomUint8(m, n); }
};

// ------------------------------------------------------------------------------------------------
template <> struct RandomVal< Int16 >
{
    static inline Int16 Get() { return GetRandomInt16(); }
    static inline Int16 Get(Int16 n) { return GetRandomInt16(n); }
    static inline Int16 Get(Int16 m, Int16 n) { return GetRandomInt16(m, n); }
};

template <> struct RandomVal< Uint16 >
{
    static inline Uint16 Get() { return GetRandomUint16(); }
    static inline Uint16 Get(Uint16 n) { return GetRandomUint16(n); }
    static inline Uint16 Get(Uint16 m, Uint16 n) { return GetRandomUint16(m, n); }
};

// ------------------------------------------------------------------------------------------------
template <> struct RandomVal< Int32 >
{
    static inline Int32 Get() { return GetRandomInt32(); }
    static inline Int32 Get(Int32 n) { return GetRandomInt32(n); }
    static inline Int32 Get(Int32 m, Int32 n) { return GetRandomInt32(m, n); }
};

template <> struct RandomVal< Uint32 >
{
    static inline Uint32 Get() { return GetRandomUint32(); }
    static inline Uint32 Get(Uint32 n) { return GetRandomUint32(n); }
    static inline Uint32 Get(Uint32 m, Uint32 n) { return GetRandomUint32(m, n); }
};

// ------------------------------------------------------------------------------------------------
template <> struct RandomVal< Int64 >
{
    static inline Int64 Get() { return GetRandomInt64(); }
    static inline Int64 Get(Int64 n) { return GetRandomInt64(n); }
    static inline Int64 Get(Int64 m, Int64 n) { return GetRandomInt64(m, n); }
};

template <> struct RandomVal< Uint64 >
{
    static inline Uint64 Get() { return GetRandomUint64(); }
    static inline Uint64 Get(Uint64 n) { return GetRandomUint64(n); }
    static inline Uint64 Get(Uint64 m, Uint64 n) { return GetRandomUint64(m, n); }
};

// ------------------------------------------------------------------------------------------------
template <> struct RandomVal< Float32 >
{
    static inline Float32 Get() { return GetRandomFloat32(); }
    static inline Float32 Get(Float32 n) { return GetRandomFloat32(n); }
    static inline Float32 Get(Float32 m, Float32 n) { return GetRandomFloat32(m, n); }
};

template <> struct RandomVal< Float64 >
{
    static inline Float64 Get() { return GetRandomFloat64(); }
    static inline Float64 Get(Float64 n) { return GetRandomFloat64(n); }
    static inline Float64 Get(Float64 m, Float64 n) { return GetRandomFloat64(m, n); }
};

// ------------------------------------------------------------------------------------------------
template <> struct RandomVal< bool >
{
    static inline bool Get() { return GetRandomBool(); }
};

} // Namespace:: SqMod
