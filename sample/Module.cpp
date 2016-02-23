// --------------------------------------------------------------------------------------------
#include "Module.hpp"
#include "sq_mod.h"

// --------------------------------------------------------------------------------------------
#include <sqrat.h>

// --------------------------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

// --------------------------------------------------------------------------------------------
#if defined(WIN32) || defined(_WIN32)
    #include <Windows.h>
#endif

// --------------------------------------------------------------------------------------------
PluginFuncs*        _Func = NULL;
PluginCallbacks*    _Clbk = NULL;
PluginInfo*         _Info = NULL;

// --------------------------------------------------------------------------------------------
HSQAPI              _SqAPI = NULL;
HSQUIRRELVM         _SqVm = NULL;

// ------------------------------------------------------------------------------------------------
void BindCallbacks();
void UnbindCallbacks();

// --------------------------------------------------------------------------------------------
void RegisterAPI(HSQUIRRELVM vm);

// --------------------------------------------------------------------------------------------
void OnSquirrelInitialize()
{
    HSQEXPORTS exp = sq_api_import(_Func);
    // Did we failed to obtain the plugin exports?
    if(!exp)
        OutputMessage("Failed to attach to %s", SQMOD_HOST_NAME);
    else
    {
        // Obtain the Squirrel API and VM
        _SqAPI = exp->GetSquirrelAPI();
        _SqVm = exp->GetSquirrelVM();
        // Expand the API into global functions
        sq_api_expand(_SqAPI);
        // Register functions
        RegisterAPI(_SqVm);
    }
}

// --------------------------------------------------------------------------------------------
void OnSquirrelTerminate()
{
    OutputMessage("Terminating sample plugin...");
    // The server may still send callbacks
    UnbindCallbacks();
    // Release squirrel resources...
}

// --------------------------------------------------------------------------------------------
int OnInternalCommand(unsigned int type, const char * text)
{
    switch(type)
    {
        case SQMOD_INITIALIZE_CMD:
            OnSquirrelInitialize();
        break;
        case SQMOD_TERMINATE_CMD:
            OnSquirrelTerminate();
        break;
        default: break;
    }
    return 1;
}

// --------------------------------------------------------------------------------------------
int OnInitServer()
{
    puts("");
    OutputMessage("Loaded Sample Squirrel Plugin");
    return 1;
}

// --------------------------------------------------------------------------------------------
SQMOD_API_EXPORT unsigned int VcmpPluginInit(PluginFuncs* functions, PluginCallbacks* callbacks, PluginInfo* info)
{
    // Store server proxies
    _Func = functions;
    _Clbk = callbacks;
    _Info = info;
    // Assign plugin information
    _Info->uPluginVer = 0x1000; // 1.0.00
    strcpy(_Info->szName, "SampleSquirrelPlugin");
    // Expand the API into global functions
    vc_api_expand(_Func);
    // Bind callbacks
    BindCallbacks();
    // Done!
    return 1;
}

// ------------------------------------------------------------------------------------------------
void BindCallbacks()
{
    _Clbk->OnInitServer             = OnInitServer;
    _Clbk->OnInternalCommand        = OnInternalCommand;
}

// ------------------------------------------------------------------------------------------------
void UnbindCallbacks()
{
    _Clbk->OnInitServer             = NULL;
    _Clbk->OnInternalCommand        = NULL;
}

// --------------------------------------------------------------------------------------------
void OutputMessageImpl(const char * msg, va_list args)
{
#if defined(WIN32) || defined(_WIN32)
    HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_SCREEN_BUFFER_INFO csb_before;
    GetConsoleScreenBufferInfo( hstdout, &csb_before);
    SetConsoleTextAttribute(hstdout, FOREGROUND_GREEN);
    printf("[MODULE] ");

    SetConsoleTextAttribute(hstdout, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
    vprintf(msg, args);
    puts("");

    SetConsoleTextAttribute(hstdout, csb_before.wAttributes);
#else
    printf("%c[0;32m[MODULE]%c[0;37m", 27, 27, msg);
    vprintf(msg, args);
    puts("");
#endif
}

// --------------------------------------------------------------------------------------------
void OutputDebug(const char * msg, ...)
{
#ifdef _DEBUG
    // Initialize the arguments list
    va_list args;
    va_start(args, msg);
    // Call the output function
    OutputMessageImpl(msg, args);
    // Finalize the arguments list
    va_end(args);
#endif
}

// --------------------------------------------------------------------------------------------
void OutputMessage(const char * msg, ...)
{
    // Initialize the arguments list
    va_list args;
    va_start(args, msg);
    // Call the output function
    OutputMessageImpl(msg, args);
    // Finalize the arguments list
    va_end(args);
}

/* --------------------------------------------------------------------------------------------
 * Sample Plugin API
*/

int SampleFunction()
{
    OutputMessage("Hello from the sample plugin function!");
    return rand();
}

class SampleType
{
private:

    int m_MyVal;

public:

    int mMyNum;

    SampleType()
        : m_MyVal(0), mMyNum(0)
    {

    }

    SampleType(int num)
        : m_MyVal(num * 2), mMyNum(num)
    {

    }

    int GetMyVal() const
    {
        return m_MyVal;
    }

    void SetMyVal(int val)
    {
        m_MyVal = val;
    }

    void SampleMethod() const
    {
        OutputMessage("I have the values %d and %d", m_MyVal, mMyNum);
    }
};

// --------------------------------------------------------------------------------------------
void RegisterAPI(HSQUIRRELVM vm)
{
    using namespace Sqrat;

    RootTable(vm).Bind(_SC("SampleType"), Class< SampleType >(vm, _SC("SampleType"))
        .Ctor()
        .Ctor< int >()
        .Var(_SC("MyNum"), &SampleType::mMyNum)
        .Prop(_SC("MyVal"), &SampleType::GetMyVal, &SampleType::SetMyVal)
        .Func(_SC("SampleMethod"), &SampleType::SampleMethod)
    );

    RootTable(vm).Func(_SC("SampleFunction"), &SampleFunction);
}
