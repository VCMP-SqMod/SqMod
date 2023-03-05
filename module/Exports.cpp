// ------------------------------------------------------------------------------------------------
#include "Core.hpp"

// ------------------------------------------------------------------------------------------------
#include <cstring>

// ------------------------------------------------------------------------------------------------
#include <sqmod.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
static int32_t RegisterCommandFn(ExtPluginCommand_t fn)
{
    return Core::Get().RegisterExtCommand(fn);
}

// ------------------------------------------------------------------------------------------------
static int32_t UnregisterCommandFn(ExtPluginCommand_t fn)
{
    return Core::Get().UnregisterExtCommand(fn);
}

// ------------------------------------------------------------------------------------------------
static int32_t SendCommandFn(int32_t target, int32_t req, int32_t tag, const uint8_t * data, size_t size)
{
    return Core::Get().SendExtCommand(target, req, tag, data, size);
}

// ------------------------------------------------------------------------------------------------
static int32_t SendCommandReplyFn(int32_t sender, int32_t tag, const uint8_t * data, size_t size)
{
    // Mark the initialization as successful by default
    const CoreState cs(SQMOD_SUCCESS);
    // Forward the call to the script callbacks
    Core::Get().EmitExtCommandReply(sender, tag, data, size);
    // Return the last known plug-in state
    return Core::Get().GetState();
}

// ------------------------------------------------------------------------------------------------
static int32_t SendCommandEventFn(int32_t sender, int32_t tag, const uint8_t * data, size_t size)
{
    // Mark the initialization as successful by default
    const CoreState cs(SQMOD_SUCCESS);
    // Forward the call to the script callbacks
    Core::Get().EmitExtCommandEvent(sender, tag, data, size);
    // Return the last known plug-in state
    return Core::Get().GetState();
}

// ------------------------------------------------------------------------------------------------
static const SQ_MOD_EXPORTS g_SqModExports{
    sizeof(SQ_MOD_EXPORTS),
    &RegisterCommandFn,
    &UnregisterCommandFn,
    &SendCommandFn,
    &SendCommandReplyFn,
    &SendCommandEventFn
};

// The server needs a pointer to a pointer, and a persistent one
static const SQ_MOD_EXPORTS * g_SqModExportsPtr = &g_SqModExports;

// ------------------------------------------------------------------------------------------------
void InitExports()
{
    // Tell the server about the pointer to the exports structure
    _Func->ExportFunctions(_Info->pluginId, reinterpret_cast< const void ** >(&g_SqModExportsPtr), sizeof(HSQ_MOD_EXPORTS));
}

} // Namespace:: SqMod
