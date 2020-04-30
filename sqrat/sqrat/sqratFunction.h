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
    #include <squirrelex.h>
#endif // SQMOD_PLUGIN_API

#include "sqratObject.h"

namespace Sqrat {

// Represents a function in Squirrel
struct Function  {

    friend class TableBase;
    friend class Table;
    friend class ArrayBase;
    friend struct Var<Function>;

    HSQOBJECT mEnv, mObj;

    // Default constructor (null)
    Function() {
        sq_resetobject(&mEnv);
        sq_resetobject(&mObj);
    }
    // Copy constructor
    Function(const Function& o) : mEnv(o.mEnv), mObj(o.mObj) {
        sq_addref(SqVM(), &mEnv);
        sq_addref(SqVM(), &mObj);
    }
    // Move constructor
    Function(Function&& o) noexcept : mEnv(o.mEnv), mObj(o.mObj) {
        sq_resetobject(&o.mEnv);
        sq_resetobject(&o.mObj);
    }
    // Constructs a Function from a slot in an Object
    Function(const Object& e, const SQChar* slot) : mEnv(e.GetObj()) {
        sq_addref(SqVM(), &mEnv);
        Object so = e.GetSlot(slot);
        mObj = so.GetObj();
        sq_addref(SqVM(), &mObj);
#if !defined (SCRAT_NO_ERROR_CHECKING)
        SQObjectType value_type = so.GetType();
        if (value_type != OT_CLOSURE && value_type != OT_NATIVECLOSURE) {
            // Note that classes can also be considered functions in Squirrel
            SQTHROW(SqVM(), _SC("function not found in slot"));
        }
#endif
    }
    // Constructs a Function from a value off the stack at the specified index
    // Assumes the Function environment is at index 1.
    Function(HSQUIRRELVM vm, SQInteger idx) {
        sq_getstackobj(vm, 1, &mEnv);
        sq_getstackobj(vm, idx, &mObj);
        sq_addref(vm, &mEnv);
        sq_addref(vm, &mObj);
#if !defined (SCRAT_NO_ERROR_CHECKING)
        SQObjectType value_type = sq_gettype(vm, idx);
        if (value_type != OT_CLOSURE && value_type != OT_NATIVECLOSURE) {
            SQTHROW(vm, FormatTypeError(vm, idx, _SC("closure")));
        }
#endif
    }
    // Constructs a Function from a value off the stack at the specified index pointed by `idx2`
    // Assumes the Function environment is at index pointed by `idx1`
    // If `idx1` is null, it defaults to root table
    // If `idx1` is an object, it will be used as the environment
    // If `idx1` is actually a function/closure then `idx2` is ignored
    Function(HSQUIRRELVM vm, SQInteger idx1, SQInteger idx2) : Function() {
        Assign(vm, idx1, idx2);
    }
    // Constructs a Function from two Squirrel objects (one is the environment object and the other is the function object)
    Function(HSQOBJECT e, HSQOBJECT o, HSQUIRRELVM vm) : mEnv(e), mObj(o) {
        sq_addref(vm, &mEnv);
        sq_addref(vm, &mObj);
    }
    // Destructor
    ~Function() {
        Release();
    }
    // Copy Assignment operator
    Function& operator=(const Function& o) {
        if (this != &o) {
            Release();
            mEnv = o.mEnv;
            mObj = o.mObj;
            sq_addref(SqVM(), &mEnv);
            sq_addref(SqVM(), &mObj);
        }
        return *this;
    }
    // Move Assignment operator
    Function& operator=(Function&& o) noexcept {
        if (this != &o) {
            Release();
            mEnv = o.mEnv;
            mObj = o.mObj;
            sq_resetobject(&o.mEnv);
            sq_resetobject(&o.mObj);
        }
        return *this;
    }
    // Checks whether the Function is null
    bool IsNull() const {
        return sq_isnull(mObj);
    }
    // Gets the Squirrel environment object for this Function (copy)
    HSQOBJECT GetEnv() const {
        return mEnv;
    }
    // Gets the Squirrel environment object for this Function (reference)
    HSQOBJECT& GetEnv() {
        return mEnv;
    }
    // Gets the Squirrel function object for this Function (copy)
    HSQOBJECT GetFunc() const {
        return mObj;
    }
    // Gets the Squirrel function object for this Function (reference)
    HSQOBJECT& GetFunc() {
        return mObj;
    }
    // Gets the Squirrel VM for this Function
    HSQUIRRELVM GetVM() const {
        return SqVM();
    }
    // Sets the Function to null (removing its references to underlying Squirrel objects)
    void Release() {
        if(!sq_isnull(mEnv)) {
            sq_release(SqVM(), &mEnv);
            sq_resetobject(&mEnv);
        }
        if(!sq_isnull(mObj)) {
            sq_release(SqVM(), &mObj);
            sq_resetobject(&mObj);
        }
    }
    // Assigns a Function from a value off the stack at the specified index pointed by `idx2`
    // Assumes the Function environment is at index pointed by `idx1`
    // If `idx1` is null, it defaults to root table
    // If `idx1` is an object, it will be used as the environment
    // If `idx1` is actually a function/closure then `idx2` is ignored
    bool Assign(HSQUIRRELVM vm, SQInteger idx1, SQInteger idx2, bool bind_null=false) {
        // Release current callback, if any
        Release();
        // Tells if the current environment was used
        bool cenv = false;
        // Retrieve the environment type
        const SQObjectType ty1 = sq_gettype(vm, idx1);
        // Should we use the root table as the environment?
        if (ty1 == OT_NULL) {
            sq_pushroottable(vm); // Push it on the stack
            sq_getstackobj(vm, -1, &mEnv); // Grab it
            sq_poptop(vm); // Clean up the stack
        // Should we use current environment?
        } else if (ty1 & (_RT_CLOSURE | _RT_NATIVECLOSURE)) {
            sq_getstackobj(vm, 1, &mEnv); // `this` as environment
            idx2 = idx1; // There is no explicit environment given
            cenv = true;
        // Is there a specific environment?
        } else if (ty1 & (_RT_TABLE | _RT_CLASS | _RT_INSTANCE)) {
            sq_getstackobj(vm, idx1, &mEnv); // Grab the given environment
        } else {
#if !defined (SCRAT_NO_ERROR_CHECKING)
            SQTHROW(vm, FormatTypeError(vm, idx1, _SC("object")));
#else
            return cenv;
#endif
        }
        // Retrieve the callback type
        const SQObjectType ty2 = sq_gettype(vm, idx2);
        // Can we bind null?
        if (bind_null && ty2 == OT_NULL) {
            sq_resetobject(&mEnv); // Drop the environment, if any
            return cenv; // Just stop knowing this is what we want
        // Is the callback is valid?
        } else if (ty2 & (_RT_CLOSURE | _RT_NATIVECLOSURE)) {
            if (SQ_FAILED(sq_getstackobj(vm, idx2, &mObj))) return false;
            // Reference the environment and function
            sq_addref(vm, &mEnv);
            sq_addref(vm, &mObj);
        } else {
            sq_resetobject(&mEnv); // Discard obtained environment
#if !defined (SCRAT_NO_ERROR_CHECKING)
            SQTHROW(vm, FormatTypeError(vm, idx2, _SC("closure")));
#endif
        }
        // Return whether current environment was used
        return cenv;
    }
    // Runs the Function and returns its value as a SharedPtr
    template<class R, class... Args> SharedPtr<R> Evaluate(Args &&... args) {
        static constexpr unsigned ARGC = sizeof...(Args) + 1; // + environment
        HSQUIRRELVM vm = SqVM();
        const SQInteger top = sq_gettop(vm);
        // Push the environment followed by the function
        sq_pushobject(vm, mObj);
        sq_pushobject(vm, mEnv);
        // Validate the funtion parameter count
#if !defined (SCRAT_NO_ERROR_CHECKING)
        SQInteger nparams;
        SQInteger nfreevars;
        if (SQ_SUCCEEDED(sq_getclosureinfo(vm, -2, &nparams, &nfreevars)) &&
            SqGlobalParamInspect< ArgFwd<Args...>::HASOPT >::Invalid(nparams, ARGC)) {
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
        HSQUIRRELVM vm = SqVM();
        const SQInteger top = sq_gettop(vm);
        // Push the environment followed by the function
        sq_pushobject(vm, mObj);
        sq_pushobject(vm, mEnv);
        // Validate the funtion parameter count
#if !defined (SCRAT_NO_ERROR_CHECKING)
        SQInteger nparams;
        SQInteger nfreevars;
        if (SQ_SUCCEEDED(sq_getclosureinfo(vm, -2, &nparams, &nfreevars)) &&
            SqGlobalParamInspect< ArgFwd<Args...>::HASOPT >::Invalid(nparams, ARGC)) {
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
    // Attempts to get the value off the stack at idx as a Function
    // Assumes the Function environment is at index 1.
    Var(HSQUIRRELVM vm, SQInteger idx1, SQInteger idx2) : value(vm, idx1, idx2) {
    }
    // Called by Sqrat::PushVar to put a Function on the stack
    static void push(HSQUIRRELVM vm, const Function& value) {
        sq_pushobject(vm, value.GetFunc());
    }
};
// Used to get and push Function instances to and from the stack as references (functions are always references in Squirrel)
template<> struct Var<Function&> : public Var<Function> {
    Var(HSQUIRRELVM vm, SQInteger idx) : Var<Function>(vm, idx)
    {
    }
    Var(HSQUIRRELVM vm, SQInteger idx1, SQInteger idx2) : Var<Function>(vm, idx1, idx2)
    {
    }
};
// Used to get and push Function instances to and from the stack as references (functions are always references in Squirrel)
template<> struct Var<const Function&> : public Var<Function> {
    Var(HSQUIRRELVM vm, SQInteger idx) : Var<Function>(vm, idx)
    {
    }
    Var(HSQUIRRELVM vm, SQInteger idx1, SQInteger idx2) : Var<Function>(vm, idx1, idx2)
    {
    }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Helper used to process callback arguments when necessary.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<> struct ArgFwdOptVar<Function, true> : public Var<Function> {
    ArgFwdOptVar(HSQUIRRELVM vm, SQInteger idx) : Var<Function>(vm, idx, idx+1) {}
};
template<> struct ArgFwdOptVar<Function&, true> : public Var<Function&> {
    ArgFwdOptVar(HSQUIRRELVM vm, SQInteger idx) : Var<Function&>(vm, idx, idx+1) {}
};
template<> struct ArgFwdOptVar<const Function&, true> : public Var<const Function&> {
    ArgFwdOptVar(HSQUIRRELVM vm, SQInteger idx) : Var<const Function&>(vm, idx, idx+1) {}
};

}

#endif
