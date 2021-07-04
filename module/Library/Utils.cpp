// ------------------------------------------------------------------------------------------------
#include "Library/Utils.hpp"
#include "Core/Utility.hpp"

// ------------------------------------------------------------------------------------------------
#include <cstdio>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Probably not the best implementation but should cover all sorts of weird cases.
*/
static SQInteger SqExtractIPv4(HSQUIRRELVM vm)
{
    // Was the IP address specified?
    if (sq_gettop(vm) <= 1)
    {
        return sq_throwerror(vm, "Missing IP address string");
    }
    // Attempt to generate the string value
    StackStrF val(vm, 2);
    // Have we failed to retrieve the string?
    if (SQ_FAILED(val.Proc(true)))
    {
        return val.mRes; // Propagate the error!
    }
    // Cleansed IP address buffer
    SQChar address[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    // Counting variables used by loops
    uint32_t i = 0, j = 0, k = 0;
    // Replicate the necessary characters from the resulted string
    for (; (i < static_cast< uint32_t >(val.mLen)) && (j < 16) && (k < 4); ++i)
    {
        // Is this a digit?
        if (std::isdigit(val.mPtr[i]) != 0)
        {
            address[j++] = val.mPtr[i]; // Just replicate it
        }
        // Is this a delimiter?
        else if (val.mPtr[i] == '.')
        {
            // Add a dummy component if one was not specified
            if (j == 0 || address[j-1] == '.')
            {
                address[j++] = '0';
            }
            // Add the actual delimiter
            address[j++] = '.';
            // Increase the block counter
            ++k;
        }
    }
    // Complete the address if it wasn't (also count the last block)
    while (++k < 4)
    {
        address[j++] = '0';
        address[j++] = '.';
    }
    // Components of the IP address
    uint32_t blocks[4] = {0, 0, 0, 0};
    // Attempt to extract the components of the IP address
    std::sscanf(address, "%u.%u.%u.%u", &blocks[0], &blocks[1], &blocks[2], &blocks[3]); // NOLINT(cert-err34-c)
    // Create a new array on the stack to hold the extracted components
    sq_newarray(vm, 4);
    // Push the elements into the array
    for (i = 0; i < 4; ++i)
    {
        // Push the element index
        sq_pushinteger(vm, i);
        // Push the element value
        sq_pushinteger(vm, Clamp(blocks[i], 0U, 255U));
        // Assign the element
        const SQRESULT res = sq_set(vm, -3);
        // See if the assignment failed
        if (SQ_FAILED(res))
        {
            return res;
        }
    }
    // Specify that we have a result on the stack
    return 1;
}

// ------------------------------------------------------------------------------------------------
extern void Register_Vector(HSQUIRRELVM vm, Table & ns);
extern void Register_Native_String(HSQUIRRELVM vm, Table & ns);
extern void Register_ServerAnnouncer(HSQUIRRELVM vm, Table & ns);

// ================================================================================================
void Register_Utils(HSQUIRRELVM vm)
{
    Table ns(vm);

    Register_Vector(vm, ns);
    Register_Native_String(vm, ns);
    Register_ServerAnnouncer(vm, ns);

    ns.SquirrelFunc(_SC("ExtractIPv4"), &SqExtractIPv4);

    RootTable(vm).Bind(_SC("SqUtils"), ns);
}

} // Namespace:: SqMod
