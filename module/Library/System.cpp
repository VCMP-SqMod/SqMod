// ------------------------------------------------------------------------------------------------
#include "Library/System.hpp"
#include "Core/Buffer.hpp"

// ------------------------------------------------------------------------------------------------
#include <cstdio>
#include <string>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
extern void Register_SysDir(HSQUIRRELVM vm);
extern void Register_SysEnv(HSQUIRRELVM vm);
extern void Register_SysPath(HSQUIRRELVM vm);
// ------------------------------------------------------------------------------------------------
static SQInteger SqSysExec(HSQUIRRELVM vm)
{
    // Attempt to retrieve the value from the stack as a string
    StackStrF val(vm, 2);
    // Have we failed to retrieve the string?
    if (SQ_FAILED(val.Proc(true)))
    {
        return val.mRes; // Propagate the error!
    }
    // Allocate a temp buffer to retrieve chunks from output
    char buffer[128];
    // Allocate a buffer which will contain the final output
    Buffer b(128);
    // Attempt to open the specified process
    FILE * pipe = popen(val.mPtr, "r");
    // The process return status
    int32_t status;
    // Did we fail to open the process?
    if (!pipe)
    {
        return sq_throwerrorf(vm, "Unable to open process [%s]", val.mPtr);
    }
    // Attempt to read process output
    try
    {
        while (!std::feof(pipe))
        {
            if (std::fgets(buffer, 128, pipe) != nullptr)
            {
                b.AppendS(buffer);
            }
        }
    }
    catch (...)
    {
        // Close the process
        status = pclose(pipe);
        // Now throw the error
        return sq_throwerrorf(vm, "Unable read process output [%d]", status);
    }
    // Close the process and obtain the exit status
    status = pclose(pipe);
    // Remember the top of the stack
    const int32_t top = sq_gettop(vm);
    // Create a new table on the stack
    sq_newtable(vm);
    // Push the element name
    sq_pushstring(vm, _SC("Status"), -1);
    // Push the element value
    sq_pushinteger(vm, status);
    // Create the element in the table
    SQRESULT res = sq_rawset(vm, -3);
    // Check the result
    if (SQ_FAILED(res))
    {
        // Clean the stack
        sq_settop(vm, top);
        // Return the error
        return res;
    }
    // Push the element name
    sq_pushstring(vm, _SC("Output"), -1);
    // Push the element value
    sq_pushstring(vm, b.Get< SQChar >(), b.Position());
    // Create the element in the table
    res = sq_rawset(vm, -3);
    // Check the result
    if (SQ_FAILED(res))
    {
        // Clean the stack
        sq_settop(vm, top);
        // Return the error
        return res;
    }
    // Specify that we want to return the table we created
    return 1;
}

// ================================================================================================
void Register_System(HSQUIRRELVM vm)
{
    Register_SysDir(vm);
    Register_SysEnv(vm);
    Register_SysPath(vm);

    RootTable(vm).SquirrelFunc(_SC("SysExec"), &SqSysExec);
}

} // Namespace:: SqMod
