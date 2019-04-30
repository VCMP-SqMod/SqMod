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

void sq_pushstringf(HSQUIRRELVM v,const SQChar *s,...)
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
    } else {
        v->Push(SQObjectPtr(SQString::Create(_ss(v),b,r)));
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
