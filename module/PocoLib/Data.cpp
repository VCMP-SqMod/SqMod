// ------------------------------------------------------------------------------------------------
#include "PocoLib/Data.hpp"

// ------------------------------------------------------------------------------------------------
#ifdef SQMOD_POCO_HAS_SQLITE
    #include <Poco/Data/SQLite/Connector.h>
#endif
#ifdef SQMOD_POCO_HAS_MYSQL
    #include <Poco/Data/MySQL/Connector.h>
#endif
#ifdef SQMOD_POCO_HAS_POSTGRESQL
    #include <Poco/Data/PostgreSQL/Connector.h>
#endif
// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQMOD_DECL_TYPENAME(SqIntegerBinding, _SC("SqIntegerBinding"))
SQMOD_DECL_TYPENAME(SqStringBinding, _SC("SqStringBinding"))
SQMOD_DECL_TYPENAME(SqFloatBinding, _SC("SqFloatBinding"))
SQMOD_DECL_TYPENAME(SqBoolBinding, _SC("SqBoolBinding"))

// ------------------------------------------------------------------------------------------------
SQMOD_DECL_TYPENAME(SqPcDataSession, _SC("SqDataSession"))
SQMOD_DECL_TYPENAME(SqPcDataStatement, _SC("SqDataStatement"))

// ------------------------------------------------------------------------------------------------
static const Poco::Data::NullData g_NullData{Poco::NULL_GENERIC};

// ------------------------------------------------------------------------------------------------
void InitializePocoDataConnectors()
{
#ifdef SQMOD_POCO_HAS_SQLITE
    Poco::Data::SQLite::Connector::registerConnector();
#endif
#ifdef SQMOD_POCO_HAS_MYSQL
    Poco::Data::MySQL::Connector::registerConnector();
#endif
#ifdef SQMOD_POCO_HAS_POSTGRESQL
    Poco::Data::PostgreSQL::Connector::registerConnector();
#endif
}

// ------------------------------------------------------------------------------------------------
void SqDataSession::SetProperty(const LightObj & value, StackStrF & name)
{
    switch (value.GetType())
    {
        case OT_NULL: {
            setProperty(name.ToStr(), Poco::Any(nullptr));
        } break;
        case OT_INTEGER: {
            setProperty(name.ToStr(), Poco::Any(value.Cast<SQInteger>()));
        } break;
        case OT_FLOAT: {
            setProperty(name.ToStr(), Poco::Any(value.Cast<SQFloat>()));
        } break;
        case OT_BOOL: {
            setProperty(name.ToStr(), Poco::Any(value.Cast<bool>()));
        } break;
        case OT_STRING: {
            setProperty(name.ToStr(), Poco::Any(value.Cast<std::string>()));
        } break;
        default: STHROWF("Unsupported property value type");
    }
}

// ------------------------------------------------------------------------------------------------
LightObj SqDataSession::GetProperty(StackStrF & name) const
{
    HSQUIRRELVM vm = name.mVM;
    // Preserve stack
    const StackGuard sg(vm);
    // Retrieve the property value
    Poco::Any a = getProperty(name.ToStr());
    // Retrieve the value type
    const auto & ti = a.type();
    // Identify the stored value
    if (a.empty() || ti == typeid(void) || ti == typeid(nullptr)) sq_pushnull(vm);
    else if (ti == typeid(bool)) sq_pushbool(vm, Poco::AnyCast< bool >(a));
    else if (ti == typeid(char)) sq_pushinteger(vm, ConvTo< SQInteger >::From(Poco::AnyCast< char >(a)));
    else if (ti == typeid(wchar_t)) sq_pushinteger(vm, ConvTo< SQInteger >::From(Poco::AnyCast< wchar_t >(a)));
    else if (ti == typeid(signed char)) sq_pushinteger(vm, ConvTo< SQInteger >::From(Poco::AnyCast< signed char >(a)));
    else if (ti == typeid(unsigned char)) sq_pushinteger(vm, ConvTo< SQInteger >::From(Poco::AnyCast< unsigned char >(a)));
    else if (ti == typeid(short int)) sq_pushinteger(vm, ConvTo< SQInteger >::From(Poco::AnyCast< short int >(a)));
    else if (ti == typeid(unsigned short int)) sq_pushinteger(vm, ConvTo< SQInteger >::From(Poco::AnyCast< unsigned short int >(a)));
    else if (ti == typeid(int)) sq_pushinteger(vm, ConvTo< SQInteger >::From(Poco::AnyCast< int >(a)));
    else if (ti == typeid(unsigned int)) sq_pushinteger(vm, ConvTo< SQInteger >::From(Poco::AnyCast< unsigned int >(a)));
    else if (ti == typeid(long int)) sq_pushinteger(vm, ConvTo< SQInteger >::From(Poco::AnyCast< long int >(a)));
    else if (ti == typeid(unsigned long int)) sq_pushinteger(vm, ConvTo< SQInteger >::From(Poco::AnyCast< unsigned long int >(a)));
    else if (ti == typeid(long long int)) sq_pushinteger(vm, ConvTo< SQInteger >::From(Poco::AnyCast< long long int >(a)));
    else if (ti == typeid(unsigned long long int)) sq_pushinteger(vm, ConvTo< SQInteger >::From(Poco::AnyCast< unsigned long long int >(a)));
    else if (ti == typeid(float)) sq_pushfloat(vm, ConvTo< SQFloat >::From(Poco::AnyCast< float >(a)));
    else if (ti == typeid(double)) sq_pushfloat(vm, ConvTo< SQFloat >::From(Poco::AnyCast< double >(a)));
    else if (ti == typeid(long double)) sq_pushfloat(vm, ConvTo< SQFloat >::From(Poco::AnyCast< long double >(a)));
    else if (ti == typeid(std::string)) {
        const auto & s = Poco::RefAnyCast< std::string >(a);
        sq_pushstring(vm, s.data(), ConvTo< SQFloat >::From(s.size()));
    } else {
        sq_throwerrorf(vm, "Unable to convert value of type (%s) to squirrel.", a.type().name());
        sq_pushnull(vm);
    }
    // Return the object from the stack
    return Var< LightObj >(vm, -1).value;

}

// ------------------------------------------------------------------------------------------------
SqDataStatement SqDataSession::GetStatement(StackStrF & data)
{
    return SqDataStatement(*this, data);
}

// ------------------------------------------------------------------------------------------------
SqDataStatement & SqDataStatement::UseEx(LightObj & obj, const std::string & name, Poco::Data::AbstractBinding::Direction dir)
{
    //
    switch (obj.GetType())
    {
        case OT_NULL: addBind(new Poco::Data::Binding<Poco::Data::NullData>(const_cast<Poco::Data::NullData&>(g_NullData), name, dir)); break;
        case OT_INTEGER:
        case OT_FLOAT:
        case OT_BOOL:
        case OT_STRING: STHROWF("Use Bind(...) for non-reference types."); break;
        case OT_INSTANCE: {
            auto type = static_cast< AbstractStaticClassData * >(obj.GetTypeTag());
            // Integer reference?
            if (type == StaticClassTypeTag< SqDataBinding< SQInteger > >::Get())
            {
                addBind(new Poco::Data::ReferenceBinding< SQInteger >(obj.CastI< SqDataBinding< SQInteger > >()->mV, name, dir)); break;
            } // Float reference?
            else if (type == StaticClassTypeTag< SqDataBinding< SQFloat > >::Get())
            {
                addBind(new Poco::Data::ReferenceBinding< SQFloat >(obj.CastI< SqDataBinding< SQFloat > >()->mV, name, dir)); break;
            } // String reference?
            else if (type == StaticClassTypeTag< SqDataBinding< String > >::Get())
            {
                addBind(new Poco::Data::ReferenceBinding< String >(obj.CastI< SqDataBinding< String > >()->mV, name, dir)); break;
            } // Bool reference?
            else if (type == StaticClassTypeTag< SqDataBinding< bool > >::Get())
            {
                addBind(new Poco::Data::ReferenceBinding< bool >(obj.CastI< SqDataBinding< bool > >()->mV, name, dir)); break;
            } // Unknown!
            else
            {
                Var< LightObj >::push(SqVM(), obj);
                String type_name = SqTypeName(SqVM(), -1);
                sq_poptop(SqVM());
                STHROWF("Can't use (%s) values", type_name.c_str()); break;
            }

        } break;
        default: STHROWF("Can't use (%s) values", SqTypeName(obj.GetType())); break;
    }
    //
    return *this;
}

// ------------------------------------------------------------------------------------------------
SqDataStatement & SqDataStatement::BindEx(LightObj & obj, const std::string & name, Poco::Data::AbstractBinding::Direction dir)
{
    //
    switch (obj.GetType())
    {
        case OT_NULL: {
            addBind(new Poco::Data::Binding<Poco::Data::NullData>(const_cast<Poco::Data::NullData&>(g_NullData), name, dir));
        } break;
        case OT_INTEGER: {
            auto v = obj.Cast<SQInteger>();
            addBind(new Poco::Data::CopyBinding<SQInteger>(v, name, dir));
        } break;
        case OT_FLOAT: {
            auto v = obj.Cast<SQFloat>();
            addBind(new Poco::Data::CopyBinding<SQFloat>(v, name, dir));
        } break;
        case OT_BOOL: {
            auto v = obj.Cast<bool>();
            addBind(new Poco::Data::CopyBinding<bool>(v, name, dir));
        } break;
        case OT_STRING: {
            Var< LightObj >::push(SqVM(), obj);
            StackStrF str(SqVM(), -1);
            str.Proc(false);
            sq_poptop(SqVM());
            addBind(new Poco::Data::CopyBinding<const char *>(str.mPtr, name, dir));
        } break;
        case OT_INSTANCE: {
            auto type = static_cast< AbstractStaticClassData * >(obj.GetTypeTag());
            // Integer reference?
            if (type == StaticClassTypeTag< SqDataBinding< SQInteger > >::Get())
            {
                //addBind(new Poco::Data::ReferenceBinding< SQInteger >(obj.CastI< SqDataBinding< SQInteger > >()->mV, name, dir)); break;
            } // Float reference?
            else if (type == StaticClassTypeTag< SqDataBinding< SQFloat > >::Get())
            {
                //addBind(new Poco::Data::ReferenceBinding< SQFloat >(obj.CastI< SqDataBinding< SQFloat > >()->mV, name, dir)); break;
            } // String reference?
            else if (type == StaticClassTypeTag< SqDataBinding< String > >::Get())
            {
                //addBind(new Poco::Data::ReferenceBinding< String >(obj.CastI< SqDataBinding< String > >()->mV, name, dir)); break;
            } // Bool reference?
            else if (type == StaticClassTypeTag< SqDataBinding< bool > >::Get())
            {
                //addBind(new Poco::Data::ReferenceBinding< bool >(obj.CastI< SqDataBinding< bool > >()->mV, name, dir)); break;
            } // Unknown!
            else
            {
                Var< LightObj >::push(SqVM(), obj);
                String type_name = SqTypeName(SqVM(), -1);
                sq_poptop(SqVM());
                STHROWF("Can't use (%s) values", type_name.c_str()); break;
            }

        } break;
        default: STHROWF("Can't use (%s) values", SqTypeName(obj.GetType())); break;
    }
    //
    return *this;
}

// ------------------------------------------------------------------------------------------------
template < class T, class U >
static void Register_POCO_Data_Binding(HSQUIRRELVM vm, Table & ns, const SQChar * name)
{
    using Binding = SqDataBinding< T >;
    // --------------------------------------------------------------------------------------------
    ns.Bind(name,
        Class< Binding, NoCopy< Binding > >(vm, U::Str)
        // Constructors
        .Ctor()
        .template Ctor()
        .template Ctor< typename Binding::OptimalArg >()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &U::Fn)
        // Properties
        .Prop(_SC("V"), &Binding::Get, &Binding::Set)
        .Prop(_SC("Value"), &Binding::Get, &Binding::Set)
        // Member Methods
        .Func(_SC("Set"), &Binding::SetEx)
        .Func(_SC("Bind"), &Binding::Bind)
        .Func(_SC("BindAs"), &Binding::BindAs)
    );
}

// ================================================================================================
void Register_POCO_Data(HSQUIRRELVM vm)
{
    Table ns(vm);

    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("Session"),
        Class< SqDataSession >(vm, SqPcDataSession::Str)
        // Constructors
        .Ctor< StackStrF &, SQInteger >()
        .Ctor< StackStrF &, StackStrF &, SQInteger >()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqPcDataSession::Fn)
        // Properties
        .Prop(_SC("IsConnected"), &SqDataSession::IsConnected)
        .Prop(_SC("IsGood"), &SqDataSession::IsGood)
        .Prop(_SC("LoginTimeout"), &SqDataSession::GetLoginTimeout, &SqDataSession::SetLoginTimeout)
        .Prop(_SC("ConnectionTimeout"), &SqDataSession::GetConnectionTimeout, &SqDataSession::SetConnectionTimeout)
        .Prop(_SC("CanTransact"), &SqDataSession::CanTransact)
        .Prop(_SC("IsTransaction"), &SqDataSession::IsTransaction)
        .Prop(_SC("TransactionIsolation"), &SqDataSession::GetTransactionIsolation, &SqDataSession::SetTransactionIsolation)
        .Prop(_SC("Connector"), &SqDataSession::GetConnector)
        .Prop(_SC("URI"), &SqDataSession::GetURI)
        // Member Methods
        .FmtFunc(_SC("Open"), &SqDataSession::Open)
        .Func(_SC("Close"), &SqDataSession::Close)
        .Func(_SC("Reconnect"), &SqDataSession::Reconnect)
        .Func(_SC("Statement"), &SqDataSession::GetStatement)
        .Func(_SC("Begin"), &SqDataSession::Begin)
        .Func(_SC("Commit"), &SqDataSession::Commit)
        .Func(_SC("Rollback"), &SqDataSession::Rollback)
        .Func(_SC("HasTransactionIsolation"), &SqDataSession::HasTransactionIsolation)
        .Func(_SC("IsTransactionIsolation"), &SqDataSession::IsTransactionIsolation)
        .FmtFunc(_SC("SetFeature"), &SqDataSession::SetFeature)
        .FmtFunc(_SC("GetFeature"), &SqDataSession::GetFeature)
        .FmtFunc(_SC("SetProperty"), &SqDataSession::SetProperty)
        .FmtFunc(_SC("GetProperty"), &SqDataSession::GetProperty)
        // Static Functions
        .StaticFunc(_SC("GetURI"), &SqDataSession::BuildURI)
        // Static Values
        .SetStaticValue(_SC("LoginTimeoutDefault"), static_cast< SQInteger >(Session::LOGIN_TIMEOUT_DEFAULT))
        .SetStaticValue(_SC("TransactionReadUncommitted"), static_cast< SQInteger >(Session::TRANSACTION_READ_UNCOMMITTED))
        .SetStaticValue(_SC("TransactionReadCommitted"), static_cast< SQInteger >(Session::TRANSACTION_READ_COMMITTED))
        .SetStaticValue(_SC("TransactionRepeatableRead"), static_cast< SQInteger >(Session::TRANSACTION_REPEATABLE_READ))
        .SetStaticValue(_SC("TransactionSerializable"), static_cast< SQInteger >(Session::TRANSACTION_SERIALIZABLE))
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("Statement"),
        Class< SqDataStatement >(vm, SqPcDataStatement::Str)
        // Constructors
        .Ctor< SqDataSession & >()
        .Ctor< SqDataSession &, StackStrF & >()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqPcDataStatement::Fn)
        // Properties
        .Prop(_SC("Async"), &SqDataStatement::GetAsync, &SqDataStatement::SetAsync)
        .Prop(_SC("Initialized"), &SqDataStatement::Initialized)
        .Prop(_SC("Paused"), &SqDataStatement::Paused)
        .Prop(_SC("Done"), &SqDataStatement::Done)
        // Member Methods
        .Func(_SC("Add"), &SqDataStatement::Add)
        .Func(_SC("Execute"), &SqDataStatement::Execute)
        .Func(_SC("ExecuteAsync"), &SqDataStatement::ExecuteAsync)
        .Func(_SC("SetAsync"), &SqDataStatement::SetAsync)
        .Func(_SC("Reset"), &SqDataStatement::Reset)
        .Func(_SC("Use"), &SqDataStatement::Use)
        .Func(_SC("UseAs"), &SqDataStatement::UseAs)
        .Func(_SC("Bind"), &SqDataStatement::Bind)
        .Func(_SC("BindAs"), &SqDataStatement::BindAs)
    );
    // --------------------------------------------------------------------------------------------
    Register_POCO_Data_Binding< SQInteger, SqIntegerBinding >(vm, ns, _SC("IntBind"));
    Register_POCO_Data_Binding< String, SqStringBinding >(vm, ns, _SC("StrBind"));
    Register_POCO_Data_Binding< SQFloat, SqFloatBinding >(vm, ns, _SC("FloatBind"));
    Register_POCO_Data_Binding< bool, SqBoolBinding >(vm, ns, _SC("BoolBind"));

    RootTable(vm).Bind(_SC("SqData"), ns);
}

} // Namespace:: SqMod
