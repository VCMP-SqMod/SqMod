/*
    see copyright notice in squirrel.h
*/
#include "sqpcheader.h"
#include "sqvm.h"
#include "sqstring.h"
#include "sqtable.h"
#include "sqarray.h"
#include "sqfuncproto.h"
#include "sqclosure.h"
#include "squserdata.h"
#include "sqcompiler.h"
#include "sqfuncstate.h"
#include "sqclass.h"

#include <squirrelex.h>
#include <stdarg.h>

#define _GETSAFE_OBJ(v,idx,type,o) { if(!sq_aux_gettypedarg(v,idx,type,&o)) return SQ_ERROR; }

#define sq_aux_paramscheck(v,count) \
{ \
    if(sq_gettop(v) < count){ v->Raise_Error(_SC("not enough params in the stack")); return SQ_ERROR; }\
}

static bool sq_aux_gettypedarg(HSQUIRRELVM v,SQInteger idx,SQObjectType type,SQObjectPtr **o)
{
    *o = &stack_get(v,idx);
    if(sq_type(**o) != type){
        SQObjectPtr oval = v->PrintObjVal(**o);
        v->Raise_Error(_SC("wrong argument type, expected '%s' got '%.50s'"),IdType2Name(type),_stringval(oval));
        return false;
    }
    return true;
}

SQRESULT sq_throwerrorf(HSQUIRRELVM v,const SQChar *err,...)
{
    SQInteger n=256;
    va_list args;
begin:
    va_start(args,err);
    SQChar *b=sq_getscratchpad(v,n);
    SQInteger r=scvsprintf(b,n,err,args);
    va_end(args);
    if (r>=n) {
        n=r+1;//required+null
        goto begin;
    } else if (r<0) {
        v->_lasterror=SQString::Create(_ss(v),_SC("@failed to generate formatted error message"));
    } else {
        v->_lasterror=SQString::Create(_ss(v),b,r);
    }
    return SQ_ERROR;
}

SQRESULT sq_pushstringf(HSQUIRRELVM v,const SQChar *s,...)
{
    SQInteger n=256;
    va_list args;
begin:
    va_start(args,s);
    SQChar *b=sq_getscratchpad(v,n);
    SQInteger r=scvsprintf(b,n,s,args);
    va_end(args);
    if (r>=n) {
        n=r+1;//required+null
        goto begin;
    } else if (r<0) {
        v->PushNull();
        return SQ_ERROR;
    } else {
        v->Push(SQObjectPtr(SQString::Create(_ss(v),b,r)));
        return SQ_OK;
    }
}

SQRESULT sq_vpushstringf(HSQUIRRELVM v,const SQChar *s,va_list l)
{
    SQInteger n=256;
    va_list args;
begin:
    va_copy(args,l);
    SQChar *b=sq_getscratchpad(v,n);
    SQInteger r=scvsprintf(b,n,s,args);
    va_end(args);
    if (r>=n) {
        n=r+1;//required+null
        goto begin;
    } else if (r<0) {
        v->PushNull();
        return SQ_ERROR;
    } else {
        v->Push(SQObjectPtr(SQString::Create(_ss(v),b,r)));
        return SQ_OK;
    }
}

SQRESULT sq_getnativeclosurepointer(HSQUIRRELVM v,SQInteger idx,SQFUNCTION *f)
{
    SQObject o = stack_get(v, idx);
    if(sq_type(o) == OT_NATIVECLOSURE)
    {
        SQNativeClosure *c = _nativeclosure(o);
        if (f) *f = c->_function;
        return SQ_OK;
    }
    return sq_throwerror(v,_SC("the object is not a native closure"));
}

SQRESULT sq_arrayreserve(HSQUIRRELVM v,SQInteger idx,SQInteger newcap)
{
    sq_aux_paramscheck(v,1);
    SQObjectPtr *arr;
    _GETSAFE_OBJ(v, idx, OT_ARRAY,arr);
    if(newcap >= 0) {
        _array(*arr)->Reserve(newcap);
        return SQ_OK;
    }
    return sq_throwerror(v,_SC("negative capacity"));
}
