// ------------------------------------------------------------------------------------------------
#include "Core.hpp"

// ------------------------------------------------------------------------------------------------
#include <cstring>

// ------------------------------------------------------------------------------------------------
#include <sqstdio.h>
#include <sqstdblob.h>
#include <sqstdstring.h>
#include <SqMod.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
static HSQUIRRELVM GetSquirrelVM()
{
    return Core::Get().GetVM();
}

// ------------------------------------------------------------------------------------------------
static SQRESULT SqModImpl_LoadScript(const SQChar * filepath, SQBool delay)
{
    // Attempt to add the specified script to the load queue
    if (Core::Get().LoadScript(filepath, static_cast< bool >(delay)))
    {
        return SQ_OK; // The script as added or already existed
    }
    // The path was invalid or was unable to pool the script
    return SQ_ERROR;
}

// ------------------------------------------------------------------------------------------------
static int32_t SqExport_PopulateModuleAPI(HSQMODAPI api, size_t size)
{
    if (!api)
    {
        return 0; // Nothing to populate!
    }
    else if (size != sizeof(SQMODAPI))
    {
        return -1; // Incompatible API!
    }

    //primitive functions
    api->GetSquirrelVM              = GetSquirrelVM;

    //logging utilities
    api->LogDbg                     = LogDbg;
    api->LogUsr                     = LogUsr;
    api->LogScs                     = LogScs;
    api->LogInf                     = LogInf;
    api->LogWrn                     = LogWrn;
    api->LogErr                     = LogErr;
    api->LogFtl                     = LogFtl;
    api->LogSDbg                    = LogSDbg;
    api->LogSUsr                    = LogSUsr;
    api->LogSScs                    = LogSScs;
    api->LogSInf                    = LogSInf;
    api->LogSWrn                    = LogSWrn;
    api->LogSErr                    = LogSErr;
    api->LogSFtl                    = LogSFtl;

    //script loading
    api->LoadScript                 = SqModImpl_LoadScript;

    return 1; // Successfully populated!
}

// ------------------------------------------------------------------------------------------------
static int32_t SqExport_PopulateSquirrelAPI(HSQLIBAPI api, size_t size)
{
    if (!api)
    {
        return 0; // Nothing to populate!
    }
    else if (size != sizeof(SQLIBAPI))
    {
        return -1; // Incompatible API!
    }

    //vm
    api->open                           = sq_open;
    api->newthread                      = sq_newthread;
    api->seterrorhandler                = sq_seterrorhandler;
    api->close                          = sq_close;
    api->setforeignptr                  = sq_setforeignptr;
    api->getforeignptr                  = sq_getforeignptr;
    api->setsharedforeignptr            = sq_setsharedforeignptr;
    api->getsharedforeignptr            = sq_getsharedforeignptr;
    api->setvmreleasehook               = sq_setvmreleasehook;
    api->getvmreleasehook               = sq_getvmreleasehook;
    api->setsharedreleasehook           = sq_setsharedreleasehook;
    api->getsharedreleasehook           = sq_getsharedreleasehook;
    api->setprintfunc                   = sq_setprintfunc;
    api->getprintfunc                   = sq_getprintfunc;
    api->geterrorfunc                   = sq_geterrorfunc;
    api->suspendvm                      = sq_suspendvm;
    api->wakeupvm                       = sq_wakeupvm;
    api->getvmstate                     = sq_getvmstate;
    api->getversion                     = sq_getversion;

    //compiler
    api->compile                        = sq_compile;
    api->compilebuffer                  = sq_compilebuffer;
    api->enabledebuginfo                = sq_enabledebuginfo;
    api->notifyallexceptions            = sq_notifyallexceptions;
    api->setcompilererrorhandler        = sq_setcompilererrorhandler;

    //stack operations
    api->push                           = sq_push;
    api->pop                            = sq_pop;
    api->poptop                         = sq_poptop;
    api->remove                         = sq_remove;
    api->gettop                         = sq_gettop;
    api->settop                         = sq_settop;
    api->reservestack                   = sq_reservestack;
    api->cmp                            = sq_cmp;
    api->move                           = sq_move;

    //object creation handling
    api->newuserdata                    = sq_newuserdata;
    api->newtable                       = sq_newtable;
    api->newtableex                     = sq_newtableex;
    api->newarray                       = sq_newarray;
    api->newclosure                     = sq_newclosure;
    api->setparamscheck                 = sq_setparamscheck;
    api->bindenv                        = sq_bindenv;
    api->setclosureroot                 = sq_setclosureroot;
    api->getclosureroot                 = sq_getclosureroot;
    api->pushstring                     = sq_pushstring;
    api->pushstringf                    = sq_pushstringf;
    api->pushfloat                      = sq_pushfloat;
    api->pushinteger                    = sq_pushinteger;
    api->pushbool                       = sq_pushbool;
    api->pushuserpointer                = sq_pushuserpointer;
    api->pushnull                       = sq_pushnull;
    api->pushthread                     = sq_pushthread;
    api->gettype                        = sq_gettype;
    api->typeof_                        = sq_typeof;
    api->getsize                        = sq_getsize;
    api->gethash                        = sq_gethash;
    api->getbase                        = sq_getbase;
    api->instanceof                     = sq_instanceof;
    api->tostring                       = sq_tostring;
    api->tobool                         = sq_tobool;
    api->getstringandsize               = sq_getstringandsize;
    api->getstring                      = sq_getstring;
    api->getinteger                     = sq_getinteger;
    api->getfloat                       = sq_getfloat;
    api->getbool                        = sq_getbool;
    api->getthread                      = sq_getthread;
    api->getuserpointer                 = sq_getuserpointer;
    api->getuserdata                    = sq_getuserdata;
    api->settypetag                     = sq_settypetag;
    api->gettypetag                     = sq_gettypetag;
    api->setreleasehook                 = sq_setreleasehook;
    api->getreleasehook                 = sq_getreleasehook;
    api->getscratchpad                  = sq_getscratchpad;
    api->getfunctioninfo                = sq_getfunctioninfo;
    api->getclosureinfo                 = sq_getclosureinfo;
    api->getclosurename                 = sq_getclosurename;
    api->setnativeclosurename           = sq_setnativeclosurename;
    api->getnativeclosurepointer        = sq_getnativeclosurepointer;
    api->setinstanceup                  = sq_setinstanceup;
    api->getinstanceup                  = sq_getinstanceup;
    api->setclassudsize                 = sq_setclassudsize;
    api->newclass                       = sq_newclass;
    api->createinstance                 = sq_createinstance;
    api->setattributes                  = sq_setattributes;
    api->getattributes                  = sq_getattributes;
    api->getclass                       = sq_getclass;
    api->weakref                        = sq_weakref;
    api->getdefaultdelegate             = sq_getdefaultdelegate;
    api->getmemberhandle                = sq_getmemberhandle;
    api->getbyhandle                    = sq_getbyhandle;
    api->setbyhandle                    = sq_setbyhandle;

    //object manipulation
    api->pushroottable                  = sq_pushroottable;
    api->pushregistrytable              = sq_pushregistrytable;
    api->pushconsttable                 = sq_pushconsttable;
    api->setroottable                   = sq_setroottable;
    api->setconsttable                  = sq_setconsttable;
    api->newslot                        = sq_newslot;
    api->deleteslot                     = sq_deleteslot;
    api->set                            = sq_set;
    api->get                            = sq_get;
    api->rawget                         = sq_rawget;
    api->rawset                         = sq_rawset;
    api->rawdeleteslot                  = sq_rawdeleteslot;
    api->newmember                      = sq_newmember;
    api->rawnewmember                   = sq_rawnewmember;
    api->arrayappend                    = sq_arrayappend;
    api->arraypop                       = sq_arraypop;
    api->arrayresize                    = sq_arrayresize;
    api->arrayreverse                   = sq_arrayreverse;
    api->arrayremove                    = sq_arrayremove;
    api->arrayinsert                    = sq_arrayinsert;
    api->setdelegate                    = sq_setdelegate;
    api->getdelegate                    = sq_getdelegate;
    api->clone                          = sq_clone;
    api->setfreevariable                = sq_setfreevariable;
    api->next                           = sq_next;
    api->getweakrefval                  = sq_getweakrefval;
    api->clear                          = sq_clear;

    //calls
    api->call                           = sq_call;
    api->resume                         = sq_resume;
    api->getlocal                       = sq_getlocal;
    api->getcallee                      = sq_getcallee;
    api->getfreevariable                = sq_getfreevariable;
    api->getonefreevariable             = sq_getonefreevariable;
    api->throwerror                     = sq_throwerror;
    api->throwerrorf                    = sq_throwerrorf;
    api->throwobject                    = sq_throwobject;
    api->reseterror                     = sq_reseterror;
    api->getlasterror                   = sq_getlasterror;

    //raw object handling
    api->getstackobj                    = sq_getstackobj;
    api->pushobject                     = sq_pushobject;
    api->addref                         = sq_addref;
    api->release                        = sq_release;
    api->getrefcount                    = sq_getrefcount;
    api->resetobject                    = sq_resetobject;
    api->objtostring                    = sq_objtostring;
    api->objtobool                      = sq_objtobool;
    api->objtointeger                   = sq_objtointeger;
    api->objtofloat                     = sq_objtofloat;
    api->objtouserpointer               = sq_objtouserpointer;
    api->getobjtypetag                  = sq_getobjtypetag;
    api->getvmrefcount                  = sq_getvmrefcount;

    //GC
    api->collectgarbage                 = sq_collectgarbage;
    api->resurrectunreachable           = sq_resurrectunreachable;

    //serialization
    api->writeclosure                   = sq_writeclosure;
    api->readclosure                    = sq_readclosure;

    //mem allocation
    api->malloc                         = sq_malloc;
    api->realloc                        = sq_realloc;
    api->free                           = sq_free;

    //debug
    api->stackinfos                     = sq_stackinfos;
    api->setdebughook                   = sq_setdebughook;
    api->setnativedebughook             = sq_setnativedebughook;

    //compiler helpers
    api->loadfile                       = sqstd_loadfile;
    api->dofile                         = sqstd_dofile;
    api->writeclosuretofile             = sqstd_writeclosuretofile;

    //blob
    api->createblob                     = sqstd_createblob;
    api->getblob                        = sqstd_getblob;
    api->getblobsize                    = sqstd_getblobsize;

    //string
    api->format                         = sqstd_format;

    return 1; // Successfully populated!
}

// ------------------------------------------------------------------------------------------------
static const SQMODEXPORTS g_SqModExports{
    sizeof(SQMODEXPORTS),
    SqExport_PopulateModuleAPI,
    SqExport_PopulateSquirrelAPI
};

// ------------------------------------------------------------------------------------------------
void InitExports()
{
    // The server needs a pointer to a pointer, and a persistent one
    static const SQMODEXPORTS * sqmodexports = &g_SqModExports;

    // Tell the server about the pointer to the exports structure
    _Func->ExportFunctions(_Info->pluginId, reinterpret_cast< const void ** >(&sqmodexports),
                            sizeof(HSQMODEXPORTS));
}

} // Namespace:: SqMod
