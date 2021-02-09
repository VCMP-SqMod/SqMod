// ------------------------------------------------------------------------------------------------
#include "PocoLib/Data.hpp"

// ------------------------------------------------------------------------------------------------
#include <sqratConst.h>

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
SQMOD_DECL_TYPENAME(SqPcDataRecordSet, _SC("SqDataRecordSet"))
SQMOD_DECL_TYPENAME(SqPcDataStatementResult, _SC("SqDataStatementResult"))

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
            setProperty(name.ToStr(), Poco::Any(value.Cast< SQInteger >()));
        } break;
        case OT_FLOAT: {
            setProperty(name.ToStr(), Poco::Any(value.Cast< SQFloat >()));
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
SqDataRecordSet SqDataSession::GetRecordSet(StackStrF & data)
{
    return SqDataRecordSet(*this, data);
}

// ------------------------------------------------------------------------------------------------
SqDataSession & SqDataSession::Execute(StackStrF & query)
{
    // Create a statement instance
    Statement stmt(impl()->createStatementImpl());
    // Add the query to the
    stmt << (query);
    // Execute it
    stmt.execute();
    // Allow chaining
    return *this;
}

// ------------------------------------------------------------------------------------------------
SqDataSession & SqDataSession::ExecuteAsync(StackStrF & query)
{
    // Create a statement instance
    Statement stmt(impl()->createStatementImpl());
    // Add the query to the
    stmt << (query);
    // Execute it
    stmt.executeAsync();
    // Allow chaining
    return *this;
}

// ------------------------------------------------------------------------------------------------
void SqDataStatement::UseEx(LightObj & obj, const std::string & name, Poco::Data::AbstractBinding::Direction dir)
{
    //
    switch (obj.GetType())
    {
        case OT_NULL: addBind(new Poco::Data::Binding<Poco::Data::NullData>(const_cast<Poco::Data::NullData&>(g_NullData), name, dir)); break;
        case OT_INTEGER:
        case OT_FLOAT:
        case OT_BOOL:
        case OT_STRING: STHROWF("Use Bind(...) for non-reference types."); break;
        case OT_INSTANCE: UseInst_(obj, name, dir); break;
        default: STHROWF("Can't use ({}) values", SqTypeName(obj.GetType())); break;
    }
}

// ------------------------------------------------------------------------------------------------
void SqDataStatement::UseInst_(LightObj & obj, const std::string & name, Poco::Data::AbstractBinding::Direction dir)
{
    auto type = static_cast< AbstractStaticClassData * >(obj.GetTypeTag());
    // Integer reference?
    if (type == StaticClassTypeTag< SqDataBinding< SQInteger > >::Get())
    {
        addBind(new Poco::Data::ReferenceBinding< SQInteger >(obj.CastI< SqDataBinding< SQInteger > >()->mV, name, dir));
    } // Float reference?
    else if (type == StaticClassTypeTag< SqDataBinding< SQFloat > >::Get())
    {
        addBind(new Poco::Data::ReferenceBinding< SQFloat >(obj.CastI< SqDataBinding< SQFloat > >()->mV, name, dir));
    } // String reference?
    else if (type == StaticClassTypeTag< SqDataBinding< String > >::Get())
    {
        addBind(new Poco::Data::ReferenceBinding< String >(obj.CastI< SqDataBinding< String > >()->mV, name, dir));
    } // Bool reference?
    else if (type == StaticClassTypeTag< SqDataBinding< bool > >::Get())
    {
        addBind(new Poco::Data::ReferenceBinding< bool >(obj.CastI< SqDataBinding< bool > >()->mV, name, dir));
    } // Integer vector reference?
    else if (type == StaticClassTypeTag< SqVector< SQInteger > >::Get())
    {
        auto p = obj.CastI< SqVector< SQInteger > >();
        addBind(new Poco::Data::ReferenceBinding< std::vector< SQInteger > >(p->ValidRef(), name, dir));
    } // Float vector reference?
    else if (type == StaticClassTypeTag< SqVector< SQFloat > >::Get())
    {
        auto p = obj.CastI< SqVector< SQFloat > >();
        addBind(new Poco::Data::ReferenceBinding< std::vector< SQFloat > >(p->ValidRef(), name, dir));
    } // String vector reference?
    else if (type == StaticClassTypeTag< SqVector< String > >::Get())
    {
        auto p = obj.CastI< SqVector< String > >();
        addBind(new Poco::Data::ReferenceBinding< std::vector< String > >(p->ValidRef(), name, dir));
    } // Bool vector reference?
    else if (type == StaticClassTypeTag< SqVector< bool > >::Get())
    {
        // There is no point in having these
        // Their usefulness is limited and pointless compared to the number of specializations needed get them to work
        STHROWF("Boolean vectors are not implemented");
    } // Unknown!
    else
    {
        Var< LightObj >::push(SqVM(), obj);
        String type_name = SqTypeName(SqVM(), -1);
        sq_poptop(SqVM());
        STHROWF("Can't use {}) values", type_name);
    }
}

// ------------------------------------------------------------------------------------------------
void SqDataStatement::BindEx(LightObj & obj, const std::string & name, Poco::Data::AbstractBinding::Direction dir)
{
    // Identify the object type
    switch (obj.GetType())
    {
        // Null?
        case OT_NULL: {
            addBind(new Poco::Data::Binding<Poco::Data::NullData>(const_cast<Poco::Data::NullData&>(g_NullData), name, dir));
        } break;
        // Integer?
        case OT_INTEGER: {
            auto v = obj.Cast< SQInteger >();
            addBind(new Poco::Data::CopyBinding< SQInteger >(v, name, dir));
        } break;
        // Float?
        case OT_FLOAT: {
            auto v = obj.Cast< SQFloat >();
            addBind(new Poco::Data::CopyBinding< SQFloat >(v, name, dir));
        } break;
        // Bool?
        case OT_BOOL: {
            auto v = obj.Cast<bool>();
            addBind(new Poco::Data::CopyBinding<bool>(v, name, dir));
        } break;
        // String?
        case OT_STRING: {
            Var< LightObj >::push(SqVM(), obj);
            StackStrF str(SqVM(), -1);
            str.Proc(false);
            sq_poptop(SqVM());
            addBind(new Poco::Data::CopyBinding<const char *>(str.mPtr, name, dir));
        } break;
        // Special?
        case OT_INSTANCE: BindInst_(obj, name, dir); break;
        default: STHROWF("Can't bind ({}) values", SqTypeName(obj.GetType())); break;
    }
}

// ------------------------------------------------------------------------------------------------
void SqDataStatement::BindInst_(LightObj & obj, const std::string & name, Poco::Data::AbstractBinding::Direction dir)
{
    auto type = static_cast< AbstractStaticClassData * >(obj.GetTypeTag());
    // Integer reference?
    if (type == StaticClassTypeTag< SqDataBinding< SQInteger > >::Get())
    {
        addBind(new Poco::Data::CopyBinding< SQInteger >(*obj.CastI< SqDataBinding< SQInteger > >()->mV, name, dir));
    } // Float reference?
    else if (type == StaticClassTypeTag< SqDataBinding< SQFloat > >::Get())
    {
        addBind(new Poco::Data::CopyBinding< SQFloat >(*obj.CastI< SqDataBinding< SQFloat > >()->mV, name, dir));
    } // String reference?
    else if (type == StaticClassTypeTag< SqDataBinding< String > >::Get())
    {
        addBind(new Poco::Data::CopyBinding< String >(*obj.CastI< SqDataBinding< String > >()->mV, name, dir));
    } // Bool reference?
    else if (type == StaticClassTypeTag< SqDataBinding< bool > >::Get())
    {
        addBind(new Poco::Data::CopyBinding< bool >(*obj.CastI< SqDataBinding< bool > >()->mV, name, dir));
    } // Integer vector reference?
    else if (type == StaticClassTypeTag< SqVector< SQInteger > >::Get())
    {
        addBind(new Poco::Data::CopyBinding< std::vector< SQInteger > >(obj.CastI< SqVector< SQInteger > >()->Valid(), name, dir));
    } // Float vector reference?
    else if (type == StaticClassTypeTag< SqVector< SQFloat > >::Get())
    {
        addBind(new Poco::Data::CopyBinding< std::vector< SQFloat > >(obj.CastI< SqVector< SQFloat > >()->Valid(), name, dir));
    } // String vector reference?
    else if (type == StaticClassTypeTag< SqVector< String > >::Get())
    {
        addBind(new Poco::Data::CopyBinding< std::vector< String > >(obj.CastI< SqVector< String > >()->Valid(), name, dir));
    } // Bool vector reference?
    else if (type == StaticClassTypeTag< SqVector< bool > >::Get())
    {
        // There is no point in having these
        // Their usefulness is limited and pointless compared to the number of specializations needed get them to work
        STHROWF("Boolean vectors are not implemented");
    } // Unknown!
    else
    {
        Var< LightObj >::push(SqVM(), obj);
        String type_name = SqTypeName(SqVM(), -1);
        sq_poptop(SqVM());
        STHROWF("Can't bind ({}) values", type_name);
    }
}

// ------------------------------------------------------------------------------------------------
SqDataStatement & SqDataStatement::Into(LightObj & obj)
{
    auto type = static_cast< AbstractStaticClassData * >(obj.GetTypeTag());
    // Integer reference?
    if (type == StaticClassTypeTag< SqDataBinding< SQInteger > >::Get())
    {
        addExtract(new Poco::Data::ReferenceExtraction< SQInteger >(obj.CastI< SqDataBinding< SQInteger > >()->mV));
    } // Float reference?
    else if (type == StaticClassTypeTag< SqDataBinding< SQFloat > >::Get())
    {
        addExtract(new Poco::Data::ReferenceExtraction< SQFloat >(obj.CastI< SqDataBinding< SQFloat > >()->mV));
    } // String reference?
    else if (type == StaticClassTypeTag< SqDataBinding< String > >::Get())
    {
        addExtract(new Poco::Data::ReferenceExtraction< String >(obj.CastI< SqDataBinding< String > >()->mV));
    } // Bool reference?
    else if (type == StaticClassTypeTag< SqDataBinding< bool > >::Get())
    {
        addExtract(new Poco::Data::ReferenceExtraction< bool >(obj.CastI< SqDataBinding< bool > >()->mV));
    } // Integer vector reference?
    else if (type == StaticClassTypeTag< SqVector< SQInteger > >::Get())
    {
        addExtract(new Poco::Data::ReferenceExtraction< std::vector< SQInteger > >(obj.CastI< SqVector< SQInteger > >()->ValidRef()));
    } // Float vector reference?
    else if (type == StaticClassTypeTag< SqVector< SQFloat > >::Get())
    {
        addExtract(new Poco::Data::ReferenceExtraction< std::vector< SQFloat > >(obj.CastI< SqVector< SQFloat > >()->ValidRef()));
    } // String vector reference?
    else if (type == StaticClassTypeTag< SqVector< String > >::Get())
    {
        addExtract(new Poco::Data::ReferenceExtraction< std::vector< String > >(obj.CastI< SqVector< String > >()->ValidRef()));
    } // Bool vector reference?
    else if (type == StaticClassTypeTag< SqVector< bool > >::Get())
    {
        // There is no point in having these
        // Their usefulness is limited and pointless compared to the number of specializations needed get them to work
        STHROWF("Boolean vectors are not implemented");
    } // Unknown!
    else
    {
        Var< LightObj >::push(SqVM(), obj);
        String type_name = SqTypeName(SqVM(), -1);
        sq_poptop(SqVM());
        STHROWF("Can't extract ({}) values", type_name);
    }
    return *this;
}

// ------------------------------------------------------------------------------------------------
SqDataStatement & SqDataStatement::Into_(LightObj & obj, LightObj & def)
{
    auto type = static_cast< AbstractStaticClassData * >(obj.GetTypeTag());
    // Integer reference?
    if (type == StaticClassTypeTag< SqDataBinding< SQInteger > >::Get())
    {
        addExtract(new Poco::Data::ReferenceExtraction< SQInteger >(obj.CastI< SqDataBinding< SQInteger > >()->mV, def.Cast< SQInteger >()));
    } // Float reference?
    else if (type == StaticClassTypeTag< SqDataBinding< SQFloat > >::Get())
    {
        addExtract(new Poco::Data::ReferenceExtraction< SQFloat >(obj.CastI< SqDataBinding< SQFloat > >()->mV, def.Cast< SQFloat >()));
    } // String reference?
    else if (type == StaticClassTypeTag< SqDataBinding< String > >::Get())
    {
        addExtract(new Poco::Data::ReferenceExtraction< String >(obj.CastI< SqDataBinding< String > >()->mV, def.Cast< String >()));
    } // Bool reference?
    else if (type == StaticClassTypeTag< SqDataBinding< bool > >::Get())
    {
        addExtract(new Poco::Data::ReferenceExtraction< bool >(obj.CastI< SqDataBinding< bool > >()->mV, def.Cast< bool >()));
    } // Integer vector reference?
    else if (type == StaticClassTypeTag< SqVector< SQInteger > >::Get())
    {
        addExtract(new Poco::Data::ReferenceExtraction< std::vector< SQInteger > >(obj.CastI< SqVector< SQInteger > >()->ValidRef(), def.Cast< SQInteger >()));
    } // Float vector reference?
    else if (type == StaticClassTypeTag< SqVector< SQFloat > >::Get())
    {
        addExtract(new Poco::Data::ReferenceExtraction< std::vector< SQFloat > >(obj.CastI< SqVector< SQFloat > >()->ValidRef(), def.Cast< SQFloat >()));
    } // String vector reference?
    else if (type == StaticClassTypeTag< SqVector< String > >::Get())
    {
        addExtract(new Poco::Data::ReferenceExtraction< std::vector< String > >(obj.CastI< SqVector< String > >()->ValidRef(), def.Cast< String >()));
    } // Bool vector reference?
    else if (type == StaticClassTypeTag< SqVector< bool > >::Get())
    {
        // There is no point in having these
        // Their usefulness is limited and pointless compared to the number of specializations needed get them to work
        STHROWF("Boolean vectors are not implemented");
    } // Unknown!
    else
    {
        Var< LightObj >::push(SqVM(), obj);
        String type_name = SqTypeName(SqVM(), -1);
        sq_poptop(SqVM());
        STHROWF("Can't extract ({}) values", type_name);
    }
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
        .Func(_SC("Use"), &Binding::Use)
        .Func(_SC("UseAs"), &Binding::UseAs)
    );
}

// ------------------------------------------------------------------------------------------------
static void ProcessPocoData()
{
    // Go over all statement results and try to update them
    for (SqDataStatementResult * inst = SqDataStatementResult::sHead; inst && inst->mNext != SqDataStatementResult::sHead; inst = inst->mNext)
    {
        if (inst->mRes.available())
        {
            // Forward the callback with the result
            if (!inst->mFunc.IsNull())
            {
                inst->mFunc.Execute(inst->mStmt, inst->mRes.data());
            }
            // Stop processing this result
            inst->UnchainInstance();
            // Release script resources
            inst->mFunc.Release();
            inst->mStmt.Release();
            inst->mSelf.Release();
        }
    }
}

// ------------------------------------------------------------------------------------------------
void TerminatePocoData()
{
    // Go over all statement results and try to update them
    for (SqDataStatementResult * inst = SqDataStatementResult::sHead; inst && inst->mNext != SqDataStatementResult::sHead; inst = inst->mNext)
    {
        // Release associated resources
        inst->mFunc.Release();
        inst->mStmt.Release();
        inst->mSelf.Release();
    }
}

// ================================================================================================
void Register_POCO_Data(HSQUIRRELVM vm, Table &)
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
        .Func(_SC("RecordSet"), &SqDataSession::GetRecordSet)
        .Func(_SC("Begin"), &SqDataSession::Begin)
        .Func(_SC("Commit"), &SqDataSession::Commit)
        .Func(_SC("Rollback"), &SqDataSession::Rollback)
        .Func(_SC("HasTransactionIsolation"), &SqDataSession::HasTransactionIsolation)
        .Func(_SC("IsTransactionIsolation"), &SqDataSession::IsTransactionIsolation)
        .FmtFunc(_SC("SetFeature"), &SqDataSession::SetFeature)
        .FmtFunc(_SC("GetFeature"), &SqDataSession::GetFeature)
        .FmtFunc(_SC("SetProperty"), &SqDataSession::SetProperty)
        .FmtFunc(_SC("GetProperty"), &SqDataSession::GetProperty)
        .FmtFunc(_SC("Execute"), &SqDataSession::Execute)
        .FmtFunc(_SC("ExecuteAsync"), &SqDataSession::ExecuteAsync)
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
    ns.Bind(_SC("StatementResult"),
        Class< SqDataStatementResult, NoConstructor< SqDataStatementResult > >(vm, SqPcDataStatementResult::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqPcDataStatementResult::Fn)
        // Member Methods
        .CbFunc(_SC("Bind"), &SqDataStatementResult::Bind)
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
        .Prop(_SC("StorageID"), &SqDataStatement::Storage)
        .Prop(_SC("Storage"), &SqDataStatement::GetStorage, &SqDataStatement::SetStorage)
        .Prop(_SC("CanModifyStorage"), &SqDataStatement::CanModifyStorage)
        .Prop(_SC("ColumnsExtracted"), &SqDataStatement::ColumnsExtracted)
        .Prop(_SC("RowsExtracted"), &SqDataStatement::RowsExtracted)
        .Prop(_SC("TotalRowCount"), &SqDataStatement::SubTotalRowCount)
        .Prop(_SC("ExtractionCount"), &SqDataStatement::ExtractionCount)
        .Prop(_SC("DataSetCount"), &SqDataStatement::DataSetCount)
        .Prop(_SC("NextDataSet"), &SqDataStatement::NextDataSet)
        .Prop(_SC("PreviousDataSet"), &SqDataStatement::PreviousDataSet)
        .Prop(_SC("HasMoreDataSets"), &SqDataStatement::HasMoreDataSets)
        // Member Methods
        .Func(_SC("Add"), &SqDataStatement::Add)
        .Func(_SC("SetAsync"), &SqDataStatement::SetAsync)
        .Func(_SC("Reset"), &SqDataStatement::Reset)
        .Func(_SC("Use"), &SqDataStatement::Use)
        .Func(_SC("UseAs"), &SqDataStatement::UseAs)
        .Func(_SC("In"), &SqDataStatement::In)
        .Func(_SC("InAs"), &SqDataStatement::InAs)
        .Func(_SC("Out"), &SqDataStatement::Out)
        .Func(_SC("OutAs"), &SqDataStatement::OutAs)
        .Func(_SC("Bind"), &SqDataStatement::Bind)
        .Func(_SC("BindAs"), &SqDataStatement::BindAs)
        .Func(_SC("Io"), &SqDataStatement::Io)
        .Func(_SC("GetColumnsExtracted"), &SqDataStatement::GetColumnsExtracted)
        .Func(_SC("GetRowsExtracted"), &SqDataStatement::GetRowsExtracted)
        .Func(_SC("GetSubTotalRowCount"), &SqDataStatement::GetSubTotalRowCount)
        // Overloaded Member Methods
        .Overload(_SC("Execute"), &SqDataStatement::Execute)
        .Overload(_SC("Execute"), &SqDataStatement::Execute_)
        .Overload(_SC("Execute_"), &SqDataStatement::ExecuteChained)
        .Overload(_SC("Execute_"), &SqDataStatement::ExecuteChained_)
        .Overload(_SC("ExecuteAsync"), &SqDataStatement::ExecuteAsync)
        .Overload(_SC("ExecuteAsync"), &SqDataStatement::ExecuteAsync_)
        .Overload(_SC("ExecuteAsync_"), &SqDataStatement::ExecuteAsyncChained)
        .Overload(_SC("ExecuteAsync_"), &SqDataStatement::ExecuteAsyncChained_)
        .Overload(_SC("Into"), &SqDataStatement::Into)
        .Overload(_SC("Into"), &SqDataStatement::Into_)
        .Overload(_SC("Limit"), &SqDataStatement::Limit1)
        .Overload(_SC("Limit"), &SqDataStatement::Limit2)
        .Overload(_SC("Limit"), &SqDataStatement::Limit3)
        .Overload(_SC("Range"), &SqDataStatement::Range)
        .Overload(_SC("Range"), &SqDataStatement::RangeEx)
        // Static Values
        .SetStaticValue(_SC("Unlimited"), static_cast< SQInteger >(Poco::Data::Limit::LIMIT_UNLIMITED))
        .SetStaticValue(_SC("WaitForever"), static_cast< SQInteger >(SqDataStatement::WAIT_FOREVER))
        .SetStaticValue(_SC("UseCurrentDataSet"), static_cast< SQInteger >(Poco::Data::StatementImpl::USE_CURRENT_DATA_SET))
        .SetStaticValue(_SC("StorageDeque"), static_cast< SQInteger >(SqDataStatement::STORAGE_DEQUE))
        .SetStaticValue(_SC("StorageVector"), static_cast< SQInteger >(SqDataStatement::STORAGE_VECTOR))
        .SetStaticValue(_SC("StorageList"), static_cast< SQInteger >(SqDataStatement::STORAGE_LIST))
        .SetStaticValue(_SC("StorageUnknown"), static_cast< SQInteger >(SqDataStatement::STORAGE_UNKNOWN))
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("RecordSet"),
        Class< SqDataRecordSet >(vm, SqPcDataRecordSet::Str)
        // Constructors
        .Ctor< SqDataSession &, StackStrF & >()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqPcDataRecordSet::Fn)
        // Properties
        .Prop(_SC("RowCount"), &SqDataRecordSet::RowCount)
        .Prop(_SC("ExtractedRowCount"), &SqDataRecordSet::ExtractedRowCount)
        .Prop(_SC("TotalRowCount"), &SqDataRecordSet::GetTotalRowCount, &SqDataRecordSet::SetTotalRowCount)
        .Prop(_SC("ColumnCount"), &SqDataRecordSet::ColumnCount)
        .Prop(_SC("IsFiltered"), &SqDataRecordSet::IsFiltered)
        // Member Methods
        .FmtFunc(_SC("SetTotalRowCount"), &SqDataRecordSet::SetTotalRowCountQ)
        .Func(_SC("First"), &SqDataRecordSet::MoveFirst)
        .Func(_SC("Next"), &SqDataRecordSet::MoveNext)
        .Func(_SC("Previous"), &SqDataRecordSet::MovePrevious)
        .Func(_SC("Last"), &SqDataRecordSet::MoveLast)
        .Func(_SC("Reset"), &SqDataRecordSet::Reset)
        .Func(_SC("ColumnTypeAt"), &SqDataRecordSet::ColumnTypeAt)
        .Func(_SC("ColumnType"), &SqDataRecordSet::ColumnType)
        .Func(_SC("ColumnName"), &SqDataRecordSet::ColumnName)
        .Func(_SC("ColumnLengthAt"), &SqDataRecordSet::ColumnLengthAt)
        .Func(_SC("ColumnLength"), &SqDataRecordSet::ColumnLength)
        .Func(_SC("ColumnPrecisionAt"), &SqDataRecordSet::ColumnPrecisionAt)
        .Func(_SC("ColumnPrecision"), &SqDataRecordSet::ColumnPrecision)
        .Func(_SC("IsNull"), &SqDataRecordSet::IsNull)
        // Overloaded Member Methods
        .Overload(_SC("ValueAt"), &SqDataRecordSet::GetValueAt)
        .Overload(_SC("ValueAt"), &SqDataRecordSet::GetValueAtOr)
        .Overload(_SC("Value"), &SqDataRecordSet::GetValue)
        .Overload(_SC("Value"), &SqDataRecordSet::GetValueOr)
    );
    // --------------------------------------------------------------------------------------------
    ns.Func(_SC("Process"), ProcessPocoData);
    // --------------------------------------------------------------------------------------------
    Register_POCO_Data_Binding< SQInteger, SqIntegerBinding >(vm, ns, _SC("IntBind"));
    Register_POCO_Data_Binding< String, SqStringBinding >(vm, ns, _SC("StrBind"));
    Register_POCO_Data_Binding< SQFloat, SqFloatBinding >(vm, ns, _SC("FloatBind"));
    Register_POCO_Data_Binding< bool, SqBoolBinding >(vm, ns, _SC("BoolBind"));

    RootTable(vm).Bind(_SC("SqData"), ns);

    // --------------------------------------------------------------------------------------------
    ConstTable(vm).Enum(_SC("SqDataColumnType"), Enumeration(vm)
        .Const(_SC("Bool"),         static_cast< SQInteger >(Poco::Data::MetaColumn::FDT_BOOL))
        .Const(_SC("Int8"),         static_cast< SQInteger >(Poco::Data::MetaColumn::FDT_INT8))
        .Const(_SC("Uint8"),        static_cast< SQInteger >(Poco::Data::MetaColumn::FDT_UINT8))
        .Const(_SC("Int16"),        static_cast< SQInteger >(Poco::Data::MetaColumn::FDT_INT16))
        .Const(_SC("Uint16"),       static_cast< SQInteger >(Poco::Data::MetaColumn::FDT_UINT16))
        .Const(_SC("Int32"),        static_cast< SQInteger >(Poco::Data::MetaColumn::FDT_INT32))
        .Const(_SC("Uint32"),       static_cast< SQInteger >(Poco::Data::MetaColumn::FDT_UINT32))
        .Const(_SC("Int64"),        static_cast< SQInteger >(Poco::Data::MetaColumn::FDT_INT64))
        .Const(_SC("Uint64"),       static_cast< SQInteger >(Poco::Data::MetaColumn::FDT_UINT64))
        .Const(_SC("Float"),        static_cast< SQInteger >(Poco::Data::MetaColumn::FDT_FLOAT))
        .Const(_SC("Double"),       static_cast< SQInteger >(Poco::Data::MetaColumn::FDT_DOUBLE))
        .Const(_SC("String"),       static_cast< SQInteger >(Poco::Data::MetaColumn::FDT_STRING))
        .Const(_SC("WString"),      static_cast< SQInteger >(Poco::Data::MetaColumn::FDT_WSTRING))
        .Const(_SC("Blob"),         static_cast< SQInteger >(Poco::Data::MetaColumn::FDT_BLOB))
        .Const(_SC("Clob"),         static_cast< SQInteger >(Poco::Data::MetaColumn::FDT_CLOB))
        .Const(_SC("Date"),         static_cast< SQInteger >(Poco::Data::MetaColumn::FDT_DATE))
        .Const(_SC("Time"),         static_cast< SQInteger >(Poco::Data::MetaColumn::FDT_TIME))
        .Const(_SC("TimeStamp"),    static_cast< SQInteger >(Poco::Data::MetaColumn::FDT_TIMESTAMP))
        .Const(_SC("Unknown"),      static_cast< SQInteger >(Poco::Data::MetaColumn::FDT_UNKNOWN))
    );
}

} // Namespace:: SqMod
