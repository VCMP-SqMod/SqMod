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
    typedef HSQUIRRELVM (*sqapi_open)(SQInteger initialstacksize);
    typedef HSQUIRRELVM (*sqapi_newthread)(HSQUIRRELVM friendvm, SQInteger initialstacksize);
    typedef void (*sqapi_seterrorhandler)(HSQUIRRELVM v);
    typedef void (*sqapi_close)(HSQUIRRELVM v);
    typedef void (*sqapi_setforeignptr)(HSQUIRRELVM v,SQUserPointer p);
    typedef SQUserPointer (*sqapi_getforeignptr)(HSQUIRRELVM v);
    typedef void (*sqapi_setsharedforeignptr)(HSQUIRRELVM v,SQUserPointer p);
    typedef SQUserPointer (*sqapi_getsharedforeignptr)(HSQUIRRELVM v);
    typedef void (*sqapi_setvmreleasehook)(HSQUIRRELVM v,SQRELEASEHOOK hook);
    typedef SQRELEASEHOOK (*sqapi_getvmreleasehook)(HSQUIRRELVM v);
    typedef void (*sqapi_setsharedreleasehook)(HSQUIRRELVM v,SQRELEASEHOOK hook);
    typedef SQRELEASEHOOK (*sqapi_getsharedreleasehook)(HSQUIRRELVM v);
    typedef void (*sqapi_setprintfunc)(HSQUIRRELVM v, SQPRINTFUNCTION printfunc,SQPRINTFUNCTION errfunc);
    typedef SQPRINTFUNCTION (*sqapi_getprintfunc)(HSQUIRRELVM v);
    typedef SQPRINTFUNCTION (*sqapi_geterrorfunc)(HSQUIRRELVM v);
    typedef SQRESULT (*sqapi_suspendvm)(HSQUIRRELVM v);
    typedef SQRESULT (*sqapi_wakeupvm)(HSQUIRRELVM v,SQBool resumedret,SQBool retval,SQBool raiseerror,SQBool throwerror);
    typedef SQInteger (*sqapi_getvmstate)(HSQUIRRELVM v);
    typedef SQInteger (*sqapi_getversion)();

    //compiler
    typedef SQRESULT (*sqapi_compile)(HSQUIRRELVM v,SQLEXREADFUNC read,SQUserPointer p,const SQChar *sourcename,SQBool raiseerror);
    typedef SQRESULT (*sqapi_compilebuffer)(HSQUIRRELVM v,const SQChar *s,SQInteger size,const SQChar *sourcename,SQBool raiseerror);
    typedef void (*sqapi_enabledebuginfo)(HSQUIRRELVM v, SQBool enable);
    typedef void (*sqapi_notifyallexceptions)(HSQUIRRELVM v, SQBool enable);
    typedef void (*sqapi_setcompilererrorhandler)(HSQUIRRELVM v,SQCOMPILERERROR f);

    //stack operations
    typedef void (*sqapi_push)(HSQUIRRELVM v,SQInteger idx);
    typedef void (*sqapi_pop)(HSQUIRRELVM v,SQInteger nelemstopop);
    typedef void (*sqapi_poptop)(HSQUIRRELVM v);
    typedef void (*sqapi_remove)(HSQUIRRELVM v,SQInteger idx);
    typedef SQInteger (*sqapi_gettop)(HSQUIRRELVM v);
    typedef void (*sqapi_settop)(HSQUIRRELVM v,SQInteger newtop);
    typedef SQRESULT (*sqapi_reservestack)(HSQUIRRELVM v,SQInteger nsize);
    typedef SQInteger (*sqapi_cmp)(HSQUIRRELVM v);
    typedef void (*sqapi_move)(HSQUIRRELVM dest,HSQUIRRELVM src,SQInteger idx);

    //object creation handling
    typedef SQUserPointer (*sqapi_newuserdata)(HSQUIRRELVM v,SQUnsignedInteger size);
    typedef void (*sqapi_newtable)(HSQUIRRELVM v);
    typedef void (*sqapi_newtableex)(HSQUIRRELVM v,SQInteger initialcapacity);
    typedef void (*sqapi_newarray)(HSQUIRRELVM v,SQInteger size);
    typedef void (*sqapi_newclosure)(HSQUIRRELVM v,SQFUNCTION func,SQUnsignedInteger nfreevars);
    typedef SQRESULT (*sqapi_setparamscheck)(HSQUIRRELVM v,SQInteger nparamscheck,const SQChar *typemask);
    typedef SQRESULT (*sqapi_bindenv)(HSQUIRRELVM v,SQInteger idx);
    typedef SQRESULT (*sqapi_setclosureroot)(HSQUIRRELVM v,SQInteger idx);
    typedef SQRESULT (*sqapi_getclosureroot)(HSQUIRRELVM v,SQInteger idx);
    typedef void (*sqapi_pushstring)(HSQUIRRELVM v,const SQChar *s,SQInteger len);
    typedef void (*sqapi_pushfloat)(HSQUIRRELVM v,SQFloat f);
    typedef void (*sqapi_pushinteger)(HSQUIRRELVM v,SQInteger n);
    typedef void (*sqapi_pushbool)(HSQUIRRELVM v,SQBool b);
    typedef void (*sqapi_pushuserpointer)(HSQUIRRELVM v,SQUserPointer p);
    typedef void (*sqapi_pushnull)(HSQUIRRELVM v);
    typedef void (*sqapi_pushthread)(HSQUIRRELVM v, HSQUIRRELVM thread);
    typedef SQObjectType (*sqapi_gettype)(HSQUIRRELVM v,SQInteger idx);
    typedef SQRESULT (*sqapi_typeof)(HSQUIRRELVM v,SQInteger idx);
    typedef SQInteger (*sqapi_getsize)(HSQUIRRELVM v,SQInteger idx);
    typedef SQHash (*sqapi_gethash)(HSQUIRRELVM v, SQInteger idx);
    typedef SQRESULT (*sqapi_getbase)(HSQUIRRELVM v,SQInteger idx);
    typedef SQBool (*sqapi_instanceof)(HSQUIRRELVM v);
    typedef SQRESULT (*sqapi_tostring)(HSQUIRRELVM v,SQInteger idx);
    typedef void (*sqapi_tobool)(HSQUIRRELVM v, SQInteger idx, SQBool *b);
    typedef SQRESULT (*sqapi_getstringandsize)(HSQUIRRELVM v,SQInteger idx,const SQChar **c,SQInteger *size);
    typedef SQRESULT (*sqapi_getstring)(HSQUIRRELVM v,SQInteger idx,const SQChar **c);
    typedef SQRESULT (*sqapi_getinteger)(HSQUIRRELVM v,SQInteger idx,SQInteger *i);
    typedef SQRESULT (*sqapi_getfloat)(HSQUIRRELVM v,SQInteger idx,SQFloat *f);
    typedef SQRESULT (*sqapi_getbool)(HSQUIRRELVM v,SQInteger idx,SQBool *b);
    typedef SQRESULT (*sqapi_getthread)(HSQUIRRELVM v,SQInteger idx,HSQUIRRELVM *thread);
    typedef SQRESULT (*sqapi_getuserpointer)(HSQUIRRELVM v,SQInteger idx,SQUserPointer *p);
    typedef SQRESULT (*sqapi_getuserdata)(HSQUIRRELVM v,SQInteger idx,SQUserPointer *p,SQUserPointer *typetag);
    typedef SQRESULT (*sqapi_settypetag)(HSQUIRRELVM v,SQInteger idx,SQUserPointer typetag);
    typedef SQRESULT (*sqapi_gettypetag)(HSQUIRRELVM v,SQInteger idx,SQUserPointer *typetag);
    typedef void (*sqapi_setreleasehook)(HSQUIRRELVM v,SQInteger idx,SQRELEASEHOOK hook);
    typedef SQRELEASEHOOK (*sqapi_getreleasehook)(HSQUIRRELVM v,SQInteger idx);
    typedef SQChar* (*sqapi_getscratchpad)(HSQUIRRELVM v,SQInteger minsize);
    typedef SQRESULT (*sqapi_getfunctioninfo)(HSQUIRRELVM v,SQInteger level,SQFunctionInfo *fi);
    typedef SQRESULT (*sqapi_getclosureinfo)(HSQUIRRELVM v,SQInteger idx,SQUnsignedInteger *nparams,SQUnsignedInteger *nfreevars);
    typedef SQRESULT (*sqapi_getclosurename)(HSQUIRRELVM v,SQInteger idx);
    typedef SQRESULT (*sqapi_setnativeclosurename)(HSQUIRRELVM v,SQInteger idx,const SQChar *name);
    typedef SQRESULT (*sqapi_setinstanceup)(HSQUIRRELVM v, SQInteger idx, SQUserPointer p);
    typedef SQRESULT (*sqapi_getinstanceup)(HSQUIRRELVM v, SQInteger idx, SQUserPointer *p,SQUserPointer typetag);
    typedef SQRESULT (*sqapi_setclassudsize)(HSQUIRRELVM v, SQInteger idx, SQInteger udsize);
    typedef SQRESULT (*sqapi_newclass)(HSQUIRRELVM v,SQBool hasbase);
    typedef SQRESULT (*sqapi_createinstance)(HSQUIRRELVM v,SQInteger idx);
    typedef SQRESULT (*sqapi_setattributes)(HSQUIRRELVM v,SQInteger idx);
    typedef SQRESULT (*sqapi_getattributes)(HSQUIRRELVM v,SQInteger idx);
    typedef SQRESULT (*sqapi_getclass)(HSQUIRRELVM v,SQInteger idx);
    typedef void (*sqapi_weakref)(HSQUIRRELVM v,SQInteger idx);
    typedef SQRESULT (*sqapi_getdefaultdelegate)(HSQUIRRELVM v,SQObjectType t);
    typedef SQRESULT (*sqapi_getmemberhandle)(HSQUIRRELVM v,SQInteger idx,HSQMEMBERHANDLE *handle);
    typedef SQRESULT (*sqapi_getbyhandle)(HSQUIRRELVM v,SQInteger idx,const HSQMEMBERHANDLE *handle);
    typedef SQRESULT (*sqapi_setbyhandle)(HSQUIRRELVM v,SQInteger idx,const HSQMEMBERHANDLE *handle);

    //object manipulation
    typedef void (*sqapi_pushroottable)(HSQUIRRELVM v);
    typedef void (*sqapi_pushregistrytable)(HSQUIRRELVM v);
    typedef void (*sqapi_pushconsttable)(HSQUIRRELVM v);
    typedef SQRESULT (*sqapi_setroottable)(HSQUIRRELVM v);
    typedef SQRESULT (*sqapi_setconsttable)(HSQUIRRELVM v);
    typedef SQRESULT (*sqapi_newslot)(HSQUIRRELVM v, SQInteger idx, SQBool bstatic);
    typedef SQRESULT (*sqapi_deleteslot)(HSQUIRRELVM v,SQInteger idx,SQBool pushval);
    typedef SQRESULT (*sqapi_set)(HSQUIRRELVM v,SQInteger idx);
    typedef SQRESULT (*sqapi_get)(HSQUIRRELVM v,SQInteger idx);
    typedef SQRESULT (*sqapi_rawget)(HSQUIRRELVM v,SQInteger idx);
    typedef SQRESULT (*sqapi_rawset)(HSQUIRRELVM v,SQInteger idx);
    typedef SQRESULT (*sqapi_rawdeleteslot)(HSQUIRRELVM v,SQInteger idx,SQBool pushval);
    typedef SQRESULT (*sqapi_newmember)(HSQUIRRELVM v,SQInteger idx,SQBool bstatic);
    typedef SQRESULT (*sqapi_rawnewmember)(HSQUIRRELVM v,SQInteger idx,SQBool bstatic);
    typedef SQRESULT (*sqapi_arrayappend)(HSQUIRRELVM v,SQInteger idx);
    typedef SQRESULT (*sqapi_arraypop)(HSQUIRRELVM v,SQInteger idx,SQBool pushval);
    typedef SQRESULT (*sqapi_arrayresize)(HSQUIRRELVM v,SQInteger idx,SQInteger newsize);
    typedef SQRESULT (*sqapi_arrayreverse)(HSQUIRRELVM v,SQInteger idx);
    typedef SQRESULT (*sqapi_arrayremove)(HSQUIRRELVM v,SQInteger idx,SQInteger itemidx);
    typedef SQRESULT (*sqapi_arrayinsert)(HSQUIRRELVM v,SQInteger idx,SQInteger destpos);
    typedef SQRESULT (*sqapi_setdelegate)(HSQUIRRELVM v,SQInteger idx);
    typedef SQRESULT (*sqapi_getdelegate)(HSQUIRRELVM v,SQInteger idx);
    typedef SQRESULT (*sqapi_clone)(HSQUIRRELVM v,SQInteger idx);
    typedef SQRESULT (*sqapi_setfreevariable)(HSQUIRRELVM v,SQInteger idx,SQUnsignedInteger nval);
    typedef SQRESULT (*sqapi_next)(HSQUIRRELVM v,SQInteger idx);
    typedef SQRESULT (*sqapi_getweakrefval)(HSQUIRRELVM v,SQInteger idx);
    typedef SQRESULT (*sqapi_clear)(HSQUIRRELVM v,SQInteger idx);

    //calls
    typedef SQRESULT (*sqapi_call)(HSQUIRRELVM v,SQInteger params,SQBool retval,SQBool raiseerror);
    typedef SQRESULT (*sqapi_resume)(HSQUIRRELVM v,SQBool retval,SQBool raiseerror);
    typedef const SQChar* (*sqapi_getlocal)(HSQUIRRELVM v,SQUnsignedInteger level,SQUnsignedInteger idx);
    typedef SQRESULT (*sqapi_getcallee)(HSQUIRRELVM v);
    typedef const SQChar* (*sqapi_getfreevariable)(HSQUIRRELVM v,SQInteger idx,SQUnsignedInteger nval);
    typedef SQRESULT (*sqapi_throwerror)(HSQUIRRELVM v,const SQChar *err);
    typedef SQRESULT (*sqapi_throwobject)(HSQUIRRELVM v);
    typedef void (*sqapi_reseterror)(HSQUIRRELVM v);
    typedef void (*sqapi_getlasterror)(HSQUIRRELVM v);

    //raw object handling
    typedef SQRESULT (*sqapi_getstackobj)(HSQUIRRELVM v,SQInteger idx,HSQOBJECT *po);
    typedef void (*sqapi_pushobject)(HSQUIRRELVM v,HSQOBJECT obj);
    typedef void (*sqapi_addref)(HSQUIRRELVM v,HSQOBJECT *po);
    typedef SQBool (*sqapi_release)(HSQUIRRELVM v,HSQOBJECT *po);
    typedef SQUnsignedInteger (*sqapi_getrefcount)(HSQUIRRELVM v,HSQOBJECT *po);
    typedef void (*sqapi_resetobject)(HSQOBJECT *po);
    typedef const SQChar* (*sqapi_objtostring)(const HSQOBJECT *o);
    typedef SQBool (*sqapi_objtobool)(const HSQOBJECT *o);
    typedef SQInteger (*sqapi_objtointeger)(const HSQOBJECT *o);
    typedef SQFloat (*sqapi_objtofloat)(const HSQOBJECT *o);
    typedef SQUserPointer (*sqapi_objtouserpointer)(const HSQOBJECT *o);
    typedef SQRESULT (*sqapi_getobjtypetag)(const HSQOBJECT *o,SQUserPointer * typetag);
    typedef SQUnsignedInteger (*sqapi_getvmrefcount)(HSQUIRRELVM v, const HSQOBJECT *po);


    //GC
    typedef SQInteger (*sqapi_collectgarbage)(HSQUIRRELVM v);
    typedef SQRESULT (*sqapi_resurrectunreachable)(HSQUIRRELVM v);

    //serialization
    typedef SQRESULT (*sqapi_writeclosure)(HSQUIRRELVM vm,SQWRITEFUNC writef,SQUserPointer up);
    typedef SQRESULT (*sqapi_readclosure)(HSQUIRRELVM vm,SQREADFUNC readf,SQUserPointer up);

    //mem allocation
    typedef void* (*sqapi_malloc)(SQUnsignedInteger size);
    typedef void* (*sqapi_realloc)(void* p,SQUnsignedInteger oldsize,SQUnsignedInteger newsize);
    typedef void (*sqapi_free)(void *p,SQUnsignedInteger size);

    //debug
    typedef SQRESULT (*sqapi_stackinfos)(HSQUIRRELVM v,SQInteger level,SQStackInfos *si);
    typedef void (*sqapi_setdebughook)(HSQUIRRELVM v);
    typedef void (*sqapi_setnativedebughook)(HSQUIRRELVM v,SQDEBUGHOOK hook);

    //compiler helpers
    typedef SQRESULT (*sqapi_loadfile)(HSQUIRRELVM v,const SQChar *filename,SQBool printerror);
    typedef SQRESULT (*sqapi_dofile)(HSQUIRRELVM v,const SQChar *filename,SQBool retval,SQBool printerror);
    typedef SQRESULT (*sqapi_writeclosuretofile)(HSQUIRRELVM v,const SQChar *filename);

    //blob
    typedef SQUserPointer (*sqapi_createblob)(HSQUIRRELVM v, SQInteger size);
    typedef SQRESULT (*sqapi_getblob)(HSQUIRRELVM v,SQInteger idx,SQUserPointer *ptr);
    typedef SQInteger (*sqapi_getblobsize)(HSQUIRRELVM v,SQInteger idx);

    //string
    typedef SQRESULT (*sqapi_format)(HSQUIRRELVM v,SQInteger nformatstringidx,SQInteger *outlen,SQChar **output);

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @cond DEV
    /// Allows modules to interface with Squirrel's C api without linking to the squirrel library.
    /// If new functions are added to the Squirrel API, they should be added here too.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    typedef struct {
        //vm
        sqapi_open                          open;
        sqapi_newthread                     newthread;
        sqapi_seterrorhandler               seterrorhandler;
        sqapi_close                         close;
        sqapi_setforeignptr                 setforeignptr;
        sqapi_getforeignptr                 getforeignptr;
        sqapi_setsharedforeignptr           setsharedforeignptr;
        sqapi_getsharedforeignptr           getsharedforeignptr;
        sqapi_setvmreleasehook              setvmreleasehook;
        sqapi_getvmreleasehook              getvmreleasehook;
        sqapi_setsharedreleasehook          setsharedreleasehook;
        sqapi_getsharedreleasehook          getsharedreleasehook;
        sqapi_setprintfunc                  setprintfunc;
        sqapi_getprintfunc                  getprintfunc;
        sqapi_geterrorfunc                  geterrorfunc;
        sqapi_suspendvm                     suspendvm;
        sqapi_wakeupvm                      wakeupvm;
        sqapi_getvmstate                    getvmstate;
        sqapi_getversion                    getversion;

        //compiler
        sqapi_compile                       compile;
        sqapi_compilebuffer                 compilebuffer;
        sqapi_enabledebuginfo               enabledebuginfo;
        sqapi_notifyallexceptions           notifyallexceptions;
        sqapi_setcompilererrorhandler       setcompilererrorhandler;

        //stack operations
        sqapi_push                          push;
        sqapi_pop                           pop;
        sqapi_poptop                        poptop;
        sqapi_remove                        remove;
        sqapi_gettop                        gettop;
        sqapi_settop                        settop;
        sqapi_reservestack                  reservestack;
        sqapi_cmp                           cmp;
        sqapi_move                          move;

        //object creation handling
        sqapi_newuserdata                   newuserdata;
        sqapi_newtable                      newtable;
        sqapi_newtableex                    newtableex;
        sqapi_newarray                      newarray;
        sqapi_newclosure                    newclosure;
        sqapi_setparamscheck                setparamscheck;
        sqapi_bindenv                       bindenv;
        sqapi_setclosureroot                setclosureroot;
        sqapi_getclosureroot                getclosureroot;
        sqapi_pushstring                    pushstring;
        sqapi_pushfloat                     pushfloat;
        sqapi_pushinteger                   pushinteger;
        sqapi_pushbool                      pushbool;
        sqapi_pushuserpointer               pushuserpointer;
        sqapi_pushnull                      pushnull;
        sqapi_pushthread                    pushthread;
        sqapi_gettype                       gettype;
        sqapi_typeof                        typeof_;
        sqapi_getsize                       getsize;
        sqapi_gethash                       gethash;
        sqapi_getbase                       getbase;
        sqapi_instanceof                    instanceof;
        sqapi_tostring                      tostring;
        sqapi_tobool                        tobool;
        sqapi_getstringandsize              getstringandsize;
        sqapi_getstring                     getstring;
        sqapi_getinteger                    getinteger;
        sqapi_getfloat                      getfloat;
        sqapi_getbool                       getbool;
        sqapi_getthread                     getthread;
        sqapi_getuserpointer                getuserpointer;
        sqapi_getuserdata                   getuserdata;
        sqapi_settypetag                    settypetag;
        sqapi_gettypetag                    gettypetag;
        sqapi_setreleasehook                setreleasehook;
        sqapi_getreleasehook                getreleasehook;
        sqapi_getscratchpad                 getscratchpad;
        sqapi_getfunctioninfo               getfunctioninfo;
        sqapi_getclosureinfo                getclosureinfo;
        sqapi_getclosurename                getclosurename;
        sqapi_setnativeclosurename          setnativeclosurename;
        sqapi_setinstanceup                 setinstanceup;
        sqapi_getinstanceup                 getinstanceup;
        sqapi_setclassudsize                setclassudsize;
        sqapi_newclass                      newclass;
        sqapi_createinstance                createinstance;
        sqapi_setattributes                 setattributes;
        sqapi_getattributes                 getattributes;
        sqapi_getclass                      getclass;
        sqapi_weakref                       weakref;
        sqapi_getdefaultdelegate            getdefaultdelegate;
        sqapi_getmemberhandle               getmemberhandle;
        sqapi_getbyhandle                   getbyhandle;
        sqapi_setbyhandle                   setbyhandle;

        //object manipulation
        sqapi_pushroottable                 pushroottable;
        sqapi_pushregistrytable             pushregistrytable;
        sqapi_pushconsttable                pushconsttable;
        sqapi_setroottable                  setroottable;
        sqapi_setconsttable                 setconsttable;
        sqapi_newslot                       newslot;
        sqapi_deleteslot                    deleteslot;
        sqapi_set                           set;
        sqapi_get                           get;
        sqapi_rawget                        rawget;
        sqapi_rawset                        rawset;
        sqapi_rawdeleteslot                 rawdeleteslot;
        sqapi_newmember                     newmember;
        sqapi_rawnewmember                  rawnewmember;
        sqapi_arrayappend                   arrayappend;
        sqapi_arraypop                      arraypop;
        sqapi_arrayresize                   arrayresize;
        sqapi_arrayreverse                  arrayreverse;
        sqapi_arrayremove                   arrayremove;
        sqapi_arrayinsert                   arrayinsert;
        sqapi_setdelegate                   setdelegate;
        sqapi_getdelegate                   getdelegate;
        sqapi_clone                         clone;
        sqapi_setfreevariable               setfreevariable;
        sqapi_next                          next;
        sqapi_getweakrefval                 getweakrefval;
        sqapi_clear                         clear;

        //calls
        sqapi_call                          call;
        sqapi_resume                        resume;
        sqapi_getlocal                      getlocal;
        sqapi_getcallee                     getcallee;
        sqapi_getfreevariable               getfreevariable;
        sqapi_throwerror                    throwerror;
        sqapi_throwobject                   throwobject;
        sqapi_reseterror                    reseterror;
        sqapi_getlasterror                  getlasterror;

        //raw object handling
        sqapi_getstackobj                   getstackobj;
        sqapi_pushobject                    pushobject;
        sqapi_addref                        addref;
        sqapi_release                       release;
        sqapi_getrefcount                   getrefcount;
        sqapi_resetobject                   resetobject;
        sqapi_objtostring                   objtostring;
        sqapi_objtobool                     objtobool;
        sqapi_objtointeger                  objtointeger;
        sqapi_objtofloat                    objtofloat;
        sqapi_objtouserpointer              objtouserpointer;
        sqapi_getobjtypetag                 getobjtypetag;
        sqapi_getvmrefcount                 getvmrefcount;

        //GC
        sqapi_collectgarbage                collectgarbage;
        sqapi_resurrectunreachable          resurrectunreachable;

        //serialization
        sqapi_writeclosure                  writeclosure;
        sqapi_readclosure                   readclosure;

        //mem allocation
        sqapi_malloc                        malloc;
        sqapi_realloc                       realloc;
        sqapi_free                          free;

        //debug
        sqapi_stackinfos                    stackinfos;
        sqapi_setdebughook                  setdebughook;
        sqapi_setnativedebughook            setnativedebughook;

        //compiler helpers
        sqapi_loadfile                      loadfile;
        sqapi_dofile                        dofile;
        sqapi_writeclosuretofile            writeclosuretofile;

        //blob
        sqapi_createblob                    createblob;
        sqapi_getblob                       getblob;
        sqapi_getblobsize                   getblobsize;

        //string
        sqapi_format                        format;
    } sq_api, SQAPI, *HSQAPI;
    /// @endcond

#ifdef SQMOD_PLUGIN_API

    //vm
    extern sqapi_open                           sq_open;
    extern sqapi_newthread                      sq_newthread;
    extern sqapi_seterrorhandler                sq_seterrorhandler;
    extern sqapi_close                          sq_close;
    extern sqapi_setforeignptr                  sq_setforeignptr;
    extern sqapi_getforeignptr                  sq_getforeignptr;
    extern sqapi_setsharedforeignptr            sq_setsharedforeignptr;
    extern sqapi_getsharedforeignptr            sq_getsharedforeignptr;
    extern sqapi_setvmreleasehook               sq_setvmreleasehook;
    extern sqapi_getvmreleasehook               sq_getvmreleasehook;
    extern sqapi_setsharedreleasehook           sq_setsharedreleasehook;
    extern sqapi_getsharedreleasehook           sq_getsharedreleasehook;
    extern sqapi_setprintfunc                   sq_setprintfunc;
    extern sqapi_getprintfunc                   sq_getprintfunc;
    extern sqapi_geterrorfunc                   sq_geterrorfunc;
    extern sqapi_suspendvm                      sq_suspendvm;
    extern sqapi_wakeupvm                       sq_wakeupvm;
    extern sqapi_getvmstate                     sq_getvmstate;
    extern sqapi_getversion                     sq_getversion;

    //compiler
    extern sqapi_compile                        sq_compile;
    extern sqapi_compilebuffer                  sq_compilebuffer;
    extern sqapi_enabledebuginfo                sq_enabledebuginfo;
    extern sqapi_notifyallexceptions            sq_notifyallexceptions;
    extern sqapi_setcompilererrorhandler        sq_setcompilererrorhandler;

    //stack operations
    extern sqapi_push                           sq_push;
    extern sqapi_pop                            sq_pop;
    extern sqapi_poptop                         sq_poptop;
    extern sqapi_remove                         sq_remove;
    extern sqapi_gettop                         sq_gettop;
    extern sqapi_settop                         sq_settop;
    extern sqapi_reservestack                   sq_reservestack;
    extern sqapi_cmp                            sq_cmp;
    extern sqapi_move                           sq_move;

    //object creation handling
    extern sqapi_newuserdata                    sq_newuserdata;
    extern sqapi_newtable                       sq_newtable;
    extern sqapi_newtableex                     sq_newtableex;
    extern sqapi_newarray                       sq_newarray;
    extern sqapi_newclosure                     sq_newclosure;
    extern sqapi_setparamscheck                 sq_setparamscheck;
    extern sqapi_bindenv                        sq_bindenv;
    extern sqapi_setclosureroot                 sq_setclosureroot;
    extern sqapi_getclosureroot                 sq_getclosureroot;
    extern sqapi_pushstring                     sq_pushstring;
    extern sqapi_pushfloat                      sq_pushfloat;
    extern sqapi_pushinteger                    sq_pushinteger;
    extern sqapi_pushbool                       sq_pushbool;
    extern sqapi_pushuserpointer                sq_pushuserpointer;
    extern sqapi_pushnull                       sq_pushnull;
    extern sqapi_pushthread                     sq_pushthread;
    extern sqapi_gettype                        sq_gettype;
    extern sqapi_typeof                         sq_typeof;
    extern sqapi_getsize                        sq_getsize;
    extern sqapi_gethash                        sq_gethash;
    extern sqapi_getbase                        sq_getbase;
    extern sqapi_instanceof                     sq_instanceof;
    extern sqapi_tostring                       sq_tostring;
    extern sqapi_tobool                         sq_tobool;
    extern sqapi_getstringandsize               sq_getstringandsize;
    extern sqapi_getstring                      sq_getstring;
    extern sqapi_getinteger                     sq_getinteger;
    extern sqapi_getfloat                       sq_getfloat;
    extern sqapi_getbool                        sq_getbool;
    extern sqapi_getthread                      sq_getthread;
    extern sqapi_getuserpointer                 sq_getuserpointer;
    extern sqapi_getuserdata                    sq_getuserdata;
    extern sqapi_settypetag                     sq_settypetag;
    extern sqapi_gettypetag                     sq_gettypetag;
    extern sqapi_setreleasehook                 sq_setreleasehook;
    extern sqapi_getreleasehook                 sq_getreleasehook;
    extern sqapi_getscratchpad                  sq_getscratchpad;
    extern sqapi_getfunctioninfo                sq_getfunctioninfo;
    extern sqapi_getclosureinfo                 sq_getclosureinfo;
    extern sqapi_getclosurename                 sq_getclosurename;
    extern sqapi_setnativeclosurename           sq_setnativeclosurename;
    extern sqapi_setinstanceup                  sq_setinstanceup;
    extern sqapi_getinstanceup                  sq_getinstanceup;
    extern sqapi_setclassudsize                 sq_setclassudsize;
    extern sqapi_newclass                       sq_newclass;
    extern sqapi_createinstance                 sq_createinstance;
    extern sqapi_setattributes                  sq_setattributes;
    extern sqapi_getattributes                  sq_getattributes;
    extern sqapi_getclass                       sq_getclass;
    extern sqapi_weakref                        sq_weakref;
    extern sqapi_getdefaultdelegate             sq_getdefaultdelegate;
    extern sqapi_getmemberhandle                sq_getmemberhandle;
    extern sqapi_getbyhandle                    sq_getbyhandle;
    extern sqapi_setbyhandle                    sq_setbyhandle;

    //object manipulation
    extern sqapi_pushroottable                  sq_pushroottable;
    extern sqapi_pushregistrytable              sq_pushregistrytable;
    extern sqapi_pushconsttable                 sq_pushconsttable;
    extern sqapi_setroottable                   sq_setroottable;
    extern sqapi_setconsttable                  sq_setconsttable;
    extern sqapi_newslot                        sq_newslot;
    extern sqapi_deleteslot                     sq_deleteslot;
    extern sqapi_set                            sq_set;
    extern sqapi_get                            sq_get;
    extern sqapi_rawget                         sq_rawget;
    extern sqapi_rawset                         sq_rawset;
    extern sqapi_rawdeleteslot                  sq_rawdeleteslot;
    extern sqapi_newmember                      sq_newmember;
    extern sqapi_rawnewmember                   sq_rawnewmember;
    extern sqapi_arrayappend                    sq_arrayappend;
    extern sqapi_arraypop                       sq_arraypop;
    extern sqapi_arrayresize                    sq_arrayresize;
    extern sqapi_arrayreverse                   sq_arrayreverse;
    extern sqapi_arrayremove                    sq_arrayremove;
    extern sqapi_arrayinsert                    sq_arrayinsert;
    extern sqapi_setdelegate                    sq_setdelegate;
    extern sqapi_getdelegate                    sq_getdelegate;
    extern sqapi_clone                          sq_clone;
    extern sqapi_setfreevariable                sq_setfreevariable;
    extern sqapi_next                           sq_next;
    extern sqapi_getweakrefval                  sq_getweakrefval;
    extern sqapi_clear                          sq_clear;

    //calls
    extern sqapi_call                           sq_call;
    extern sqapi_resume                         sq_resume;
    extern sqapi_getlocal                       sq_getlocal;
    extern sqapi_getcallee                      sq_getcallee;
    extern sqapi_getfreevariable                sq_getfreevariable;
    extern sqapi_throwerror                     sq_throwerror;
    extern sqapi_throwobject                    sq_throwobject;
    extern sqapi_reseterror                     sq_reseterror;
    extern sqapi_getlasterror                   sq_getlasterror;

    //raw object handling
    extern sqapi_getstackobj                    sq_getstackobj;
    extern sqapi_pushobject                     sq_pushobject;
    extern sqapi_addref                         sq_addref;
    extern sqapi_release                        sq_release;
    extern sqapi_getrefcount                    sq_getrefcount;
    extern sqapi_resetobject                    sq_resetobject;
    extern sqapi_objtostring                    sq_objtostring;
    extern sqapi_objtobool                      sq_objtobool;
    extern sqapi_objtointeger                   sq_objtointeger;
    extern sqapi_objtofloat                     sq_objtofloat;
    extern sqapi_objtouserpointer               sq_objtouserpointer;
    extern sqapi_getobjtypetag                  sq_getobjtypetag;
    extern sqapi_getvmrefcount                  sq_getvmrefcount;

    //GC
    extern sqapi_collectgarbage                 sq_collectgarbage;
    extern sqapi_resurrectunreachable           sq_resurrectunreachable;

    //serialization
    extern sqapi_writeclosure                   sq_writeclosure;
    extern sqapi_readclosure                    sq_readclosure;

    //mem allocation
    extern sqapi_malloc                         sq_malloc;
    extern sqapi_realloc                        sq_realloc;
    extern sqapi_free                           sq_free;

    //debug
    extern sqapi_stackinfos                     sq_stackinfos;
    extern sqapi_setdebughook                   sq_setdebughook;
    extern sqapi_setnativedebughook             sq_setnativedebughook;

    //compiler helpers
    extern sqapi_loadfile                       sqstd_loadfile;
    extern sqapi_dofile                         sqstd_dofile;
    extern sqapi_writeclosuretofile             sqstd_writeclosuretofile;

    //blob
    extern sqapi_createblob                     sqstd_createblob;
    extern sqapi_getblob                        sqstd_getblob;
    extern sqapi_getblobsize                    sqstd_getblobsize;

    //string
    extern sqapi_format                         sqstd_format;

#endif // SQMOD_PLUGIN_API

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*_SQ_API_H_*/
