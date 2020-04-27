//
// SqratUtil: Squirrel Utilities
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

#if !defined(_SCRAT_UTIL_H_)
#define _SCRAT_UTIL_H_

#ifdef SQMOD_PLUGIN_API
    #include <SqAPI.h>
#else
    #include <squirrelex.h>
    #include <sqstdstring.h>
#endif // SQMOD_PLUGIN_API

#include <cassert>
#include <map>
#include <string.h>

#if defined(SCRAT_USE_CXX11_OPTIMIZATIONS)
#include <unordered_map>
#endif

namespace Sqrat {

#if defined(__GNUC__)
    #define SQ_UNREACHABLE __builtin_unreachable();
#elif defined(_MSVC)
static _Noreturn void unreachable() { return; }
    #define SQ_UNREACHABLE unreachable();
#else
    #define SQ_UNREACHABLE assert(0);
#endif

/// @cond DEV

#if defined(SCRAT_USE_CXX11_OPTIMIZATIONS)
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Define an unordered map for Sqrat to use based on whether SCRAT_USE_CXX11_OPTIMIZATIONS is defined or not
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    template<class Key, class T>
    struct unordered_map {
        typedef std::unordered_map<Key, T> type;
    };
#else
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Define an unordered map for Sqrat to use based on whether SCRAT_USE_CXX11_OPTIMIZATIONS is defined or not
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    template<class Key, class T>
    struct unordered_map {
        typedef std::map<Key, T> type;
    };
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Define an inline function to avoid MSVC's "conditional expression is constant" warning
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _MSC_VER
    template <typename T>
    inline T _c_def(T value) { return value; }
    #define SQRAT_CONST_CONDITION(value) _c_def(value)
#else
    #define SQRAT_CONST_CONDITION(value) value
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Define helpers to create portable import / export macros
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if defined(SCRAT_EXPORT)
    #if defined(_WIN32)
        // Windows compilers need a specific keyword for export
        #define SQRAT_API __declspec(dllexport)
    #else
        #if __GNUC__ >= 4
            // GCC 4 has special keywords for showing/hiding symbols,
            // the same keyword is used for both importing and exporting
            #define SQRAT_API __attribute__ ((__visibility__ ("default")))
        #else
            // GCC < 4 has no mechanism to explicitly hide symbols, everything's exported
            #define SQRAT_API

        #endif
    #endif
#elif defined(SCRAT_IMPORT)
    #if defined(_WIN32)
        // Windows compilers need a specific keyword for import
        #define SQRAT_API __declspec(dllimport)
    #else
        #if __GNUC__ >= 4
            // GCC 4 has special keywords for showing/hiding symbols,
            // the same keyword is used for both importing and exporting
            #define SQRAT_API __attribute__ ((__visibility__ ("default")))
        #else
            // GCC < 4 has no mechanism to explicitly hide symbols, everything's exported
            #define SQRAT_API
        #endif
    #endif
#else
    #define SQRAT_API
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Define macros for internal error handling
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if defined (SCRAT_NO_ERROR_CHECKING)
    #define SQCATCH(vm)          if (SQRAT_CONST_CONDITION(false))
    #define SQCATCH_NOEXCEPT(vm) if (SQRAT_CONST_CONDITION(false))
    #define SQCLEAR(vm)
    #define SQRETHROW(vm)
    #define SQTHROW(vm, err)
    #define SQTRY()
    #define SQWHAT(vm)           _SC("")
    #define SQWHAT_NOEXCEPT(vm)  _SC("")
#elif defined (SCRAT_USE_EXCEPTIONS)
    #define SQCATCH(vm)          } catch (const ::Sqrat::Exception& e)
    #define SQCATCH_NOEXCEPT(vm) if (SQRAT_CONST_CONDITION(false))
    #define SQCLEAR(vm)
    #ifdef _MSC_VER // avoid MSVC's "unreachable code" warning
        #define SQRETHROW(vm)      if (SQRAT_CONST_CONDITION(true)) throw
        #define SQTHROW(vm, err)   if (SQRAT_CONST_CONDITION(true)) throw ::Sqrat::Exception(err)
    #else
        #define SQRETHROW(vm)      throw
        #define SQTHROW(vm, err)   throw ::Sqrat::Exception(err)
    #endif
    #define SQTRY()              try {
    #define SQWHAT(vm)           e.Message().c_str()
    #define SQWHAT_NOEXCEPT(vm)  _SC("")
#else
    #define SQCATCH(vm)          if (SQRAT_CONST_CONDITION(false))
    #define SQCATCH_NOEXCEPT(vm) if (Error::Occurred(vm))
    #define SQCLEAR(vm)          Error::Clear(vm)
    #define SQRETHROW(vm)
    #define SQTHROW(vm, err)     Error::Throw(vm, err)
    #define SQTRY()
    #define SQWHAT(vm)           _SC("")
    #define SQWHAT_NOEXCEPT(vm)  Error::Message(vm).c_str()
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Removes unused variable warnings in a way that Doxygen can understand
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename T>
void SQUNUSED(const T&) {
}

/// @endcond

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Defines a string that is definitely compatible with the version of Squirrel being used (normally this is std::string)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef std::basic_string<SQChar> string;

/// @cond DEV
#ifdef SQUNICODE
/* from http://stackoverflow.com/questions/15333259/c-stdwstring-to-stdstring-quick-and-dirty-conversion-for-use-as-key-in,
   only works for ASCII chars */
/**
* Convert a std::string into a std::wstring
*/
static std::wstring string_to_wstring(const std::string& str)
{
    return std::wstring(str.begin(), str.end());
}

/**
* Convert a std::wstring into a std::string
*/
static std::string wstring_to_string(const std::wstring& wstr)
{
    return std::string(wstr.begin(), wstr.end());
}

#endif // SQUNICODE
template < class T >
struct SqDefaultAllocator;
template < class T >
struct SqDefaultDelete;
template < class T >
struct SqDefaultDestructor;

template <class T, class D=SqDefaultDelete<T>>
class SharedPtr;

template <class T, class D=SqDefaultDelete<T>>
class WeakPtr;

/// @endcond

/// Provides the member typedef `type` that names T (i.e., the identity transformation).
template< class T > struct SqTypeIdentity { using type = T; };

/// Helper class that defines a VM that can be used as a fallback VM in case no other one is given to a piece of code
class DefaultVM {
private:
    static HSQUIRRELVM& StaticVM() {
        static HSQUIRRELVM vm;
        return vm;
    }
public:
    // Gets the default VM (copy)
    static HSQUIRRELVM Get() {
        return StaticVM();
    }
    // Gets the default VM (reference)
    static HSQUIRRELVM & Get_() {
        return StaticVM();
    }
    // Sets the default VM to a given VM
    static void Set(HSQUIRRELVM vm) {
        StaticVM() = vm;
    }
};

/// Alias for SqVM()
inline HSQUIRRELVM SqVM() noexcept { return DefaultVM::Get_(); }

#if !defined (SCRAT_NO_ERROR_CHECKING) && !defined (SCRAT_USE_EXCEPTIONS)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// The class that must be used to deal with errors that Sqrat has
///
/// \remarks
/// When documentation in Sqrat says, "This function MUST have its error handled if it occurred," that
/// means that after the function has been run, you must call Error::Occurred to see if the function
/// ran successfully. If the function did not run successfully, then you MUST either call Error::Clear
/// or Error::Message to clear the error buffer so new ones may occur and Sqrat does not get confused.
///
/// \remarks
/// Any error thrown inside of a bound C++ function will be thrown in the given Squirrel VM and
/// automatically handled.
///
/// \remarks
/// If compiling with SCRAT_USE_EXCEPTIONS defined, Sqrat will throw exceptions instead of using this
/// class to handle errors. This means that functions must be enclosed in try blocks that catch
/// Sqrat::Exception instead of checking for errors with Error::Occurred.
///
/// \remarks
/// If compiling with SCRAT_NO_ERROR_CHECKING defined, Sqrat will run significantly faster,
/// but it will no longer check for errors and the Error class itself will not be defined.
/// In this mode, a Squirrel script may crash the C++ application if errors occur in it.
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Error {
public:

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Clears the error associated with a given VM
    ///
    /// \param vm Target VM
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    static void Clear(HSQUIRRELVM vm) {
        sq_pushregistrytable(vm);
        sq_pushstring(vm, "__error", -1);
        sq_rawdeleteslot(vm, -2, false);
        sq_pop(vm, 1);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Clears the error associated with a given VM and returns the associated error message
    ///
    /// \param vm Target VM
    ///
    /// \return String containing a nice error message
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    static string Message(HSQUIRRELVM vm) {
        sq_pushregistrytable(vm);
        sq_pushstring(vm, "__error", -1);
        if (SQ_SUCCEEDED(sq_rawget(vm, -2))) {
            string** ud;
            sq_getuserdata(vm, -1, (SQUserPointer*)&ud, NULL);
            sq_pop(vm, 1);
            string err = **ud;
            sq_pushstring(vm, "__error", -1);
            sq_rawdeleteslot(vm, -2, false);
            sq_pop(vm, 1);
            return err;
        }
        sq_pushstring(vm, "__error", -1);
        sq_rawdeleteslot(vm, -2, false);
        sq_pop(vm, 1);
        return string(_SC("an unknown error has occurred"));
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Returns whether a Sqrat error has occurred with a given VM
    ///
    /// \param vm Target VM
    ///
    /// \return True if an error has occurred, otherwise false
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    static bool Occurred(HSQUIRRELVM vm) {
        sq_pushregistrytable(vm);
        sq_pushstring(vm, "__error", -1);
        if (SQ_SUCCEEDED(sq_rawget(vm, -2))) {
            sq_pop(vm, 2);
            return true;
        }
        sq_pop(vm, 1);
        return false;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Raises an error in a given VM with a given error message
    ///
    /// \param vm  Target VM
    /// \param err A nice error message
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    static void Throw(HSQUIRRELVM vm, const string& err) {
        sq_pushregistrytable(vm);
        sq_pushstring(vm, "__error", -1);
        if (SQ_FAILED(sq_rawget(vm, -2))) {
            sq_pushstring(vm, "__error", -1);
            string** ud = reinterpret_cast<string**>(sq_newuserdata(vm, sizeof(string*)));
            *ud = new string(err);
            sq_setreleasehook(vm, -1, &error_cleanup_hook);
            sq_rawset(vm, -3);
            sq_pop(vm, 1);
            return;
        }
        sq_pop(vm, 2);
    }

private:

    Error() {}

    static SQInteger error_cleanup_hook(SQUserPointer ptr, SQInteger size) {
        SQUNUSED(size);
        string** ud = reinterpret_cast<string**>(ptr);
        delete *ud;
        return 0;
    }
};
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Tells Sqrat whether Squirrel error handling should be used
///
/// \remarks
/// If true, if a runtime error occurs during the execution of a call, the VM will invoke its error handler.
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ErrorHandling {
private:

    static bool& errorHandling() {
        static bool eh = true;
        return eh;
    }

public:

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Returns whether Squirrel error handling is enabled
    ///
    /// \return True if error handling is enabled, otherwise false
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    static bool IsEnabled() {
        return errorHandling();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Enables or disables Squirrel error handling
    ///
    /// \param enable True to enable, false to disable
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    static void Enable(bool enable) {
        errorHandling() = enable;
    }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Sqrat exception class
///
/// \remarks
/// Used only when SCRAT_USE_EXCEPTIONS is defined (see Sqrat::Error)
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Exception {
public:

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Constructs an exception
    ///
    /// \param msg A nice error message
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Exception(const SQChar * msg) : message(msg) {}

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Constructs an exception
    ///
    /// \param msg A nice error message
    /// \param len Length of the message
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Exception(const SQChar * msg, SQInteger len) : message(msg, len > 0 ? len : 0) {}

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Constructs an exception
    ///
    /// \param msg A nice error message
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    explicit Exception(string && msg) noexcept : message(std::forward< string >(msg)) {}

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Constructs an exception
    ///
    /// \param msg A nice error message
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    explicit Exception(const string& msg) : message(msg) {}

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Copy constructor
    ///
    /// \param ex Exception to copy
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Exception(const Exception& ex) : message(ex.message) {}

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Returns a string identifying the exception
    ///
    /// \return A nice error message
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    const string& Message() const {
        return message;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Returns a C string identifying the exception
    ///
    /// \return A nice error message
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    const SQChar * what() const {
        return message.c_str();
    }

private:

    string message;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Returns a string that has been formatted to give a nice type error message (for usage with Class::SquirrelFunc)
///
/// \param vm           VM the error occurred with
/// \param idx          Index on the stack of the argument that had a type error
/// \param expectedType The name of the type that the argument should have been
///
/// \return String containing a nice type error message
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline string FormatTypeError(HSQUIRRELVM vm, SQInteger idx, const string& expectedType) {
    string err = _SC("wrong type (") + expectedType + _SC(" expected");
#if (SQUIRREL_VERSION_NUMBER>= 200) && (SQUIRREL_VERSION_NUMBER < 300) // Squirrel 2.x
    err = err + _SC(")");
#else // Squirrel 3.x
    if (SQ_SUCCEEDED(sq_typeof(vm, idx))) {
        const SQChar* actualType;
        sq_tostring(vm, -1);
        sq_getstring(vm, -1, &actualType);
        sq_pop(vm, 2);
        err = err + _SC(", got ") + actualType + _SC(")");
    } else {
        err = err + _SC(", got unknown)");
    }
#endif
    return err;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Returns the last error that occurred with a Squirrel VM (not associated with Sqrat errors)
///
/// \param vm Target VM
///
/// \return String containing a nice error message
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline string LastErrorString(HSQUIRRELVM vm) {
    const SQChar* sqErr;
    SQInteger size;
    sq_getlasterror(vm);
    if (sq_gettype(vm, -1) == OT_NULL) {
        sq_pop(vm, 1);
        return string();
    }
    sq_tostring(vm, -1);
    sq_getstringandsize(vm, -1, &sqErr, &size);
    sq_pop(vm, 2);
    return string(sqErr, size);
}
        
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Primary template of `SqDefaultAllocator`.
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template < class T > struct SqDefaultAllocator {
    // ----------------------------------------------------------------------------------------------------------------
    // Type of value used to store a single element of the given type with the proper alignment.
    using StorageType = typename std::aligned_storage< sizeof(T), alignof(T) >::type;
    // ----------------------------------------------------------------------------------------------------------------
    // Allocate memory (possibly uninitialized) for the specialized type.
    template < class... A > static T * New(A&&... a) { return new T(std::forward< A >(a)...); }
    // Allocate memory (preferably uninitialized) for the specialized type.
    static T * NewRaw() { return reinterpret_cast< T * >(new StorageType); }
    // ----------------------------------------------------------------------------------------------------------------
    // Allocate memory (possibly uninitialized) for the specialized type.
    template < class... A > static T * NewN(size_t n, A&&... a) {
        T * p = NewRawN(n);
        assert(p);
        for (size_t i = 0; i < n; ++i) {
            new (&p[i]) T(std::forward< A >(a)...); // Invoke constructor
        }
        return p;
    }
    // Allocate memory (preferably uninitialized) for the specialized type.
    static T * NewRawN(size_t n) { return reinterpret_cast< T * >(new StorageType[n]); }
    // ----------------------------------------------------------------------------------------------------------------
    // Deallocate memory (possibly uninitialized) for the specialized type.
    static void Delete(T * p) { delete p; }
    // Deallocate memory (preferably uninitialized) for the specialized type.
    static void DeleteRaw(T * p) { delete reinterpret_cast< StorageType * >(p); }
    // ----------------------------------------------------------------------------------------------------------------
    // Deallocate memory (possibly uninitialized) for the specialized type.
    static void Delete(T * p, size_t SQ_UNUSED_ARG(n)) { delete[] p; }
    // Deallocate memory (preferably uninitialized) for the specialized type.
    static void DeleteRaw(T * p, size_t SQ_UNUSED_ARG(n)) { delete[] reinterpret_cast< StorageType * >(p); }
};
// ====================================================================================================================
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Primary template of `SqDefaultDelete`. Will invoke the instance destructor and free the associated memory.
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template < class T > struct SqDefaultDelete {
    // The type given via template parameter.
    using ValueType = T;
    // Default constructor.
    constexpr SqDefaultDelete() noexcept = default;
    // Function call operator that forwards the call to delete on the given pointer.
    void operator () (T * p) const {
        static_assert(!std::is_void< T >::value, "Cannot delete pointer to incomplete type.");
        static_assert(sizeof(T), "Cannot delete pointer to incomplete type.");
        delete p;
    }
};
// --------------------------------------------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Specialization of `SqDefaultDelete` for arrays. Will invoke each instance destructor and free the associated memory.
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template < class T > struct SqDefaultDelete< T[] > {
    // The type given via template parameter.
    using ValueType = T;
    // Default constructor.
    constexpr SqDefaultDelete() noexcept = default;
    // Function call operator that forwards the call to delete on the given pointer.
    void operator () (T * p, size_t SQ_UNUSED_ARG(n)) const {
        static_assert(sizeof(T), "Cannot delete pointer to incomplete type.");
        delete[] p;
    }
};
// --------------------------------------------------------------------------------------------------------------------
// Omit specialization for array objects with a compile time length.
template < class T, unsigned N > struct SqDefaultDelete< T[N] >;
// ====================================================================================================================
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Primary template of `SqDefaultNoDelete`. Will do absolutely nothing.
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template < class T > struct SqDefaultNoDelete {
    // The type given via template parameter.
    using ValueType = T;
    // Default constructor.
    constexpr SqDefaultNoDelete() noexcept = default;
    // Dummy function call operator.
    void operator () (T * SQ_UNUSED_ARG(p)) const { }
};
// ====================================================================================================================
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Primary template of `SqDefaultDestructor`. Will only invoke the instance destructor and not free the associated memory.
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template < class T > struct SqDefaultDestructor {
    // The type given via template parameter.
    using ValueType = T;
    // Default constructor.
    constexpr SqDefaultDestructor() noexcept = default;
    // Function call operator that forwards the call ro `~T()` on the given pointer.
    void operator () (T * p) const {
        static_assert(!std::is_void< T >::value, "Cannot destroy pointer to incomplete type.");
        static_assert(sizeof(T), "Cannot destroy pointer to incomplete type.");
        assert(p == nullptr);
        p->~T();
    }
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Specialization of `SqDefaultDestructor` for arrays. Will only invoke each instance destructor and not free the associated memory.
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template < class T > struct SqDefaultDestructor< T[] > {
    // The type given via template parameter.
    using ValueType = T;
    // Default constructor.
    constexpr SqDefaultDestructor() noexcept = default;
    // Function call operator that forwards the call to `~T()` on the given pointer.
    void operator () (T * p, size_t SQ_UNUSED_ARG(n)) const {
        static_assert(sizeof(T), "Cannot destroy pointer to incomplete type.");
        assert(p == nullptr && n > 0);
        for (size_t i = 0; i < n; ++i) p[i].~T(); // Invoke destructor
    }
};
// --------------------------------------------------------------------------------------------------------------------
// Omit specialization for array objects with a compile time length.
template < class T, unsigned N > struct SqDefaultDestructor< T[N] >;

// ====================================================================================================================
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Primary template of `SqDefaultNoDestructor`. Will do absolutely nothing.
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template < class T > struct SqDefaultNoDestructor {
    // The type given via template parameter.
    using ValueType = T;
    // Default constructor.
    constexpr SqDefaultNoDestructor() noexcept = default;
    // Dummy function call operator.
    void operator () (T * SQ_UNUSED_ARG(p)) const { }
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Used internally to store the counters of managed pointers.
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct SqReferenceCounter {
    typedef unsigned int ValueType; ///< The type of value that is used to represent a count.

    ValueType mHard; ///< Strong reference count. If equal to zero, the object has been destroyed.
    ValueType mSoft; ///< Weak reference count. If equal to zero, the counter has been destroyed.

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Default constructor
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    SqReferenceCounter()
        : mHard(0), mSoft(0)
    { }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Construct with specific counts
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    SqReferenceCounter(ValueType hard, ValueType soft)
        : mHard(hard), mSoft(soft)
    { }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// A smart pointer that retains shared ownership of an object through a pointer (see std::shared_ptr)
///
/// \tparam T Type of pointer
///
/// \remarks
/// SharedPtr exists to automatically delete an object when all references to it are destroyed.
///
/// \remarks
/// std::shared_ptr was not used because it is a C++11 feature.
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <class T, class D>
class SharedPtr
{
    template <class U, class V>
    friend class SharedPtr;

    template <class U, class V>
    friend class WeakPtr;

    typedef SqReferenceCounter Counter;

private:

    T*            m_Ptr;
    Counter*      m_Ref;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Constructs with explicit pointer and counter.
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    SharedPtr(T * ptr, Counter * ref)
        : m_Ptr(ptr)
        , m_Ref(ref)
    {
        if (m_Ptr != NULL)
        {
            ++(m_Ref->mHard);
            ++(m_Ref->mSoft);
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Assigns a new pointer.
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void Assign(T * ptr)
    {
        if (m_Ptr != ptr)
        {
            Reset();

            if (ptr != NULL)
            {
                m_Ptr = ptr;
                m_Ref = new Counter(1,1);
            }
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Assigns a new pointer and counter.
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void Assign(T * ptr, Counter * ref)
    {
        if (m_Ptr != ptr)
        {
            Reset();

            if (ptr != NULL)
            {
                m_Ptr = ptr;
                m_Ref = ref;

                ++(m_Ref->mHard);
                ++(m_Ref->mSoft);
            }
        }
    }

public:

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Constructs a new SharedPtr
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    SharedPtr()
        : m_Ptr(NULL)
        , m_Ref(NULL)
    {

    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Constructs a new SharedPtr from an object allocated with the new operator
    ///
    /// \param ptr Should be the return value from a call to the new operator
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    explicit SharedPtr(T* ptr)
        : m_Ptr(NULL)
        , m_Ref(NULL)
    {
        Assign(ptr);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Constructs a new SharedPtr from an object allocated with the new operator
    ///
    /// \param ptr Should be the return value from a call to the new operator
    ///
    /// \tparam U Type of pointer (usually doesnt need to be defined explicitly)
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    template <class U>
    explicit SharedPtr(U* ptr)
        : m_Ptr(NULL)
        , m_Ref(NULL)
    {
        Assign(static_cast<T*>(ptr));
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Copy constructor
    ///
    /// \param copy SharedPtr to copy
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    SharedPtr(const SharedPtr<T,D>& copy)
        : SharedPtr(copy.m_Ptr, copy.m_Ref)
    {
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Copy constructor
    ///
    /// \param copy SharedPtr to copy
    ///
    /// \tparam U Type of copy (usually doesnt need to be defined explicitly)
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    template <class U>
    SharedPtr(const SharedPtr<U,D>& copy)
        : SharedPtr(static_cast<T*>(copy.m_Ptr), copy.m_Ref)
    {
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Move constructor
    ///
    /// \param other SharedPtr to move
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    SharedPtr(SharedPtr<T,D>&& other) noexcept
        : m_Ptr(other.m_Ptr)
        , m_Ref(other.m_Ref)
    {
        other.m_Ptr = NULL;
        other.m_Ref = NULL;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Move constructor
    ///
    /// \param other SharedPtr to move
    ///
    /// \tparam U Type of pointer (usually doesnt need to be defined explicitly)
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    template <class U>
    SharedPtr(SharedPtr<U,D>&& other) noexcept
        : m_Ptr(static_cast<T*>(other.m_Ptr))
        , m_Ref(other.m_Ref)
    {
        other.m_Ptr = NULL;
        other.m_Ref = NULL;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Copy constructor
    ///
    /// \param copy WeakPtr to copy
    ///
    /// \tparam U Type of copy (usually doesnt need to be defined explicitly)
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    SharedPtr(const WeakPtr<T,D>& copy)
        : SharedPtr(copy.m_Ptr, copy.m_Ref)
    {
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Copy constructor
    ///
    /// \param copy WeakPtr to copy
    ///
    /// \tparam U Type of copy (usually doesnt need to be defined explicitly)
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    template <class U>
    SharedPtr(const WeakPtr<U,D>& copy)
        : SharedPtr(static_cast<T*>(copy.m_Ptr), copy.m_Ref)
    {
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Destructs the owned object if no more SharedPtr link to it
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ~SharedPtr()
    {
        Reset();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Assigns the SharedPtr
    ///
    /// \param copy SharedPtr to copy
    ///
    /// \return The SharedPtr itself
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    SharedPtr<T,D>& operator=(const SharedPtr<T,D>& copy)
    {
        Assign(copy.m_Ptr, copy.m_Ref);

        return *this;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Assigns the SharedPtr
    ///
    /// \param copy SharedPtr to copy
    ///
    /// \tparam U Type of copy (usually doesnt need to be defined explicitly)
    ///
    /// \return The SharedPtr itself
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    template <class U>
    SharedPtr<T,D>& operator=(const SharedPtr<U,D>& copy)
    {
        Assign(static_cast<T*>(copy.m_Ptr), copy.m_Ref);

        return *this;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Assigns the SharedPtr
    ///
    /// \param other SharedPtr to move
    ///
    /// \return The SharedPtr itself
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    SharedPtr<T,D>& operator=(SharedPtr<T,D>&& other) noexcept
    {
        if (m_Ptr != other.m_Ptr)
        {
            Reset();

            m_Ptr = other.m_Ptr;
            m_Ref = other.m_Ref;

            other.m_Ptr = NULL;
            other.m_Ref = NULL;
        }

        return *this;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Assigns the SharedPtr
    ///
    /// \param other SharedPtr to move
    ///
    /// \tparam U Type of pointer (usually doesnt need to be defined explicitly)
    ///
    /// \return The SharedPtr itself
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    template <class U>
    SharedPtr<T,D>& operator=(SharedPtr<U,D>&& other) noexcept
    {
        if (m_Ptr != static_cast<T*>(other.m_Ptr))
        {
            Reset();

            m_Ptr = static_cast<T*>(other.m_Ptr);
            m_Ref = other.m_Ref;

            other.m_Ptr = NULL;
            other.m_Ref = NULL;
        }

        return *this;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Sets up a new object to be managed by the SharedPtr
    ///
    /// \param ptr Should be the return value from a call to the new operator
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void Init(T* ptr)
    {
        Assign(ptr);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Sets up a new object to be managed by the SharedPtr
    ///
    /// \param ptr Should be the return value from a call to the new operator
    ///
    /// \tparam U Type of copy (usually doesnt need to be defined explicitly)
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    template <class U>
    void Init(U* ptr)
    {
        Assign(static_cast<T*>(ptr));
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Clears the owned object for this SharedPtr and deletes it if no more SharedPtr link to it
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void Reset()
    {
        if (m_Ptr != NULL)
        {
            --(m_Ref->mHard);

            if (m_Ref->mHard == 0)
            {
                D{}(m_Ptr);
            }

            --(m_Ref->mSoft);

            if (m_Ref->mSoft == 0)
            {
                delete m_Ref;
            }

            m_Ptr = NULL;
            m_Ref = NULL;
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Checks if there is an associated managed object
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    operator bool () const // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
    {
        return m_Ptr;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Checks if there is NOT an associated managed object
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    bool operator!() const
    {
        return !m_Ptr;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Compares with another SharedPtr
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    template <typename U>
    bool operator ==(const SharedPtr<U,D>& right) const
    {
        return m_Ptr == right.m_Ptr;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Compares with another SharedPtr
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    bool operator ==(const SharedPtr<T,D>& right) const
    {
        return m_Ptr == right.m_Ptr;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Compares with another pointer
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    template <typename U>
    bool friend operator ==(const SharedPtr<T,D>& left, const U* right)
    {
        return left.m_Ptr == right;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Compares with another pointer
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    bool friend operator ==(const SharedPtr<T,D>& left, const T* right)
    {
        return left.m_Ptr == right;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Compares with another pointer
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    template <typename U>
    bool friend operator ==(const U* left, const SharedPtr<T,D>& right)
    {
        return left == right.m_Ptr;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Compares with another pointer
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    bool friend operator ==(const T* left, const SharedPtr<T,D>& right)
    {
        return left == right.m_Ptr;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Compares with another SharedPtr
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    template <typename U>
    bool operator !=(const SharedPtr<U,D>& right) const
    {
        return m_Ptr != right.m_Ptr;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Compares with another SharedPtr
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    bool operator !=(const SharedPtr<T,D>& right) const
    {
        return m_Ptr != right.m_Ptr;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Compares with another pointer
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    template <typename U>
    bool friend operator !=(const SharedPtr<T,D>& left, const U* right)
    {
        return left.m_Ptr != right;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Compares with another pointer
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    bool friend operator !=(const SharedPtr<T,D>& left, const T* right)
    {
        return left.m_Ptr != right;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Compares with another pointer
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    template <typename U>
    bool friend operator !=(const U* left, const SharedPtr<T,D>& right)
    {
        return left != right.m_Ptr;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Compares with another pointer
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    bool friend operator !=(const T* left, const SharedPtr<T,D>& right)
    {
        return left != right.m_Ptr;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Dereferences pointer to the managed object
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    T& operator*() const
    {
        assert(m_Ptr != NULL); // fails when dereferencing a null SharedPtr
        return *m_Ptr;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Dereferences pointer to the managed object
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    T* operator->() const
    {
        assert(m_Ptr != NULL); // fails when dereferencing a null SharedPtr
        return m_Ptr;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Gets the underlying pointer
    ///
    /// \return Pointer to the managed object
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    T* Get() const
    {
        return m_Ptr;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Gets the number of references to the underlying pointer
    ///
    /// \return Number of references
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Counter::ValueType Count() const
    {
        return m_Ref ? m_Ref->mHard : 0;
    }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// A smart pointer that retains a non-owning ("weak") reference to an object that is managed by SharedPtr (see std::weak_ptr)
///
/// \tparam T Type of pointer
///
/// \remarks
/// WeakPtr exists for when an object that may be deleted at any time needs to be accessed if it exists.
///
/// \remarks
/// std::weak_ptr was not used because it is a C++11 feature.
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <class T, class D>
class WeakPtr
{
    template <class U, class V>
    friend class SharedPtr;

    typedef SqReferenceCounter Counter;

private:

    T*            m_Ptr;
    Counter*      m_Ref;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Constructs with explicit pointer and counter.
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    WeakPtr(T * ptr, Counter * ref)
        : m_Ptr(ptr)
        , m_Ref(ref)
    {
        if (m_Ptr != NULL)
        {
            ++(m_Ref->mSoft);
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Initializes the pointer and counter.
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void Initialize(T * ptr, Counter * ref)
    {
        if (ptr != NULL)
        {
            m_Ptr = ptr;
            m_Ref = ref;

            ++(m_Ref->mSoft);
        }
        else
        {
            m_Ptr = NULL;
            m_Ref = NULL;
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Assigns a new pointer and counter.
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void Assign(T * ptr, Counter * ref)
    {
        if (m_Ptr != ptr)
        {
            Reset();

            if (ptr != NULL)
            {
                m_Ptr = ptr;
                m_Ref = ref;

                ++(m_Ref->mSoft);
            }
        }
    }

public:

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Constructs a new WeakPtr
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    WeakPtr()
        : m_Ptr(NULL)
        , m_Ref(NULL)
    {

    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Copy constructor
    ///
    /// \param copy WeakPtr to copy
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    WeakPtr(const WeakPtr<T,D>& copy)
    {
        Initialize(copy.m_Ptr, copy.m_Ref);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Copy constructor
    ///
    /// \param copy WeakPtr to copy
    ///
    /// \tparam U Type of copy (usually doesnt need to be defined explicitly)
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    template <class U>
    WeakPtr(const WeakPtr<U,D>& copy)
    {
        Initialize(static_cast<T*>(copy.m_Ptr), copy.m_Ref);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Move constructor
    ///
    /// \param other WeakPtr to move
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    WeakPtr(WeakPtr<T,D>&& other) noexcept
        : m_Ptr(other.m_Ptr)
        , m_Ref(other.m_Ref)
    {
        other.m_Ptr = NULL;
        other.m_Ref = NULL;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Move constructor
    ///
    /// \param other WeakPtr to move
    ///
    /// \tparam U Type of pointer (usually doesnt need to be defined explicitly)
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    template <class U>
    WeakPtr(WeakPtr<U,D>&& other) noexcept
        : m_Ptr(static_cast<T*>(other.m_Ptr))
        , m_Ref(other.m_Ref)
    {
        other.m_Ptr = NULL;
        other.m_Ref = NULL;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Copy constructor
    ///
    /// \param copy SharedPtr to copy
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    WeakPtr(const SharedPtr<T,D>& copy)
    {
        Initialize(copy.m_Ptr, copy.m_Ref);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Copy constructor
    ///
    /// \param copy SharedPtr to copy
    ///
    /// \tparam U Type of copy (usually doesnt need to be defined explicitly)
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    template <class U>
    WeakPtr(const SharedPtr<U,D>& copy)
    {
        Initialize(static_cast<T*>(copy.m_Ptr), copy.m_Ref);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Destructs the WeakPtr but has no influence on the object that was managed
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ~WeakPtr()
    {
        Reset();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Assigns the WeakPtr
    ///
    /// \param copy WeakPtr to copy
    ///
    /// \return The WeakPtr itself
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    WeakPtr<T,D>& operator=(const WeakPtr<T,D>& copy)
    {
        Assign(copy.m_Ptr, copy.m_Ref);

        return *this;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Assigns the WeakPtr
    ///
    /// \param copy WeakPtr to copy
    ///
    /// \tparam U Type of copy (usually doesnt need to be defined explicitly)
    ///
    /// \return The WeakPtr itself
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    template <class U>
    WeakPtr<T,D>& operator=(const WeakPtr<U,D>& copy)
    {
        Assign(static_cast<T*>(copy.m_Ptr), copy.m_Ref);

        return *this;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Assigns the WeakPtr
    ///
    /// \param other WeakPtr to move
    ///
    /// \return The WeakPtr itself
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    WeakPtr<T,D>& operator=(WeakPtr<T,D>&& other) noexcept
    {
        if (m_Ptr != other.m_Ptr)
        {
            Reset();

            m_Ptr = other.m_Ptr;
            m_Ref = other.m_Ref;

            other.m_Ptr = NULL;
            other.m_Ref = NULL;
        }

        return *this;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Assigns the WeakPtr
    ///
    /// \param other WeakPtr to move
    ///
    /// \tparam U Type of pointer (usually doesnt need to be defined explicitly)
    ///
    /// \return The WeakPtr itself
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    template <class U>
    WeakPtr<T,D>& operator=(WeakPtr<U,D>&& other) noexcept
    {
        if (m_Ptr != static_cast<T*>(other.m_Ptr))
        {
            Reset();

            m_Ptr = static_cast<T*>(other.m_Ptr);
            m_Ref = other.m_Ref;

            other.m_Ptr = NULL;
            other.m_Ref = NULL;
        }

        return *this;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Assigns the SharedPtr
    ///
    /// \param copy SharedPtr to copy
    ///
    /// \return The WeakPtr itself
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    WeakPtr<T,D>& operator=(const SharedPtr<T,D>& copy)
    {
        Assign(copy.m_Ptr, copy.m_Ref);

        return *this;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Assigns the SharedPtr
    ///
    /// \param copy SharedPtr to copy
    ///
    /// \tparam U Type of copy (usually doesnt need to be defined explicitly)
    ///
    /// \return The WeakPtr itself
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    template <class U>
    WeakPtr<T,D>& operator=(const SharedPtr<U,D>& copy)
    {
        Assign(static_cast<T*>(copy.m_Ptr), copy.m_Ref);

        return *this;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Checks whether the managed object exists
    ///
    /// \return True if the managed object does not exist, false otherwise
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    bool Expired() const
    {
        return (m_Ptr == NULL || m_Ref->mHard == 0);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Creates a new SharedPtr that shares ownership of the managed object
    ///
    /// \return A SharedPtr which shares ownership of the managed object
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    SharedPtr<T,D> Lock() const
    {
        return SharedPtr<T,D>(m_Ptr, m_Ref);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Clears the associated object for this WeakPtr
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void Reset()
    {
        if (m_Ptr != NULL)
        {
            --(m_Ref->mSoft);

            if (m_Ref->mSoft == 0)
            {
                delete m_Ref;
            }

            m_Ptr = NULL;
            m_Ref = NULL;
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Gets the number of weak references to the underlying pointer
    ///
    /// \return Number of references
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Counter::ValueType Count() const
    {
        return m_Ref ? m_Ref->mSoft : 0;
    }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Helper structure for popping elements from the stack.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct SqPopGuard
{
    HSQUIRRELVM mVM; // The VM from which the elements must be popped.
    SQInteger   mNum; // The number of elements to be popped.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Base constructor.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    SqPopGuard(HSQUIRRELVM vm, SQInteger num)
        : mVM(vm), mNum(num)
    {
        //...
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Destructor. Pops the specified elements from the stack.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ~SqPopGuard()
    {
        sq_pop(mVM, mNum);
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Increment the number of elements to be popped.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    SqPopGuard & operator ++ ()
    {
        ++mNum;
        return *this;
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Decrement the number of elements to be popped.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    SqPopGuard & operator -- ()
    {
        --mNum;
        return *this;
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Increase the number of elements to be popped.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    SqPopGuard & operator += (SQInteger n)
    {
        mNum += n;
        return *this;
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Decrease the number of elements to be popped.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    SqPopGuard & operator -= (SQInteger n)
    {
        mNum -= n;
        return *this;
    }
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Implements RAII to restore the VM stack to it's initial size on function exit.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct StackGuard
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Default constructor.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    StackGuard()
        : m_VM(SqVM()), m_Top(sq_gettop(m_VM))
    {
        /* ... */
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Base constructor.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    StackGuard(HSQUIRRELVM vm)
        : m_VM(vm), m_Top(sq_gettop(vm))
    {
        /* ... */
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Copy constructor. (disabled)
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    StackGuard(const StackGuard &) = delete;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Move constructor. (disabled)
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    StackGuard(StackGuard &&) = delete;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Destructor.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ~StackGuard()
    {
        Restore();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Copy assignment operator. (disabled)
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    StackGuard & operator = (const StackGuard &) = delete;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Move assignment operator. (disabled)
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    StackGuard & operator = (StackGuard &&) = delete;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Restore the stack to what was known to be.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void Restore() const
    {
        // Retrieve the new stack top
        const SQInteger top = sq_gettop(m_VM);
        // Did the stack size change?
        if (top > m_Top)
        {
            sq_pop(m_VM, top - m_Top); // Trim the stack
        }
    }

private:

    HSQUIRRELVM m_VM; ///< The VM where the stack should be restored.
    SQInteger   m_Top; ///< The top of the stack when this instance was created.
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Helper function to transform a negative index into a positive index.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline SQInteger IndexAbs(SQInteger top, SQInteger idx)
{
    return (idx <= -1) ? (top + idx + 1) : idx;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Hashing utilities.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef const uint8_t * FnvHashData;

static constexpr uint32_t FnvHashSeed32    = 2166136261u;
static constexpr uint32_t FnvHashPrime32   = 16777619u;
// Hash a single byte.
inline uint32_t Fnv1a32(uint8_t byte, uint32_t hash = FnvHashSeed32)
{
    return (byte ^ hash) * FnvHashPrime32;
}
// Hash an array of bytes. 32-bit variant.
inline uint32_t FnvHash32(FnvHashData data, size_t size, uint32_t hash = FnvHashSeed32)
{
    assert(data);
    while (size--)
    {
        hash = Fnv1a32(*(data++), hash);
    }
    return hash;
}
static constexpr uint64_t FnvHashSeed64    = 14695981039346656037llu;
static constexpr uint64_t FnvHashPrime64   = 1099511628211llu;
// Hash a single byte.
inline uint64_t Fnv1a64(uint8_t byte, uint64_t hash = FnvHashSeed64)
{
    return (byte ^ hash) * FnvHashPrime64;
}
// Hash an array of bytes. 64-bit variant.
inline uint64_t FnvHash64(FnvHashData data, size_t size, uint64_t hash = FnvHashSeed64)
{
    assert(data);
    while (size--)
    {
        hash = Fnv1a64(*(data++), hash);
    }
    return hash;
}
#ifdef _SQ64
    static constexpr size_t FnvHashSeed    = FnvHashSeed64;
    static constexpr size_t FnvHashPrime   = FnvHashPrime64;
#else
    static constexpr size_t FnvHashSeed    = FnvHashSeed32;
    static constexpr size_t FnvHashPrime   = FnvHashPrime32;
#endif // _SQ64
// Hash a single byte.
inline size_t Fnv1a(uint8_t byte, size_t hash = FnvHashSeed)
{
    return (byte ^ hash) * FnvHashPrime;
}

// Hash an array of bytes.
inline size_t FnvHash(const uint8_t * data, size_t size, size_t hash = FnvHashSeed) {
    assert(data);
    while (size--)
    {
        hash = Fnv1a(*(data++), hash);
    }
    return hash;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Helper structure for retrieving a value from the stack as a string or a formatted string.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct StackStrF
{
    const SQChar *  mPtr; ///< Pointer to the C string that was retrieved.
    SQInteger       mLen; ///< The string length if it could be retrieved.
    SQRESULT        mRes; ///< The result of the retrieval attempts.
    HSQOBJECT       mObj; ///< Strong reference to the string object.
    HSQUIRRELVM     mVM; ///< The associated virtual machine.
    SQInteger       mIdx; ///< The index where the string should be retrieved from.

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Default constructor.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    StackStrF()
        : mPtr(_SC(""))
        , mLen(0)
        , mRes(SQ_OK)
        , mObj()
        , mVM(nullptr)
        , mIdx(-1)
    {
        sq_resetobject(&mObj); // Reset the converted value object
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Compile time string constructor.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    template < size_t N > StackStrF(const SQChar(&str)[N])
        : mPtr(str)
        , mLen(N)
        , mRes(SQ_OK)
        , mObj()
        , mVM(nullptr)
        , mIdx(-1)
    {
        sq_resetobject(&mObj); // Reset the converted value object
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Base constructor.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    StackStrF(HSQUIRRELVM vm, SQInteger idx)
        : mPtr(_SC(""))
        , mLen(0)
        , mRes(SQ_OK)
        , mObj()
        , mVM(vm)
        , mIdx(idx)
    {
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Copy constructor. (disabled)
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    StackStrF(const StackStrF & o) = delete;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Move constructor.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    StackStrF(StackStrF && o) noexcept
        : mPtr(o.mPtr)
        , mLen(o.mLen)
        , mRes(o.mRes)
        , mObj(o.mObj)
        , mVM(o.mVM)
        , mIdx(o.mIdx)
    {
        o.mPtr = _SC("");
        o.mLen = 0;
        o.mRes = SQ_OK;
        o.mVM = nullptr;
        o.mIdx = -1;
        sq_resetobject(&o.mObj);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Destructor.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ~StackStrF()
    {
        if (!sq_isnull(mObj))
        {
            sq_release(mVM ? mVM : SqVM(), &mObj);
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Copy assignment operator. (disabled)
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    StackStrF & operator = (const StackStrF & o) = delete;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Move assignment operator.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    StackStrF & operator = (StackStrF && o) noexcept
    {
        if (this != &o)
        {
            // Release
            if (!sq_isnull(mObj))
            {
                sq_release(mVM ? mVM : SqVM(), &mObj);
                sq_resetobject(&mObj);
            }
            // Replicate
            mPtr = o.mPtr;
            mLen = o.mLen;
            mRes = o.mRes;
            mObj = o.mObj;
            mVM = o.mVM;
            mIdx = o.mIdx;
            // Own
            o.mPtr = _SC("");
            o.mLen = 0;
            o.mRes = SQ_OK;
            o.mVM = nullptr;
            o.mIdx = -1;
            sq_resetobject(&o.mObj);
        }
        return *this;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Release any object references and assign a new target if necessary, then return self for chaining.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    StackStrF & Release(HSQUIRRELVM vm = nullptr,  SQInteger idx = -1)
    {
        if (!sq_isnull(mObj))
        {
            sq_release(mVM ? mVM : SqVM(), &mObj);
        }
        mPtr = _SC("");
        mLen = 0;
        mRes = SQ_OK;
        mVM = vm;
        mIdx = idx;
        sq_resetobject(&mObj);
        return *this;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Actual implementation.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    SQRESULT Proc(bool fmt = false, SQInteger top = -1)
    {
        // If there's no virtual machine, just ignore the request
        if (mVM == nullptr)
        {
            return SQ_OK;
        }
        // Release the converted value object
        else if (!sq_isnull(mObj))
        {
            sq_release(mVM, &mObj);
        }
        // Reset the converted value object
        sq_resetobject(&mObj);
        // Grab the top of the stack, if necessary
        if (top < 0)
        {
            top = sq_gettop(mVM);
        }
        // Make the stack index absolute
        mIdx = IndexAbs(top, mIdx);
        // Was the string or value specified?
        if (top <= (mIdx - 1))
        {
            mRes = sq_throwerror(mVM, "Missing string or value");
        }
        // If null was specified then treat it as a dummy
        else if (sq_gettype(mVM, mIdx) == OT_NULL)
        {
            // Default to an empty string and ignore formatting even if possible
            mPtr = _SC("");
            mLen = 0;
            mRes = SQ_OK;
        }
        // Do we have enough values to call the format function and are we allowed to?
        else if (fmt && (top - 1) >= mIdx)
        {
            // Pointer to the generated string
            SQChar * str = nullptr;
            // Attempt to generate the specified string format
            mRes = sqstd_format(mVM, mIdx, &mLen, &str);
            // Did the format succeeded but ended up with a null string pointer?
            if (SQ_SUCCEEDED(mRes) && !str)
            {
                mRes = sq_throwerror(mVM, "Unable to generate the string");
            }
            else
            {
                mPtr = const_cast< const SQChar * >(str);
            }
        }
        // Is the value on the stack an actual string?
        else if (sq_gettype(mVM, mIdx) == OT_STRING)
        {
            // Obtain a reference to the string object
            mRes = sq_getstackobj(mVM, mIdx, &mObj);
            // Could we retrieve the object from the stack?
            if (SQ_SUCCEEDED(mRes))
            {
                // Keep a strong reference to the object
                sq_addref(mVM, &mObj);
                // Attempt to retrieve the string value from the stack
                mRes = sq_getstringandsize(mVM, mIdx, &mPtr, &mLen);
            }
            // Did the retrieval succeeded but ended up with a null string pointer?
            if (SQ_SUCCEEDED(mRes) && !mPtr)
            {
                mRes = sq_throwerror(mVM, "Unable to retrieve the string");
            }
        }
        // We have to try and convert it to string
        else
        {
            // Attempt to convert the value from the stack to a string
            mRes = sq_tostring(mVM, mIdx);
            // Could we convert the specified value to string?
            if (SQ_SUCCEEDED(mRes))
            {
                // Obtain a reference to the resulted object
                mRes = sq_getstackobj(mVM, -1, &mObj);
                // Could we retrieve the object from the stack?
                if (SQ_SUCCEEDED(mRes))
                {
                    // Keep a strong reference to the object
                    sq_addref(mVM, &mObj);
                    // Attempt to obtain the string pointer
                    mRes = sq_getstringandsize(mVM, -1, &mPtr, &mLen);
                }
            }
            // Pop a value from the stack regardless of the result
            sq_pop(mVM, 1);
            // Did the retrieval succeeded but ended up with a null string pointer?
            if (SQ_SUCCEEDED(mRes) && !mPtr)
            {
                mRes = sq_throwerror(mVM, "Unable to retrieve the value");
            }
        }
        return mRes;
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Compute the hash of the managed string using the FNV-1a algorithm.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    size_t ToHash() const
    {
        return mLen ? FnvHash(reinterpret_cast< FnvHashData >(mPtr), static_cast< size_t >(mLen) * sizeof(SQChar)) : FnvHashSeed;
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Compute the string hash and cache it into the mRes member.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void CacheHash()
    {
        mRes = static_cast< SQInteger >(ToHash());
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Retrieve the cached string hash.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    size_t GetHash() const
    {
        return static_cast< size_t >(mRes);
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Compute the hash of the managed string, cashe it then retrieve it.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    size_t HashIt()
    {
        CacheHash();
        return GetHash();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Provide a dummy non constant instance when calling functions that can work with placeholders.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    static StackStrF & Dummy()
    {
        static StackStrF o;
        o.Release();
        return o;
    }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Throws the current error in a given VM as a program exception.
///
/// \param vm   Target VM
/// \param keep Whether to clear the error from the VM
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline void ErrorToException(HSQUIRRELVM vm, bool keep = false) {
    // Get the error object on the stack
    sq_getlasterror(vm);
    // See if there's an actual error
    if (sq_gettype(vm, -1) == OT_NULL) {
        sq_poptop(vm); // Pop the object from the stack
        return; // Done here!
    }
    StackStrF s(vm, -1);
    // Attempt to the the object as a string
    if (SQ_FAILED(s.Proc(false))) {
        sq_poptop(vm); // Pop the object from the stack
        throw ::Sqrat::Exception(_SC("Unidentifiable script object used as error."));
    }
    sq_poptop(vm); // Pop the object from the stack
    // Should the error be kept in the VM?
    if (!keep) {
        sq_reseterror(vm);
    }
    // Throw the error message
    throw ::Sqrat::Exception(s.mPtr, s.mLen);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// RAII approach to make sure an instance is deleted regardless of what exceptions are thrown.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template < typename T > struct DeleteGuard
{
private:

    // --------------------------------------------------------------------------------------------
    T * m_Ptr; // Pointer to the instance to manage.

public:

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Default constructor.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    explicit DeleteGuard(T * ptr)
        : m_Ptr(ptr)
    {
        /* ... */
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Copy constructor. (disabled)
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    DeleteGuard(const DeleteGuard & o) = delete;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Move constructor.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    DeleteGuard(DeleteGuard && o) noexcept
        : m_Ptr(o.m_Ptr)
    {
        o.m_Ptr = nullptr;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Destructor.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ~DeleteGuard()
    {
        if (m_Ptr)
        {
            delete m_Ptr;
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Copy assignment operator. (disabled)
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    DeleteGuard & operator = (const DeleteGuard & o) = delete;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Move assignment operator. (disabled)
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    DeleteGuard & operator = (DeleteGuard && o) = delete;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Implicit conversion the managed instance type.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    operator T * () const
    {
        return m_Ptr;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Retrieve the managed instance.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    T * Get() const
    {
        return m_Ptr;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Release the managed instance.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void Release()
    {
        m_Ptr = nullptr;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Retrieve and release the managed instance.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    T * Grab()
    {
        T * ptr = m_Ptr;
        m_Ptr = nullptr;
        return ptr;
    }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Helper structure used to make it easy to track instances of a certain type.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template < typename T > struct SqChainedInstances
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Default constructor.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    SqChainedInstances()
        : m_Prev(nullptr), m_Next(nullptr)
    {
        //...
    }

protected:

    SqChainedInstances *   m_Prev; // Previous instance in the chain.
    SqChainedInstances *   m_Next; // Next instance in the chain.

    static SqChainedInstances * s_Head; // The head of the instance chain.

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Attach the instance to the chain.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void ChainInstance()
    {
        // Is there an existing head?
        if (s_Head == nullptr)
        {
            // There was no existing head
            m_Prev = m_Next = nullptr;
            // We're the head
            s_Head = this;
        }
        // Is there a preceding instance before the current head?
        else if (s_Head->m_Prev == nullptr)
        {
            // Grab the current head as the next instance in the chain
            m_Next = s_Head;
            // Become the new head and the preceding instance of the current head
            m_Next->m_Prev = s_Head = this;
        }
        else
        {
            // Grab the current head as the next instance in the chain
            m_Next = s_Head;
            // Become the new head and the next instance of the preceding instance of the current head
            m_Next->m_Prev->m_Next = s_Head = this;
            // Become the preceding instance of the current head
            m_Next->m_Prev = this;
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Detach the instance from the chain.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void UnchainInstance()
    {
        // Is there an instance after us?
        if (m_Next != nullptr)
        {
            // Link the next instance with the one before us
            m_Next->m_Prev = m_Prev;
            // Are we the current head?
            if (s_Head == this)
            {
                s_Head = m_Next; // Make the next one the head
            }
        }
        // Is there an instance before us?
        if (m_Prev != nullptr)
        {
            // Link the previous instance with the one after us
            m_Prev->m_Next = m_Next;
            // Are we the current head?
            if (s_Head == nullptr || s_Head == this)
            {
                // If there was no instance after us then make the previous one the head
                s_Head = m_Prev;
            }
        }
        // Are we the current and the only head?
        else if (s_Head == this)
        {
            s_Head = nullptr; // No more instances of this type
        }
    }
};

template < typename T > SqChainedInstances< T > * SqChainedInstances< T >::s_Head = nullptr;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @cond DEV
/// Used internally to get and manipulate the underlying type of variables - retrieved from cppreference.com
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> struct remove_const                                                {typedef T type;};
template<class T> struct remove_const<const T>                                       {typedef T type;};
template<class T> struct remove_volatile                                             {typedef T type;};
template<class T> struct remove_volatile<volatile T>                                 {typedef T type;};
template<class T> struct remove_cv                                                   {typedef typename remove_volatile<typename remove_const<T>::type>::type type;};
template<class T> struct is_pointer_helper                                           {static constexpr bool value = false;};
template<class T> struct is_pointer_helper<T*>                                       {static constexpr bool value = true;};
template<class T,class D> struct is_pointer_helper<SharedPtr<T,D> >                            {static constexpr bool value = true;};
template<class T,class D> struct is_pointer_helper<WeakPtr<T,D> >                              {static constexpr bool value = true;};
template<class T> struct is_pointer : is_pointer_helper<typename remove_cv<T>::type> {};
template<class T> struct is_reference                                                {static constexpr bool value = false;};
template<class T> struct is_reference<T&>                                            {static constexpr bool value = true;};
/// @endcond


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fast integer to string implementation to avoid using itoa. See https://github.com/jeaiii/itoa
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 2 chars at a time, little endian only, unaligned 2 byte writes
static constexpr uint16_t S100s[] = {
//  '00',   '10',   '20',   '30',   '40',   '50',   '60',   '70',   '80',   '90',
    0x3030, 0x3130, 0x3230, 0x3330, 0x3430, 0x3530, 0x3630, 0x3730, 0x3830, 0x3930,
//  '01',   '11',   '21',   '31',   '41',   '51',   '61',   '71',   '81',   '91',
    0x3031, 0x3131, 0x3231, 0x3331, 0x3431, 0x3531, 0x3631, 0x3731, 0x3831, 0x3931,
//  '02',   '12',   '22',   '32',   '42',   '52',   '62',   '72',   '82',   '92',
    0x3032, 0x3132, 0x3232, 0x3332, 0x3432, 0x3532, 0x3632, 0x3732, 0x3832, 0x3932,
//  '03',   '13',   '23',   '33',   '43',   '53',   '63',   '73',   '83',   '93',
    0x3033, 0x3133, 0x3233, 0x3333, 0x3433, 0x3533, 0x3633, 0x3733, 0x3833, 0x3933,
//  '04',   '14',   '24',   '34',   '44',   '54',   '64',   '74',   '84',   '94',
    0x3034, 0x3134, 0x3234, 0x3334, 0x3434, 0x3534, 0x3634, 0x3734, 0x3834, 0x3934,
//  '05',   '15',   '25',   '35',   '45',   '55',   '65',   '75',   '85',   '95',
    0x3035, 0x3135, 0x3235, 0x3335, 0x3435, 0x3535, 0x3635, 0x3735, 0x3835, 0x3935,
//  '06',   '16',   '26',   '36',   '46',   '56',   '66',   '76',   '86',   '96',
    0x3036, 0x3136, 0x3236, 0x3336, 0x3436, 0x3536, 0x3636, 0x3736, 0x3836, 0x3936,
//  '07',   '17',   '27',   '37',   '47',   '57',   '67',   '77',   '87',   '97',
    0x3037, 0x3137, 0x3237, 0x3337, 0x3437, 0x3537, 0x3637, 0x3737, 0x3837, 0x3937,
//  '08',   '18',   '28',   '38',   '48',   '58',   '68',   '78',   '88',   '98',
    0x3038, 0x3138, 0x3238, 0x3338, 0x3438, 0x3538, 0x3638, 0x3738, 0x3838, 0x3938,
//  '09',   '19',   '29',   '39',   '49',   '59',   '69',   '79',   '89',   '99',
    0x3039, 0x3139, 0x3239, 0x3339, 0x3439, 0x3539, 0x3639, 0x3739, 0x3839, 0x3939,
};

// /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define A(N) t = (1ULL << (32 + N / 5 * N * 53 / 16)) / static_cast< uint32_t >(1e##N) + 1 - N / 9, t *= u, t >>= N / 5 * N * 53 / 16, t += N / 5 * 4

// --------------------------------------------------------------------------------------------------------------------
#define W(N, I) *reinterpret_cast< uint16_t * >(&b[N]) = S100s[I]
#define Q(N) b[N] = static_cast< char >((10ULL * static_cast< uint32_t >(t)) >> 32) + '0'
#define D(N) W(N, t >> 32)
#define E t = 100ULL * static_cast< uint32_t >(t)

// --------------------------------------------------------------------------------------------------------------------
#define L0 b[0] = static_cast< char >(u) + '0'
#define L1 W(0, u)
#define L2 A(1), D(0), Q(2)
#define L3 A(2), D(0), E, D(2)
#define L4 A(3), D(0), E, D(2), Q(4)
#define L5 A(4), D(0), E, D(2), E, D(4)
#define L6 A(5), D(0), E, D(2), E, D(4), Q(6)
#define L7 A(6), D(0), E, D(2), E, D(4), E, D(6)
#define L8 A(7), D(0), E, D(2), E, D(4), E, D(6), Q(8)
#define L9 A(8), D(0), E, D(2), E, D(4), E, D(6), E, D(8)

// --------------------------------------------------------------------------------------------------------------------
#define LN(N) (L##N, b += N + 1, l += N + 1)
#define LZ(N) (L##N, l += N + 1)
#define LG(F) (u<100 ? u<10 ? F(0) : F(1) : u<1000000U ? u<10000 ? u<1000 ? F(2) : F(3) : u<100000 ? F(4) : F(5) : u<100000000 ? u<10000000 ? F(6) : F(7) : u<1000000000 ? F(8) : F(9))

// /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Convert a 32-bit binary unsigned integral value to its ASCII string representation.
/// Returns the number of characters written to the given memory buffer.
inline unsigned U32ToA(uint32_t u, char * b) noexcept
{
    uint32_t l = 0;
    uint64_t t;
    (void)LG(LZ);
    return l;
}
/// Convert the given value into the given buffer, including the null character.
inline unsigned U32ToA_(uint32_t n, char * b) noexcept
{
    // Reuse the function the omits the null character and store the result
    unsigned l = U32ToA(n, b);
    // Append the null character and return the previously stored result
    return b[l] = '\0', l;
}

// /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Convert a 32-bit binary signed integral value to its ASCII string representation.
/// Returns the number of characters written to the given memory buffer.
inline unsigned I32ToA(int32_t n, char * b) noexcept
{
    // Transform negative signed integers into positive unsigned integers and write the '-' symbol
    uint32_t v = n < 0 ? *(b++) = '-', 0 - static_cast< uint32_t >(n) : n;
    // Append the null character and return the previously stored result
    return U32ToA(v, b);
}
/// Convert the given value into the given buffer, including the null character.
inline unsigned I32ToA_(int32_t n, char * b) noexcept
{
    // Reuse the function the omits the null character and store the result
    unsigned l = I32ToA(n, b);
    // Append the null character and return the previously stored result
    return b[l] = '\0', l;
}

// /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Convert a 64-bit binary unsigned integral value to its ASCII string representation.
/// Returns the number of characters written to the given memory buffer.
inline unsigned U64ToA(uint64_t n, char * b) noexcept
{
    // https://github.com/jeaiii/itoa

    uint32_t u, l = 0;
    uint64_t t;

    if (static_cast< uint32_t >(n >> 32) == 0)
    {
        u = static_cast< uint32_t >(n), (void)LG(LZ);
        return l;
    }

    uint64_t a = n / 100000000LLU;

    if (static_cast< uint32_t >(a >> 32) == 0)
    {
        u = static_cast< uint32_t >(a);
        LG(LN);
    }
    else
    {
        u = static_cast< uint32_t >(a / 100000000LLU);
        LG(LN);
        u = a % 100000000LLU;
        LN(7);
    }

    u = n % 100000000LLU;
    LZ(7);

    return l;
}
/// Convert the given value into the given buffer, including the null character.
inline unsigned U64ToA_(uint64_t n, char * b) noexcept
{
    // Reuse the function the omits the null character and store the result
    unsigned l = U64ToA(n, b);
    // Append the null character and return the previously stored result
    return b[l] = '\0', l;
}

// /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Convert a 64-bit binary signed integral value to its ASCII string representation.
/// Returns the number of characters written to the given memory buffer.
inline unsigned I64ToA(int64_t v, char * b) noexcept
{
        // Transform negative signed integers into positive unsigned integers and write the '-' symbol
        uint64_t n = v < 0 ? *(b++) = '-', 0 - static_cast< uint64_t >(v) : v;
        // Forward the call to the actual implementation and return the result
        return U64ToA(n, b);
}
/// Convert the given value into the given buffer, including the null character.
inline unsigned I64ToA_(int64_t n, char * b) noexcept
{
    // Reuse the function the omits the null character and store the result
    unsigned l = I64ToA(n, b);
    // Append the null character and return the previously stored result
    return b[l] = '\0', l;
}

// /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#undef A

// --------------------------------------------------------------------------------------------------------------------
#undef W
#undef Q
#undef D
#undef E

// --------------------------------------------------------------------------------------------------------------------
#undef L0
#undef L1
#undef L2
#undef L3
#undef L4
#undef L5
#undef L6
#undef L7
#undef L8
#undef L9

// --------------------------------------------------------------------------------------------------------------------
#undef LN
#undef LZ
#undef LG

}

#endif
