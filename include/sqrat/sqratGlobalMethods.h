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
			if (!SQRAT_CONST_CONDITION(overloaded) && sq_gettop(vm) != static_cast<SQInteger>(startIdx + sizeof...(A))) {
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
			if (!SQRAT_CONST_CONDITION(overloaded) && sq_gettop(vm) != static_cast<SQInteger>(startIdx + sizeof...(A))) {
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
			if (!SQRAT_CONST_CONDITION(overloaded) && sq_gettop(vm) != static_cast<SQInteger>(startIdx + sizeof...(A))) {
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
template <class R> struct SqGlobalFmt {
    // Function proxy
    template <SQInteger startIdx, bool overloaded, class... A> static SQFUNCTION GetProxy() noexcept {
        typedef R(*M)(A...);
        return +[](HSQUIRRELVM vm) noexcept -> SQInteger {
#if !defined (SCRAT_NO_ERROR_CHECKING)
            if (!SQRAT_CONST_CONDITION(overloaded) && sq_gettop(vm) < static_cast<SQInteger>(startIdx + sizeof...(A))) {
                return sq_throwerror(vm, _SC("wrong number of parameters"));
            }
#endif
            M* method;
            sq_getuserdata(vm, -1, reinterpret_cast<SQUserPointer*>(&method), NULL);

            SQTRY()
                ArgPop<A...> a(vm, startIdx, true);
                SQCATCH_NOEXCEPT(vm) {
                    return sq_throwerror(vm, SQWHAT_NOEXCEPT(vm));
                }
                // Validate the format
                if (SQ_FAILED(a.Last().value.mRes)) {
                    return a.Last().value.mRes;
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

template <class R> struct SqGlobalFmt<R&> {
    // Function proxy
    template <SQInteger startIdx, bool overloaded, class... A> static SQFUNCTION GetProxy() noexcept {
        typedef R&(*M)(A...);
        return +[](HSQUIRRELVM vm) noexcept -> SQInteger {
#if !defined (SCRAT_NO_ERROR_CHECKING)
            if (!SQRAT_CONST_CONDITION(overloaded) && sq_gettop(vm) < static_cast<SQInteger>(startIdx + sizeof...(A))) {
                return sq_throwerror(vm, _SC("wrong number of parameters"));
            }
#endif
            M* method;
            sq_getuserdata(vm, -1, reinterpret_cast<SQUserPointer*>(&method), NULL);

            SQTRY()
                ArgPop<A...> a(vm, startIdx, true);
                SQCATCH_NOEXCEPT(vm) {
                    return sq_throwerror(vm, SQWHAT_NOEXCEPT(vm));
                }
                // Validate the format
                if (SQ_FAILED(a.Last().value.mRes)) {
                    return a.Last().value.mRes;
                }
                R& ret = a.template Eval<R&,M>(*method);
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

template <> struct SqGlobalFmt<void> {
    // Function proxy
    template <SQInteger startIdx, bool overloaded, class... A> static SQFUNCTION GetProxy() noexcept {
        typedef void(*M)(A...);
        return +[](HSQUIRRELVM vm) noexcept -> SQInteger {
#if !defined (SCRAT_NO_ERROR_CHECKING)
            if (!SQRAT_CONST_CONDITION(overloaded) && sq_gettop(vm) < static_cast<SQInteger>(startIdx + sizeof...(A))) {
                return sq_throwerror(vm, _SC("wrong number of parameters"));
            }
#endif
            M* method;
            sq_getuserdata(vm, -1, reinterpret_cast<SQUserPointer*>(&method), NULL);

            SQTRY()
                ArgPop<A...> a(vm, startIdx, true);
                SQCATCH_NOEXCEPT(vm) {
                    return sq_throwerror(vm, SQWHAT_NOEXCEPT(vm));
                }
                // Validate the format
                if (SQ_FAILED(a.Last().value.mRes)) {
                    return a.Last().value.mRes;
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

// Formatted Global Function Resolver
template <class R,class... A> SQFUNCTION SqGlobalFmtFunc(R (* /*method*/)(A...)) noexcept {
    return SqGlobalFmt<R>::template GetProxy<2, false, A...>();
}

// Formatted Global Function Resolver
template <class R,class... A> SQFUNCTION SqGlobalFmtFunc(R& (* /*method*/)(A...)) noexcept {
    return SqGlobalFmt<R&>::template GetProxy<2, false, A...>();
}

// Formatted Member Global Function Resolver
template <class R,class T,class... A> SQFUNCTION SqMemberGlobalFmtFunc(R (* /*method*/)(T, A...)) noexcept {
    return SqGlobalFmt<R>::template GetProxy<1, false, T, A...>();
}

// Formatted Member Global Function Resolver
template <class R,class T,class... A> SQFUNCTION SqMemberGlobalFmtFunc(R& (* /*method*/)(T, A...)) noexcept {
    return SqGlobalFmt<R&>::template GetProxy<1, false, T, A...>();
}

/// @endcond

}

#endif
