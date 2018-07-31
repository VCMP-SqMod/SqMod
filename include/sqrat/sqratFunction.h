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

// Represents a function in Squirrel
class Function  {

    friend class TableBase;
    friend class Table;
    friend class ArrayBase;
    friend struct Var<Function>;

private:

    HSQOBJECT env, obj;

public:
    // Default constructor (null)
    Function() noexcept {
        sq_resetobject(&env);
        sq_resetobject(&obj);
    }
    // Copy constructor
    Function(const Function& sf) noexcept : env(sf.env), obj(sf.obj) {
        sq_addref(DefaultVM::Get_(), &env);
        sq_addref(DefaultVM::Get_(), &obj);
    }
    // Move constructor
    Function(Function&& sf) noexcept : env(sf.env), obj(sf.obj) {
        sq_resetobject(&sf.GetEnv());
        sq_resetobject(&sf.GetFunc());
    }
    // Constructs a Function from a slot in an Object
    Function(const Object& e, const SQChar* slot) : env(e.GetObject()) {
        sq_addref(DefaultVM::Get_(), &env);
        Object so = e.GetSlot(slot);
        obj = so.GetObject();
        sq_addref(DefaultVM::Get_(), &obj);
#if !defined (SCRAT_NO_ERROR_CHECKING)
        SQObjectType value_type = so.GetType();
        if (value_type != OT_CLOSURE && value_type != OT_NATIVECLOSURE) {
            // Note that classes can also be considered functions in Squirrel
            SQTHROW(DefaultVM::Get_(), _SC("function not found in slot"));
        }
#endif
    }
    // Constructs a Function from a value off the stack at the specified index
    // Assumes the Function environment is at index 1.
    Function(HSQUIRRELVM vm, SQInteger idx) {
        sq_getstackobj(vm, 1, &env);
        sq_getstackobj(vm, idx, &obj);
        sq_addref(vm, &env);
        sq_addref(vm, &obj);
#if !defined (SCRAT_NO_ERROR_CHECKING)
        SQObjectType value_type = sq_gettype(vm, idx);
        if (value_type != OT_CLOSURE && value_type != OT_NATIVECLOSURE) {
            SQTHROW(vm, FormatTypeError(vm, idx, _SC("closure")));
        }
#endif
    }
    // Constructs a Function from two Squirrel objects (one is the environment object and the other is the function object)
    Function(HSQUIRRELVM vm, HSQOBJECT e, HSQOBJECT o) noexcept : env(e), obj(o) {
        sq_addref(vm, &env);
        sq_addref(vm, &obj);
    }
    // Destructor
    ~Function() {
        Release();
    }
    // Copy Assignment operator
    Function& operator=(const Function& sf) noexcept {
        Release();
        env = sf.env;
        obj = sf.obj;
        if (!sf.IsNull()) {
            sq_addref(DefaultVM::Get_(), &env);
            sq_addref(DefaultVM::Get_(), &obj);
        }
        return *this;
    }
    // Move Assignment operator
    Function& operator=(Function&& sf) noexcept {
        Release();
        env = sf.env;
        obj = sf.obj;
        sq_resetobject(&sf.GetEnv());
        sq_resetobject(&sf.GetFunc());
        return *this;
    }
    // Checks whether the Function is null
    bool IsNull() const noexcept {
        return sq_isnull(obj);
    }
    // Gets the Squirrel environment object for this Function (copy)
    HSQOBJECT GetEnv() const noexcept {
        return env;
    }
    // Gets the Squirrel environment object for this Function (reference)
    HSQOBJECT& GetEnv() noexcept {
        return env;
    }
    // Gets the Squirrel function object for this Function (copy)
    HSQOBJECT GetFunc() const noexcept {
        return obj;
    }
    // Gets the Squirrel function object for this Function (reference)
    HSQOBJECT& GetFunc() noexcept {
        return obj;
    }
    // Gets the Squirrel VM for this Function
    HSQUIRRELVM & GetVM() const noexcept {
        return DefaultVM::Get_();
    }
    // Sets the Function to null (removing its references to underlying Squirrel objects)
    void Release() noexcept {
        if(!IsNull()) {
            sq_release(DefaultVM::Get_(), &env);
            sq_release(DefaultVM::Get_(), &obj);
            sq_resetobject(&env);
            sq_resetobject(&obj);
        }
    }
    // Sets the Function to null (removing its references to underlying Squirrel objects)
    // This doesn't call release if the reference count is 1.
    // Workaround for some weird squirrel behavior that generates an assert in debug mode.
    void ReleaseGently() noexcept {
        if(!IsNull()) {
            sq_release(DefaultVM::Get_(), &env);
            if (sq_getrefcount(DefaultVM::Get_(), &obj) > 1) {
                sq_release(DefaultVM::Get_(), &obj);
            }
            sq_resetobject(&env);
            sq_resetobject(&obj);
        }
    }
    // Runs the Function and returns its value as a SharedPtr
    template<class R, class... Args> SharedPtr<R> Evaluate(Args &&... args) {
        static constexpr unsigned ARGC = sizeof...(Args) + 1; // + environment
        HSQUIRRELVM vm = DefaultVM::Get_();
        SQInteger top = sq_gettop(vm);
        // Push the environment followed by the function
        sq_pushobject(vm, obj);
        sq_pushobject(vm, env);
        // Validate the funtion parameter count
#if !defined (SCRAT_NO_ERROR_CHECKING)
        SQUnsignedInteger nparams;
        SQUnsignedInteger nfreevars;
        if (SQ_SUCCEEDED(sq_getclosureinfo(vm, -2, &nparams, &nfreevars)) && (nparams != ARGC)) {
            sq_pop(vm, 2);
            SQTHROW(vm, _SC("wrong number of parameters"));
            return SharedPtr<R>();
        }
#endif
        // Push the arguments
        PushVars(vm, std::forward<Args>(args)...);

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
    // Runs the Function
    template< class... Args >
    void Execute(Args &&... args) {
        static constexpr unsigned ARGC = sizeof...(Args) + 1; // + environment
        HSQUIRRELVM vm = DefaultVM::Get_();
        SQInteger top = sq_gettop(vm);
        // Push the environment followed by the function
        sq_pushobject(vm, obj);
        sq_pushobject(vm, env);
        // Validate the funtion parameter count
#if !defined (SCRAT_NO_ERROR_CHECKING)
        SQUnsignedInteger nparams;
        SQUnsignedInteger nfreevars;
        if (SQ_SUCCEEDED(sq_getclosureinfo(vm, -2, &nparams, &nfreevars)) && (nparams != ARGC)) {
            sq_pop(vm, 2);
            SQTHROW(vm, _SC("wrong number of parameters"));
            return;
        }
#endif

        PushVars(vm, std::forward<Args>(args)...);

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
    // Runs the Function
    template< class... Args > void operator() (Args &&... args) {
        Execute(std::forward<Args>(args)...);
    }
};

// Used to get and push Function instances to and from the stack as references (functions are always references in Squirrel)
template<> struct Var<Function> {
    /// The actual value of get operations
    Function value;
    // Attempts to get the value off the stack at idx as a Function
    // Assumes the Function environment is at index 1.
    Var(HSQUIRRELVM vm, SQInteger idx) : value(vm, idx) {
    }
    // Called by Sqrat::PushVar to put a Function on the stack
    static void push(HSQUIRRELVM vm, const Function& value) noexcept {
        sq_pushobject(vm, value.GetFunc());
    }
};
// Used to get and push Function instances to and from the stack as references (functions are always references in Squirrel)
template<> struct Var<Function&> : public Var<Function> {
    Var(HSQUIRRELVM vm, SQInteger idx) : Var<Function>(vm, idx)
    {
    }
};
// Used to get and push Function instances to and from the stack as references (functions are always references in Squirrel)
template<> struct Var<const Function&> : public Var<Function> {
    Var(HSQUIRRELVM vm, SQInteger idx) : Var<Function>(vm, idx)
    {
    }
};

}

#endif
