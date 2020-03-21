/*
    see copyright notice in squirrel.h
*/
#ifndef _SQUIRRELEX_H_
#define _SQUIRRELEX_H_

#include <squirrel.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef SQMOD_PLUGIN_API

SQUIRREL_API SQRESULT sq_throwerrorf(HSQUIRRELVM v,const SQChar *err,...);
SQUIRREL_API void sq_pushstringf(HSQUIRRELVM v,const SQChar *s,...);
SQUIRREL_API SQRESULT sq_getnativeclosurepointer(HSQUIRRELVM v,SQInteger idx,SQFUNCTION *f);

#endif // SQMOD_PLUGIN_API

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*_SQUIRRELEX_H_*/
