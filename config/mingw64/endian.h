#ifndef _ENDIAN_H_
#define _ENDIAN_H_

#if defined(__MINGW32__) || defined(__MINGW64__)
    // Workaround for MinGW and it's lack of <endian.h> file
    #define __BYTE_ORDER    __BYTE_ORDER__
    #define __LITTLE_ENDIAN __ORDER_LITTLE_ENDIAN__
    #define __BIG_ENDIAN    __ORDER_BIG_ENDIAN__
#elif !defined(_MSC_VER)
    // Just include the system file
    #include <endian.h>
#endif

#endif // _ENDIAN_H_