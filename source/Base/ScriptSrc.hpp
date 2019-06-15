#ifndef _BASE_SCRIPTSRC_HPP_
#define _BASE_SCRIPTSRC_HPP_

// ------------------------------------------------------------------------------------------------
#include "Base/Utility.hpp"

// ------------------------------------------------------------------------------------------------
#include <vector>
#include <utility>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
class Core;

/* ------------------------------------------------------------------------------------------------
 * Hold a information about loaded scripts as it's contents and executable code.
*/
class ScriptSrc
{
public:

    // --------------------------------------------------------------------------------------------
    typedef std::vector< std::pair< Uint32, Uint32 > > Line;

    // --------------------------------------------------------------------------------------------
    Script      mExec; // Reference to the script object.
    String      mPath; // Path to the script file.
    String      mData; // The contents of the script file.
    Line        mLine; // List of lines of code in the data.
    bool        mInfo; // Whether this script contains line information.
    bool        mDelay; // Don't execute immediately after compilation.

    /* --------------------------------------------------------------------------------------------
     * Read file contents and calculate information about the lines of code.
    */
    void Process();

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    ScriptSrc(HSQUIRRELVM vm, const String & path, bool delay = false, bool info = false)
        : ScriptSrc(vm, String(path), delay, info) 
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    ScriptSrc(HSQUIRRELVM vm, String && path, bool delay = false, bool info = false);

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
     * Fetches a line from the code. Can also triim whitespace at the beginning.
    */
    String FetchLine(size_t line, bool trim = true) const;
};


} // Namespace:: SqMod

#endif // _BASE_SCRIPTSRC_HPP_
