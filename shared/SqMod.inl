// ------------------------------------------------------------------------------------------------
#ifdef SQMOD_PLUGIN_API

//primitive functions
SqModAPI_GetSquirrelVM                      SqMod_GetSquirrelVM                         = NULL;

//logging utilities
SqModAPI_LogMessage                         SqMod_LogDbg                                = NULL;
SqModAPI_LogMessage                         SqMod_LogUsr                                = NULL;
SqModAPI_LogMessage                         SqMod_LogScs                                = NULL;
SqModAPI_LogMessage                         SqMod_LogInf                                = NULL;
SqModAPI_LogMessage                         SqMod_LogWrn                                = NULL;
SqModAPI_LogMessage                         SqMod_LogErr                                = NULL;
SqModAPI_LogMessage                         SqMod_LogFtl                                = NULL;
SqModAPI_LogMessage                         SqMod_LogSDbg                               = NULL;
SqModAPI_LogMessage                         SqMod_LogSUsr                               = NULL;
SqModAPI_LogMessage                         SqMod_LogSScs                               = NULL;
SqModAPI_LogMessage                         SqMod_LogSInf                               = NULL;
SqModAPI_LogMessage                         SqMod_LogSWrn                               = NULL;
SqModAPI_LogMessage                         SqMod_LogSErr                               = NULL;
SqModAPI_LogMessage                         SqMod_LogSFtl                               = NULL;

//script loading
SqModAPI_LoadScript                         SqMod_LoadScript                            = NULL;

//numeric utilities
SqModAPI_GetSLongValue                      SqMod_GetSLongValue                         = NULL;
SqModAPI_PushSLongObject                    SqMod_PushSLongObject                       = NULL;
SqModAPI_GetULongValue                      SqMod_GetULongValue                         = NULL;
SqModAPI_PushULongObject                    SqMod_PushULongObject                       = NULL;

//time utilities
SqModAPI_GetCurrentSysTime                  SqMod_GetCurrentSysTime                     = NULL;
SqModAPI_GetEpochTimeMicro                  SqMod_GetEpochTimeMicro                     = NULL;
SqModAPI_GetEpochTimeMilli                  SqMod_GetEpochTimeMilli                     = NULL;
SqModAPI_ValidDate                          SqMod_ValidDate                             = NULL;
SqModAPI_IsLeapYear                         SqMod_IsLeapYear                            = NULL;
SqModAPI_DaysInYear                         SqMod_DaysInYear                            = NULL;
SqModAPI_DaysInMonth                        SqMod_DaysInMonth                           = NULL;
SqModAPI_DayOfYear                          SqMod_DayOfYear                             = NULL;
SqModAPI_DateRangeToSeconds                 SqMod_DateRangeToSeconds                    = NULL;
SqModAPI_GetTimestamp                       SqMod_GetTimestamp                          = NULL;
SqModAPI_PushTimestamp                      SqMod_PushTimestamp                         = NULL;
SqModAPI_GetDate                            SqMod_GetDate                               = NULL;
SqModAPI_PushDate                           SqMod_PushDate                              = NULL;
SqModAPI_GetTime                            SqMod_GetTime                               = NULL;
SqModAPI_PushTime                           SqMod_PushTime                              = NULL;
SqModAPI_GetDatetime                        SqMod_GetDatetime                           = NULL;
SqModAPI_PushDatetime                       SqMod_PushDatetime                          = NULL;

//stack utilities
SqModAPI_PopStackInteger                    SqMod_PopStackInteger                       = NULL;
SqModAPI_PopStackFloat                      SqMod_PopStackFloat                         = NULL;
SqModAPI_PopStackSLong                      SqMod_PopStackSLong                         = NULL;
SqModAPI_PopStackULong                      SqMod_PopStackULong                         = NULL;

//buffer utilities
SqModAPI_PushBuffer                         SqMod_PushBuffer                            = NULL;
SqModAPI_PushBufferData                     SqMod_PushBufferData                        = NULL;
SqModAPI_GetBufferInfo                      SqMod_GetBufferInfo                         = NULL;
SqModAPI_GetBufferData                      SqMod_GetBufferData                         = NULL;
SqModAPI_GetBufferSize                      SqMod_GetBufferSize                         = NULL;
SqModAPI_GetBufferCursor                    SqMod_GetBufferCursor                       = NULL;

#endif // SQMOD_PLUGIN_API

// ------------------------------------------------------------------------------------------------
uint8_t sqmod_api_expand(HSQMODAPI sqmodapi)
{
    if (!sqmodapi)
    {
        return 0;
    }

#ifdef SQMOD_PLUGIN_API

    //primitive functions
    SqMod_GetSquirrelVM                         = sqmodapi->GetSquirrelVM;

    //logging utilities
    SqMod_LogDbg                                = sqmodapi->LogDbg;
    SqMod_LogUsr                                = sqmodapi->LogUsr;
    SqMod_LogScs                                = sqmodapi->LogScs;
    SqMod_LogInf                                = sqmodapi->LogInf;
    SqMod_LogWrn                                = sqmodapi->LogWrn;
    SqMod_LogErr                                = sqmodapi->LogErr;
    SqMod_LogFtl                                = sqmodapi->LogFtl;
    SqMod_LogSDbg                               = sqmodapi->LogSDbg;
    SqMod_LogSUsr                               = sqmodapi->LogSUsr;
    SqMod_LogSScs                               = sqmodapi->LogSScs;
    SqMod_LogSInf                               = sqmodapi->LogSInf;
    SqMod_LogSWrn                               = sqmodapi->LogSWrn;
    SqMod_LogSErr                               = sqmodapi->LogSErr;
    SqMod_LogSFtl                               = sqmodapi->LogSFtl;

    //script loading
    SqMod_LoadScript                            = sqmodapi->LoadScript;

    //numeric utilities
    SqMod_GetSLongValue                         = sqmodapi->GetSLongValue;
    SqMod_PushSLongObject                       = sqmodapi->PushSLongObject;
    SqMod_GetULongValue                         = sqmodapi->GetULongValue;
    SqMod_PushULongObject                       = sqmodapi->PushULongObject;

    //time utilities
    SqMod_GetCurrentSysTime                     = sqmodapi->GetCurrentSysTime;
    SqMod_GetEpochTimeMicro                     = sqmodapi->GetEpochTimeMicro;
    SqMod_GetEpochTimeMilli                     = sqmodapi->GetEpochTimeMilli;
    SqMod_ValidDate                             = sqmodapi->ValidDate;
    SqMod_IsLeapYear                            = sqmodapi->IsLeapYear;
    SqMod_DaysInYear                            = sqmodapi->DaysInYear;
    SqMod_DaysInMonth                           = sqmodapi->DaysInMonth;
    SqMod_DayOfYear                             = sqmodapi->DayOfYear;
    SqMod_DateRangeToSeconds                    = sqmodapi->DateRangeToSeconds;
    SqMod_GetTimestamp                          = sqmodapi->GetTimestamp;
    SqMod_PushTimestamp                         = sqmodapi->PushTimestamp;
    SqMod_GetDate                               = sqmodapi->GetDate;
    SqMod_PushDate                              = sqmodapi->PushDate;
    SqMod_GetTime                               = sqmodapi->GetTime;
    SqMod_PushTime                              = sqmodapi->PushTime;
    SqMod_GetDatetime                           = sqmodapi->GetDatetime;
    SqMod_PushDatetime                          = sqmodapi->PushDatetime;

    //stack utilities
    SqMod_PopStackInteger                       = sqmodapi->PopStackInteger;
    SqMod_PopStackFloat                         = sqmodapi->PopStackFloat;
    SqMod_PopStackSLong                         = sqmodapi->PopStackSLong;
    SqMod_PopStackULong                         = sqmodapi->PopStackULong;

    //buffer utilities
    SqMod_PushBuffer                            = sqmodapi->PushBuffer;
    SqMod_PushBufferData                        = sqmodapi->PushBufferData;
    SqMod_GetBufferInfo                         = sqmodapi->GetBufferInfo;
    SqMod_GetBufferData                         = sqmodapi->GetBufferData;
    SqMod_GetBufferSize                         = sqmodapi->GetBufferSize;
    SqMod_GetBufferCursor                       = sqmodapi->GetBufferCursor;

#endif // SQMOD_PLUGIN_API

    return 1;
}

// ------------------------------------------------------------------------------------------------
void sqmod_api_collapse()
{
#ifdef SQMOD_PLUGIN_API

    //primitive functions
    SqMod_GetSquirrelVM                         = NULL;

    //logging utilities
    SqMod_LogDbg                                = NULL;
    SqMod_LogUsr                                = NULL;
    SqMod_LogScs                                = NULL;
    SqMod_LogInf                                = NULL;
    SqMod_LogWrn                                = NULL;
    SqMod_LogErr                                = NULL;
    SqMod_LogFtl                                = NULL;
    SqMod_LogSDbg                               = NULL;
    SqMod_LogSUsr                               = NULL;
    SqMod_LogSScs                               = NULL;
    SqMod_LogSInf                               = NULL;
    SqMod_LogSWrn                               = NULL;
    SqMod_LogSErr                               = NULL;
    SqMod_LogSFtl                               = NULL;

    //script loading
    SqMod_LoadScript                            = NULL;

    //numeric utilities
    SqMod_GetSLongValue                         = NULL;
    SqMod_PushSLongObject                       = NULL;
    SqMod_GetULongValue                         = NULL;
    SqMod_PushULongObject                       = NULL;

    //time utilities
    SqMod_GetCurrentSysTime                     = NULL;
    SqMod_GetEpochTimeMicro                     = NULL;
    SqMod_GetEpochTimeMilli                     = NULL;
    SqMod_ValidDate                             = NULL;
    SqMod_IsLeapYear                            = NULL;
    SqMod_DaysInYear                            = NULL;
    SqMod_DaysInMonth                           = NULL;
    SqMod_DayOfYear                             = NULL;
    SqMod_DateRangeToSeconds                    = NULL;
    SqMod_GetTimestamp                          = NULL;
    SqMod_PushTimestamp                         = NULL;
    SqMod_GetDate                               = NULL;
    SqMod_PushDate                              = NULL;
    SqMod_GetTime                               = NULL;
    SqMod_PushTime                              = NULL;
    SqMod_GetDatetime                           = NULL;
    SqMod_PushDatetime                          = NULL;

    //stack utilities
    SqMod_PopStackInteger                       = NULL;
    SqMod_PopStackFloat                         = NULL;
    SqMod_PopStackSLong                         = NULL;
    SqMod_PopStackULong                         = NULL;

    //buffer utilities
    SqMod_PushBuffer                            = NULL;
    SqMod_PushBufferData                        = NULL;
    SqMod_GetBufferInfo                         = NULL;
    SqMod_GetBufferData                         = NULL;
    SqMod_GetBufferSize                         = NULL;
    SqMod_GetBufferCursor                       = NULL;

#endif // SQMOD_PLUGIN_API
}

// ------------------------------------------------------------------------------------------------
#ifdef SQMOD_PLUGIN_API

//vm
SqLibAPI_open                               SqLib_open                                  = NULL;
SqLibAPI_newthread                          SqLib_newthread                             = NULL;
SqLibAPI_seterrorhandler                    SqLib_seterrorhandler                       = NULL;
SqLibAPI_close                              SqLib_close                                 = NULL;
SqLibAPI_setforeignptr                      SqLib_setforeignptr                         = NULL;
SqLibAPI_getforeignptr                      SqLib_getforeignptr                         = NULL;
SqLibAPI_setsharedforeignptr                SqLib_setsharedforeignptr                   = NULL;
SqLibAPI_getsharedforeignptr                SqLib_getsharedforeignptr                   = NULL;
SqLibAPI_setvmreleasehook                   SqLib_setvmreleasehook                      = NULL;
SqLibAPI_getvmreleasehook                   SqLib_getvmreleasehook                      = NULL;
SqLibAPI_setsharedreleasehook               SqLib_setsharedreleasehook                  = NULL;
SqLibAPI_getsharedreleasehook               SqLib_getsharedreleasehook                  = NULL;
SqLibAPI_setprintfunc                       SqLib_setprintfunc                          = NULL;
SqLibAPI_getprintfunc                       SqLib_getprintfunc                          = NULL;
SqLibAPI_geterrorfunc                       SqLib_geterrorfunc                          = NULL;
SqLibAPI_suspendvm                          SqLib_suspendvm                             = NULL;
SqLibAPI_wakeupvm                           SqLib_wakeupvm                              = NULL;
SqLibAPI_getvmstate                         SqLib_getvmstate                            = NULL;
SqLibAPI_getversion                         SqLib_getversion                            = NULL;

//compiler
SqLibAPI_compile                            SqLib_compile                               = NULL;
SqLibAPI_compilebuffer                      SqLib_compilebuffer                         = NULL;
SqLibAPI_enabledebuginfo                    SqLib_enabledebuginfo                       = NULL;
SqLibAPI_notifyallexceptions                SqLib_notifyallexceptions                   = NULL;
SqLibAPI_setcompilererrorhandler            SqLib_setcompilererrorhandler               = NULL;

//stack operations
SqLibAPI_push                               SqLib_push                                  = NULL;
SqLibAPI_pop                                SqLib_pop                                   = NULL;
SqLibAPI_poptop                             SqLib_poptop                                = NULL;
SqLibAPI_remove                             SqLib_remove                                = NULL;
SqLibAPI_gettop                             SqLib_gettop                                = NULL;
SqLibAPI_settop                             SqLib_settop                                = NULL;
SqLibAPI_reservestack                       SqLib_reservestack                          = NULL;
SqLibAPI_cmp                                SqLib_cmp                                   = NULL;
SqLibAPI_move                               SqLib_move                                  = NULL;

//object creation handling
SqLibAPI_newuserdata                        SqLib_newuserdata                           = NULL;
SqLibAPI_newtable                           SqLib_newtable                              = NULL;
SqLibAPI_newtableex                         SqLib_newtableex                            = NULL;
SqLibAPI_newarray                           SqLib_newarray                              = NULL;
SqLibAPI_newclosure                         SqLib_newclosure                            = NULL;
SqLibAPI_setparamscheck                     SqLib_setparamscheck                        = NULL;
SqLibAPI_bindenv                            SqLib_bindenv                               = NULL;
SqLibAPI_setclosureroot                     SqLib_setclosureroot                        = NULL;
SqLibAPI_getclosureroot                     SqLib_getclosureroot                        = NULL;
SqLibAPI_pushstring                         SqLib_pushstring                            = NULL;
SqLibAPI_pushstringf                        SqLib_pushstringf                           = NULL;
SqLibAPI_pushfloat                          SqLib_pushfloat                             = NULL;
SqLibAPI_pushinteger                        SqLib_pushinteger                           = NULL;
SqLibAPI_pushbool                           SqLib_pushbool                              = NULL;
SqLibAPI_pushuserpointer                    SqLib_pushuserpointer                       = NULL;
SqLibAPI_pushnull                           SqLib_pushnull                              = NULL;
SqLibAPI_pushthread                         SqLib_pushthread                            = NULL;
SqLibAPI_gettype                            SqLib_gettype                               = NULL;
SqLibAPI_typeof                             SqLib_typeof                                = NULL;
SqLibAPI_getsize                            SqLib_getsize                               = NULL;
SqLibAPI_gethash                            SqLib_gethash                               = NULL;
SqLibAPI_getbase                            SqLib_getbase                               = NULL;
SqLibAPI_instanceof                         SqLib_instanceof                            = NULL;
SqLibAPI_tostring                           SqLib_tostring                              = NULL;
SqLibAPI_tobool                             SqLib_tobool                                = NULL;
SqLibAPI_getstringandsize                   SqLib_getstringandsize                      = NULL;
SqLibAPI_getstring                          SqLib_getstring                             = NULL;
SqLibAPI_getinteger                         SqLib_getinteger                            = NULL;
SqLibAPI_getfloat                           SqLib_getfloat                              = NULL;
SqLibAPI_getbool                            SqLib_getbool                               = NULL;
SqLibAPI_getthread                          SqLib_getthread                             = NULL;
SqLibAPI_getuserpointer                     SqLib_getuserpointer                        = NULL;
SqLibAPI_getuserdata                        SqLib_getuserdata                           = NULL;
SqLibAPI_settypetag                         SqLib_settypetag                            = NULL;
SqLibAPI_gettypetag                         SqLib_gettypetag                            = NULL;
SqLibAPI_setreleasehook                     SqLib_setreleasehook                        = NULL;
SqLibAPI_getreleasehook                     SqLib_getreleasehook                        = NULL;
SqLibAPI_getscratchpad                      SqLib_getscratchpad                         = NULL;
SqLibAPI_getfunctioninfo                    SqLib_getfunctioninfo                       = NULL;
SqLibAPI_getclosureinfo                     SqLib_getclosureinfo                        = NULL;
SqLibAPI_getclosurename                     SqLib_getclosurename                        = NULL;
SqLibAPI_setnativeclosurename               SqLib_setnativeclosurename                  = NULL;
SqLibAPI_getnativeclosurepointer            SqLib_getnativeclosurepointer               = NULL;
SqLibAPI_setinstanceup                      SqLib_setinstanceup                         = NULL;
SqLibAPI_getinstanceup                      SqLib_getinstanceup                         = NULL;
SqLibAPI_setclassudsize                     SqLib_setclassudsize                        = NULL;
SqLibAPI_newclass                           SqLib_newclass                              = NULL;
SqLibAPI_createinstance                     SqLib_createinstance                        = NULL;
SqLibAPI_setattributes                      SqLib_setattributes                         = NULL;
SqLibAPI_getattributes                      SqLib_getattributes                         = NULL;
SqLibAPI_getclass                           SqLib_getclass                              = NULL;
SqLibAPI_weakref                            SqLib_weakref                               = NULL;
SqLibAPI_getdefaultdelegate                 SqLib_getdefaultdelegate                    = NULL;
SqLibAPI_getmemberhandle                    SqLib_getmemberhandle                       = NULL;
SqLibAPI_getbyhandle                        SqLib_getbyhandle                           = NULL;
SqLibAPI_setbyhandle                        SqLib_setbyhandle                           = NULL;

//object manipulation
SqLibAPI_pushroottable                      SqLib_pushroottable                         = NULL;
SqLibAPI_pushregistrytable                  SqLib_pushregistrytable                     = NULL;
SqLibAPI_pushconsttable                     SqLib_pushconsttable                        = NULL;
SqLibAPI_setroottable                       SqLib_setroottable                          = NULL;
SqLibAPI_setconsttable                      SqLib_setconsttable                         = NULL;
SqLibAPI_newslot                            SqLib_newslot                               = NULL;
SqLibAPI_deleteslot                         SqLib_deleteslot                            = NULL;
SqLibAPI_set                                SqLib_set                                   = NULL;
SqLibAPI_get                                SqLib_get                                   = NULL;
SqLibAPI_rawget                             SqLib_rawget                                = NULL;
SqLibAPI_rawset                             SqLib_rawset                                = NULL;
SqLibAPI_rawdeleteslot                      SqLib_rawdeleteslot                         = NULL;
SqLibAPI_newmember                          SqLib_newmember                             = NULL;
SqLibAPI_rawnewmember                       SqLib_rawnewmember                          = NULL;
SqLibAPI_arrayappend                        SqLib_arrayappend                           = NULL;
SqLibAPI_arraypop                           SqLib_arraypop                              = NULL;
SqLibAPI_arrayresize                        SqLib_arrayresize                           = NULL;
SqLibAPI_arrayreverse                       SqLib_arrayreverse                          = NULL;
SqLibAPI_arrayremove                        SqLib_arrayremove                           = NULL;
SqLibAPI_arrayinsert                        SqLib_arrayinsert                           = NULL;
SqLibAPI_setdelegate                        SqLib_setdelegate                           = NULL;
SqLibAPI_getdelegate                        SqLib_getdelegate                           = NULL;
SqLibAPI_clone                              SqLib_clone                                 = NULL;
SqLibAPI_setfreevariable                    SqLib_setfreevariable                       = NULL;
SqLibAPI_next                               SqLib_next                                  = NULL;
SqLibAPI_getweakrefval                      SqLib_getweakrefval                         = NULL;
SqLibAPI_clear                              SqLib_clear                                 = NULL;

//calls
SqLibAPI_call                               SqLib_call                                  = NULL;
SqLibAPI_resume                             SqLib_resume                                = NULL;
SqLibAPI_getlocal                           SqLib_getlocal                              = NULL;
SqLibAPI_getcallee                          SqLib_getcallee                             = NULL;
SqLibAPI_getfreevariable                    SqLib_getfreevariable                       = NULL;
SqLibAPI_getonefreevariable                 SqLib_getonefreevariable                    = NULL;
SqLibAPI_throwerror                         SqLib_throwerror                            = NULL;
SqLibAPI_throwerrorf                        SqLib_throwerrorf                           = NULL;
SqLibAPI_throwobject                        SqLib_throwobject                           = NULL;
SqLibAPI_reseterror                         SqLib_reseterror                            = NULL;
SqLibAPI_getlasterror                       SqLib_getlasterror                          = NULL;

//raw object handling
SqLibAPI_getstackobj                        SqLib_getstackobj                           = NULL;
SqLibAPI_pushobject                         SqLib_pushobject                            = NULL;
SqLibAPI_addref                             SqLib_addref                                = NULL;
SqLibAPI_release                            SqLib_release                               = NULL;
SqLibAPI_getrefcount                        SqLib_getrefcount                           = NULL;
SqLibAPI_resetobject                        SqLib_resetobject                           = NULL;
SqLibAPI_objtostring                        SqLib_objtostring                           = NULL;
SqLibAPI_objtobool                          SqLib_objtobool                             = NULL;
SqLibAPI_objtointeger                       SqLib_objtointeger                          = NULL;
SqLibAPI_objtofloat                         SqLib_objtofloat                            = NULL;
SqLibAPI_objtouserpointer                   SqLib_objtouserpointer                      = NULL;
SqLibAPI_getobjtypetag                      SqLib_getobjtypetag                         = NULL;
SqLibAPI_getvmrefcount                      SqLib_getvmrefcount                         = NULL;

//GC
SqLibAPI_collectgarbage                     SqLib_collectgarbage                        = NULL;
SqLibAPI_resurrectunreachable               SqLib_resurrectunreachable                  = NULL;

//serialization
SqLibAPI_writeclosure                       SqLib_writeclosure                          = NULL;
SqLibAPI_readclosure                        SqLib_readclosure                           = NULL;

//mem allocation
SqLibAPI_malloc                             SqLib_malloc                                = NULL;
SqLibAPI_realloc                            SqLib_realloc                               = NULL;
SqLibAPI_free                               SqLib_free                                  = NULL;

//debug
SqLibAPI_stackinfos                         SqLib_stackinfos                            = NULL;
SqLibAPI_setdebughook                       SqLib_setdebughook                          = NULL;
SqLibAPI_setnativedebughook                 SqLib_setnativedebughook                    = NULL;

//compiler helpers
SqLibAPI_loadfile                           SqLib_loadfile                              = NULL;
SqLibAPI_dofile                             SqLib_dofile                                = NULL;
SqLibAPI_writeclosuretofile                 SqLib_writeclosuretofile                    = NULL;

//blob
SqLibAPI_createblob                         SqLib_createblob                            = NULL;
SqLibAPI_getblob                            SqLib_getblob                               = NULL;
SqLibAPI_getblobsize                        SqLib_getblobsize                           = NULL;

//string
SqLibAPI_format                             SqLib_format                                = NULL;
#endif // SQMOD_PLUGIN_API

// ------------------------------------------------------------------------------------------------
uint8_t sqlib_api_expand(HSQLIBAPI sqlibapi)
{
    if (!sqlibapi)
    {
        return 0;
    }

#ifdef SQMOD_PLUGIN_API

    //vm
    SqLib_open                                  = sqlibapi->open;
    SqLib_newthread                             = sqlibapi->newthread;
    SqLib_seterrorhandler                       = sqlibapi->seterrorhandler;
    SqLib_close                                 = sqlibapi->close;
    SqLib_setforeignptr                         = sqlibapi->setforeignptr;
    SqLib_getforeignptr                         = sqlibapi->getforeignptr;
    SqLib_setsharedforeignptr                   = sqlibapi->setsharedforeignptr;
    SqLib_getsharedforeignptr                   = sqlibapi->getsharedforeignptr;
    SqLib_setvmreleasehook                      = sqlibapi->setvmreleasehook;
    SqLib_getvmreleasehook                      = sqlibapi->getvmreleasehook;
    SqLib_setsharedreleasehook                  = sqlibapi->setsharedreleasehook;
    SqLib_getsharedreleasehook                  = sqlibapi->getsharedreleasehook;
    SqLib_setprintfunc                          = sqlibapi->setprintfunc;
    SqLib_getprintfunc                          = sqlibapi->getprintfunc;
    SqLib_geterrorfunc                          = sqlibapi->geterrorfunc;
    SqLib_suspendvm                             = sqlibapi->suspendvm;
    SqLib_wakeupvm                              = sqlibapi->wakeupvm;
    SqLib_getvmstate                            = sqlibapi->getvmstate;
    SqLib_getversion                            = sqlibapi->getversion;

    //compiler
    SqLib_compile                               = sqlibapi->compile;
    SqLib_compilebuffer                         = sqlibapi->compilebuffer;
    SqLib_enabledebuginfo                       = sqlibapi->enabledebuginfo;
    SqLib_notifyallexceptions                   = sqlibapi->notifyallexceptions;
    SqLib_setcompilererrorhandler               = sqlibapi->setcompilererrorhandler;

    //stack operations
    SqLib_push                                  = sqlibapi->push;
    SqLib_pop                                   = sqlibapi->pop;
    SqLib_poptop                                = sqlibapi->poptop;
    SqLib_remove                                = sqlibapi->remove;
    SqLib_gettop                                = sqlibapi->gettop;
    SqLib_settop                                = sqlibapi->settop;
    SqLib_reservestack                          = sqlibapi->reservestack;
    SqLib_cmp                                   = sqlibapi->cmp;
    SqLib_move                                  = sqlibapi->move;

    //object creation handling
    SqLib_newuserdata                           = sqlibapi->newuserdata;
    SqLib_newtable                              = sqlibapi->newtable;
    SqLib_newtableex                            = sqlibapi->newtableex;
    SqLib_newarray                              = sqlibapi->newarray;
    SqLib_newclosure                            = sqlibapi->newclosure;
    SqLib_setparamscheck                        = sqlibapi->setparamscheck;
    SqLib_bindenv                               = sqlibapi->bindenv;
    SqLib_setclosureroot                        = sqlibapi->setclosureroot;
    SqLib_getclosureroot                        = sqlibapi->getclosureroot;
    SqLib_pushstring                            = sqlibapi->pushstring;
    SqLib_pushstringf                           = sqlibapi->pushstringf;
    SqLib_pushfloat                             = sqlibapi->pushfloat;
    SqLib_pushinteger                           = sqlibapi->pushinteger;
    SqLib_pushbool                              = sqlibapi->pushbool;
    SqLib_pushuserpointer                       = sqlibapi->pushuserpointer;
    SqLib_pushnull                              = sqlibapi->pushnull;
    SqLib_pushthread                            = sqlibapi->pushthread;
    SqLib_gettype                               = sqlibapi->gettype;
    SqLib_typeof                                = sqlibapi->typeof_;
    SqLib_getsize                               = sqlibapi->getsize;
    SqLib_gethash                               = sqlibapi->gethash;
    SqLib_getbase                               = sqlibapi->getbase;
    SqLib_instanceof                            = sqlibapi->instanceof;
    SqLib_tostring                              = sqlibapi->tostring;
    SqLib_tobool                                = sqlibapi->tobool;
    SqLib_getstringandsize                      = sqlibapi->getstringandsize;
    SqLib_getstring                             = sqlibapi->getstring;
    SqLib_getinteger                            = sqlibapi->getinteger;
    SqLib_getfloat                              = sqlibapi->getfloat;
    SqLib_getbool                               = sqlibapi->getbool;
    SqLib_getthread                             = sqlibapi->getthread;
    SqLib_getuserpointer                        = sqlibapi->getuserpointer;
    SqLib_getuserdata                           = sqlibapi->getuserdata;
    SqLib_settypetag                            = sqlibapi->settypetag;
    SqLib_gettypetag                            = sqlibapi->gettypetag;
    SqLib_setreleasehook                        = sqlibapi->setreleasehook;
    SqLib_getreleasehook                        = sqlibapi->getreleasehook;
    SqLib_getscratchpad                         = sqlibapi->getscratchpad;
    SqLib_getfunctioninfo                       = sqlibapi->getfunctioninfo;
    SqLib_getclosureinfo                        = sqlibapi->getclosureinfo;
    SqLib_getclosurename                        = sqlibapi->getclosurename;
    SqLib_setnativeclosurename                  = sqlibapi->setnativeclosurename;
    SqLib_getnativeclosurepointer               = sqlibapi->getnativeclosurepointer;
    SqLib_setinstanceup                         = sqlibapi->setinstanceup;
    SqLib_getinstanceup                         = sqlibapi->getinstanceup;
    SqLib_setclassudsize                        = sqlibapi->setclassudsize;
    SqLib_newclass                              = sqlibapi->newclass;
    SqLib_createinstance                        = sqlibapi->createinstance;
    SqLib_setattributes                         = sqlibapi->setattributes;
    SqLib_getattributes                         = sqlibapi->getattributes;
    SqLib_getclass                              = sqlibapi->getclass;
    SqLib_weakref                               = sqlibapi->weakref;
    SqLib_getdefaultdelegate                    = sqlibapi->getdefaultdelegate;
    SqLib_getmemberhandle                       = sqlibapi->getmemberhandle;
    SqLib_getbyhandle                           = sqlibapi->getbyhandle;
    SqLib_setbyhandle                           = sqlibapi->setbyhandle;

    //object manipulation
    SqLib_pushroottable                         = sqlibapi->pushroottable;
    SqLib_pushregistrytable                     = sqlibapi->pushregistrytable;
    SqLib_pushconsttable                        = sqlibapi->pushconsttable;
    SqLib_setroottable                          = sqlibapi->setroottable;
    SqLib_setconsttable                         = sqlibapi->setconsttable;
    SqLib_newslot                               = sqlibapi->newslot;
    SqLib_deleteslot                            = sqlibapi->deleteslot;
    SqLib_set                                   = sqlibapi->set;
    SqLib_get                                   = sqlibapi->get;
    SqLib_rawget                                = sqlibapi->rawget;
    SqLib_rawset                                = sqlibapi->rawset;
    SqLib_rawdeleteslot                         = sqlibapi->rawdeleteslot;
    SqLib_newmember                             = sqlibapi->newmember;
    SqLib_rawnewmember                          = sqlibapi->rawnewmember;
    SqLib_arrayappend                           = sqlibapi->arrayappend;
    SqLib_arraypop                              = sqlibapi->arraypop;
    SqLib_arrayresize                           = sqlibapi->arrayresize;
    SqLib_arrayreverse                          = sqlibapi->arrayreverse;
    SqLib_arrayremove                           = sqlibapi->arrayremove;
    SqLib_arrayinsert                           = sqlibapi->arrayinsert;
    SqLib_setdelegate                           = sqlibapi->setdelegate;
    SqLib_getdelegate                           = sqlibapi->getdelegate;
    SqLib_clone                                 = sqlibapi->clone;
    SqLib_setfreevariable                       = sqlibapi->setfreevariable;
    SqLib_next                                  = sqlibapi->next;
    SqLib_getweakrefval                         = sqlibapi->getweakrefval;
    SqLib_clear                                 = sqlibapi->clear;

    //calls
    SqLib_call                                  = sqlibapi->call;
    SqLib_resume                                = sqlibapi->resume;
    SqLib_getlocal                              = sqlibapi->getlocal;
    SqLib_getcallee                             = sqlibapi->getcallee;
    SqLib_getfreevariable                       = sqlibapi->getfreevariable;
    SqLib_getonefreevariable                    = sqlibapi->getonefreevariable;
    SqLib_throwerror                            = sqlibapi->throwerror;
    SqLib_throwerrorf                           = sqlibapi->throwerrorf;
    SqLib_throwobject                           = sqlibapi->throwobject;
    SqLib_reseterror                            = sqlibapi->reseterror;
    SqLib_getlasterror                          = sqlibapi->getlasterror;

    //raw object handling
    SqLib_getstackobj                           = sqlibapi->getstackobj;
    SqLib_pushobject                            = sqlibapi->pushobject;
    SqLib_addref                                = sqlibapi->addref;
    SqLib_release                               = sqlibapi->release;
    SqLib_getrefcount                           = sqlibapi->getrefcount;
    SqLib_resetobject                           = sqlibapi->resetobject;
    SqLib_objtostring                           = sqlibapi->objtostring;
    SqLib_objtobool                             = sqlibapi->objtobool;
    SqLib_objtointeger                          = sqlibapi->objtointeger;
    SqLib_objtofloat                            = sqlibapi->objtofloat;
    SqLib_objtouserpointer                      = sqlibapi->objtouserpointer;
    SqLib_getobjtypetag                         = sqlibapi->getobjtypetag;
    SqLib_getvmrefcount                         = sqlibapi->getvmrefcount;

    //GC
    SqLib_collectgarbage                        = sqlibapi->collectgarbage;
    SqLib_resurrectunreachable                  = sqlibapi->resurrectunreachable;

    //serialization
    SqLib_writeclosure                          = sqlibapi->writeclosure;
    SqLib_readclosure                           = sqlibapi->readclosure;

    //mem allocation
    SqLib_malloc                                = sqlibapi->malloc;
    SqLib_realloc                               = sqlibapi->realloc;
    SqLib_free                                  = sqlibapi->free;

    //debug
    SqLib_stackinfos                            = sqlibapi->stackinfos;
    SqLib_setdebughook                          = sqlibapi->setdebughook;
    SqLib_setnativedebughook                    = sqlibapi->setnativedebughook;

    //compiler helpers
    SqLib_loadfile                              = sqlibapi->loadfile;
    SqLib_dofile                                = sqlibapi->dofile;
    SqLib_writeclosuretofile                    = sqlibapi->writeclosuretofile;

    //blob
    SqLib_createblob                            = sqlibapi->createblob;
    SqLib_getblob                               = sqlibapi->getblob;
    SqLib_getblobsize                           = sqlibapi->getblobsize;

    //string
    SqLib_format                                = sqlibapi->format;

#endif // SQMOD_PLUGIN_API

    return 1;
}

// ------------------------------------------------------------------------------------------------
void sqlib_api_collapse()
{
#ifdef SQMOD_PLUGIN_API

    //vm
    SqLib_open                                  = NULL;
    SqLib_newthread                             = NULL;
    SqLib_seterrorhandler                       = NULL;
    SqLib_close                                 = NULL;
    SqLib_setforeignptr                         = NULL;
    SqLib_getforeignptr                         = NULL;
    SqLib_setsharedforeignptr                   = NULL;
    SqLib_getsharedforeignptr                   = NULL;
    SqLib_setvmreleasehook                      = NULL;
    SqLib_getvmreleasehook                      = NULL;
    SqLib_setsharedreleasehook                  = NULL;
    SqLib_getsharedreleasehook                  = NULL;
    SqLib_setprintfunc                          = NULL;
    SqLib_getprintfunc                          = NULL;
    SqLib_geterrorfunc                          = NULL;
    SqLib_suspendvm                             = NULL;
    SqLib_wakeupvm                              = NULL;
    SqLib_getvmstate                            = NULL;
    SqLib_getversion                            = NULL;

    //compiler
    SqLib_compile                               = NULL;
    SqLib_compilebuffer                         = NULL;
    SqLib_enabledebuginfo                       = NULL;
    SqLib_notifyallexceptions                   = NULL;
    SqLib_setcompilererrorhandler               = NULL;

    //stack operations
    SqLib_push                                  = NULL;
    SqLib_pop                                   = NULL;
    SqLib_poptop                                = NULL;
    SqLib_remove                                = NULL;
    SqLib_gettop                                = NULL;
    SqLib_settop                                = NULL;
    SqLib_reservestack                          = NULL;
    SqLib_cmp                                   = NULL;
    SqLib_move                                  = NULL;

    //object creation handling
    SqLib_newuserdata                           = NULL;
    SqLib_newtable                              = NULL;
    SqLib_newtableex                            = NULL;
    SqLib_newarray                              = NULL;
    SqLib_newclosure                            = NULL;
    SqLib_setparamscheck                        = NULL;
    SqLib_bindenv                               = NULL;
    SqLib_setclosureroot                        = NULL;
    SqLib_getclosureroot                        = NULL;
    SqLib_pushstring                            = NULL;
    SqLib_pushstringf                           = NULL;
    SqLib_pushfloat                             = NULL;
    SqLib_pushinteger                           = NULL;
    SqLib_pushbool                              = NULL;
    SqLib_pushuserpointer                       = NULL;
    SqLib_pushnull                              = NULL;
    SqLib_pushthread                            = NULL;
    SqLib_gettype                               = NULL;
    SqLib_typeof                                = NULL;
    SqLib_getsize                               = NULL;
    SqLib_gethash                               = NULL;
    SqLib_getbase                               = NULL;
    SqLib_instanceof                            = NULL;
    SqLib_tostring                              = NULL;
    SqLib_tobool                                = NULL;
    SqLib_getstringandsize                      = NULL;
    SqLib_getstring                             = NULL;
    SqLib_getinteger                            = NULL;
    SqLib_getfloat                              = NULL;
    SqLib_getbool                               = NULL;
    SqLib_getthread                             = NULL;
    SqLib_getuserpointer                        = NULL;
    SqLib_getuserdata                           = NULL;
    SqLib_settypetag                            = NULL;
    SqLib_gettypetag                            = NULL;
    SqLib_setreleasehook                        = NULL;
    SqLib_getreleasehook                        = NULL;
    SqLib_getscratchpad                         = NULL;
    SqLib_getfunctioninfo                       = NULL;
    SqLib_getclosureinfo                        = NULL;
    SqLib_getclosurename                        = NULL;
    SqLib_setnativeclosurename                  = NULL;
    SqLib_getnativeclosurepointer               = NULL;
    SqLib_setinstanceup                         = NULL;
    SqLib_getinstanceup                         = NULL;
    SqLib_setclassudsize                        = NULL;
    SqLib_newclass                              = NULL;
    SqLib_createinstance                        = NULL;
    SqLib_setattributes                         = NULL;
    SqLib_getattributes                         = NULL;
    SqLib_getclass                              = NULL;
    SqLib_weakref                               = NULL;
    SqLib_getdefaultdelegate                    = NULL;
    SqLib_getmemberhandle                       = NULL;
    SqLib_getbyhandle                           = NULL;
    SqLib_setbyhandle                           = NULL;

    //object manipulation
    SqLib_pushroottable                         = NULL;
    SqLib_pushregistrytable                     = NULL;
    SqLib_pushconsttable                        = NULL;
    SqLib_setroottable                          = NULL;
    SqLib_setconsttable                         = NULL;
    SqLib_newslot                               = NULL;
    SqLib_deleteslot                            = NULL;
    SqLib_set                                   = NULL;
    SqLib_get                                   = NULL;
    SqLib_rawget                                = NULL;
    SqLib_rawset                                = NULL;
    SqLib_rawdeleteslot                         = NULL;
    SqLib_newmember                             = NULL;
    SqLib_rawnewmember                          = NULL;
    SqLib_arrayappend                           = NULL;
    SqLib_arraypop                              = NULL;
    SqLib_arrayresize                           = NULL;
    SqLib_arrayreverse                          = NULL;
    SqLib_arrayremove                           = NULL;
    SqLib_arrayinsert                           = NULL;
    SqLib_setdelegate                           = NULL;
    SqLib_getdelegate                           = NULL;
    SqLib_clone                                 = NULL;
    SqLib_setfreevariable                       = NULL;
    SqLib_next                                  = NULL;
    SqLib_getweakrefval                         = NULL;
    SqLib_clear                                 = NULL;

    //calls
    SqLib_call                                  = NULL;
    SqLib_resume                                = NULL;
    SqLib_getlocal                              = NULL;
    SqLib_getcallee                             = NULL;
    SqLib_getfreevariable                       = NULL;
    SqLib_getonefreevariable                    = NULL;
    SqLib_throwerror                            = NULL;
    SqLib_throwerrorf                           = NULL;
    SqLib_throwobject                           = NULL;
    SqLib_reseterror                            = NULL;
    SqLib_getlasterror                          = NULL;

    //raw object handling
    SqLib_getstackobj                           = NULL;
    SqLib_pushobject                            = NULL;
    SqLib_addref                                = NULL;
    SqLib_release                               = NULL;
    SqLib_getrefcount                           = NULL;
    SqLib_resetobject                           = NULL;
    SqLib_objtostring                           = NULL;
    SqLib_objtobool                             = NULL;
    SqLib_objtointeger                          = NULL;
    SqLib_objtofloat                            = NULL;
    SqLib_objtouserpointer                      = NULL;
    SqLib_getobjtypetag                         = NULL;
    SqLib_getvmrefcount                         = NULL;

    //GC
    SqLib_collectgarbage                        = NULL;
    SqLib_resurrectunreachable                  = NULL;

    //serialization
    SqLib_writeclosure                          = NULL;
    SqLib_readclosure                           = NULL;

    //mem allocation
    SqLib_malloc                                = NULL;
    SqLib_realloc                               = NULL;
    SqLib_free                                  = NULL;

    //debug
    SqLib_stackinfos                            = NULL;
    SqLib_setdebughook                          = NULL;
    SqLib_setnativedebughook                    = NULL;

    //compiler helpers
    SqLib_loadfile                              = NULL;
    SqLib_dofile                                = NULL;
    SqLib_writeclosuretofile                    = NULL;

    //blob
    SqLib_createblob                            = NULL;
    SqLib_getblob                               = NULL;
    SqLib_getblobsize                           = NULL;

    //string
    SqLib_format                                = NULL;

#endif // SQMOD_PLUGIN_API
}
