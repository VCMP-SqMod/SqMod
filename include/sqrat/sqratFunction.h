//
// sqratFunction: Squirrel Function Wrapper
//

//
// Copyright (c) 2009 Brandon Jones
// Copyirght 2011 Li-Cheng (Andy) Tai
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

#if !defined(_SCRAT_SQFUNC_H_)
#define _SCRAT_SQFUNC_H_

#ifdef SQMOD_PLUGIN_API
    #include <SqAPI.h>
#else
    #include <squirrel.h>
#endif // SQMOD_PLUGIN_API

#include "sqratObject.h"

namespace Sqrat {

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents a function in Squirrel
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Function  {

    friend class TableBase;
    friend class Table;
    friend class ArrayBase;
    friend struct Var<Function>;

private:

    HSQUIRRELVM vm;
    HSQOBJECT env, obj;

public:
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Default constructor (null)
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Function() {
        sq_resetobject(&env);
        sq_resetobject(&obj);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Copy constructor
    ///
    /// \param sf Function to copy
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Function(const Function& sf) : vm(sf.vm), env(sf.env), obj(sf.obj) {
        sq_addref(vm, &env);
        sq_addref(vm, &obj);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Move constructor
    ///
    /// \param sf Function to move
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Function(Function&& sf) : vm(sf.vm), env(sf.env), obj(sf.obj) {
        sq_resetobject(&sf.GetEnv());
        sq_resetobject(&sf.GetFunc());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Constructs a Function from a slot in an Object
    ///
    /// \param e    Object that potentially contains a Squirrel function in a slot
    /// \param slot Name of the slot to look for the Squirrel function in
    ///
    /// \remarks
    /// This function MUST have its Error handled if it occurred.
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Function(const Object& e, const SQChar* slot) : vm(e.GetVM()), env(e.GetObject()) {
        sq_addref(vm, &env);
        Object so = e.GetSlot(slot);
        obj = so.GetObject();
        sq_addref(vm, &obj);
#if !defined (SCRAT_NO_ERROR_CHECKING)
        SQObjectType value_type = so.GetType();
        if (value_type != OT_CLOSURE && value_type != OT_NATIVECLOSURE && value_type != OT_CLASS) {
            // Note that classes can also be considered functions in Squirrel
            SQTHROW(vm, _SC("function not found in slot"));
        }
#endif
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Constructs a Function from two Squirrel objects (one is the environment object and the other is the function object)
    ///
    /// \param v VM that the function will exist in
    /// \param e Squirrel object that should represent the environment of the function
    /// \param o Squirrel object that should already represent a Squirrel function
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Function(HSQUIRRELVM v, HSQOBJECT e, HSQOBJECT o) : vm(v), env(e), obj(o) {
        sq_addref(vm, &env);
        sq_addref(vm, &obj);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Destructor
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ~Function() {
        Release();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Assignment operator
    ///
    /// \param sf Function to copy
    ///
    /// \return The Function itself
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Function& operator=(const Function& sf) {
        Release();
        vm = sf.vm;
        env = sf.env;
        obj = sf.obj;
        sq_addref(vm, &env);
        sq_addref(vm, &obj);
        return *this;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Assignment operator
    ///
    /// \param sf Function to move
    ///
    /// \return The Function itself
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Function& operator=(Function&& sf) {
        Release();
        vm = sf.vm;
        env = sf.env;
        obj = sf.obj;
        sq_resetobject(&sf.GetEnv());
        sq_resetobject(&sf.GetFunc());
        return *this;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Checks whether the Function is null
    ///
    /// \return True if the Function currently has a null value, otherwise false
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    bool IsNull() {
        return sq_isnull(obj);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Gets the Squirrel environment object for this Function (copy)
    ///
    /// \return Squirrel object representing the Function environment
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    HSQOBJECT GetEnv() const {
        return env;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Gets the Squirrel environment object for this Function (reference)
    ///
    /// \return Squirrel object representing the Function environment
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    HSQOBJECT& GetEnv() {
        return env;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Gets the Squirrel function object for this Function (copy)
    ///
    /// \return Squirrel object representing the Function
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    HSQOBJECT GetFunc() const {
        return obj;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Gets the Squirrel function object for this Function (reference)
    ///
    /// \return Squirrel object representing the Function
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    HSQOBJECT& GetFunc() {
        return obj;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Gets the Squirrel VM for this Function (copy)
    ///
    /// \return Squirrel VM associated with the Function
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    HSQUIRRELVM GetVM() const {
        return vm;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Gets the Squirrel VM for this Function (reference)
    ///
    /// \return Squirrel VM associated with the Function
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    HSQUIRRELVM& GetVM() {
        return vm;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Sets the Function to null (removing its references to underlying Squirrel objects)
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void Release() {
        if(!IsNull()) {
            sq_release(vm, &env);
            sq_release(vm, &obj);
            sq_resetobject(&env);
            sq_resetobject(&obj);
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Sets the Function to null (removing its references to underlying Squirrel objects)
    ///
    /// \remarks
    /// This doesn't call release if the reference count is 1.
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void ReleaseGently() {
        if(!IsNull()) {
            sq_release(vm, &env);
            if (sq_getrefcount(vm, &obj) > 1)
            {
                sq_release(vm, &obj);
            }
            sq_resetobject(&env);
            sq_resetobject(&obj);
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Runs the Function and returns its value as a SharedPtr
    ///
    /// \return SharedPtr containing the return value (or null if failed)
    ///
    /// \remarks
    /// This function MUST have its Error handled if it occurred.
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    template <class R, class... Args>
    SharedPtr<R> Evaluate(Args &&... args) {
        static constexpr unsigned ARGC = sizeof...(Args) + 1; // + environment

        SQInteger top = sq_gettop(vm);

        sq_pushobject(vm, obj);
        sq_pushobject(vm, env);

#if !defined (SCRAT_NO_ERROR_CHECKING)
        SQUnsignedInteger nparams;
        SQUnsignedInteger nfreevars;
        if (SQ_SUCCEEDED(sq_getclosureinfo(vm, -2, &nparams, &nfreevars)) && (nparams != ARGC)) {
            sq_pop(vm, 2);
            SQTHROW(vm, _SC("wrong number of parameters"));
            return SharedPtr<R>();
        }
#endif

        PushVars(vm, std::forward< Args >(args)...);

#if !defined (SCRAT_NO_ERROR_CHECKING)
        SQRESULT result = sq_call(vm, ARGC, true, ErrorHandling::IsEnabled());

        //handle an error: pop the stack and throw the exception
        if (SQ_FAILED(result)) {
            sq_settop(vm, top);
            SQTHROW(vm, LastErrorString(vm));
            return SharedPtr<R>();
        }
#else
        sq_call(vm, ARGC, true, ErrorHandling::IsEnabled());
#endif

        SharedPtr<R> ret = Var<SharedPtr<R> >(vm, -1).value;
        sq_settop(vm, top);
        return ret;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Runs the Function
    ///
    /// \remarks
    /// This function MUST have its Error handled if it occurred.
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    template < class... Args >
    void Execute(Args &&... args) {
        static constexpr unsigned ARGC = sizeof...(Args) + 1; // + environment

        SQInteger top = sq_gettop(vm);

        sq_pushobject(vm, obj);
        sq_pushobject(vm, env);

#if !defined (SCRAT_NO_ERROR_CHECKING)
        SQUnsignedInteger nparams;
        SQUnsignedInteger nfreevars;
        if (SQ_SUCCEEDED(sq_getclosureinfo(vm, -2, &nparams, &nfreevars)) && (nparams != ARGC)) {
            sq_pop(vm, 2);
            SQTHROW(vm, _SC("wrong number of parameters"));
            return;
        }
#endif

        PushVars(vm, std::forward< Args >(args)...);

#if !defined (SCRAT_NO_ERROR_CHECKING)
        SQRESULT result = sq_call(vm, ARGC, false, ErrorHandling::IsEnabled());
        sq_settop(vm, top);

        //handle an error: throw the exception
        if (SQ_FAILED(result)) {
            SQTHROW(vm, LastErrorString(vm));
            return;
        }
#else
        sq_call(vm, ARGC, false, ErrorHandling::IsEnabled());
        sq_settop(vm, top);
#endif
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Runs the Function
    ///
    /// \remarks
    /// This function MUST have its Error handled if it occurred.
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    template < class... Args > void operator()(Args &&... args) {
        Execute(std::forward< Args >(args)...);
    }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Used to get and push Function instances to and from the stack as references (functions are always references in Squirrel)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<>
struct Var<Function> {

    Function value; ///< The actual value of get operations

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Attempts to get the value off the stack at idx as a Function
    ///
    /// \param vm  Target VM
    /// \param idx Index trying to be read
    ///
    /// \remarks
    /// Assumes the Function environment is at index 1.
    ///
    /// \remarks
    /// This function MUST have its Error handled if it occurred.
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Var(HSQUIRRELVM vm, SQInteger idx) {
        HSQOBJECT sqEnv;
        HSQOBJECT sqValue;
        sq_getstackobj(vm, 1, &sqEnv);
        sq_getstackobj(vm, idx, &sqValue);
        value = Function(vm, sqEnv, sqValue);
#if !defined (SCRAT_NO_ERROR_CHECKING)
        SQObjectType value_type = sq_gettype(vm, idx);
        if (value_type != OT_CLOSURE && value_type != OT_NATIVECLOSURE) {
            SQTHROW(vm, FormatTypeError(vm, idx, _SC("closure")));
        }
#endif
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Called by Sqrat::PushVar to put a Function on the stack
    ///
    /// \param vm    Target VM
    /// \param value Value to push on to the VM's stack
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    static void push(HSQUIRRELVM vm, const Function& value) {
        sq_pushobject(vm, value.GetFunc());
    }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Used to get and push Function instances to and from the stack as references (functions are always references in Squirrel)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<>
struct Var<Function&> : Var<Function> {Var(HSQUIRRELVM vm, SQInteger idx) : Var<Function>(vm, idx) {}};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Used to get and push Function instances to and from the stack as references (functions are always references in Squirrel)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<>
struct Var<const Function&> : Var<Function> {Var(HSQUIRRELVM vm, SQInteger idx) : Var<Function>(vm, idx) {}};

}

#endif
