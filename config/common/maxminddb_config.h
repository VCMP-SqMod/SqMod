#ifndef MAXMINDDB_CONFIG_H
#define MAXMINDDB_CONFIG_H

#ifndef MMDB_UINT128_USING_MODE
/* Define as 1 if we we use unsigned int __atribute__ ((__mode__(TI))) for uint128 values */
#if __x86_64__
	#define MMDB_UINT128_USING_MODE 1
#else
	#define MMDB_UINT128_USING_MODE 0
#endif

#endif

#ifndef MMDB_UINT128_IS_BYTE_ARRAY
/* Define as 1 if we don't have an unsigned __int128 type */\
#if __x86_64__
	#undef MMDB_UINT128_IS_BYTE_ARRAY
#else
	#define MMDB_UINT128_IS_BYTE_ARRAY 1
#endif

#endif

/* Name of package */
#define PACKAGE "libmaxminddb"

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT "support@maxmind.com"

/* Define to the full name of this package. */
#define PACKAGE_NAME "libmaxminddb"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "libmaxminddb 1.4.2"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "libmaxminddb"

/* Define to the home page for this package. */
#define PACKAGE_URL ""

/* Define to the version of this package. */
#define PACKAGE_VERSION "1.4.2"

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Version number of package */
#define VERSION "1.4.2"

#endif                          /* MAXMINDDB_CONFIG_H */
