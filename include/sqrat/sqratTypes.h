//
// SqratTypes: Type Translators
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

#if !defined(_SCRAT_TYPES_H_)
#define _SCRAT_TYPES_H_

#ifdef SQUNICODE
#include <cstdlib>
#include <cstring>
#endif

#ifdef SQMOD_PLUGIN_API
    #include <SqAPI.h>
#else
    #include <squirrel.h>
#endif // SQMOD_PLUGIN_API

#include <string>

#include "sqratClassType.h"
#include "sqratUtil.h"

namespace Sqrat {

template <typename T, bool b>
struct popAsInt
{
    T value;
    popAsInt(HSQUIRRELVM vm, SQInteger idx)
    {
        SQObjectType value_type = sq_gettype(vm, idx);
        switch(value_type) {
        case OT_BOOL:
            SQBool sqValueb;
            sq_getbool(vm, idx, &sqValueb);
            value = static_cast<T>(sqValueb);
            break;
        case OT_INTEGER:
            SQInteger sqValue;
            sq_getinteger(vm, idx, &sqValue);
            value = static_cast<T>(sqValue);
            break;
        case OT_FLOAT:
            SQFloat sqValuef;
            sq_getfloat(vm, idx, &sqValuef);
			value = static_cast<T>(static_cast<int>(sqValuef));
            break;
        default:
            SQTHROW(vm, FormatTypeError(vm, idx, _SC("integer")));
            value = static_cast<T>(0);
            break;
        }
    }
};

template <typename T>
struct popAsInt<T, false>
{
    T value;  // cannot be initialized because unknown constructor parameters
    popAsInt(HSQUIRRELVM /*vm*/, SQInteger /*idx*/)
    {
        // keep the current error message already set previously, do not touch that here
    }
};

template <typename T>
struct popAsFloat
{
    T value;
    popAsFloat(HSQUIRRELVM vm, SQInteger idx)
    {
        SQObjectType value_type = sq_gettype(vm, idx);
        switch(value_type) {
        case OT_BOOL:
            SQBool sqValueb;
            sq_getbool(vm, idx, &sqValueb);
            value = static_cast<T>(sqValueb);
            break;
        case OT_INTEGER:
            SQInteger sqValue; \
            sq_getinteger(vm, idx, &sqValue);
            value = static_cast<T>(sqValue);
            break;
        case OT_FLOAT:
            SQFloat sqValuef;
            sq_getfloat(vm, idx, &sqValuef);
            value = static_cast<T>(sqValuef);
            break;
        default:
            SQTHROW(vm, FormatTypeError(vm, idx, _SC("float")));
            value = 0;
            break;
        }
    }
};

/// @endcond

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Used to get and push class instances to and from the stack as copies
///
/// \tparam T Type of instance (usually doesnt need to be defined explicitly)
///
/// \remarks
/// This specialization requires T to have a default constructor.
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
struct Var {

    T value; ///< The actual value of get operations

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Attempts to get the value off the stack at idx as the given type
    ///
    /// \param vm  Target VM
    /// \param idx Index trying to be read
    ///
    /// \remarks
    /// This function MUST have its Error handled if it occurred.
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Var(HSQUIRRELVM vm, SQInteger idx) {
        SQTRY()
        T* ptr = ClassType<T>::GetInstance(vm, idx);
        if (ptr != NULL) {
            value = *ptr;
#if !defined (SCRAT_NO_ERROR_CHECKING)
        } else if (std::is_convertible<T, SQInteger>::value) { /* value is likely of integral type like enums */
            SQCLEAR(vm); // clear the previous error
            value = popAsInt<T, std::is_convertible<T, SQInteger>::value>(vm, idx).value;
#endif
        } else {
            // initialize value to avoid warnings
            value = popAsInt<T, std::is_convertible<T, SQInteger>::value>(vm, idx).value;
        }
        SQCATCH(vm) {
#if defined (SCRAT_USE_EXCEPTIONS)
            SQUNUSED(e); // avoid "unreferenced local variable" warning
#endif
            if (std::is_convertible<T, SQInteger>::value) { /* value is likely of integral type like enums */
                value = popAsInt<T, std::is_convertible<T, SQInteger>::value>(vm, idx).value;
            } else {
                SQRETHROW(vm);
            }
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Called by Sqrat::PushVar to put a class object on the stack
    ///
    /// \param vm    Target VM
    /// \param value Value to push on to the VM's stack
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    static void push(HSQUIRRELVM vm, const T& value) {
        if (ClassType<T>::hasClassData(vm))
            ClassType<T>::PushInstanceCopy(vm, value);
        else /* try integral type */
            pushAsInt<T, std::is_convertible<T, SQInteger>::value>().push(vm, value);
    }

private:

    template <class T2, bool b>
    struct pushAsInt {
        void push(HSQUIRRELVM vm, const T2& /*value*/) {
            assert(false); // fails because called before a Sqrat::Class for T exists and T is not convertible to SQInteger
            sq_pushnull(vm);
        }
    };

    template <class T2>
    struct pushAsInt<T2, true> {
        void push(HSQUIRRELVM vm, const T2& value) {
            sq_pushinteger(vm, static_cast<SQInteger>(value));
        }
    };
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Used to get and push class instances to and from the stack as references
///
/// \tparam T Type of instance (usually doesnt need to be defined explicitly)
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
struct Var<T&> {

    T& value; ///< The actual value of get operations

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Attempts to get the value off the stack at idx as the given type
    ///
    /// \param vm  Target VM
    /// \param idx Index trying to be read
    ///
    /// \remarks
    /// This function MUST have its Error handled if it occurred.
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Var(HSQUIRRELVM vm, SQInteger idx) : value(*ClassType<T>::GetInstance(vm, idx)) {
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Called by Sqrat::PushVarR to put a class object on the stack
    ///
    /// \param vm    Target VM
    /// \param value Value to push on to the VM's stack
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    static void push(HSQUIRRELVM vm, T& value) {
        if (ClassType<T>::hasClassData(vm))
            ClassType<T>::PushInstance(vm, &value);
        else /* try integral type */
            pushAsInt<T, std::is_convertible<T, SQInteger>::value>().push(vm, value);
    }

private:

    template <class T2, bool b>
    struct pushAsInt {
        void push(HSQUIRRELVM vm, const T2& /*value*/) {
            assert(false); // fails because called before a Sqrat::Class for T exists and T is not convertible to SQInteger
            sq_pushnull(vm);
        }
    };

    template <class T2>
    struct pushAsInt<T2, true> {
        void push(HSQUIRRELVM vm, const T2& value) {
            sq_pushinteger(vm, static_cast<SQInteger>(value));
        }
    };
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Used to get and push class instances to and from the stack as pointers
///
/// \tparam T Type of instance (usually doesnt need to be defined explicitly)
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
struct Var<T*> {

    T* value; ///< The actual value of get operations

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Attempts to get the value off the stack at idx as the given type
    ///
    /// \param vm  Target VM
    /// \param idx Index trying to be read
    ///
    /// \remarks
    /// This function MUST have its Error handled if it occurred.
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Var(HSQUIRRELVM vm, SQInteger idx) : value(ClassType<T>::GetInstance(vm, idx, true)) {
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Called by Sqrat::PushVar to put a class object on the stack
    ///
    /// \param vm    Target VM
    /// \param value Value to push on to the VM's stack
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    static void push(HSQUIRRELVM vm, T* value) {
        ClassType<T>::PushInstance(vm, value);
    }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Used to get and push class instances to and from the stack as pointers to const data
///
/// \tparam T Type of instance (usually doesnt need to be defined explicitly)
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
struct Var<T* const> {

    T* const value; ///< The actual value of get operations

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Attempts to get the value off the stack at idx as the given type
    ///
    /// \param vm  Target VM
    /// \param idx Index trying to be read
    ///
    /// \remarks
    /// This function MUST have its Error handled if it occurred.
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Var(HSQUIRRELVM vm, SQInteger idx) : value(ClassType<T>::GetInstance(vm, idx, true)) {
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Called by Sqrat::PushVar to put a class object on the stack
    ///
    /// \param vm    Target VM
    /// \param value Value to push on to the VM's stack
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    static void push(HSQUIRRELVM vm, T* const value) {
        ClassType<T>::PushInstance(vm, value);
    }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Used to get and push class instances to and from the stack as const references
///
/// \tparam T Type of instance (usually doesnt need to be defined explicitly)
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
struct Var<const T&> {

    const T& value; ///< The actual value of get operations

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Attempts to get the value off the stack at idx as the given type
    ///
    /// \param vm  Target VM
    /// \param idx Index trying to be read
    ///
    /// \remarks
    /// This function MUST have its Error handled if it occurred.
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Var(HSQUIRRELVM vm, SQInteger idx) : value(*ClassType<T>::GetInstance(vm, idx)) {
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Called by Sqrat::PushVar to put a class object on the stack
    ///
    /// \param vm    Target VM
    /// \param value Value to push on to the VM's stack
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    static void push(HSQUIRRELVM vm, const T& value) {
        ClassType<T>::PushInstanceCopy(vm, value);
    }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Used to get and push class instances to and from the stack as const pointers
///
/// \tparam T Type of instance (usually doesnt need to be defined explicitly)
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
struct Var<const T*> {

    const T* value; ///< The actual value of get operations

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Attempts to get the value off the stack at idx as the given type
    ///
    /// \param vm  Target VM
    /// \param idx Index trying to be read
    ///
    /// \remarks
    /// This function MUST have its Error handled if it occurred.
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Var(HSQUIRRELVM vm, SQInteger idx) : value(ClassType<T>::GetInstance(vm, idx, true)) {
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Called by Sqrat::PushVar to put a class object on the stack
    ///
    /// \param vm    Target VM
    /// \param value Value to push on to the VM's stack
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    static void push(HSQUIRRELVM vm, const T* value) {
        ClassType<T>::PushInstance(vm, const_cast<T*>(value));
    }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Used to get and push class instances to and from the stack as const pointers to const data
///
/// \tparam T Type of instance (usually doesnt need to be defined explicitly)
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
struct Var<const T* const> {

    const T* const value; ///< The actual value of get operations

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Attempts to get the value off the stack at idx as the given type
    ///
    /// \param vm  Target VM
    /// \param idx Index trying to be read
    ///
    /// \remarks
    /// This function MUST have its Error handled if it occurred.
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Var(HSQUIRRELVM vm, SQInteger idx) : value(ClassType<T>::GetInstance(vm, idx, true)) {
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Called by Sqrat::PushVar to put a class object on the stack
    ///
    /// \param vm    Target VM
    /// \param value Value to push on to the VM's stack
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    static void push(HSQUIRRELVM vm, const T* const value) {
        ClassType<T>::PushInstance(vm, const_cast<T*>(value));
    }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Used to get (as copies) and push (as references) class instances to and from the stack as a SharedPtr
///
/// \tparam T Type of instance (usually doesnt need to be defined explicitly)
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> void PushVarR(HSQUIRRELVM vm, T& value);
template<class T>
struct Var<SharedPtr<T> > {

    SharedPtr<T> value; ///< The actual value of get operations

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Attempts to get the value off the stack at idx as the given type
    ///
    /// \param vm  Target VM
    /// \param idx Index trying to be read
    ///
    /// \remarks
    /// This function MUST have its Error handled if it occurred.
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Var(HSQUIRRELVM vm, SQInteger idx) {
        if (sq_gettype(vm, idx) != OT_NULL) {
            Var<T> instance(vm, idx);
            SQCATCH_NOEXCEPT(vm) {
                return;
            }
            value.Init(new T(instance.value));
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Called by Sqrat::PushVar to put a class object on the stack
    ///
    /// \param vm    Target VM
    /// \param value Value to push on to the VM's stack
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    static void push(HSQUIRRELVM vm, const SharedPtr<T>& value) {
        PushVarR(vm, *value);
    }
};

// Integer types
#define SCRAT_INTEGER( type ) \
 template<> \
 struct Var<type> { \
     type value; \
     Var(HSQUIRRELVM vm, SQInteger idx) { \
         value = popAsInt<type, true>(vm, idx).value; \
     } \
     static void push(HSQUIRRELVM vm, const type& value) { \
         sq_pushinteger(vm, static_cast<SQInteger>(value)); \
     } \
 };\
 \
 template<> \
 struct Var<const type&> { \
     type value; \
     Var(HSQUIRRELVM vm, SQInteger idx) { \
         value = popAsInt<type, true>(vm, idx).value; \
     } \
     static void push(HSQUIRRELVM vm, const type& value) { \
         sq_pushinteger(vm, static_cast<SQInteger>(value)); \
     } \
 };

SCRAT_INTEGER(unsigned int)
SCRAT_INTEGER(signed int)
SCRAT_INTEGER(unsigned long)
SCRAT_INTEGER(signed long)
SCRAT_INTEGER(unsigned short)
SCRAT_INTEGER(signed short)
SCRAT_INTEGER(unsigned char)
SCRAT_INTEGER(signed char)
SCRAT_INTEGER(unsigned long long)
SCRAT_INTEGER(signed long long)

#ifdef _MSC_VER
#if defined(__int64)
SCRAT_INTEGER(unsigned __int64)
SCRAT_INTEGER(signed __int64)
#endif
#endif

// Float types
#define SCRAT_FLOAT( type ) \
 template<> \
 struct Var<type> { \
     type value; \
     Var(HSQUIRRELVM vm, SQInteger idx) { \
         value = popAsFloat<type>(vm, idx).value; \
     } \
     static void push(HSQUIRRELVM vm, const type& value) { \
         sq_pushfloat(vm, static_cast<SQFloat>(value)); \
     } \
 }; \
 \
 template<> \
 struct Var<const type&> { \
     type value; \
     Var(HSQUIRRELVM vm, SQInteger idx) { \
         value = popAsFloat<type>(vm, idx).value; \
     } \
     static void push(HSQUIRRELVM vm, const type& value) { \
         sq_pushfloat(vm, static_cast<SQFloat>(value)); \
     } \
 };

SCRAT_FLOAT(float)
SCRAT_FLOAT(double)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Used to get and push bools to and from the stack
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<>
struct Var<bool> {

    bool value; ///< The actual value of get operations

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Attempts to get the value off the stack at idx as a bool
    ///
    /// \param vm  Target VM
    /// \param idx Index trying to be read
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Var(HSQUIRRELVM vm, SQInteger idx) {
        SQBool sqValue;
        sq_tobool(vm, idx, &sqValue);
        value = (sqValue != 0);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Called by Sqrat::PushVar to put a bool on the stack
    ///
    /// \param vm    Target VM
    /// \param value Value to push on to the VM's stack
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    static void push(HSQUIRRELVM vm, const bool& value) {
        sq_pushbool(vm, static_cast<SQBool>(value));
    }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Used to get and push const bool references to and from the stack
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<>
struct Var<const bool&> {

    bool value; ///< The actual value of get operations

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Attempts to get the value off the stack at idx as a bool
    ///
    /// \param vm  Target VM
    /// \param idx Index trying to be read
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Var(HSQUIRRELVM vm, SQInteger idx) {
        SQBool sqValue;
        sq_tobool(vm, idx, &sqValue);
        value = (sqValue != 0);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Called by Sqrat::PushVar to put a bool on the stack
    ///
    /// \param vm    Target VM
    /// \param value Value to push on to the VM's stack
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    static void push(HSQUIRRELVM vm, const bool& value) {
        sq_pushbool(vm, static_cast<SQBool>(value));
    }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Used to get and push SQChar arrays to and from the stack (usually is a char array)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<>
struct Var<SQChar*> {
private:

    HSQOBJECT obj; /* hold a reference to the object holding value during the Var struct lifetime*/
    HSQUIRRELVM v;

public:

    SQChar* value; ///< The actual value of get operations
    SQInteger size; ///< The size of the obtained string

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Attempts to get the value off the stack at idx as a character array
    ///
    /// \param vm  Target VM
    /// \param idx Index trying to be read
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Var(HSQUIRRELVM vm, SQInteger idx) {
        sq_tostring(vm, idx);
        sq_getstackobj(vm, -1, &obj);
        sq_getstringandsize(vm, -1, (const SQChar**)&value, &size);
        sq_addref(vm, &obj);
        sq_pop(vm,1);
        v = vm;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Destructor
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ~Var()
    {
        if(v && !sq_isnull(obj)) {
            sq_release(v, &obj);
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Called by Sqrat::PushVar to put a character array on the stack
    ///
    /// \param vm    Target VM
    /// \param value Value to push on to the VM's stack
    /// \param len   Length of the string (defaults to finding the length by searching for a terminating null-character)
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    static void push(HSQUIRRELVM vm, const SQChar* value, SQInteger len = -1) {
        sq_pushstring(vm, value, len);
    }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Used to get and push const SQChar arrays to and from the stack (usually is a const char array)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<>
struct Var<const SQChar*> {
private:

    HSQOBJECT obj; /* hold a reference to the object holding value during the Var struct lifetime*/
    HSQUIRRELVM v;

public:

    const SQChar* value; ///< The actual value of get operations
    SQInteger size; ///< The size of the obtained string

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Attempts to get the value off the stack at idx as a character array
    ///
    /// \param vm  Target VM
    /// \param idx Index trying to be read
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Var(HSQUIRRELVM vm, SQInteger idx) {
        sq_tostring(vm, idx);
        sq_getstackobj(vm, -1, &obj);
        sq_getstringandsize(vm, -1, &value, &size);
        sq_addref(vm, &obj);
        sq_pop(vm,1);
        v = vm;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Destructor
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ~Var()
    {
        if(v && !sq_isnull(obj)) {
            sq_release(v, &obj);
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Called by Sqrat::PushVar to put a character array on the stack
    ///
    /// \param vm    Target VM
    /// \param value Value to push on to the VM's stack
    /// \param len   Length of the string (defaults to finding the length by searching for a terminating null-character)
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    static void push(HSQUIRRELVM vm, const SQChar* value, SQInteger len = -1) {
        sq_pushstring(vm, value, len);
    }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Used to get and push strings to and from the stack (string is usually std::string)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<>
struct Var<string> {

    string value; ///< The actual value of get operations

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Attempts to get the value off the stack at idx as a string
    ///
    /// \param vm  Target VM
    /// \param idx Index trying to be read
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Var(HSQUIRRELVM vm, SQInteger idx) {
        const SQChar* ret;
        SQInteger len;
        sq_tostring(vm, idx);
        sq_getstringandsize(vm, -1, &ret, &len);
        value.assign(ret, len);
        sq_pop(vm,1);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Called by Sqrat::PushVar to put a string on the stack
    ///
    /// \param vm    Target VM
    /// \param value Value to push on to the VM's stack
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    static void push(HSQUIRRELVM vm, const string& value) {
        sq_pushstring(vm, value.c_str(), value.size());
    }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Used to get and push const string references to and from the stack as copies (strings are always copied)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<>
struct Var<const string&> {

    string value; ///< The actual value of get operations

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Attempts to get the value off the stack at idx as a string
    ///
    /// \param vm  Target VM
    /// \param idx Index trying to be read
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Var(HSQUIRRELVM vm, SQInteger idx) {
        const SQChar* ret;
        SQInteger len;
        sq_tostring(vm, idx);
        sq_getstringandsize(vm, -1, &ret, &len);
        value.assign(ret, len);
        sq_pop(vm,1);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Called by Sqrat::PushVar to put a string on the stack
    ///
    /// \param vm    Target VM
    /// \param value Value to push on to the VM's stack
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    static void push(HSQUIRRELVM vm, const string& value) {
        sq_pushstring(vm, value.c_str(), value.size());
    }
};

#ifdef SQUNICODE
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Used to get and push std::string to and from the stack when SQChar is not char (must define SQUNICODE)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<>
struct Var<std::string> {

    std::string value; ///< The actual value of get operations

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Attempts to get the value off the stack at idx as a string
    ///
    /// \param vm  Target VM
    /// \param idx Index trying to be read
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Var(HSQUIRRELVM vm, SQInteger idx) {
        const SQChar* ret;
        SQInteger len;
        sq_tostring(vm, idx);
        sq_getstringandsize(vm, -1, &ret, &len);
        value = wstring_to_string(string(ret, len));
        sq_pop(vm,1);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Called by Sqrat::PushVar to put a string on the stack
    ///
    /// \param vm    Target VM
    /// \param value Value to push on to the VM's stack
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    static void push(HSQUIRRELVM vm, const std::string& value) {
        std::wstring s = string_to_wstring(value);
        sq_pushstring(vm, s.c_str(), s.size());
    }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Used to get and push const std::string references to and from the stack when SQChar is not char (must define SQUNICODE)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<>
struct Var<const std::string&> {

    std::string value; ///< The actual value of get operations

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Attempts to get the value off the stack at idx as a string
    ///
    /// \param vm  Target VM
    /// \param idx Index trying to be read
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Var(HSQUIRRELVM vm, SQInteger idx) {
        const SQChar* ret;
        SQInteger len;
        sq_tostring(vm, idx);
        sq_getstringandsize(vm, -1, &ret, &len);
        value = wstring_to_string(string(ret, len));
        sq_pop(vm,1);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Called by Sqrat::PushVar to put a string on the stack
    ///
    /// \param vm    Target VM
    /// \param value Value to push on to the VM's stack
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    static void push(HSQUIRRELVM vm, const std::string& value) {
        std::wstring s = string_to_wstring(value);
        sq_pushstring(vm, s.c_str(), s.size());
    }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Used to get and push SQChar arrays to and from the stack when SQChar is not char (must define SQUNICODE)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<>
struct Var<char*> {
private:

    HSQOBJECT obj;/* hold a reference to the object holding value during the Var struct lifetime*/
    HSQUIRRELVM v;

public:

    char* value; ///< The actual value of get operations
    String::size_type size; ///< The size of the obtained string

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Attempts to get the value off the stack at idx as a character array
    ///
    /// \param vm  Target VM
    /// \param idx Index trying to be read
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Var(HSQUIRRELVM vm, SQInteger idx) {
        std::string holder;
        const SQChar *sv;
        SQInteger len;
        sq_tostring(vm, idx);
        sq_getstackobj(vm, -1, &obj);
        sq_getstringandsize(vm, -1, &sv, &len);
        sq_addref(vm, &obj);
        sq_pop(vm,1);
        v = vm;
        holder = wstring_to_string(string(sv, len));
        value = strdup(holder.c_str());
        size = holder.size();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Destructor
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ~Var()
    {
        if(v && !sq_isnull(obj)) {
            sq_release(v, &obj);
            free(value);
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Called by Sqrat::PushVar to put a character array on the stack
    ///
    /// \param vm    Target VM
    /// \param value Value to push on to the VM's stack
    /// \param len   Length of the string (defaults to finding the length by searching for a terminating null-character)
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    static void push(HSQUIRRELVM vm, const char* value, SQInteger len = -1) {
        sq_pushstring(vm, string_to_wstring(std::string(value)).c_str(), len);
    }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Used to get and push const SQChar arrays to and from the stack when SQChar is not char (must define SQUNICODE)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<>
struct Var<const char*> {
private:

    HSQOBJECT obj; /* hold a reference to the object holding value during the Var struct lifetime*/
    HSQUIRRELVM v;

public:

    char* value; ///< The actual value of get operations
    String::size_type size; ///< The size of the obtained string

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Attempts to get the value off the stack at idx as a character array
    ///
    /// \param vm  Target VM
    /// \param idx Index trying to be read
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Var(HSQUIRRELVM vm, SQInteger idx) {
        std::string holder;
        const SQChar *sv;
        SQInteger len;
        sq_tostring(vm, idx);
        sq_getstackobj(vm, -1, &obj);
        sq_getstringandsize(vm, -1, &sv, &len);
        sq_addref(vm, &obj);
        sq_pop(vm,1);
        v = vm;
        holder = wstring_to_string(string(sv, len));
        value = strdup(holder.c_str());
        size = holder.size();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Destructor
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ~Var()
    {
        if(v && !sq_isnull(obj)) {
            sq_release(v, &obj);
            free(value);
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Called by Sqrat::PushVar to put a character array on the stack
    ///
    /// \param vm    Target VM
    /// \param value Value to push on to the VM's stack
    /// \param len   Length of the string (defaults to finding the length by searching for a terminating null-character)
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    static void push(HSQUIRRELVM vm, const char* value, SQInteger len = -1) {
        sq_pushstring(vm, string_to_wstring(std::string(value)).c_str(), len);
    }
};
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Used to get and push StackStrF instances to and from the stack as references (StackStrF should not be a reference)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<>
struct Var<StackStrF> {

    StackStrF value; ///< The actual value of get operations

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Attempts to get the value off the stack at idx as an StackStrF
    ///
    /// \param vm  Target VM
    /// \param idx Index trying to be read
    ///
    /// \remarks
    /// This function MUST have its Error handled if it occurred.
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Var(HSQUIRRELVM vm, SQInteger idx) : value(vm, idx) {
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Called by Sqrat::PushVar to put an StackStrF on the stack
    ///
    /// \param vm    Target VM
    /// \param value Value to push on to the VM's stack
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    static void push(HSQUIRRELVM vm, const StackStrF& value) {
        if (sq_isstring(value.mObj)) {
            sq_pushobject(vm, value.mObj);
        } else {
            sq_pushstring(vm, value.mPtr, value.mLen);
        }
    }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Used to get and push StackStrF instances to and from the stack as references (StackStrF should not be a reference)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<>
struct Var<StackStrF&> : Var<StackStrF> {Var(HSQUIRRELVM vm, SQInteger idx) : Var<StackStrF>(vm, idx) {}};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Used to get and push StackStrF instances to and from the stack as references (StackStrF should not be a reference)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<>
struct Var<const StackStrF&> : Var<StackStrF> {Var(HSQUIRRELVM vm, SQInteger idx) : Var<StackStrF>(vm, idx) {}};

// Non-referencable type definitions
template<class T> struct is_referencable {static const bool value = true;};

#define SCRAT_MAKE_NONREFERENCABLE( type ) \
 template<> struct is_referencable<type> {static const bool value = false;};

SCRAT_MAKE_NONREFERENCABLE(unsigned int)
SCRAT_MAKE_NONREFERENCABLE(signed int)
SCRAT_MAKE_NONREFERENCABLE(unsigned long)
SCRAT_MAKE_NONREFERENCABLE(signed long)
SCRAT_MAKE_NONREFERENCABLE(unsigned short)
SCRAT_MAKE_NONREFERENCABLE(signed short)
SCRAT_MAKE_NONREFERENCABLE(unsigned char)
SCRAT_MAKE_NONREFERENCABLE(signed char)
SCRAT_MAKE_NONREFERENCABLE(unsigned long long)
SCRAT_MAKE_NONREFERENCABLE(signed long long)
SCRAT_MAKE_NONREFERENCABLE(float)
SCRAT_MAKE_NONREFERENCABLE(double)
SCRAT_MAKE_NONREFERENCABLE(bool)
SCRAT_MAKE_NONREFERENCABLE(string)

#ifdef _MSC_VER
#if defined(__int64)
SCRAT_MAKE_NONREFERENCABLE(unsigned __int64)
SCRAT_MAKE_NONREFERENCABLE(signed __int64)
#endif
#endif

#ifdef SQUNICODE
SCRAT_MAKE_NONREFERENCABLE(std::string)
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Pushes a value on to a given VM's stack
///
/// \param vm    VM that the variable will be pushed on to the stack of
/// \param value The actual value being pushed
///
/// \tparam T Type of value (usually doesnt need to be defined explicitly)
///
/// \remarks
/// What this function does is defined by Sqrat::Var template specializations,
/// and thus you can create custom functionality for it by making new template specializations.
/// When making a custom type that is not referencable, you must use SCRAT_MAKE_NONREFERENCABLE( type )
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
inline void PushVar(HSQUIRRELVM vm, T* value) {
    Var<T*>::push(vm, value);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Pushes a value on to a given VM's stack
///
/// \param vm    VM that the variable will be pushed on to the stack of
/// \param value The actual value being pushed
///
/// \tparam T Type of value (usually doesnt need to be defined explicitly)
///
/// \remarks
/// What this function does is defined by Sqrat::Var template specializations,
/// and thus you can create custom functionality for it by making new template specializations.
/// When making a custom type that is not referencable, you must use SCRAT_MAKE_NONREFERENCABLE( type )
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
inline void PushVar(HSQUIRRELVM vm, const T& value) {
    Var<T>::push(vm, value);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Pushes a pack of values on to a given VM's stack
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
inline void PushVars(HSQUIRRELVM vm, T value) {
    PushVar(vm, value);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Pushes a pack of values on to a given VM's stack
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T, class... Ts>
inline void PushVars(HSQUIRRELVM vm, T value, Ts &&... tail) {
    PushVar(vm, value);
    PushVars(vm, std::forward<Ts>(tail)...);
}

/// @cond DEV
template<class T, bool b>
struct PushVarR_helper {
    inline static void push(HSQUIRRELVM vm, T value) {
        PushVar<T>(vm, value);
    }
};
template<class T>
struct PushVarR_helper<T, false> {
    inline static void push(HSQUIRRELVM vm, const T& value) {
        PushVar<const T&>(vm, value);
    }
};
/// @endcond


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Pushes a reference on to a given VM's stack (some types cannot be referenced and will be copied instead)
///
/// \param vm    VM that the reference will be pushed on to the stack of
/// \param value The actual referenced value being pushed
///
/// \tparam T Type of value (usually doesnt need to be defined explicitly)
///
/// \remarks
/// What this function does is defined by Sqrat::Var template specializations,
/// and thus you can create custom functionality for it by making new template specializations.
/// When making a custom type that is not referencable, you must use SCRAT_MAKE_NONREFERENCABLE( type )
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
inline void PushVarR(HSQUIRRELVM vm, T& value) {
    if (!std::is_pointer<T>::value && is_referencable<typename std::remove_cv<T>::type>::value) {
        Var<T&>::push(vm, value);
    } else {
        PushVarR_helper<T, std::is_pointer<T>::value>::push(vm, value);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Helper used to process formatted arguments when necessary.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<class> struct ArgPopHasFmt { static constexpr bool value = false; };
template<> struct ArgPopHasFmt<StackStrF> { static constexpr bool value = true; };
template<> struct ArgPopHasFmt<const StackStrF> { static constexpr bool value = true; };
template<> struct ArgPopHasFmt<StackStrF&> { static constexpr bool value = true; };
template<> struct ArgPopHasFmt<const StackStrF&> { static constexpr bool value = true; };

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Helper used to process formatted arguments when necessary.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<bool> struct ArgPopFmt {
    static inline SQInteger Proc(StackStrF &, bool) {
        return SQ_OK;
    }
};
template<> struct ArgPopFmt<true> {
    static inline SQInteger Proc(StackStrF & s, bool dummy = false) {
        return s.Proc(true, dummy);
    }
    static inline SQInteger Get(StackStrF & s) {
        return s.mRes;
    }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Helper used to pop multiple variables from the stack and forward them to a functor.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<class...> struct ArgPop;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Special case for when nothing has to be popped.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<>
struct ArgPop<> {
    // Used to tell whether the last template parameter is a StackStrF type
    static constexpr bool HASFMT = false;
    // Base constructor. Does nothing.
    ArgPop(HSQUIRRELVM /*vm*/, SQInteger /*idx*/)
    { }
    // Process formatted arguments if necessary
    SQInteger Proc(bool dummy = false) { (void)(dummy); return SQ_OK; }
    SQInteger ProcRes() { return SQ_OK; }
    // Forward the arguments to a function object
    template<class F> void Call(HSQUIRRELVM vm, F f) {
        f(vm);
    }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Incremental specialization for when types are actually specified.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T1>
struct ArgPop<T1> {
    // Used to tell whether the last template parameter is a StackStrF type
    static constexpr bool HASFMT = ArgPopHasFmt<T1>::value;
    // Implement ours
    Var<T1> V1;
    // Base constructor. Can also pass extra parameters to the last popped argument.
    ArgPop(HSQUIRRELVM vm, SQInteger idx)
        : V1(vm, idx)
    { }
    // Retrieve the last popped variable
    Var<T1> & Last() { return V1; }
    const Var<T1> & Last() const { return V1; }
    // Process formatted arguments if necessary
    SQInteger Proc(bool dummy = false) { return ArgPopFmt<HASFMT>::Proc(V1.value, dummy); }
    SQInteger ProcRes() { return ArgPopFmt<HASFMT>::Get(V1.value); }
    // Forward the arguments to a function object
    template<class F> void Call(HSQUIRRELVM vm, F f) {
        f(vm,V1.value);
    }
};

template<class T1,class T2>
struct ArgPop<T1,T2> : public ArgPop<T1> {
    using Base = ArgPop<T1>;
    // Used to tell whether the last template parameter is a StackStrF type
    static constexpr bool HASFMT = ArgPopHasFmt<T2>::value;
    // Import from base classes
    using Base::V1;
    // Implement ours
    Var<T2> V2;
    // Base constructor. Can also pass extra parameters to the last popped argument.
    ArgPop(HSQUIRRELVM vm, SQInteger idx)
        : ArgPop<T1>(vm, idx)
        , V2(vm, idx+1)
    { }
    // Retrieve the last popped variable
    Var<T2> & Last() { return V2; }
    const Var<T2> & Last() const { return V2; }
    // Process formatted arguments if necessary
    SQInteger Proc(bool dummy = false) { return ArgPopFmt<HASFMT>::Proc(V2.value, dummy); }
    SQInteger ProcRes() { return ArgPopFmt<HASFMT>::Get(V2.value); }
    // Forward the arguments to a function object
    template<class F> void Call(HSQUIRRELVM vm, F f) {
        f(vm,V1.value,V2.value);
    }
};

template<class T1,class T2,class T3>
struct ArgPop<T1,T2,T3> : public ArgPop<T1,T2> {
    using Base = ArgPop<T1,T2>;
    // Used to tell whether the last template parameter is a StackStrF type
    static constexpr bool HASFMT = ArgPopHasFmt<T3>::value;
    // Import from base classes
    using Base::V1;
    using Base::V2;
    // Implement ours
    Var<T3> V3;
    // Base constructor. Can also pass extra parameters to the last popped argument.
    ArgPop(HSQUIRRELVM vm, SQInteger idx)
        : ArgPop<T1,T2>(vm, idx)
        , V3(vm, idx+2)
    { }
    // Retrieve the last popped variable
    Var<T3> & Last() { return V3; }
    const Var<T3> & Last() const { return V3; }
    // Process formatted arguments if necessary
    SQInteger Proc(bool dummy = false) { return ArgPopFmt<HASFMT>::Proc(V3.value, dummy); }
    SQInteger ProcRes() { return ArgPopFmt<HASFMT>::Get(V3.value); }
    // Forward the arguments to a function object
    template<class F> void Call(HSQUIRRELVM vm, F f) {
        f(vm,V1.value,V2.value,V3.value);
    }
};

template<class T1,class T2,class T3,class T4>
struct ArgPop<T1,T2,T3,T4> : public ArgPop<T1,T2,T3> {
    using Base = ArgPop<T1,T2,T3>;
    // Used to tell whether the last template parameter is a StackStrF type
    static constexpr bool HASFMT = ArgPopHasFmt<T4>::value;
    // Import from base classes
    using Base::V1;
    using Base::V2;
    using Base::V3;
    // Implement ours
    Var<T4> V4;
    // Base constructor. Can also pass extra parameters to the last popped argument.
    ArgPop(HSQUIRRELVM vm, SQInteger idx)
        : ArgPop<T1,T2,T3>(vm, idx)
        , V4(vm, idx+3)
    { }
    // Retrieve the last popped variable
    Var<T4> & Last() { return V4; }
    const Var<T4> & Last() const { return V4; }
    // Process formatted arguments if necessary
    SQInteger Proc(bool dummy = false) { return ArgPopFmt<HASFMT>::Proc(V4.value, dummy); }
    SQInteger ProcRes() { return ArgPopFmt<HASFMT>::Get(V4.value); }
    // Forward the arguments to a function object
    template<class F> void Call(HSQUIRRELVM vm, F f) {
        f(vm,V1.value,V2.value,V3.value,V4.value);
    }
};

template<class T1,class T2,class T3,class T4,class T5>
struct ArgPop<T1,T2,T3,T4,T5> : public ArgPop<T1,T2,T3,T4> {
    using Base = ArgPop<T1,T2,T3,T4>;
    // Used to tell whether the last template parameter is a StackStrF type
    static constexpr bool HASFMT = ArgPopHasFmt<T5>::value;
    // Import from base classes
    using Base::V1;
    using Base::V2;
    using Base::V3;
    using Base::V4;
    // Implement ours
    Var<T5> V5;
    // Base constructor. Can also pass extra parameters to the last popped argument.
    ArgPop(HSQUIRRELVM vm, SQInteger idx)
        : ArgPop<T1,T2,T3,T4>(vm, idx)
        , V5(vm, idx+4)
    { }
    // Retrieve the last popped variable
    Var<T5> & Last() { return V5; }
    const Var<T5> & Last() const { return V5; }
    // Process formatted arguments if necessary
    SQInteger Proc(bool dummy = false) { return ArgPopFmt<HASFMT>::Proc(V5.value, dummy); }
    SQInteger ProcRes() { return ArgPopFmt<HASFMT>::Get(V5.value); }
    // Forward the arguments to a function object
    template<class F> void Call(HSQUIRRELVM vm, F f) {
        f(vm,V1.value,V2.value,V3.value,V4.value,V5.value);
    }
};

template<class T1,class T2,class T3,class T4,class T5,class T6>
struct ArgPop<T1,T2,T3,T4,T5,T6> : public ArgPop<T1,T2,T3,T4,T5> {
    using Base = ArgPop<T1,T2,T3,T4,T5>;
    // Used to tell whether the last template parameter is a StackStrF type
    static constexpr bool HASFMT = ArgPopHasFmt<T6>::value;
    // Import from base classes
    using Base::V1;
    using Base::V2;
    using Base::V3;
    using Base::V4;
    using Base::V5;
    // Implement ours
    Var<T6> V6;
    // Base constructor. Can also pass extra parameters to the last popped argument.
    ArgPop(HSQUIRRELVM vm, SQInteger idx)
        : ArgPop<T1,T2,T3,T4,T5>(vm, idx)
        , V6(vm, idx+5)
    { }
    // Retrieve the last popped variable
    Var<T6> & Last() { return V6; }
    const Var<T6> & Last() const { return V6; }
    // Process formatted arguments if necessary
    SQInteger Proc(bool dummy = false) { return ArgPopFmt<HASFMT>::Proc(V6.value, dummy); }
    SQInteger ProcRes() { return ArgPopFmt<HASFMT>::Get(V6.value); }
    // Forward the arguments to a function object
    template<class F> void Call(HSQUIRRELVM vm, F f) {
        f(vm,V1.value,V2.value,V3.value,V4.value,V5.value,V6.value);
    }
};

template<class T1,class T2,class T3,class T4,class T5,class T6,class T7>
struct ArgPop<T1,T2,T3,T4,T5,T6,T7> : public ArgPop<T1,T2,T3,T4,T5,T6> {
    using Base = ArgPop<T1,T2,T3,T4,T5,T6>;
    // Used to tell whether the last template parameter is a StackStrF type
    static constexpr bool HASFMT = ArgPopHasFmt<T7>::value;
    // Import from base classes
    using Base::V1;
    using Base::V2;
    using Base::V3;
    using Base::V4;
    using Base::V5;
    using Base::V6;
    // Implement ours
    Var<T7> V7;
    // Base constructor. Can also pass extra parameters to the last popped argument.
    ArgPop(HSQUIRRELVM vm, SQInteger idx)
        : ArgPop<T1,T2,T3,T4,T5,T6>(vm, idx)
        , V7(vm, idx+6)
    { }
    // Retrieve the last popped variable
    Var<T7> & Last() { return V7; }
    const Var<T7> & Last() const { return V7; }
    // Process formatted arguments if necessary
    SQInteger Proc(bool dummy = false) { return ArgPopFmt<HASFMT>::Proc(V7.value, dummy); }
    SQInteger ProcRes() { return ArgPopFmt<HASFMT>::Get(V7.value); }
    // Forward the arguments to a function object
    template<class F> void Call(HSQUIRRELVM vm, F f) {
        f(vm,V1.value,V2.value,V3.value,V4.value,V5.value,V6.value,V7.value);
    }
};

template<class T1,class T2,class T3,class T4,class T5,class T6,class T7,class T8>
struct ArgPop<T1,T2,T3,T4,T5,T6,T7,T8> : public ArgPop<T1,T2,T3,T4,T5,T6,T7> {
    using Base = ArgPop<T1,T2,T3,T4,T5,T6,T7>;
    // Used to tell whether the last template parameter is a StackStrF type
    static constexpr bool HASFMT = ArgPopHasFmt<T8>::value;
    // Import from base classes
    using Base::V1;
    using Base::V2;
    using Base::V3;
    using Base::V4;
    using Base::V5;
    using Base::V6;
    using Base::V7;
    // Implement ours
    Var<T8> V8;
    // Base constructor. Can also pass extra parameters to the last popped argument.
    ArgPop(HSQUIRRELVM vm, SQInteger idx)
        : ArgPop<T1,T2,T3,T4,T5,T6,T7>(vm, idx)
        , V8(vm, idx+7)
    { }
    // Retrieve the last popped variable
    Var<T8> & Last() { return V8; }
    const Var<T8> & Last() const { return V8; }
    // Process formatted arguments if necessary
    SQInteger Proc(bool dummy = false) { return ArgPopFmt<HASFMT>::Proc(V8.value, dummy); }
    SQInteger ProcRes() { return ArgPopFmt<HASFMT>::Get(V8.value); }
    // Forward the arguments to a function object
    template<class F> void Call(HSQUIRRELVM vm, F f) {
        f(vm,V1.value,V2.value,V3.value,V4.value,V5.value,V6.value,V7.value,V8.value);
    }
};

template<class T1,class T2,class T3,class T4,class T5,class T6,class T7,class T8,class T9>
struct ArgPop<T1,T2,T3,T4,T5,T6,T7,T8,T9> : public ArgPop<T1,T2,T3,T4,T5,T6,T7,T8> {
    using Base = ArgPop<T1,T2,T3,T4,T5,T6,T7,T8>;
    // Used to tell whether the last template parameter is a StackStrF type
    static constexpr bool HASFMT = ArgPopHasFmt<T9>::value;
    // Import from base classes
    using Base::V1;
    using Base::V2;
    using Base::V3;
    using Base::V4;
    using Base::V5;
    using Base::V6;
    using Base::V7;
    using Base::V8;
    // Implement ours
    Var<T9> V9;
    // Base constructor. Can also pass extra parameters to the last popped argument.
    ArgPop(HSQUIRRELVM vm, SQInteger idx)
        : ArgPop<T1,T2,T3,T4,T5,T6,T7,T8>(vm, idx)
        , V9(vm, idx+8)
    { }
    // Retrieve the last popped variable
    Var<T9> & Last() { return V9; }
    const Var<T9> & Last() const { return V9; }
    // Process formatted arguments if necessary
    SQInteger Proc(bool dummy = false) { return ArgPopFmt<HASFMT>::Proc(V9.value, dummy); }
    SQInteger ProcRes() { return ArgPopFmt<HASFMT>::Get(V9.value); }
    // Forward the arguments to a function object
    template<class F> void Call(HSQUIRRELVM vm, F f) {
        f(vm,V1.value,V2.value,V3.value,V4.value,V5.value,V6.value,V7.value,V8.value,V9.value);
    }
};

template<class T1,class T2,class T3,class T4,class T5,class T6,class T7,class T8,class T9,class T10>
struct ArgPop<T1,T2,T3,T4,T5,T6,T7,T8,T9,T10> : public ArgPop<T1,T2,T3,T4,T5,T6,T7,T8,T9> {
    using Base = ArgPop<T1,T2,T3,T4,T5,T6,T7,T8,T9>;
    // Used to tell whether the last template parameter is a StackStrF type
    static constexpr bool HASFMT = ArgPopHasFmt<T10>::value;
    // Import from base classes
    using Base::V1;
    using Base::V2;
    using Base::V3;
    using Base::V4;
    using Base::V5;
    using Base::V6;
    using Base::V7;
    using Base::V8;
    using Base::V9;
    // Implement ours
    Var<T10> V10;
    // Base constructor. Can also pass extra parameters to the last popped argument.
    ArgPop(HSQUIRRELVM vm, SQInteger idx)
        : ArgPop<T1,T2,T3,T4,T5,T6,T7,T8,T9>(vm, idx)
        , V10(vm, idx+9)
    { }
    // Retrieve the last popped variable
    Var<T10> & Last() { return V10; }
    const Var<T10> & Last() const { return V10; }
    // Process formatted arguments if necessary
    SQInteger Proc(bool dummy = false) { return ArgPopFmt<HASFMT>::Proc(V10.value, dummy); }
    SQInteger ProcRes() { return ArgPopFmt<HASFMT>::Get(V10.value); }
    // Forward the arguments to a function object
    template<class F> void Call(HSQUIRRELVM vm, F f) {
        f(vm,V1.value,V2.value,V3.value,V4.value,V5.value,V6.value,V7.value,V8.value,V9.value,V10.value);
    }
};

template<class T1,class T2,class T3,class T4,class T5,class T6,class T7,class T8,class T9,class T10,class T11>
struct ArgPop<T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11> : public ArgPop<T1,T2,T3,T4,T5,T6,T7,T8,T9,T10> {
    using Base = ArgPop<T1,T2,T3,T4,T5,T6,T7,T8,T9,T10>;
    // Used to tell whether the last template parameter is a StackStrF type
    static constexpr bool HASFMT = ArgPopHasFmt<T11>::value;
    // Import from base classes
    using Base::V1;
    using Base::V2;
    using Base::V3;
    using Base::V4;
    using Base::V5;
    using Base::V6;
    using Base::V7;
    using Base::V8;
    using Base::V9;
    using Base::V10;
    // Implement ours
    Var<T11> V11;
    // Base constructor. Can also pass extra parameters to the last popped argument.
    ArgPop(HSQUIRRELVM vm, SQInteger idx)
        : ArgPop<T1,T2,T3,T4,T5,T6,T7,T8,T9,T10>(vm, idx)
        , V11(vm, idx+10)
    { }
    // Retrieve the last popped variable
    Var<T11> & Last() { return V11; }
    const Var<T11> & Last() const { return V11; }
    // Process formatted arguments if necessary
    SQInteger Proc(bool dummy = false) { return ArgPopFmt<HASFMT>::Proc(V11.value, dummy); }
    SQInteger ProcRes() { return ArgPopFmt<HASFMT>::Get(V11.value); }
    // Forward the arguments to a function object
    template<class F> void Call(HSQUIRRELVM vm, F f) {
        f(vm,V1.value,V2.value,V3.value,V4.value,V5.value,V6.value,V7.value,V8.value,V9.value,V10.value,V11.value);
    }
};

template<class T1,class T2,class T3,class T4,class T5,class T6,class T7,class T8,class T9,class T10,class T11,class T12>
struct ArgPop<T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12> : public ArgPop<T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11> {
    using Base = ArgPop<T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11>;
    // Used to tell whether the last template parameter is a StackStrF type
    static constexpr bool HASFMT = ArgPopHasFmt<T12>::value;
    // Import from base classes
    using Base::V1;
    using Base::V2;
    using Base::V3;
    using Base::V4;
    using Base::V5;
    using Base::V6;
    using Base::V7;
    using Base::V8;
    using Base::V9;
    using Base::V10;
    using Base::V11;
    // Implement ours
    Var<T12> V12;
    // Base constructor. Can also pass extra parameters to the last popped argument.
    ArgPop(HSQUIRRELVM vm, SQInteger idx)
        : ArgPop<T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11>(vm, idx)
        , V12(vm, idx+11)
    { }
    // Retrieve the last popped variable
    Var<T12> & Last() { return V12; }
    const Var<T12> & Last() const { return V12; }
    // Process formatted arguments if necessary
    SQInteger Proc(bool dummy = false) { return ArgPopFmt<HASFMT>::Proc(V12.value, dummy); }
    SQInteger ProcRes() { return ArgPopFmt<HASFMT>::Get(V12.value); }
    // Forward the arguments to a function object
    template<class F> void Call(HSQUIRRELVM vm, F f) {
        f(vm,V1.value,V2.value,V3.value,V4.value,V5.value,V6.value,V7.value,V8.value,V9.value,V10.value,V11.value,V12.value);
    }
};

template<class T1,class T2,class T3,class T4,class T5,class T6,class T7,class T8,class T9,class T10,class T11,class T12,class T13>
struct ArgPop<T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13> : public ArgPop<T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12> {
    using Base = ArgPop<T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12>;
    // Used to tell whether the last template parameter is a StackStrF type
    static constexpr bool HASFMT = ArgPopHasFmt<T13>::value;
    // Import from base classes
    using Base::V1;
    using Base::V2;
    using Base::V3;
    using Base::V4;
    using Base::V5;
    using Base::V6;
    using Base::V7;
    using Base::V8;
    using Base::V9;
    using Base::V10;
    using Base::V11;
    using Base::V12;
    // Implement ours
    Var<T13> V13;
    // Base constructor. Can also pass extra parameters to the last popped argument.
    ArgPop(HSQUIRRELVM vm, SQInteger idx)
        : ArgPop<T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12>(vm, idx)
        , V13(vm, idx+12)
    { }
    // Retrieve the last popped variable
    Var<T13> & Last() { return V13; }
    const Var<T13> & Last() const { return V13; }
    // Process formatted arguments if necessary
    SQInteger Proc(bool dummy = false) { return ArgPopFmt<HASFMT>::Proc(V13.value, dummy); }
    SQInteger ProcRes() { return ArgPopFmt<HASFMT>::Get(V13.value); }
    // Forward the arguments to a function object
    template<class F> void Call(HSQUIRRELVM vm, F f) {
        f(vm,V1.value,V2.value,V3.value,V4.value,V5.value,V6.value,V7.value,V8.value,V9.value,V10.value,V11.value,V12.value,V13.value);
    }
};

template<class T1,class T2,class T3,class T4,class T5,class T6,class T7,class T8,class T9,class T10,class T11,class T12,class T13,class T14>
struct ArgPop<T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14> : public ArgPop<T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13> {
    using Base = ArgPop<T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13>;
    // Used to tell whether the last template parameter is a StackStrF type
    static constexpr bool HASFMT = ArgPopHasFmt<T14>::value;
    // Import from base classes
    using Base::V1;
    using Base::V2;
    using Base::V3;
    using Base::V4;
    using Base::V5;
    using Base::V6;
    using Base::V7;
    using Base::V8;
    using Base::V9;
    using Base::V10;
    using Base::V11;
    using Base::V12;
    using Base::V13;
    // Implement ours
    Var<T14> V14;
    // Base constructor. Can also pass extra parameters to the last popped argument.
    ArgPop(HSQUIRRELVM vm, SQInteger idx)
        : ArgPop<T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13>(vm, idx)
        , V14(vm, idx+13)
    { }
    // Retrieve the last popped variable
    Var<T14> & Last() { return V14; }
    const Var<T14> & Last() const { return V14; }
    // Process formatted arguments if necessary
    SQInteger Proc(bool dummy = false) { return ArgPopFmt<HASFMT>::Proc(V14.value, dummy); }
    SQInteger ProcRes() { return ArgPopFmt<HASFMT>::Get(V14.value); }
    // Forward the arguments to a function object
    template<class F> void Call(HSQUIRRELVM vm, F f) {
        f(vm,V1.value,V2.value,V3.value,V4.value,V5.value,V6.value,V7.value,V8.value,V9.value,V10.value,V11.value,V12.value,V13.value,V14.value);
    }
};

}

#endif
