//
// SqratArray: Array Binding
//

//
// Copyright 2011 Alston Chen
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

#include "sqratObject.h"
#include "sqratLightObj.h"
#include "sqratFunction.h"
#include "sqratGlobalMethods.h"

namespace Sqrat {

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// The base class for Array that implements almost all of its functionality
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ArrayBase : public Object {
public:

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Default constructor (null)
    ///
    /// \param v VM that the array will exist in
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ArrayBase() : Object(true) {
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Construct the ArrayBase from an Object that already exists
    ///
    /// \param obj An Object that should already represent a Squirrel array
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    explicit ArrayBase(const Object& obj) : Object(obj) {
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Construct the ArrayBase from an Object that already exists
    ///
    /// \param obj An Object that should already represent a Squirrel array
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    explicit ArrayBase(Object&& obj) noexcept : Object(std::move(obj)) {
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Construct the ArrayBase from an LightObj that already exists
    ///
    /// \param obj An LightObj that should already represent a Squirrel array
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    explicit ArrayBase(const LightObj& obj) : Object(obj) {
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Construct the ArrayBase from an LightObj that already exists
    ///
    /// \param obj An LightObj that should already represent a Squirrel array
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    explicit ArrayBase(LightObj&& obj) noexcept : Object(std::move(obj)) {
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Copy constructor
    ///
    /// \param sa ArrayBase to copy
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ArrayBase(const ArrayBase& sa) = default;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Move constructor
    ///
    /// \param sa ArrayBase to move
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ArrayBase(ArrayBase&& sa) noexcept : Object(std::move(sa)) {
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Construct the ArrayBase from a HSQOBJECT and HSQUIRRELVM that already exist
    ///
    /// \param o Squirrel object that should already represent a Squirrel array
    /// \param v Squirrel VM that contains the Squirrel object given
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    explicit ArrayBase(HSQOBJECT o, HSQUIRRELVM v = SqVM()) : Object(o, v) {
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Assignment operator
    ///
    /// \param sa ArrayBase to copy
    ///
    /// \return The ArrayBase itself
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ArrayBase& operator=(const ArrayBase& sa) = default;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Assignment operator
    ///
    /// \param sa ArrayBase to move
    ///
    /// \return The ArrayBase itself
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ArrayBase& operator=(ArrayBase&& sa) noexcept {
        Object::operator = (std::move(sa));
        return *this;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Binds a Table or Class to the Array (can be used to facilitate namespaces)
    ///
    /// \param index The index in the array being assigned a Table or Class
    /// \param obj   Table or Class that is being placed in the Array
    ///
    /// \remarks
    /// Bind cannot be called "inline" like other functions because it introduces order-of-initialization bugs.
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void Bind(const SQInteger index, Object& obj) {
        HSQUIRRELVM vm = SqVM();
        sq_pushobject(vm, GetObj());
        sq_pushinteger(vm, index);
        sq_pushobject(vm, obj.GetObj());
        sq_set(vm, -3);
        sq_pop(vm,1); // pop array
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Binds a raw Squirrel closure to the Array
    ///
    /// \param index The index in the array being assigned a function
    /// \param func  Squirrel function that is being placed in the Array
    /// \param name  The name to associate with the function.
    ///
    /// \return The Array itself so the call can be chained
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ArrayBase& SquirrelFunc(const SQInteger index, SQFUNCTION func, const SQChar* name = nullptr) {
        HSQUIRRELVM vm = SqVM();
        sq_pushobject(vm, GetObj());
        sq_pushinteger(vm, index);
        sq_newclosure(vm, func, 0);
        // Set the closure name (for debug purposes)
        if (name) sq_setnativeclosurename(vm, -1, name);
        sq_set(vm, -3);
        sq_pop(vm,1); // pop array
        return *this;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Binds a raw Squirrel closure to the Array
    ///
    /// \param index The index in the array being assigned a function
    /// \param func  Squirrel function that is being placed in the Array
    /// \param name  The name to associate with the function.
    /// \param pnum  Number of parameters the function expects.
    /// \param mask  Types of parameters the function expects.
    ///
    /// \return The Array itself so the call can be chained
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ArrayBase& SquirrelFunc(const SQInteger index, SQFUNCTION func, SQInteger pnum, const SQChar * mask, const SQChar* name = nullptr) {
        HSQUIRRELVM vm = SqVM();
        sq_pushobject(vm, GetObj());
        sq_pushinteger(vm, index);
        sq_newclosure(vm, func, 0);
        // Set the closure name (for debug purposes)
        if (name) sq_setnativeclosurename(vm, -1, name);
        // Set parameter validation
        sq_setparamscheck(vm, pnum, mask);
        sq_set(vm, -3);
        sq_pop(vm,1); // pop array
        return *this;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Sets an index in the Array to a specific value
    ///
    /// \param index The index in the array being assigned a value
    /// \param val   Value that is being placed in the Array
    ///
    /// \tparam V Type of value (usually doesnt need to be defined explicitly)
    ///
    /// \return The Array itself so the call can be chained
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    template<class V>
    ArrayBase& SetValue(const SQInteger index, const V& val) {
        HSQUIRRELVM vm = SqVM();
        sq_pushobject(vm, GetObj());
        sq_pushinteger(vm, index);
        PushVar(vm, val);
        sq_set(vm, -3);
        sq_pop(vm,1); // pop array
        return *this;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Sets an index in the Array to a specific instance (like a reference)
    ///
    /// \param index The index in the array being assigned a value
    /// \param val   Pointer to the instance that is being placed in the Array
    ///
    /// \tparam V Type of instance (usually doesnt need to be defined explicitly)
    ///
    /// \return The Array itself so the call can be chained
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    template<class V>
    ArrayBase& SetInstance(const SQInteger index, V* val) {
        BindInstance<V>(index, val, false);
        return *this;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Sets an index in the Array to a specific function
    ///
    /// \param index  The index in the array being assigned a value
    /// \param method Function that is being placed in the Array
    ///
    /// \tparam F Type of function (only define this if you need to choose a certain template specialization or overload)
    ///
    /// \return The Array itself so the call can be chained
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    template<class F>
    ArrayBase& Func(const SQInteger index, F method, const SQChar* name = nullptr) {
        BindFunc(index, &method, sizeof(method), SqGlobalFunc(method), static_cast<bool>(name));
        return *this;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Returns the element at a given index
    ///
    /// \param index Index of the element
    ///
    /// \tparam T Type of element (fails if element is not of this type)
    ///
    /// \return SharedPtr containing the element (or null if failed)
    ///
    /// \remarks
    /// This function MUST have its Error handled if it occurred.
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    template <typename T>
    SharedPtr<T> GetValue(int index) {
        HSQUIRRELVM vm = SqVM();
        sq_pushobject(vm, mObj);
        sq_pushinteger(vm, index);
#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (SQ_FAILED(sq_get(vm, -2))) {
            sq_pop(vm, 1);
            SQTHROW(vm, _SC("illegal index"));
            return SharedPtr<T>(); // avoid "not all control paths return a value" warning
        }
#else
        sq_get(vm, -2);
#endif
        SQTRY()
        Var<SharedPtr<T> > element(vm, -1);
        SQCATCH_NOEXCEPT(vm) {
            sq_pop(vm, 2);
            return SharedPtr<T>();
        }
        sq_pop(vm, 2);
        return element.value;
        SQCATCH(vm) {
#if defined (SCRAT_USE_EXCEPTIONS)
            SQUNUSED(e); // avoid "unreferenced local variable" warning
#endif
            sq_pop(vm, 2);
            SQRETHROW(vm);
        }
        SQ_UNREACHABLE
        return SharedPtr<T>(); // avoid "not all control paths return a value" warning
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Gets a Function from an index in the Array
    ///
    /// \param index The index in the array that contains the Function
    ///
    /// \return Function found in the Array (null if failed)
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Function GetFunction(const SQInteger index) {
        HSQUIRRELVM vm = SqVM();
        HSQOBJECT funcObj;
        sq_pushobject(vm, GetObj());
        sq_pushinteger(vm, index);
#if !defined (SCRAT_NO_ERROR_CHECKING)
        if(SQ_FAILED(sq_get(vm, -2))) {
            sq_pop(vm, 1);
            return Function();
        }
        SQObjectType value_type = sq_gettype(vm, -1);
        if (value_type != OT_CLOSURE && value_type != OT_NATIVECLOSURE) {
            sq_pop(vm, 2);
            return Function();
        }
#else
        sq_get(vm, -2);
#endif
        sq_getstackobj(vm, -1, &funcObj);
        Function ret(GetObj(), funcObj, vm); // must addref before the pop!
        sq_pop(vm, 2);
        return ret;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Fills a C array with the elements of the Array
    ///
    /// \param array C array to be filled
    /// \param size  The amount of elements to fill the C array with
    ///
    /// \tparam T Type of elements (fails if any elements in Array are not of this type)
    ///
    /// \remarks
    /// This function MUST have its Error handled if it occurred.
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    template <typename T>
    void GetArray(T* array, int size) const {
        HSQUIRRELVM vm = SqVM();
        HSQOBJECT value = GetObj();
        sq_pushobject(vm, value);
#if !defined (SCRAT_NO_ERROR_CHECKING)
        if (size > sq_getsize(vm, -1)) {
            sq_pop(vm, 1);
            SQTHROW(vm, _SC("array buffer size too big"));
            return;
        }
#endif
        sq_pushnull(vm);
        SQInteger i;
        while (SQ_SUCCEEDED(sq_next(vm, -2))) {
            sq_getinteger(vm, -2, &i);
            if (i >= size) break;
            SQTRY()
            Var<const T&> element(vm, -1);
            SQCATCH_NOEXCEPT(vm) {
                sq_pop(vm, 4);
                return;
            }
            sq_pop(vm, 2);
            array[i] = element.value;
            SQCATCH(vm) {
#if defined (SCRAT_USE_EXCEPTIONS)
                SQUNUSED(e); // avoid "unreferenced local variable" warning
#endif
                sq_pop(vm, 4);
                SQRETHROW(vm);
            }
        }
        sq_pop(vm, 2); // pops the null iterator and the array object
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Appends a value to the end of the Array
    ///
    /// \param val Value that is being placed in the Array
    ///
    /// \tparam V Type of value (usually doesnt need to be defined explicitly)
    ///
    /// \return The Array itself so the call can be chained
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    template<class V>
    ArrayBase& Append(const V& val) {
        HSQUIRRELVM vm = SqVM();
        sq_pushobject(vm, GetObj());
        PushVar(vm, val);
        sq_arrayappend(vm, -2);
        sq_pop(vm,1); // pop array
        return *this;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Appends an instance to the end of the Array (like a reference)
    ///
    /// \param val Pointer to the instance that is being placed in the Array
    ///
    /// \tparam V Type of instance (usually doesnt need to be defined explicitly)
    ///
    /// \return The Array itself so the call can be chained
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    template<class V>
    ArrayBase& Append(V* val) {
        HSQUIRRELVM vm = SqVM();
        sq_pushobject(vm, GetObj());
        PushVar(vm, val);
        sq_arrayappend(vm, -2);
        sq_pop(vm,1); // pop array
        return *this;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Inserts a value in a position in the Array
    ///
    /// \param destpos Index to put the new value in
    /// \param val     Value that is being placed in the Array
    ///
    /// \tparam V Type of value (usually doesnt need to be defined explicitly)
    ///
    /// \return The Array itself so the call can be chained
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    template<class V>
    ArrayBase& Insert(const SQInteger destpos, const V& val) {
        HSQUIRRELVM vm = SqVM();
        sq_pushobject(vm, GetObj());
        PushVar(vm, val);
        sq_arrayinsert(vm, -2, destpos);
        sq_pop(vm,1); // pop array
        return *this;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Inserts an instance in a position in the Array (like a reference)
    ///
    /// \param destpos Index to put the new value in
    /// \param val     Pointer to the instance that is being placed in the Array
    ///
    /// \tparam V Type of instance (usually doesnt need to be defined explicitly)
    ///
    /// \return The Array itself so the call can be chained
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    template<class V>
    ArrayBase& Insert(const SQInteger destpos, V* val) {
        HSQUIRRELVM vm = SqVM();
        sq_pushobject(vm, GetObj());
        PushVar(vm, val);
        sq_arrayinsert(vm, -2, destpos);
        sq_pop(vm,1); // pop array
        return *this;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Removes the last element from the Array
    ///
    /// \return Object for the element that was removed (null if failed)
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Object Pop() {
        HSQUIRRELVM vm = SqVM();
        HSQOBJECT slotObj;
        sq_pushobject(vm, GetObj());
        if(SQ_FAILED(sq_arraypop(vm, -1, true))) {
            sq_pop(vm, 1);
            return Object(); // Return a nullptr object
        } else {
            sq_getstackobj(vm, -1, &slotObj);
            Object ret(slotObj, vm);
            sq_pop(vm, 2);
            return ret;
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Removes an element at a specific index from the Array
    ///
    /// \param itemidx Index of the element being removed
    ///
    /// \return The Array itself so the call can be chained
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ArrayBase& Remove(const SQInteger itemidx) {
        HSQUIRRELVM vm = SqVM();
        sq_pushobject(vm, GetObj());
        sq_arrayremove(vm, -1, itemidx);
        sq_pop(vm,1); // pop array
        return *this;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Resizes the Array
    ///
    /// \param newsize Desired size of the Array in number of elements
    ///
    /// \return The Array itself so the call can be chained
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ArrayBase& Resize(const SQInteger newsize) {
        HSQUIRRELVM vm = SqVM();
        sq_pushobject(vm, GetObj());
        sq_arrayresize(vm, -1, newsize);
        sq_pop(vm,1); // pop array
        return *this;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Reallocates the Array
    ///
    /// \param newcap Desired capacity of the Array in number of elements
    ///
    /// \return The Array itself so the call can be chained
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ArrayBase& Reserve(const SQInteger newcap) {
        HSQUIRRELVM vm = SqVM();
        sq_pushobject(vm, GetObj());
        sq_arrayreserve(vm, -1, newcap);
        sq_pop(vm,1); // pop array
        return *this;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Reverses the elements of the array in place
    ///
    /// \return The Array itself so the call can be chained
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ArrayBase& Reverse() {
        HSQUIRRELVM vm = SqVM();
        sq_pushobject(vm, GetObj());
        sq_arrayreverse(vm, -1);
        sq_pop(vm,1); // pop array
        return *this;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Returns the length of the Array
    ///
    /// \return Length in number of elements
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    SQInteger Length() const {
        HSQUIRRELVM vm = SqVM();
        sq_pushobject(vm, mObj);
        SQInteger r = sq_getsize(vm, -1);
        sq_pop(vm, 1);
        return r;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Appends values to the end of the Array
    ///
    /// \param func Functor that is continuously called to push values on the stack
    ///
    /// \tparam F Type of functor (usually doesnt need to be defined explicitly)
    ///
    /// \return The Array itself so the call can be chained
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    template<class F, class... A>
    ArrayBase& AppendFrom(F&& func, A &&... a) {
        HSQUIRRELVM vm = SqVM();
        const StackGuard sg(vm);
        sq_pushobject(vm, GetObj());
        while (func(vm, std::forward< A >(a)...))
        {
            sq_arrayappend(vm, -2);
        }
        //sq_pop(vm,1); // pop array
        return *this;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Appends values to the end of the Array
    ///
    /// \param func Functor that is continuously called to push values on the stack
    ///
    /// \tparam F Type of functor (usually doesnt need to be defined explicitly)
    ///
    /// \return The Array itself so the call can be chained
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    template<class F, class... A>
    ArrayBase& AppendFromCounted(F&& func, A &&... a) {
        HSQUIRRELVM vm = SqVM();
        const StackGuard sg(vm);
        sq_pushobject(vm, GetObj());
        for (SQInteger i = 0; func(vm, i, std::forward< A >(a)...); ++i)
        {
            sq_arrayappend(vm, -2);
        }
        //sq_pop(vm,1); // pop array
        return *this;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Appends values to the end of the Array
    ///
    /// \param vec Vector to be appended to the array
    ///
    /// \tparam T Type of value (usually doesnt need to be defined explicitly)
    ///
    /// \return The Array itself so the call can be chained
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    template<class T>
    ArrayBase& AppendFromVector(const std::vector< T > & vec) {
        HSQUIRRELVM vm = SqVM();
        const StackGuard sg(vm);
        sq_pushobject(vm, GetObj());
        for (const auto & v : vec)
        {
            Var< const T & >::push(vm, v);
            sq_arrayappend(vm, -2);
        }
        //sq_pop(vm,1); // pop array
        return *this;
    }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents an array in Squirrel
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Array : public ArrayBase {
public:

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Default constructor (null)
    ///
    /// \remarks
    /// The Array is invalid until it is given a VM to exist in.
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Array() {
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Constructs an Array
    ///
    /// \param v    VM to create the Array in
    /// \param size An optional size hint
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Array(HSQUIRRELVM vm, const SQInteger size = 0) : ArrayBase() {
        sq_newarray(vm, size);
        sq_getstackobj(vm,-1,&mObj);
        sq_addref(vm, &mObj);
        sq_pop(vm,1);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Construct the Array from an Object that already exists
    ///
    /// \param obj An Object that should already represent a Squirrel array
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Array(const Object& obj) : ArrayBase(obj) {
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Construct the Array from an Object that already exists
    ///
    /// \param obj An Object that should already represent a Squirrel array
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Array(Object&& obj) noexcept : ArrayBase(std::move(obj)) {
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Construct the Array from an LightObj that already exists
    ///
    /// \param obj An LightObj that should already represent a Squirrel array
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Array(const LightObj& obj) : ArrayBase(obj) {
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Construct the Array from an LightObj that already exists
    ///
    /// \param obj An LightObj that should already represent a Squirrel array
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Array(LightObj&& obj) noexcept : ArrayBase(std::move(obj)) {
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Construct the Array from a HSQOBJECT and HSQUIRRELVM that already exist
    ///
    /// \param o Squirrel object that should already represent a Squirrel array
    /// \param v Squirrel VM that contains the Squirrel object given
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Array(HSQOBJECT o, HSQUIRRELVM v = SqVM()) : ArrayBase(o, v) {
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Copy constructor
    ///
    /// \param sa Array to copy
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Array(const Array& sa) : ArrayBase(sa) {
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Move constructor
    ///
    /// \param sa Array to move
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Array(Array&& sa) noexcept : ArrayBase(std::move(sa)) {
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Assignment operator
    ///
    /// \param sa Array to copy
    ///
    /// \return The Array itself
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Array& operator=(const Array& sa) {
        ArrayBase::operator = (sa);
        return *this;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Assignment operator
    ///
    /// \param sa Array to move
    ///
    /// \return The Array itself
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Array& operator=(Array&& sa) noexcept {
        ArrayBase::operator = (std::move(sa));
        return *this;
    }

};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Used to get and push Array instances to and from the stack as references (arrays are always references in Squirrel)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<>
struct Var<Array> {

    Array value; ///< The actual value of get operations

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Attempts to get the value off the stack at idx as an Array
    ///
    /// \param vm  Target VM
    /// \param idx Index trying to be read
    ///
    /// \remarks
    /// This function MUST have its Error handled if it occurred.
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Var(HSQUIRRELVM vm, SQInteger idx) {
        HSQOBJECT obj;
        sq_resetobject(&obj);
        sq_getstackobj(vm,idx,&obj);
        value = Array(obj, vm);
#if !defined (SCRAT_NO_ERROR_CHECKING)
        SQObjectType value_type = sq_gettype(vm, idx);
        if (value_type != OT_ARRAY) {
            SQTHROW(vm, FormatTypeError(vm, idx, _SC("array")));
        }
#endif
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Called by Sqrat::PushVar to put an Array reference on the stack
    ///
    /// \param vm    Target VM
    /// \param value Value to push on to the VM's stack
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    static void push(HSQUIRRELVM vm, const Array& value) {
        HSQOBJECT obj;
        sq_resetobject(&obj);
        obj = value.GetObj();
        sq_pushobject(vm,obj);
    }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Used to get and push Array instances to and from the stack as references (arrays are always references in Squirrel)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<>
struct Var<Array&> : Var<Array> {Var(HSQUIRRELVM vm, SQInteger idx) : Var<Array>(vm, idx) {}};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Used to get and push Array instances to and from the stack as references (arrays are always references in Squirrel)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<>
struct Var<const Array&> : Var<Array> {Var(HSQUIRRELVM vm, SQInteger idx) : Var<Array>(vm, idx) {}};

}
