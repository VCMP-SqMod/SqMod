/*  see copyright notice in squirrel.h */
#ifndef _SQSTD_AUXLIB_H_
#define _SQSTD_AUXLIB_H_

#ifdef __cplusplus
extern "C" {
#endif

SQUIRREL_API void sqstd_dumptypename(HSQUIRRELVM v, SQInteger idx, const SQChar *name, SQPRINTFUNCTION pf);
SQUIRREL_API void sqstd_dumpstack(HSQUIRRELVM v, SQInteger bottom, SQInteger top, SQPRINTFUNCTION pf);

SQUIRREL_API void sqstd_seterrorhandlers(HSQUIRRELVM v);
SQUIRREL_API void sqstd_printcallstack(HSQUIRRELVM v);

SQUIRREL_API SQRESULT sqstd_throwerrorf(HSQUIRRELVM v,const SQChar *err,...);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /* _SQSTD_AUXLIB_H_ */
