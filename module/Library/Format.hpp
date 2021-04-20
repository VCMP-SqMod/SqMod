#pragma once

// ------------------------------------------------------------------------------------------------
#include "Core/Utility.hpp"

// ------------------------------------------------------------------------------------------------
#include <fmt/args.h>
#include <fmt/format.h>
#include <fmt/locale.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Utility used to generate a formatted string with parameters from the VM stack.
*/
struct FormatContext
{
    /* --------------------------------------------------------------------------------------------
     * The type of container that will store the argument values.
    */
    using Args = fmt::dynamic_format_arg_store< fmt::format_context >;

    /* --------------------------------------------------------------------------------------------
     * Format arguments container.
    */
    Args mArgs{};

    /* --------------------------------------------------------------------------------------------
     * Script result status.
    */
    SQInteger mRes{SQ_OK};

    /* --------------------------------------------------------------------------------------------
     * Format string.
    */
    fmt::basic_string_view< SQChar > mStr{};

    /* --------------------------------------------------------------------------------------------
     * Output string buffer.
    */
    String mOut{};

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    FormatContext() = default;

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    FormatContext(const FormatContext & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move constructor. (disabled)
    */
    FormatContext(FormatContext && o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~FormatContext() =  default;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator. (disabled)
    */
    FormatContext & operator = (const FormatContext & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator. (disabled)
    */
    FormatContext & operator = (FormatContext && o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Process the formatted string.
    */
    SQMOD_NODISCARD SQInteger Proc(HSQUIRRELVM vm, SQInteger text, SQInteger args, SQInteger end = -1)
    {
        if (SQ_SUCCEEDED(Process(vm, text, args, end)))
        {
            try
            {
                mOut = fmt::vformat(mStr, mArgs);
            }
            catch (const std::exception & e)
            {
                mRes = sq_throwerror(vm, e.what());
            }
        }
        // Return result
        return mRes;
    }

    /* --------------------------------------------------------------------------------------------
     * Process the formatted string.
    */
    SQMOD_NODISCARD SQInteger Process(HSQUIRRELVM vm, SQInteger text, SQInteger args, SQInteger end = -1);

    /* --------------------------------------------------------------------------------------------
     * Process the formatted string.
    */
    SQMOD_NODISCARD SQInteger Generate(HSQUIRRELVM vm)
    {
        // Attempt to generate the format string
        if (SQ_SUCCEEDED(mRes))
        {
            try
            {
                mOut = fmt::vformat(mStr, mArgs);
            }
            catch (const std::exception & e)
            {
                mRes = sq_throwerror(vm, e.what());
            }
        }
        // Return result
        return mRes;
    }

    /* --------------------------------------------------------------------------------------------
     * Process the formatted string.
    */
    SQMOD_NODISCARD SQInteger GenerateLoc(HSQUIRRELVM vm, const SQChar * loc)
    {
        // Attempt to generate the format string
        if (SQ_SUCCEEDED(mRes))
        {
            try
            {
                mOut = fmt::vformat(std::locale(loc), mStr, mArgs);
            }
            catch (const std::exception & e)
            {
                mRes = sq_throwerror(vm, e.what());
            }
        }
        // Return result
        return mRes;
    }
};

/* ------------------------------------------------------------------------------------------------
 * Helper function used to process a formatted string into the specified StackStrF instance.
*/
void ExtendedFormatProcess(StackStrF & ss, SQInteger top);

} // Namespace:: SqMod
