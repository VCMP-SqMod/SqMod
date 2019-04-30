//
// SqModule: API used to communicate with and register squirrel modules
//

//
// Copyright (c) 2016 Sandu Liviu Catalin (aka. S.L.C)
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
//  1. The origin of this software must not be misrepresented; you must not
//  claim that you wrote the original software. If you use this software
//  in a product, an acknowledgment in the product documentation would be
//  appreciated but is not required.
//
//  2. Altered source versions must be plainly marked as such, and must not be
//  misrepresented as being the original software.
//
//  3. This notice may not be removed or altered from any source
//  distribution.
//

#if !defined(_SQ_API_H_)
#define _SQ_API_H_

#include "squirrel.h"

#ifdef __cplusplus
extern "C" {
#endif

    //vm
    typedef HSQUIRRELVM (*SqLibAPI_open)(SQInteger initialstacksize);
    typedef HSQUIRRELVM (*SqLibAPI_newthread)(HSQUIRRELVM friendvm, SQInteger initialstacksize);
    typedef void (*SqLibAPI_seterrorhandler)(HSQUIRRELVM v);
    typedef void (*SqLibAPI_close)(HSQUIRRELVM v);
    typedef void (*SqLibAPI_setforeignptr)(HSQUIRRELVM v,SQUserPointer p);
    typedef SQUserPointer (*SqLibAPI_getforeignptr)(HSQUIRRELVM v);
    typedef void (*SqLibAPI_setsharedforeignptr)(HSQUIRRELVM v,SQUserPointer p);
    typedef SQUserPointer (*SqLibAPI_getsharedforeignptr)(HSQUIRRELVM v);
    typedef void (*SqLibAPI_setvmreleasehook)(HSQUIRRELVM v,SQRELEASEHOOK hook);
    typedef SQRELEASEHOOK (*SqLibAPI_getvmreleasehook)(HSQUIRRELVM v);
    typedef void (*SqLibAPI_setsharedreleasehook)(HSQUIRRELVM v,SQRELEASEHOOK hook);
    typedef SQRELEASEHOOK (*SqLibAPI_getsharedreleasehook)(HSQUIRRELVM v);
    typedef void (*SqLibAPI_setprintfunc)(HSQUIRRELVM v, SQPRINTFUNCTION printfunc,SQPRINTFUNCTION errfunc);
    typedef SQPRINTFUNCTION (*SqLibAPI_getprintfunc)(HSQUIRRELVM v);
    typedef SQPRINTFUNCTION (*SqLibAPI_geterrorfunc)(HSQUIRRELVM v);
    typedef SQRESULT (*SqLibAPI_suspendvm)(HSQUIRRELVM v);
    typedef SQRESULT (*SqLibAPI_wakeupvm)(HSQUIRRELVM v,SQBool resumedret,SQBool retval,SQBool raiseerror,SQBool throwerror);
    typedef SQInteger (*SqLibAPI_getvmstate)(HSQUIRRELVM v);
    typedef SQInteger (*SqLibAPI_getversion)();

    //compiler
    typedef SQRESULT (*SqLibAPI_compile)(HSQUIRRELVM v,SQLEXREADFUNC read,SQUserPointer p,const SQChar *sourcename,SQBool raiseerror);
    typedef SQRESULT (*SqLibAPI_compilebuffer)(HSQUIRRELVM v,const SQChar *s,SQInteger size,const SQChar *sourcename,SQBool raiseerror);
    typedef void (*SqLibAPI_enabledebuginfo)(HSQUIRRELVM v, SQBool enable);
    typedef void (*SqLibAPI_notifyallexceptions)(HSQUIRRELVM v, SQBool enable);
    typedef void (*SqLibAPI_setcompilererrorhandler)(HSQUIRRELVM v,SQCOMPILERERROR f);

    //stack operations
    typedef void (*SqLibAPI_push)(HSQUIRRELVM v,SQInteger idx);
    typedef void (*SqLibAPI_pop)(HSQUIRRELVM v,SQInteger nelemstopop);
    typedef void (*SqLibAPI_poptop)(HSQUIRRELVM v);
    typedef void (*SqLibAPI_remove)(HSQUIRRELVM v,SQInteger idx);
    typedef SQInteger (*SqLibAPI_gettop)(HSQUIRRELVM v);
    typedef void (*SqLibAPI_settop)(HSQUIRRELVM v,SQInteger newtop);
    typedef SQRESULT (*SqLibAPI_reservestack)(HSQUIRRELVM v,SQInteger nsize);
    typedef SQInteger (*SqLibAPI_cmp)(HSQUIRRELVM v);
    typedef void (*SqLibAPI_move)(HSQUIRRELVM dest,HSQUIRRELVM src,SQInteger idx);

    //object creation handling
    typedef SQUserPointer (*SqLibAPI_newuserdata)(HSQUIRRELVM v,SQUnsignedInteger size);
    typedef void (*SqLibAPI_newtable)(HSQUIRRELVM v);
    typedef void (*SqLibAPI_newtableex)(HSQUIRRELVM v,SQInteger initialcapacity);
    typedef void (*SqLibAPI_newarray)(HSQUIRRELVM v,SQInteger size);
    typedef void (*SqLibAPI_newclosure)(HSQUIRRELVM v,SQFUNCTION func,SQUnsignedInteger nfreevars);
    typedef SQRESULT (*SqLibAPI_setparamscheck)(HSQUIRRELVM v,SQInteger nparamscheck,const SQChar *typemask);
    typedef SQRESULT (*SqLibAPI_bindenv)(HSQUIRRELVM v,SQInteger idx);
    typedef SQRESULT (*SqLibAPI_setclosureroot)(HSQUIRRELVM v,SQInteger idx);
    typedef SQRESULT (*SqLibAPI_getclosureroot)(HSQUIRRELVM v,SQInteger idx);
    typedef void (*SqLibAPI_pushstring)(HSQUIRRELVM v,const SQChar *s,SQInteger len);
    typedef void (*SqLibAPI_pushstringf)(HSQUIRRELVM v,const SQChar *s,...);
    typedef void (*SqLibAPI_pushfloat)(HSQUIRRELVM v,SQFloat f);
    typedef void (*SqLibAPI_pushinteger)(HSQUIRRELVM v,SQInteger n);
    typedef void (*SqLibAPI_pushbool)(HSQUIRRELVM v,SQBool b);
    typedef void (*SqLibAPI_pushuserpointer)(HSQUIRRELVM v,SQUserPointer p);
    typedef void (*SqLibAPI_pushnull)(HSQUIRRELVM v);
    typedef void (*SqLibAPI_pushthread)(HSQUIRRELVM v, HSQUIRRELVM thread);
    typedef SQObjectType (*SqLibAPI_gettype)(HSQUIRRELVM v,SQInteger idx);
    typedef SQRESULT (*SqLibAPI_typeof)(HSQUIRRELVM v,SQInteger idx);
    typedef SQInteger (*SqLibAPI_getsize)(HSQUIRRELVM v,SQInteger idx);
    typedef SQHash (*SqLibAPI_gethash)(HSQUIRRELVM v, SQInteger idx);
    typedef SQRESULT (*SqLibAPI_getbase)(HSQUIRRELVM v,SQInteger idx);
    typedef SQBool (*SqLibAPI_instanceof)(HSQUIRRELVM v);
    typedef SQRESULT (*SqLibAPI_tostring)(HSQUIRRELVM v,SQInteger idx);
    typedef void (*SqLibAPI_tobool)(HSQUIRRELVM v, SQInteger idx, SQBool *b);
    typedef SQRESULT (*SqLibAPI_getstringandsize)(HSQUIRRELVM v,SQInteger idx,const SQChar **c,SQInteger *size);
    typedef SQRESULT (*SqLibAPI_getstring)(HSQUIRRELVM v,SQInteger idx,const SQChar **c);
    typedef SQRESULT (*SqLibAPI_getinteger)(HSQUIRRELVM v,SQInteger idx,SQInteger *i);
    typedef SQRESULT (*SqLibAPI_getfloat)(HSQUIRRELVM v,SQInteger idx,SQFloat *f);
    typedef SQRESULT (*SqLibAPI_getbool)(HSQUIRRELVM v,SQInteger idx,SQBool *b);
    typedef SQRESULT (*SqLibAPI_getthread)(HSQUIRRELVM v,SQInteger idx,HSQUIRRELVM *thread);
    typedef SQRESULT (*SqLibAPI_getuserpointer)(HSQUIRRELVM v,SQInteger idx,SQUserPointer *p);
    typedef SQRESULT (*SqLibAPI_getuserdata)(HSQUIRRELVM v,SQInteger idx,SQUserPointer *p,SQUserPointer *typetag);
    typedef SQRESULT (*SqLibAPI_settypetag)(HSQUIRRELVM v,SQInteger idx,SQUserPointer typetag);
    typedef SQRESULT (*SqLibAPI_gettypetag)(HSQUIRRELVM v,SQInteger idx,SQUserPointer *typetag);
    typedef void (*SqLibAPI_setreleasehook)(HSQUIRRELVM v,SQInteger idx,SQRELEASEHOOK hook);
    typedef SQRELEASEHOOK (*SqLibAPI_getreleasehook)(HSQUIRRELVM v,SQInteger idx);
    typedef SQChar* (*SqLibAPI_getscratchpad)(HSQUIRRELVM v,SQInteger minsize);
    typedef SQRESULT (*SqLibAPI_getfunctioninfo)(HSQUIRRELVM v,SQInteger level,SQFunctionInfo *fi);
    typedef SQRESULT (*SqLibAPI_getclosureinfo)(HSQUIRRELVM v,SQInteger idx,SQInteger *nparams,SQInteger *nfreevars);
    typedef SQRESULT (*SqLibAPI_getclosurename)(HSQUIRRELVM v,SQInteger idx);
    typedef SQRESULT (*SqLibAPI_setnativeclosurename)(HSQUIRRELVM v,SQInteger idx,const SQChar *name);
    typedef SQRESULT (*SqLibAPI_getnativeclosurepointer)(HSQUIRRELVM v,SQInteger idx,SQFUNCTION *f);
    typedef SQRESULT (*SqLibAPI_setinstanceup)(HSQUIRRELVM v, SQInteger idx, SQUserPointer p);
    typedef SQRESULT (*SqLibAPI_getinstanceup)(HSQUIRRELVM v, SQInteger idx, SQUserPointer *p,SQUserPointer typetag);
    typedef SQRESULT (*SqLibAPI_setclassudsize)(HSQUIRRELVM v, SQInteger idx, SQInteger udsize);
    typedef SQRESULT (*SqLibAPI_newclass)(HSQUIRRELVM v,SQBool hasbase);
    typedef SQRESULT (*SqLibAPI_createinstance)(HSQUIRRELVM v,SQInteger idx);
    typedef SQRESULT (*SqLibAPI_setattributes)(HSQUIRRELVM v,SQInteger idx);
    typedef SQRESULT (*SqLibAPI_getattributes)(HSQUIRRELVM v,SQInteger idx);
    typedef SQRESULT (*SqLibAPI_getclass)(HSQUIRRELVM v,SQInteger idx);
    typedef void (*SqLibAPI_weakref)(HSQUIRRELVM v,SQInteger idx);
    typedef SQRESULT (*SqLibAPI_getdefaultdelegate)(HSQUIRRELVM v,SQObjectType t);
    typedef SQRESULT (*SqLibAPI_getmemberhandle)(HSQUIRRELVM v,SQInteger idx,HSQMEMBERHANDLE *handle);
    typedef SQRESULT (*SqLibAPI_getbyhandle)(HSQUIRRELVM v,SQInteger idx,const HSQMEMBERHANDLE *handle);
    typedef SQRESULT (*SqLibAPI_setbyhandle)(HSQUIRRELVM v,SQInteger idx,const HSQMEMBERHANDLE *handle);

    //object manipulation
    typedef void (*SqLibAPI_pushroottable)(HSQUIRRELVM v);
    typedef void (*SqLibAPI_pushregistrytable)(HSQUIRRELVM v);
    typedef void (*SqLibAPI_pushconsttable)(HSQUIRRELVM v);
    typedef SQRESULT (*SqLibAPI_setroottable)(HSQUIRRELVM v);
    typedef SQRESULT (*SqLibAPI_setconsttable)(HSQUIRRELVM v);
    typedef SQRESULT (*SqLibAPI_newslot)(HSQUIRRELVM v, SQInteger idx, SQBool bstatic);
    typedef SQRESULT (*SqLibAPI_deleteslot)(HSQUIRRELVM v,SQInteger idx,SQBool pushval);
    typedef SQRESULT (*SqLibAPI_set)(HSQUIRRELVM v,SQInteger idx);
    typedef SQRESULT (*SqLibAPI_get)(HSQUIRRELVM v,SQInteger idx);
    typedef SQRESULT (*SqLibAPI_rawget)(HSQUIRRELVM v,SQInteger idx);
    typedef SQRESULT (*SqLibAPI_rawset)(HSQUIRRELVM v,SQInteger idx);
    typedef SQRESULT (*SqLibAPI_rawdeleteslot)(HSQUIRRELVM v,SQInteger idx,SQBool pushval);
    typedef SQRESULT (*SqLibAPI_newmember)(HSQUIRRELVM v,SQInteger idx,SQBool bstatic);
    typedef SQRESULT (*SqLibAPI_rawnewmember)(HSQUIRRELVM v,SQInteger idx,SQBool bstatic);
    typedef SQRESULT (*SqLibAPI_arrayappend)(HSQUIRRELVM v,SQInteger idx);
    typedef SQRESULT (*SqLibAPI_arraypop)(HSQUIRRELVM v,SQInteger idx,SQBool pushval);
    typedef SQRESULT (*SqLibAPI_arrayresize)(HSQUIRRELVM v,SQInteger idx,SQInteger newsize);
    typedef SQRESULT (*SqLibAPI_arrayreverse)(HSQUIRRELVM v,SQInteger idx);
    typedef SQRESULT (*SqLibAPI_arrayremove)(HSQUIRRELVM v,SQInteger idx,SQInteger itemidx);
    typedef SQRESULT (*SqLibAPI_arrayinsert)(HSQUIRRELVM v,SQInteger idx,SQInteger destpos);
    typedef SQRESULT (*SqLibAPI_setdelegate)(HSQUIRRELVM v,SQInteger idx);
    typedef SQRESULT (*SqLibAPI_getdelegate)(HSQUIRRELVM v,SQInteger idx);
    typedef SQRESULT (*SqLibAPI_clone)(HSQUIRRELVM v,SQInteger idx);
    typedef SQRESULT (*SqLibAPI_setfreevariable)(HSQUIRRELVM v,SQInteger idx,SQUnsignedInteger nval);
    typedef SQRESULT (*SqLibAPI_next)(HSQUIRRELVM v,SQInteger idx);
    typedef SQRESULT (*SqLibAPI_getweakrefval)(HSQUIRRELVM v,SQInteger idx);
    typedef SQRESULT (*SqLibAPI_clear)(HSQUIRRELVM v,SQInteger idx);

    //calls
    typedef SQRESULT (*SqLibAPI_call)(HSQUIRRELVM v,SQInteger params,SQBool retval,SQBool raiseerror);
    typedef SQRESULT (*SqLibAPI_resume)(HSQUIRRELVM v,SQBool retval,SQBool raiseerror);
    typedef const SQChar* (*SqLibAPI_getlocal)(HSQUIRRELVM v,SQUnsignedInteger level,SQUnsignedInteger idx);
    typedef SQRESULT (*SqLibAPI_getcallee)(HSQUIRRELVM v);
    typedef const SQChar* (*SqLibAPI_getfreevariable)(HSQUIRRELVM v,SQInteger idx,SQUnsignedInteger nval);
    typedef const SQChar* (*SqLibAPI_getonefreevariable)(HSQUIRRELVM v,SQUnsignedInteger nval);
    typedef SQRESULT (*SqLibAPI_throwerror)(HSQUIRRELVM v,const SQChar *err);
    typedef SQRESULT (*SqLibAPI_throwerrorf)(HSQUIRRELVM v,const SQChar *err,...);
    typedef SQRESULT (*SqLibAPI_throwobject)(HSQUIRRELVM v);
    typedef void (*SqLibAPI_reseterror)(HSQUIRRELVM v);
    typedef void (*SqLibAPI_getlasterror)(HSQUIRRELVM v);

    //raw object handling
    typedef SQRESULT (*SqLibAPI_getstackobj)(HSQUIRRELVM v,SQInteger idx,HSQOBJECT *po);
    typedef void (*SqLibAPI_pushobject)(HSQUIRRELVM v,HSQOBJECT obj);
    typedef void (*SqLibAPI_addref)(HSQUIRRELVM v,HSQOBJECT *po);
    typedef SQBool (*SqLibAPI_release)(HSQUIRRELVM v,HSQOBJECT *po);
    typedef SQUnsignedInteger (*SqLibAPI_getrefcount)(HSQUIRRELVM v,HSQOBJECT *po);
    typedef void (*SqLibAPI_resetobject)(HSQOBJECT *po);
    typedef const SQChar* (*SqLibAPI_objtostring)(const HSQOBJECT *o);
    typedef SQBool (*SqLibAPI_objtobool)(const HSQOBJECT *o);
    typedef SQInteger (*SqLibAPI_objtointeger)(const HSQOBJECT *o);
    typedef SQFloat (*SqLibAPI_objtofloat)(const HSQOBJECT *o);
    typedef SQUserPointer (*SqLibAPI_objtouserpointer)(const HSQOBJECT *o);
    typedef SQRESULT (*SqLibAPI_getobjtypetag)(const HSQOBJECT *o,SQUserPointer * typetag);
    typedef SQUnsignedInteger (*SqLibAPI_getvmrefcount)(HSQUIRRELVM v, const HSQOBJECT *po);


    //GC
    typedef SQInteger (*SqLibAPI_collectgarbage)(HSQUIRRELVM v);
    typedef SQRESULT (*SqLibAPI_resurrectunreachable)(HSQUIRRELVM v);

    //serialization
    typedef SQRESULT (*SqLibAPI_writeclosure)(HSQUIRRELVM vm,SQWRITEFUNC writef,SQUserPointer up);
    typedef SQRESULT (*SqLibAPI_readclosure)(HSQUIRRELVM vm,SQREADFUNC readf,SQUserPointer up);

    //mem allocation
    typedef void* (*SqLibAPI_malloc)(SQUnsignedInteger size);
    typedef void* (*SqLibAPI_realloc)(void* p,SQUnsignedInteger oldsize,SQUnsignedInteger newsize);
    typedef void (*SqLibAPI_free)(void *p,SQUnsignedInteger size);

    //debug
    typedef SQRESULT (*SqLibAPI_stackinfos)(HSQUIRRELVM v,SQInteger level,SQStackInfos *si);
    typedef void (*SqLibAPI_setdebughook)(HSQUIRRELVM v);
    typedef void (*SqLibAPI_setnativedebughook)(HSQUIRRELVM v,SQDEBUGHOOK hook);

    //compiler helpers
    typedef SQRESULT (*SqLibAPI_loadfile)(HSQUIRRELVM v,const SQChar *filename,SQBool printerror);
    typedef SQRESULT (*SqLibAPI_dofile)(HSQUIRRELVM v,const SQChar *filename,SQBool retval,SQBool printerror);
    typedef SQRESULT (*SqLibAPI_writeclosuretofile)(HSQUIRRELVM v,const SQChar *filename);

    //blob
    typedef SQUserPointer (*SqLibAPI_createblob)(HSQUIRRELVM v, SQInteger size);
    typedef SQRESULT (*SqLibAPI_getblob)(HSQUIRRELVM v,SQInteger idx,SQUserPointer *ptr);
    typedef SQInteger (*SqLibAPI_getblobsize)(HSQUIRRELVM v,SQInteger idx);

    //string
    typedef SQRESULT (*SqLibAPI_format)(HSQUIRRELVM v,SQInteger nformatstringidx,SQInteger *outlen,SQChar **output);

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @cond DEV
    /// Allows modules to interface with Squirrel's C api without linking to the squirrel library.
    /// If new functions are added to the Squirrel API, they should be added here too.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    typedef struct
    {
        //vm
        SqLibAPI_open                               open;
        SqLibAPI_newthread                          newthread;
        SqLibAPI_seterrorhandler                    seterrorhandler;
        SqLibAPI_close                              close;
        SqLibAPI_setforeignptr                      setforeignptr;
        SqLibAPI_getforeignptr                      getforeignptr;
        SqLibAPI_setsharedforeignptr                setsharedforeignptr;
        SqLibAPI_getsharedforeignptr                getsharedforeignptr;
        SqLibAPI_setvmreleasehook                   setvmreleasehook;
        SqLibAPI_getvmreleasehook                   getvmreleasehook;
        SqLibAPI_setsharedreleasehook               setsharedreleasehook;
        SqLibAPI_getsharedreleasehook               getsharedreleasehook;
        SqLibAPI_setprintfunc                       setprintfunc;
        SqLibAPI_getprintfunc                       getprintfunc;
        SqLibAPI_geterrorfunc                       geterrorfunc;
        SqLibAPI_suspendvm                          suspendvm;
        SqLibAPI_wakeupvm                           wakeupvm;
        SqLibAPI_getvmstate                         getvmstate;
        SqLibAPI_getversion                         getversion;

        //compiler
        SqLibAPI_compile                            compile;
        SqLibAPI_compilebuffer                      compilebuffer;
        SqLibAPI_enabledebuginfo                    enabledebuginfo;
        SqLibAPI_notifyallexceptions                notifyallexceptions;
        SqLibAPI_setcompilererrorhandler            setcompilererrorhandler;

        //stack operations
        SqLibAPI_push                               push;
        SqLibAPI_pop                                pop;
        SqLibAPI_poptop                             poptop;
        SqLibAPI_remove                             remove;
        SqLibAPI_gettop                             gettop;
        SqLibAPI_settop                             settop;
        SqLibAPI_reservestack                       reservestack;
        SqLibAPI_cmp                                cmp;
        SqLibAPI_move                               move;

        //object creation handling
        SqLibAPI_newuserdata                        newuserdata;
        SqLibAPI_newtable                           newtable;
        SqLibAPI_newtableex                         newtableex;
        SqLibAPI_newarray                           newarray;
        SqLibAPI_newclosure                         newclosure;
        SqLibAPI_setparamscheck                     setparamscheck;
        SqLibAPI_bindenv                            bindenv;
        SqLibAPI_setclosureroot                     setclosureroot;
        SqLibAPI_getclosureroot                     getclosureroot;
        SqLibAPI_pushstring                         pushstring;
        SqLibAPI_pushstringf                        pushstringf;
        SqLibAPI_pushfloat                          pushfloat;
        SqLibAPI_pushinteger                        pushinteger;
        SqLibAPI_pushbool                           pushbool;
        SqLibAPI_pushuserpointer                    pushuserpointer;
        SqLibAPI_pushnull                           pushnull;
        SqLibAPI_pushthread                         pushthread;
        SqLibAPI_gettype                            gettype;
        SqLibAPI_typeof                             typeof_;
        SqLibAPI_getsize                            getsize;
        SqLibAPI_gethash                            gethash;
        SqLibAPI_getbase                            getbase;
        SqLibAPI_instanceof                         instanceof;
        SqLibAPI_tostring                           tostring;
        SqLibAPI_tobool                             tobool;
        SqLibAPI_getstringandsize                   getstringandsize;
        SqLibAPI_getstring                          getstring;
        SqLibAPI_getinteger                         getinteger;
        SqLibAPI_getfloat                           getfloat;
        SqLibAPI_getbool                            getbool;
        SqLibAPI_getthread                          getthread;
        SqLibAPI_getuserpointer                     getuserpointer;
        SqLibAPI_getuserdata                        getuserdata;
        SqLibAPI_settypetag                         settypetag;
        SqLibAPI_gettypetag                         gettypetag;
        SqLibAPI_setreleasehook                     setreleasehook;
        SqLibAPI_getreleasehook                     getreleasehook;
        SqLibAPI_getscratchpad                      getscratchpad;
        SqLibAPI_getfunctioninfo                    getfunctioninfo;
        SqLibAPI_getclosureinfo                     getclosureinfo;
        SqLibAPI_getclosurename                     getclosurename;
        SqLibAPI_setnativeclosurename               setnativeclosurename;
        SqLibAPI_getnativeclosurepointer            getnativeclosurepointer;
        SqLibAPI_setinstanceup                      setinstanceup;
        SqLibAPI_getinstanceup                      getinstanceup;
        SqLibAPI_setclassudsize                     setclassudsize;
        SqLibAPI_newclass                           newclass;
        SqLibAPI_createinstance                     createinstance;
        SqLibAPI_setattributes                      setattributes;
        SqLibAPI_getattributes                      getattributes;
        SqLibAPI_getclass                           getclass;
        SqLibAPI_weakref                            weakref;
        SqLibAPI_getdefaultdelegate                 getdefaultdelegate;
        SqLibAPI_getmemberhandle                    getmemberhandle;
        SqLibAPI_getbyhandle                        getbyhandle;
        SqLibAPI_setbyhandle                        setbyhandle;

        //object manipulation
        SqLibAPI_pushroottable                      pushroottable;
        SqLibAPI_pushregistrytable                  pushregistrytable;
        SqLibAPI_pushconsttable                     pushconsttable;
        SqLibAPI_setroottable                       setroottable;
        SqLibAPI_setconsttable                      setconsttable;
        SqLibAPI_newslot                            newslot;
        SqLibAPI_deleteslot                         deleteslot;
        SqLibAPI_set                                set;
        SqLibAPI_get                                get;
        SqLibAPI_rawget                             rawget;
        SqLibAPI_rawset                             rawset;
        SqLibAPI_rawdeleteslot                      rawdeleteslot;
        SqLibAPI_newmember                          newmember;
        SqLibAPI_rawnewmember                       rawnewmember;
        SqLibAPI_arrayappend                        arrayappend;
        SqLibAPI_arraypop                           arraypop;
        SqLibAPI_arrayresize                        arrayresize;
        SqLibAPI_arrayreverse                       arrayreverse;
        SqLibAPI_arrayremove                        arrayremove;
        SqLibAPI_arrayinsert                        arrayinsert;
        SqLibAPI_setdelegate                        setdelegate;
        SqLibAPI_getdelegate                        getdelegate;
        SqLibAPI_clone                              clone;
        SqLibAPI_setfreevariable                    setfreevariable;
        SqLibAPI_next                               next;
        SqLibAPI_getweakrefval                      getweakrefval;
        SqLibAPI_clear                              clear;

        //calls
        SqLibAPI_call                               call;
        SqLibAPI_resume                             resume;
        SqLibAPI_getlocal                           getlocal;
        SqLibAPI_getcallee                          getcallee;
        SqLibAPI_getfreevariable                    getfreevariable;
        SqLibAPI_getonefreevariable                 getonefreevariable;
        SqLibAPI_throwerror                         throwerror;
        SqLibAPI_throwerrorf                        throwerrorf;
        SqLibAPI_throwobject                        throwobject;
        SqLibAPI_reseterror                         reseterror;
        SqLibAPI_getlasterror                       getlasterror;

        //raw object handling
        SqLibAPI_getstackobj                        getstackobj;
        SqLibAPI_pushobject                         pushobject;
        SqLibAPI_addref                             addref;
        SqLibAPI_release                            release;
        SqLibAPI_getrefcount                        getrefcount;
        SqLibAPI_resetobject                        resetobject;
        SqLibAPI_objtostring                        objtostring;
        SqLibAPI_objtobool                          objtobool;
        SqLibAPI_objtointeger                       objtointeger;
        SqLibAPI_objtofloat                         objtofloat;
        SqLibAPI_objtouserpointer                   objtouserpointer;
        SqLibAPI_getobjtypetag                      getobjtypetag;
        SqLibAPI_getvmrefcount                      getvmrefcount;

        //GC
        SqLibAPI_collectgarbage                     collectgarbage;
        SqLibAPI_resurrectunreachable               resurrectunreachable;

        //serialization
        SqLibAPI_writeclosure                       writeclosure;
        SqLibAPI_readclosure                        readclosure;

        //mem allocation
        SqLibAPI_malloc                             malloc;
        SqLibAPI_realloc                            realloc;
        SqLibAPI_free                               free;

        //debug
        SqLibAPI_stackinfos                         stackinfos;
        SqLibAPI_setdebughook                       setdebughook;
        SqLibAPI_setnativedebughook                 setnativedebughook;

        //compiler helpers
        SqLibAPI_loadfile                           loadfile;
        SqLibAPI_dofile                             dofile;
        SqLibAPI_writeclosuretofile                 writeclosuretofile;

        //blob
        SqLibAPI_createblob                         createblob;
        SqLibAPI_getblob                            getblob;
        SqLibAPI_getblobsize                        getblobsize;

        //string
        SqLibAPI_format                             format;
    } sqlib_api, SQLIBAPI, *HSQLIBAPI;
    /// @endcond

#ifdef SQMOD_PLUGIN_API

    //vm
    extern SqLibAPI_open                            SqLib_open;
    extern SqLibAPI_newthread                       SqLib_newthread;
    extern SqLibAPI_seterrorhandler                 SqLib_seterrorhandler;
    extern SqLibAPI_close                           SqLib_close;
    extern SqLibAPI_setforeignptr                   SqLib_setforeignptr;
    extern SqLibAPI_getforeignptr                   SqLib_getforeignptr;
    extern SqLibAPI_setsharedforeignptr             SqLib_setsharedforeignptr;
    extern SqLibAPI_getsharedforeignptr             SqLib_getsharedforeignptr;
    extern SqLibAPI_setvmreleasehook                SqLib_setvmreleasehook;
    extern SqLibAPI_getvmreleasehook                SqLib_getvmreleasehook;
    extern SqLibAPI_setsharedreleasehook            SqLib_setsharedreleasehook;
    extern SqLibAPI_getsharedreleasehook            SqLib_getsharedreleasehook;
    extern SqLibAPI_setprintfunc                    SqLib_setprintfunc;
    extern SqLibAPI_getprintfunc                    SqLib_getprintfunc;
    extern SqLibAPI_geterrorfunc                    SqLib_geterrorfunc;
    extern SqLibAPI_suspendvm                       SqLib_suspendvm;
    extern SqLibAPI_wakeupvm                        SqLib_wakeupvm;
    extern SqLibAPI_getvmstate                      SqLib_getvmstate;
    extern SqLibAPI_getversion                      SqLib_getversion;

    //compiler
    extern SqLibAPI_compile                         SqLib_compile;
    extern SqLibAPI_compilebuffer                   SqLib_compilebuffer;
    extern SqLibAPI_enabledebuginfo                 SqLib_enabledebuginfo;
    extern SqLibAPI_notifyallexceptions             SqLib_notifyallexceptions;
    extern SqLibAPI_setcompilererrorhandler         SqLib_setcompilererrorhandler;

    //stack operations
    extern SqLibAPI_push                            SqLib_push;
    extern SqLibAPI_pop                             SqLib_pop;
    extern SqLibAPI_poptop                          SqLib_poptop;
    extern SqLibAPI_remove                          SqLib_remove;
    extern SqLibAPI_gettop                          SqLib_gettop;
    extern SqLibAPI_settop                          SqLib_settop;
    extern SqLibAPI_reservestack                    SqLib_reservestack;
    extern SqLibAPI_cmp                             SqLib_cmp;
    extern SqLibAPI_move                            SqLib_move;

    //object creation handling
    extern SqLibAPI_newuserdata                     SqLib_newuserdata;
    extern SqLibAPI_newtable                        SqLib_newtable;
    extern SqLibAPI_newtableex                      SqLib_newtableex;
    extern SqLibAPI_newarray                        SqLib_newarray;
    extern SqLibAPI_newclosure                      SqLib_newclosure;
    extern SqLibAPI_setparamscheck                  SqLib_setparamscheck;
    extern SqLibAPI_bindenv                         SqLib_bindenv;
    extern SqLibAPI_setclosureroot                  SqLib_setclosureroot;
    extern SqLibAPI_getclosureroot                  SqLib_getclosureroot;
    extern SqLibAPI_pushstring                      SqLib_pushstring;
    extern SqLibAPI_pushstringf                     SqLib_pushstringf;
    extern SqLibAPI_pushfloat                       SqLib_pushfloat;
    extern SqLibAPI_pushinteger                     SqLib_pushinteger;
    extern SqLibAPI_pushbool                        SqLib_pushbool;
    extern SqLibAPI_pushuserpointer                 SqLib_pushuserpointer;
    extern SqLibAPI_pushnull                        SqLib_pushnull;
    extern SqLibAPI_pushthread                      SqLib_pushthread;
    extern SqLibAPI_gettype                         SqLib_gettype;
    extern SqLibAPI_typeof                          SqLib_typeof;
    extern SqLibAPI_getsize                         SqLib_getsize;
    extern SqLibAPI_gethash                         SqLib_gethash;
    extern SqLibAPI_getbase                         SqLib_getbase;
    extern SqLibAPI_instanceof                      SqLib_instanceof;
    extern SqLibAPI_tostring                        SqLib_tostring;
    extern SqLibAPI_tobool                          SqLib_tobool;
    extern SqLibAPI_getstringandsize                SqLib_getstringandsize;
    extern SqLibAPI_getstring                       SqLib_getstring;
    extern SqLibAPI_getinteger                      SqLib_getinteger;
    extern SqLibAPI_getfloat                        SqLib_getfloat;
    extern SqLibAPI_getbool                         SqLib_getbool;
    extern SqLibAPI_getthread                       SqLib_getthread;
    extern SqLibAPI_getuserpointer                  SqLib_getuserpointer;
    extern SqLibAPI_getuserdata                     SqLib_getuserdata;
    extern SqLibAPI_settypetag                      SqLib_settypetag;
    extern SqLibAPI_gettypetag                      SqLib_gettypetag;
    extern SqLibAPI_setreleasehook                  SqLib_setreleasehook;
    extern SqLibAPI_getreleasehook                  SqLib_getreleasehook;
    extern SqLibAPI_getscratchpad                   SqLib_getscratchpad;
    extern SqLibAPI_getfunctioninfo                 SqLib_getfunctioninfo;
    extern SqLibAPI_getclosureinfo                  SqLib_getclosureinfo;
    extern SqLibAPI_getclosurename                  SqLib_getclosurename;
    extern SqLibAPI_setnativeclosurename            SqLib_setnativeclosurename;
    extern SqLibAPI_getnativeclosurepointer         SqLib_getnativeclosurepointer;
    extern SqLibAPI_setinstanceup                   SqLib_setinstanceup;
    extern SqLibAPI_getinstanceup                   SqLib_getinstanceup;
    extern SqLibAPI_setclassudsize                  SqLib_setclassudsize;
    extern SqLibAPI_newclass                        SqLib_newclass;
    extern SqLibAPI_createinstance                  SqLib_createinstance;
    extern SqLibAPI_setattributes                   SqLib_setattributes;
    extern SqLibAPI_getattributes                   SqLib_getattributes;
    extern SqLibAPI_getclass                        SqLib_getclass;
    extern SqLibAPI_weakref                         SqLib_weakref;
    extern SqLibAPI_getdefaultdelegate              SqLib_getdefaultdelegate;
    extern SqLibAPI_getmemberhandle                 SqLib_getmemberhandle;
    extern SqLibAPI_getbyhandle                     SqLib_getbyhandle;
    extern SqLibAPI_setbyhandle                     SqLib_setbyhandle;

    //object manipulation
    extern SqLibAPI_pushroottable                   SqLib_pushroottable;
    extern SqLibAPI_pushregistrytable               SqLib_pushregistrytable;
    extern SqLibAPI_pushconsttable                  SqLib_pushconsttable;
    extern SqLibAPI_setroottable                    SqLib_setroottable;
    extern SqLibAPI_setconsttable                   SqLib_setconsttable;
    extern SqLibAPI_newslot                         SqLib_newslot;
    extern SqLibAPI_deleteslot                      SqLib_deleteslot;
    extern SqLibAPI_set                             SqLib_set;
    extern SqLibAPI_get                             SqLib_get;
    extern SqLibAPI_rawget                          SqLib_rawget;
    extern SqLibAPI_rawset                          SqLib_rawset;
    extern SqLibAPI_rawdeleteslot                   SqLib_rawdeleteslot;
    extern SqLibAPI_newmember                       SqLib_newmember;
    extern SqLibAPI_rawnewmember                    SqLib_rawnewmember;
    extern SqLibAPI_arrayappend                     SqLib_arrayappend;
    extern SqLibAPI_arraypop                        SqLib_arraypop;
    extern SqLibAPI_arrayresize                     SqLib_arrayresize;
    extern SqLibAPI_arrayreverse                    SqLib_arrayreverse;
    extern SqLibAPI_arrayremove                     SqLib_arrayremove;
    extern SqLibAPI_arrayinsert                     SqLib_arrayinsert;
    extern SqLibAPI_setdelegate                     SqLib_setdelegate;
    extern SqLibAPI_getdelegate                     SqLib_getdelegate;
    extern SqLibAPI_clone                           SqLib_clone;
    extern SqLibAPI_setfreevariable                 SqLib_setfreevariable;
    extern SqLibAPI_next                            SqLib_next;
    extern SqLibAPI_getweakrefval                   SqLib_getweakrefval;
    extern SqLibAPI_clear                           SqLib_clear;

    //calls
    extern SqLibAPI_call                            SqLib_call;
    extern SqLibAPI_resume                          SqLib_resume;
    extern SqLibAPI_getlocal                        SqLib_getlocal;
    extern SqLibAPI_getcallee                       SqLib_getcallee;
    extern SqLibAPI_getfreevariable                 SqLib_getfreevariable;
    extern SqLibAPI_getonefreevariable              SqLib_getonefreevariable;
    extern SqLibAPI_throwerror                      SqLib_throwerror;
    extern SqLibAPI_throwerrorf                     SqLib_throwerrorf;
    extern SqLibAPI_throwobject                     SqLib_throwobject;
    extern SqLibAPI_reseterror                      SqLib_reseterror;
    extern SqLibAPI_getlasterror                    SqLib_getlasterror;

    //raw object handling
    extern SqLibAPI_getstackobj                     SqLib_getstackobj;
    extern SqLibAPI_pushobject                      SqLib_pushobject;
    extern SqLibAPI_addref                          SqLib_addref;
    extern SqLibAPI_release                         SqLib_release;
    extern SqLibAPI_getrefcount                     SqLib_getrefcount;
    extern SqLibAPI_resetobject                     SqLib_resetobject;
    extern SqLibAPI_objtostring                     SqLib_objtostring;
    extern SqLibAPI_objtobool                       SqLib_objtobool;
    extern SqLibAPI_objtointeger                    SqLib_objtointeger;
    extern SqLibAPI_objtofloat                      SqLib_objtofloat;
    extern SqLibAPI_objtouserpointer                SqLib_objtouserpointer;
    extern SqLibAPI_getobjtypetag                   SqLib_getobjtypetag;
    extern SqLibAPI_getvmrefcount                   SqLib_getvmrefcount;

    //GC
    extern SqLibAPI_collectgarbage                  SqLib_collectgarbage;
    extern SqLibAPI_resurrectunreachable            SqLib_resurrectunreachable;

    //serialization
    extern SqLibAPI_writeclosure                    SqLib_writeclosure;
    extern SqLibAPI_readclosure                     SqLib_readclosure;

    //mem allocation
    extern SqLibAPI_malloc                          SqLib_malloc;
    extern SqLibAPI_realloc                         SqLib_realloc;
    extern SqLibAPI_free                            SqLib_free;

    //debug
    extern SqLibAPI_stackinfos                      SqLib_stackinfos;
    extern SqLibAPI_setdebughook                    SqLib_setdebughook;
    extern SqLibAPI_setnativedebughook              SqLib_setnativedebughook;

    //compiler helpers
    extern SqLibAPI_loadfile                        SqLib_loadfile;
    extern SqLibAPI_dofile                          SqLib_dofile;
    extern SqLibAPI_writeclosuretofile              SqLib_writeclosuretofile;

    //blob
    extern SqLibAPI_createblob                      SqLib_createblob;
    extern SqLibAPI_getblob                         SqLib_getblob;
    extern SqLibAPI_getblobsize                     SqLib_getblobsize;

    //string
    extern SqLibAPI_format                          SqLib_format;

    //vm
    #define sq_open                                 SqLib_open
    #define sq_newthread                            SqLib_newthread
    #define sq_seterrorhandler                      SqLib_seterrorhandler
    #define sq_close                                SqLib_close
    #define sq_setforeignptr                        SqLib_setforeignptr
    #define sq_getforeignptr                        SqLib_getforeignptr
    #define sq_setsharedforeignptr                  SqLib_setsharedforeignptr
    #define sq_getsharedforeignptr                  SqLib_getsharedforeignptr
    #define sq_setvmreleasehook                     SqLib_setvmreleasehook
    #define sq_getvmreleasehook                     SqLib_getvmreleasehook
    #define sq_setsharedreleasehook                 SqLib_setsharedreleasehook
    #define sq_getsharedreleasehook                 SqLib_getsharedreleasehook
    #define sq_setprintfunc                         SqLib_setprintfunc
    #define sq_getprintfunc                         SqLib_getprintfunc
    #define sq_geterrorfunc                         SqLib_geterrorfunc
    #define sq_suspendvm                            SqLib_suspendvm
    #define sq_wakeupvm                             SqLib_wakeupvm
    #define sq_getvmstate                           SqLib_getvmstate
    #define sq_getversion                           SqLib_getversion

    //compiler
    #define sq_compile                              SqLib_compile
    #define sq_compilebuffer                        SqLib_compilebuffer
    #define sq_enabledebuginfo                      SqLib_enabledebuginfo
    #define sq_notifyallexceptions                  SqLib_notifyallexceptions
    #define sq_setcompilererrorhandler              SqLib_setcompilererrorhandler

    //stack operations
    #define sq_push                                 SqLib_push
    #define sq_pop                                  SqLib_pop
    #define sq_poptop                               SqLib_poptop
    #define sq_remove                               SqLib_remove
    #define sq_gettop                               SqLib_gettop
    #define sq_settop                               SqLib_settop
    #define sq_reservestack                         SqLib_reservestack
    #define sq_cmp                                  SqLib_cmp
    #define sq_move                                 SqLib_move

    //object creation handling
    #define sq_newuserdata                          SqLib_newuserdata
    #define sq_newtable                             SqLib_newtable
    #define sq_newtableex                           SqLib_newtableex
    #define sq_newarray                             SqLib_newarray
    #define sq_newclosure                           SqLib_newclosure
    #define sq_setparamscheck                       SqLib_setparamscheck
    #define sq_bindenv                              SqLib_bindenv
    #define sq_setclosureroot                       SqLib_setclosureroot
    #define sq_getclosureroot                       SqLib_getclosureroot
    #define sq_pushstring                           SqLib_pushstring
    #define sq_pushstringf                          SqLib_pushstringf
    #define sq_pushfloat                            SqLib_pushfloat
    #define sq_pushinteger                          SqLib_pushinteger
    #define sq_pushbool                             SqLib_pushbool
    #define sq_pushuserpointer                      SqLib_pushuserpointer
    #define sq_pushnull                             SqLib_pushnull
    #define sq_pushthread                           SqLib_pushthread
    #define sq_gettype                              SqLib_gettype
    #define sq_typeof                               SqLib_typeof
    #define sq_getsize                              SqLib_getsize
    #define sq_gethash                              SqLib_gethash
    #define sq_getbase                              SqLib_getbase
    #define sq_instanceof                           SqLib_instanceof
    #define sq_tostring                             SqLib_tostring
    #define sq_tobool                               SqLib_tobool
    #define sq_getstringandsize                     SqLib_getstringandsize
    #define sq_getstring                            SqLib_getstring
    #define sq_getinteger                           SqLib_getinteger
    #define sq_getfloat                             SqLib_getfloat
    #define sq_getbool                              SqLib_getbool
    #define sq_getthread                            SqLib_getthread
    #define sq_getuserpointer                       SqLib_getuserpointer
    #define sq_getuserdata                          SqLib_getuserdata
    #define sq_settypetag                           SqLib_settypetag
    #define sq_gettypetag                           SqLib_gettypetag
    #define sq_setreleasehook                       SqLib_setreleasehook
    #define sq_getreleasehook                       SqLib_getreleasehook
    #define sq_getscratchpad                        SqLib_getscratchpad
    #define sq_getfunctioninfo                      SqLib_getfunctioninfo
    #define sq_getclosureinfo                       SqLib_getclosureinfo
    #define sq_getclosurename                       SqLib_getclosurename
    #define sq_setnativeclosurename                 SqLib_setnativeclosurename
    #define sq_getnativeclosurepointer              SqLib_getnativeclosurepointer
    #define sq_setinstanceup                        SqLib_setinstanceup
    #define sq_getinstanceup                        SqLib_getinstanceup
    #define sq_setclassudsize                       SqLib_setclassudsize
    #define sq_newclass                             SqLib_newclass
    #define sq_createinstance                       SqLib_createinstance
    #define sq_setattributes                        SqLib_setattributes
    #define sq_getattributes                        SqLib_getattributes
    #define sq_getclass                             SqLib_getclass
    #define sq_weakref                              SqLib_weakref
    #define sq_getdefaultdelegate                   SqLib_getdefaultdelegate
    #define sq_getmemberhandle                      SqLib_getmemberhandle
    #define sq_getbyhandle                          SqLib_getbyhandle
    #define sq_setbyhandle                          SqLib_setbyhandle

    //object manipulation
    #define sq_pushroottable                        SqLib_pushroottable
    #define sq_pushregistrytable                    SqLib_pushregistrytable
    #define sq_pushconsttable                       SqLib_pushconsttable
    #define sq_setroottable                         SqLib_setroottable
    #define sq_setconsttable                        SqLib_setconsttable
    #define sq_newslot                              SqLib_newslot
    #define sq_deleteslot                           SqLib_deleteslot
    #define sq_set                                  SqLib_set
    #define sq_get                                  SqLib_get
    #define sq_rawget                               SqLib_rawget
    #define sq_rawset                               SqLib_rawset
    #define sq_rawdeleteslot                        SqLib_rawdeleteslot
    #define sq_newmember                            SqLib_newmember
    #define sq_rawnewmember                         SqLib_rawnewmember
    #define sq_arrayappend                          SqLib_arrayappend
    #define sq_arraypop                             SqLib_arraypop
    #define sq_arrayresize                          SqLib_arrayresize
    #define sq_arrayreverse                         SqLib_arrayreverse
    #define sq_arrayremove                          SqLib_arrayremove
    #define sq_arrayinsert                          SqLib_arrayinsert
    #define sq_setdelegate                          SqLib_setdelegate
    #define sq_getdelegate                          SqLib_getdelegate
    #define sq_clone                                SqLib_clone
    #define sq_setfreevariable                      SqLib_setfreevariable
    #define sq_next                                 SqLib_next
    #define sq_getweakrefval                        SqLib_getweakrefval
    #define sq_clear                                SqLib_clear

    //calls
    #define sq_call                                 SqLib_call
    #define sq_resume                               SqLib_resume
    #define sq_getlocal                             SqLib_getlocal
    #define sq_getcallee                            SqLib_getcallee
    #define sq_getfreevariable                      SqLib_getfreevariable
    #define sq_getonefreevariable                   SqLib_getonefreevariable
    #define sq_throwerror                           SqLib_throwerror
    #define sq_throwerrorf                          SqLib_throwerrorf
    #define sq_throwobject                          SqLib_throwobject
    #define sq_reseterror                           SqLib_reseterror
    #define sq_getlasterror                         SqLib_getlasterror

    //raw object handling
    #define sq_getstackobj                          SqLib_getstackobj
    #define sq_pushobject                           SqLib_pushobject
    #define sq_addref                               SqLib_addref
    #define sq_release                              SqLib_release
    #define sq_getrefcount                          SqLib_getrefcount
    #define sq_resetobject                          SqLib_resetobject
    #define sq_objtostring                          SqLib_objtostring
    #define sq_objtobool                            SqLib_objtobool
    #define sq_objtointeger                         SqLib_objtointeger
    #define sq_objtofloat                           SqLib_objtofloat
    #define sq_objtouserpointer                     SqLib_objtouserpointer
    #define sq_getobjtypetag                        SqLib_getobjtypetag
    #define sq_getvmrefcount                        SqLib_getvmrefcount

    //GC
    #define sq_collectgarbage                       SqLib_collectgarbage
    #define sq_resurrectunreachable                 SqLib_resurrectunreachable

    //serialization
    #define sq_writeclosure                         SqLib_writeclosure
    #define sq_readclosure                          SqLib_readclosure

    //mem allocation
    #define sq_malloc                               SqLib_malloc
    #define sq_realloc                              SqLib_realloc
    #define sq_free                                 SqLib_free

    //debug
    #define sq_stackinfos                           SqLib_stackinfos
    #define sq_setdebughook                         SqLib_setdebughook
    #define sq_setnativedebughook                   SqLib_setnativedebughook

    //compiler helpers
    #define sqstd_loadfile                          SqLib_loadfile
    #define sqstd_dofile                            SqLib_dofile
    #define sqstd_writeclosuretofile                SqLib_writeclosuretofile

    //blob
    #define sqstd_createblob                        SqLib_createblob
    #define sqstd_getblob                           SqLib_getblob
    #define sqstd_getblobsize                       SqLib_getblobsize

    //string
    #define sqstd_format                            SqLib_format

#endif // SQMOD_PLUGIN_API

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*_SQ_API_H_*/
