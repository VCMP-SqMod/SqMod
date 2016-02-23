// ------------------------------------------------------------------------------------------------
#ifdef SQMOD_PLUGIN_API

/*vm*/
sqapi_open                          sq_open = NULL;
sqapi_newthread                     sq_newthread = NULL;
sqapi_seterrorhandler               sq_seterrorhandler = NULL;
sqapi_close                         sq_close = NULL;
sqapi_setforeignptr                 sq_setforeignptr = NULL;
sqapi_getforeignptr                 sq_getforeignptr = NULL;
sqapi_setsharedforeignptr           sq_setsharedforeignptr = NULL;
sqapi_getsharedforeignptr           sq_getsharedforeignptr = NULL;
sqapi_setvmreleasehook              sq_setvmreleasehook = NULL;
sqapi_getvmreleasehook              sq_getvmreleasehook = NULL;
sqapi_setsharedreleasehook          sq_setsharedreleasehook = NULL;
sqapi_getsharedreleasehook          sq_getsharedreleasehook = NULL;
sqapi_setprintfunc                  sq_setprintfunc = NULL;
sqapi_getprintfunc                  sq_getprintfunc = NULL;
sqapi_geterrorfunc                  sq_geterrorfunc = NULL;
sqapi_suspendvm                     sq_suspendvm = NULL;
sqapi_wakeupvm                      sq_wakeupvm = NULL;
sqapi_getvmstate                    sq_getvmstate = NULL;
sqapi_getversion                    sq_getversion = NULL;

/*compiler*/
sqapi_compile                       sq_compile = NULL;
sqapi_compilebuffer                 sq_compilebuffer = NULL;
sqapi_enabledebuginfo               sq_enabledebuginfo = NULL;
sqapi_notifyallexceptions           sq_notifyallexceptions = NULL;
sqapi_setcompilererrorhandler       sq_setcompilererrorhandler = NULL;

/*stack operations*/
sqapi_push                          sq_push = NULL;
sqapi_pop                           sq_pop = NULL;
sqapi_poptop                        sq_poptop = NULL;
sqapi_remove                        sq_remove = NULL;
sqapi_gettop                        sq_gettop = NULL;
sqapi_settop                        sq_settop = NULL;
sqapi_reservestack                  sq_reservestack = NULL;
sqapi_cmp                           sq_cmp = NULL;
sqapi_move                          sq_move = NULL;

/*object creation handling*/
sqapi_newuserdata                   sq_newuserdata = NULL;
sqapi_newtable                      sq_newtable = NULL;
sqapi_newtableex                    sq_newtableex = NULL;
sqapi_newarray                      sq_newarray = NULL;
sqapi_newclosure                    sq_newclosure = NULL;
sqapi_setparamscheck                sq_setparamscheck = NULL;
sqapi_bindenv                       sq_bindenv = NULL;
sqapi_setclosureroot                sq_setclosureroot = NULL;
sqapi_getclosureroot                sq_getclosureroot = NULL;
sqapi_pushstring                    sq_pushstring = NULL;
sqapi_pushfloat                     sq_pushfloat = NULL;
sqapi_pushinteger                   sq_pushinteger = NULL;
sqapi_pushbool                      sq_pushbool = NULL;
sqapi_pushuserpointer               sq_pushuserpointer = NULL;
sqapi_pushnull                      sq_pushnull = NULL;
sqapi_pushthread                    sq_pushthread = NULL;
sqapi_gettype                       sq_gettype = NULL;
sqapi_typeof                        sq_typeof = NULL;
sqapi_getsize                       sq_getsize = NULL;
sqapi_gethash                       sq_gethash = NULL;
sqapi_getbase                       sq_getbase = NULL;
sqapi_instanceof                    sq_instanceof = NULL;
sqapi_tostring                      sq_tostring = NULL;
sqapi_tobool                        sq_tobool = NULL;
sqapi_getstring                     sq_getstring = NULL;
sqapi_getinteger                    sq_getinteger = NULL;
sqapi_getfloat                      sq_getfloat = NULL;
sqapi_getbool                       sq_getbool = NULL;
sqapi_getthread                     sq_getthread = NULL;
sqapi_getuserpointer                sq_getuserpointer = NULL;
sqapi_getuserdata                   sq_getuserdata = NULL;
sqapi_settypetag                    sq_settypetag = NULL;
sqapi_gettypetag                    sq_gettypetag = NULL;
sqapi_setreleasehook                sq_setreleasehook = NULL;
sqapi_getreleasehook                sq_getreleasehook = NULL;
sqapi_getscratchpad                 sq_getscratchpad = NULL;
sqapi_getfunctioninfo               sq_getfunctioninfo = NULL;
sqapi_getclosureinfo                sq_getclosureinfo = NULL;
sqapi_getclosurename                sq_getclosurename = NULL;
sqapi_setnativeclosurename          sq_setnativeclosurename = NULL;
sqapi_setinstanceup                 sq_setinstanceup = NULL;
sqapi_getinstanceup                 sq_getinstanceup = NULL;
sqapi_setclassudsize                sq_setclassudsize = NULL;
sqapi_newclass                      sq_newclass = NULL;
sqapi_createinstance                sq_createinstance = NULL;
sqapi_setattributes                 sq_setattributes = NULL;
sqapi_getattributes                 sq_getattributes = NULL;
sqapi_getclass                      sq_getclass = NULL;
sqapi_weakref                       sq_weakref = NULL;
sqapi_getdefaultdelegate            sq_getdefaultdelegate = NULL;
sqapi_getmemberhandle               sq_getmemberhandle = NULL;
sqapi_getbyhandle                   sq_getbyhandle = NULL;
sqapi_setbyhandle                   sq_setbyhandle = NULL;

/*object manipulation*/
sqapi_pushroottable                 sq_pushroottable = NULL;
sqapi_pushregistrytable             sq_pushregistrytable = NULL;
sqapi_pushconsttable                sq_pushconsttable = NULL;
sqapi_setroottable                  sq_setroottable = NULL;
sqapi_setconsttable                 sq_setconsttable = NULL;
sqapi_newslot                       sq_newslot = NULL;
sqapi_deleteslot                    sq_deleteslot = NULL;
sqapi_set                           sq_set = NULL;
sqapi_get                           sq_get = NULL;
sqapi_rawget                        sq_rawget = NULL;
sqapi_rawset                        sq_rawset = NULL;
sqapi_rawdeleteslot                 sq_rawdeleteslot = NULL;
sqapi_newmember                     sq_newmember = NULL;
sqapi_rawnewmember                  sq_rawnewmember = NULL;
sqapi_arrayappend                   sq_arrayappend = NULL;
sqapi_arraypop                      sq_arraypop = NULL;
sqapi_arrayresize                   sq_arrayresize = NULL;
sqapi_arrayreverse                  sq_arrayreverse = NULL;
sqapi_arrayremove                   sq_arrayremove = NULL;
sqapi_arrayinsert                   sq_arrayinsert = NULL;
sqapi_setdelegate                   sq_setdelegate = NULL;
sqapi_getdelegate                   sq_getdelegate = NULL;
sqapi_clone                         sq_clone = NULL;
sqapi_setfreevariable               sq_setfreevariable = NULL;
sqapi_next                          sq_next = NULL;
sqapi_getweakrefval                 sq_getweakrefval = NULL;
sqapi_clear                         sq_clear = NULL;

/*calls*/
sqapi_call                          sq_call = NULL;
sqapi_resume                        sq_resume = NULL;
sqapi_getlocal                      sq_getlocal = NULL;
sqapi_getcallee                     sq_getcallee = NULL;
sqapi_getfreevariable               sq_getfreevariable = NULL;
sqapi_throwerror                    sq_throwerror = NULL;
sqapi_throwobject                   sq_throwobject = NULL;
sqapi_reseterror                    sq_reseterror = NULL;
sqapi_getlasterror                  sq_getlasterror = NULL;

/*raw object handling*/
sqapi_getstackobj                   sq_getstackobj = NULL;
sqapi_pushobject                    sq_pushobject = NULL;
sqapi_addref                        sq_addref = NULL;
sqapi_release                       sq_release = NULL;
sqapi_getrefcount                   sq_getrefcount = NULL;
sqapi_resetobject                   sq_resetobject = NULL;
sqapi_objtostring                   sq_objtostring = NULL;
sqapi_objtobool                     sq_objtobool = NULL;
sqapi_objtointeger                  sq_objtointeger = NULL;
sqapi_objtofloat                    sq_objtofloat = NULL;
sqapi_objtouserpointer              sq_objtouserpointer = NULL;
sqapi_getobjtypetag                 sq_getobjtypetag = NULL;
sqapi_getvmrefcount                 sq_getvmrefcount = NULL;

/*GC*/
sqapi_collectgarbage                sq_collectgarbage = NULL;
sqapi_resurrectunreachable          sq_resurrectunreachable = NULL;

/*serialization*/
sqapi_writeclosure                  sq_writeclosure = NULL;
sqapi_readclosure                   sq_readclosure = NULL;

/*mem allocation*/
sqapi_malloc                        sq_malloc = NULL;
sqapi_realloc                       sq_realloc = NULL;
sqapi_free                          sq_free = NULL;

/*debug*/
sqapi_stackinfos                    sq_stackinfos = NULL;
sqapi_setdebughook                  sq_setdebughook = NULL;
sqapi_setnativedebughook            sq_setnativedebughook = NULL;

/*compiler helpers*/
sqapi_loadfile                      sq_loadfile = NULL;
sqapi_dofile                        sq_dofile = NULL;
sqapi_writeclosuretofile            sq_writeclosuretofile = NULL;

/*blob*/
sqapi_createblob                    sq_createblob = NULL;
sqapi_getblob                       sq_getblob = NULL;
sqapi_getblobsize                   sq_getblobsize = NULL;

/*string*/
sqapi_format                        sq_format = NULL;

#endif // SQMOD_PLUGIN_API

// ------------------------------------------------------------------------------------------------
HSQEXPORTS sq_api_import(PluginFuncs * vcapi)
{
    // Make sure a valid plugin api and reference to exports structure pointer was specified
    if (!vcapi)
    {
        return NULL;
    }
    unsigned int struct_size;
    // Attempt to find the main plugin ID
    int plugin_id = vcapi->FindPlugin((char *)(SQMOD_HOST_NAME));
    // Attempt to retrieve the plugin exports
    void ** plugin_exports = vcapi->GetPluginExports(plugin_id, &struct_size);
    // See if we have any imports from Squirrel
    if (plugin_exports == NULL || struct_size <= 0)
    {
        return NULL;
    }
    // Simply return the obtain poiner to the exports structure
    return *(sq_exports **)plugin_exports;
}

// ------------------------------------------------------------------------------------------------
SQRESULT sq_api_expand(HSQAPI sqapi)
{
    if (!sqapi)
        return SQ_ERROR;

#ifdef SQMOD_PLUGIN_API

    /*vm*/
    sq_open                         = sqapi->open;
    sq_newthread                    = sqapi->newthread;
    sq_seterrorhandler              = sqapi->seterrorhandler;
    sq_close                        = sqapi->close;
    sq_setforeignptr                = sqapi->setforeignptr;
    sq_getforeignptr                = sqapi->getforeignptr;
    sq_setsharedforeignptr          = sqapi->setsharedforeignptr;
    sq_getsharedforeignptr          = sqapi->getsharedforeignptr;
    sq_setvmreleasehook             = sqapi->setvmreleasehook;
    sq_getvmreleasehook             = sqapi->getvmreleasehook;
    sq_setsharedreleasehook         = sqapi->setsharedreleasehook;
    sq_getsharedreleasehook         = sqapi->getsharedreleasehook;
    sq_setprintfunc                 = sqapi->setprintfunc;
    sq_getprintfunc                 = sqapi->getprintfunc;
    sq_geterrorfunc                 = sqapi->geterrorfunc;
    sq_suspendvm                    = sqapi->suspendvm;
    sq_wakeupvm                     = sqapi->wakeupvm;
    sq_getvmstate                   = sqapi->getvmstate;
    sq_getversion                   = sqapi->getversion;

    /*compiler*/
    sq_compile                      = sqapi->compile;
    sq_compilebuffer                = sqapi->compilebuffer;
    sq_enabledebuginfo              = sqapi->enabledebuginfo;
    sq_notifyallexceptions          = sqapi->notifyallexceptions;
    sq_setcompilererrorhandler      = sqapi->setcompilererrorhandler;

    /*stack operations*/
    sq_push                         = sqapi->push;
    sq_pop                          = sqapi->pop;
    sq_poptop                       = sqapi->poptop;
    sq_remove                       = sqapi->remove;
    sq_gettop                       = sqapi->gettop;
    sq_settop                       = sqapi->settop;
    sq_reservestack                 = sqapi->reservestack;
    sq_cmp                          = sqapi->cmp;
    sq_move                         = sqapi->move;

    /*object creation handling*/
    sq_newuserdata                  = sqapi->newuserdata;
    sq_newtable                     = sqapi->newtable;
    sq_newtableex                   = sqapi->newtableex;
    sq_newarray                     = sqapi->newarray;
    sq_newclosure                   = sqapi->newclosure;
    sq_setparamscheck               = sqapi->setparamscheck;
    sq_bindenv                      = sqapi->bindenv;
    sq_setclosureroot               = sqapi->setclosureroot;
    sq_getclosureroot               = sqapi->getclosureroot;
    sq_pushstring                   = sqapi->pushstring;
    sq_pushfloat                    = sqapi->pushfloat;
    sq_pushinteger                  = sqapi->pushinteger;
    sq_pushbool                     = sqapi->pushbool;
    sq_pushuserpointer              = sqapi->pushuserpointer;
    sq_pushnull                     = sqapi->pushnull;
    sq_pushthread                   = sqapi->pushthread;
    sq_gettype                      = sqapi->gettype;
    sq_typeof                       = sqapi->typeof_;
    sq_getsize                      = sqapi->getsize;
    sq_gethash                      = sqapi->gethash;
    sq_getbase                      = sqapi->getbase;
    sq_instanceof                   = sqapi->instanceof;
    sq_tostring                     = sqapi->tostring;
    sq_tobool                       = sqapi->tobool;
    sq_getstring                    = sqapi->getstring;
    sq_getinteger                   = sqapi->getinteger;
    sq_getfloat                     = sqapi->getfloat;
    sq_getbool                      = sqapi->getbool;
    sq_getthread                    = sqapi->getthread;
    sq_getuserpointer               = sqapi->getuserpointer;
    sq_getuserdata                  = sqapi->getuserdata;
    sq_settypetag                   = sqapi->settypetag;
    sq_gettypetag                   = sqapi->gettypetag;
    sq_setreleasehook               = sqapi->setreleasehook;
    sq_getreleasehook               = sqapi->getreleasehook;
    sq_getscratchpad                = sqapi->getscratchpad;
    sq_getfunctioninfo              = sqapi->getfunctioninfo;
    sq_getclosureinfo               = sqapi->getclosureinfo;
    sq_getclosurename               = sqapi->getclosurename;
    sq_setnativeclosurename         = sqapi->setnativeclosurename;
    sq_setinstanceup                = sqapi->setinstanceup;
    sq_getinstanceup                = sqapi->getinstanceup;
    sq_setclassudsize               = sqapi->setclassudsize;
    sq_newclass                     = sqapi->newclass;
    sq_createinstance               = sqapi->createinstance;
    sq_setattributes                = sqapi->setattributes;
    sq_getattributes                = sqapi->getattributes;
    sq_getclass                     = sqapi->getclass;
    sq_weakref                      = sqapi->weakref;
    sq_getdefaultdelegate           = sqapi->getdefaultdelegate;
    sq_getmemberhandle              = sqapi->getmemberhandle;
    sq_getbyhandle                  = sqapi->getbyhandle;
    sq_setbyhandle                  = sqapi->setbyhandle;

    /*object manipulation*/
    sq_pushroottable                = sqapi->pushroottable;
    sq_pushregistrytable            = sqapi->pushregistrytable;
    sq_pushconsttable               = sqapi->pushconsttable;
    sq_setroottable                 = sqapi->setroottable;
    sq_setconsttable                = sqapi->setconsttable;
    sq_newslot                      = sqapi->newslot;
    sq_deleteslot                   = sqapi->deleteslot;
    sq_set                          = sqapi->set;
    sq_get                          = sqapi->get;
    sq_rawget                       = sqapi->rawget;
    sq_rawset                       = sqapi->rawset;
    sq_rawdeleteslot                = sqapi->rawdeleteslot;
    sq_newmember                    = sqapi->newmember;
    sq_rawnewmember                 = sqapi->rawnewmember;
    sq_arrayappend                  = sqapi->arrayappend;
    sq_arraypop                     = sqapi->arraypop;
    sq_arrayresize                  = sqapi->arrayresize;
    sq_arrayreverse                 = sqapi->arrayreverse;
    sq_arrayremove                  = sqapi->arrayremove;
    sq_arrayinsert                  = sqapi->arrayinsert;
    sq_setdelegate                  = sqapi->setdelegate;
    sq_getdelegate                  = sqapi->getdelegate;
    sq_clone                        = sqapi->clone;
    sq_setfreevariable              = sqapi->setfreevariable;
    sq_next                         = sqapi->next;
    sq_getweakrefval                = sqapi->getweakrefval;
    sq_clear                        = sqapi->clear;

    /*calls*/
    sq_call                         = sqapi->call;
    sq_resume                       = sqapi->resume;
    sq_getlocal                     = sqapi->getlocal;
    sq_getcallee                    = sqapi->getcallee;
    sq_getfreevariable              = sqapi->getfreevariable;
    sq_throwerror                   = sqapi->throwerror;
    sq_throwobject                  = sqapi->throwobject;
    sq_reseterror                   = sqapi->reseterror;
    sq_getlasterror                 = sqapi->getlasterror;

    /*raw object handling*/
    sq_getstackobj                  = sqapi->getstackobj;
    sq_pushobject                   = sqapi->pushobject;
    sq_addref                       = sqapi->addref;
    sq_release                      = sqapi->release;
    sq_getrefcount                  = sqapi->getrefcount;
    sq_resetobject                  = sqapi->resetobject;
    sq_objtostring                  = sqapi->objtostring;
    sq_objtobool                    = sqapi->objtobool;
    sq_objtointeger                 = sqapi->objtointeger;
    sq_objtofloat                   = sqapi->objtofloat;
    sq_objtouserpointer             = sqapi->objtouserpointer;
    sq_getobjtypetag                = sqapi->getobjtypetag;
    sq_getvmrefcount                = sqapi->getvmrefcount;

    /*GC*/
    sq_collectgarbage               = sqapi->collectgarbage;
    sq_resurrectunreachable         = sqapi->resurrectunreachable;

    /*serialization*/
    sq_writeclosure                 = sqapi->writeclosure;
    sq_readclosure                  = sqapi->readclosure;

    /*mem allocation*/
    sq_malloc                       = sqapi->malloc;
    sq_realloc                      = sqapi->realloc;
    sq_free                         = sqapi->free;

    /*debug*/
    sq_stackinfos                   = sqapi->stackinfos;
    sq_setdebughook                 = sqapi->setdebughook;
    sq_setnativedebughook           = sqapi->setnativedebughook;

    /*compiler helpers*/
    sq_loadfile                     = sqapi->loadfile;
    sq_dofile                       = sqapi->dofile;
    sq_writeclosuretofile           = sqapi->writeclosuretofile;

    /*blob*/
    sq_createblob                   = sqapi->createblob;
    sq_getblob                      = sqapi->getblob;
    sq_getblobsize                  = sqapi->getblobsize;

    /*string*/
    sq_format                       = sqapi->format;

#endif // SQMOD_PLUGIN_API

    return SQ_OK;
}

// ------------------------------------------------------------------------------------------------
void sq_api_collapse()
{
#ifdef SQMOD_PLUGIN_API

    /*vm*/
    sq_open                         = NULL;
    sq_newthread                    = NULL;
    sq_seterrorhandler              = NULL;
    sq_close                        = NULL;
    sq_setforeignptr                = NULL;
    sq_getforeignptr                = NULL;
    sq_setsharedforeignptr          = NULL;
    sq_getsharedforeignptr          = NULL;
    sq_setvmreleasehook             = NULL;
    sq_getvmreleasehook             = NULL;
    sq_setsharedreleasehook         = NULL;
    sq_getsharedreleasehook         = NULL;
    sq_setprintfunc                 = NULL;
    sq_getprintfunc                 = NULL;
    sq_geterrorfunc                 = NULL;
    sq_suspendvm                    = NULL;
    sq_wakeupvm                     = NULL;
    sq_getvmstate                   = NULL;
    sq_getversion                   = NULL;

    /*compiler*/
    sq_compile                      = NULL;
    sq_compilebuffer                = NULL;
    sq_enabledebuginfo              = NULL;
    sq_notifyallexceptions          = NULL;
    sq_setcompilererrorhandler      = NULL;

    /*stack operations*/
    sq_push                         = NULL;
    sq_pop                          = NULL;
    sq_poptop                       = NULL;
    sq_remove                       = NULL;
    sq_gettop                       = NULL;
    sq_settop                       = NULL;
    sq_reservestack                 = NULL;
    sq_cmp                          = NULL;
    sq_move                         = NULL;

    /*object creation handling*/
    sq_newuserdata                  = NULL;
    sq_newtable                     = NULL;
    sq_newtableex                   = NULL;
    sq_newarray                     = NULL;
    sq_newclosure                   = NULL;
    sq_setparamscheck               = NULL;
    sq_bindenv                      = NULL;
    sq_setclosureroot               = NULL;
    sq_getclosureroot               = NULL;
    sq_pushstring                   = NULL;
    sq_pushfloat                    = NULL;
    sq_pushinteger                  = NULL;
    sq_pushbool                     = NULL;
    sq_pushuserpointer              = NULL;
    sq_pushnull                     = NULL;
    sq_pushthread                   = NULL;
    sq_gettype                      = NULL;
    sq_typeof                       = NULL;
    sq_getsize                      = NULL;
    sq_gethash                      = NULL;
    sq_getbase                      = NULL;
    sq_instanceof                   = NULL;
    sq_tostring                     = NULL;
    sq_tobool                       = NULL;
    sq_getstring                    = NULL;
    sq_getinteger                   = NULL;
    sq_getfloat                     = NULL;
    sq_getbool                      = NULL;
    sq_getthread                    = NULL;
    sq_getuserpointer               = NULL;
    sq_getuserdata                  = NULL;
    sq_settypetag                   = NULL;
    sq_gettypetag                   = NULL;
    sq_setreleasehook               = NULL;
    sq_getreleasehook               = NULL;
    sq_getscratchpad                = NULL;
    sq_getfunctioninfo              = NULL;
    sq_getclosureinfo               = NULL;
    sq_getclosurename               = NULL;
    sq_setnativeclosurename         = NULL;
    sq_setinstanceup                = NULL;
    sq_getinstanceup                = NULL;
    sq_setclassudsize               = NULL;
    sq_newclass                     = NULL;
    sq_createinstance               = NULL;
    sq_setattributes                = NULL;
    sq_getattributes                = NULL;
    sq_getclass                     = NULL;
    sq_weakref                      = NULL;
    sq_getdefaultdelegate           = NULL;
    sq_getmemberhandle              = NULL;
    sq_getbyhandle                  = NULL;
    sq_setbyhandle                  = NULL;

    /*object manipulation*/
    sq_pushroottable                = NULL;
    sq_pushregistrytable            = NULL;
    sq_pushconsttable               = NULL;
    sq_setroottable                 = NULL;
    sq_setconsttable                = NULL;
    sq_newslot                      = NULL;
    sq_deleteslot                   = NULL;
    sq_set                          = NULL;
    sq_get                          = NULL;
    sq_rawget                       = NULL;
    sq_rawset                       = NULL;
    sq_rawdeleteslot                = NULL;
    sq_newmember                    = NULL;
    sq_rawnewmember                 = NULL;
    sq_arrayappend                  = NULL;
    sq_arraypop                     = NULL;
    sq_arrayresize                  = NULL;
    sq_arrayreverse                 = NULL;
    sq_arrayremove                  = NULL;
    sq_arrayinsert                  = NULL;
    sq_setdelegate                  = NULL;
    sq_getdelegate                  = NULL;
    sq_clone                        = NULL;
    sq_setfreevariable              = NULL;
    sq_next                         = NULL;
    sq_getweakrefval                = NULL;
    sq_clear                        = NULL;

    /*calls*/
    sq_call                         = NULL;
    sq_resume                       = NULL;
    sq_getlocal                     = NULL;
    sq_getcallee                    = NULL;
    sq_getfreevariable              = NULL;
    sq_throwerror                   = NULL;
    sq_throwobject                  = NULL;
    sq_reseterror                   = NULL;
    sq_getlasterror                 = NULL;

    /*raw object handling*/
    sq_getstackobj                  = NULL;
    sq_pushobject                   = NULL;
    sq_addref                       = NULL;
    sq_release                      = NULL;
    sq_getrefcount                  = NULL;
    sq_resetobject                  = NULL;
    sq_objtostring                  = NULL;
    sq_objtobool                    = NULL;
    sq_objtointeger                 = NULL;
    sq_objtofloat                   = NULL;
    sq_objtouserpointer             = NULL;
    sq_getobjtypetag                = NULL;
    sq_getvmrefcount                = NULL;

    /*GC*/
    sq_collectgarbage               = NULL;
    sq_resurrectunreachable         = NULL;

    /*serialization*/
    sq_writeclosure                 = NULL;
    sq_readclosure                  = NULL;

    /*mem allocation*/
    sq_malloc                       = NULL;
    sq_realloc                      = NULL;
    sq_free                         = NULL;

    /*debug*/
    sq_stackinfos                   = NULL;
    sq_setdebughook                 = NULL;
    sq_setnativedebughook           = NULL;

    /*compiler helpers*/
    sq_loadfile                     = NULL;
    sq_dofile                       = NULL;
    sq_writeclosuretofile           = NULL;

    /*blob*/
    sq_createblob                   = NULL;
    sq_getblob                      = NULL;
    sq_getblobsize                  = NULL;

    /*string*/
    sq_format                       = NULL;

#endif // SQMOD_PLUGIN_API
}

// --------------------------------------------------------------------------------------------
//PLUGIN SYSTEM
SDK_GetServerVersion                    SqVC_GetServerVersion = NULL;
SDK_GetServerSettings                   SqVC_GetServerSettings = NULL;
SDK_ExportFunctions                     SqVC_ExportFunctions = NULL;
SDK_GetNumberOfPlugins                  SqVC_GetNumberOfPlugins = NULL;
SDK_GetPluginInfo                       SqVC_GetPluginInfo = NULL;
SDK_FindPlugin                          SqVC_FindPlugin = NULL;
SDK_GetPluginExports                    SqVC_GetPluginExports = NULL;
SDK_GetTime                             SqVC_GetTime = NULL;

// --------------------------------------------------------------------------------------------
//MESSAGES
SDK_printf                              SqVC_printf = NULL;
SDK_SendCustomCommand                   SqVC_SendCustomCommand = NULL;
SDK_SendClientMessage                   SqVC_SendClientMessage = NULL;
SDK_SendGameMessage                     SqVC_SendGameMessage = NULL;

// --------------------------------------------------------------------------------------------
//SERVER SETTINGS
SDK_SetServerName                       SqVC_SetServerName = NULL;
SDK_GetServerName                       SqVC_GetServerName = NULL;
SDK_SetMaxPlayers                       SqVC_SetMaxPlayers = NULL;
SDK_GetMaxPlayers                       SqVC_GetMaxPlayers = NULL;
SDK_SetServerPassword                   SqVC_SetServerPassword = NULL;
SDK_GetServerPassword                   SqVC_GetServerPassword = NULL;
SDK_SetGameModeText                     SqVC_SetGameModeText = NULL;
SDK_GetGameModeText                     SqVC_GetGameModeText = NULL;
SDK_ShutdownServer                      SqVC_ShutdownServer = NULL;

// --------------------------------------------------------------------------------------------
//WORLD: settings
SDK_SetWorldBounds                      SqVC_SetWorldBounds = NULL;
SDK_GetWorldBounds                      SqVC_GetWorldBounds = NULL;
SDK_SetWastedSettings                   SqVC_SetWastedSettings = NULL;
SDK_GetWastedSettings                   SqVC_GetWastedSettings = NULL;
SDK_SetTimeRate                         SqVC_SetTimeRate = NULL;
SDK_GetTimeRate                         SqVC_GetTimeRate = NULL;
SDK_SetHour                             SqVC_SetHour = NULL;
SDK_GetHour                             SqVC_GetHour = NULL;
SDK_SetMinute                           SqVC_SetMinute = NULL;
SDK_GetMinute                           SqVC_GetMinute = NULL;
SDK_SetWeather                          SqVC_SetWeather = NULL;
SDK_GetWeather                          SqVC_GetWeather = NULL;
SDK_SetGravity                          SqVC_SetGravity = NULL;
SDK_GetGravity                          SqVC_GetGravity = NULL;
SDK_SetGamespeed                        SqVC_SetGamespeed = NULL;
SDK_GetGamespeed                        SqVC_GetGamespeed = NULL;
SDK_SetWaterLevel                       SqVC_SetWaterLevel = NULL;
SDK_GetWaterLevel                       SqVC_GetWaterLevel = NULL;
SDK_SetMaxHeight                        SqVC_SetMaxHeight = NULL;
SDK_GetMaxHeight                        SqVC_GetMaxHeight = NULL;
SDK_SetKillCmdDelay                     SqVC_SetKillCmdDelay = NULL;
SDK_GetKillCmdDelay                     SqVC_GetKillCmdDelay = NULL;
SDK_SetVehiclesForcedRespawnHeight      SqVC_SetVehiclesForcedRespawnHeight = NULL;
SDK_GetVehiclesForcedRespawnHeight      SqVC_GetVehiclesForcedRespawnHeight = NULL;

// --------------------------------------------------------------------------------------------
//WORLD: toggles
SDK_ToggleSyncFrameLimiter              SqVC_ToggleSyncFrameLimiter = NULL;
SDK_EnabledSyncFrameLimiter             SqVC_EnabledSyncFrameLimiter = NULL;
SDK_ToggleFrameLimiter                  SqVC_ToggleFrameLimiter = NULL;
SDK_EnabledFrameLimiter                 SqVC_EnabledFrameLimiter = NULL;
SDK_ToggleTaxiBoostJump                 SqVC_ToggleTaxiBoostJump = NULL;
SDK_EnabledTaxiBoostJump                SqVC_EnabledTaxiBoostJump = NULL;
SDK_ToggleDriveOnWater                  SqVC_ToggleDriveOnWater = NULL;
SDK_EnabledDriveOnWater                 SqVC_EnabledDriveOnWater = NULL;
SDK_ToggleFastSwitch                    SqVC_ToggleFastSwitch = NULL;
SDK_EnabledFastSwitch                   SqVC_EnabledFastSwitch = NULL;
SDK_ToggleFriendlyFire                  SqVC_ToggleFriendlyFire = NULL;
SDK_EnabledFriendlyFire                 SqVC_EnabledFriendlyFire = NULL;
SDK_ToggleDisableDriveby                SqVC_ToggleDisableDriveby = NULL;
SDK_EnabledDisableDriveby               SqVC_EnabledDisableDriveby = NULL;
SDK_TogglePerfectHandling               SqVC_TogglePerfectHandling = NULL;
SDK_EnabledPerfectHandling              SqVC_EnabledPerfectHandling = NULL;
SDK_ToggleFlyingCars                    SqVC_ToggleFlyingCars = NULL;
SDK_EnabledFlyingCars                   SqVC_EnabledFlyingCars = NULL;
SDK_ToggleJumpSwitch                    SqVC_ToggleJumpSwitch = NULL;
SDK_EnabledJumpSwitch                   SqVC_EnabledJumpSwitch = NULL;
SDK_ToggleShowMarkers                   SqVC_ToggleShowMarkers = NULL;
SDK_EnabledShowMarkers                  SqVC_EnabledShowMarkers = NULL;
SDK_ToggleOnlyShowTeamMarkers           SqVC_ToggleOnlyShowTeamMarkers = NULL;
SDK_EnabledOnlyShowTeamMarkers          SqVC_EnabledOnlyShowTeamMarkers = NULL;
SDK_ToggleStuntBike                     SqVC_ToggleStuntBike = NULL;
SDK_EnabledStuntBike                    SqVC_EnabledStuntBike = NULL;
SDK_ToggleShootInAir                    SqVC_ToggleShootInAir = NULL;
SDK_EnabledShootInAir                   SqVC_EnabledShootInAir = NULL;
SDK_ToggleShowNametags                  SqVC_ToggleShowNametags = NULL;
SDK_EnabledShowNametags                 SqVC_EnabledShowNametags = NULL;
SDK_ToggleJoinMessages                  SqVC_ToggleJoinMessages = NULL;
SDK_EnabledJoinMessages                 SqVC_EnabledJoinMessages = NULL;
SDK_ToggleDeathMessages                 SqVC_ToggleDeathMessages = NULL;
SDK_EnabledDeathMessages                SqVC_EnabledDeathMessages = NULL;
SDK_ToggleChatTagsByDefaultEnabled      SqVC_ToggleChatTagsByDefaultEnabled = NULL;
SDK_EnabledChatTagsByDefault            SqVC_EnabledChatTagsByDefault = NULL;

// --------------------------------------------------------------------------------------------
//MISC
SDK_CreateExplosion                     SqVC_CreateExplosion = NULL;
SDK_PlaySound                           SqVC_PlaySound = NULL;
SDK_HideMapObject                       SqVC_HideMapObject = NULL;
SDK_ShowMapObject                       SqVC_ShowMapObject = NULL;
SDK_ShowAllMapObjects                   SqVC_ShowAllMapObjects = NULL;

//WEAPONDATA
SDK_SetWeaponDataValue                  SqVC_SetWeaponDataValue = NULL;
SDK_GetWeaponDataValue                  SqVC_GetWeaponDataValue = NULL;
SDK_ResetWeaponDataValue                SqVC_ResetWeaponDataValue = NULL;
SDK_IsWeaponDataValueModified           SqVC_IsWeaponDataValueModified = NULL;
SDK_ResetWeaponData                     SqVC_ResetWeaponData = NULL;
SDK_ResetAllWeaponData                  SqVC_ResetAllWeaponData = NULL;

// --------------------------------------------------------------------------------------------
//KEYBINDS
SDK_GetKeyBindUnusedSlot                SqVC_GetKeyBindUnusedSlot = NULL;
SDK_GetKeyBindData                      SqVC_GetKeyBindData = NULL;
SDK_RegisterKeyBind                     SqVC_RegisterKeyBind = NULL;
SDK_RemoveKeyBind                       SqVC_RemoveKeyBind = NULL;
SDK_RemoveAllKeyBinds                   SqVC_RemoveAllKeyBinds = NULL;

//BLIPS
SDK_CreateCoordBlip                     SqVC_CreateCoordBlip = NULL;
SDK_DestroyCoordBlip                    SqVC_DestroyCoordBlip = NULL;
SDK_GetCoordBlipInfo                    SqVC_GetCoordBlipInfo = NULL;

// --------------------------------------------------------------------------------------------
//SPRITES
SDK_CreateSprite                        SqVC_CreateSprite = NULL;
SDK_DestroySprite                       SqVC_DestroySprite = NULL;
SDK_ShowSprite                          SqVC_ShowSprite = NULL;
SDK_HideSprite                          SqVC_HideSprite = NULL;
SDK_MoveSprite                          SqVC_MoveSprite = NULL;
SDK_SetSpriteCenter                     SqVC_SetSpriteCenter = NULL;
SDK_RotateSprite                        SqVC_RotateSprite = NULL;
SDK_SetSpriteAlpha                      SqVC_SetSpriteAlpha = NULL;
SDK_SetSpriteRelativity                 SqVC_SetSpriteRelativity = NULL;

//TEXTDRAWS
SDK_CreateTextdraw                      SqVC_CreateTextdraw = NULL;
SDK_DestroyTextdraw                     SqVC_DestroyTextdraw = NULL;
SDK_ShowTextdraw                        SqVC_ShowTextdraw = NULL;
SDK_HideTextdraw                        SqVC_HideTextdraw = NULL;
SDK_MoveTextdraw                        SqVC_MoveTextdraw = NULL;
SDK_SetTextdrawColour                   SqVC_SetTextdrawColour = NULL;
SDK_SetTextdrawRelativity               SqVC_SetTextdrawRelativity = NULL;

// --------------------------------------------------------------------------------------------
//RADIOS
SDK_AddRadioStream                      SqVC_AddRadioStream = NULL;
SDK_RemoveRadioStream                   SqVC_RemoveRadioStream = NULL;

// --------------------------------------------------------------------------------------------
//CLASSES
SDK_SetUseClasses                       SqVC_SetUseClasses = NULL;
SDK_GetUseClasses                       SqVC_GetUseClasses = NULL;
SDK_GetPlayerClass                      SqVC_GetPlayerClass = NULL;
SDK_AddPlayerClass                      SqVC_AddPlayerClass = NULL;
SDK_SetSpawnPlayerPos                   SqVC_SetSpawnPlayerPos = NULL;
SDK_SetSpawnCameraPos                   SqVC_SetSpawnCameraPos = NULL;
SDK_SetSpawnCameraLookAt                SqVC_SetSpawnCameraLookAt = NULL;

// --------------------------------------------------------------------------------------------
//ADMIN
SDK_IsPlayerAdmin                       SqVC_IsPlayerAdmin = NULL;
SDK_SetPlayerAdmin                      SqVC_SetPlayerAdmin = NULL;
SDK_GetPlayerIP                         SqVC_GetPlayerIP = NULL;
SDK_KickPlayer                          SqVC_KickPlayer = NULL;
SDK_BanPlayer                           SqVC_BanPlayer = NULL;
SDK_BanIP                               SqVC_BanIP = NULL;
SDK_UnbanIP                             SqVC_UnbanIP = NULL;
SDK_IsIPBanned                          SqVC_IsIPBanned = NULL;

// --------------------------------------------------------------------------------------------
//PLAYERS: basic
SDK_GetPlayerIDFromName                 SqVC_GetPlayerIDFromName = NULL;
SDK_IsPlayerConnected                   SqVC_IsPlayerConnected = NULL;
SDK_IsPlayerSpawned                     SqVC_IsPlayerSpawned = NULL;
SDK_IsPlayerStreamedForPlayer           SqVC_IsPlayerStreamedForPlayer = NULL;
SDK_GetPlayerKey                        SqVC_GetPlayerKey = NULL;
SDK_SetPlayerWorld                      SqVC_SetPlayerWorld = NULL;
SDK_GetPlayerWorld                      SqVC_GetPlayerWorld = NULL;
SDK_SetPlayerSecWorld                   SqVC_SetPlayerSecWorld = NULL;
SDK_GetPlayerSecWorld                   SqVC_GetPlayerSecWorld = NULL;
SDK_GetPlayerUniqueWorld                SqVC_GetPlayerUniqueWorld = NULL;
SDK_IsPlayerWorldCompatible             SqVC_IsPlayerWorldCompatible = NULL;
SDK_GetPlayerState                      SqVC_GetPlayerState = NULL;
SDK_GetPlayerName                       SqVC_GetPlayerName = NULL;
SDK_SetPlayerName                       SqVC_SetPlayerName = NULL;
SDK_SetPlayerTeam                       SqVC_SetPlayerTeam = NULL;
SDK_GetPlayerTeam                       SqVC_GetPlayerTeam = NULL;
SDK_SetPlayerSkin                       SqVC_SetPlayerSkin = NULL;
SDK_GetPlayerSkin                       SqVC_GetPlayerSkin = NULL;
SDK_SetPlayerColour                     SqVC_SetPlayerColour = NULL;
SDK_GetPlayerColour                     SqVC_GetPlayerColour = NULL;
SDK_ForcePlayerSpawn                    SqVC_ForcePlayerSpawn = NULL;
SDK_ForcePlayerSelect                   SqVC_ForcePlayerSelect = NULL;
SDK_ForceAllSelect                      SqVC_ForceAllSelect = NULL;

// --------------------------------------------------------------------------------------------
//PLAYERS: score, ping, money, typing
SDK_GivePlayerMoney                     SqVC_GivePlayerMoney = NULL;
SDK_SetPlayerMoney                      SqVC_SetPlayerMoney = NULL;
SDK_GetPlayerMoney                      SqVC_GetPlayerMoney = NULL;
SDK_SetPlayerScore                      SqVC_SetPlayerScore = NULL;
SDK_GetPlayerScore                      SqVC_GetPlayerScore = NULL;
SDK_GetPlayerPing                       SqVC_GetPlayerPing = NULL;
SDK_IsPlayerTyping                      SqVC_IsPlayerTyping = NULL;
SDK_GetPlayerFPS                        SqVC_GetPlayerFPS = NULL;
SDK_GetPlayerUID                        SqVC_GetPlayerUID = NULL;
SDK_GetPlayerWantedLevel                SqVC_GetPlayerWantedLevel = NULL;

// --------------------------------------------------------------------------------------------
//PLAYERS: health and location
SDK_SetPlayerHealth                     SqVC_SetPlayerHealth = NULL;
SDK_GetPlayerHealth                     SqVC_GetPlayerHealth = NULL;
SDK_SetPlayerArmour                     SqVC_SetPlayerArmour = NULL;
SDK_GetPlayerArmour                     SqVC_GetPlayerArmour = NULL;
SDK_SetPlayerImmunityFlags              SqVC_SetPlayerImmunityFlags = NULL;
SDK_GetPlayerImmunityFlags              SqVC_GetPlayerImmunityFlags = NULL;
SDK_SetPlayerPos                        SqVC_SetPlayerPos = NULL;
SDK_GetPlayerPos                        SqVC_GetPlayerPos = NULL;
SDK_SetPlayerSpeed                      SqVC_SetPlayerSpeed = NULL;
SDK_GetPlayerSpeed                      SqVC_GetPlayerSpeed = NULL;
SDK_AddPlayerSpeed                      SqVC_AddPlayerSpeed = NULL;
SDK_SetPlayerHeading                    SqVC_SetPlayerHeading = NULL;
SDK_GetPlayerHeading                    SqVC_GetPlayerHeading = NULL;
SDK_SetPlayerAlpha                      SqVC_SetPlayerAlpha = NULL;
SDK_GetPlayerAlpha                      SqVC_GetPlayerAlpha = NULL;
SDK_GetPlayerOnFireStatus               SqVC_GetPlayerOnFireStatus = NULL;
SDK_GetPlayerCrouchStatus               SqVC_GetPlayerCrouchStatus = NULL;
SDK_GetPlayerAction                     SqVC_GetPlayerAction = NULL;
SDK_GetPlayerGameKeys                   SqVC_GetPlayerGameKeys = NULL;
SDK_GetPlayerAimPos                     SqVC_GetPlayerAimPos = NULL;
SDK_GetPlayerAimDir                     SqVC_GetPlayerAimDir = NULL;

// --------------------------------------------------------------------------------------------
//PLAYERS: vehicle
SDK_PutPlayerInVehicle                  SqVC_PutPlayerInVehicle = NULL;
SDK_RemovePlayerFromVehicle             SqVC_RemovePlayerFromVehicle = NULL;
SDK_GetPlayerInVehicleStatus            SqVC_GetPlayerInVehicleStatus = NULL;
SDK_GetPlayerInVehicleSlot              SqVC_GetPlayerInVehicleSlot = NULL;
SDK_GetPlayerVehicleID                  SqVC_GetPlayerVehicleID = NULL;

// --------------------------------------------------------------------------------------------
//PLAYERS: toggles
SDK_TogglePlayerControllable            SqVC_TogglePlayerControllable = NULL;
SDK_EnabledPlayerControllable           SqVC_EnabledPlayerControllable = NULL;
SDK_TogglePlayerDriveby                 SqVC_TogglePlayerDriveby = NULL;
SDK_EnabledPlayerDriveby                SqVC_EnabledPlayerDriveby = NULL;
SDK_TogglePlayerWhiteScanlines          SqVC_TogglePlayerWhiteScanlines = NULL;
SDK_EnabledPlayerWhiteScanlines         SqVC_EnabledPlayerWhiteScanlines = NULL;
SDK_TogglePlayerGreenScanlines          SqVC_TogglePlayerGreenScanlines = NULL;
SDK_EnabledPlayerGreenScanlines         SqVC_EnabledPlayerGreenScanlines = NULL;
SDK_TogglePlayerWidescreen              SqVC_TogglePlayerWidescreen = NULL;
SDK_EnabledPlayerWidescreen             SqVC_EnabledPlayerWidescreen = NULL;
SDK_TogglePlayerShowMarkers             SqVC_TogglePlayerShowMarkers = NULL;
SDK_EnabledPlayerShowMarkers            SqVC_EnabledPlayerShowMarkers = NULL;
SDK_TogglePlayerAttackPriv              SqVC_TogglePlayerAttackPriv = NULL;
SDK_EnabledPlayerAttackPriv             SqVC_EnabledPlayerAttackPriv = NULL;
SDK_TogglePlayerHasMarker               SqVC_TogglePlayerHasMarker = NULL;
SDK_EnabledPlayerHasMarker              SqVC_EnabledPlayerHasMarker = NULL;
SDK_TogglePlayerChatTagsEnabled         SqVC_TogglePlayerChatTagsEnabled = NULL;
SDK_EnabledPlayerChatTags               SqVC_EnabledPlayerChatTags = NULL;
SDK_TogglePlayerDrunkEffects            SqVC_TogglePlayerDrunkEffects = NULL;
SDK_EnabledPlayerDrunkEffects           SqVC_EnabledPlayerDrunkEffects = NULL;

// --------------------------------------------------------------------------------------------
//PLAYERS: weapons
SDK_GivePlayerWeapon                    SqVC_GivePlayerWeapon = NULL;
SDK_SetPlayerWeapon                     SqVC_SetPlayerWeapon = NULL;
SDK_GetPlayerWeapon                     SqVC_GetPlayerWeapon = NULL;
SDK_GetPlayerWeaponAmmo                 SqVC_GetPlayerWeaponAmmo = NULL;
SDK_SetPlayerWeaponSlot                 SqVC_SetPlayerWeaponSlot = NULL;
SDK_GetPlayerWeaponSlot                 SqVC_GetPlayerWeaponSlot = NULL;
SDK_GetPlayerWeaponAtSlot               SqVC_GetPlayerWeaponAtSlot = NULL;
SDK_GetPlayerAmmoAtSlot                 SqVC_GetPlayerAmmoAtSlot = NULL;
SDK_RemovePlayerWeapon                  SqVC_RemovePlayerWeapon = NULL;
SDK_RemoveAllWeapons                    SqVC_RemoveAllWeapons = NULL;

// --------------------------------------------------------------------------------------------
//PLAYERS: camera
SDK_SetCameraPosition                   SqVC_SetCameraPosition = NULL;
SDK_RestoreCamera                       SqVC_RestoreCamera = NULL;
SDK_IsCameraLocked                      SqVC_IsCameraLocked = NULL;

// --------------------------------------------------------------------------------------------
//PLAYERS: misc
SDK_SetPlayerAnimation                  SqVC_SetPlayerAnimation = NULL;
SDK_SetPlayerWantedLevel                SqVC_SetPlayerWantedLevel = NULL;
SDK_GetPlayerStandingOnVehicle          SqVC_GetPlayerStandingOnVehicle = NULL;
SDK_GetPlayerStandingOnObject           SqVC_GetPlayerStandingOnObject = NULL;
SDK_IsPlayerAway                        SqVC_IsPlayerAway = NULL;
SDK_GetPlayerSpectateTarget             SqVC_GetPlayerSpectateTarget = NULL;
SDK_SetPlayerSpectateTarget             SqVC_SetPlayerSpectateTarget = NULL;
SDK_RedirectPlayerToServer              SqVC_RedirectPlayerToServer = NULL;

// --------------------------------------------------------------------------------------------
//VEHICLES
SDK_CreateVehicle                       SqVC_CreateVehicle = NULL;
SDK_DeleteVehicle                       SqVC_DeleteVehicle = NULL;
SDK_GetVehicleSyncSource                SqVC_GetVehicleSyncSource = NULL;
SDK_GetVehicleSyncType                  SqVC_GetVehicleSyncType = NULL;
SDK_IsVehicleStreamedForPlayer          SqVC_IsVehicleStreamedForPlayer = NULL;
SDK_SetVehicleWorld                     SqVC_SetVehicleWorld = NULL;
SDK_GetVehicleWorld                     SqVC_GetVehicleWorld = NULL;
SDK_GetVehicleModel                     SqVC_GetVehicleModel = NULL;
SDK_GetVehicleOccupant                  SqVC_GetVehicleOccupant = NULL;
SDK_RespawnVehicle                      SqVC_RespawnVehicle = NULL;
SDK_SetVehicleImmunityFlags             SqVC_SetVehicleImmunityFlags = NULL;
SDK_GetVehicleImmunityFlags             SqVC_GetVehicleImmunityFlags = NULL;
SDK_KillVehicle                         SqVC_KillVehicle = NULL;
SDK_IsVehicleWrecked                    SqVC_IsVehicleWrecked = NULL;
SDK_SetVehiclePos                       SqVC_SetVehiclePos = NULL;
SDK_GetVehiclePos                       SqVC_GetVehiclePos = NULL;
SDK_SetVehicleRot                       SqVC_SetVehicleRot = NULL;
SDK_SetVehicleRotEuler                  SqVC_SetVehicleRotEuler = NULL;
SDK_GetVehicleRot                       SqVC_GetVehicleRot = NULL;
SDK_GetVehicleRotEuler                  SqVC_GetVehicleRotEuler = NULL;
SDK_SetVehicleSpeed                     SqVC_SetVehicleSpeed = NULL;
SDK_GetVehicleSpeed                     SqVC_GetVehicleSpeed = NULL;
SDK_AddVehicleSpeed                     SqVC_AddVehicleSpeed = NULL;
SDK_SetVehicleRelSpeed                  SqVC_SetVehicleRelSpeed = NULL;
SDK_GetVehicleRelSpeed                  SqVC_GetVehicleRelSpeed = NULL;
SDK_AddVehicleRelSpeed                  SqVC_AddVehicleRelSpeed = NULL;
SDK_SetVehicleTurnSpeed                 SqVC_SetVehicleTurnSpeed = NULL;
SDK_GetVehicleTurnSpeed                 SqVC_GetVehicleTurnSpeed = NULL;
SDK_AddVehicleTurnSpeed                 SqVC_AddVehicleTurnSpeed = NULL;
SDK_SetVehicleRelTurnSpeed              SqVC_SetVehicleRelTurnSpeed = NULL;
SDK_GetVehicleRelTurnSpeed              SqVC_GetVehicleRelTurnSpeed = NULL;
SDK_AddVehicleRelTurnSpeed              SqVC_AddVehicleRelTurnSpeed = NULL;
SDK_SetVehicleSpawnPos                  SqVC_SetVehicleSpawnPos = NULL;
SDK_GetVehicleSpawnPos                  SqVC_GetVehicleSpawnPos = NULL;
SDK_SetVehicleSpawnRot                  SqVC_SetVehicleSpawnRot = NULL;
SDK_SetVehicleSpawnRotEuler             SqVC_SetVehicleSpawnRotEuler = NULL;
SDK_GetVehicleSpawnRot                  SqVC_GetVehicleSpawnRot = NULL;
SDK_GetVehicleSpawnRotEuler             SqVC_GetVehicleSpawnRotEuler = NULL;
SDK_SetVehicleIdleRespawnTimer          SqVC_SetVehicleIdleRespawnTimer = NULL;
SDK_GetVehicleIdleRespawnTimer          SqVC_GetVehicleIdleRespawnTimer = NULL;
SDK_SetVehicleHealth                    SqVC_SetVehicleHealth = NULL;
SDK_GetVehicleHealth                    SqVC_GetVehicleHealth = NULL;
SDK_SetVehicleColour                    SqVC_SetVehicleColour = NULL;
SDK_GetVehicleColour                    SqVC_GetVehicleColour = NULL;
SDK_SetVehicleDoorsLocked               SqVC_SetVehicleDoorsLocked = NULL;
SDK_GetVehicleDoorsLocked               SqVC_GetVehicleDoorsLocked = NULL;
SDK_SetVehiclePartStatus                SqVC_SetVehiclePartStatus = NULL;
SDK_GetVehiclePartStatus                SqVC_GetVehiclePartStatus = NULL;
SDK_SetVehicleTyreStatus                SqVC_SetVehicleTyreStatus = NULL;
SDK_GetVehicleTyreStatus                SqVC_GetVehicleTyreStatus = NULL;
SDK_SetVehicleDamageData                SqVC_SetVehicleDamageData = NULL;
SDK_GetVehicleDamageData                SqVC_GetVehicleDamageData = NULL;
SDK_SetVehicleAlarm                     SqVC_SetVehicleAlarm = NULL;
SDK_GetVehicleAlarm                     SqVC_GetVehicleAlarm = NULL;
SDK_SetVehicleLights                    SqVC_SetVehicleLights = NULL;
SDK_GetVehicleLights                    SqVC_GetVehicleLights = NULL;
SDK_SetVehicleRadio                     SqVC_SetVehicleRadio = NULL;
SDK_GetVehicleRadio                     SqVC_GetVehicleRadio = NULL;
SDK_SetVehicleRadioLocked               SqVC_SetVehicleRadioLocked = NULL;
SDK_IsVehicleRadioLocked                SqVC_IsVehicleRadioLocked = NULL;
SDK_GetVehicleGhostState                SqVC_GetVehicleGhostState = NULL;
SDK_SetVehicleGhostState                SqVC_SetVehicleGhostState = NULL;
SDK_GetVehicleTurretRotation            SqVC_GetVehicleTurretRotation = NULL;

// --------------------------------------------------------------------------------------------
//VEHICLES: handling
SDK_ResetAllVehicleHandlings            SqVC_ResetAllVehicleHandlings = NULL;
SDK_ExistsHandlingRule                  SqVC_ExistsHandlingRule = NULL;
SDK_SetHandlingRule                     SqVC_SetHandlingRule = NULL;
SDK_GetHandlingRule                     SqVC_GetHandlingRule = NULL;
SDK_ResetHandlingRule                   SqVC_ResetHandlingRule = NULL;
SDK_ResetHandling                       SqVC_ResetHandling = NULL;
SDK_ExistsInstHandlingRule              SqVC_ExistsInstHandlingRule = NULL;
SDK_SetInstHandlingRule                 SqVC_SetInstHandlingRule = NULL;
SDK_GetInstHandlingRule                 SqVC_GetInstHandlingRule = NULL;
SDK_ResetInstHandlingRule               SqVC_ResetInstHandlingRule = NULL;
SDK_ResetInstHandling                   SqVC_ResetInstHandling = NULL;

// --------------------------------------------------------------------------------------------
//PICKUPS
SDK_CreatePickup                        SqVC_CreatePickup = NULL;
SDK_DeletePickup                        SqVC_DeletePickup = NULL;
SDK_IsPickupStreamedForPlayer           SqVC_IsPickupStreamedForPlayer = NULL;
SDK_SetPickupWorld                      SqVC_SetPickupWorld = NULL;
SDK_GetPickupWorld                      SqVC_GetPickupWorld = NULL;
SDK_PickupGetAlpha                      SqVC_PickupGetAlpha = NULL;
SDK_PickupSetAlpha                      SqVC_PickupSetAlpha = NULL;
SDK_PickupIsAutomatic                   SqVC_PickupIsAutomatic = NULL;
SDK_PickupSetAutomatic                  SqVC_PickupSetAutomatic = NULL;
SDK_SetPickupAutoTimer                  SqVC_SetPickupAutoTimer = NULL;
SDK_GetPickupAutoTimer                  SqVC_GetPickupAutoTimer = NULL;
SDK_PickupRefresh                       SqVC_PickupRefresh = NULL;
SDK_PickupGetPos                        SqVC_PickupGetPos = NULL;
SDK_PickupSetPos                        SqVC_PickupSetPos = NULL;
SDK_PickupGetModel                      SqVC_PickupGetModel = NULL;
SDK_PickupGetQuantity                   SqVC_PickupGetQuantity = NULL;

// --------------------------------------------------------------------------------------------
//OBJECTS
SDK_CreateObject                        SqVC_CreateObject = NULL;
SDK_DeleteObject                        SqVC_DeleteObject = NULL;
SDK_IsObjectStreamedForPlayer           SqVC_IsObjectStreamedForPlayer = NULL;
SDK_GetObjectModel                      SqVC_GetObjectModel = NULL;
SDK_SetObjectWorld                      SqVC_SetObjectWorld = NULL;
SDK_GetObjectWorld                      SqVC_GetObjectWorld = NULL;
SDK_SetObjectAlpha                      SqVC_SetObjectAlpha = NULL;
SDK_GetObjectAlpha                      SqVC_GetObjectAlpha = NULL;
SDK_MoveObjectTo                        SqVC_MoveObjectTo = NULL;
SDK_MoveObjectBy                        SqVC_MoveObjectBy = NULL;
SDK_SetObjectPos                        SqVC_SetObjectPos = NULL;
SDK_GetObjectPos                        SqVC_GetObjectPos = NULL;
SDK_RotObjectTo                         SqVC_RotObjectTo = NULL;
SDK_RotObjectToEuler                    SqVC_RotObjectToEuler = NULL;
SDK_RotObjectBy                         SqVC_RotObjectBy = NULL;
SDK_RotObjectByEuler                    SqVC_RotObjectByEuler = NULL;
SDK_GetObjectRot                        SqVC_GetObjectRot = NULL;
SDK_GetObjectRotEuler                   SqVC_GetObjectRotEuler = NULL;
SDK_SetObjectShotReport                 SqVC_SetObjectShotReport = NULL;
SDK_IsObjectShotReport                  SqVC_IsObjectShotReport = NULL;
SDK_SetObjectBumpReport                 SqVC_SetObjectBumpReport = NULL;
SDK_IsObjectBumpReport                  SqVC_IsObjectBumpReport = NULL;

// --------------------------------------------------------------------------------------------
// TODO: Move these functions to proper sections on major plugin update
SDK_ToggleWallglitch                    SqVC_ToggleWallglitch = NULL;
SDK_EnabledWallglitch                   SqVC_EnabledWallglitch = NULL;
SDK_SetVehicleSiren                     SqVC_SetVehicleSiren = NULL;
SDK_GetVehicleSiren                     SqVC_GetVehicleSiren = NULL;
SDK_GetPlayerUID2                       SqVC_GetPlayerUID2 = NULL;

// --------------------------------------------------------------------------------------------
SDK_CreateCheckpoint                    SqVC_CreateCheckpoint = NULL;
SDK_DeleteCheckpoint                    SqVC_DeleteCheckpoint = NULL;
SDK_IsCheckpointStreamedForPlayer       SqVC_IsCheckpointStreamedForPlayer = NULL;
SDK_SetCheckpointWorld                  SqVC_SetCheckpointWorld = NULL;
SDK_GetCheckpointWorld                  SqVC_GetCheckpointWorld = NULL;
SDK_SetCheckpointColor                  SqVC_SetCheckpointColor = NULL;
SDK_GetCheckpointColor                  SqVC_GetCheckpointColor = NULL;
SDK_SetCheckpointPos                    SqVC_SetCheckpointPos = NULL;
SDK_GetCheckpointPos                    SqVC_GetCheckpointPos = NULL;
SDK_SetCheckpointRadius                 SqVC_SetCheckpointRadius = NULL;
SDK_GetCheckpointRadius                 SqVC_GetCheckpointRadius = NULL;
SDK_GetCheckpointOwner                  SqVC_GetCheckpointOwner = NULL;

// --------------------------------------------------------------------------------------------
SDK_CreateSphere                        SqVC_CreateSphere = NULL;
SDK_DeleteSphere                        SqVC_DeleteSphere = NULL;
SDK_IsSphereStreamedForPlayer           SqVC_IsSphereStreamedForPlayer = NULL;
SDK_SetSphereWorld                      SqVC_SetSphereWorld = NULL;
SDK_GetSphereWorld                      SqVC_GetSphereWorld = NULL;
SDK_SetSphereColor                      SqVC_SetSphereColor = NULL;
SDK_GetSphereColor                      SqVC_GetSphereColor = NULL;
SDK_SetSpherePos                        SqVC_SetSpherePos = NULL;
SDK_GetSpherePos                        SqVC_GetSpherePos = NULL;
SDK_SetSphereRadius                     SqVC_SetSphereRadius = NULL;
SDK_GetSphereRadius                     SqVC_GetSphereRadius = NULL;
SDK_GetSphereOwner                      SqVC_GetSphereOwner = NULL;

// ------------------------------------------------------------------------------------------------
SQRESULT vc_api_expand(PluginFuncs * vcapi)
{
    if (!vcapi)
        return SQ_ERROR;

    // --------------------------------------------------------------------------------------------
    //PLUGIN SYSTEM
    SqVC_GetServerVersion                   = vcapi->GetServerVersion;
    SqVC_GetServerSettings                  = vcapi->GetServerSettings;
    SqVC_ExportFunctions                    = vcapi->ExportFunctions;
    SqVC_GetNumberOfPlugins                 = vcapi->GetNumberOfPlugins;
    SqVC_GetPluginInfo                      = vcapi->GetPluginInfo;
    SqVC_FindPlugin                         = vcapi->FindPlugin;
    SqVC_GetPluginExports                   = vcapi->GetPluginExports;
    SqVC_GetTime                            = vcapi->GetTime;

    // --------------------------------------------------------------------------------------------
    //MESSAGES
    SqVC_printf                             = vcapi->printf;
    SqVC_SendCustomCommand                  = vcapi->SendCustomCommand;
    SqVC_SendClientMessage                  = vcapi->SendClientMessage;
    SqVC_SendGameMessage                    = vcapi->SendGameMessage;

    // --------------------------------------------------------------------------------------------
    //SERVER SETTINGS
    SqVC_SetServerName                      = vcapi->SetServerName;
    SqVC_GetServerName                      = vcapi->GetServerName;
    SqVC_SetMaxPlayers                      = vcapi->SetMaxPlayers;
    SqVC_GetMaxPlayers                      = vcapi->GetMaxPlayers;
    SqVC_SetServerPassword                  = vcapi->SetServerPassword;
    SqVC_GetServerPassword                  = vcapi->GetServerPassword;
    SqVC_SetGameModeText                    = vcapi->SetGameModeText;
    SqVC_GetGameModeText                    = vcapi->GetGameModeText;
    SqVC_ShutdownServer                     = vcapi->ShutdownServer;

    // --------------------------------------------------------------------------------------------
    //WORLD: settings
    SqVC_SetWorldBounds                     = vcapi->SetWorldBounds;
    SqVC_GetWorldBounds                     = vcapi->GetWorldBounds;
    SqVC_SetWastedSettings                  = vcapi->SetWastedSettings;
    SqVC_GetWastedSettings                  = vcapi->GetWastedSettings;
    SqVC_SetTimeRate                        = vcapi->SetTimeRate;
    SqVC_GetTimeRate                        = vcapi->GetTimeRate;
    SqVC_SetHour                            = vcapi->SetHour;
    SqVC_GetHour                            = vcapi->GetHour;
    SqVC_SetMinute                          = vcapi->SetMinute;
    SqVC_GetMinute                          = vcapi->GetMinute;
    SqVC_SetWeather                         = vcapi->SetWeather;
    SqVC_GetWeather                         = vcapi->GetWeather;
    SqVC_SetGravity                         = vcapi->SetGravity;
    SqVC_GetGravity                         = vcapi->GetGravity;
    SqVC_SetGamespeed                       = vcapi->SetGamespeed;
    SqVC_GetGamespeed                       = vcapi->GetGamespeed;
    SqVC_SetWaterLevel                      = vcapi->SetWaterLevel;
    SqVC_GetWaterLevel                      = vcapi->GetWaterLevel;
    SqVC_SetMaxHeight                       = vcapi->SetMaxHeight;
    SqVC_GetMaxHeight                       = vcapi->GetMaxHeight;
    SqVC_SetKillCmdDelay                    = vcapi->SetKillCmdDelay;
    SqVC_GetKillCmdDelay                    = vcapi->GetKillCmdDelay;
    SqVC_SetVehiclesForcedRespawnHeight     = vcapi->SetVehiclesForcedRespawnHeight;
    SqVC_GetVehiclesForcedRespawnHeight     = vcapi->GetVehiclesForcedRespawnHeight;

    // --------------------------------------------------------------------------------------------
    //WORLD: toggles
    SqVC_ToggleSyncFrameLimiter             = vcapi->ToggleSyncFrameLimiter;
    SqVC_EnabledSyncFrameLimiter            = vcapi->EnabledSyncFrameLimiter;
    SqVC_ToggleFrameLimiter                 = vcapi->ToggleFrameLimiter;
    SqVC_EnabledFrameLimiter                = vcapi->EnabledFrameLimiter;
    SqVC_ToggleTaxiBoostJump                = vcapi->ToggleTaxiBoostJump;
    SqVC_EnabledTaxiBoostJump               = vcapi->EnabledTaxiBoostJump;
    SqVC_ToggleDriveOnWater                 = vcapi->ToggleDriveOnWater;
    SqVC_EnabledDriveOnWater                = vcapi->EnabledDriveOnWater;
    SqVC_ToggleFastSwitch                   = vcapi->ToggleFastSwitch;
    SqVC_EnabledFastSwitch                  = vcapi->EnabledFastSwitch;
    SqVC_ToggleFriendlyFire                 = vcapi->ToggleFriendlyFire;
    SqVC_EnabledFriendlyFire                = vcapi->EnabledFriendlyFire;
    SqVC_ToggleDisableDriveby               = vcapi->ToggleDisableDriveby;
    SqVC_EnabledDisableDriveby              = vcapi->EnabledDisableDriveby;
    SqVC_TogglePerfectHandling              = vcapi->TogglePerfectHandling;
    SqVC_EnabledPerfectHandling             = vcapi->EnabledPerfectHandling;
    SqVC_ToggleFlyingCars                   = vcapi->ToggleFlyingCars;
    SqVC_EnabledFlyingCars                  = vcapi->EnabledFlyingCars;
    SqVC_ToggleJumpSwitch                   = vcapi->ToggleJumpSwitch;
    SqVC_EnabledJumpSwitch                  = vcapi->EnabledJumpSwitch;
    SqVC_ToggleShowMarkers                  = vcapi->ToggleShowMarkers;
    SqVC_EnabledShowMarkers                 = vcapi->EnabledShowMarkers;
    SqVC_ToggleOnlyShowTeamMarkers          = vcapi->ToggleOnlyShowTeamMarkers;
    SqVC_EnabledOnlyShowTeamMarkers         = vcapi->EnabledOnlyShowTeamMarkers;
    SqVC_ToggleStuntBike                    = vcapi->ToggleStuntBike;
    SqVC_EnabledStuntBike                   = vcapi->EnabledStuntBike;
    SqVC_ToggleShootInAir                   = vcapi->ToggleShootInAir;
    SqVC_EnabledShootInAir                  = vcapi->EnabledShootInAir;
    SqVC_ToggleShowNametags                 = vcapi->ToggleShowNametags;
    SqVC_EnabledShowNametags                = vcapi->EnabledShowNametags;
    SqVC_ToggleJoinMessages                 = vcapi->ToggleJoinMessages;
    SqVC_EnabledJoinMessages                = vcapi->EnabledJoinMessages;
    SqVC_ToggleDeathMessages                = vcapi->ToggleDeathMessages;
    SqVC_EnabledDeathMessages               = vcapi->EnabledDeathMessages;
    SqVC_ToggleChatTagsByDefaultEnabled     = vcapi->ToggleChatTagsByDefaultEnabled;
    SqVC_EnabledChatTagsByDefault           = vcapi->EnabledChatTagsByDefault;

    // --------------------------------------------------------------------------------------------
    //MISC
    SqVC_CreateExplosion                    = vcapi->CreateExplosion;
    SqVC_PlaySound                          = vcapi->PlaySound;
    SqVC_HideMapObject                      = vcapi->HideMapObject;
    SqVC_ShowMapObject                      = vcapi->ShowMapObject;
    SqVC_ShowAllMapObjects                  = vcapi->ShowAllMapObjects;

    //WEAPONDATA
    SqVC_SetWeaponDataValue                 = vcapi->SetWeaponDataValue;
    SqVC_GetWeaponDataValue                 = vcapi->GetWeaponDataValue;
    SqVC_ResetWeaponDataValue               = vcapi->ResetWeaponDataValue;
    SqVC_IsWeaponDataValueModified          = vcapi->IsWeaponDataValueModified;
    SqVC_ResetWeaponData                    = vcapi->ResetWeaponData;
    SqVC_ResetAllWeaponData                 = vcapi->ResetAllWeaponData;

    // --------------------------------------------------------------------------------------------
    //KEYBINDS
    SqVC_GetKeyBindUnusedSlot               = vcapi->GetKeyBindUnusedSlot;
    SqVC_GetKeyBindData                     = vcapi->GetKeyBindData;
    SqVC_RegisterKeyBind                    = vcapi->RegisterKeyBind;
    SqVC_RemoveKeyBind                      = vcapi->RemoveKeyBind;
    SqVC_RemoveAllKeyBinds                  = vcapi->RemoveAllKeyBinds;

    //BLIPS
    SqVC_CreateCoordBlip                    = vcapi->CreateCoordBlip;
    SqVC_DestroyCoordBlip                   = vcapi->DestroyCoordBlip;
    SqVC_GetCoordBlipInfo                   = vcapi->GetCoordBlipInfo;

    // --------------------------------------------------------------------------------------------
    //SPRITES
    SqVC_CreateSprite                       = vcapi->CreateSprite;
    SqVC_DestroySprite                      = vcapi->DestroySprite;
    SqVC_ShowSprite                         = vcapi->ShowSprite;
    SqVC_HideSprite                         = vcapi->HideSprite;
    SqVC_MoveSprite                         = vcapi->MoveSprite;
    SqVC_SetSpriteCenter                    = vcapi->SetSpriteCenter;
    SqVC_RotateSprite                       = vcapi->RotateSprite;
    SqVC_SetSpriteAlpha                     = vcapi->SetSpriteAlpha;
    SqVC_SetSpriteRelativity                = vcapi->SetSpriteRelativity;

    //TEXTDRAWS
    SqVC_CreateTextdraw                     = vcapi->CreateTextdraw;
    SqVC_DestroyTextdraw                    = vcapi->DestroyTextdraw;
    SqVC_ShowTextdraw                       = vcapi->ShowTextdraw;
    SqVC_HideTextdraw                       = vcapi->HideTextdraw;
    SqVC_MoveTextdraw                       = vcapi->MoveTextdraw;
    SqVC_SetTextdrawColour                  = vcapi->SetTextdrawColour;
    SqVC_SetTextdrawRelativity              = vcapi->SetTextdrawRelativity;

    // --------------------------------------------------------------------------------------------
    //RADIOS
    SqVC_AddRadioStream                     = vcapi->AddRadioStream;
    SqVC_RemoveRadioStream                  = vcapi->RemoveRadioStream;

    // --------------------------------------------------------------------------------------------
    //CLASSES
    SqVC_SetUseClasses                      = vcapi->SetUseClasses;
    SqVC_GetUseClasses                      = vcapi->GetUseClasses;
    SqVC_GetPlayerClass                     = vcapi->GetPlayerClass;
    SqVC_AddPlayerClass                     = vcapi->AddPlayerClass;
    SqVC_SetSpawnPlayerPos                  = vcapi->SetSpawnPlayerPos;
    SqVC_SetSpawnCameraPos                  = vcapi->SetSpawnCameraPos;
    SqVC_SetSpawnCameraLookAt               = vcapi->SetSpawnCameraLookAt;

    // --------------------------------------------------------------------------------------------
    //ADMIN
    SqVC_IsPlayerAdmin                      = vcapi->IsPlayerAdmin;
    SqVC_SetPlayerAdmin                     = vcapi->SetPlayerAdmin;
    SqVC_GetPlayerIP                        = vcapi->GetPlayerIP;
    SqVC_KickPlayer                         = vcapi->KickPlayer;
    SqVC_BanPlayer                          = vcapi->BanPlayer;
    SqVC_BanIP                              = vcapi->BanIP;
    SqVC_UnbanIP                            = vcapi->UnbanIP;
    SqVC_IsIPBanned                         = vcapi->IsIPBanned;

    // --------------------------------------------------------------------------------------------
    //PLAYERS: basic
    SqVC_GetPlayerIDFromName                = vcapi->GetPlayerIDFromName;
    SqVC_IsPlayerConnected                  = vcapi->IsPlayerConnected;
    SqVC_IsPlayerSpawned                    = vcapi->IsPlayerSpawned;
    SqVC_IsPlayerStreamedForPlayer          = vcapi->IsPlayerStreamedForPlayer;
    SqVC_GetPlayerKey                       = vcapi->GetPlayerKey;
    SqVC_SetPlayerWorld                     = vcapi->SetPlayerWorld;
    SqVC_GetPlayerWorld                     = vcapi->GetPlayerWorld;
    SqVC_SetPlayerSecWorld                  = vcapi->SetPlayerSecWorld;
    SqVC_GetPlayerSecWorld                  = vcapi->GetPlayerSecWorld;
    SqVC_GetPlayerUniqueWorld               = vcapi->GetPlayerUniqueWorld;
    SqVC_IsPlayerWorldCompatible            = vcapi->IsPlayerWorldCompatible;
    SqVC_GetPlayerState                     = vcapi->GetPlayerState;
    SqVC_GetPlayerName                      = vcapi->GetPlayerName;
    SqVC_SetPlayerName                      = vcapi->SetPlayerName;
    SqVC_SetPlayerTeam                      = vcapi->SetPlayerTeam;
    SqVC_GetPlayerTeam                      = vcapi->GetPlayerTeam;
    SqVC_SetPlayerSkin                      = vcapi->SetPlayerSkin;
    SqVC_GetPlayerSkin                      = vcapi->GetPlayerSkin;
    SqVC_SetPlayerColour                    = vcapi->SetPlayerColour;
    SqVC_GetPlayerColour                    = vcapi->GetPlayerColour;
    SqVC_ForcePlayerSpawn                   = vcapi->ForcePlayerSpawn;
    SqVC_ForcePlayerSelect                  = vcapi->ForcePlayerSelect;
    SqVC_ForceAllSelect                     = vcapi->ForceAllSelect;

    // --------------------------------------------------------------------------------------------
    //PLAYERS: score, ping, money, typing
    SqVC_GivePlayerMoney                    = vcapi->GivePlayerMoney;
    SqVC_SetPlayerMoney                     = vcapi->SetPlayerMoney;
    SqVC_GetPlayerMoney                     = vcapi->GetPlayerMoney;
    SqVC_SetPlayerScore                     = vcapi->SetPlayerScore;
    SqVC_GetPlayerScore                     = vcapi->GetPlayerScore;
    SqVC_GetPlayerPing                      = vcapi->GetPlayerPing;
    SqVC_IsPlayerTyping                     = vcapi->IsPlayerTyping;
    SqVC_GetPlayerFPS                       = vcapi->GetPlayerFPS;
    SqVC_GetPlayerUID                       = vcapi->GetPlayerUID;
    SqVC_GetPlayerWantedLevel               = vcapi->GetPlayerWantedLevel;

    // --------------------------------------------------------------------------------------------
    //PLAYERS: health and location
    SqVC_SetPlayerHealth                    = vcapi->SetPlayerHealth;
    SqVC_GetPlayerHealth                    = vcapi->GetPlayerHealth;
    SqVC_SetPlayerArmour                    = vcapi->SetPlayerArmour;
    SqVC_GetPlayerArmour                    = vcapi->GetPlayerArmour;
    SqVC_SetPlayerImmunityFlags             = vcapi->SetPlayerImmunityFlags;
    SqVC_GetPlayerImmunityFlags             = vcapi->GetPlayerImmunityFlags;
    SqVC_SetPlayerPos                       = vcapi->SetPlayerPos;
    SqVC_GetPlayerPos                       = vcapi->GetPlayerPos;
    SqVC_SetPlayerSpeed                     = vcapi->SetPlayerSpeed;
    SqVC_GetPlayerSpeed                     = vcapi->GetPlayerSpeed;
    SqVC_AddPlayerSpeed                     = vcapi->AddPlayerSpeed;
    SqVC_SetPlayerHeading                   = vcapi->SetPlayerHeading;
    SqVC_GetPlayerHeading                   = vcapi->GetPlayerHeading;
    SqVC_SetPlayerAlpha                     = vcapi->SetPlayerAlpha;
    SqVC_GetPlayerAlpha                     = vcapi->GetPlayerAlpha;
    SqVC_GetPlayerOnFireStatus              = vcapi->GetPlayerOnFireStatus;
    SqVC_GetPlayerCrouchStatus              = vcapi->GetPlayerCrouchStatus;
    SqVC_GetPlayerAction                    = vcapi->GetPlayerAction;
    SqVC_GetPlayerGameKeys                  = vcapi->GetPlayerGameKeys;
    SqVC_GetPlayerAimPos                    = vcapi->GetPlayerAimPos;
    SqVC_GetPlayerAimDir                    = vcapi->GetPlayerAimDir;

    // --------------------------------------------------------------------------------------------
    //PLAYERS: vehicle
    SqVC_PutPlayerInVehicle                 = vcapi->PutPlayerInVehicle;
    SqVC_RemovePlayerFromVehicle            = vcapi->RemovePlayerFromVehicle;
    SqVC_GetPlayerInVehicleStatus           = vcapi->GetPlayerInVehicleStatus;
    SqVC_GetPlayerInVehicleSlot             = vcapi->GetPlayerInVehicleSlot;
    SqVC_GetPlayerVehicleID                 = vcapi->GetPlayerVehicleID;

    // --------------------------------------------------------------------------------------------
    //PLAYERS: toggles
    SqVC_TogglePlayerControllable           = vcapi->TogglePlayerControllable;
    SqVC_EnabledPlayerControllable          = vcapi->EnabledPlayerControllable;
    SqVC_TogglePlayerDriveby                = vcapi->TogglePlayerDriveby;
    SqVC_EnabledPlayerDriveby               = vcapi->EnabledPlayerDriveby;
    SqVC_TogglePlayerWhiteScanlines         = vcapi->TogglePlayerWhiteScanlines;
    SqVC_EnabledPlayerWhiteScanlines        = vcapi->EnabledPlayerWhiteScanlines;
    SqVC_TogglePlayerGreenScanlines         = vcapi->TogglePlayerGreenScanlines;
    SqVC_EnabledPlayerGreenScanlines        = vcapi->EnabledPlayerGreenScanlines;
    SqVC_TogglePlayerWidescreen             = vcapi->TogglePlayerWidescreen;
    SqVC_EnabledPlayerWidescreen            = vcapi->EnabledPlayerWidescreen;
    SqVC_TogglePlayerShowMarkers            = vcapi->TogglePlayerShowMarkers;
    SqVC_EnabledPlayerShowMarkers           = vcapi->EnabledPlayerShowMarkers;
    SqVC_TogglePlayerAttackPriv             = vcapi->TogglePlayerAttackPriv;
    SqVC_EnabledPlayerAttackPriv            = vcapi->EnabledPlayerAttackPriv;
    SqVC_TogglePlayerHasMarker              = vcapi->TogglePlayerHasMarker;
    SqVC_EnabledPlayerHasMarker             = vcapi->EnabledPlayerHasMarker;
    SqVC_TogglePlayerChatTagsEnabled        = vcapi->TogglePlayerChatTagsEnabled;
    SqVC_EnabledPlayerChatTags              = vcapi->EnabledPlayerChatTags;
    SqVC_TogglePlayerDrunkEffects           = vcapi->TogglePlayerDrunkEffects;
    SqVC_EnabledPlayerDrunkEffects          = vcapi->EnabledPlayerDrunkEffects;

    // --------------------------------------------------------------------------------------------
    //PLAYERS: weapons
    SqVC_GivePlayerWeapon                   = vcapi->GivePlayerWeapon;
    SqVC_SetPlayerWeapon                    = vcapi->SetPlayerWeapon;
    SqVC_GetPlayerWeapon                    = vcapi->GetPlayerWeapon;
    SqVC_GetPlayerWeaponAmmo                = vcapi->GetPlayerWeaponAmmo;
    SqVC_SetPlayerWeaponSlot                = vcapi->SetPlayerWeaponSlot;
    SqVC_GetPlayerWeaponSlot                = vcapi->GetPlayerWeaponSlot;
    SqVC_GetPlayerWeaponAtSlot              = vcapi->GetPlayerWeaponAtSlot;
    SqVC_GetPlayerAmmoAtSlot                = vcapi->GetPlayerAmmoAtSlot;
    SqVC_RemovePlayerWeapon                 = vcapi->RemovePlayerWeapon;
    SqVC_RemoveAllWeapons                   = vcapi->RemoveAllWeapons;

    // --------------------------------------------------------------------------------------------
    //PLAYERS: camera
    SqVC_SetCameraPosition                  = vcapi->SetCameraPosition;
    SqVC_RestoreCamera                      = vcapi->RestoreCamera;
    SqVC_IsCameraLocked                     = vcapi->IsCameraLocked;

    // --------------------------------------------------------------------------------------------
    //PLAYERS: misc
    SqVC_SetPlayerAnimation                 = vcapi->SetPlayerAnimation;
    SqVC_SetPlayerWantedLevel               = vcapi->SetPlayerWantedLevel;
    SqVC_GetPlayerStandingOnVehicle         = vcapi->GetPlayerStandingOnVehicle;
    SqVC_GetPlayerStandingOnObject          = vcapi->GetPlayerStandingOnObject;
    SqVC_IsPlayerAway                       = vcapi->IsPlayerAway;
    SqVC_GetPlayerSpectateTarget            = vcapi->GetPlayerSpectateTarget;
    SqVC_SetPlayerSpectateTarget            = vcapi->SetPlayerSpectateTarget;
    SqVC_RedirectPlayerToServer             = vcapi->RedirectPlayerToServer;

    // --------------------------------------------------------------------------------------------
    //VEHICLES
    SqVC_CreateVehicle                      = vcapi->CreateVehicle;
    SqVC_DeleteVehicle                      = vcapi->DeleteVehicle;
    SqVC_GetVehicleSyncSource               = vcapi->GetVehicleSyncSource;
    SqVC_GetVehicleSyncType                 = vcapi->GetVehicleSyncType;
    SqVC_IsVehicleStreamedForPlayer         = vcapi->IsVehicleStreamedForPlayer;
    SqVC_SetVehicleWorld                    = vcapi->SetVehicleWorld;
    SqVC_GetVehicleWorld                    = vcapi->GetVehicleWorld;
    SqVC_GetVehicleModel                    = vcapi->GetVehicleModel;
    SqVC_GetVehicleOccupant                 = vcapi->GetVehicleOccupant;
    SqVC_RespawnVehicle                     = vcapi->RespawnVehicle;
    SqVC_SetVehicleImmunityFlags            = vcapi->SetVehicleImmunityFlags;
    SqVC_GetVehicleImmunityFlags            = vcapi->GetVehicleImmunityFlags;
    SqVC_KillVehicle                        = vcapi->KillVehicle;
    SqVC_IsVehicleWrecked                   = vcapi->IsVehicleWrecked;
    SqVC_SetVehiclePos                      = vcapi->SetVehiclePos;
    SqVC_GetVehiclePos                      = vcapi->GetVehiclePos;
    SqVC_SetVehicleRot                      = vcapi->SetVehicleRot;
    SqVC_SetVehicleRotEuler                 = vcapi->SetVehicleRotEuler;
    SqVC_GetVehicleRot                      = vcapi->GetVehicleRot;
    SqVC_GetVehicleRotEuler                 = vcapi->GetVehicleRotEuler;
    SqVC_SetVehicleSpeed                    = vcapi->SetVehicleSpeed;
    SqVC_GetVehicleSpeed                    = vcapi->GetVehicleSpeed;
    SqVC_AddVehicleSpeed                    = vcapi->AddVehicleSpeed;
    SqVC_SetVehicleRelSpeed                 = vcapi->SetVehicleRelSpeed;
    SqVC_GetVehicleRelSpeed                 = vcapi->GetVehicleRelSpeed;
    SqVC_AddVehicleRelSpeed                 = vcapi->AddVehicleRelSpeed;
    SqVC_SetVehicleTurnSpeed                = vcapi->SetVehicleTurnSpeed;
    SqVC_GetVehicleTurnSpeed                = vcapi->GetVehicleTurnSpeed;
    SqVC_AddVehicleTurnSpeed                = vcapi->AddVehicleTurnSpeed;
    SqVC_SetVehicleRelTurnSpeed             = vcapi->SetVehicleRelTurnSpeed;
    SqVC_GetVehicleRelTurnSpeed             = vcapi->GetVehicleRelTurnSpeed;
    SqVC_AddVehicleRelTurnSpeed             = vcapi->AddVehicleRelTurnSpeed;
    SqVC_SetVehicleSpawnPos                 = vcapi->SetVehicleSpawnPos;
    SqVC_GetVehicleSpawnPos                 = vcapi->GetVehicleSpawnPos;
    SqVC_SetVehicleSpawnRot                 = vcapi->SetVehicleSpawnRot;
    SqVC_SetVehicleSpawnRotEuler            = vcapi->SetVehicleSpawnRotEuler;
    SqVC_GetVehicleSpawnRot                 = vcapi->GetVehicleSpawnRot;
    SqVC_GetVehicleSpawnRotEuler            = vcapi->GetVehicleSpawnRotEuler;
    SqVC_SetVehicleIdleRespawnTimer         = vcapi->SetVehicleIdleRespawnTimer;
    SqVC_GetVehicleIdleRespawnTimer         = vcapi->GetVehicleIdleRespawnTimer;
    SqVC_SetVehicleHealth                   = vcapi->SetVehicleHealth;
    SqVC_GetVehicleHealth                   = vcapi->GetVehicleHealth;
    SqVC_SetVehicleColour                   = vcapi->SetVehicleColour;
    SqVC_GetVehicleColour                   = vcapi->GetVehicleColour;
    SqVC_SetVehicleDoorsLocked              = vcapi->SetVehicleDoorsLocked;
    SqVC_GetVehicleDoorsLocked              = vcapi->GetVehicleDoorsLocked;
    SqVC_SetVehiclePartStatus               = vcapi->SetVehiclePartStatus;
    SqVC_GetVehiclePartStatus               = vcapi->GetVehiclePartStatus;
    SqVC_SetVehicleTyreStatus               = vcapi->SetVehicleTyreStatus;
    SqVC_GetVehicleTyreStatus               = vcapi->GetVehicleTyreStatus;
    SqVC_SetVehicleDamageData               = vcapi->SetVehicleDamageData;
    SqVC_GetVehicleDamageData               = vcapi->GetVehicleDamageData;
    SqVC_SetVehicleAlarm                    = vcapi->SetVehicleAlarm;
    SqVC_GetVehicleAlarm                    = vcapi->GetVehicleAlarm;
    SqVC_SetVehicleLights                   = vcapi->SetVehicleLights;
    SqVC_GetVehicleLights                   = vcapi->GetVehicleLights;
    SqVC_SetVehicleRadio                    = vcapi->SetVehicleRadio;
    SqVC_GetVehicleRadio                    = vcapi->GetVehicleRadio;
    SqVC_SetVehicleRadioLocked              = vcapi->SetVehicleRadioLocked;
    SqVC_IsVehicleRadioLocked               = vcapi->IsVehicleRadioLocked;
    SqVC_GetVehicleGhostState               = vcapi->GetVehicleGhostState;
    SqVC_SetVehicleGhostState               = vcapi->SetVehicleGhostState;
    SqVC_GetVehicleTurretRotation           = vcapi->GetVehicleTurretRotation;

    // --------------------------------------------------------------------------------------------
    //VEHICLES: handling
    SqVC_ResetAllVehicleHandlings           = vcapi->ResetAllVehicleHandlings;
    SqVC_ExistsHandlingRule                 = vcapi->ExistsHandlingRule;
    SqVC_SetHandlingRule                    = vcapi->SetHandlingRule;
    SqVC_GetHandlingRule                    = vcapi->GetHandlingRule;
    SqVC_ResetHandlingRule                  = vcapi->ResetHandlingRule;
    SqVC_ResetHandling                      = vcapi->ResetHandling;
    SqVC_ExistsInstHandlingRule             = vcapi->ExistsInstHandlingRule;
    SqVC_SetInstHandlingRule                = vcapi->SetInstHandlingRule;
    SqVC_GetInstHandlingRule                = vcapi->GetInstHandlingRule;
    SqVC_ResetInstHandlingRule              = vcapi->ResetInstHandlingRule;
    SqVC_ResetInstHandling                  = vcapi->ResetInstHandling;

    // --------------------------------------------------------------------------------------------
    //PICKUPS
    SqVC_CreatePickup                       = vcapi->CreatePickup;
    SqVC_DeletePickup                       = vcapi->DeletePickup;
    SqVC_IsPickupStreamedForPlayer          = vcapi->IsPickupStreamedForPlayer;
    SqVC_SetPickupWorld                     = vcapi->SetPickupWorld;
    SqVC_GetPickupWorld                     = vcapi->GetPickupWorld;
    SqVC_PickupGetAlpha                     = vcapi->PickupGetAlpha;
    SqVC_PickupSetAlpha                     = vcapi->PickupSetAlpha;
    SqVC_PickupIsAutomatic                  = vcapi->PickupIsAutomatic;
    SqVC_PickupSetAutomatic                 = vcapi->PickupSetAutomatic;
    SqVC_SetPickupAutoTimer                 = vcapi->SetPickupAutoTimer;
    SqVC_GetPickupAutoTimer                 = vcapi->GetPickupAutoTimer;
    SqVC_PickupRefresh                      = vcapi->PickupRefresh;
    SqVC_PickupGetPos                       = vcapi->PickupGetPos;
    SqVC_PickupSetPos                       = vcapi->PickupSetPos;
    SqVC_PickupGetModel                     = vcapi->PickupGetModel;
    SqVC_PickupGetQuantity                  = vcapi->PickupGetQuantity;

    // --------------------------------------------------------------------------------------------
    //OBJECTS
    SqVC_CreateObject                       = vcapi->CreateObject;
    SqVC_DeleteObject                       = vcapi->DeleteObject;
    SqVC_IsObjectStreamedForPlayer          = vcapi->IsObjectStreamedForPlayer;
    SqVC_GetObjectModel                     = vcapi->GetObjectModel;
    SqVC_SetObjectWorld                     = vcapi->SetObjectWorld;
    SqVC_GetObjectWorld                     = vcapi->GetObjectWorld;
    SqVC_SetObjectAlpha                     = vcapi->SetObjectAlpha;
    SqVC_GetObjectAlpha                     = vcapi->GetObjectAlpha;
    SqVC_MoveObjectTo                       = vcapi->MoveObjectTo;
    SqVC_MoveObjectBy                       = vcapi->MoveObjectBy;
    SqVC_SetObjectPos                       = vcapi->SetObjectPos;
    SqVC_GetObjectPos                       = vcapi->GetObjectPos;
    SqVC_RotObjectTo                        = vcapi->RotObjectTo;
    SqVC_RotObjectToEuler                   = vcapi->RotObjectToEuler;
    SqVC_RotObjectBy                        = vcapi->RotObjectBy;
    SqVC_RotObjectByEuler                   = vcapi->RotObjectByEuler;
    SqVC_GetObjectRot                       = vcapi->GetObjectRot;
    SqVC_GetObjectRotEuler                  = vcapi->GetObjectRotEuler;
    SqVC_SetObjectShotReport                = vcapi->SetObjectShotReport;
    SqVC_IsObjectShotReport                 = vcapi->IsObjectShotReport;
    SqVC_SetObjectBumpReport                = vcapi->SetObjectBumpReport;
    SqVC_IsObjectBumpReport                 = vcapi->IsObjectBumpReport;

    // --------------------------------------------------------------------------------------------
    // TODO: Move these functions to proper sections on major plugin update
    SqVC_ToggleWallglitch                   = vcapi->ToggleWallglitch;
    SqVC_EnabledWallglitch                  = vcapi->EnabledWallglitch;
    SqVC_SetVehicleSiren                    = vcapi->SetVehicleSiren;
    SqVC_GetVehicleSiren                    = vcapi->GetVehicleSiren;
    SqVC_GetPlayerUID2                      = vcapi->GetPlayerUID2;

    // --------------------------------------------------------------------------------------------
    SqVC_CreateCheckpoint                   = vcapi->CreateCheckpoint;
    SqVC_DeleteCheckpoint                   = vcapi->DeleteCheckpoint;
    SqVC_IsCheckpointStreamedForPlayer      = vcapi->IsCheckpointStreamedForPlayer;
    SqVC_SetCheckpointWorld                 = vcapi->SetCheckpointWorld;
    SqVC_GetCheckpointWorld                 = vcapi->GetCheckpointWorld;
    SqVC_SetCheckpointColor                 = vcapi->SetCheckpointColor;
    SqVC_GetCheckpointColor                 = vcapi->GetCheckpointColor;
    SqVC_SetCheckpointPos                   = vcapi->SetCheckpointPos;
    SqVC_GetCheckpointPos                   = vcapi->GetCheckpointPos;
    SqVC_SetCheckpointRadius                = vcapi->SetCheckpointRadius;
    SqVC_GetCheckpointRadius                = vcapi->GetCheckpointRadius;
    SqVC_GetCheckpointOwner                 = vcapi->GetCheckpointOwner;

    // --------------------------------------------------------------------------------------------
    SqVC_CreateSphere                       = vcapi->CreateSphere;
    SqVC_DeleteSphere                       = vcapi->DeleteSphere;
    SqVC_IsSphereStreamedForPlayer          = vcapi->IsSphereStreamedForPlayer;
    SqVC_SetSphereWorld                     = vcapi->SetSphereWorld;
    SqVC_GetSphereWorld                     = vcapi->GetSphereWorld;
    SqVC_SetSphereColor                     = vcapi->SetSphereColor;
    SqVC_GetSphereColor                     = vcapi->GetSphereColor;
    SqVC_SetSpherePos                       = vcapi->SetSpherePos;
    SqVC_GetSpherePos                       = vcapi->GetSpherePos;
    SqVC_SetSphereRadius                    = vcapi->SetSphereRadius;
    SqVC_GetSphereRadius                    = vcapi->GetSphereRadius;
    SqVC_GetSphereOwner                     = vcapi->GetSphereOwner;

    return SQ_OK;
}

// ------------------------------------------------------------------------------------------------
void vc_api_collapse()
{
    // --------------------------------------------------------------------------------------------
    //PLUGIN SYSTEM
    SqVC_GetServerVersion                   = NULL;
    SqVC_GetServerSettings                  = NULL;
    SqVC_ExportFunctions                    = NULL;
    SqVC_GetNumberOfPlugins                 = NULL;
    SqVC_GetPluginInfo                      = NULL;
    SqVC_FindPlugin                         = NULL;
    SqVC_GetPluginExports                   = NULL;
    SqVC_GetTime                            = NULL;

    // --------------------------------------------------------------------------------------------
    //MESSAGES
    SqVC_printf                             = NULL;
    SqVC_SendCustomCommand                  = NULL;
    SqVC_SendClientMessage                  = NULL;
    SqVC_SendGameMessage                    = NULL;

    // --------------------------------------------------------------------------------------------
    //SERVER SETTINGS
    SqVC_SetServerName                      = NULL;
    SqVC_GetServerName                      = NULL;
    SqVC_SetMaxPlayers                      = NULL;
    SqVC_GetMaxPlayers                      = NULL;
    SqVC_SetServerPassword                  = NULL;
    SqVC_GetServerPassword                  = NULL;
    SqVC_SetGameModeText                    = NULL;
    SqVC_GetGameModeText                    = NULL;
    SqVC_ShutdownServer                     = NULL;

    // --------------------------------------------------------------------------------------------
    //WORLD: settings
    SqVC_SetWorldBounds                     = NULL;
    SqVC_GetWorldBounds                     = NULL;
    SqVC_SetWastedSettings                  = NULL;
    SqVC_GetWastedSettings                  = NULL;
    SqVC_SetTimeRate                        = NULL;
    SqVC_GetTimeRate                        = NULL;
    SqVC_SetHour                            = NULL;
    SqVC_GetHour                            = NULL;
    SqVC_SetMinute                          = NULL;
    SqVC_GetMinute                          = NULL;
    SqVC_SetWeather                         = NULL;
    SqVC_GetWeather                         = NULL;
    SqVC_SetGravity                         = NULL;
    SqVC_GetGravity                         = NULL;
    SqVC_SetGamespeed                       = NULL;
    SqVC_GetGamespeed                       = NULL;
    SqVC_SetWaterLevel                      = NULL;
    SqVC_GetWaterLevel                      = NULL;
    SqVC_SetMaxHeight                       = NULL;
    SqVC_GetMaxHeight                       = NULL;
    SqVC_SetKillCmdDelay                    = NULL;
    SqVC_GetKillCmdDelay                    = NULL;
    SqVC_SetVehiclesForcedRespawnHeight     = NULL;
    SqVC_GetVehiclesForcedRespawnHeight     = NULL;

    // --------------------------------------------------------------------------------------------
    //WORLD: toggles
    SqVC_ToggleSyncFrameLimiter             = NULL;
    SqVC_EnabledSyncFrameLimiter            = NULL;
    SqVC_ToggleFrameLimiter                 = NULL;
    SqVC_EnabledFrameLimiter                = NULL;
    SqVC_ToggleTaxiBoostJump                = NULL;
    SqVC_EnabledTaxiBoostJump               = NULL;
    SqVC_ToggleDriveOnWater                 = NULL;
    SqVC_EnabledDriveOnWater                = NULL;
    SqVC_ToggleFastSwitch                   = NULL;
    SqVC_EnabledFastSwitch                  = NULL;
    SqVC_ToggleFriendlyFire                 = NULL;
    SqVC_EnabledFriendlyFire                = NULL;
    SqVC_ToggleDisableDriveby               = NULL;
    SqVC_EnabledDisableDriveby              = NULL;
    SqVC_TogglePerfectHandling              = NULL;
    SqVC_EnabledPerfectHandling             = NULL;
    SqVC_ToggleFlyingCars                   = NULL;
    SqVC_EnabledFlyingCars                  = NULL;
    SqVC_ToggleJumpSwitch                   = NULL;
    SqVC_EnabledJumpSwitch                  = NULL;
    SqVC_ToggleShowMarkers                  = NULL;
    SqVC_EnabledShowMarkers                 = NULL;
    SqVC_ToggleOnlyShowTeamMarkers          = NULL;
    SqVC_EnabledOnlyShowTeamMarkers         = NULL;
    SqVC_ToggleStuntBike                    = NULL;
    SqVC_EnabledStuntBike                   = NULL;
    SqVC_ToggleShootInAir                   = NULL;
    SqVC_EnabledShootInAir                  = NULL;
    SqVC_ToggleShowNametags                 = NULL;
    SqVC_EnabledShowNametags                = NULL;
    SqVC_ToggleJoinMessages                 = NULL;
    SqVC_EnabledJoinMessages                = NULL;
    SqVC_ToggleDeathMessages                = NULL;
    SqVC_EnabledDeathMessages               = NULL;
    SqVC_ToggleChatTagsByDefaultEnabled     = NULL;
    SqVC_EnabledChatTagsByDefault           = NULL;

    // --------------------------------------------------------------------------------------------
    //MISC
    SqVC_CreateExplosion                    = NULL;
    SqVC_PlaySound                          = NULL;
    SqVC_HideMapObject                      = NULL;
    SqVC_ShowMapObject                      = NULL;
    SqVC_ShowAllMapObjects                  = NULL;

    //WEAPONDATA
    SqVC_SetWeaponDataValue                 = NULL;
    SqVC_GetWeaponDataValue                 = NULL;
    SqVC_ResetWeaponDataValue               = NULL;
    SqVC_IsWeaponDataValueModified          = NULL;
    SqVC_ResetWeaponData                    = NULL;
    SqVC_ResetAllWeaponData                 = NULL;

    // --------------------------------------------------------------------------------------------
    //KEYBINDS
    SqVC_GetKeyBindUnusedSlot               = NULL;
    SqVC_GetKeyBindData                     = NULL;
    SqVC_RegisterKeyBind                    = NULL;
    SqVC_RemoveKeyBind                      = NULL;
    SqVC_RemoveAllKeyBinds                  = NULL;

    //BLIPS
    SqVC_CreateCoordBlip                    = NULL;
    SqVC_DestroyCoordBlip                   = NULL;
    SqVC_GetCoordBlipInfo                   = NULL;

    // --------------------------------------------------------------------------------------------
    //SPRITES
    SqVC_CreateSprite                       = NULL;
    SqVC_DestroySprite                      = NULL;
    SqVC_ShowSprite                         = NULL;
    SqVC_HideSprite                         = NULL;
    SqVC_MoveSprite                         = NULL;
    SqVC_SetSpriteCenter                    = NULL;
    SqVC_RotateSprite                       = NULL;
    SqVC_SetSpriteAlpha                     = NULL;
    SqVC_SetSpriteRelativity                = NULL;

    //TEXTDRAWS
    SqVC_CreateTextdraw                     = NULL;
    SqVC_DestroyTextdraw                    = NULL;
    SqVC_ShowTextdraw                       = NULL;
    SqVC_HideTextdraw                       = NULL;
    SqVC_MoveTextdraw                       = NULL;
    SqVC_SetTextdrawColour                  = NULL;
    SqVC_SetTextdrawRelativity              = NULL;

    // --------------------------------------------------------------------------------------------
    //RADIOS
    SqVC_AddRadioStream                     = NULL;
    SqVC_RemoveRadioStream                  = NULL;

    // --------------------------------------------------------------------------------------------
    //CLASSES
    SqVC_SetUseClasses                      = NULL;
    SqVC_GetUseClasses                      = NULL;
    SqVC_GetPlayerClass                     = NULL;
    SqVC_AddPlayerClass                     = NULL;
    SqVC_SetSpawnPlayerPos                  = NULL;
    SqVC_SetSpawnCameraPos                  = NULL;
    SqVC_SetSpawnCameraLookAt               = NULL;

    // --------------------------------------------------------------------------------------------
    //ADMIN
    SqVC_IsPlayerAdmin                      = NULL;
    SqVC_SetPlayerAdmin                     = NULL;
    SqVC_GetPlayerIP                        = NULL;
    SqVC_KickPlayer                         = NULL;
    SqVC_BanPlayer                          = NULL;
    SqVC_BanIP                              = NULL;
    SqVC_UnbanIP                            = NULL;
    SqVC_IsIPBanned                         = NULL;

    // --------------------------------------------------------------------------------------------
    //PLAYERS: basic
    SqVC_GetPlayerIDFromName                = NULL;
    SqVC_IsPlayerConnected                  = NULL;
    SqVC_IsPlayerSpawned                    = NULL;
    SqVC_IsPlayerStreamedForPlayer          = NULL;
    SqVC_GetPlayerKey                       = NULL;
    SqVC_SetPlayerWorld                     = NULL;
    SqVC_GetPlayerWorld                     = NULL;
    SqVC_SetPlayerSecWorld                  = NULL;
    SqVC_GetPlayerSecWorld                  = NULL;
    SqVC_GetPlayerUniqueWorld               = NULL;
    SqVC_IsPlayerWorldCompatible            = NULL;
    SqVC_GetPlayerState                     = NULL;
    SqVC_GetPlayerName                      = NULL;
    SqVC_SetPlayerName                      = NULL;
    SqVC_SetPlayerTeam                      = NULL;
    SqVC_GetPlayerTeam                      = NULL;
    SqVC_SetPlayerSkin                      = NULL;
    SqVC_GetPlayerSkin                      = NULL;
    SqVC_SetPlayerColour                    = NULL;
    SqVC_GetPlayerColour                    = NULL;
    SqVC_ForcePlayerSpawn                   = NULL;
    SqVC_ForcePlayerSelect                  = NULL;
    SqVC_ForceAllSelect                     = NULL;

    // --------------------------------------------------------------------------------------------
    //PLAYERS: score, ping, money, typing
    SqVC_GivePlayerMoney                    = NULL;
    SqVC_SetPlayerMoney                     = NULL;
    SqVC_GetPlayerMoney                     = NULL;
    SqVC_SetPlayerScore                     = NULL;
    SqVC_GetPlayerScore                     = NULL;
    SqVC_GetPlayerPing                      = NULL;
    SqVC_IsPlayerTyping                     = NULL;
    SqVC_GetPlayerFPS                       = NULL;
    SqVC_GetPlayerUID                       = NULL;
    SqVC_GetPlayerWantedLevel               = NULL;

    // --------------------------------------------------------------------------------------------
    //PLAYERS: health and location
    SqVC_SetPlayerHealth                    = NULL;
    SqVC_GetPlayerHealth                    = NULL;
    SqVC_SetPlayerArmour                    = NULL;
    SqVC_GetPlayerArmour                    = NULL;
    SqVC_SetPlayerImmunityFlags             = NULL;
    SqVC_GetPlayerImmunityFlags             = NULL;
    SqVC_SetPlayerPos                       = NULL;
    SqVC_GetPlayerPos                       = NULL;
    SqVC_SetPlayerSpeed                     = NULL;
    SqVC_GetPlayerSpeed                     = NULL;
    SqVC_AddPlayerSpeed                     = NULL;
    SqVC_SetPlayerHeading                   = NULL;
    SqVC_GetPlayerHeading                   = NULL;
    SqVC_SetPlayerAlpha                     = NULL;
    SqVC_GetPlayerAlpha                     = NULL;
    SqVC_GetPlayerOnFireStatus              = NULL;
    SqVC_GetPlayerCrouchStatus              = NULL;
    SqVC_GetPlayerAction                    = NULL;
    SqVC_GetPlayerGameKeys                  = NULL;
    SqVC_GetPlayerAimPos                    = NULL;
    SqVC_GetPlayerAimDir                    = NULL;

    // --------------------------------------------------------------------------------------------
    //PLAYERS: vehicle
    SqVC_PutPlayerInVehicle                 = NULL;
    SqVC_RemovePlayerFromVehicle            = NULL;
    SqVC_GetPlayerInVehicleStatus           = NULL;
    SqVC_GetPlayerInVehicleSlot             = NULL;
    SqVC_GetPlayerVehicleID                 = NULL;

    // --------------------------------------------------------------------------------------------
    //PLAYERS: toggles
    SqVC_TogglePlayerControllable           = NULL;
    SqVC_EnabledPlayerControllable          = NULL;
    SqVC_TogglePlayerDriveby                = NULL;
    SqVC_EnabledPlayerDriveby               = NULL;
    SqVC_TogglePlayerWhiteScanlines         = NULL;
    SqVC_EnabledPlayerWhiteScanlines        = NULL;
    SqVC_TogglePlayerGreenScanlines         = NULL;
    SqVC_EnabledPlayerGreenScanlines        = NULL;
    SqVC_TogglePlayerWidescreen             = NULL;
    SqVC_EnabledPlayerWidescreen            = NULL;
    SqVC_TogglePlayerShowMarkers            = NULL;
    SqVC_EnabledPlayerShowMarkers           = NULL;
    SqVC_TogglePlayerAttackPriv             = NULL;
    SqVC_EnabledPlayerAttackPriv            = NULL;
    SqVC_TogglePlayerHasMarker              = NULL;
    SqVC_EnabledPlayerHasMarker             = NULL;
    SqVC_TogglePlayerChatTagsEnabled        = NULL;
    SqVC_EnabledPlayerChatTags              = NULL;
    SqVC_TogglePlayerDrunkEffects           = NULL;
    SqVC_EnabledPlayerDrunkEffects          = NULL;

    // --------------------------------------------------------------------------------------------
    //PLAYERS: weapons
    SqVC_GivePlayerWeapon                   = NULL;
    SqVC_SetPlayerWeapon                    = NULL;
    SqVC_GetPlayerWeapon                    = NULL;
    SqVC_GetPlayerWeaponAmmo                = NULL;
    SqVC_SetPlayerWeaponSlot                = NULL;
    SqVC_GetPlayerWeaponSlot                = NULL;
    SqVC_GetPlayerWeaponAtSlot              = NULL;
    SqVC_GetPlayerAmmoAtSlot                = NULL;
    SqVC_RemovePlayerWeapon                 = NULL;
    SqVC_RemoveAllWeapons                   = NULL;

    // --------------------------------------------------------------------------------------------
    //PLAYERS: camera
    SqVC_SetCameraPosition                  = NULL;
    SqVC_RestoreCamera                      = NULL;
    SqVC_IsCameraLocked                     = NULL;

    // --------------------------------------------------------------------------------------------
    //PLAYERS: misc
    SqVC_SetPlayerAnimation                 = NULL;
    SqVC_SetPlayerWantedLevel               = NULL;
    SqVC_GetPlayerStandingOnVehicle         = NULL;
    SqVC_GetPlayerStandingOnObject          = NULL;
    SqVC_IsPlayerAway                       = NULL;
    SqVC_GetPlayerSpectateTarget            = NULL;
    SqVC_SetPlayerSpectateTarget            = NULL;
    SqVC_RedirectPlayerToServer             = NULL;

    // --------------------------------------------------------------------------------------------
    //VEHICLES
    SqVC_CreateVehicle                      = NULL;
    SqVC_DeleteVehicle                      = NULL;
    SqVC_GetVehicleSyncSource               = NULL;
    SqVC_GetVehicleSyncType                 = NULL;
    SqVC_IsVehicleStreamedForPlayer         = NULL;
    SqVC_SetVehicleWorld                    = NULL;
    SqVC_GetVehicleWorld                    = NULL;
    SqVC_GetVehicleModel                    = NULL;
    SqVC_GetVehicleOccupant                 = NULL;
    SqVC_RespawnVehicle                     = NULL;
    SqVC_SetVehicleImmunityFlags            = NULL;
    SqVC_GetVehicleImmunityFlags            = NULL;
    SqVC_KillVehicle                        = NULL;
    SqVC_IsVehicleWrecked                   = NULL;
    SqVC_SetVehiclePos                      = NULL;
    SqVC_GetVehiclePos                      = NULL;
    SqVC_SetVehicleRot                      = NULL;
    SqVC_SetVehicleRotEuler                 = NULL;
    SqVC_GetVehicleRot                      = NULL;
    SqVC_GetVehicleRotEuler                 = NULL;
    SqVC_SetVehicleSpeed                    = NULL;
    SqVC_GetVehicleSpeed                    = NULL;
    SqVC_AddVehicleSpeed                    = NULL;
    SqVC_SetVehicleRelSpeed                 = NULL;
    SqVC_GetVehicleRelSpeed                 = NULL;
    SqVC_AddVehicleRelSpeed                 = NULL;
    SqVC_SetVehicleTurnSpeed                = NULL;
    SqVC_GetVehicleTurnSpeed                = NULL;
    SqVC_AddVehicleTurnSpeed                = NULL;
    SqVC_SetVehicleRelTurnSpeed             = NULL;
    SqVC_GetVehicleRelTurnSpeed             = NULL;
    SqVC_AddVehicleRelTurnSpeed             = NULL;
    SqVC_SetVehicleSpawnPos                 = NULL;
    SqVC_GetVehicleSpawnPos                 = NULL;
    SqVC_SetVehicleSpawnRot                 = NULL;
    SqVC_SetVehicleSpawnRotEuler            = NULL;
    SqVC_GetVehicleSpawnRot                 = NULL;
    SqVC_GetVehicleSpawnRotEuler            = NULL;
    SqVC_SetVehicleIdleRespawnTimer         = NULL;
    SqVC_GetVehicleIdleRespawnTimer         = NULL;
    SqVC_SetVehicleHealth                   = NULL;
    SqVC_GetVehicleHealth                   = NULL;
    SqVC_SetVehicleColour                   = NULL;
    SqVC_GetVehicleColour                   = NULL;
    SqVC_SetVehicleDoorsLocked              = NULL;
    SqVC_GetVehicleDoorsLocked              = NULL;
    SqVC_SetVehiclePartStatus               = NULL;
    SqVC_GetVehiclePartStatus               = NULL;
    SqVC_SetVehicleTyreStatus               = NULL;
    SqVC_GetVehicleTyreStatus               = NULL;
    SqVC_SetVehicleDamageData               = NULL;
    SqVC_GetVehicleDamageData               = NULL;
    SqVC_SetVehicleAlarm                    = NULL;
    SqVC_GetVehicleAlarm                    = NULL;
    SqVC_SetVehicleLights                   = NULL;
    SqVC_GetVehicleLights                   = NULL;
    SqVC_SetVehicleRadio                    = NULL;
    SqVC_GetVehicleRadio                    = NULL;
    SqVC_SetVehicleRadioLocked              = NULL;
    SqVC_IsVehicleRadioLocked               = NULL;
    SqVC_GetVehicleGhostState               = NULL;
    SqVC_SetVehicleGhostState               = NULL;
    SqVC_GetVehicleTurretRotation           = NULL;

    // --------------------------------------------------------------------------------------------
    //VEHICLES: handling
    SqVC_ResetAllVehicleHandlings           = NULL;
    SqVC_ExistsHandlingRule                 = NULL;
    SqVC_SetHandlingRule                    = NULL;
    SqVC_GetHandlingRule                    = NULL;
    SqVC_ResetHandlingRule                  = NULL;
    SqVC_ResetHandling                      = NULL;
    SqVC_ExistsInstHandlingRule             = NULL;
    SqVC_SetInstHandlingRule                = NULL;
    SqVC_GetInstHandlingRule                = NULL;
    SqVC_ResetInstHandlingRule              = NULL;
    SqVC_ResetInstHandling                  = NULL;

    // --------------------------------------------------------------------------------------------
    //PICKUPS
    SqVC_CreatePickup                       = NULL;
    SqVC_DeletePickup                       = NULL;
    SqVC_IsPickupStreamedForPlayer          = NULL;
    SqVC_SetPickupWorld                     = NULL;
    SqVC_GetPickupWorld                     = NULL;
    SqVC_PickupGetAlpha                     = NULL;
    SqVC_PickupSetAlpha                     = NULL;
    SqVC_PickupIsAutomatic                  = NULL;
    SqVC_PickupSetAutomatic                 = NULL;
    SqVC_SetPickupAutoTimer                 = NULL;
    SqVC_GetPickupAutoTimer                 = NULL;
    SqVC_PickupRefresh                      = NULL;
    SqVC_PickupGetPos                       = NULL;
    SqVC_PickupSetPos                       = NULL;
    SqVC_PickupGetModel                     = NULL;
    SqVC_PickupGetQuantity                  = NULL;

    // --------------------------------------------------------------------------------------------
    //OBJECTS
    SqVC_CreateObject                       = NULL;
    SqVC_DeleteObject                       = NULL;
    SqVC_IsObjectStreamedForPlayer          = NULL;
    SqVC_GetObjectModel                     = NULL;
    SqVC_SetObjectWorld                     = NULL;
    SqVC_GetObjectWorld                     = NULL;
    SqVC_SetObjectAlpha                     = NULL;
    SqVC_GetObjectAlpha                     = NULL;
    SqVC_MoveObjectTo                       = NULL;
    SqVC_MoveObjectBy                       = NULL;
    SqVC_SetObjectPos                       = NULL;
    SqVC_GetObjectPos                       = NULL;
    SqVC_RotObjectTo                        = NULL;
    SqVC_RotObjectToEuler                   = NULL;
    SqVC_RotObjectBy                        = NULL;
    SqVC_RotObjectByEuler                   = NULL;
    SqVC_GetObjectRot                       = NULL;
    SqVC_GetObjectRotEuler                  = NULL;
    SqVC_SetObjectShotReport                = NULL;
    SqVC_IsObjectShotReport                 = NULL;
    SqVC_SetObjectBumpReport                = NULL;
    SqVC_IsObjectBumpReport                 = NULL;

    // --------------------------------------------------------------------------------------------
    // TODO: Move these functions to proper sections on major plugin update
    SqVC_ToggleWallglitch                   = NULL;
    SqVC_EnabledWallglitch                  = NULL;
    SqVC_SetVehicleSiren                    = NULL;
    SqVC_GetVehicleSiren                    = NULL;
    SqVC_GetPlayerUID2                      = NULL;

    // --------------------------------------------------------------------------------------------
    SqVC_CreateCheckpoint                   = NULL;
    SqVC_DeleteCheckpoint                   = NULL;
    SqVC_IsCheckpointStreamedForPlayer      = NULL;
    SqVC_SetCheckpointWorld                 = NULL;
    SqVC_GetCheckpointWorld                 = NULL;
    SqVC_SetCheckpointColor                 = NULL;
    SqVC_GetCheckpointColor                 = NULL;
    SqVC_SetCheckpointPos                   = NULL;
    SqVC_GetCheckpointPos                   = NULL;
    SqVC_SetCheckpointRadius                = NULL;
    SqVC_GetCheckpointRadius                = NULL;
    SqVC_GetCheckpointOwner                 = NULL;

    // --------------------------------------------------------------------------------------------
    SqVC_CreateSphere                       = NULL;
    SqVC_DeleteSphere                       = NULL;
    SqVC_IsSphereStreamedForPlayer          = NULL;
    SqVC_SetSphereWorld                     = NULL;
    SqVC_GetSphereWorld                     = NULL;
    SqVC_SetSphereColor                     = NULL;
    SqVC_GetSphereColor                     = NULL;
    SqVC_SetSpherePos                       = NULL;
    SqVC_GetSpherePos                       = NULL;
    SqVC_SetSphereRadius                    = NULL;
    SqVC_GetSphereRadius                    = NULL;
    SqVC_GetSphereOwner                     = NULL;
}