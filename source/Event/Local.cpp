#include "Event/Local.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
LocalEvent::LocalEvent() noexcept
{

}

// ------------------------------------------------------------------------------------------------
LocalEvent::LocalEvent(SQInt32 type) noexcept
{

}

// ------------------------------------------------------------------------------------------------
LocalEvent::LocalEvent(SQInt32 type, bool suspended) noexcept
{

}

// ------------------------------------------------------------------------------------------------
LocalEvent::LocalEvent(const LocalEvent & o) noexcept
{

}

// ------------------------------------------------------------------------------------------------
LocalEvent::LocalEvent(LocalEvent && o) noexcept
{

}

// ------------------------------------------------------------------------------------------------
LocalEvent::~LocalEvent()
{

}

// ------------------------------------------------------------------------------------------------
LocalEvent & LocalEvent::operator = (const LocalEvent & o) noexcept
{
    return *this;
}

// ------------------------------------------------------------------------------------------------
LocalEvent & LocalEvent::operator = (LocalEvent && o) noexcept
{
    return *this;
}

// ------------------------------------------------------------------------------------------------


// ================================================================================================
bool Register_LocalEvent(HSQUIRRELVM vm)
{
    // Output debugging information
    LogDbg("Beginning registration of <LocalEvent> type");
    // Attempt to register the specified type
    Sqrat::RootTable(vm).Bind(_SC("LocalEvent"), Sqrat::Class< LocalEvent >(vm, _SC("LocalEvent"))
        .Ctor()
    );
    // Output debugging information
    LogDbg("Registration of <CAutomobile> type was successful");
    // Registration succeeded
    return true;
}


} // Namespace:: SqMod
