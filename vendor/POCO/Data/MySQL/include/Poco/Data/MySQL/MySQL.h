//
// MySQL.h
//
// Library: Data/MySQL
// Package: MySQL
// Module:  MySQL
//
// Basic definitions for the MySQL library.
//
// Copyright (c) 2008, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef MySQL_MySQL_INCLUDED
#define MySQL_MySQL_INCLUDED

#include "Poco/Foundation.h"
#if defined(__MINGW32__) || defined(__MINGW64__)
	#include <mysql/mysql.h>
#else
	#include <mysql.h>
#endif

//
// The following block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the ODBC_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// ODBC_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
//
#if defined(_WIN32) && defined(POCO_DLL)
	#if defined(MySQL_EXPORTS)
		#define MySQL_API __declspec(dllexport)
	#else
		#define MySQL_API __declspec(dllimport)
	#endif
#endif


#if !defined(MySQL_API)
	#if !defined(POCO_NO_GCC_API_ATTRIBUTE) && defined (__GNUC__) && (__GNUC__ >= 4)
		#define MySQL_API __attribute__ ((visibility ("default")))
	#else
		#define MySQL_API
	#endif
#endif


//
// Automatically link Data library.
//
#if defined(_MSC_VER) && !defined(POCO_NO_AUTOMATIC_LIBS)
	#if !defined(MySQL_EXPORTS)
		#pragma comment(lib, "PocoDataMySQL" POCO_LIB_SUFFIX)
	#endif
	#if defined(LIBMARIADB)
		#pragma comment(lib, "libmariadb")
	#else
		#pragma comment(lib, "libmysql")
	#endif
#endif

//
// Detect support for JSON data type
//
#if defined(MARIADB_VERSION_ID)
	#if MARIADB_VERSION_ID >= 100207
		#define POCO_MYSQL_JSON
	#endif
#else
	#if MYSQL_VERSION_ID >= 50708
		#define POCO_MYSQL_JSON
	#endif
#endif

#endif // MySQL_MySQL_INCLUDED
