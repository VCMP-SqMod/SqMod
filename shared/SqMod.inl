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
sqapi_loadfile                      sqstd_loadfile = NULL;
sqapi_dofile                        sqstd_dofile = NULL;
sqapi_writeclosuretofile            sqstd_writeclosuretofile = NULL;

/*blob*/
sqapi_createblob                    sqstd_createblob = NULL;
sqapi_getblob                       sqstd_getblob = NULL;
sqapi_getblobsize                   sqstd_getblobsize = NULL;

/*string*/
sqapi_format                        sqstd_format = NULL;

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
    const void ** plugin_exports = vcapi->GetPluginExports(plugin_id, &struct_size);
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
    sqstd_loadfile                  = sqapi->loadfile;
    sqstd_dofile                    = sqapi->dofile;
    sqstd_writeclosuretofile        = sqapi->writeclosuretofile;

    /*blob*/
    sqstd_createblob                = sqapi->createblob;
    sqstd_getblob                   = sqapi->getblob;
    sqstd_getblobsize               = sqapi->getblobsize;

    /*string*/
    sqstd_format                    = sqapi->format;

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
    sqstd_loadfile                  = NULL;
    sqstd_dofile                    = NULL;
    sqstd_writeclosuretofile        = NULL;

    /*blob*/
    sqstd_createblob                = NULL;
    sqstd_getblob                   = NULL;
    sqstd_getblobsize               = NULL;

    /*string*/
    sqstd_format                    = NULL;

#endif // SQMOD_PLUGIN_API
}
