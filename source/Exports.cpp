// ------------------------------------------------------------------------------------------------
#include "Core.hpp"

// ------------------------------------------------------------------------------------------------
#include <stdlib.h>
#include <string.h>

// ------------------------------------------------------------------------------------------------
#include <sqstdio.h>
#include <sqstdblob.h>
#include <sqstdstring.h>
#include <sq_mod.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
static SQAPI       g_SqAPI;
static SQEXPORTS   g_SqExports;

// ------------------------------------------------------------------------------------------------
static HSQAPI GetSquirrelAPI()
{
    return &g_SqAPI;
}

// ------------------------------------------------------------------------------------------------
static HSQUIRRELVM GetSquirrelVM()
{
    if (_Core)
        return _Core->GetVM();
    return NULL;
}

// ------------------------------------------------------------------------------------------------
void InitExports()
{
    static HSQEXPORTS sqexports = &g_SqExports;

    // Assign the functions that should be exported
    g_SqExports.uStructSize = sizeof(SQEXPORTS);
    g_SqExports.GetSquirrelAPI = GetSquirrelAPI;
    g_SqExports.GetSquirrelVM = GetSquirrelVM;
    // Export them to the server
    _Func->ExportFunctions(_Info->nPluginId, (void **)(&sqexports), sizeof(SQEXPORTS));
    /*vm*/
    g_SqAPI.open                        = sq_open;
    g_SqAPI.newthread                   = sq_newthread;
    g_SqAPI.seterrorhandler             = sq_seterrorhandler;
    g_SqAPI.close                       = sq_close;
    g_SqAPI.setforeignptr               = sq_setforeignptr;
    g_SqAPI.getforeignptr               = sq_getforeignptr;
    g_SqAPI.setsharedforeignptr         = sq_setsharedforeignptr;
    g_SqAPI.getsharedforeignptr         = sq_getsharedforeignptr;
    g_SqAPI.setvmreleasehook            = sq_setvmreleasehook;
    g_SqAPI.getvmreleasehook            = sq_getvmreleasehook;
    g_SqAPI.setsharedreleasehook        = sq_setsharedreleasehook;
    g_SqAPI.getsharedreleasehook        = sq_getsharedreleasehook;
    g_SqAPI.setprintfunc                = sq_setprintfunc;
    g_SqAPI.getprintfunc                = sq_getprintfunc;
    g_SqAPI.geterrorfunc                = sq_geterrorfunc;
    g_SqAPI.suspendvm                   = sq_suspendvm;
    g_SqAPI.wakeupvm                    = sq_wakeupvm;
    g_SqAPI.getvmstate                  = sq_getvmstate;
    g_SqAPI.getversion                  = sq_getversion;

    /*compiler*/
    g_SqAPI.compile                     = sq_compile;
    g_SqAPI.compilebuffer               = sq_compilebuffer;
    g_SqAPI.enabledebuginfo             = sq_enabledebuginfo;
    g_SqAPI.notifyallexceptions         = sq_notifyallexceptions;
    g_SqAPI.setcompilererrorhandler     = sq_setcompilererrorhandler;

    /*stack operations*/
    g_SqAPI.push                        = sq_push;
    g_SqAPI.pop                         = sq_pop;
    g_SqAPI.poptop                      = sq_poptop;
    g_SqAPI.remove                      = sq_remove;
    g_SqAPI.gettop                      = sq_gettop;
    g_SqAPI.settop                      = sq_settop;
    g_SqAPI.reservestack                = sq_reservestack;
    g_SqAPI.cmp                         = sq_cmp;
    g_SqAPI.move                        = sq_move;

    /*object creation handling*/
    g_SqAPI.newuserdata                 = sq_newuserdata;
    g_SqAPI.newtable                    = sq_newtable;
    g_SqAPI.newtableex                  = sq_newtableex;
    g_SqAPI.newarray                    = sq_newarray;
    g_SqAPI.newclosure                  = sq_newclosure;
    g_SqAPI.setparamscheck              = sq_setparamscheck;
    g_SqAPI.bindenv                     = sq_bindenv;
    g_SqAPI.setclosureroot              = sq_setclosureroot;
    g_SqAPI.getclosureroot              = sq_getclosureroot;
    g_SqAPI.pushstring                  = sq_pushstring;
    g_SqAPI.pushfloat                   = sq_pushfloat;
    g_SqAPI.pushinteger                 = sq_pushinteger;
    g_SqAPI.pushbool                    = sq_pushbool;
    g_SqAPI.pushuserpointer             = sq_pushuserpointer;
    g_SqAPI.pushnull                    = sq_pushnull;
    g_SqAPI.pushthread                  = sq_pushthread;
    g_SqAPI.gettype                     = sq_gettype;
    g_SqAPI.typeof_                     = sq_typeof;
    g_SqAPI.getsize                     = sq_getsize;
    g_SqAPI.gethash                     = sq_gethash;
    g_SqAPI.getbase                     = sq_getbase;
    g_SqAPI.instanceof                  = sq_instanceof;
    g_SqAPI.tostring                    = sq_tostring;
    g_SqAPI.tobool                      = sq_tobool;
    g_SqAPI.getstring                   = sq_getstring;
    g_SqAPI.getinteger                  = sq_getinteger;
    g_SqAPI.getfloat                    = sq_getfloat;
    g_SqAPI.getbool                     = sq_getbool;
    g_SqAPI.getthread                   = sq_getthread;
    g_SqAPI.getuserpointer              = sq_getuserpointer;
    g_SqAPI.getuserdata                 = sq_getuserdata;
    g_SqAPI.settypetag                  = sq_settypetag;
    g_SqAPI.gettypetag                  = sq_gettypetag;
    g_SqAPI.setreleasehook              = sq_setreleasehook;
    g_SqAPI.getreleasehook              = sq_getreleasehook;
    g_SqAPI.getscratchpad               = sq_getscratchpad;
    g_SqAPI.getfunctioninfo             = sq_getfunctioninfo;
    g_SqAPI.getclosureinfo              = sq_getclosureinfo;
    g_SqAPI.getclosurename              = sq_getclosurename;
    g_SqAPI.setnativeclosurename        = sq_setnativeclosurename;
    g_SqAPI.setinstanceup               = sq_setinstanceup;
    g_SqAPI.getinstanceup               = sq_getinstanceup;
    g_SqAPI.setclassudsize              = sq_setclassudsize;
    g_SqAPI.newclass                    = sq_newclass;
    g_SqAPI.createinstance              = sq_createinstance;
    g_SqAPI.setattributes               = sq_setattributes;
    g_SqAPI.getattributes               = sq_getattributes;
    g_SqAPI.getclass                    = sq_getclass;
    g_SqAPI.weakref                     = sq_weakref;
    g_SqAPI.getdefaultdelegate          = sq_getdefaultdelegate;
    g_SqAPI.getmemberhandle             = sq_getmemberhandle;
    g_SqAPI.getbyhandle                 = sq_getbyhandle;
    g_SqAPI.setbyhandle                 = sq_setbyhandle;

    /*object manipulation*/
    g_SqAPI.pushroottable               = sq_pushroottable;
    g_SqAPI.pushregistrytable           = sq_pushregistrytable;
    g_SqAPI.pushconsttable              = sq_pushconsttable;
    g_SqAPI.setroottable                = sq_setroottable;
    g_SqAPI.setconsttable               = sq_setconsttable;
    g_SqAPI.newslot                     = sq_newslot;
    g_SqAPI.deleteslot                  = sq_deleteslot;
    g_SqAPI.set                         = sq_set;
    g_SqAPI.get                         = sq_get;
    g_SqAPI.rawget                      = sq_rawget;
    g_SqAPI.rawset                      = sq_rawset;
    g_SqAPI.rawdeleteslot               = sq_rawdeleteslot;
    g_SqAPI.newmember                   = sq_newmember;
    g_SqAPI.rawnewmember                = sq_rawnewmember;
    g_SqAPI.arrayappend                 = sq_arrayappend;
    g_SqAPI.arraypop                    = sq_arraypop;
    g_SqAPI.arrayresize                 = sq_arrayresize;
    g_SqAPI.arrayreverse                = sq_arrayreverse;
    g_SqAPI.arrayremove                 = sq_arrayremove;
    g_SqAPI.arrayinsert                 = sq_arrayinsert;
    g_SqAPI.setdelegate                 = sq_setdelegate;
    g_SqAPI.getdelegate                 = sq_getdelegate;
    g_SqAPI.clone                       = sq_clone;
    g_SqAPI.setfreevariable             = sq_setfreevariable;
    g_SqAPI.next                        = sq_next;
    g_SqAPI.getweakrefval               = sq_getweakrefval;
    g_SqAPI.clear                       = sq_clear;

    /*calls*/
    g_SqAPI.call                        = sq_call;
    g_SqAPI.resume                      = sq_resume;
    g_SqAPI.getlocal                    = sq_getlocal;
    g_SqAPI.getcallee                   = sq_getcallee;
    g_SqAPI.getfreevariable             = sq_getfreevariable;
    g_SqAPI.throwerror                  = sq_throwerror;
    g_SqAPI.throwobject                 = sq_throwobject;
    g_SqAPI.reseterror                  = sq_reseterror;
    g_SqAPI.getlasterror                = sq_getlasterror;

    /*raw object handling*/
    g_SqAPI.getstackobj                 = sq_getstackobj;
    g_SqAPI.pushobject                  = sq_pushobject;
    g_SqAPI.addref                      = sq_addref;
    g_SqAPI.release                     = sq_release;
    g_SqAPI.getrefcount                 = sq_getrefcount;
    g_SqAPI.resetobject                 = sq_resetobject;
    g_SqAPI.objtostring                 = sq_objtostring;
    g_SqAPI.objtobool                   = sq_objtobool;
    g_SqAPI.objtointeger                = sq_objtointeger;
    g_SqAPI.objtofloat                  = sq_objtofloat;
    g_SqAPI.objtouserpointer            = sq_objtouserpointer;
    g_SqAPI.getobjtypetag               = sq_getobjtypetag;
    g_SqAPI.getvmrefcount               = sq_getvmrefcount;

    /*GC*/
    g_SqAPI.collectgarbage              = sq_collectgarbage;
    g_SqAPI.resurrectunreachable        = sq_resurrectunreachable;

    /*serialization*/
    g_SqAPI.writeclosure                = sq_writeclosure;
    g_SqAPI.readclosure                 = sq_readclosure;

    /*mem allocation*/
    g_SqAPI.malloc                      = sq_malloc;
    g_SqAPI.realloc                     = sq_realloc;
    g_SqAPI.free                        = sq_free;

    /*debug*/
    g_SqAPI.stackinfos                  = sq_stackinfos;
    g_SqAPI.setdebughook                = sq_setdebughook;
    g_SqAPI.setnativedebughook          = sq_setnativedebughook;

    /*compiler helpers*/
    g_SqAPI.loadfile                    = sqstd_loadfile;
    g_SqAPI.dofile                      = sqstd_dofile;
    g_SqAPI.writeclosuretofile          = sqstd_writeclosuretofile;

    /*blob*/
    g_SqAPI.createblob                  = sqstd_createblob;
    g_SqAPI.getblob                     = sqstd_getblob;
    g_SqAPI.getblobsize                 = sqstd_getblobsize;

    /*string*/
    g_SqAPI.format                      = sqstd_format;
}

} // Namespace:: SqMod
