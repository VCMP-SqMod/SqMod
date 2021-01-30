#pragma once

// ------------------------------------------------------------------------------------------------
#include "SqBase.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Attempt to generate a moderately unique number to be used as a seed for random numbers.
*/
uint32_t GenerateSeed();

// ------------------------------------------------------------------------------------------------
void ReseedRandom();
void ReseedRandom(uint32_t n);

// ------------------------------------------------------------------------------------------------
SQMOD_NODISCARD int8_t GetRandomInt8();
SQMOD_NODISCARD int8_t GetRandomInt8(int8_t n);
SQMOD_NODISCARD int8_t GetRandomInt8(int8_t m, int8_t n);

// ------------------------------------------------------------------------------------------------
SQMOD_NODISCARD uint8_t GetRandomUint8();
SQMOD_NODISCARD uint8_t GetRandomUint8(uint8_t n);
SQMOD_NODISCARD uint8_t GetRandomUint8(uint8_t m, uint8_t n);

// ------------------------------------------------------------------------------------------------
SQMOD_NODISCARD int16_t GetRandomInt16();
SQMOD_NODISCARD int16_t GetRandomInt16(int16_t n);
SQMOD_NODISCARD int16_t GetRandomInt16(int16_t m, int16_t n);

// ------------------------------------------------------------------------------------------------
SQMOD_NODISCARD uint16_t GetRandomUint16();
SQMOD_NODISCARD uint16_t GetRandomUint16(uint16_t n);
SQMOD_NODISCARD uint16_t GetRandomUint16(uint16_t m, uint16_t n);

// ------------------------------------------------------------------------------------------------
SQMOD_NODISCARD int32_t GetRandomInt32();
SQMOD_NODISCARD int32_t GetRandomInt32(int32_t n);
SQMOD_NODISCARD int32_t GetRandomInt32(int32_t m, int32_t n);

// ------------------------------------------------------------------------------------------------
SQMOD_NODISCARD uint32_t GetRandomUint32();
SQMOD_NODISCARD uint32_t GetRandomUint32(uint32_t n);
SQMOD_NODISCARD uint32_t GetRandomUint32(uint32_t m, uint32_t n);

// ------------------------------------------------------------------------------------------------
SQMOD_NODISCARD int64_t GetRandomInt64();
SQMOD_NODISCARD int64_t GetRandomInt64(int64_t n);
SQMOD_NODISCARD int64_t GetRandomInt64(int64_t m, int64_t n);

// ------------------------------------------------------------------------------------------------
SQMOD_NODISCARD uint64_t GetRandomUint64();
SQMOD_NODISCARD uint64_t GetRandomUint64(uint64_t n);
SQMOD_NODISCARD uint64_t GetRandomUint64(uint64_t m, uint64_t n);

// ------------------------------------------------------------------------------------------------
SQMOD_NODISCARD float GetRandomFloat32();
SQMOD_NODISCARD float GetRandomFloat32(float n);
SQMOD_NODISCARD float GetRandomFloat32(float m, float n);

// ------------------------------------------------------------------------------------------------
SQMOD_NODISCARD double GetRandomFloat64();
SQMOD_NODISCARD double GetRandomFloat64(double n);
SQMOD_NODISCARD double GetRandomFloat64(double m, double n);

// ------------------------------------------------------------------------------------------------
void GetRandomString(String & str, String::size_type len);
void GetRandomString(String & str, String::size_type len, String::value_type n);
void GetRandomString(String & str, String::size_type len, String::value_type m, String::value_type n);

// ------------------------------------------------------------------------------------------------
SQMOD_NODISCARD bool GetRandomBool();

// ------------------------------------------------------------------------------------------------
template <typename T> struct RandomVal
{ /* ... */ };

// ------------------------------------------------------------------------------------------------
template <> struct RandomVal< int8_t >
{
    static inline int8_t Get() { return GetRandomInt8(); }
    static inline int8_t Get(int8_t n) { return GetRandomInt8(n); }
    static inline int8_t Get(int8_t m, int8_t n) { return GetRandomInt8(m, n); }
};

template <> struct RandomVal< uint8_t >
{
    static inline uint8_t Get() { return GetRandomUint8(); }
    static inline uint8_t Get(uint8_t n) { return GetRandomUint8(n); }
    static inline uint8_t Get(uint8_t m, uint8_t n) { return GetRandomUint8(m, n); }
};

// ------------------------------------------------------------------------------------------------
template <> struct RandomVal< int16_t >
{
    static inline int16_t Get() { return GetRandomInt16(); }
    static inline int16_t Get(int16_t n) { return GetRandomInt16(n); }
    static inline int16_t Get(int16_t m, int16_t n) { return GetRandomInt16(m, n); }
};

template <> struct RandomVal< uint16_t >
{
    static inline uint16_t Get() { return GetRandomUint16(); }
    static inline uint16_t Get(uint16_t n) { return GetRandomUint16(n); }
    static inline uint16_t Get(uint16_t m, uint16_t n) { return GetRandomUint16(m, n); }
};

// ------------------------------------------------------------------------------------------------
template <> struct RandomVal< int32_t >
{
    static inline int32_t Get() { return GetRandomInt32(); }
    static inline int32_t Get(int32_t n) { return GetRandomInt32(n); }
    static inline int32_t Get(int32_t m, int32_t n) { return GetRandomInt32(m, n); }
};

template <> struct RandomVal< uint32_t >
{
    static inline uint32_t Get() { return GetRandomUint32(); }
    static inline uint32_t Get(uint32_t n) { return GetRandomUint32(n); }
    static inline uint32_t Get(uint32_t m, uint32_t n) { return GetRandomUint32(m, n); }
};

// ------------------------------------------------------------------------------------------------
template <> struct RandomVal< int64_t >
{
    static inline int64_t Get() { return GetRandomInt64(); }
    static inline int64_t Get(int64_t n) { return GetRandomInt64(n); }
    static inline int64_t Get(int64_t m, int64_t n) { return GetRandomInt64(m, n); }
};

template <> struct RandomVal< uint64_t >
{
    static inline uint64_t Get() { return GetRandomUint64(); }
    static inline uint64_t Get(uint64_t n) { return GetRandomUint64(n); }
    static inline uint64_t Get(uint64_t m, uint64_t n) { return GetRandomUint64(m, n); }
};

// ------------------------------------------------------------------------------------------------
template <> struct RandomVal< float >
{
    static inline float Get() { return GetRandomFloat32(); }
    static inline float Get(float n) { return GetRandomFloat32(n); }
    static inline float Get(float m, float n) { return GetRandomFloat32(m, n); }
};

template <> struct RandomVal< double >
{
    static inline double Get() { return GetRandomFloat64(); }
    static inline double Get(double n) { return GetRandomFloat64(n); }
    static inline double Get(double m, double n) { return GetRandomFloat64(m, n); }
};

// ------------------------------------------------------------------------------------------------
template <> struct RandomVal< bool >
{
    static inline bool Get() { return GetRandomBool(); }
};

} // Namespace:: SqMod
