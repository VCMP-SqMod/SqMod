//
// SqratMemberMethods: Member Methods
//

//
// Copyright (c) 2009 Brandon Jones
// Copyright 2011 Li-Cheng (Andy) Tai
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

#if !defined(_SCRAT_MEMBER_METHODS_H_)
#define _SCRAT_MEMBER_METHODS_H_

#ifdef SQMOD_PLUGIN_API
    #include <SqAPI.h>
#else
    #include <squirrelex.h>
#endif // SQMOD_PLUGIN_API

#include "sqratTypes.h"

namespace Sqrat {

/// @cond DEV

//
// Squirrel Member Functions
//
template <class C,class R> struct SqMemberProxy {
    template <class... A> static SQInteger Run(HSQUIRRELVM vm) {
        ArgFwd<A...>{}.Call(vm, 2, [](HSQUIRRELVM vm, A... a) {
            typedef R(C::*M)(A...);
            C* inst = Var<C*>(vm, 1).value;
            M* methodPtr;
            sq_getuserdata(vm, -1, reinterpret_cast<SQUserPointer*>(&methodPtr), nullptr);
            M method = *methodPtr;
            R ret = (inst->*method)(std::forward< A >(a)...);
            PushVar(vm, ret);
        });
        return 1;
    }
    template <class... A> static SQInteger RunC(HSQUIRRELVM vm) {
        ArgFwd<A...>{}.Call(vm, 2, [](HSQUIRRELVM vm, A... a) {
            typedef R(C::*M)(A...) const;
            C* inst = Var<C*>(vm, 1).value;
            M* methodPtr;
            sq_getuserdata(vm, -1, reinterpret_cast<SQUserPointer*>(&methodPtr), nullptr);
            M method = *methodPtr;
            R ret = (inst->*method)(std::forward< A >(a)...);
            PushVar(vm, ret);
        });
        return 1;
    }
};

//
// reference return specialization
//

template <class C, class R> struct SqMemberProxy<C,R&> {
    template <class... A> static SQInteger Run(HSQUIRRELVM vm) {
        ArgFwd<A...>{}.Call(vm, 2, [](HSQUIRRELVM vm, A... a) {
            typedef R&(C::*M)(A...);
            C* inst = Var<C*>(vm, 1).value;
            M* methodPtr;
            sq_getuserdata(vm, -1, reinterpret_cast<SQUserPointer*>(&methodPtr), nullptr);
            M method = *methodPtr;
            R& ret = (inst->*method)(std::forward< A >(a)...);
            PushVarR(vm, ret);
        });
        return 1;
    }
    template <class... A> static SQInteger RunC(HSQUIRRELVM vm) {
        ArgFwd<A...>{}.Call(vm, 2, [](HSQUIRRELVM vm, A... a) {
            typedef R&(C::*M)(A...) const;
            C* inst = Var<C*>(vm, 1).value;
            M* methodPtr;
            sq_getuserdata(vm, -1, reinterpret_cast<SQUserPointer*>(&methodPtr), nullptr);
            M method = *methodPtr;
            R& ret = (inst->*method)(std::forward< A >(a)...);
            PushVarR(vm, ret);
        });
        return 1;
    }
};

//
// void return specialization
//

template <class C> struct SqMemberProxy<C, void> {
    template <class... A> static SQInteger Run(HSQUIRRELVM vm) {
        ArgFwd<A...>{}.Call(vm, 2, [](HSQUIRRELVM vm, A... a) {
            typedef void(C::*M)(A...);
            C* inst = Var<C*>(vm, 1).value;
            M* methodPtr;
            sq_getuserdata(vm, -1, reinterpret_cast<SQUserPointer*>(&methodPtr), nullptr);
            M method = *methodPtr;
            (inst->*method)(std::forward< A >(a)...);
        });
        return 0;
    }
    template <class... A> static SQInteger RunC(HSQUIRRELVM vm) {
        ArgFwd<A...>{}.Call(vm, 2, [](HSQUIRRELVM vm, A... a) {
            typedef void(C::*M)(A...) const;
            C* inst = Var<C*>(vm, 1).value;
            M* methodPtr;
            sq_getuserdata(vm, -1, reinterpret_cast<SQUserPointer*>(&methodPtr), nullptr);
            M method = *methodPtr;
            (inst->*method)(std::forward< A >(a)...);
        });
        return 0;
    }
};

template<bool> struct SqMemberParamCheck {
    static inline bool Invalid(SQInteger top, SQInteger count) {
        return top != count;
    }
};
template<> struct SqMemberParamCheck<true> {
    static inline bool Invalid(SQInteger top, SQInteger count) {
        return top < count;
    }
};

//
// Squirrel Member Functions
//
template <class C,class R> struct SqMember {
    // Function proxy
    template <bool overloaded, class... A> static SQFUNCTION GetProxy() {
        return +[](HSQUIRRELVM vm) -> SQInteger {
#if !defined (SCRAT_NO_ERROR_CHECKING)
            if (!SQRAT_CONST_CONDITION(overloaded) &&
                SqGlobalParamCheck< ArgFwd<A...>::HASOPT >::Invalid(sq_gettop(vm), 2 + sizeof...(A))) {
                return sq_throwerror(vm, _SC("wrong number of parameters"));
            }
#endif
            try {
                return SqMemberProxy<C, R>:: template Run<A...>(vm);
            } catch (const Exception& e) {
                return sq_throwerror(vm, e.what());
            } catch (...) {
                return sq_throwerror(vm, _SC("unknown exception occured"));
            }
            SQ_UNREACHABLE
        };
    }
    // Function proxy
    template <bool overloaded, class... A> static SQFUNCTION GetProxyC() {
        return +[](HSQUIRRELVM vm) -> SQInteger {
#if !defined (SCRAT_NO_ERROR_CHECKING)
            if (!SQRAT_CONST_CONDITION(overloaded) &&
                SqGlobalParamCheck< ArgFwd<A...>::HASOPT >::Invalid(sq_gettop(vm), 2 + sizeof...(A))) {
                return sq_throwerror(vm, _SC("wrong number of parameters"));
            }
#endif
            try {
                return SqMemberProxy<C,R>::template RunC<A...>(vm);
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

template <class C, class R> struct SqMember<C,R&> {
    // Function proxy
    template <bool overloaded, class... A> static SQFUNCTION GetProxy() {
        return +[](HSQUIRRELVM vm) -> SQInteger {
#if !defined (SCRAT_NO_ERROR_CHECKING)
            if (!SQRAT_CONST_CONDITION(overloaded) &&
                SqGlobalParamCheck< ArgFwd<A...>::HASOPT >::Invalid(sq_gettop(vm), 2 + sizeof...(A))) {
                return sq_throwerror(vm, _SC("wrong number of parameters"));
            }
#endif
            try {
                return SqMemberProxy<C,R&>::template Run<A...>(vm);
            } catch (const Exception& e) {
                return sq_throwerror(vm, e.what());
            } catch (...) {
                return sq_throwerror(vm, _SC("unknown exception occured"));
            }
            SQ_UNREACHABLE
        };
    }
    // Function proxy
    template <bool overloaded, class... A> static SQFUNCTION GetProxyC() {
        return +[](HSQUIRRELVM vm) -> SQInteger {
#if !defined (SCRAT_NO_ERROR_CHECKING)
            if (!SQRAT_CONST_CONDITION(overloaded) &&
                SqGlobalParamCheck< ArgFwd<A...>::HASOPT >::Invalid(sq_gettop(vm), 2 + sizeof...(A))) {
                return sq_throwerror(vm, _SC("wrong number of parameters"));
            }
#endif
            try {
                return SqMemberProxy<C,R&>::template RunC<A...>(vm);
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

template <class C> struct SqMember<C, void> {
    // Function proxy
    template <bool overloaded, class... A> static SQFUNCTION GetProxy() {
        return +[](HSQUIRRELVM vm) -> SQInteger {
#if !defined (SCRAT_NO_ERROR_CHECKING)
            if (!SQRAT_CONST_CONDITION(overloaded) &&
                SqGlobalParamCheck< ArgFwd<A...>::HASOPT >::Invalid(sq_gettop(vm), 2 + sizeof...(A))) {
                return sq_throwerror(vm, _SC("wrong number of parameters"));
            }
#endif
            try {
                return SqMemberProxy<C, void>::template Run<A...>(vm);
            } catch (const Exception& e) {
                return sq_throwerror(vm, e.what());
            } catch (...) {
                return sq_throwerror(vm, _SC("unknown exception occured"));
            }
            SQ_UNREACHABLE
        };
    }
    // Function proxy
    template <bool overloaded, class... A> static SQFUNCTION GetProxyC() {
        return +[](HSQUIRRELVM vm) -> SQInteger {
#if !defined (SCRAT_NO_ERROR_CHECKING)
            if (!SQRAT_CONST_CONDITION(overloaded) &&
                SqGlobalParamCheck< ArgFwd<A...>::HASOPT >::Invalid(sq_gettop(vm), 2 + sizeof...(A))) {
                return sq_throwerror(vm, _SC("wrong number of parameters"));
            }
#endif
            try {
                return SqMemberProxy<C,void>::template RunC<A...>(vm);
            } catch (const Exception& e) {
                return sq_throwerror(vm, e.what());
            } catch (...) {
                return sq_throwerror(vm, _SC("unknown exception occured"));
            }
            SQ_UNREACHABLE
        };
    }
};

// Member Function Resolver
template <class C, class R,class... A> SQFUNCTION SqMemberFunc(R (C::* /*method*/)(A...)) {
    return SqMember<C,R>::template GetProxy<false, A...>();
}

// Member Function Resolver
template <class C, class R,class... A> SQFUNCTION SqMemberFunc(R (C::* /*method*/)(A...) const) {
    return SqMember<C,R>::template GetProxyC<false, A...>();
}

// Member Function Resolver
template <class C, class R,class... A> SQFUNCTION SqMemberFunc(R& (C::* /*method*/)(A...)) {
    return SqMember<C,R&>::template GetProxy<false, A...>();
}

// Member Function Resolver
template <class C, class R,class... A> SQFUNCTION SqMemberFunc(R& (C::* /*method*/)(A...) const) {
    return SqMember<C,R&>::template GetProxyC<false, A...>();
}

//
// Variable Get
//

template <class C, class V>
inline SQInteger sqDefaultGet(HSQUIRRELVM vm) {
    C* ptr;
    SQTRY()
    ptr = Var<C*>(vm, 1).value;
    SQCATCH_NOEXCEPT(vm) {
        SQCLEAR(vm); // clear the previous error
        return sq_throwerror(vm, SQWHAT_NOEXCEPT(vm));
    }
    SQCATCH(vm) {
        return sq_throwerror(vm, SQWHAT(vm));
    }

    typedef V C::*M;
    M* memberPtr = nullptr;
    sq_getuserdata(vm, -1, (SQUserPointer*)&memberPtr, nullptr); // Get Member...
    M member = *memberPtr;

    PushVarR(vm, ptr->*member);

    return 1;
}

template <class C, class V>
inline SQInteger sqStaticGet(HSQUIRRELVM vm) {
    typedef V *M;
    M* memberPtr = nullptr;
    sq_getuserdata(vm, -1, (SQUserPointer*)&memberPtr, nullptr); // Get Member...
    M member = *memberPtr;

    PushVarR(vm, *member);

    return 1;
}

inline SQInteger sqVarGet(HSQUIRRELVM vm) {
    // Find the get method in the get table
    sq_push(vm, 2);
#if !defined (SCRAT_NO_ERROR_CHECKING)
    if (SQ_FAILED(sq_rawget(vm, -2))) {
#if (SQUIRREL_VERSION_NUMBER>= 200) && (SQUIRREL_VERSION_NUMBER < 300) // Squirrel 2.x
        return sq_throwerror(vm, _SC("member variable not found"));
#else // Squirrel 3.x
        sq_pushnull(vm);
        return sq_throwobject(vm);
#endif
    }
#else
    sq_rawget(vm, -2);
#endif

    // push 'this'
    sq_push(vm, 1);

    // Call the getter
#if !defined (SCRAT_NO_ERROR_CHECKING)
    SQRESULT result = sq_call(vm, 1, true, ErrorHandling::IsEnabled());
    if (SQ_FAILED(result)) {
        return sq_throwerror(vm, LastErrorString(vm).c_str());
    }
#else
    sq_call(vm, 1, true, ErrorHandling::IsEnabled());
#endif

    return 1;
}


//
// Variable Set
//

template <class C, class V>
inline SQInteger sqDefaultSet(HSQUIRRELVM vm) {
    C* ptr;
    SQTRY()
    ptr = Var<C*>(vm, 1).value;
    SQCATCH_NOEXCEPT(vm) {
        SQCLEAR(vm); // clear the previous error
        return sq_throwerror(vm, SQWHAT_NOEXCEPT(vm));
    }
    SQCATCH(vm) {
        return sq_throwerror(vm, SQWHAT(vm));
    }

    typedef V C::*M;
    M* memberPtr = nullptr;
    sq_getuserdata(vm, -1, (SQUserPointer*)&memberPtr, nullptr); // Get Member...
    M member = *memberPtr;

    SQTRY()
    if (is_pointer<V>::value || is_reference<V>::value) {
        ptr->*member = Var<V>(vm, 2).value;
    } else {
        ptr->*member = Var<const V&>(vm, 2).value;
    }
    SQCATCH_NOEXCEPT(vm) {
        return sq_throwerror(vm, SQWHAT_NOEXCEPT(vm));
    }
    SQCATCH(vm) {
        return sq_throwerror(vm, SQWHAT(vm));
    }

    return 0;
}

template <class C, class V>
inline SQInteger sqStaticSet(HSQUIRRELVM vm) {
    typedef V *M;
    M* memberPtr = nullptr;
    sq_getuserdata(vm, -1, (SQUserPointer*)&memberPtr, nullptr); // Get Member...
    M member = *memberPtr;

    SQTRY()
    if (is_pointer<V>::value || is_reference<V>::value) {
        *member = Var<V>(vm, 2).value;
    } else {
        *member = Var<const V&>(vm, 2).value;
    }
    SQCATCH_NOEXCEPT(vm) {
        return sq_throwerror(vm, SQWHAT_NOEXCEPT(vm));
    }
    SQCATCH(vm) {
        return sq_throwerror(vm, SQWHAT(vm));
    }

    return 0;
}

inline SQInteger sqVarSet(HSQUIRRELVM vm) {
    // Find the set method in the set table
    sq_push(vm, 2);
#if !defined (SCRAT_NO_ERROR_CHECKING)
    if (SQ_FAILED(sq_rawget(vm, -2))) {
#if (SQUIRREL_VERSION_NUMBER>= 200) && (SQUIRREL_VERSION_NUMBER < 300) // Squirrel 2.x
        return sq_throwerror(vm, _SC("member variable not found"));
#else // Squirrel 3.x
        sq_pushnull(vm);
        return sq_throwobject(vm);
#endif
    }
#else
    sq_rawget(vm, -2);
#endif

    // push 'this'
    sq_push(vm, 1);
    sq_push(vm, 3);

    // Call the setter
#if !defined (SCRAT_NO_ERROR_CHECKING)
    SQRESULT result = sq_call(vm, 2, false, ErrorHandling::IsEnabled());
    if (SQ_FAILED(result)) {
        return sq_throwerror(vm, LastErrorString(vm).c_str());
    }
#else
    sq_call(vm, 2, false, ErrorHandling::IsEnabled());
#endif

    return 0;
}

/// @endcond

}

#endif
