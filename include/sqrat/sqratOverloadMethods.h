//
// SqratOverloadMethods: Overload Methods
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

#if !defined(_SQRAT_OVERLOAD_METHODS_H_)
#define _SQRAT_OVERLOAD_METHODS_H_

#ifdef SQMOD_PLUGIN_API
    #include <SqAPI.h>
#else
    #include <squirrel.h>
#endif // SQMOD_PLUGIN_API

#include <sstream>
#include "sqratTypes.h"
#include "sqratUtil.h"
#include "sqratGlobalMethods.h"
#include "sqratMemberMethods.h"

namespace Sqrat {

/// @cond DEV

//
// Overload name generator
//

class SqOverloadName {
public:

    static string Get(const SQChar* name, int args) {
        std::basic_stringstream<SQChar> overloadName;
        overloadName << _SC("__overload_") << name << args;

        return overloadName.str();
    }
};


//
// Squirrel Overload Functions
//

template <class R>
class SqOverload {
public:

    static SQInteger Func(HSQUIRRELVM vm) {
        // Get the arg count
        int argCount = sq_gettop(vm) - 2;

        const SQChar* funcName;
        sq_getstring(vm, -1, &funcName); // get the function name (free variable)

        string overloadName = SqOverloadName::Get(funcName, argCount);

        sq_pushstring(vm, overloadName.c_str(), -1);

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (SQ_FAILED(sq_get(vm, 1))) { // Lookup the proper overload
            return sq_throwerror(vm, _SC("wrong number of parameters"));
        }
#else
        sq_get(vm, 1);
#endif

        // Push the args again
        for (int i = 1; i <= argCount + 1; ++i) {
            sq_push(vm, i);
        }

#if !defined (SCRAT_NO_ERROR_CHECKING)
        SQRESULT result = sq_call(vm, argCount + 1, true, ErrorHandling::IsEnabled());
        if (SQ_FAILED(result)) {
            return sq_throwerror(vm, LastErrorString(vm).c_str());
        }
#else
        sq_call(vm, argCount + 1, true, ErrorHandling::IsEnabled());
#endif

        return 1;
    }
};


//
// void return specialization
//

template <>
class SqOverload<void> {
public:

    static SQInteger Func(HSQUIRRELVM vm) {
        // Get the arg count
        int argCount = sq_gettop(vm) - 2;

        const SQChar* funcName;
        sq_getstring(vm, -1, &funcName); // get the function name (free variable)

        string overloadName = SqOverloadName::Get(funcName, argCount);

        sq_pushstring(vm, overloadName.c_str(), -1);

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (SQ_FAILED(sq_get(vm, 1))) { // Lookup the proper overload
            return sq_throwerror(vm, _SC("wrong number of parameters"));
        }
#else
        sq_get(vm, 1);
#endif

        // Push the args again
        for (int i = 1; i <= argCount + 1; ++i) {
            sq_push(vm, i);
        }

#if !defined (SCRAT_NO_ERROR_CHECKING)
        SQRESULT result = sq_call(vm, argCount + 1, false, ErrorHandling::IsEnabled());
        if (SQ_FAILED(result)) {
            return sq_throwerror(vm, LastErrorString(vm).c_str());
        }
#else
        sq_call(vm, argCount + 1, false, ErrorHandling::IsEnabled());
#endif

        return 0;
    }
};

// Global Overloaded Function Resolver
template <class R,class... A> SQFUNCTION SqGlobalOverloadedFunc(R (* /*method*/)(A...)) noexcept {
    return SqGlobal<R>::template GetProxy<2, true, A...>();
}

// Global Overloaded Function Resolver
template <class R,class... A> SQFUNCTION SqGlobalOverloadedFunc(R& (* /*method*/)(A...)) noexcept {
    return SqGlobal<R&>::template GetProxy<2, true, A...>();
}

// Member Global Overloaded Function Resolver
template <class R,class T,class... A> SQFUNCTION SqMemberGlobalOverloadedFunc(R (* /*method*/)(T, A...)) noexcept {
    return SqGlobal<R>::template GetProxy<1, true, T, A...>();
}

// Member Global Overloaded Function Resolver
template <class R,class T,class... A> SQFUNCTION SqMemberGlobalOverloadedFunc(R& (* /*method*/)(T, A...)) noexcept {
    return SqGlobal<R&>::template GetProxy<1, true, T, A...>();
}

// Member Overloaded Function Resolver
template <class C, class R,class... A> SQFUNCTION SqMemberOverloadedFunc(R (C::* /*method*/)(A...)) noexcept {
    return SqMember<C,R>::template GetProxy<true, A...>();
}

// Member Overloaded Function Resolver
template <class C, class R,class... A> SQFUNCTION SqMemberOverloadedFunc(R (C::* /*method*/)(A...) const) noexcept {
    return SqMember<C,R>::template GetProxyC<true, A...>();
}

// Member Overloaded Function Resolver
template <class C, class R,class... A> SQFUNCTION SqMemberOverloadedFunc(R& (C::* /*method*/)(A...)) noexcept {
    return SqMember<C,R&>::template GetProxy<true, A...>();
}

// Member Overloaded Function Resolver
template <class C, class R,class... A> SQFUNCTION SqMemberOverloadedFunc(R& (C::* /*method*/)(A...) const) noexcept {
    return SqMember<C,R&>::template GetProxyC<true, A...>();
}

//
// Overload handler resolver
//

template <class R>
inline SQFUNCTION SqOverloadFunc(R (* /*method*/)) {
    return &SqOverload<R>::Func;
}

template <class C, class R>
inline SQFUNCTION SqOverloadFunc(R (C::* /*method*/)) {
    return &SqOverload<R>::Func;
}

template <class C, class R>
inline SQFUNCTION SqOverloadFunc(R (C::* /*method*/)() const ) {
    return &SqOverload<R>::Func;
}

template <class C, class R, class A1>
inline SQFUNCTION SqOverloadFunc(R (C::* /*method*/)(A1) const ) {
    return &SqOverload<R>::Func;
}

template <class C, class R, class A1, class A2>
inline SQFUNCTION SqOverloadFunc(R (C::* /*method*/)(A1, A2) const ) {
    return &SqOverload<R>::Func;
}

template <class C, class R, class A1, class A2, class A3>
inline SQFUNCTION SqOverloadFunc(R (C::* /*method*/)(A1, A2, A3) const ) {
    return &SqOverload<R>::Func;
}

template <class C, class R, class A1, class A2, class A3, class A4>
inline SQFUNCTION SqOverloadFunc(R (C::* /*method*/)(A1, A2, A3, A4) const ) {
    return &SqOverload<R>::Func;
}

template <class C, class R, class A1, class A2, class A3, class A4, class A5>
inline SQFUNCTION SqOverloadFunc(R (C::* /*method*/)(A1, A2, A3, A4, A5) const ) {
    return &SqOverload<R>::Func;
}

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6>
inline SQFUNCTION SqOverloadFunc(R (C::* /*method*/)(A1, A2, A3, A4, A5, A6) const ) {
    return &SqOverload<R>::Func;
}

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7>
inline SQFUNCTION SqOverloadFunc(R (C::* /*method*/)(A1, A2, A3, A4, A5, A6, A7) const ) {
    return &SqOverload<R>::Func;
}

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8>
inline SQFUNCTION SqOverloadFunc(R (C::* /*method*/)(A1, A2, A3, A4, A5, A6, A7, A8) const ) {
    return &SqOverload<R>::Func;
}

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9>
inline SQFUNCTION SqOverloadFunc(R (C::* /*method*/)(A1, A2, A3, A4, A5, A6, A7, A8, A9) const ) {
    return &SqOverload<R>::Func;
}

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10>
inline SQFUNCTION SqOverloadFunc(R (C::* /*method*/)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10) const ) {
    return &SqOverload<R>::Func;
}

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11>
inline SQFUNCTION SqOverloadFunc(R (C::* /*method*/)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11) const ) {
    return &SqOverload<R>::Func;
}

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12>
inline SQFUNCTION SqOverloadFunc(R (C::* /*method*/)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12) const ) {
    return &SqOverload<R>::Func;
}

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13>
inline SQFUNCTION SqOverloadFunc(R (C::* /*method*/)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13) const ) {
    return &SqOverload<R>::Func;
}

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14>
inline SQFUNCTION SqOverloadFunc(R (C::* /*method*/)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14) const ) {
    return &SqOverload<R>::Func;
}

//
// Query argument count
//
template <class R, class... Args>
inline int SqGetArgCount(R (* /*method*/)(Args...)) {
    return sizeof...(Args);
}

//
// Query member function argument count
//
template <class C, class R, class... Args>
inline int SqGetArgCount(R (C::* /*method*/)(Args...)) {
    return sizeof...(Args);
}

//
// Query const member function argument count
//
template <class C, class R, class... Args>
inline int SqGetArgCount(R (C::* /*method*/)(Args...) const) {
    return sizeof...(Args);
}

/// @endcond

}

#endif
