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
#include "sqratOverloadMethods.h"
#include "sqratUtil.h"
#include "sqratLightObj.h"

namespace Sqrat {

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// The base class for classes that represent Squirrel objects
///
/// \remarks
/// All Object and derived classes MUST be destroyed before calling sq_close or your application will crash when exiting.
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Object {
protected:

/// @cond DEV
    HSQOBJECT mObj;
    bool mRelease;

    explicit Object(bool releaseOnDestroy) : mRelease(releaseOnDestroy) {
        sq_resetobject(&mObj);
    }
/// @endcond

public:

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Default constructor (null)
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Object() : Object(true) { }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Copy constructor
    ///
    /// \param so Object to copy
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Object(const Object& so) : mObj(so.mObj), mRelease(so.mRelease) {
        sq_addref(SqVM(), &mObj);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Move constructor
    ///
    /// \param so Object to move
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Object(Object&& so) noexcept : mObj(so.mObj), mRelease(so.mRelease) {
        so.ResetObj();
        so.mRelease = false;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Light object copy constructor
    ///
    /// \param so Object to copy
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Object(const LightObj& so) : mObj(so.mObj), mRelease(!so.IsNull()) {
        sq_addref(SqVM(), &mObj);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Light object copy constructor
    ///
    /// \param so Object to move
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Object(LightObj&& so) noexcept : mObj(so.mObj), mRelease(!so.IsNull()) {
        so.Reset();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Constructs an Object from a Squirrel object
    ///
    /// \param o Squirrel object
    /// \param v VM that the object will exist in
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Object(HSQOBJECT o, HSQUIRRELVM vm = SqVM()) : mObj(o), mRelease(!sq_isnull(o)) {
        sq_addref(vm, &mObj);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Constructs an Object from a Squirrel object at a certain index on the stack
    ///
    /// \param i Index of the Squirrel object on stack
    /// \param v VM that the object will exist in
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Object(SQInteger i, HSQUIRRELVM vm = SqVM()) : mRelease(true) {
        if (SQ_FAILED(sq_getstackobj(vm, i, &mObj))) {
            sq_resetobject(&mObj);
            mRelease = false;
        } else {
            sq_addref(vm, &mObj);
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Constructs an Object from a C++ instance
    ///
    /// \param instance Pointer to a C++ class instance that has been bound already
    /// \param v        VM that the object will exist in
    ///
    /// \tparam T Type of instance
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    template<class T>
    Object(T* instance, HSQUIRRELVM vm = SqVM()) : mRelease(true) {
        // Preserve the stack state
        const StackGuard sg(vm);
        // Push the instance on the stack
        ClassType<T>::PushInstance(vm, instance);
        // Attempt to retrieve it
        if (SQ_FAILED(sq_getstackobj(vm, -1, &mObj))) {
            sq_resetobject(&mObj);
            // nothing to release anymore
            mRelease = false;
        } else {
            sq_addref(vm, &mObj);
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Constructs an Object from a C++ type
    ///
    /// \param t        Identity of the type to create.
    /// \param v        VM that the object will exist in.
    /// \param a        Arguments to forward to the type constructor.
    ///
    /// \tparam T Type of instance
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    template<class T, class... A>
    Object(SqTypeIdentity< T > SQ_UNUSED_ARG(t), HSQUIRRELVM v, A&&... a) : mRelease(true) {
        HSQUIRRELVM vm = SqVM();
        // Create the instance and guard it to make sure it gets deleted in case of exceptions
        DeleteGuard< T > instance(new T(std::forward< A >(a)...));
        // Preserve the stack state
        const StackGuard sg(vm);
        // Push the instance on the stack
        ClassType<T>::PushInstance(vm, instance);
        // Attempt to retrieve it
        if (SQ_FAILED(sq_getstackobj(vm, -1, &mObj))) {
            sq_resetobject(&mObj);
            // nothing to release anymore
            mRelease = false;
        } else {
            sq_addref(vm, &mObj);
        }
        // Stop guarding the instance
        instance.Release();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Destructor
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    virtual ~Object() {
        if(mRelease) {
            Release();
            mRelease = false;
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Assignment operator
    ///
    /// \param so Object to copy
    ///
    /// \return The Object itself
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Object& operator=(const Object& so) {
        if(mRelease) {
            Release();
        }
        mObj = so.mObj;
        mRelease = so.mRelease;
        sq_addref(SqVM(), &GetObj());
        return *this;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Assignment operator
    ///
    /// \param so Object to move
    ///
    /// \return The Object itself
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Object& operator=(Object&& so) noexcept {
        if(mRelease) {
            Release();
        }
        mObj = so.mObj;
        mRelease = so.mRelease;
        so.Reset();
        return *this;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Assignment operator
    ///
    /// \param so LightObj to copy
    ///
    /// \return The Object itself
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Object& operator=(const LightObj& so) {
        if(mRelease) {
            Release();
        }
        mObj = so.mObj;
        mRelease = !so.IsNull();
        sq_addref(SqVM(), &GetObj());
        return *this;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Assignment operator
    ///
    /// \param so LightObj to move
    ///
    /// \return The Object itself
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Object& operator=(LightObj&& so) noexcept {
        if(mRelease) {
            Release();
        }
        mObj = so.mObj;
        mRelease = !so.IsNull();
        so.Reset();
        return *this;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Discards obj member value without releasing the reference.
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void ResetObj() {
        sq_resetobject(&GetObj());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Discards any member values without releasing any references.
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void Reset() {
        sq_resetobject(&GetObj());
        mRelease = false;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Gets the Squirrel VM for this Object (copy)
    ///
    /// \return Squirrel VM associated with the Object
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    HSQUIRRELVM GetVM() const {
        return SqVM();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Gets the type of the Object as defined by the Squirrel API
    ///
    /// \return SQObjectType for the Object
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    SQObjectType GetType() const {
        return GetObj()._type;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Checks whether the Object is null
    ///
    /// \return True if the Object currently has a null value, otherwise false
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    bool IsNull() const {
        return sq_isnull(GetObj());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Gets the Squirrel object for this Object (copy)
    ///
    /// \return Squirrel object
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    virtual HSQOBJECT GetObj() const {
        return mObj;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Gets the Squirrel object for this Object (reference)
    ///
    /// \return Squirrel object
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    virtual HSQOBJECT& GetObj() {
        return mObj;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Allows the Object to be inputted directly into places that expect a HSQOBJECT
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    operator HSQOBJECT&() {
        return GetObj();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Sets the Object to null (removing its references to underlying Squirrel objects)
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void Release() {
        if (!sq_isnull(mObj))
        {
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
    /// Retrieves the type-tag of the managed object
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    AbstractStaticClassData * GetTypeTag() const {
        AbstractStaticClassData* typeTag;
        sq_pushobject(SqVM(), GetObj());
        sq_gettypetag(SqVM(), -1, (SQUserPointer*)&typeTag);
        sq_pop(SqVM(), 1);
        return typeTag;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Attempts to get the value of a slot from the object
    ///
    /// \param slot Name of the slot
    ///
    /// \return An Object representing the value of the slot (can be a null object if nothing was found)
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Object GetSlot(const SQChar* slot) const {
        HSQUIRRELVM vm = SqVM();
        HSQOBJECT slotObj;
        sq_pushobject(vm, GetObj());
        sq_pushstring(vm, slot, -1);

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if(SQ_FAILED(sq_get(vm, -2))) {
            sq_pop(vm, 1);
            return Object{}; // Return a nullptr object
        } else {
            sq_getstackobj(vm, -1, &slotObj);
            Object ret(slotObj, vm); // must addref before the pop!
            sq_pop(vm, 2);
            return ret;
        }
#else
        sq_get(vm, -2);
        sq_getstackobj(vm, -1, &slotObj);
        Object ret(slotObj, vm); // must addref before the pop!
        sq_pop(vm, 2);
        return ret;
#endif
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Attempts to get the value of an index from the object
    ///
    /// \param index Index of the slot
    ///
    /// \return An Object representing the value of the slot (can be a null object if nothing was found)
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Object GetSlot(SQInteger index) const {
        HSQUIRRELVM vm = SqVM();
        HSQOBJECT slotObj;
        sq_pushobject(vm, GetObj());
        sq_pushinteger(vm, index);

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if(SQ_FAILED(sq_get(vm, -2))) {
            sq_pop(vm, 1);
            return Object{}; // Return a nullptr object
        } else {
            sq_getstackobj(vm, -1, &slotObj);
            Object ret(slotObj, vm); // must addref before the pop!
            sq_pop(vm, 2);
            return ret;
        }
#else
        sq_get(vm, -2);
        sq_getstackobj(vm, -1, &slotObj);
        Object ret(slotObj, vm); // must addref before the pop!
        sq_pop(vm, 2);
        return ret;
#endif
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Checks if the object has a slot with a specified key
    ///
    /// \param key Name of the key
    ///
    /// \return True if the Object has a value associated with key, otherwise false
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    bool HasKey(const SQChar* key) const {
        HSQUIRRELVM vm = SqVM();
        sq_pushobject(vm, GetObj());
        sq_pushstring(vm, key, -1);
        if (SQ_FAILED(sq_get(vm, -2))) {
            sq_pop(vm, 1);
            return false;
        }
        sq_pop(vm, 2);
        return true;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Checks if the object has a slot with a specified index
    ///
    /// \param index Index to check
    ///
    /// \return True if the Object has a value associated with index, otherwise false
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    bool HasKey(SQInteger index) const {
        HSQUIRRELVM vm = SqVM();
        sq_pushobject(vm, GetObj());
        sq_pushinteger(vm, index);
        if (SQ_FAILED(sq_get(vm, -2))) {
            sq_pop(vm, 1);
            return false;
        }
        sq_pop(vm, 2);
        return true;
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
        sq_pushobject(vm, GetObj());
        T ret = Var<T>(vm, -1).value;
        sq_pop(vm, 1);
        return ret;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Allows Object to be used like C++ arrays with the [] operator
    ///
    /// \param slot The slot key
    ///
    /// \tparam T Type of the slot key (usually doesnt need to be defined explicitly)
    ///
    /// \return An Object representing the value of the slot (can be a null object if nothing was found)
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    template <class T>
    inline Object operator[](T slot)
    {
        return GetSlot(slot);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Returns the size of the Object
    ///
    /// \return Size of Object
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    SQInteger GetSize() const {
        HSQUIRRELVM vm = SqVM();
        sq_pushobject(vm, GetObj());
        SQInteger ret = sq_getsize(vm, -1);
        sq_pop(vm, 1);
        return ret;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Iterator for going over the slots in the object using Object::Next
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    struct iterator
    {
        /// @cond DEV
        friend class Object;
        /// @endcond

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// Default constructor (null)
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        iterator()
        {
            Index = 0;
            sq_resetobject(&Key);
            sq_resetobject(&Value);
        }

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// Returns the string value of the key the iterator is on if possible
        ///
        /// \return String or nullptr
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        const SQChar* getName() { return sq_objtostring(&Key); }

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// Gets the Squirrel object for the key the iterator is on
        ///
        /// \return HSQOBJECT representing a key
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        HSQOBJECT getKey() { return Key; }

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// Gets the Squirrel object for the value the iterator is on
        ///
        /// \return HSQOBJECT representing a value
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        HSQOBJECT getValue() { return Value; }
    private:

        HSQOBJECT Key;
        HSQOBJECT Value;
        SQInteger Index;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Used to go through all the slots in an Object (same limitations as sq_next)
    ///
    /// \param iter An iterator being used for going through the slots
    ///
    /// \return Whether there is a next slot
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    bool Next(iterator& iter) const
    {
        HSQUIRRELVM vm = SqVM();
        sq_pushobject(vm, mObj);
        sq_pushinteger(vm,iter.Index);
        if(SQ_SUCCEEDED(sq_next(vm,-2)))
        {
            sq_getstackobj(vm,-1,&iter.Value);
            sq_getstackobj(vm,-2,&iter.Key);
            sq_getinteger(vm,-3,&iter.Index);
            sq_pop(vm,4);
            return true;
        }
        else
        {
            sq_pop(vm,2);
            return false;
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///  Used to go through all the slots in an Object (same limitations as sq_next)
    ///
    /// \param func Functor that is continuously called to process values on the stack
    ///
    /// \tparam F Type of functor (usually doesnt need to be defined explicitly)
    ///
    /// \return Nothing
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    template<class F>
    SQRESULT Foreach(F&& func) const
    {
        HSQUIRRELVM vm = SqVM();
        const StackGuard sg(vm);
        sq_pushobject(vm, mObj);
        sq_pushnull(vm);
        SQRESULT res = SQ_OK;
        for(SQInteger i = 0; SQ_SUCCEEDED(sq_next(vm,-2)); ++i)
        {
            res = func(vm, i);
            if (SQ_FAILED(res))
            {
                break;
            }
            sq_pop(vm,2);
        }
        return res;
    }

protected:
/// @cond DEV

    // Bind a function and it's associated Squirrel closure to the object
    inline void BindFunc(const SQChar* name, void* method, size_t methodSize, SQFUNCTION func, bool staticVar = false) {
        HSQUIRRELVM vm = SqVM();
        // Push object/environment
        sq_pushobject(vm, GetObj());
        // Push name where the closure will be stored
        sq_pushstring(vm, name, -1);
        // Push the native closure pointer as a free variable
        SQUserPointer methodPtr = sq_newuserdata(vm, static_cast<SQUnsignedInteger>(methodSize));
        memcpy(methodPtr, method, methodSize);
        // Create the native closure
        sq_newclosure(vm, func, 1);
        // Set the closure name (for debug purposes)
        sq_setnativeclosurename(vm, -1, name);
        // Include it into the object
        sq_newslot(vm, -3, staticVar);
        // pop object/environment
        sq_pop(vm,1);
    }

    inline void BindFunc(const SQInteger index, void* method, size_t methodSize, SQFUNCTION func, bool staticVar = false, const SQChar* name = nullptr) {
        HSQUIRRELVM vm = SqVM();
        // Push object/environment
        sq_pushobject(vm, GetObj());
        // Push index where the closure will be stored
        sq_pushinteger(vm, index);
        // Push the native closure pointer as a free variable
        SQUserPointer methodPtr = sq_newuserdata(vm, static_cast<SQUnsignedInteger>(methodSize));
        memcpy(methodPtr, method, methodSize);
        // Create the native closure
        sq_newclosure(vm, func, 1);
        // Set the closure name (for debug purposes)
        if (name) sq_setnativeclosurename(vm, -1, name);
        // Include it into the object
        sq_newslot(vm, -3, staticVar);
        // pop object/environment
        sq_pop(vm,1);
    }


    // Bind a function and it's associated Squirrel closure to the object
    inline void BindOverload(const SQChar* name, void* method, size_t methodSize, SQFUNCTION func, SQFUNCTION overload, int argCount, bool staticVar = false) {
        HSQUIRRELVM vm = SqVM();
        string overloadName;
        overloadName.reserve(15);
        SqOverloadName::Get(name, argCount, overloadName);
        // Push object/environment
        sq_pushobject(vm, GetObj());

        // Bind overload handler
        sq_pushstring(vm, name, -1);
        // function name is passed as a free variable
        //sq_pushstring(vm, name, -1);
        sq_push(vm, -1); // <- Let's cheat(?) by pushing the same object
        sq_newclosure(vm, overload, 1);
        // Set the closure name (for debug purposes)
        sq_setnativeclosurename(vm, -1, name);
        // Include it into the object
        sq_newslot(vm, -3, staticVar);

        // Bind overloaded function
        sq_pushstring(vm, overloadName.c_str(), static_cast<SQInteger>(overloadName.size()));
        // Push the native closure pointer as a free variable
        SQUserPointer methodPtr = sq_newuserdata(vm, static_cast<SQUnsignedInteger>(methodSize));
        memcpy(methodPtr, method, methodSize);
        sq_newclosure(vm, func, 1);
        // Set the closure name (for debug purposes)
        sq_setnativeclosurename(vm, -1, overloadName.c_str());
        // Include it into the object
        sq_newslot(vm, -3, staticVar);
        // pop object/environment
        sq_pop(vm, 1);
    }

    // Set the value of a variable on the object. Changes to values set this way are not reciprocated
    template<class V>
    inline void BindValue(const SQChar* name, const V& val, bool staticVar = false) {
        HSQUIRRELVM vm = SqVM();
        sq_pushobject(vm, GetObj());
        sq_pushstring(vm, name, -1);
        PushVar(vm, val);
        sq_newslot(vm, -3, staticVar);
        sq_pop(vm,1); // pop table
    }
    template<class V>
    inline void BindValue(const SQInteger index, const V& val, bool staticVar = false) {
        HSQUIRRELVM vm = SqVM();
        sq_pushobject(vm, GetObj());
        sq_pushinteger(vm, index);
        PushVar(vm, val);
        sq_newslot(vm, -3, staticVar);
        sq_pop(vm,1); // pop table
    }

    // Set the value of an instance on the object. Changes to values set this way are reciprocated back to the source instance
    template<class V>
    inline void BindInstance(const SQChar* name, V* val, bool staticVar = false) {
        HSQUIRRELVM vm = SqVM();
        sq_pushobject(vm, GetObj());
        sq_pushstring(vm, name, -1);
        PushVar(vm, val);
        sq_newslot(vm, -3, staticVar);
        sq_pop(vm,1); // pop table
    }
    template<class V>
    inline void BindInstance(const SQInteger index, V* val, bool staticVar = false) {
        HSQUIRRELVM vm = SqVM();
        sq_pushobject(vm, GetObj());
        sq_pushinteger(vm, index);
        PushVar(vm, val);
        sq_newslot(vm, -3, staticVar);
        sq_pop(vm,1); // pop table
    }

/// @endcond
};

/// @cond DEV
template<>
inline void Object::BindValue<int>(const SQChar* name, const int & val, bool staticVar /* = false */) {
        HSQUIRRELVM vm = SqVM();
    sq_pushobject(vm, GetObj());
    sq_pushstring(vm, name, -1);
    PushVar<int>(vm, val);
    sq_newslot(vm, -3, staticVar);
    sq_pop(vm,1); // pop table
}
/// @endcond

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline LightObj::LightObj(const Object& obj) : mObj(obj.GetObj()) {
    if (!sq_isnull(mObj)) {
        sq_addref(obj.GetVM(), &mObj);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Used to get and push Object instances to and from the stack as references (Object is always a reference)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<>
struct Var<Object> {

    Object value; ///< The actual value of get operations

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Attempts to get the value off the stack at idx as an Object
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
    /// Called by Sqrat::PushVar to put an Object on the stack
    ///
    /// \param vm    Target VM
    /// \param value Value to push on to the VM's stack
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    static void push(HSQUIRRELVM vm, const Object& value) {
        sq_pushobject(vm, value.GetObj());
    }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Used to get and push Object instances to and from the stack as references (Object is always a reference)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<>
struct Var<Object&> : Var<Object> {Var(HSQUIRRELVM vm, SQInteger idx) : Var<Object>(vm, idx) {}};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Used to get and push Object instances to and from the stack as references (Object is always a reference)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<>
struct Var<const Object&> : Var<Object> {Var(HSQUIRRELVM vm, SQInteger idx) : Var<Object>(vm, idx) {}};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Create a script object from the specified value on the default VM.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template < typename T > Object MakeObject(const T & v)
{
    // Remember the current stack size
    const StackGuard sg;
    // Transform the specified value into a script object
    PushVar< T >(SqVM(), v);
    // Get the object from the stack and return it
    return Var< Object >(SqVM(), -1).value;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Create a script object from the specified value on the specified VM.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template < typename T > Object MakeObject(HSQUIRRELVM vm, const T & v)
{
    // Remember the current stack size
    const StackGuard sg;
    // Transform the specified value into a script object
    PushVar< T >(vm, v);
    // Get the object from the stack and return it
    return Var< Object >(vm, -1).value;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Used to get and push pure script objects to and from the stack as references (HSQOBJECT is always a reference)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<>
struct Var<HSQOBJECT> {

    HSQOBJECT value; ///< The actual value of get operations

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Attempts to get the value off the stack at idx as an HSQOBJECT
    ///
    /// \param vm  Target VM
    /// \param idx Index trying to be read
    ///
    /// \remarks
    /// This function MUST have its Error handled if it occurred.
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Var(HSQUIRRELVM vm, SQInteger idx) {
        if (SQ_FAILED(sq_getstackobj(vm, idx, &value))) {
            sq_resetobject(&value);
        } else {
            sq_addref(vm, &value);
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Called by Sqrat::PushVar to put an HSQOBJECT on the stack
    ///
    /// \param vm    Target VM
    /// \param value Value to push on to the VM's stack
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    static void push(HSQUIRRELVM vm, const HSQOBJECT& value) {
        sq_pushobject(vm, value);
    }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Used to get and push pure script objects to and from the stack as references (HSQOBJECT is always a reference)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<>
struct Var<HSQOBJECT&> : Var<HSQOBJECT> {Var(HSQUIRRELVM vm, SQInteger idx) : Var<HSQOBJECT>(vm, idx) {}};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Used to get and push pure script objects to and from the stack as references (HSQOBJECT is always a reference)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<>
struct Var<const HSQOBJECT&> : Var<HSQOBJECT> {Var(HSQUIRRELVM vm, SQInteger idx) : Var<HSQOBJECT>(vm, idx) {}};

}
