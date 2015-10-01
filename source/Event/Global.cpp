#include "Event/Global.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
GlobalEvent::GlobalEvent() noexcept
{

}

// ------------------------------------------------------------------------------------------------
GlobalEvent::GlobalEvent(SQInt32 type) noexcept
{

}

// ------------------------------------------------------------------------------------------------
GlobalEvent::GlobalEvent(SQInt32 type, bool suspended) noexcept
{

}

// ------------------------------------------------------------------------------------------------
GlobalEvent::GlobalEvent(const GlobalEvent & o) noexcept
{

}

// ------------------------------------------------------------------------------------------------
GlobalEvent::GlobalEvent(GlobalEvent && o) noexcept
{

}

// ------------------------------------------------------------------------------------------------
GlobalEvent::~GlobalEvent()
{

}

// ------------------------------------------------------------------------------------------------
GlobalEvent & GlobalEvent::operator = (const GlobalEvent & o) noexcept
{
    return *this;
}

// ------------------------------------------------------------------------------------------------
GlobalEvent & GlobalEvent::operator = (GlobalEvent && o) noexcept
{
    return *this;
}

// ================================================================================================
bool Register_GlobalEvent(HSQUIRRELVM vm)
{
    // Output debugging information
    LogDbg("Beginning registration of <GlobalEvent> type");
    // Attempt to register the specified type
    Sqrat::RootTable(vm).Bind(_SC("GlobalEvent"), Sqrat::Class< GlobalEvent >(vm, _SC("GlobalEvent"))
        .Ctor()
    );
    // Output debugging information
    LogDbg("Registration of <CAutomobile> type was successful");
    // Registration succeeded
    return true;
}


} // Namespace:: SqMod
