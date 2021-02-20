// ------------------------------------------------------------------------------------------------
#include "PocoLib/Crypto.hpp"

// ------------------------------------------------------------------------------------------------
#include <iostream>
#include <sstream>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

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
	catch (const Poco::Exception& e)
	{
        return sq_throwerrorf(vm, _SC("Failed to hash: %s"), e.displayText().c_str());
	}
    catch (const std::exception & e)
    {
        return sq_throwerrorf(vm, _SC("Failed to hash: %s"), e.what());
    }
    // At this point we have a valid string on the stack
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqEncodeBase32(HSQUIRRELVM vm)
{
    // Attempt to retrieve the value from the stack as a string
    StackStrF val(vm, 2);
    // Have we failed to retrieve the string?
    if (SQ_FAILED(val.Proc(true)))
    {
        return val.mRes; // Propagate the error!
    }
    // Prevent any exceptions from reach the VM
    try
    {
        // Create a string receiver
        std::ostringstream out;
        // Create the encoder
        Poco::Base32Encoder enc(out);
        // Encode the string
        enc.write(val.mPtr, static_cast< size_t >(val.mLen));
        // Close the encoder
        enc.close();
        // Push the resulted string on the stack
        Var< String >::push(vm, out.str());
    }
	catch (const Poco::Exception& e)
	{
        return sq_throwerrorf(vm, _SC("Failed to encode: %s"), e.displayText().c_str());
	}
    catch (const std::exception & e)
    {
        return sq_throwerrorf(vm, _SC("Failed to encode: %s"), e.what());
    }
    // At this point we have a valid string on the stack
    return 1;
}
// ------------------------------------------------------------------------------------------------
static SQInteger SqDecodeBase32(HSQUIRRELVM vm)
{
    // Attempt to retrieve the value from the stack as a string
    StackStrF val(vm, 2);
    // Have we failed to retrieve the string?
    if (SQ_FAILED(val.Proc(true)))
    {
        return val.mRes; // Propagate the error!
    }
    // Prevent any exceptions from reach the VM
    try
    {
        // Create a string receiver
        std::stringstream io;
        // Put the encoded that in the stream
        io.write(val.mPtr, static_cast< size_t >(val.mLen));
        // Create the encoder
        Poco::Base32Decoder dec(io);
        // Create a string to receive the decoded data
        String out;
        // Decode the string
        dec >> out;
        // Push the resulted string on the stack
        Var< String >::push(vm, out);
    }
	catch (const Poco::Exception& e)
	{
        return sq_throwerrorf(vm, _SC("Failed to decode: %s"), e.displayText().c_str());
	}
    catch (const std::exception & e)
    {
        return sq_throwerrorf(vm, _SC("Failed to decode: %s"), e.what());
    }
    // At this point we have a valid string on the stack
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqEncodeBase64(HSQUIRRELVM vm)
{
    // Attempt to retrieve the value from the stack as a string
    StackStrF val(vm, 2);
    // Have we failed to retrieve the string?
    if (SQ_FAILED(val.Proc(true)))
    {
        return val.mRes; // Propagate the error!
    }
    // Prevent any exceptions from reach the VM
    try
    {
        // Create a string receiver
        std::ostringstream out;
        // Create the encoder
        Poco::Base64Encoder enc(out);
        // Encode the string
        enc.write(val.mPtr, static_cast< size_t >(val.mLen));
        // Close the encoder
        enc.close();
        // Push the resulted string on the stack
        Var< String >::push(vm, out.str());
    }
	catch (const Poco::Exception& e)
	{
        return sq_throwerrorf(vm, _SC("Failed to encode: %s"), e.displayText().c_str());
	}
    catch (const std::exception & e)
    {
        return sq_throwerrorf(vm, _SC("Failed to encode: %s"), e.what());
    }
    // At this point we have a valid string on the stack
    return 1;
}
// ------------------------------------------------------------------------------------------------
static SQInteger SqDecodeBase64(HSQUIRRELVM vm)
{
    // Attempt to retrieve the value from the stack as a string
    StackStrF val(vm, 2);
    // Have we failed to retrieve the string?
    if (SQ_FAILED(val.Proc(true)))
    {
        return val.mRes; // Propagate the error!
    }
    // Prevent any exceptions from reach the VM
    try
    {
        // Create a string receiver
        std::stringstream io;
        // Put the encoded that in the stream
        io.write(val.mPtr, static_cast< size_t >(val.mLen));
        // Create the encoder
        Poco::Base64Decoder dec(io);
        // Create a string to receive the decoded data
        String out;
        // Decode the string
        dec >> out;
        // Push the resulted string on the stack
        Var< String >::push(vm, out);
    }
	catch (const Poco::Exception& e)
	{
        return sq_throwerrorf(vm, _SC("Failed to decode: %s"), e.displayText().c_str());
	}
    catch (const std::exception & e)
    {
        return sq_throwerrorf(vm, _SC("Failed to decode: %s"), e.what());
    }
    // At this point we have a valid string on the stack
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqGetCRC32(HSQUIRRELVM vm)
{
    // Attempt to retrieve the value from the stack as a string
    StackStrF val(vm, 2);
    // Have we failed to retrieve the string?
    if (SQ_FAILED(val.Proc(true)))
    {
        return val.mRes; // Propagate the error!
    }
    // Prevent any exceptions from reach the VM
    try
    {
        // Create the checksum computer
        Poco::Checksum c(Poco::Checksum::TYPE_CRC32);
        // Give it the data to process
        c.update(val.mPtr, static_cast< uint32_t >(val.mLen));
        // Push the result on the stack
        sq_pushinteger(vm, static_cast< SQInteger >(c.checksum()));
    }
	catch (const Poco::Exception& e)
	{
        return sq_throwerrorf(vm, _SC("Failed to compute checksum: %s"), e.displayText().c_str());
	}
    catch (const std::exception & e)
    {
        return sq_throwerrorf(vm, _SC("Failed to compute checksum: %s"), e.what());
    }
    // At this point we have a valid string on the stack
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqGetADLER32(HSQUIRRELVM vm)
{
    // Attempt to retrieve the value from the stack as a string
    StackStrF val(vm, 2);
    // Have we failed to retrieve the string?
    if (SQ_FAILED(val.Proc(true)))
    {
        return val.mRes; // Propagate the error!
    }
    // Prevent any exceptions from reach the VM
    try
    {
        // Create the checksum computer
        Poco::Checksum c(Poco::Checksum::TYPE_ADLER32);
        // Give it the data to process
        c.update(val.mPtr, static_cast< uint32_t >(val.mLen));
        // Push the result on the stack
        sq_pushinteger(vm, static_cast< SQInteger >(c.checksum()));
    }
	catch (const Poco::Exception& e)
	{
        return sq_throwerrorf(vm, _SC("Failed to compute checksum: %s"), e.displayText().c_str());
	}
    catch (const std::exception & e)
    {
        return sq_throwerrorf(vm, _SC("Failed to compute checksum: %s"), e.what());
    }
    // At this point we have a valid string on the stack
    return 1;
}

// ================================================================================================
void Register_POCO_Crypto(HSQUIRRELVM vm, Table &)
{
    Table ns(vm);

    ns.SquirrelFunc(_SC("Hash"), &SqGetHash);
    ns.SquirrelFunc(_SC("EncodeBase32"), &SqEncodeBase32);
    ns.SquirrelFunc(_SC("DecodeBase32"), &SqDecodeBase32);
    ns.SquirrelFunc(_SC("EncodeBase64"), &SqEncodeBase64);
    ns.SquirrelFunc(_SC("DecodeBase64"), &SqDecodeBase64);
    ns.SquirrelFunc(_SC("CRC32"), &SqGetCRC32);
    ns.SquirrelFunc(_SC("ADLER32"), &SqGetADLER32);

    RootTable(vm).Bind(_SC("SqCrypto"), ns);
}

} // Namespace:: SqMod
