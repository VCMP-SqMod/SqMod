// ------------------------------------------------------------------------------------------------
#include "Handle/Manager.hpp"

// ------------------------------------------------------------------------------------------------
#include <ctime>
#include <algorithm>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
ManagerHnd::Managers ManagerHnd::s_Managers;

// ------------------------------------------------------------------------------------------------
void ManagerHnd::Validate() const
{
    // Is the handle valid?
    if ((m_Hnd == nullptr))
    {
        STHROWF("Invalid Mongoose manager reference");
    }
}

// ------------------------------------------------------------------------------------------------
void ManagerHnd::Validate(CSStr act) const
{
    // Is the handle valid?
    if ((m_Hnd == nullptr))
    {
        STHROWF("Invalid Mongoose manager reference while: %s", act);
    }
}

// ------------------------------------------------------------------------------------------------
ManagerHnd::Handle::Handle()
    : mMgr()
    , mRef(1)
{
    // Initialize event manager object
    mg_mgr_init(&mMgr, this);
    // Add this instance to the pool
    s_Managers.push_back(this);
}

// ------------------------------------------------------------------------------------------------
ManagerHnd::Handle::~Handle()
{
    // Attempt to release manager resources
    mg_mgr_free(&mMgr);
    // Remove this from the pool
    s_Managers.erase(std::remove(s_Managers.begin(), s_Managers.end(), this), s_Managers.end());
}

// ------------------------------------------------------------------------------------------------
Int64 ManagerHnd::Handle::Pool(Int32 milli)
{
    // Pool the manager for events and store the returned timestamp
    std::time_t timestamp = mg_mgr_poll(&mMgr, milli);
    // Create a tm structure pointing to the unix epoch time
    std::tm epoch_strt;
    epoch_strt.tm_sec = 0;
    epoch_strt.tm_min = 0;
    epoch_strt.tm_hour = 0;
    epoch_strt.tm_mday = 1;
    epoch_strt.tm_mon = 0;
    epoch_strt.tm_year = 70;
    epoch_strt.tm_isdst = -1;
    // Convert it to a time structure
    std::time_t basetime = std::mktime(&epoch_strt);
    // Return obtained timestamp
    return  static_cast< Int64 >(std::difftime(timestamp, basetime));
}

} // Namespace:: SqMod