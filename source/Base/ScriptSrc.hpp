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
    typedef std::vector< Uint32 > Line;

    // --------------------------------------------------------------------------------------------
    Script      mExec; // Reference to the script object.
    String      mPath; // Path to the script file.
    String      mData; // The contents of the script file.
    Line        mLine; // List of lines of code in the data.

    /* --------------------------------------------------------------------------------------------
     * Read file contents and calculate information about the lines of code.
    */
    void Process();

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    ScriptSrc(HSQUIRRELVM vm, const String & path, bool info = false);

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

};


} // Namespace:: SqMod

#endif // _BASE_SCRIPTSRC_HPP_
