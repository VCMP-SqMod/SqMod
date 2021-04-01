#pragma once

// ------------------------------------------------------------------------------------------------
#include "Core/Utility.hpp"

// ------------------------------------------------------------------------------------------------
#include <fmt/args.h>
#include <fmt/format.h>

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
    SQMOD_NODISCARD SQInteger Proc(HSQUIRRELVM vm, SQInteger text, SQInteger args, SQInteger end = -1);
};

} // Namespace:: SqMod
