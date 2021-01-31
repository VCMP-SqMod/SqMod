// ------------------------------------------------------------------------------------------------
#include "PocoLib/Crypto.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------
static SQInteger SqGetHash(HSQUIRRELVM vm)
{
    // Attempt to retrieve the digest from the stack as a string
    StackStrF dig(vm, 2);
    // Have we failed to retrieve the string?
    if (SQ_FAILED(dig.Proc(false)))
    {
        return dig.mRes; // Propagate the error!
    }
    // Attempt to retrieve the value from the stack as a string
    StackStrF val(vm, 3);
    // Have we failed to retrieve the string?
    if (SQ_FAILED(val.Proc(true)))
    {
        return val.mRes; // Propagate the error!
    }
    // Prevent any exceptions from reach the VM
    try
    {
        // Create the digest engine instance
        Poco::Crypto::DigestEngine dg(dig.ToStr());
        // Attempt to digest the given value
        dg.update(val.mPtr, static_cast< size_t >(val.mLen));
        // Retrieve the data as hex string
        std::string hex = Poco::Crypto::DigestEngine::digestToHex(dg.digest());
        // Push the result on the stack
        Var< String >::push(vm, hex);
    }
    catch (const std::exception & e)
    {
        return sq_throwerrorf(vm, _SC("Failed to hash: %s"), e.what());
    }
    // At this point we have a valid string on the stack
    return 1;
}

// ================================================================================================
void Register_POCO_Crypto(HSQUIRRELVM vm)
{
    Table ns(vm);

    ns.SquirrelFunc(_SC("Hash"), &SqGetHash);

    RootTable(vm).Bind(_SC("SqCrypto"), ns);
}

} // Namespace:: SqMod
