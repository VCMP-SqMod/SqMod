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

#if !defined(_SCRAT_OBJECT_H_)
#define _SCRAT_OBJECT_H_

#ifdef SQMOD_PLUGIN_API
    #include <SqAPI.h>
#else
    #include <squirrelex.h>
#endif // SQMOD_PLUGIN_API

#include <string.h>

#include "sqratAllocator.h"
#include "sqratTypes.h"
#include "sqratOverloadMethods.h"
#include "sqratUtil.h"

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
    HSQUIRRELVM vm;
    HSQOBJECT obj;
    bool release;

    Object(HSQUIRRELVM v, bool releaseOnDestroy = true) : vm(v), release(releaseOnDestroy) {
        sq_resetobject(&obj);
    }
/// @endcond

public:

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Default constructor (null)
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Object() : vm(0), release(true) {
        sq_resetobject(&obj);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Copy constructor
    ///
    /// \param so Object to copy
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Object(const Object& so) : vm(so.vm), obj(so.obj), release(so.release) {
        sq_addref(vm, &obj);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Move constructor
    ///
    /// \param so Object to move
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Object(Object&& so) : vm(so.vm), obj(so.obj), release(so.release) {
        sq_resetobject(&so.GetObject());
        so.release = false;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Constructs an Object from a Squirrel object
    ///
    /// \param o Squirrel object
    /// \param v VM that the object will exist in
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Object(HSQOBJECT o, HSQUIRRELVM v = DefaultVM::Get()) : vm(v), obj(o), release(true) {
        sq_addref(vm, &obj);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Constructs an Object from a Squirrel object at a certain index on the stack
    ///
    /// \param i Index of the Squirrel object on stack
    /// \param v VM that the object will exist in
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Object(SQInteger i, HSQUIRRELVM v = DefaultVM::Get()) : vm(v), release(true) {
        if (SQ_FAILED(sq_getstackobj(vm, i, &obj))) {
            sq_resetobject(&obj);
        } else {
            sq_addref(vm, &obj);
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
    Object(T* instance, HSQUIRRELVM v = DefaultVM::Get()) : vm(v), release(true) {
        // Preserve the stack state
        const StackGuard sg(vm);
        // Push the instance on the stack
        ClassType<T>::PushInstance(vm, instance);
        // Attempt to retrieve it
        if (SQ_FAILED(sq_getstackobj(vm, -1, &obj))) {
            sq_resetobject(&obj);
            // nothing to release anymore
            release = false;
        } else {
            sq_addref(vm, &obj);
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Destructor
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    virtual ~Object() {
        if(release) {
            Release();
            release = false;
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
        if(release) {
            Release();
        }
        vm = so.vm;
        obj = so.obj;
        release = so.release;
        sq_addref(vm, &GetObject());
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
    Object& operator=(Object&& so) {
        if(release) {
            Release();
        }
        vm = so.vm;
        obj = so.obj;
        release = so.release;
        sq_resetobject(&so.GetObject());
        so.release = false;
        return *this;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Gets the Squirrel VM for this Object (reference)
    ///
    /// \return Squirrel VM associated with the Object
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    HSQUIRRELVM& GetVM() {
        return vm;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Gets the Squirrel VM for this Object (copy)
    ///
    /// \return Squirrel VM associated with the Object
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    HSQUIRRELVM GetVM() const {
        return vm;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Gets the type of the Object as defined by the Squirrel API
    ///
    /// \return SQObjectType for the Object
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    SQObjectType GetType() const {
        return GetObject()._type;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Checks whether the Object is null
    ///
    /// \return True if the Object currently has a null value, otherwise false
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    bool IsNull() const {
        return sq_isnull(GetObject());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Gets the Squirrel object for this Object (copy)
    ///
    /// \return Squirrel object
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    virtual HSQOBJECT GetObject() const {
        return obj;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Gets the Squirrel object for this Object (reference)
    ///
    /// \return Squirrel object
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    virtual HSQOBJECT& GetObject() {
        return obj;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Allows the Object to be inputted directly into places that expect a HSQOBJECT
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    operator HSQOBJECT&() {
        return GetObject();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Sets the Object to null (removing its references to underlying Squirrel objects)
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void Release() {
        sq_release(vm, &obj);
        sq_resetobject(&obj);
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
        HSQOBJECT slotObj;
        sq_pushobject(vm, GetObject());
        sq_pushstring(vm, slot, -1);

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if(SQ_FAILED(sq_get(vm, -2))) {
            sq_pop(vm, 1);
            return Object(vm); // Return a NULL object
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
        HSQOBJECT slotObj;
        sq_pushobject(vm, GetObject());
        sq_pushinteger(vm, index);

#if !defined (SCRAT_NO_ERROR_CHECKING)
        if(SQ_FAILED(sq_get(vm, -2))) {
            sq_pop(vm, 1);
            return Object(vm); // Return a NULL object
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
        sq_pushobject(vm, GetObject());
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
        sq_pushobject(vm, GetObject());
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
        sq_pushobject(vm, GetObject());
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
        sq_pushobject(vm, GetObject());
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
        /// \return String or NULL
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
        sq_pushobject(vm,obj);
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
    void Foreach(F&& func) const
    {
        const StackGuard sg(vm);
        sq_pushobject(vm,obj);
        sq_pushnull(vm);
        while(SQ_SUCCEEDED(sq_next(vm,-2)))
        {
            if (!func(vm))
            {
                return;
            }
            sq_pop(vm,2);
        }
    }

protected:
/// @cond DEV

    // Bind a function and it's associated Squirrel closure to the object
    inline void BindFunc(const SQChar* name, void* method, size_t methodSize, SQFUNCTION func, bool staticVar = false) {
        // Push object/environment
        sq_pushobject(vm, GetObject());
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
        // Push object/environment
        sq_pushobject(vm, GetObject());
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
        string overloadName;
        overloadName.reserve(15);
        SqOverloadName::Get(name, argCount, overloadName);
        // Push object/environment
        sq_pushobject(vm, GetObject());

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
        sq_pushobject(vm, GetObject());
        sq_pushstring(vm, name, -1);
        PushVar(vm, val);
        sq_newslot(vm, -3, staticVar);
        sq_pop(vm,1); // pop table
    }
    template<class V>
    inline void BindValue(const SQInteger index, const V& val, bool staticVar = false) {
        sq_pushobject(vm, GetObject());
        sq_pushinteger(vm, index);
        PushVar(vm, val);
        sq_newslot(vm, -3, staticVar);
        sq_pop(vm,1); // pop table
    }

    // Set the value of an instance on the object. Changes to values set this way are reciprocated back to the source instance
    template<class V>
    inline void BindInstance(const SQChar* name, V* val, bool staticVar = false) {
        sq_pushobject(vm, GetObject());
        sq_pushstring(vm, name, -1);
        PushVar(vm, val);
        sq_newslot(vm, -3, staticVar);
        sq_pop(vm,1); // pop table
    }
    template<class V>
    inline void BindInstance(const SQInteger index, V* val, bool staticVar = false) {
        sq_pushobject(vm, GetObject());
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
    sq_pushobject(vm, GetObject());
    sq_pushstring(vm, name, -1);
    PushVar<int>(vm, val);
    sq_newslot(vm, -3, staticVar);
    sq_pop(vm,1); // pop table
}
/// @endcond

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
        sq_pushobject(vm, value.GetObject());
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
    PushVar< T >(DefaultVM::Get(), v);
    // Get the object from the stack and return it
    return Var< Object >(DefaultVM::Get(), -1).value;
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
        sq_addref(DefaultVM::Get(), &mObj);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Move constructor
    ///
    /// \param so LightObj to move
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    LightObj(LightObj&& so) : mObj(so.mObj) {
        sq_resetobject(&so.mObj);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Constructs a LightObj from a Squirrel object
    ///
    /// \param o Squirrel object
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    LightObj(HSQOBJECT o) : mObj(o) {
        sq_addref(DefaultVM::Get(), &mObj);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Constructs a LightObj from a Squirrel object at a certain index on the stack
    ///
    /// \param i Index of the Squirrel object on stack
    /// \param v VM that the object will exist in
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    LightObj(SQInteger i, HSQUIRRELVM v = DefaultVM::Get()) {
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
    LightObj(const SQChar * s, SQInteger l, HSQUIRRELVM v = DefaultVM::Get()) {
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
    LightObj(const Object& obj) : mObj(obj.GetObject()) {
        if (!sq_isnull(mObj)) {
            sq_addref(obj.GetVM(), &mObj);
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Constructs an LightObj from a C++ instance
    ///
    /// \param instance Pointer to a C++ class instance that has been bound already
    /// \param v        VM that the object will exist in
    ///
    /// \tparam T Type of instance
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    template<class T>
    LightObj(T* instance, HSQUIRRELVM v = DefaultVM::Get()) {
        // Preserve the stack state
        const StackGuard sg(v);
        // Push the instance on the stack
        ClassType<T>::PushInstance(v, instance);
        // Attempt to retrieve it
        if (SQ_FAILED(sq_getstackobj(v, -1, &mObj))) {
            sq_resetobject(&mObj);
        } else {
            sq_addref(v, &mObj);
        }
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
    LightObj(DeleteGuard<T> guard, HSQUIRRELVM v = DefaultVM::Get()) : LightObj(guard.Get(), v) {
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
            sq_addref(DefaultVM::Get(), &mObj);
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
    LightObj& operator=(LightObj&& so) {
        if (this != &so) {
            Release();
            mObj = so.mObj;
            sq_resetobject(&so.mObj);
        }
        return *this;
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
    HSQOBJECT GetObject() const {
        return mObj;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Gets the Squirrel object for this LightObj (reference)
    ///
    /// \return Squirrel object
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    HSQOBJECT& GetObject() {
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
        return DefaultVM::Get();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Sets the LightObj to null (removing its references to underlying Squirrel objects)
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void Release() {
        // Should we release any object?
        if (!sq_isnull(mObj)) {
            sq_release(DefaultVM::Get(), &mObj);
            sq_resetobject(&mObj);
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Binds a Table or Class to the object (can be used to facilitate namespaces)
    ///
    /// \param name The key in the table being assigned a Table or Class
    /// \param obj  Table or Class that is being placed in the table
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void Bind(const SQChar* name, LightObj& obj) {
        HSQUIRRELVM vm = DefaultVM::Get();
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
        HSQUIRRELVM vm = DefaultVM::Get();
        sq_pushobject(vm, mObj);
        Var<T> v(vm, -1);
        sq_pop(vm, 1);
        return v.value;
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
        HSQUIRRELVM vm = DefaultVM::Get();
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
    PushVar< T >(DefaultVM::Get(), v);
    // Get the object from the stack and return it
    return Var< LightObj >(DefaultVM::Get(), -1).value;
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

#endif
