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
    #include <squirrel.h>
#endif // SQMOD_PLUGIN_API

#include "sqratTypes.h"

namespace Sqrat {

/// @cond DEV

//
// Squirrel Global Functions
//
template <class R> struct SqGlobal {
    // Function proxy
    template <SQInteger startIdx, bool overloaded, class... A> static SQFUNCTION GetProxy() noexcept {
		typedef R(*M)(A...);
		return +[](HSQUIRRELVM vm) noexcept -> SQInteger {
#if !defined (SCRAT_NO_ERROR_CHECKING)
			if (!SQRAT_CONST_CONDITION(overloaded) && sq_gettop(vm) != startIdx + 2) {
				return sq_throwerror(vm, _SC("wrong number of parameters"));
			}
#endif
			M* method;
			sq_getuserdata(vm, -1, reinterpret_cast<SQUserPointer*>(&method), NULL);

			SQTRY()
				ArgPop<A...> a(vm, startIdx);
				SQCATCH_NOEXCEPT(vm) {
					return sq_throwerror(vm, SQWHAT_NOEXCEPT(vm));
				}
				R ret = a.template Eval<R,M>(*method);
				SQCATCH_NOEXCEPT(vm) {
					return sq_throwerror(vm, SQWHAT_NOEXCEPT(vm));
				}
				PushVar(vm, ret);
			SQCATCH(vm) {
				return sq_throwerror(vm, SQWHAT(vm));
			}

			return 1;
		};
    }
};

//
// reference return specialization
//

template <class R> struct SqGlobal<R&> {
    // Function proxy
    template <SQInteger startIdx, bool overloaded, class... A> static SQFUNCTION GetProxy() noexcept {
		typedef R&(*M)(A...);
		return +[](HSQUIRRELVM vm) noexcept -> SQInteger {
#if !defined (SCRAT_NO_ERROR_CHECKING)
			if (!SQRAT_CONST_CONDITION(overloaded) && sq_gettop(vm) != startIdx + 2) {
				return sq_throwerror(vm, _SC("wrong number of parameters"));
			}
#endif
			M* method;
			sq_getuserdata(vm, -1, reinterpret_cast<SQUserPointer*>(&method), NULL);

			SQTRY()
				ArgPop<A...> a(vm, startIdx);
				SQCATCH_NOEXCEPT(vm) {
					return sq_throwerror(vm, SQWHAT_NOEXCEPT(vm));
				}
				R& ret = a.template Eval<R&, M>(*method);
				SQCATCH_NOEXCEPT(vm) {
					return sq_throwerror(vm, SQWHAT_NOEXCEPT(vm));
				}
				PushVarR(vm, ret);
			SQCATCH(vm) {
				return sq_throwerror(vm, SQWHAT(vm));
			}

			return 1;
		};
    }
};

//
// void return specialization
//

template <> struct SqGlobal<void> {
    // Function proxy
    template <SQInteger startIdx, bool overloaded, class... A> static SQFUNCTION GetProxy() noexcept {
		typedef void(*M)(A...);
		return +[](HSQUIRRELVM vm) noexcept -> SQInteger {
#if !defined (SCRAT_NO_ERROR_CHECKING)
			if (!SQRAT_CONST_CONDITION(overloaded) && sq_gettop(vm) != startIdx + 2) {
				return sq_throwerror(vm, _SC("wrong number of parameters"));
			}
#endif
			M* method;
			sq_getuserdata(vm, -1, reinterpret_cast<SQUserPointer*>(&method), NULL);

			SQTRY()
				ArgPop<A...> a(vm, startIdx);
				SQCATCH_NOEXCEPT(vm) {
					return sq_throwerror(vm, SQWHAT_NOEXCEPT(vm));
				}
				a.template Exec<M>(*method);
				SQCATCH_NOEXCEPT(vm) {
					return sq_throwerror(vm, SQWHAT_NOEXCEPT(vm));
				}
			SQCATCH(vm) {
				return sq_throwerror(vm, SQWHAT(vm));
			}

			return 0;
		};
    }
};

// Global Function Resolver
template <class R,class... A> SQFUNCTION SqGlobalFunc(R (* /*method*/)(A...)) noexcept {
    return SqGlobal<R>::template GetProxy<2, false, A...>();
}

// Global Function Resolver
template <class R,class... A> SQFUNCTION SqGlobalFunc(R& (* /*method*/)(A...)) noexcept {
    return SqGlobal<R&>::template GetProxy<2, false, A...>();
}

// Member Global Function Resolver
template <class R,class T,class... A> SQFUNCTION SqMemberGlobalFunc(R (* /*method*/)(T, A...)) noexcept {
    return SqGlobal<R>::template GetProxy<1, false, T, A...>();
}

// Member Global Function Resolver
template <class R,class T,class... A> SQFUNCTION SqMemberGlobalFunc(R& (* /*method*/)(T, A...)) noexcept {
    return SqGlobal<R&>::template GetProxy<1, false, T, A...>();
}

//
// Squirrel Global Functions
//
template <class R>
class SqGlobalFmt {
public:

    // Arg Count 0
    template <bool overloaded /*= false*/>
    static SQInteger Func0(HSQUIRRELVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && sq_gettop(vm) < 2) {
            return sq_throwerror(vm, _SC("wrong number of parameters"));
        }
#endif

        typedef R (*M)(const StackStrF &);
        M* method;
        sq_getuserdata(vm, -1, (SQUserPointer*)&method, NULL);

        SQTRY()

        const StackStrF fmt(vm, 2, true, sq_gettop(vm) == 2);
        // Validate the format
        if (SQ_FAILED(fmt.mRes)) {
            return fmt.mRes;
        }

        R ret = (*method)(fmt);
        SQCATCH_NOEXCEPT(vm) {
            return sq_throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        PushVar(vm, ret);
        SQCATCH(vm) {
            return sq_throwerror(vm, SQWHAT(vm));
        }
        return 1;
    }

    // Arg Count 1
    template <class A1, SQInteger startIdx, bool overloaded /*= false*/>
    static SQInteger Func1(HSQUIRRELVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && sq_gettop(vm) < (startIdx + 1)) {
            return sq_throwerror(vm, _SC("wrong number of parameters"));
        }
#endif

        typedef R (*M)(A1, const StackStrF &);
        M* method;
        sq_getuserdata(vm, -1, (SQUserPointer*)&method, NULL);

        SQTRY()
        Var<A1> a1(vm, startIdx);
        SQCATCH_NOEXCEPT(vm) {
            return sq_throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }

        const StackStrF fmt(vm, startIdx + 1, true);
        // Validate the format
        if (SQ_FAILED(fmt.mRes)) {
            return fmt.mRes;
        }

        R ret = (*method)(
                    a1.value,
                    fmt
                );
        SQCATCH_NOEXCEPT(vm) {
            return sq_throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        PushVar(vm, ret);
        SQCATCH(vm) {
            return sq_throwerror(vm, SQWHAT(vm));
        }
        return 1;
    }

    // Arg Count 2
    template <class A1, class A2, SQInteger startIdx, bool overloaded /*= false*/>
    static SQInteger Func2(HSQUIRRELVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && sq_gettop(vm) < (startIdx + 2)) {
            return sq_throwerror(vm, _SC("wrong number of parameters"));
        }
#endif

        typedef R (*M)(A1, A2, const StackStrF &);
        M* method;
        sq_getuserdata(vm, -1, (SQUserPointer*)&method, NULL);

        SQTRY()
        Var<A1> a1(vm, startIdx);
        Var<A2> a2(vm, startIdx + 1);
        SQCATCH_NOEXCEPT(vm) {
            return sq_throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }

        const StackStrF fmt(vm, startIdx + 2, true);
        // Validate the format
        if (SQ_FAILED(fmt.mRes)) {
            return fmt.mRes;
        }

        R ret = (*method)(
                    a1.value,
                    a2.value,
                    fmt
                );
        SQCATCH_NOEXCEPT(vm) {
            return sq_throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        PushVar(vm, ret);
        SQCATCH(vm) {
            return sq_throwerror(vm, SQWHAT(vm));
        }
        return 1;
    }

    // Arg Count 3
    template <class A1, class A2, class A3, SQInteger startIdx, bool overloaded /*= false*/>
    static SQInteger Func3(HSQUIRRELVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && sq_gettop(vm) < (startIdx + 3)) {
            return sq_throwerror(vm, _SC("wrong number of parameters"));
        }
#endif

        typedef R (*M)(A1, A2, A3, const StackStrF &);
        M* method;
        sq_getuserdata(vm, -1, (SQUserPointer*)&method, NULL);

        SQTRY()
        Var<A1> a1(vm, startIdx);
        Var<A2> a2(vm, startIdx + 1);
        Var<A3> a3(vm, startIdx + 2);
        SQCATCH_NOEXCEPT(vm) {
            return sq_throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }

        const StackStrF fmt(vm, startIdx + 3, true);
        // Validate the format
        if (SQ_FAILED(fmt.mRes)) {
            return fmt.mRes;
        }

        R ret = (*method)(
                    a1.value,
                    a2.value,
                    a3.value,
                    fmt
                );
        SQCATCH_NOEXCEPT(vm) {
            return sq_throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        PushVar(vm, ret);
        SQCATCH(vm) {
            return sq_throwerror(vm, SQWHAT(vm));
        }
        return 1;
    }

    // Arg Count 4
    template <class A1, class A2, class A3, class A4, SQInteger startIdx, bool overloaded /*= false*/>
    static SQInteger Func4(HSQUIRRELVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && sq_gettop(vm) < (startIdx + 4)) {
            return sq_throwerror(vm, _SC("wrong number of parameters"));
        }
#endif

        typedef R (*M)(A1, A2, A3, A4, const StackStrF &);
        M* method;
        sq_getuserdata(vm, -1, (SQUserPointer*)&method, NULL);

        SQTRY()
        Var<A1> a1(vm, startIdx);
        Var<A2> a2(vm, startIdx + 1);
        Var<A3> a3(vm, startIdx + 2);
        Var<A4> a4(vm, startIdx + 3);
        SQCATCH_NOEXCEPT(vm) {
            return sq_throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }

        const StackStrF fmt(vm, startIdx + 4, true);
        // Validate the format
        if (SQ_FAILED(fmt.mRes)) {
            return fmt.mRes;
        }

        R ret = (*method)(
                    a1.value,
                    a2.value,
                    a3.value,
                    a4.value,
                    fmt
                );
        SQCATCH_NOEXCEPT(vm) {
            return sq_throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        PushVar(vm, ret);
        SQCATCH(vm) {
            return sq_throwerror(vm, SQWHAT(vm));
        }
        return 1;
    }

    // Arg Count 5
    template <class A1, class A2, class A3, class A4, class A5, SQInteger startIdx, bool overloaded /*= false*/>
    static SQInteger Func5(HSQUIRRELVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && sq_gettop(vm) < (startIdx + 5)) {
            return sq_throwerror(vm, _SC("wrong number of parameters"));
        }
#endif

        typedef R (*M)(A1, A2, A3, A4, A5, const StackStrF &);
        M* method;
        sq_getuserdata(vm, -1, (SQUserPointer*)&method, NULL);

        SQTRY()
        Var<A1> a1(vm, startIdx);
        Var<A2> a2(vm, startIdx + 1);
        Var<A3> a3(vm, startIdx + 2);
        Var<A4> a4(vm, startIdx + 3);
        Var<A5> a5(vm, startIdx + 4);
        SQCATCH_NOEXCEPT(vm) {
            return sq_throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }

        const StackStrF fmt(vm, startIdx + 5, true);
        // Validate the format
        if (SQ_FAILED(fmt.mRes)) {
            return fmt.mRes;
        }

        R ret = (*method)(
                    a1.value,
                    a2.value,
                    a3.value,
                    a4.value,
                    a5.value,
                    fmt
                );
        SQCATCH_NOEXCEPT(vm) {
            return sq_throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        PushVar(vm, ret);
        SQCATCH(vm) {
            return sq_throwerror(vm, SQWHAT(vm));
        }
        return 1;
    }

    // Arg Count 6
    template <class A1, class A2, class A3, class A4, class A5, class A6, SQInteger startIdx, bool overloaded /*= false*/>
    static SQInteger Func6(HSQUIRRELVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && sq_gettop(vm) < (startIdx + 6)) {
            return sq_throwerror(vm, _SC("wrong number of parameters"));
        }
#endif

        typedef R (*M)(A1, A2, A3, A4, A5, A6, const StackStrF &);
        M* method;
        sq_getuserdata(vm, -1, (SQUserPointer*)&method, NULL);

        SQTRY()
        Var<A1> a1(vm, startIdx);
        Var<A2> a2(vm, startIdx + 1);
        Var<A3> a3(vm, startIdx + 2);
        Var<A4> a4(vm, startIdx + 3);
        Var<A5> a5(vm, startIdx + 4);
        Var<A6> a6(vm, startIdx + 5);
        SQCATCH_NOEXCEPT(vm) {
            return sq_throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }

        const StackStrF fmt(vm, startIdx + 6, true);
        // Validate the format
        if (SQ_FAILED(fmt.mRes)) {
            return fmt.mRes;
        }

        R ret = (*method)(
                    a1.value,
                    a2.value,
                    a3.value,
                    a4.value,
                    a5.value,
                    a6.value,
                    fmt
                );
        SQCATCH_NOEXCEPT(vm) {
            return sq_throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        PushVar(vm, ret);
        SQCATCH(vm) {
            return sq_throwerror(vm, SQWHAT(vm));
        }
        return 1;
    }

    // Arg Count 7
    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, SQInteger startIdx, bool overloaded /*= false*/>
    static SQInteger Func7(HSQUIRRELVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && sq_gettop(vm) < (startIdx + 7)) {
            return sq_throwerror(vm, _SC("wrong number of parameters"));
        }
#endif

        typedef R (*M)(A1, A2, A3, A4, A5, A6, A7, const StackStrF &);
        M* method;
        sq_getuserdata(vm, -1, (SQUserPointer*)&method, NULL);

        SQTRY()
        Var<A1> a1(vm, startIdx);
        Var<A2> a2(vm, startIdx + 1);
        Var<A3> a3(vm, startIdx + 2);
        Var<A4> a4(vm, startIdx + 3);
        Var<A5> a5(vm, startIdx + 4);
        Var<A6> a6(vm, startIdx + 5);
        Var<A7> a7(vm, startIdx + 6);
        SQCATCH_NOEXCEPT(vm) {
            return sq_throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }

        const StackStrF fmt(vm, startIdx + 7, true);
        // Validate the format
        if (SQ_FAILED(fmt.mRes)) {
            return fmt.mRes;
        }

        R ret = (*method)(
                    a1.value,
                    a2.value,
                    a3.value,
                    a4.value,
                    a5.value,
                    a6.value,
                    a7.value,
                    fmt
                );
        SQCATCH_NOEXCEPT(vm) {
            return sq_throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        PushVar(vm, ret);
        SQCATCH(vm) {
            return sq_throwerror(vm, SQWHAT(vm));
        }
        return 1;
    }

    // Arg Count 8
    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, SQInteger startIdx, bool overloaded /*= false*/>
    static SQInteger Func8(HSQUIRRELVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && sq_gettop(vm) < (startIdx + 8)) {
            return sq_throwerror(vm, _SC("wrong number of parameters"));
        }
#endif

        typedef R (*M)(A1, A2, A3, A4, A5, A6, A7, A8, const StackStrF &);
        M* method;
        sq_getuserdata(vm, -1, (SQUserPointer*)&method, NULL);

        SQTRY()
        Var<A1> a1(vm, startIdx);
        Var<A2> a2(vm, startIdx + 1);
        Var<A3> a3(vm, startIdx + 2);
        Var<A4> a4(vm, startIdx + 3);
        Var<A5> a5(vm, startIdx + 4);
        Var<A6> a6(vm, startIdx + 5);
        Var<A7> a7(vm, startIdx + 6);
        Var<A8> a8(vm, startIdx + 7);
        SQCATCH_NOEXCEPT(vm) {
            return sq_throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }

        const StackStrF fmt(vm, startIdx + 8, true);
        // Validate the format
        if (SQ_FAILED(fmt.mRes)) {
            return fmt.mRes;
        }

        R ret = (*method)(
                    a1.value,
                    a2.value,
                    a3.value,
                    a4.value,
                    a5.value,
                    a6.value,
                    a7.value,
                    a8.value,
                    fmt
                );
        SQCATCH_NOEXCEPT(vm) {
            return sq_throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        PushVar(vm, ret);
        SQCATCH(vm) {
            return sq_throwerror(vm, SQWHAT(vm));
        }
        return 1;
    }
};


//
// reference return specialization
//

template <class R>
class SqGlobalFmt<R&> {
public:

    // Arg Count 0
    template <bool overloaded /*= false*/>
    static SQInteger Func0(HSQUIRRELVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && sq_gettop(vm) < 2) {
            return sq_throwerror(vm, _SC("wrong number of parameters"));
        }
#endif

        typedef R& (*M)(const StackStrF &);
        M* method;
        sq_getuserdata(vm, -1, (SQUserPointer*)&method, NULL);

        SQTRY()

        const StackStrF fmt(vm, 2, true, sq_gettop(vm) == 2);
        // Validate the format
        if (SQ_FAILED(fmt.mRes)) {
            return fmt.mRes;
        }

        R& ret = (*method)(fmt);
        SQCATCH_NOEXCEPT(vm) {
            return sq_throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        PushVarR(vm, ret);
        SQCATCH(vm) {
            return sq_throwerror(vm, SQWHAT(vm));
        }
        return 1;
    }

    // Arg Count 1
    template <class A1, SQInteger startIdx, bool overloaded /*= false*/>
    static SQInteger Func1(HSQUIRRELVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && sq_gettop(vm) < (startIdx + 1)) {
            return sq_throwerror(vm, _SC("wrong number of parameters"));
        }
#endif

        typedef R& (*M)(A1, const StackStrF &);
        M* method;
        sq_getuserdata(vm, -1, (SQUserPointer*)&method, NULL);

        SQTRY()
        Var<A1> a1(vm, startIdx);
        SQCATCH_NOEXCEPT(vm) {
            return sq_throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }

        const StackStrF fmt(vm, startIdx + 1, true);
        // Validate the format
        if (SQ_FAILED(fmt.mRes)) {
            return fmt.mRes;
        }

        R& ret = (*method)(
                    a1.value,
                    fmt
                );
        SQCATCH_NOEXCEPT(vm) {
            return sq_throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        PushVarR(vm, ret);
        SQCATCH(vm) {
            return sq_throwerror(vm, SQWHAT(vm));
        }
        return 1;
    }

    // Arg Count 2
    template <class A1, class A2, SQInteger startIdx, bool overloaded /*= false*/>
    static SQInteger Func2(HSQUIRRELVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && sq_gettop(vm) < (startIdx + 2)) {
            return sq_throwerror(vm, _SC("wrong number of parameters"));
        }
#endif

        typedef R& (*M)(A1, A2, const StackStrF &);
        M* method;
        sq_getuserdata(vm, -1, (SQUserPointer*)&method, NULL);

        SQTRY()
        Var<A1> a1(vm, startIdx);
        Var<A2> a2(vm, startIdx + 1);
        SQCATCH_NOEXCEPT(vm) {
            return sq_throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }

        const StackStrF fmt(vm, startIdx + 2, true);
        // Validate the format
        if (SQ_FAILED(fmt.mRes)) {
            return fmt.mRes;
        }

        R& ret = (*method)(
                    a1.value,
                    a2.value,
                    fmt
                );
        SQCATCH_NOEXCEPT(vm) {
            return sq_throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        PushVarR(vm, ret);
        SQCATCH(vm) {
            return sq_throwerror(vm, SQWHAT(vm));
        }
        return 1;
    }

    // Arg Count 3
    template <class A1, class A2, class A3, SQInteger startIdx, bool overloaded /*= false*/>
    static SQInteger Func3(HSQUIRRELVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && sq_gettop(vm) < (startIdx + 3)) {
            return sq_throwerror(vm, _SC("wrong number of parameters"));
        }
#endif

        typedef R& (*M)(A1, A2, A3, const StackStrF &);
        M* method;
        sq_getuserdata(vm, -1, (SQUserPointer*)&method, NULL);

        SQTRY()
        Var<A1> a1(vm, startIdx);
        Var<A2> a2(vm, startIdx + 1);
        Var<A3> a3(vm, startIdx + 2);
        SQCATCH_NOEXCEPT(vm) {
            return sq_throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }

        const StackStrF fmt(vm, startIdx + 3, true);
        // Validate the format
        if (SQ_FAILED(fmt.mRes)) {
            return fmt.mRes;
        }

        R& ret = (*method)(
                    a1.value,
                    a2.value,
                    a3.value,
                    fmt
                );
        SQCATCH_NOEXCEPT(vm) {
            return sq_throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        PushVarR(vm, ret);
        SQCATCH(vm) {
            return sq_throwerror(vm, SQWHAT(vm));
        }
        return 1;
    }

    // Arg Count 4
    template <class A1, class A2, class A3, class A4, SQInteger startIdx, bool overloaded /*= false*/>
    static SQInteger Func4(HSQUIRRELVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && sq_gettop(vm) < (startIdx + 4)) {
            return sq_throwerror(vm, _SC("wrong number of parameters"));
        }
#endif

        typedef R& (*M)(A1, A2, A3, A4, const StackStrF &);
        M* method;
        sq_getuserdata(vm, -1, (SQUserPointer*)&method, NULL);

        SQTRY()
        Var<A1> a1(vm, startIdx);
        Var<A2> a2(vm, startIdx + 1);
        Var<A3> a3(vm, startIdx + 2);
        Var<A4> a4(vm, startIdx + 3);
        SQCATCH_NOEXCEPT(vm) {
            return sq_throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }

        const StackStrF fmt(vm, startIdx + 4, true);
        // Validate the format
        if (SQ_FAILED(fmt.mRes)) {
            return fmt.mRes;
        }

        R& ret = (*method)(
                    a1.value,
                    a2.value,
                    a3.value,
                    a4.value,
                    fmt
                );
        SQCATCH_NOEXCEPT(vm) {
            return sq_throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        PushVarR(vm, ret);
        SQCATCH(vm) {
            return sq_throwerror(vm, SQWHAT(vm));
        }
        return 1;
    }

    // Arg Count 5
    template <class A1, class A2, class A3, class A4, class A5, SQInteger startIdx, bool overloaded /*= false*/>
    static SQInteger Func5(HSQUIRRELVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && sq_gettop(vm) < (startIdx + 5)) {
            return sq_throwerror(vm, _SC("wrong number of parameters"));
        }
#endif

        typedef R& (*M)(A1, A2, A3, A4, A5, const StackStrF &);
        M* method;
        sq_getuserdata(vm, -1, (SQUserPointer*)&method, NULL);

        SQTRY()
        Var<A1> a1(vm, startIdx);
        Var<A2> a2(vm, startIdx + 1);
        Var<A3> a3(vm, startIdx + 2);
        Var<A4> a4(vm, startIdx + 3);
        Var<A5> a5(vm, startIdx + 4);
        SQCATCH_NOEXCEPT(vm) {
            return sq_throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }

        const StackStrF fmt(vm, startIdx + 5, true);
        // Validate the format
        if (SQ_FAILED(fmt.mRes)) {
            return fmt.mRes;
        }

        R& ret = (*method)(
                    a1.value,
                    a2.value,
                    a3.value,
                    a4.value,
                    a5.value,
                    fmt
                );
        SQCATCH_NOEXCEPT(vm) {
            return sq_throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        PushVarR(vm, ret);
        SQCATCH(vm) {
            return sq_throwerror(vm, SQWHAT(vm));
        }
        return 1;
    }

    // Arg Count 6
    template <class A1, class A2, class A3, class A4, class A5, class A6, SQInteger startIdx, bool overloaded /*= false*/>
    static SQInteger Func6(HSQUIRRELVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && sq_gettop(vm) < (startIdx + 6)) {
            return sq_throwerror(vm, _SC("wrong number of parameters"));
        }
#endif

        typedef R& (*M)(A1, A2, A3, A4, A5, A6, const StackStrF &);
        M* method;
        sq_getuserdata(vm, -1, (SQUserPointer*)&method, NULL);

        SQTRY()
        Var<A1> a1(vm, startIdx);
        Var<A2> a2(vm, startIdx + 1);
        Var<A3> a3(vm, startIdx + 2);
        Var<A4> a4(vm, startIdx + 3);
        Var<A5> a5(vm, startIdx + 4);
        Var<A6> a6(vm, startIdx + 5);
        SQCATCH_NOEXCEPT(vm) {
            return sq_throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }

        const StackStrF fmt(vm, startIdx + 6, true);
        // Validate the format
        if (SQ_FAILED(fmt.mRes)) {
            return fmt.mRes;
        }

        R& ret = (*method)(
                    a1.value,
                    a2.value,
                    a3.value,
                    a4.value,
                    a5.value,
                    a6.value,
                    fmt
                );
        SQCATCH_NOEXCEPT(vm) {
            return sq_throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        PushVarR(vm, ret);
        SQCATCH(vm) {
            return sq_throwerror(vm, SQWHAT(vm));
        }
        return 1;
    }

    // Arg Count 7
    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, SQInteger startIdx, bool overloaded /*= false*/>
    static SQInteger Func7(HSQUIRRELVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && sq_gettop(vm) < (startIdx + 7)) {
            return sq_throwerror(vm, _SC("wrong number of parameters"));
        }
#endif

        typedef R& (*M)(A1, A2, A3, A4, A5, A6, A7, const StackStrF &);
        M* method;
        sq_getuserdata(vm, -1, (SQUserPointer*)&method, NULL);

        SQTRY()
        Var<A1> a1(vm, startIdx);
        Var<A2> a2(vm, startIdx + 1);
        Var<A3> a3(vm, startIdx + 2);
        Var<A4> a4(vm, startIdx + 3);
        Var<A5> a5(vm, startIdx + 4);
        Var<A6> a6(vm, startIdx + 5);
        Var<A7> a7(vm, startIdx + 6);
        SQCATCH_NOEXCEPT(vm) {
            return sq_throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }

        const StackStrF fmt(vm, startIdx + 7, true);
        // Validate the format
        if (SQ_FAILED(fmt.mRes)) {
            return fmt.mRes;
        }

        R& ret = (*method)(
                    a1.value,
                    a2.value,
                    a3.value,
                    a4.value,
                    a5.value,
                    a6.value,
                    a7.value,
                    fmt
                );
        SQCATCH_NOEXCEPT(vm) {
            return sq_throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        PushVarR(vm, ret);
        SQCATCH(vm) {
            return sq_throwerror(vm, SQWHAT(vm));
        }
        return 1;
    }

    // Arg Count 8
    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, SQInteger startIdx, bool overloaded /*= false*/>
    static SQInteger Func8(HSQUIRRELVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && sq_gettop(vm) < (startIdx + 8)) {
            return sq_throwerror(vm, _SC("wrong number of parameters"));
        }
#endif

        typedef R& (*M)(A1, A2, A3, A4, A5, A6, A7, A8, const StackStrF &);
        M* method;
        sq_getuserdata(vm, -1, (SQUserPointer*)&method, NULL);

        SQTRY()
        Var<A1> a1(vm, startIdx);
        Var<A2> a2(vm, startIdx + 1);
        Var<A3> a3(vm, startIdx + 2);
        Var<A4> a4(vm, startIdx + 3);
        Var<A5> a5(vm, startIdx + 4);
        Var<A6> a6(vm, startIdx + 5);
        Var<A7> a7(vm, startIdx + 6);
        Var<A8> a8(vm, startIdx + 7);
        SQCATCH_NOEXCEPT(vm) {
            return sq_throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }

        const StackStrF fmt(vm, startIdx + 8, true);
        // Validate the format
        if (SQ_FAILED(fmt.mRes)) {
            return fmt.mRes;
        }

        R& ret = (*method)(
                    a1.value,
                    a2.value,
                    a3.value,
                    a4.value,
                    a5.value,
                    a6.value,
                    a7.value,
                    a8.value,
                    fmt
                );
        SQCATCH_NOEXCEPT(vm) {
            return sq_throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        PushVarR(vm, ret);
        SQCATCH(vm) {
            return sq_throwerror(vm, SQWHAT(vm));
        }
        return 1;
    }
};


//
// void return specialization
//

template <>
class SqGlobalFmt<void> {
public:

    // Arg Count 0
    template <bool overloaded /*= false*/>
    static SQInteger Func0(HSQUIRRELVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && sq_gettop(vm) < 2) {
            return sq_throwerror(vm, _SC("wrong number of parameters"));
        }
#endif

        typedef void (*M)(const StackStrF &);
        M* method;
        sq_getuserdata(vm, -1, (SQUserPointer*)&method, NULL);

        SQTRY()

        const StackStrF fmt(vm, 2, true, sq_gettop(vm) == 2);
        // Validate the format
        if (SQ_FAILED(fmt.mRes)) {
            return fmt.mRes;
        }

        (*method)(fmt);
        SQCATCH_NOEXCEPT(vm) {
            return sq_throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            return sq_throwerror(vm, SQWHAT(vm));
        }
        return 0;
    }

    // Arg Count 1
    template <class A1, SQInteger startIdx, bool overloaded /*= false*/>
    static SQInteger Func1(HSQUIRRELVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && sq_gettop(vm) < (startIdx + 1)) {
            return sq_throwerror(vm, _SC("wrong number of parameters"));
        }
#endif

        typedef void (*M)(A1, const StackStrF &);
        M* method;
        sq_getuserdata(vm, -1, (SQUserPointer*)&method, NULL);

        SQTRY()
        Var<A1> a1(vm, startIdx);
        SQCATCH_NOEXCEPT(vm) {
            return sq_throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }

        const StackStrF fmt(vm, startIdx + 1, true);
        // Validate the format
        if (SQ_FAILED(fmt.mRes)) {
            return fmt.mRes;
        }

        (*method)(
            a1.value,
            fmt
        );
        SQCATCH_NOEXCEPT(vm) {
            return sq_throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            return sq_throwerror(vm, SQWHAT(vm));
        }
        return 0;
    }

    // Arg Count 2
    template <class A1, class A2, SQInteger startIdx, bool overloaded /*= false*/>
    static SQInteger Func2(HSQUIRRELVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && sq_gettop(vm) < (startIdx + 2)) {
            return sq_throwerror(vm, _SC("wrong number of parameters"));
        }
#endif

        typedef void (*M)(A1, A2, const StackStrF &);
        M* method;
        sq_getuserdata(vm, -1, (SQUserPointer*)&method, NULL);

        SQTRY()
        Var<A1> a1(vm, startIdx);
        Var<A2> a2(vm, startIdx + 1);
        SQCATCH_NOEXCEPT(vm) {
            return sq_throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }

        const StackStrF fmt(vm, startIdx + 2, true);
        // Validate the format
        if (SQ_FAILED(fmt.mRes)) {
            return fmt.mRes;
        }

        (*method)(
            a1.value,
            a2.value,
            fmt
        );
        SQCATCH_NOEXCEPT(vm) {
            return sq_throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            return sq_throwerror(vm, SQWHAT(vm));
        }
        return 0;
    }

    // Arg Count 3
    template <class A1, class A2, class A3, SQInteger startIdx, bool overloaded /*= false*/>
    static SQInteger Func3(HSQUIRRELVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && sq_gettop(vm) < (startIdx + 3)) {
            return sq_throwerror(vm, _SC("wrong number of parameters"));
        }
#endif

        typedef void (*M)(A1, A2, A3, const StackStrF &);
        M* method;
        sq_getuserdata(vm, -1, (SQUserPointer*)&method, NULL);

        SQTRY()
        Var<A1> a1(vm, startIdx);
        Var<A2> a2(vm, startIdx + 1);
        Var<A3> a3(vm, startIdx + 2);
        SQCATCH_NOEXCEPT(vm) {
            return sq_throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }

        const StackStrF fmt(vm, startIdx + 3, true);
        // Validate the format
        if (SQ_FAILED(fmt.mRes)) {
            return fmt.mRes;
        }

        (*method)(
            a1.value,
            a2.value,
            a3.value,
            fmt
        );
        SQCATCH_NOEXCEPT(vm) {
            return sq_throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            return sq_throwerror(vm, SQWHAT(vm));
        }
        return 0;
    }

    // Arg Count 4
    template <class A1, class A2, class A3, class A4, SQInteger startIdx, bool overloaded /*= false*/>
    static SQInteger Func4(HSQUIRRELVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && sq_gettop(vm) < (startIdx + 4)) {
            return sq_throwerror(vm, _SC("wrong number of parameters"));
        }
#endif

        typedef void (*M)(A1, A2, A3, A4, const StackStrF &);
        M* method;
        sq_getuserdata(vm, -1, (SQUserPointer*)&method, NULL);

        SQTRY()
        Var<A1> a1(vm, startIdx);
        Var<A2> a2(vm, startIdx + 1);
        Var<A3> a3(vm, startIdx + 2);
        Var<A4> a4(vm, startIdx + 3);
        SQCATCH_NOEXCEPT(vm) {
            return sq_throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }

        const StackStrF fmt(vm, startIdx + 4, true);
        // Validate the format
        if (SQ_FAILED(fmt.mRes)) {
            return fmt.mRes;
        }

        (*method)(
            a1.value,
            a2.value,
            a3.value,
            a4.value,
            fmt
        );
        SQCATCH_NOEXCEPT(vm) {
            return sq_throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            return sq_throwerror(vm, SQWHAT(vm));
        }
        return 0;
    }

    // Arg Count 5
    template <class A1, class A2, class A3, class A4, class A5, SQInteger startIdx, bool overloaded /*= false*/>
    static SQInteger Func5(HSQUIRRELVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && sq_gettop(vm) < (startIdx + 5)) {
            return sq_throwerror(vm, _SC("wrong number of parameters"));
        }
#endif

        typedef void (*M)(A1, A2, A3, A4, A5, const StackStrF &);
        M* method;
        sq_getuserdata(vm, -1, (SQUserPointer*)&method, NULL);

        SQTRY()
        Var<A1> a1(vm, startIdx);
        Var<A2> a2(vm, startIdx + 1);
        Var<A3> a3(vm, startIdx + 2);
        Var<A4> a4(vm, startIdx + 3);
        Var<A5> a5(vm, startIdx + 4);
        SQCATCH_NOEXCEPT(vm) {
            return sq_throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }

        const StackStrF fmt(vm, startIdx + 5, true);
        // Validate the format
        if (SQ_FAILED(fmt.mRes)) {
            return fmt.mRes;
        }

        (*method)(
            a1.value,
            a2.value,
            a3.value,
            a4.value,
            a5.value,
            fmt
        );
        SQCATCH_NOEXCEPT(vm) {
            return sq_throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            return sq_throwerror(vm, SQWHAT(vm));
        }
        return 0;
    }

    // Arg Count 6
    template <class A1, class A2, class A3, class A4, class A5, class A6, SQInteger startIdx, bool overloaded /*= false*/>
    static SQInteger Func6(HSQUIRRELVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && sq_gettop(vm) < (startIdx + 6)) {
            return sq_throwerror(vm, _SC("wrong number of parameters"));
        }
#endif

        typedef void (*M)(A1, A2, A3, A4, A5, A6, const StackStrF &);
        M* method;
        sq_getuserdata(vm, -1, (SQUserPointer*)&method, NULL);

        SQTRY()
        Var<A1> a1(vm, startIdx);
        Var<A2> a2(vm, startIdx + 1);
        Var<A3> a3(vm, startIdx + 2);
        Var<A4> a4(vm, startIdx + 3);
        Var<A5> a5(vm, startIdx + 4);
        Var<A6> a6(vm, startIdx + 5);
        SQCATCH_NOEXCEPT(vm) {
            return sq_throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }

        const StackStrF fmt(vm, startIdx + 6, true);
        // Validate the format
        if (SQ_FAILED(fmt.mRes)) {
            return fmt.mRes;
        }

        (*method)(
            a1.value,
            a2.value,
            a3.value,
            a4.value,
            a5.value,
            a6.value,
            fmt
        );
        SQCATCH_NOEXCEPT(vm) {
            return sq_throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            return sq_throwerror(vm, SQWHAT(vm));
        }
        return 0;
    }

    // Arg Count 7
    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, SQInteger startIdx, bool overloaded /*= false*/>
    static SQInteger Func7(HSQUIRRELVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && sq_gettop(vm) < (startIdx + 7)) {
            return sq_throwerror(vm, _SC("wrong number of parameters"));
        }
#endif

        typedef void (*M)(A1, A2, A3, A4, A5, A6, A7, const StackStrF &);
        M* method;
        sq_getuserdata(vm, -1, (SQUserPointer*)&method, NULL);

        SQTRY()
        Var<A1> a1(vm, startIdx);
        Var<A2> a2(vm, startIdx + 1);
        Var<A3> a3(vm, startIdx + 2);
        Var<A4> a4(vm, startIdx + 3);
        Var<A5> a5(vm, startIdx + 4);
        Var<A6> a6(vm, startIdx + 5);
        Var<A7> a7(vm, startIdx + 6);
        SQCATCH_NOEXCEPT(vm) {
            return sq_throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }

        const StackStrF fmt(vm, startIdx + 7, true);
        // Validate the format
        if (SQ_FAILED(fmt.mRes)) {
            return fmt.mRes;
        }

        (*method)(
            a1.value,
            a2.value,
            a3.value,
            a4.value,
            a5.value,
            a6.value,
            a7.value,
            fmt
        );
        SQCATCH_NOEXCEPT(vm) {
            return sq_throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            return sq_throwerror(vm, SQWHAT(vm));
        }
        return 0;
    }

    // Arg Count 8
    template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, SQInteger startIdx, bool overloaded /*= false*/>
    static SQInteger Func8(HSQUIRRELVM vm) {

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (!SQRAT_CONST_CONDITION(overloaded) && sq_gettop(vm) < (startIdx + 8)) {
            return sq_throwerror(vm, _SC("wrong number of parameters"));
        }
#endif

        typedef void (*M)(A1, A2, A3, A4, A5, A6, A7, A8, const StackStrF &);
        M* method;
        sq_getuserdata(vm, -1, (SQUserPointer*)&method, NULL);

        SQTRY()
        Var<A1> a1(vm, startIdx);
        Var<A2> a2(vm, startIdx + 1);
        Var<A3> a3(vm, startIdx + 2);
        Var<A4> a4(vm, startIdx + 3);
        Var<A5> a5(vm, startIdx + 4);
        Var<A6> a6(vm, startIdx + 5);
        Var<A7> a7(vm, startIdx + 6);
        Var<A8> a8(vm, startIdx + 7);
        SQCATCH_NOEXCEPT(vm) {
            return sq_throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }

        const StackStrF fmt(vm, startIdx + 8, true);
        // Validate the format
        if (SQ_FAILED(fmt.mRes)) {
            return fmt.mRes;
        }

        (*method)(
            a1.value,
            a2.value,
            a3.value,
            a4.value,
            a5.value,
            a6.value,
            a7.value,
            a8.value,
            fmt
        );
        SQCATCH_NOEXCEPT(vm) {
            return sq_throwerror(vm, SQWHAT_NOEXCEPT(vm));
        }
        SQCATCH(vm) {
            return sq_throwerror(vm, SQWHAT(vm));
        }
        return 0;
    }
};


//
// Formatted Global Function Resolvers
//

// Arg Count 0
template <class R>
SQFUNCTION SqGlobalFmtFunc(R (* /*method*/)(const StackStrF &)) {
    return &SqGlobalFmt<R>::template Func0<false>;
}

// Arg Count 0
template <class R>
SQFUNCTION SqGlobalFmtFunc(R& (* /*method*/)(const StackStrF &)) {
    return &SqGlobalFmt<R&>::template Func0<false>;
}

// Arg Count 1
template <class R, class A1>
SQFUNCTION SqGlobalFmtFunc(R (* /*method*/)(A1, const StackStrF &)) {
    return &SqGlobalFmt<R>::template Func1<A1, 2, false>;
}

// Arg Count 1
template <class R, class A1>
SQFUNCTION SqGlobalFmtFunc(R& (* /*method*/)(A1, const StackStrF &)) {
    return &SqGlobalFmt<R&>::template Func1<A1, 2, false>;
}

// Arg Count 2
template <class R, class A1, class A2>
SQFUNCTION SqGlobalFmtFunc(R (* /*method*/)(A1, A2, const StackStrF &)) {
    return &SqGlobalFmt<R>::template Func2<A1, A2, 2, false>;
}

// Arg Count 2
template <class R, class A1, class A2>
SQFUNCTION SqGlobalFmtFunc(R& (* /*method*/)(A1, A2, const StackStrF &)) {
    return &SqGlobalFmt<R&>::template Func2<A1, A2, 2, false>;
}

// Arg Count 3
template <class R, class A1, class A2, class A3>
SQFUNCTION SqGlobalFmtFunc(R (* /*method*/)(A1, A2, A3, const StackStrF &)) {
    return &SqGlobalFmt<R>::template Func3<A1, A2, A3, 2, false>;
}

// Arg Count 3
template <class R, class A1, class A2, class A3>
SQFUNCTION SqGlobalFmtFunc(R& (* /*method*/)(A1, A2, A3, const StackStrF &)) {
    return &SqGlobalFmt<R&>::template Func3<A1, A2, A3, 2, false>;
}

// Arg Count 4
template <class R, class A1, class A2, class A3, class A4>
SQFUNCTION SqGlobalFmtFunc(R (* /*method*/)(A1, A2, A3, A4, const StackStrF &)) {
    return &SqGlobalFmt<R>::template Func4<A1, A2, A3, A4, 2, false>;
}

// Arg Count 4
template <class R, class A1, class A2, class A3, class A4>
SQFUNCTION SqGlobalFmtFunc(R& (* /*method*/)(A1, A2, A3, A4, const StackStrF &)) {
    return &SqGlobalFmt<R&>::template Func4<A1, A2, A3, A4, 2, false>;
}

// Arg Count 5
template <class R, class A1, class A2, class A3, class A4, class A5>
SQFUNCTION SqGlobalFmtFunc(R (* /*method*/)(A1, A2, A3, A4, A5, const StackStrF &)) {
    return &SqGlobalFmt<R>::template Func5<A1, A2, A3, A4, A5, 2, false>;
}

// Arg Count 5
template <class R, class A1, class A2, class A3, class A4, class A5>
SQFUNCTION SqGlobalFmtFunc(R& (* /*method*/)(A1, A2, A3, A4, A5, const StackStrF &)) {
    return &SqGlobalFmt<R&>::template Func5<A1, A2, A3, A4, A5, 2, false>;
}

// Arg Count 6
template <class R, class A1, class A2, class A3, class A4, class A5, class A6>
SQFUNCTION SqGlobalFmtFunc(R (* /*method*/)(A1, A2, A3, A4, A5, A6, const StackStrF &)) {
    return &SqGlobalFmt<R>::template Func6<A1, A2, A3, A4, A5, A6, 2, false>;
}

// Arg Count 6
template <class R, class A1, class A2, class A3, class A4, class A5, class A6>
SQFUNCTION SqGlobalFmtFunc(R& (* /*method*/)(A1, A2, A3, A4, A5, A6, const StackStrF &)) {
    return &SqGlobalFmt<R&>::template Func6<A1, A2, A3, A4, A5, A6, 2, false>;
}

// Arg Count 7
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7>
SQFUNCTION SqGlobalFmtFunc(R (* /*method*/)(A1, A2, A3, A4, A5, A6, A7, const StackStrF &)) {
    return &SqGlobalFmt<R>::template Func7<A1, A2, A3, A4, A5, A6, A7, 2, false>;
}

// Arg Count 7
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7>
SQFUNCTION SqGlobalFmtFunc(R& (* /*method*/)(A1, A2, A3, A4, A5, A6, A7, const StackStrF &)) {
    return &SqGlobalFmt<R&>::template Func7<A1, A2, A3, A4, A5, A6, A7, 2, false>;
}

// Arg Count 8
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8>
SQFUNCTION SqGlobalFmtFunc(R (* /*method*/)(A1, A2, A3, A4, A5, A6, A7, A8, const StackStrF &)) {
    return &SqGlobalFmt<R>::template Func8<A1, A2, A3, A4, A5, A6, A7, A8, 2, false>;
}

// Arg Count 8
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8>
SQFUNCTION SqGlobalFmtFunc(R& (* /*method*/)(A1, A2, A3, A4, A5, A6, A7, A8, const StackStrF &)) {
    return &SqGlobalFmt<R&>::template Func8<A1, A2, A3, A4, A5, A6, A7, A8, 2, false>;
}

//
// Formatted Member Global Function Resolvers
//

// Arg Count 1
template <class R, class A1>
SQFUNCTION SqMemberGlobalFmtFunc(R (* /*method*/)(A1, const StackStrF &)) {
    return &SqGlobalFmt<R>::template Func1<A1, 1, false>;
}

// Arg Count 1
template <class R, class A1>
SQFUNCTION SqMemberGlobalFmtFunc(R& (* /*method*/)(A1, const StackStrF &)) {
    return &SqGlobalFmt<R&>::template Func1<A1, 1, false>;
}

// Arg Count 2
template <class R, class A1, class A2>
SQFUNCTION SqMemberGlobalFmtFunc(R (* /*method*/)(A1, A2, const StackStrF &)) {
    return &SqGlobalFmt<R>::template Func2<A1, A2, 1, false>;
}

// Arg Count 2
template <class R, class A1, class A2>
SQFUNCTION SqMemberGlobalFmtFunc(R& (* /*method*/)(A1, A2, const StackStrF &)) {
    return &SqGlobalFmt<R&>::template Func2<A1, A2, 1, false>;
}

// Arg Count 3
template <class R, class A1, class A2, class A3>
SQFUNCTION SqMemberGlobalFmtFunc(R (* /*method*/)(A1, A2, A3, const StackStrF &)) {
    return &SqGlobalFmt<R>::template Func3<A1, A2, A3, 1, false>;
}

// Arg Count 3
template <class R, class A1, class A2, class A3>
SQFUNCTION SqMemberGlobalFmtFunc(R& (* /*method*/)(A1, A2, A3, const StackStrF &)) {
    return &SqGlobalFmt<R&>::template Func3<A1, A2, A3, 1, false>;
}

// Arg Count 4
template <class R, class A1, class A2, class A3, class A4>
SQFUNCTION SqMemberGlobalFmtFunc(R (* /*method*/)(A1, A2, A3, A4, const StackStrF &)) {
    return &SqGlobalFmt<R>::template Func4<A1, A2, A3, A4, 1, false>;
}

// Arg Count 4
template <class R, class A1, class A2, class A3, class A4>
SQFUNCTION SqMemberGlobalFmtFunc(R& (* /*method*/)(A1, A2, A3, A4, const StackStrF &)) {
    return &SqGlobalFmt<R&>::template Func4<A1, A2, A3, A4, 1, false>;
}

// Arg Count 5
template <class R, class A1, class A2, class A3, class A4, class A5>
SQFUNCTION SqMemberGlobalFmtFunc(R (* /*method*/)(A1, A2, A3, A4, A5, const StackStrF &)) {
    return &SqGlobalFmt<R>::template Func5<A1, A2, A3, A4, A5, 1, false>;
}

// Arg Count 5
template <class R, class A1, class A2, class A3, class A4, class A5>
SQFUNCTION SqMemberGlobalFmtFunc(R& (* /*method*/)(A1, A2, A3, A4, A5, const StackStrF &)) {
    return &SqGlobalFmt<R&>::template Func5<A1, A2, A3, A4, A5, 1, false>;
}

// Arg Count 6
template <class R, class A1, class A2, class A3, class A4, class A5, class A6>
SQFUNCTION SqMemberGlobalFmtFunc(R (* /*method*/)(A1, A2, A3, A4, A5, A6, const StackStrF &)) {
    return &SqGlobalFmt<R>::template Func6<A1, A2, A3, A4, A5, A6, 1, false>;
}

// Arg Count 6
template <class R, class A1, class A2, class A3, class A4, class A5, class A6>
SQFUNCTION SqMemberGlobalFmtFunc(R& (* /*method*/)(A1, A2, A3, A4, A5, A6, const StackStrF &)) {
    return &SqGlobalFmt<R&>::template Func6<A1, A2, A3, A4, A5, A6, 1, false>;
}

// Arg Count 7
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7>
SQFUNCTION SqMemberGlobalFmtFunc(R (* /*method*/)(A1, A2, A3, A4, A5, A6, A7, const StackStrF &)) {
    return &SqGlobalFmt<R>::template Func7<A1, A2, A3, A4, A5, A6, A7, 1, false>;
}

// Arg Count 7
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7>
SQFUNCTION SqMemberGlobalFmtFunc(R& (* /*method*/)(A1, A2, A3, A4, A5, A6, A7, const StackStrF &)) {
    return &SqGlobalFmt<R&>::template Func7<A1, A2, A3, A4, A5, A6, A7, 1, false>;
}

// Arg Count 8
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8>
SQFUNCTION SqMemberGlobalFmtFunc(R (* /*method*/)(A1, A2, A3, A4, A5, A6, A7, A8, const StackStrF &)) {
    return &SqGlobalFmt<R>::template Func8<A1, A2, A3, A4, A5, A6, A7, A8, 1, false>;
}

// Arg Count 8
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8>
SQFUNCTION SqMemberGlobalFmtFunc(R& (* /*method*/)(A1, A2, A3, A4, A5, A6, A7, A8, const StackStrF &)) {
    return &SqGlobalFmt<R&>::template Func8<A1, A2, A3, A4, A5, A6, A7, A8, 1, false>;
}

/// @endcond

}

#endif
