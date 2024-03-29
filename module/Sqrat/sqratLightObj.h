//
// SqratObject: Referenced Squirrel Object Wrapper
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

#pragma once

#include <squirrelex.h>

#include <cstring>

#include "sqratAllocator.h"
#include "sqratTypes.h"
#include "sqratUtil.h"

namespace Sqrat {

class Object;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// A lightweight wrapper arround the Squirrel objects that implements RAII and still remains a POD type.
///
/// \remarks
/// All LightObj and derived classes MUST be destroyed before calling sq_close or your application will crash when exiting.
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct LightObj {

    HSQOBJECT mObj;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Default constructor (null)
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    LightObj() {
        sq_resetobject(&mObj);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Copy constructor
    ///
    /// \param so LightObj to copy
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    LightObj(const LightObj& so) : mObj(so.mObj) {
        sq_addref(SqVM(), &mObj);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Move constructor
    ///
    /// \param so LightObj to move
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    LightObj(LightObj&& so) noexcept : mObj(so.mObj) {
        sq_resetobject(&so.mObj);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Constructs a LightObj from a Squirrel object
    ///
    /// \param o Squirrel object
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    explicit LightObj(const HSQOBJECT & o) : mObj(o) {
        sq_addref(SqVM(), &mObj);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Constructs a LightObj from a string object
    ///
    /// \param o Squirrel object
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    explicit LightObj(const string & s) : LightObj(s.c_str(), static_cast< SQInteger >(s.size())) {
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Constructs a LightObj from a Squirrel object at a certain index on the stack
    ///
    /// \param i Index of the Squirrel object on stack
    /// \param v VM that the object will exist in
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    explicit LightObj(SQInteger i, HSQUIRRELVM v = SqVM()) {
        if (SQ_FAILED(sq_getstackobj(v, i, &mObj))) {
            sq_resetobject(&mObj);
        } else {
            sq_addref(v, &mObj);
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Constructs a LightObj from a string
    ///
    /// \param i The string itself
    /// \param i The length of the string
    /// \param v VM that the object will exist in
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    explicit LightObj(const SQChar * s, SQInteger l=-1, HSQUIRRELVM v = SqVM()) {
        sq_pushstring(v, s, l);
        if (SQ_FAILED(sq_getstackobj(v, -1, &mObj))) {
            sq_resetobject(&mObj);
        } else {
            sq_addref(v, &mObj);
        }
        sq_pop(v, 1);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Construct a LightObj from a regular Object instance.
    ///
    /// \param so Object to copy
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    explicit LightObj(const Object& obj);

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Constructs a LightObj from a C++ instance
    ///
    /// \param instance Pointer to a C++ class instance that has been bound already
    /// \param v        VM that the object will exist in
    ///
    /// \tparam T Type of instance
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    template<class T>
    explicit LightObj(T* instance, HSQUIRRELVM vm = SqVM()) {
        // Preserve the stack state
        const StackGuard sg(vm);
        // Push the instance on the stack
        ClassType<T>::PushInstance(vm, instance);
        // Attempt to retrieve it
        if (SQ_FAILED(sq_getstackobj(vm, -1, &mObj))) {
            sq_resetobject(&mObj);
        } else {
            sq_addref(vm, &mObj);
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Constructs a LightObj from a C++ instance only if that instance exists. Otherwise, null is beings used.
    ///
    /// \param instance Pointer to a C++ class instance that has been bound already
    /// \param v        VM that the object will exist in
    ///
    /// \tparam T Type of instance
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    template<class T>
    explicit LightObj(T* instance, std::nullptr_t, HSQUIRRELVM vm = SqVM()) {
        // Preserve the stack state
        const StackGuard sg(vm);
        // Push the instance on the stack
        ClassType<T>::PushInstance(vm, instance, nullptr);
        // Attempt to retrieve it
        if (SQ_FAILED(sq_getstackobj(vm, -1, &mObj))) {
            sq_resetobject(&mObj);
        } else {
            sq_addref(vm, &mObj);
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Constructs a LightObj from a C++ type
    ///
    /// \param t        Identity of the type to create.
    /// \param v        VM that the object will exist in.
    /// \param a        Arguments to forward to the type constructor.
    ///
    /// \tparam T Type of instance
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    template<class T, class... A>
    LightObj(SqTypeIdentity< T > SQ_UNUSED_ARG(t), HSQUIRRELVM vm, A&&... a) : LightObj(DeleteGuard< T >(new T(std::forward< A >(a)...))) {
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Constructs a LightObj from a C++ type
    ///
    /// \param t        Identity of the type to create.
    /// \param v        VM that the object will exist in.
    /// \param a        Arguments to forward to the type constructor.
    ///
    /// \tparam T Type of instance
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    template<class T>
    LightObj(SqInPlace SQ_UNUSED_ARG(t), HSQUIRRELVM vm, T value) {
        // Push the value on the stack
        Var<T>::push(vm, std::forward< T >(value));
        // Attempt to retrieve it
        if (SQ_FAILED(sq_getstackobj(vm, -1, &mObj))) {
            sq_resetobject(&mObj);
        } else {
            sq_addref(vm, &mObj);
        }
        // Pop the value from the stack
        sq_poptop(vm);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Constructs an LightObj from a C++ instance wrapped inside a DeleteGuard
    ///
    /// \param instance Pointer to a C++ class instance that has been bound already
    /// \param v        VM that the object will exist in
    ///
    /// \tparam T Type of instance
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    template<class T>
    LightObj(DeleteGuard<T> && guard, HSQUIRRELVM v = SqVM()) : LightObj(guard.Get(), v) {
        guard.Release();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Constructs an LightObj from a C++ instance wrapped inside a DeleteGuard
    ///
    /// \param instance Pointer to a C++ class instance that has been bound already
    /// \param v        VM that the object will exist in
    ///
    /// \tparam T Type of instance
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    template<class T>
    LightObj(DeleteGuard<T> & guard, HSQUIRRELVM v = SqVM()) : LightObj(guard.Get(), v) {
        guard.Release();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Destructor
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ~LightObj() {
        Release();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Assignment operator
    ///
    /// \param so LightObj to copy
    ///
    /// \return The LightObj itself
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    LightObj& operator=(const LightObj& so) {
        if (this != &so) {
            Release();
            mObj = so.mObj;
            sq_addref(SqVM(), &mObj);
        }
        return *this;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Assignment operator
    ///
    /// \param so LightObj to move
    ///
    /// \return The LightObj itself
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    LightObj& operator=(LightObj&& so) noexcept {
        if (this != &so) {
            Release();
            mObj = so.mObj;
            sq_resetobject(&so.mObj);
        }
        return *this;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Discards obj member value without releasing the reference. Only present for compatibility with Object type.
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void ResetObj() {
        sq_resetobject(&mObj);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Discards any member values without releasing any references. Only present for compatibility with Object type.
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void Reset() {
        sq_resetobject(&mObj);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Gets the type of the LightObj as defined by the Squirrel API
    ///
    /// \return SQObjectType for the LightObj
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    SQObjectType GetType() const {
        return mObj._type;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Checks whether the LightObj is null
    ///
    /// \return True if the LightObj currently has a null value, otherwise false
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    bool IsNull() const {
        return sq_isnull(mObj);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Gets the Squirrel object for this LightObj (copy)
    ///
    /// \return Squirrel object
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    HSQOBJECT GetObj() const {
        return mObj;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Gets the Squirrel object for this LightObj (reference)
    ///
    /// \return Squirrel object
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    HSQOBJECT& GetObj() {
        return mObj;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Allows the LightObj to be inputted directly into places that expect a HSQOBJECT
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    operator HSQOBJECT&() {
        return mObj;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Gets the Squirrel VM for this Object (copy)
    ///
    /// \return Squirrel VM associated with the Object
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    inline HSQUIRRELVM GetVM() const {
        return SqVM();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Sets the LightObj to null (removing its references to underlying Squirrel objects)
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void Release() {
        // Should we release any object?
        if (!sq_isnull(mObj)) {
            sq_release(SqVM(), &mObj);
            sq_resetobject(&mObj);
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Retrieves the number of references that exist to the managed object
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    SQUnsignedInteger RefCount() {
        return sq_getrefcount(SqVM(), &mObj);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Retrieves type-tag of the managed object
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    SQUserPointer GetTypeTag() {
        SQUserPointer typeTag;
        sq_pushobject(SqVM(), GetObj());
        sq_gettypetag(SqVM(), -1, &typeTag);
        sq_pop(SqVM(), 1);
        return typeTag;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Binds a Table or Class to the object (can be used to facilitate namespaces)
    ///
    /// \param name The key in the table being assigned a Table or Class
    /// \param obj  Table or Class that is being placed in the table
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void Bind(const SQChar* name, LightObj& obj) {
        HSQUIRRELVM vm = SqVM();
        sq_pushobject(vm, mObj);
        sq_pushstring(vm, name, -1);
        sq_pushobject(vm, obj.mObj);
        sq_newslot(vm, -3, false);
        sq_pop(vm,1); // pop table
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Casts the object to a certain C++ type
    ///
    /// \tparam T Type to cast to
    ///
    /// \return A copy of the value of the Object with the given type
    ///
    /// \remarks
    /// This function MUST have its Error handled if it occurred.
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    template <class T>
    T Cast() const {
        HSQUIRRELVM vm = SqVM();
        sq_pushobject(vm, mObj);
        Var<T> v(vm, -1);
        sq_pop(vm, 1);
        return std::move(v.value);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Casts the object to a certain C++ type instance
    ///
    /// \tparam T Type to cast to
    ///
    /// \return A pointer to the value of the Object with the given type
    ///
    /// \remarks
    /// This function MUST have its Error handled if it occurred.
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    template <class T>
    T * CastI() const {
        HSQUIRRELVM vm = SqVM();
        sq_pushobject(vm, mObj);
        Var<T *> v(vm, -1);
        sq_pop(vm, 1);
        return v.value;
    }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Used to get and push LightObj instances to and from the stack as references (LightObj is always a reference)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<>
struct Var<LightObj> {

    LightObj value; ///< The actual value of get operations

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Attempts to get the value off the stack at idx as an LightObj
    ///
    /// \param vm  Target VM
    /// \param idx Index trying to be read
    ///
    /// \remarks
    /// This function MUST have its Error handled if it occurred.
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Var(HSQUIRRELVM vm, SQInteger idx) : value(idx, vm) {
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Called by Sqrat::PushVar to put an LightObj on the stack
    ///
    /// \param vm    Target VM
    /// \param value Value to push on to the VM's stack
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    static void push(HSQUIRRELVM vm, const LightObj& value) {
        sq_pushobject(vm, value.mObj);
    }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Used to get and push LightObj instances to and from the stack as references (LightObj is always a reference)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<>
struct Var<LightObj&> : Var<LightObj> {Var(HSQUIRRELVM vm, SQInteger idx) : Var<LightObj>(vm, idx) {}};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Used to get and push LightObj instances to and from the stack as references (LightObj is always a reference)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<>
struct Var<const LightObj&> : Var<LightObj> {Var(HSQUIRRELVM vm, SQInteger idx) : Var<LightObj>(vm, idx) {}};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Create a script object from the specified value on the default VM.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template < typename T > LightObj MakeLightObj(const T & v)
{
    // Remember the current stack size
    const StackGuard sg;
    // Transform the specified value into a script object
    PushVar< T >(SqVM(), v);
    // Get the object from the stack and return it
    return Var< LightObj >(SqVM(), -1).value;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Create a script object from the specified value on the specified VM.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template < typename T > LightObj MakeLightObj(HSQUIRRELVM vm, const T & v)
{
    // Remember the current stack size
    const StackGuard sg;
    // Transform the specified value into a script object
    PushVar< T >(vm, v);
    // Get the object from the stack and return it
    return Var< LightObj >(vm, -1).value;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline StackStrF::StackStrF(HSQUIRRELVM vm, LightObj & o)
    : StackStrF(vm, -1)
{
    sq_pushobject(vm, o.mObj);
    // Process the object on the stack
    Proc(false);
    // Restore stack
    sq_pop(vm, 1);
}

}
