//
// SqratScript: Script Compilation and Execution
//

//
// Copyright (c) 2009 Brandon Jones
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
//  1. The origin of this software must not be misrepresented; you must not
//  claim that you wrote the original software. If you use this software
//  in a product, an acknowledgment in the product documentation would be
//  appreciated but is not required.
//
//  2. Altered source versions must be plainly marked as such, and must not be
//  misrepresented as being the original software.
//
//  3. This notice may not be removed or altered from any source
//  distribution.
//

#pragma once

#include <squirrelex.h>
#include <sqstdio.h>

#include <string.h>

#include "sqratObject.h"

namespace Sqrat {

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Helper class for managing Squirrel scripts
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Script : public Object {
public:

    using Object::Object;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Default constructor
    ///
    /// \param v VM that the Script will be associated with
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Script() : Object(true) {
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Sets up the Script using a string containing a Squirrel script
    ///
    /// \param script String containing a file path to a Squirrel script
    /// \param name   Optional string containing the script's name (for errors)
    ///
    /// \remarks
    /// This function MUST have its Error handled if it occurred.
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void CompileString(const string& script, const string& name = _SC("")) {
        HSQUIRRELVM vm = SqVM();
        if(!sq_isnull(mObj)) {
            sq_release(vm, &mObj);
            sq_resetobject(&mObj);
        }

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if(SQ_FAILED(sq_compilebuffer(vm, script.c_str(), static_cast<SQInteger>(script.size() /** sizeof(SQChar)*/), name.c_str(), true))) {
            SQTHROW(vm, LastErrorString(vm));
            return;
        }
#else
        sq_compilebuffer(vm, script.c_str(), static_cast<SQInteger>(script.size() /** sizeof(SQChar)*/), name.c_str(), true);
#endif
        sq_getstackobj(vm,-1,&mObj);
        sq_addref(vm, &mObj);
        sq_pop(vm, 1);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Sets up the Script using a string containing a Squirrel script
    ///
    /// \param script String containing a file path to a Squirrel script
    /// \param errMsg String that is filled with any errors that may occur
    /// \param name   Optional string containing the script's name (for errors)
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    bool CompileString(const string& script, string& errMsg, const string& name = _SC("")) {
        HSQUIRRELVM vm = SqVM();
        if(!sq_isnull(mObj)) {
            sq_release(vm, &mObj);
            sq_resetobject(&mObj);
        }

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if(SQ_FAILED(sq_compilebuffer(vm, script.c_str(), static_cast<SQInteger>(script.size() /** sizeof(SQChar)*/), name.c_str(), true))) {
            errMsg = LastErrorString(vm);
            return false;
        }
#else
        sq_compilebuffer(vm, script.c_str(), static_cast<SQInteger>(script.size() /** sizeof(SQChar)*/), name.c_str(), true);
#endif
        sq_getstackobj(vm,-1,&mObj);
        sq_addref(vm, &mObj);
        sq_pop(vm, 1);
        return true;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Sets up the Script using a file containing a Squirrel script
    ///
    /// \param path File path containing a Squirrel script
    ///
    /// \remarks
    /// This function MUST have its Error handled if it occurred.
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void CompileFile(const string& path) {
        HSQUIRRELVM vm = SqVM();
        if(!sq_isnull(mObj)) {
            sq_release(vm, &mObj);
            sq_resetobject(&mObj);
        }

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if(SQ_FAILED(sqstd_loadfile(vm, path.c_str(), true))) {
            SQTHROW(vm, LastErrorString(vm));
            return;
        }
#else
        sqstd_loadfile(vm, path.c_str(), true);
#endif
        sq_getstackobj(vm,-1,&mObj);
        sq_addref(vm, &mObj);
        sq_pop(vm, 1);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Sets up the Script using a file containing a Squirrel script
    ///
    /// \param path   File path containing a Squirrel script
    /// \param errMsg String that is filled with any errors that may occur
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    bool CompileFile(const string& path, string& errMsg) {
        HSQUIRRELVM vm = SqVM();
        if(!sq_isnull(mObj)) {
            sq_release(vm, &mObj);
            sq_resetobject(&mObj);
        }

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if(SQ_FAILED(sqstd_loadfile(vm, path.c_str(), true))) {
            errMsg = LastErrorString(vm);
            return false;
        }
#else
        sqstd_loadfile(vm, path.c_str(), true);
#endif
        sq_getstackobj(vm,-1,&mObj);
        sq_addref(vm, &mObj);
        sq_pop(vm, 1);
        return true;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Runs the script
    ///
    /// \remarks
    /// This function MUST have its Error handled if it occurred.
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void Run() {
        HSQUIRRELVM vm = SqVM();
#if !defined (SCRAT_NO_ERROR_CHECKING)
        if(!sq_isnull(mObj)) {
            SQRESULT result;
            SQInteger top = sq_gettop(vm);
            sq_pushobject(vm, mObj);
            sq_pushroottable(vm);
            result = sq_call(vm, 1, false, true);
            sq_settop(vm, top);
            if(SQ_FAILED(result)) {
                SQTHROW(vm, LastErrorString(vm));
                return;
            }
        }
#else
        SQInteger top = sq_gettop(vm);
        sq_pushobject(vm, obj);
        sq_pushroottable(vm);
        sq_call(vm, 1, false, true);
        sq_settop(vm, top);
#endif
    }

#if !defined (SCRAT_NO_ERROR_CHECKING)
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Runs the script
    ///
    /// \param errMsg String that is filled with any errors that may occur
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    bool Run(string& errMsg) {
        HSQUIRRELVM vm = SqVM();
        if(!sq_isnull(mObj)) {
            SQRESULT result;
            SQInteger top = sq_gettop(vm);
            sq_pushobject(vm, mObj);
            sq_pushroottable(vm);
            result = sq_call(vm, 1, false, true);
            sq_settop(vm, top);
            if(SQ_FAILED(result)) {
                errMsg = LastErrorString(vm);
                return false;
            }
            return true;
        }
        return false;
    }
#endif

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Writes the byte code of the Script to a file
    ///
    /// \param path File path to write to
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void WriteCompiledFile(const string& path) {
        HSQUIRRELVM vm = SqVM();
#if !defined (SCRAT_NO_ERROR_CHECKING)
        if(!sq_isnull(mObj)) {
            sq_pushobject(vm, mObj);
            sqstd_writeclosuretofile(vm, path.c_str());
        }
#else
        sq_pushobject(vm, obj);
        sqstd_writeclosuretofile(vm, path.c_str());
#endif
        sq_pop(vm, 1); // needed?
    }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Used to get and push Script instances to and from the stack as references (Script is always a reference)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<>
struct Var<Script> {

    Script value; ///< The actual value of get operations

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Attempts to get the value off the stack at idx as an Script
    ///
    /// \param vm  Target VM
    /// \param idx Index trying to be read
    ///
    /// \remarks
    /// This function MUST have its Error handled if it occurred.
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Var(HSQUIRRELVM vm, SQInteger idx) : value(idx, vm) {
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Called by Sqrat::PushVar to put an Script on the stack
    ///
    /// \param vm    Target VM
    /// \param value Value to push on to the VM's stack
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    static void push(HSQUIRRELVM vm, const Script& value) {
        sq_pushobject(vm, value.GetObj());
    }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Used to get and push Script instances to and from the stack as references (Script is always a reference)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<>
struct Var<Script&> : Var<Script> {Var(HSQUIRRELVM vm, SQInteger idx) : Var<Script>(vm, idx) {}};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Used to get and push Script instances to and from the stack as references (Script is always a reference)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<>
struct Var<const Script&> : Var<Script> {Var(HSQUIRRELVM vm, SQInteger idx) : Var<Script>(vm, idx) {}};

}
