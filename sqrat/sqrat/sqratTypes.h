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

#include <cstdlib>
#include <cstring>

#ifdef SQMOD_PLUGIN_API
    #include <SqAPI.h>
#else
    #include <squirrelex.h>
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
        if (SQ_FAILED(value.Proc())) {
            ErrorToException(vm);
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Attempts to get the value off the stack at idx as an StackStrF
    ///
    /// \param vm  Target VM
    /// \param idx Index trying to be read
    /// \param fmt Whether to mimmic a format function
    ///
    /// \remarks
    /// This function MUST have its Error handled if it occurred.
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Var(HSQUIRRELVM vm, SQInteger idx, bool fmt) : value(vm, idx) {
        if (SQ_FAILED(value.Proc(fmt))) {
            ErrorToException(vm);
        }
    }
    // Quick and dirty way to subtract the function pointer from the stack top as to not confuse formatted functions
    enum DropFuncPtrOverload { DropFuncPtr };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Attempts to get the value off the stack at idx as an StackStrF
    ///
    /// \param vm  Target VM
    /// \param idx Index trying to be read
    /// \param fmt Whether to mimmic a format function
    /// \param top Number of slots to ignore from the stack top
    ///
    /// \remarks
    /// This function MUST have its Error handled if it occurred.
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Var(HSQUIRRELVM vm, SQInteger idx, bool fmt, DropFuncPtrOverload /*fpsub*/) : value(vm, idx) {
        if (SQ_FAILED(value.Proc(fmt, sq_gettop(vm) - 1))) {
            ErrorToException(vm);
        }
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
struct Var<StackStrF&> : Var<StackStrF> {
    Var(HSQUIRRELVM vm, SQInteger idx) : Var<StackStrF>(vm, idx) {}
    Var(HSQUIRRELVM vm, SQInteger idx, bool fmt) : Var<StackStrF>(vm, idx, fmt) {}
    Var(HSQUIRRELVM vm, SQInteger idx, bool fmt, Var<StackStrF>::DropFuncPtrOverload /*fpsub*/)
        : Var<StackStrF>(vm, idx, fmt, Var<StackStrF>::DropFuncPtr) {}
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Used to get and push StackStrF instances to and from the stack as references (StackStrF should not be a reference)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<>
struct Var<const StackStrF&> : Var<StackStrF> {
    Var(HSQUIRRELVM vm, SQInteger idx) : Var<StackStrF>(vm, idx) {}
    Var(HSQUIRRELVM vm, SQInteger idx, bool fmt) : Var<StackStrF>(vm, idx, fmt) {}
    Var(HSQUIRRELVM vm, SQInteger idx, bool fmt, Var<StackStrF>::DropFuncPtrOverload /*fpsub*/)
        : Var<StackStrF>(vm, idx, fmt, Var<StackStrF>::DropFuncPtr) {}
};

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
/// Pushes a pack of values on to a given VM's stack. Special case for when there are no arguments to push.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline void PushVars(HSQUIRRELVM /*vm*/) { }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Pushes a pack of values on to a given VM's stack. Special case for when there is a single argument left to push.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
inline void PushVars(HSQUIRRELVM vm, T value) {
    PushVar(vm, value);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Pushes a pack of values on to a given VM's stack. Primary implementation which recursively pushes the parameters.
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

// Forward declaration
struct Function;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Helper used to identify optional arguments when necessary.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<class> struct ArgFwdHasOpt { static constexpr bool value = false; };

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Helper used to process formatted arguments when necessary.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<> struct ArgFwdHasOpt<StackStrF> { static constexpr bool value = true; };
template<> struct ArgFwdHasOpt<const StackStrF> { static constexpr bool value = true; };
template<> struct ArgFwdHasOpt<StackStrF&> { static constexpr bool value = true; };
template<> struct ArgFwdHasOpt<const StackStrF&> { static constexpr bool value = true; };

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Helper used to process callback arguments when necessary.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<> struct ArgFwdHasOpt<Function> { static constexpr bool value = true; };
template<> struct ArgFwdHasOpt<const Function> { static constexpr bool value = true; };
template<> struct ArgFwdHasOpt<Function&> { static constexpr bool value = true; };
template<> struct ArgFwdHasOpt<const Function&> { static constexpr bool value = true; };

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Helper used to process optional arguments when necessary.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T, bool> struct ArgFwdOptVar : public Var<T> {
    ArgFwdOptVar(HSQUIRRELVM vm, SQInteger idx) : Var<T>(vm, idx) {}
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Helper used to process formatted arguments when necessary.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<> struct ArgFwdOptVar<StackStrF, true> : public Var<StackStrF> {
    ArgFwdOptVar(HSQUIRRELVM vm, SQInteger idx) : Var<StackStrF>(vm, idx, true, Var<StackStrF>::DropFuncPtr) {}
};
template<> struct ArgFwdOptVar<StackStrF&, true> : public Var<StackStrF&> {
    ArgFwdOptVar(HSQUIRRELVM vm, SQInteger idx) : Var<StackStrF&>(vm, idx, true, Var<StackStrF>::DropFuncPtr) {}
};
template<> struct ArgFwdOptVar<const StackStrF&, true> : public Var<const StackStrF&> {
    ArgFwdOptVar(HSQUIRRELVM vm, SQInteger idx) : Var<const StackStrF&>(vm, idx, true, Var<StackStrF>::DropFuncPtr) {}
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Helper used to get the return type of a given functor used to forward arguments.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <class F> using ArgFwdRet = typename std::result_of<F>::type;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Helper used to pop multiple variables from the stack and forward them to a functor.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<class...> struct ArgFwd;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Special case for when nothing has to be popped.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<>
struct ArgFwd<> {
    // Used to tell whether the last template parameter is a StackStrF type
    static constexpr bool HASOPT = false;
    // Forward the arguments to a function object
    template<class F> inline auto Call(HSQUIRRELVM vm, SQInteger /*idx*/, F f) {
        return f(vm);
    }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Incremental specialization for when types are actually specified.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T1>
struct ArgFwd<T1> {
    // Used to tell whether the last template parameter is a StackStrF type
    static constexpr bool HASOPT = ArgFwdHasOpt<T1>::value;
    // Forward the arguments to a function object
    template<class F> inline auto Call(HSQUIRRELVM vm, SQInteger idx, F f) {
        ArgFwdOptVar<T1, HASOPT> a1(vm, idx);
        return f(vm,a1.value);
    }
};

template<class T1,class T2>
struct ArgFwd<T1,T2> {
    // Used to tell whether the last template parameter is a StackStrF type
    static constexpr bool HASOPT = ArgFwdHasOpt<T2>::value;
    // Forward the arguments to a function object
    template<class F> inline auto Call(HSQUIRRELVM vm, SQInteger idx, F f) {
        Var<T1> a1(vm, idx++);
        ArgFwdOptVar<T2, HASOPT> a2(vm, idx);
        return f(vm,a1.value,a2.value);
    }
};

template<class T1,class T2,class T3>
struct ArgFwd<T1,T2,T3> {
    // Used to tell whether the last template parameter is a StackStrF type
    static constexpr bool HASOPT = ArgFwdHasOpt<T3>::value;
    // Forward the arguments to a function object
    template<class F> inline auto Call(HSQUIRRELVM vm, SQInteger idx, F f) {
        Var<T1> a1(vm, idx++);
        Var<T2> a2(vm, idx++);
        ArgFwdOptVar<T3, HASOPT> a3(vm, idx);
        return f(vm,a1.value,a2.value,a3.value);
    }
};


template<class T1,class T2,class T3,class T4>
struct ArgFwd<T1,T2,T3,T4> {
    // Used to tell whether the last template parameter is a StackStrF type
    static constexpr bool HASOPT = ArgFwdHasOpt<T4>::value;
    // Forward the arguments to a function object
    template<class F> inline auto Call(HSQUIRRELVM vm, SQInteger idx, F f) {
        Var<T1> a1(vm, idx++);
        Var<T2> a2(vm, idx++);
        Var<T3> a3(vm, idx++);
        ArgFwdOptVar<T4, HASOPT> a4(vm, idx);
        return f(vm,a1.value,a2.value,a3.value,a4.value);
    }
};

template<class T1,class T2,class T3,class T4,class T5>
struct ArgFwd<T1,T2,T3,T4,T5> {
    // Used to tell whether the last template parameter is a StackStrF type
    static constexpr bool HASOPT = ArgFwdHasOpt<T5>::value;
    // Forward the arguments to a function object
    template<class F> inline auto Call(HSQUIRRELVM vm, SQInteger idx, F f) {
        Var<T1> a1(vm, idx++);
        Var<T2> a2(vm, idx++);
        Var<T3> a3(vm, idx++);
        Var<T4> a4(vm, idx++);
        ArgFwdOptVar<T5, HASOPT> a5(vm, idx);
        return f(vm,a1.value,a2.value,a3.value,a4.value,a5.value);
    }
};

template<class T1,class T2,class T3,class T4,class T5,class T6>
struct ArgFwd<T1,T2,T3,T4,T5,T6> {
    // Used to tell whether the last template parameter is a StackStrF type
    static constexpr bool HASOPT = ArgFwdHasOpt<T6>::value;
    // Forward the arguments to a function object
    template<class F> inline auto Call(HSQUIRRELVM vm, SQInteger idx, F f) {
        Var<T1> a1(vm, idx++);
        Var<T2> a2(vm, idx++);
        Var<T3> a3(vm, idx++);
        Var<T4> a4(vm, idx++);
        Var<T5> a5(vm, idx++);
        ArgFwdOptVar<T6, HASOPT> a6(vm, idx);
        return f(vm,a1.value,a2.value,a3.value,a4.value,a5.value,a6.value);
    }
};

template<class T1,class T2,class T3,class T4,class T5,class T6,class T7>
struct ArgFwd<T1,T2,T3,T4,T5,T6,T7> {
    // Used to tell whether the last template parameter is a StackStrF type
    static constexpr bool HASOPT = ArgFwdHasOpt<T7>::value;
    // Forward the arguments to a function object
    template<class F> inline auto Call(HSQUIRRELVM vm, SQInteger idx, F f) {
        Var<T1> a1(vm, idx++);
        Var<T2> a2(vm, idx++);
        Var<T3> a3(vm, idx++);
        Var<T4> a4(vm, idx++);
        Var<T5> a5(vm, idx++);
        Var<T6> a6(vm, idx++);
        ArgFwdOptVar<T7, HASOPT> a7(vm, idx);
        return f(vm,a1.value,a2.value,a3.value,a4.value,a5.value,a6.value,a7.value);
    }
};

template<class T1,class T2,class T3,class T4,class T5,class T6,class T7,class T8>
struct ArgFwd<T1,T2,T3,T4,T5,T6,T7,T8> {
    // Used to tell whether the last template parameter is a StackStrF type
    static constexpr bool HASOPT = ArgFwdHasOpt<T8>::value;
    // Forward the arguments to a function object
    template<class F> inline auto Call(HSQUIRRELVM vm, SQInteger idx, F f) {
        Var<T1> a1(vm, idx++);
        Var<T2> a2(vm, idx++);
        Var<T3> a3(vm, idx++);
        Var<T4> a4(vm, idx++);
        Var<T5> a5(vm, idx++);
        Var<T6> a6(vm, idx++);
        Var<T7> a7(vm, idx++);
        ArgFwdOptVar<T8, HASOPT> a8(vm, idx);
        return f(vm,a1.value,a2.value,a3.value,a4.value,a5.value,a6.value,a7.value,a8.value);
    }
};

template<class T1,class T2,class T3,class T4,class T5,class T6,class T7,class T8,class T9>
struct ArgFwd<T1,T2,T3,T4,T5,T6,T7,T8,T9> {
    // Used to tell whether the last template parameter is a StackStrF type
    static constexpr bool HASOPT = ArgFwdHasOpt<T9>::value;
    // Forward the arguments to a function object
    template<class F> inline auto Call(HSQUIRRELVM vm, SQInteger idx, F f) {
        Var<T1> a1(vm, idx++);
        Var<T2> a2(vm, idx++);
        Var<T3> a3(vm, idx++);
        Var<T4> a4(vm, idx++);
        Var<T5> a5(vm, idx++);
        Var<T6> a6(vm, idx++);
        Var<T7> a7(vm, idx++);
        Var<T8> a8(vm, idx++);
        ArgFwdOptVar<T9, HASOPT> a9(vm, idx);
        return f(vm,a1.value,a2.value,a3.value,a4.value,a5.value,a6.value,a7.value,a8.value,a9.value);
    }
};

template<class T1,class T2,class T3,class T4,class T5,class T6,class T7,class T8,class T9,class T10>
struct ArgFwd<T1,T2,T3,T4,T5,T6,T7,T8,T9,T10> {
    // Used to tell whether the last template parameter is a StackStrF type
    static constexpr bool HASOPT = ArgFwdHasOpt<T10>::value;
    // Forward the arguments to a function object
    template<class F> inline auto Call(HSQUIRRELVM vm, SQInteger idx, F f) {
        Var<T1> a1(vm, idx++);
        Var<T2> a2(vm, idx++);
        Var<T3> a3(vm, idx++);
        Var<T4> a4(vm, idx++);
        Var<T5> a5(vm, idx++);
        Var<T6> a6(vm, idx++);
        Var<T7> a7(vm, idx++);
        Var<T8> a8(vm, idx++);
        Var<T9> a9(vm, idx++);
        ArgFwdOptVar<T10, HASOPT> a10(vm, idx);
        return f(vm,a1.value,a2.value,a3.value,a4.value,a5.value,a6.value,a7.value,a8.value,a9.value,a10.value);
    }
};

template<class T1,class T2,class T3,class T4,class T5,class T6,class T7,class T8,class T9,class T10,class T11>
struct ArgFwd<T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11> {
    // Used to tell whether the last template parameter is a StackStrF type
    static constexpr bool HASOPT = ArgFwdHasOpt<T11>::value;
    // Forward the arguments to a function object
    template<class F> inline auto Call(HSQUIRRELVM vm, SQInteger idx, F f) {
        Var<T1> a1(vm, idx++);
        Var<T2> a2(vm, idx++);
        Var<T3> a3(vm, idx++);
        Var<T4> a4(vm, idx++);
        Var<T5> a5(vm, idx++);
        Var<T6> a6(vm, idx++);
        Var<T7> a7(vm, idx++);
        Var<T8> a8(vm, idx++);
        Var<T9> a9(vm, idx++);
        Var<T10> a10(vm, idx++);
        ArgFwdOptVar<T11, HASOPT> a11(vm, idx);
        return f(vm,a1.value,a2.value,a3.value,a4.value,a5.value,a6.value,a7.value,a8.value,a9.value,a10.value,a11.value);
    }
};

template<class T1,class T2,class T3,class T4,class T5,class T6,class T7,class T8,class T9,class T10,class T11,class T12>
struct ArgFwd<T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12> {
    // Used to tell whether the last template parameter is a StackStrF type
    static constexpr bool HASOPT = ArgFwdHasOpt<T12>::value;
    // Forward the arguments to a function object
    template<class F> inline auto Call(HSQUIRRELVM vm, SQInteger idx, F f) {
        Var<T1> a1(vm, idx++);
        Var<T2> a2(vm, idx++);
        Var<T3> a3(vm, idx++);
        Var<T4> a4(vm, idx++);
        Var<T5> a5(vm, idx++);
        Var<T6> a6(vm, idx++);
        Var<T7> a7(vm, idx++);
        Var<T8> a8(vm, idx++);
        Var<T9> a9(vm, idx++);
        Var<T10> a10(vm, idx++);
        Var<T11> a11(vm, idx++);
        ArgFwdOptVar<T12, HASOPT> a12(vm, idx);
        return f(vm,a1.value,a2.value,a3.value,a4.value,a5.value,a6.value,a7.value,a8.value,a9.value,a10.value,a11.value,a12.value);
    }
};

template<class T1,class T2,class T3,class T4,class T5,class T6,class T7,class T8,class T9,class T10,class T11,class T12,class T13>
struct ArgFwd<T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13> {
    // Used to tell whether the last template parameter is a StackStrF type
    static constexpr bool HASOPT = ArgFwdHasOpt<T13>::value;
    // Forward the arguments to a function object
    template<class F> inline auto Call(HSQUIRRELVM vm, SQInteger idx, F f) {
        Var<T1> a1(vm, idx++);
        Var<T2> a2(vm, idx++);
        Var<T3> a3(vm, idx++);
        Var<T4> a4(vm, idx++);
        Var<T5> a5(vm, idx++);
        Var<T6> a6(vm, idx++);
        Var<T7> a7(vm, idx++);
        Var<T8> a8(vm, idx++);
        Var<T9> a9(vm, idx++);
        Var<T10> a10(vm, idx++);
        Var<T11> a11(vm, idx++);
        Var<T12> a12(vm, idx++);
        ArgFwdOptVar<T13, HASOPT> a13(vm, idx);
        return f(vm,a1.value,a2.value,a3.value,a4.value,a5.value,a6.value,a7.value,a8.value,a9.value,a10.value,a11.value,a12.value,a13.value);
    }
};

template<class T1,class T2,class T3,class T4,class T5,class T6,class T7,class T8,class T9,class T10,class T11,class T12,class T13,class T14>
struct ArgFwd<T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14> {
    // Used to tell whether the last template parameter is a StackStrF type
    static constexpr bool HASOPT = ArgFwdHasOpt<T14>::value;
    // Forward the arguments to a function object
    template<class F> inline auto Call(HSQUIRRELVM vm, SQInteger idx, F f) {
        Var<T1> a1(vm, idx++);
        Var<T2> a2(vm, idx++);
        Var<T3> a3(vm, idx++);
        Var<T4> a4(vm, idx++);
        Var<T5> a5(vm, idx++);
        Var<T6> a6(vm, idx++);
        Var<T7> a7(vm, idx++);
        Var<T8> a8(vm, idx++);
        Var<T9> a9(vm, idx++);
        Var<T10> a10(vm, idx++);
        Var<T11> a11(vm, idx++);
        Var<T12> a12(vm, idx++);
        Var<T13> a13(vm, idx++);
        ArgFwdOptVar<T14, HASOPT> a14(vm, idx);
        return f(vm,a1.value,a2.value,a3.value,a4.value,a5.value,a6.value,a7.value,a8.value,a9.value,a10.value,a11.value,a12.value,a13.value,a14.value);
    }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Used to wrap calling a static member function as a raw function by providing the actual instance before the VM.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template < class T, SQInteger (*Fn)(T &, HSQUIRRELVM) > inline SQInteger SqRawMemberFnWrap(HSQUIRRELVM vm)
{
    // The wrapped instance
    T * inst = nullptr;
    // Attempt to extract the argument values
    try
    {
        inst = Var< T * >(vm, 1).value;
    }
    catch (const Sqrat::Exception & e)
    {
        return sq_throwerror(vm, e.what());
    }
    // Forward the call (assume the instance is valid!)
    return Fn(*inst, vm);
}

}

#endif
