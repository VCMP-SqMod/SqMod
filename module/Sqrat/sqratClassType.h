//
// SqratClassType: Type Translators
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
//    1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
//
//    2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
//
//    3. This notice may not be removed or altered from any source
//    distribution.
//

#pragma once

#include <squirrelex.h>

#include <typeinfo>

#include "sqratUtil.h"
#include "Core/VecMap.hpp"

namespace Sqrat
{

/// @cond DEV

// The copy function for a class
typedef SQInteger (*COPYFUNC)(HSQUIRRELVM, SQInteger, const void*);

//Obsolete: Every Squirrel class instance made by Sqrat has its type tag set to a AbstractStaticClassData object that is unique per C++ class
struct AbstractStaticClassData {
    AbstractStaticClassData() = default;
    virtual ~AbstractStaticClassData() = default;
    virtual SQUserPointer Cast(SQUserPointer ptr, SQUserPointer classType) = 0;
    AbstractStaticClassData* baseClass{nullptr};
    string                   className{};
    COPYFUNC                 copyFunc{};
};

// Optimization technique with certain risks and requirements. Read the warnings.
// Provides an application-wide unique pointer for types and is used as an identification method.
template<class T>
struct StaticClassTypeTag {
    StaticClassTypeTag() noexcept = default;
    /* @WARNING : this makes the assumption that one (and only one) VM exists at once. which in this application is actually true. */
    static AbstractStaticClassData* TAG;
    // We don't need these:
    StaticClassTypeTag(const StaticClassTypeTag&) = delete;
    StaticClassTypeTag(StaticClassTypeTag&&) = delete;
    StaticClassTypeTag& operator=(const StaticClassTypeTag&) = delete;
    StaticClassTypeTag& operator=(StaticClassTypeTag&&) = delete;
    // Just so we don't have to access it directly.
    static inline AbstractStaticClassData* Get() noexcept { return TAG; }
};
// We provide initial unique tag to prevent any collision with null.
template<class T> AbstractStaticClassData* StaticClassTypeTag<T>::TAG=nullptr;

// StaticClassData keeps track of the nearest base class B and the class associated with itself C in order to cast C++ pointers to the right base class
template<class C, class B>
struct StaticClassData : public AbstractStaticClassData, public StaticClassTypeTag<C> {
    StaticClassData() : AbstractStaticClassData() { StaticClassTypeTag<C>::TAG = static_cast<AbstractStaticClassData*>(this); }
    ~StaticClassData() override { StaticClassTypeTag<C>::TAG = nullptr; }
    SQUserPointer Cast(SQUserPointer ptr, SQUserPointer classType) override {
        if (classType != this) {
            ptr = baseClass->Cast(static_cast<B*>(static_cast<C*>(ptr)), classType);
        }
        return ptr;
    }
};

// Base class for ClassData to provide a common interface
struct AbstractClassData {
    HSQOBJECT classObj{};
    HSQOBJECT getTable{};
    HSQOBJECT setTable{};
    AbstractClassData() = default;
    virtual ~AbstractClassData() { Release(); }
    virtual size_t InstanceCount() = 0;
    // This should be called before closing the VM
    void Release() {
        // Should we release class object?
        if (!sq_isnull(classObj)) {
            sq_release(SqVM(), &classObj);
        }
        // Should we release get table object?
        if (!sq_isnull(getTable)) {
            sq_release(SqVM(), &getTable);
        }
        // Should we release set table object?
        if (!sq_isnull(setTable)) {
            sq_release(SqVM(), &setTable);
        }
    }
    void Reset() {
        sq_resetobject(&classObj);
        sq_resetobject(&getTable);
        sq_resetobject(&setTable);
    }
};

// Every Squirrel class object created by Sqrat in every VM has its own unique ClassData object stored in the registry table of the VM
template<class C>
struct ClassData : public AbstractClassData {
    SharedPtr<std::unordered_map<C*, HSQOBJECT>> instances{};
    SharedPtr<AbstractStaticClassData> staticData{};
    ClassData() = default;
    ~ClassData() override = default;
    size_t InstanceCount() override { return instances->size(); }
};

// Inrternal structure used to keep track of created objects.
struct VMContext {
    using ClsPtr = std::unique_ptr<AbstractClassData>;
    VecMap<std::string, ClsPtr> classes{};
    VMContext() = default;
    ~VMContext() = default;
};
// Retrieve the associated context from VM
inline VMContext* GetVMContext(HSQUIRRELVM vm) { return reinterpret_cast<VMContext*>(sq_getforeignptr(vm)); }

// Lookup static class data by type_info rather than a template because C++ cannot export generic templates
/*
class _ClassType_helper {
public:
#if defined(SCRAT_IMPORT)
    static SQRAT_API WeakPtr<AbstractStaticClassData>& _getStaticClassData(const std::type_info* type);
#else
    struct compare_type_info {
        bool operator ()(const std::type_info* left, const std::type_info* right) const {
            return left->before(*right) != 0;
        }
    };
    static SQRAT_API WeakPtr<AbstractStaticClassData>& _getStaticClassData(const std::type_info* type) {
        static std::map<const std::type_info*, WeakPtr<AbstractStaticClassData>, compare_type_info> data;
        return data[type];
    }
#endif
};
*/

// Internal helper class for managing classes
template<class C>
class ClassType {
public:

    static inline ClassData<C>* getClassData(HSQUIRRELVM vm) {
        auto* ctx = reinterpret_cast<VMContext*>(sq_getforeignptr(vm));
        return static_cast<ClassData<C>*>(ctx->classes[ClassName()].get());
/*
        sq_pushregistrytable(vm);
        sq_pushstring(vm, "__classes", -1);
#ifndef NDEBUG
        SQRESULT r = sq_rawget(vm, -2);
        assert(SQ_SUCCEEDED(r)); // fails if getClassData is called when the data does not exist for the given VM yet (bind the class)
#else
        sq_rawget(vm, -2);
#endif
        sq_pushstring(vm, ClassName().c_str(), -1);
#ifndef NDEBUG
        r = sq_rawget(vm, -2);
        assert(SQ_SUCCEEDED(r)); // fails if getClassData is called when the data does not exist for the given VM yet (bind the class)
#else
        sq_rawget(vm, -2);
#endif
        ClassData<C>** ud;
        sq_getuserdata(vm, -1, (SQUserPointer*)&ud, nullptr);
        sq_pop(vm, 3);
        return *ud;
*/
    }

    //static WeakPtr<AbstractStaticClassData>& getStaticClassData() {
    //    return _ClassType_helper::_getStaticClassData(&typeid(C));
    //}
    static AbstractStaticClassData* getStaticClassData() {
        return StaticClassTypeTag<C>::Get();
    }

    static inline bool hasClassData(HSQUIRRELVM vm) {
        //if (!getStaticClassData().Expired()) {
        if (StaticClassTypeTag<C>::Get()!=nullptr) {
            VMContext* ctx = reinterpret_cast<VMContext*>(sq_getforeignptr(vm));
            return ctx->classes.exists(ClassName());
/*
            sq_pushregistrytable(vm);
            sq_pushstring(vm, "__classes", -1);
            if (SQ_SUCCEEDED(sq_rawget(vm, -2))) {
                sq_pushstring(vm, ClassName().c_str(), -1);
                if (SQ_SUCCEEDED(sq_rawget(vm, -2))) {
                    sq_pop(vm, 3);
                    return true;
                }
                sq_pop(vm, 1);
            }
            sq_pop(vm, 1);
*/
        }
        return false;
    }

    //static inline AbstractStaticClassData*& BaseClass() {
    //    assert(!getStaticClassData().Expired()); // fails because called before a Sqrat::Class for this type exists
    //    return getStaticClassData().Lock()->baseClass;
    //}

    static inline AbstractStaticClassData*& BaseClass() {
        assert(StaticClassTypeTag<C>::Get()); // fails because called before a Sqrat::Class for this type exists
        return StaticClassTypeTag<C>::Get()->baseClass;
    }

    //static inline string& ClassName() {
    //    assert(!getStaticClassData().Expired()); // fails because called before a Sqrat::Class for this type exists
    //    return getStaticClassData().Lock()->className;
    //}

    static inline string& ClassName() {
        assert(StaticClassTypeTag<C>::Get()); // fails because called before a Sqrat::Class for this type exists
        return StaticClassTypeTag<C>::Get()->className;
    }

    //static inline COPYFUNC& CopyFunc() {
    //    assert(!getStaticClassData().Expired()); // fails because called before a Sqrat::Class for this type exists
    //    return getStaticClassData().Lock()->copyFunc;
    //}

    static inline COPYFUNC& CopyFunc() {
        assert(StaticClassTypeTag<C>::Get()); // fails because called before a Sqrat::Class for this type exists
        return StaticClassTypeTag<C>::Get()->copyFunc;
    }

    static SQInteger DeleteInstance(SQUserPointer ptr, SQInteger size) {
        SQUNUSED(size);
        auto* instance = reinterpret_cast<std::pair<C*, SharedPtr<std::unordered_map<C*, HSQOBJECT>> >*>(ptr);
        instance->second->erase(instance->first);
        delete instance;
        return 0;
    }

    static void PushInstance(HSQUIRRELVM vm, C* ptr, std::nullptr_t) {
        if (!ptr) {
            sq_pushnull(vm);
            return;
        }

        ClassData<C>* cd = getClassData(vm);

        typename std::unordered_map<C*, HSQOBJECT>::iterator it = cd->instances->find(ptr);
        if (it != cd->instances->end()) {
            sq_pushobject(vm, it->second);
        } else sq_pushnull(vm);
    }

    static void PushInstance(HSQUIRRELVM vm, C* ptr) {
        if (!ptr) {
            sq_pushnull(vm);
            return;
        }

        ClassData<C>* cd = getClassData(vm);

        typename std::unordered_map<C*, HSQOBJECT>::iterator it = cd->instances->find(ptr);
        if (it != cd->instances->end()) {
            sq_pushobject(vm, it->second);
            return;
        }

        sq_pushobject(vm, cd->classObj);
        sq_createinstance(vm, -1);
        sq_remove(vm, -2);
        sq_setinstanceup(vm, -1, new std::pair<C*, SharedPtr<std::unordered_map<C*, HSQOBJECT>> >(ptr, cd->instances));
        sq_setreleasehook(vm, -1, &DeleteInstance);
        sq_getstackobj(vm, -1, &((*cd->instances)[ptr]));
    }

    static void PushInstanceCopy(HSQUIRRELVM vm, const C& value) {
        sq_pushobject(vm, getClassData(vm)->classObj);
        sq_createinstance(vm, -1);
        sq_remove(vm, -2);
#ifndef NDEBUG
        SQRESULT result = CopyFunc()(vm, -1, &value);
        assert(SQ_SUCCEEDED(result)); // fails when trying to copy an object defined as non-copyable
#else
        CopyFunc()(vm, -1, &value);
#endif
    }

    static C* GetInstance(HSQUIRRELVM vm, SQInteger idx, bool nullAllowed = false) {
        //AbstractStaticClassData* classType = nullptr;
        AbstractStaticClassData* classType = StaticClassTypeTag<C>::Get();
        std::pair<C*, SharedPtr<std::unordered_map<C*, HSQOBJECT>> >* instance = nullptr;
        //if (hasClassData(vm)) /* type checking only done if the value has type data else it may be enum */
        if (classType!=nullptr) /* type checking only done if the value has type data else it may be enum */
        {
            if (nullAllowed && sq_gettype(vm, idx) == OT_NULL) {
                return nullptr;
            }

            //classType = getStaticClassData().Lock().Get();

#if !defined (SCRAT_NO_ERROR_CHECKING)
            if (SQ_FAILED(sq_getinstanceup(vm, idx, (SQUserPointer*)&instance, classType))) {
                SQTHROW(vm, FormatTypeError(vm, idx, ClassName()));
                return nullptr;
            }

            if (instance == nullptr) {
                SQTHROW(vm, _SC("got unconstructed native class (call base.constructor in the constructor of Squirrel classes that extend native classes)"));
                return nullptr;
            }
#else
            sq_getinstanceup(vm, idx, (SQUserPointer*)&instance, 0);
#endif
        }
        else /* value is likely of integral type like enums, cannot return a pointer */
        {
            SQTHROW(vm, FormatTypeError(vm, idx, _SC("unknown")));
            return nullptr;
        }
        AbstractStaticClassData* actualType;
        sq_gettypetag(vm, idx, (SQUserPointer*)&actualType);
        if (actualType == nullptr) {
            SQInteger top = sq_gettop(vm);
            sq_getclass(vm, idx);
            while (actualType == nullptr) {
                sq_getbase(vm, -1);
                sq_gettypetag(vm, -1, (SQUserPointer*)&actualType);
            }
            sq_settop(vm, top);
        }
        if (classType != actualType) {
            return static_cast<C*>(actualType->Cast(instance->first, classType));
        }
        return instance->first;
    }
};

/// @endcond

}
