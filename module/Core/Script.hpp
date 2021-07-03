#pragma once

// ------------------------------------------------------------------------------------------------
#include "Core/Common.hpp"

// ------------------------------------------------------------------------------------------------
#include <vector>
#include <utility>

// ------------------------------------------------------------------------------------------------
#include <sqratScript.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
class Core;

/* ------------------------------------------------------------------------------------------------
 * Hold a information about loaded scripts as it's contents and executable code.
*/
struct ScriptSrc
{
    // --------------------------------------------------------------------------------------------
    typedef std::vector< std::pair< uint32_t, uint32_t > > Line;

    // --------------------------------------------------------------------------------------------
    Script      mExec{}; // Reference to the script object.
    Function    mFunc{}; // Callback to invoke after script was executed.
    LightObj    mCtx{}; // Object to be passed over to the callback as context.
    String      mPath{}; // Path to the script file.
    String      mData{}; // The contents of the script file.
    Line        mLine{}; // List of lines of code in the data.
    bool        mInfo{false}; // Whether this script contains line information.
    bool        mDelay{false}; // Don't execute immediately after compilation.

    /* --------------------------------------------------------------------------------------------
     * Read file contents and calculate information about the lines of code.
    */
    void Process();

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    explicit ScriptSrc(const String & path, Function & cb, LightObj & ctx, bool delay = false, bool info = false);

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    ScriptSrc(const ScriptSrc & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    ScriptSrc(ScriptSrc && o) = default;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    ScriptSrc & operator = (const ScriptSrc & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    ScriptSrc & operator = (ScriptSrc && o) = default;

    /* --------------------------------------------------------------------------------------------
     * Fetches a line from the code. Can also trim whitespace at the beginning.
    */
    SQMOD_NODISCARD String FetchLine(size_t line, bool trim = true) const;
};


} // Namespace:: SqMod
