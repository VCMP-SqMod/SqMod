//
// SqratConst: Constant and Enumeration Binding
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

#if !defined(_SCRAT_CONST_H_)
#define _SCRAT_CONST_H_

#ifdef SQMOD_PLUGIN_API
    #include <SqAPI.h>
#else
    #include <squirrelex.h>
#endif // SQMOD_PLUGIN_API

#include <string.h>

#include "sqratObject.h"

namespace Sqrat {

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Helper to remember the type of the constant.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum ConstType
{
    SQET_STRING = 0,
    SQET_INT    = 1,
    SQET_REAL   = 2,
    SQET_BOOL   = 3
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Helper class that represents an enumeration value. Used to reduce compilation times and executable size.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct EnumElement
{
    // The name of the constant
    const SQChar *          Name;
    // The value of the constant
    union {
        const SQChar *      mSTR;
        const SQInteger     mINT;
        const SQFloat       mREAL;
        const bool          mBOOL;
    };
    // The type of the constant value
    const unsigned short    Type;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Constructors that can identify the type and perform the proper conversion.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    EnumElement(const SQChar * name, const SQChar * value)
        : Name(name), mSTR(value), Type(SQET_STRING)
    { /* ... */ }
    EnumElement(const SQChar * name, bool value)
        : Name(name), mBOOL(value), Type(SQET_BOOL)
    { /* ... */ }
    EnumElement(const SQChar * name, signed char value)
        : Name(name), mINT(static_cast< SQInteger >(value)), Type(SQET_INT)
    { /* ... */ }
    EnumElement(const SQChar * name, unsigned char value)
        : Name(name), mINT(static_cast< SQInteger >(value)), Type(SQET_INT)
    { /* ... */ }
    EnumElement(const SQChar * name, signed short value)
        : Name(name), mINT(static_cast< SQInteger >(value)), Type(SQET_INT)
    { /* ... */ }
    EnumElement(const SQChar * name, unsigned short value)
        : Name(name), mINT(static_cast< SQInteger >(value)), Type(SQET_INT)
    { /* ... */ }
    EnumElement(const SQChar * name, signed int value)
        : Name(name), mINT(static_cast< SQInteger >(value)), Type(SQET_INT)
    { /* ... */ }
    EnumElement(const SQChar * name, unsigned int value)
        : Name(name), mINT(static_cast< SQInteger >(value)), Type(SQET_INT)
    { /* ... */ }
    EnumElement(const SQChar * name, signed long value)
        : Name(name), mINT(static_cast< SQInteger >(value)), Type(SQET_INT)
    { /* ... */ }
    EnumElement(const SQChar * name, unsigned long value)
        : Name(name), mINT(static_cast< SQInteger >(value)), Type(SQET_INT)
    { /* ... */ }
    EnumElement(const SQChar * name, signed long long value)
        : Name(name), mINT(static_cast< SQInteger >(value)), Type(SQET_INT)
    { /* ... */ }
    EnumElement(const SQChar * name, unsigned long long value)
        : Name(name), mINT(static_cast< SQInteger >(value)), Type(SQET_INT)
    { /* ... */ }
    EnumElement(const SQChar * name, float value)
        : Name(name), mREAL(static_cast< SQFloat >(value)), Type(SQET_REAL)
    { /* ... */ }
    EnumElement(const SQChar * name, double value)
        : Name(name), mREAL(static_cast< SQFloat >(value)), Type(SQET_REAL)
    { /* ... */ }
    template < typename T > EnumElement(const SQChar * name, T value)
        : Name(name), mINT(static_cast< SQInteger >(value)), Type(SQET_INT)
    { /* ... */ }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// HHelper class that represents an enumeration. Used to reduce compilation times and executable size.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct EnumElements
{
    // The name of the enumeration if this is an enumeration otherwise null
    const SQChar *      Name;
    // The constant values
    const EnumElement * Values;
    // The number of values
    const unsigned int  Count;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Constructor that can identify the number of values at compile-time.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    template < size_t N > EnumElements(const EnumElement(&values)[N])
        : EnumElements(nullptr, values, N)
    { /* ... */ }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Constructor that takes an explicit number of values at run-time.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    EnumElements(const EnumElement * values, unsigned int count)
        : EnumElements(nullptr, values, count)
    { /* ... */ }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Constructor that can identify the number of values at compile-time.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    template < size_t N > EnumElements(const SQChar * name, const EnumElement(&values)[N])
        : EnumElements(name, values, N)
    { /* ... */ }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Constructor that takes an explicit number of values at run-time.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    EnumElements(const SQChar * name, const EnumElement * values, unsigned int count)
        : Name(name), Values(values), Count(count)
    { /* ... */ }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Register the managed constants into the specified constants table.
    ///
    /// \remarks
    /// The constants table could be either the main constants table or an enumeration.
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    template < typename T > void Bind(T & table) const
    {
        for (unsigned int i = 0; i < Count; ++i)
        {
            switch (Values[i].Type)
            {
                case SQET_STRING:   table.Const(Values[i].Name, Values[i].mSTR); break;
                case SQET_INT:      table.Const(Values[i].Name, Values[i].mINT); break;
                case SQET_REAL:     table.Const(Values[i].Name, Values[i].mREAL); break;
                case SQET_BOOL:     table.Const(Values[i].Name, Values[i].mBOOL); break;
                default:            SQTHROW(table.GetVM(), "Unknown constant value type");
            }
        }
    }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Facilitates exposing a C++ enumeration to Squirrel
///
/// \remarks
/// The Enumeration class only facilitates binding C++ enumerations that contain only integers,
/// floats, and strings because the Squirrel constant table can only contain these types of
/// values. Other types of enumerations can be bound using Class::SetStaticValue instead.
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Enumeration : public Object {
public:
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Constructs the Enumeration object
    ///
    /// An Enumeration object doesnt do anything on its own.
    /// It must be told what constant values it contains.
    /// This is done using Enumeration::Const.
    /// Then the Enumeration must be exposed to Squirrel.
    /// This is done by calling ConstTable::Enum with the Enumeration.
    ///
    /// \param v           Squirrel virtual machine to create the Enumeration for
    /// \param createTable Whether the underlying table that values are bound to is created by the constructor
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Enumeration(HSQUIRRELVM v = DefaultVM::Get(), bool createTable = true) : Object(v, false) {
        if(createTable) {
            sq_newtable(vm);
            sq_getstackobj(vm,-1,&obj);
            sq_addref(vm, &obj);
            sq_pop(vm,1);
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Binds an enumeration value
    ///
    /// \param name Name of the value as it will appear in Squirrel
    /// \param val  Value to bind
    ///
    /// \return The Enumeration itself so the call can be chained
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    virtual Enumeration& Const(const SQChar* name, const int val) {
        BindValue<int>(name, val, false);
        return *this;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Binds an enumeration value
    ///
    /// \param name Name of the value as it will appear in Squirrel
    /// \param val  Value to bind
    ///
    /// \return The Enumeration itself so the call can be chained
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    virtual Enumeration& Const(const SQChar* name, const long long int val) {
        BindValue<long long int>(name, val, false);
        return *this;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Binds an enumeration value
    ///
    /// \param name Name of the value as it will appear in Squirrel
    /// \param val  Value to bind
    ///
    /// \return The Enumeration itself so the call can be chained
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    virtual Enumeration& Const(const SQChar* name, const float val) {
        BindValue<float>(name, val, false);
        return *this;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Binds an enumeration value
    ///
    /// \param name Name of the value as it will appear in Squirrel
    /// \param val  Value to bind
    ///
    /// \return The Enumeration itself so the call can be chained
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    virtual Enumeration& Const(const SQChar* name, const double val) {
        BindValue<double>(name, val, false);
        return *this;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Binds an enumeration value
    ///
    /// \param name Name of the value as it will appear in Squirrel
    /// \param val  Value to bind
    ///
    /// \return The Enumeration itself so the call can be chained
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    virtual Enumeration& Const(const SQChar* name, const SQChar* val) {
        BindValue<const SQChar*>(name, val, false);
        return *this;
    }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Facilitates exposing a C++ constant to Squirrel
///
/// \remarks
/// The ConstTable class only facilitates binding C++ constants that are integers,
/// floats, and strings because the Squirrel constant table can only contain these types of
/// values. Other types of constants can be bound using Class::SetStaticValue instead.
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ConstTable : public Enumeration {
public:
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Constructs a ConstTable object to represent the given VM's const table
    ///
    /// \param v VM to get the ConstTable for
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ConstTable(HSQUIRRELVM v = DefaultVM::Get()) : Enumeration(v, false) {
        sq_pushconsttable(vm);
        sq_getstackobj(vm,-1, &obj);
        sq_pop(v,1); // No addref needed, since the consttable is always around
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Binds a constant value
    ///
    /// \param name Name of the value as it will appear in Squirrel
    /// \param val  Value to bind
    ///
    /// \return The ConstTable itself so the call can be chained
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    virtual ConstTable& Const(const SQChar* name, const int val) {
        Enumeration::Const(name, val);
        return *this;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Binds a constant value
    ///
    /// \param name Name of the value as it will appear in Squirrel
    /// \param val  Value to bind
    ///
    /// \return The ConstTable itself so the call can be chained
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    virtual ConstTable& Const(const SQChar* name, const float val) {
        Enumeration::Const(name, val);
        return *this;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Binds a constant value
    ///
    /// \param name Name of the value as it will appear in Squirrel
    /// \param val  Value to bind
    ///
    /// \return The ConstTable itself so the call can be chained
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    virtual ConstTable& Const(const SQChar* name, const SQChar* val) {
        Enumeration::Const(name, val);
        return *this;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Binds an Enumeration to the ConstTable
    ///
    /// \param name Name of the enumeration as it will appear in Squirrel
    /// \param en   Enumeration to bind
    ///
    /// \return The ConstTable itself so the call can be chained
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ConstTable& Enum(const SQChar* name, Enumeration& en) {
        sq_pushobject(vm, GetObject());
        sq_pushstring(vm, name, -1);
        sq_pushobject(vm, en.GetObject());
        sq_newslot(vm, -3, false);
        sq_pop(vm,1); // pop table
        return *this;
    }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Register a dynamic amount of enumerations into the constants table.
///
/// \remarks
/// The number of enumerations have to be manually specified.
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline void RegisterEnumerationsPtr(HSQUIRRELVM vm, const EnumElements * elist, unsigned int count)
{
    // Grab a reference to the constants table
    ConstTable ct(vm);
    // Iterate over the specified enumerations
    for (unsigned int i = 0; i < count; ++i)
    {
        // Validate the enumeration name
        if (!(elist[i].Name ) || *(elist[i].Name) == '\0')
        {
            SQTHROW(vm, _SC("Invalid or empty enumeration name"));
        }
        // Prepare an enumeration table
        Enumeration e(vm);
        // Bind the enumeration values
        elist[i].Bind(e);
        // Bind the enumeration to the constant table
        ct.Enum(elist[i].Name, e);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Register a specific amount of enumerations identified at compile-time into the constants table.
///
/// \remarks
/// The number of enumerations will be automatically detected.
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template < size_t N > inline void RegisterEnumerations(HSQUIRRELVM vm, const EnumElements(&elist)[N])
{
    RegisterEnumerationsPtr(vm, elist, N); // Just forward the information
}

}

#endif
