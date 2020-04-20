//
// SqratGlobalMethods: Global Methods
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

#if !defined(_SCRAT_GLOBAL_METHODS_H_)
#define _SCRAT_GLOBAL_METHODS_H_

#ifdef SQMOD_PLUGIN_API
    #include <SqAPI.h>
#else
    #include <squirrelex.h>
#endif // SQMOD_PLUGIN_API

#include "sqratTypes.h"

namespace Sqrat {

/// @cond DEV

//
// Squirrel Global Functions
//
template <class R> struct SqGlobalProxy {
    template <class... A> static SQInteger Run(HSQUIRRELVM vm, SQInteger idx) {
        ArgFwd<A...>{}.Call(vm, idx, [](HSQUIRRELVM vm, A... a) {
            typedef R(*M)(A...);
            M* method;
            sq_getuserdata(vm, -1, reinterpret_cast<SQUserPointer*>(&method), nullptr);
            R ret = (*method)(a...);
            PushVar(vm, ret);
        });
        return 1;
    }
};

//
// reference return specialization
//

template <class R> struct SqGlobalProxy<R&> {
    template <class... A> static SQInteger Run(HSQUIRRELVM vm, SQInteger idx) {
        ArgFwd<A...>{}.Call(vm, idx, [](HSQUIRRELVM vm, A... a) {
            typedef R&(*M)(A...);
            M* method;
            sq_getuserdata(vm, -1, reinterpret_cast<SQUserPointer*>(&method), nullptr);
            R& ret = (*method)(a...);
            PushVarR(vm, ret);
        });
        return 1;
    }
};

//
// void return specialization
//

template <> struct SqGlobalProxy<void> {
    template <class... A> static SQInteger Run(HSQUIRRELVM vm, SQInteger idx) {
        ArgFwd<A...>{}.Call(vm, idx, [](HSQUIRRELVM vm, A... a) {
            typedef void(*M)(A...);
            M* method;
            sq_getuserdata(vm, -1, reinterpret_cast<SQUserPointer*>(&method), nullptr);
            (*method)(a...);
        });
        return 0;
    }
};

template<bool> struct SqGlobalParamCheck {
    static inline bool Invalid(SQInteger top, SQInteger count) {
        return top != count;
    }
};
template<> struct SqGlobalParamCheck<true> {
    static inline bool Invalid(SQInteger top, SQInteger count) {
        return top < (count - 1);
    }
};

template<bool> struct SqGlobalParamInspect {
    static inline bool Invalid(SQInteger need, SQInteger have) {
        return need != have;
    }
};
template<> struct SqGlobalParamInspect<true> {
    static inline bool Invalid(SQInteger need, SQInteger have) {
        return need < (have - 1);
    }
};

//
// Squirrel Global Functions
//
template <class R> struct SqGlobal {
    // Function proxy
    template <SQInteger startIdx, bool overloaded, class... A> static SQFUNCTION GetProxy() {
        return +[](HSQUIRRELVM vm) -> SQInteger {
#if !defined (SCRAT_NO_ERROR_CHECKING)
            if (!SQRAT_CONST_CONDITION(overloaded) &&
                SqGlobalParamCheck< ArgFwd<A...>::HASOPT >::Invalid(sq_gettop(vm), startIdx + sizeof...(A))) {
                return sq_throwerror(vm, _SC("wrong number of parameters"));
            }
#endif
            try {
                return SqGlobalProxy<R>::template Run<A...>(vm, startIdx);
            } catch (const Exception& e) {
                return sq_throwerror(vm, e.what());
            } catch (...) {
                return sq_throwerror(vm, _SC("unknown exception occured"));
            }
            SQ_UNREACHABLE
        };
    }
};

//
// reference return specialization
//

template <class R> struct SqGlobal<R&> {
    // Function proxy
    template <SQInteger startIdx, bool overloaded, class... A> static SQFUNCTION GetProxy() {
        return +[](HSQUIRRELVM vm) -> SQInteger {
#if !defined (SCRAT_NO_ERROR_CHECKING)
            if (!SQRAT_CONST_CONDITION(overloaded) &&
                SqGlobalParamCheck< ArgFwd<A...>::HASOPT >::Invalid(sq_gettop(vm), startIdx + sizeof...(A))) {
                return sq_throwerror(vm, _SC("wrong number of parameters"));
            }
#endif
            try {
                return SqGlobalProxy<R&>::template Run<A...>(vm, startIdx);
            } catch (const Exception& e) {
                return sq_throwerror(vm, e.what());
            } catch (...) {
                return sq_throwerror(vm, _SC("unknown exception occured"));
            }
            SQ_UNREACHABLE
        };
    }
};

//
// void return specialization
//

template <> struct SqGlobal<void> {
    // Function proxy
    template <SQInteger startIdx, bool overloaded, class... A> static SQFUNCTION GetProxy() {
        return +[](HSQUIRRELVM vm) -> SQInteger {
#if !defined (SCRAT_NO_ERROR_CHECKING)
            if (!SQRAT_CONST_CONDITION(overloaded) &&
                SqGlobalParamCheck< ArgFwd<A...>::HASOPT >::Invalid(sq_gettop(vm), startIdx + sizeof...(A))) {
                return sq_throwerror(vm, _SC("wrong number of parameters"));
            }
#endif
            try {
                return SqGlobalProxy<void>::Run<A...>(vm, startIdx);
            } catch (const Exception& e) {
                return sq_throwerror(vm, e.what());
            } catch (...) {
                return sq_throwerror(vm, _SC("unknown exception occured"));
            }
            SQ_UNREACHABLE
        };
    }
};

// Global Function Resolver
template <class R,class... A> SQFUNCTION SqGlobalFunc(R (* /*method*/)(A...)) {
    return SqGlobal<R>::template GetProxy<2, false, A...>();
}

// Global Function Resolver
template <class R,class... A> SQFUNCTION SqGlobalFunc(R& (* /*method*/)(A...)) {
    return SqGlobal<R&>::template GetProxy<2, false, A...>();
}

// Member Global Function Resolver
template <class R,class T,class... A> SQFUNCTION SqMemberGlobalFunc(R (* /*method*/)(T, A...)) {
    return SqGlobal<R>::template GetProxy<1, false, T, A...>();
}

// Member Global Function Resolver
template <class R,class T,class... A> SQFUNCTION SqMemberGlobalFunc(R& (* /*method*/)(T, A...)) {
    return SqGlobal<R&>::template GetProxy<1, false, T, A...>();
}

/// @endcond

}

#endif
