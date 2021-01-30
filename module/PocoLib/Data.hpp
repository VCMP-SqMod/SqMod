#pragma once

// ------------------------------------------------------------------------------------------------
#include "Core/Utility.hpp"

// ------------------------------------------------------------------------------------------------
#include <Poco/Data/Session.h>
#include <Poco/Data/Statement.h>
#include <Poco/Data/RecordSet.h>

// ------------------------------------------------------------------------------------------------
namespace std { // NOLINT(cert-dcl58-cpp)

/* ------------------------------------------------------------------------------------------------
 * Allows the StackStrF to write its data into a std::ostream type.
*/
inline std::ostream & operator << (std::ostream & out, const ::Sqrat::StackStrF & str)
{
    if (str.mLen)
    {
        out.write(str.mPtr, str.mLen);
    }
    return out;
}

/* ------------------------------------------------------------------------------------------------
 * Allows the StackStrF to write its data into a std::ostringstream type.
*/
inline std::ostringstream & operator << (std::ostringstream & out, const ::Sqrat::StackStrF & str)
{
    if (str.mLen)
    {
        out.write(str.mPtr, str.mLen);
    }
    return out;
}

} // Namespace:: std

// ------------------------------------------------------------------------------------------------
namespace Poco { // NOLINT(modernize-concat-nested-namespaces)
namespace Data {

/* ------------------------------------------------------------------------------------------------
 * Implementation of AbstractBinding for shared ownership binding of values.
 * Because we cannot take references to script variables, we use this as a proxy.
*/
template <class T> struct ReferenceBinding : public AbstractBinding
{
    using ValType = T;
    using ValPtr = SharedPtr<ValType>;
    using Type = ReferenceBinding<ValType>;
    using Ptr = SharedPtr<Type>;

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    explicit ReferenceBinding(const ValPtr& val,  const std::string& name = "", Direction direction = PD_IN)
        : AbstractBinding(name, direction), m_Value(val), m_Bound(false)
    {
    }

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~ReferenceBinding() override = default;

    /* --------------------------------------------------------------------------------------------
     * Retrieve columns occupied.
    */
    SQMOD_NODISCARD std::size_t numOfColumnsHandled() const override
    {
        return TypeHandler<T>::size();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve rows occupied.
    */
    SQMOD_NODISCARD std::size_t numOfRowsHandled() const override
    {
        return 1;
    }

    /* --------------------------------------------------------------------------------------------
     * Check if binding is available.
    */
    SQMOD_NODISCARD bool canBind() const override
    {
        return !m_Bound;
    }

    /* --------------------------------------------------------------------------------------------
     * Bind the value.
    */
    void bind(std::size_t pos) override
    {
        poco_assert_dbg(!getBinder().isNull());
        TypeHandler<T>::bind(pos, *m_Value, getBinder(), getDirection());
        m_Bound = true;
    }

    /* --------------------------------------------------------------------------------------------
     * Reset the binding.
    */
    void reset () override
    {
        m_Bound = false;
        AbstractBinder::Ptr pBinder = getBinder();
        poco_assert_dbg (!pBinder.isNull());
        pBinder->reset();
    }

private:

    ValPtr m_Value;
    bool   m_Bound;
};

} // Namespace:: Data
} // Namespace:: Poco

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
using namespace Poco::Data::Keywords;
// ------------------------------------------------------------------------------------------------
using Poco::Data::Session;
using Poco::Data::Statement;
using Poco::Data::RecordSet;
using Poco::Data::ReferenceBinding;

// ------------------------------------------------------------------------------------------------
struct SqDataSession;
struct SqDataStatement;

/* ------------------------------------------------------------------------------------------------
 * Utility used to transform optimal argument type to stored type.
*/
template < class T > struct SqDataBindingOpt
{
    /* --------------------------------------------------------------------------------------------
     * Optimal argument type. Unchanged when not specialized.
    */
    using Type = T;

    /* --------------------------------------------------------------------------------------------
     * Convert the optimal type to the stored type. Does nothing special in this case.
    */
    inline static Type & Get(Type & v) { return v; }
    inline static const Type & Get(const Type & v) { return v; }
    // --------------------------------------------------------------------------------------------
    inline static void Put(T & o, Type & v) { o = v; }
    inline static void Put(T & o, const Type & v) { o = v; }
};

/* ------------------------------------------------------------------------------------------------
 * Specialization of SqDataBindingOpt for std::string type.
*/
template < > struct SqDataBindingOpt< String >
{
    /* --------------------------------------------------------------------------------------------
     * Optimal argument type.
    */
    using Type = StackStrF;

    /* --------------------------------------------------------------------------------------------
     * Convert the optimal type to the stored type.
    */
    inline static String Get(Type & v) { return v.ToStr(); }
    inline static String Get(const Type & v) { return v.ToStr(); }
    // --------------------------------------------------------------------------------------------
    inline static void Put(String & o, Type & v) { o.assign(v.mPtr, v.GetSize()); }
    inline static void Put(String & o, const Type & v) { o.assign(v.mPtr, v.GetSize()); }
};

/* ------------------------------------------------------------------------------------------------
 * Wrapper around values that must be passed as reference. Manual lifetime management!
*/
template < class T > struct SqDataBinding
{
    /* --------------------------------------------------------------------------------------------
     * Reference binding type.
    */
    using Binding = ReferenceBinding< T >;

    /* --------------------------------------------------------------------------------------------
     * Optimal type to receive a value of this type as function argument. Mainly for strings.
    */
    using OptimalType = typename SqDataBindingOpt< T >::Type;

    /* --------------------------------------------------------------------------------------------
     * Same as OptimalType but preferably with a reference qualifier to avoid copies.
    */
    using OptimalArg = typename std::conditional< std::is_same< T, OptimalType >::value, T, OptimalType & >::type;

    /* --------------------------------------------------------------------------------------------
     * Reference value.
    */
    typename Binding::ValPtr mV{};

    /* --------------------------------------------------------------------------------------------
     * Default constructor. Initializes to a default value.
    */
    SqDataBinding()
        : mV(new T())
    {
    }

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor. Initializes to a specific value.
    */
    explicit SqDataBinding(OptimalArg v)
        : mV(new T())
    {
        SqDataBindingOpt< T >::Put(*mV, v);
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    SqDataBinding(const SqDataBinding &) = default;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    SqDataBinding(SqDataBinding &&) noexcept = default;

    /* --------------------------------------------------------------------------------------------
     * Destroys the Statement.
    */
    ~SqDataBinding() = default;

    /* --------------------------------------------------------------------------------------------
     * Assignment operator.
    */
    SqDataBinding & operator = (const SqDataBinding &) = default;

    /* --------------------------------------------------------------------------------------------
     * Move assignment.
    */
    SqDataBinding & operator = (SqDataBinding &&) noexcept = default;

    /* --------------------------------------------------------------------------------------------
     * Retrieve a value from the instance.
    */
    SQMOD_NODISCARD const T & Get() const { return *mV; }

    /* --------------------------------------------------------------------------------------------
     * Modify a value from the instance.
    */
    void Set(OptimalArg v) { SqDataBindingOpt< T >::Put(*mV, v); }

    /* --------------------------------------------------------------------------------------------
     * Modify a value from the instance.
    */
    SqDataBinding & SetEx(OptimalArg v) { SqDataBindingOpt< T >::Put(*mV, v); return *this; }

    /* --------------------------------------------------------------------------------------------
     * Bind the value to a statement.
    */
    SqDataBinding & Bind(SqDataStatement & stmt);

    /* --------------------------------------------------------------------------------------------
     * Bind the value to a statement with a specific name.
    */
    SqDataBinding & BindAs(SqDataStatement & stmt, StackStrF & name);
};

/* ------------------------------------------------------------------------------------------------
 * A Session holds a connection to a Database and creates Statement objects.
*/
struct SqDataSession : public Session
{
    /* --------------------------------------------------------------------------------------------
     * Creates a new session, using the given connection (must be in "connection:///info" format).
    */
    SqDataSession(StackStrF & conn, SQInteger timeout)
        : Session(conn.ToStr(), ConvTo< size_t >::From(timeout))
    {
    }

    /* --------------------------------------------------------------------------------------------
     * Creates a new session, using the given connector, and connection information.
    */
    SqDataSession(StackStrF & conn, StackStrF & info, SQInteger timeout)
        : Session(conn.ToStr(), info.ToStr(), ConvTo< size_t >::From(timeout))
    {
    }

    /* --------------------------------------------------------------------------------------------
     * Creates a session by copying another one.
    */
	SqDataSession(const SqDataSession &) = default;

    /* --------------------------------------------------------------------------------------------
     * Creates a session by moving another one.
    */
	SqDataSession(SqDataSession &&) noexcept = default;

    /* --------------------------------------------------------------------------------------------
     * Destroys the Session.
    */
	~SqDataSession() = default;

    /* --------------------------------------------------------------------------------------------
     * Assignment operator.
    */
	SqDataSession & operator = (const SqDataSession &) = default;

    /* --------------------------------------------------------------------------------------------
     * Move assignment.
    */
	SqDataSession & operator = (SqDataSession &&) noexcept = default;

    /* --------------------------------------------------------------------------------------------
     * Opens the session using the supplied string.
     * Can also be used with default empty string to reconnect a disconnected session.
    */
	void Open(StackStrF & connect) { open(connect.ToStr()); }

    /* --------------------------------------------------------------------------------------------
     * Closes the session.
    */
	void Close() { close(); }

    /* --------------------------------------------------------------------------------------------
     * Returns true if session is connected, false otherwise.
    */
	bool IsConnected() { return isConnected(); }

    /* --------------------------------------------------------------------------------------------
     * Closes the session and opens it.
    */
	void Reconnect() { return reconnect(); }

    /* --------------------------------------------------------------------------------------------
     * Returns true if the session is good and can be used, false otherwise.
    */
	SQMOD_NODISCARD bool IsGood() { return isGood(); }

    /* --------------------------------------------------------------------------------------------
     * Sets the session login timeout value.
    */
    void SetLoginTimeout(SQInteger timeout) { setLoginTimeout(ConvTo< size_t >::From(timeout)); }

    /* --------------------------------------------------------------------------------------------
     * Returns the session login timeout value.
    */
	SQMOD_NODISCARD SQInteger GetLoginTimeout() const { return static_cast< SQInteger >(getLoginTimeout()); }

    /* --------------------------------------------------------------------------------------------
     * Sets the session connection timeout value.
    */
	void SetConnectionTimeout(SQInteger timeout) { setConnectionTimeout(ConvTo< size_t >::From(timeout)); }

    /* --------------------------------------------------------------------------------------------
     * Returns the session connection timeout value.
    */
	SQInteger GetConnectionTimeout() { return static_cast< SQInteger >(getConnectionTimeout()); }

    /* --------------------------------------------------------------------------------------------
     * Starts a transaction.
    */
	void Begin() { begin(); }

    /* --------------------------------------------------------------------------------------------
     * Commits and ends a transaction.
    */
	void Commit() { commit(); }

    /* --------------------------------------------------------------------------------------------
     * Rolls back and ends a transaction.
    */
	void Rollback() { rollback(); }

    /* --------------------------------------------------------------------------------------------
     * Returns true if session has transaction capabilities.
    */
	SQMOD_NODISCARD bool CanTransact() { return canTransact(); }

    /* --------------------------------------------------------------------------------------------
     * Returns true if a transaction is in progress, false otherwise.
    */
	SQMOD_NODISCARD bool IsTransaction() { return isTransaction(); }

    /* --------------------------------------------------------------------------------------------
     * Sets the transaction isolation level.
    */
	void SetTransactionIsolation(SQInteger ti) { setTransactionIsolation(static_cast< uint32_t >(ti)); }

    /* --------------------------------------------------------------------------------------------
     * Returns the transaction isolation level.
    */
	SQMOD_NODISCARD SQInteger GetTransactionIsolation() { return static_cast< SQInteger >(getTransactionIsolation()); }

    /* --------------------------------------------------------------------------------------------
     * Returns true if the transaction isolation level corresponding to the supplied bitmask is supported.
    */
	bool HasTransactionIsolation(SQInteger ti) { return hasTransactionIsolation(static_cast< uint32_t >(ti)); }

    /* --------------------------------------------------------------------------------------------
     * Returns true if the transaction isolation level corresponds to the supplied bitmask.
    */
	bool IsTransactionIsolation(SQInteger ti) { return isTransactionIsolation(static_cast< uint32_t >(ti)); }

    /* --------------------------------------------------------------------------------------------
     * Returns the connector name for this session.
    */
	SQMOD_NODISCARD std::string GetConnector() const { return connector(); }

    /* --------------------------------------------------------------------------------------------
     * Returns the URI for this session.
    */
	SQMOD_NODISCARD std::string GetURI() const { return uri(); }

    /* --------------------------------------------------------------------------------------------
     * Utility function that teturns the URI formatted from supplied arguments as "connector:///info".
    */
	SQMOD_NODISCARD static std::string BuildURI(StackStrF & connector, StackStrF & info)
	{
	    return uri(connector.ToStr(), info.ToStr());
	}

    /* --------------------------------------------------------------------------------------------
     * Set the state of a feature.
    */
	void SetFeature(bool state, StackStrF & name) { setFeature(name.ToStr(), state); }

    /* --------------------------------------------------------------------------------------------
     * Look up the state of a feature.
    */
	SQMOD_NODISCARD bool GetFeature(StackStrF & name) const { return getFeature(name.ToStr()); }

    /* --------------------------------------------------------------------------------------------
     * Set the value of a property.
    */
	void SetProperty(const LightObj & value, StackStrF & name);

    /* --------------------------------------------------------------------------------------------
     * Look up the value of a property.
    */
	SQMOD_NODISCARD LightObj GetProperty(StackStrF & name) const;

    /* --------------------------------------------------------------------------------------------
     * Look up the value of a property.
    */
    SQMOD_NODISCARD SqDataStatement GetStatement(StackStrF & data);
};

/* ------------------------------------------------------------------------------------------------
 * Statement is used to execute SQL statements.
*/
struct SqDataStatement : public Statement
{
    /* --------------------------------------------------------------------------------------------
     * Creates the Statement for the given Session.
    */
    explicit SqDataStatement(SqDataSession & session)
        : Statement(session)
    {
    }

    /* --------------------------------------------------------------------------------------------
     * Creates the Statement for the given Session and adds initial SQL code.
    */
    explicit SqDataStatement(SqDataSession & session, StackStrF & data)
        : Statement(session)
    {
        Add(data);
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. If the statement has been executed asynchronously and has not been
     * synchronized prior to copy operation (i.e. is copied while executing), this constructor shall synchronize it.
    */
    SqDataStatement(const SqDataStatement &) = default;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    SqDataStatement(SqDataStatement &&) noexcept = default;

    /* --------------------------------------------------------------------------------------------
     * Destroys the Statement.
    */
    ~SqDataStatement() = default;

    /* --------------------------------------------------------------------------------------------
     * Assignment operator.
    */
    SqDataStatement & operator = (const SqDataStatement &) = default;

    /* --------------------------------------------------------------------------------------------
     * Move assignment.
    */
    SqDataStatement & operator = (SqDataStatement &&) noexcept = default;

    /* --------------------------------------------------------------------------------------------
     * Concatenates data with the SQL statement string.
    */
    SqDataStatement & Add(StackStrF & data)
    {
        Statement::operator<<(data);
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Returns true if statement was marked for asynchronous execution.
    */
    bool GetAsync()
    {
        return isAsync();
    }

    /* --------------------------------------------------------------------------------------------
     * Returns true if the statement was initialized (i.e. not executed yet).
    */
    bool Initialized()
    {
        return initialized();
    }

    /* --------------------------------------------------------------------------------------------
     * Returns true if the statement was paused (a range limit stopped it and there is more work to do).
    */
    bool Paused()
    {
        return paused();
    }

    /* --------------------------------------------------------------------------------------------
     * Returns true if the statement was completely executed or false if a range limit stopped it
     * and there is more work to do. When no limit is set, it will always return true after calling execute().
    */
    bool Done()
    {
        return done();
    }

    /* --------------------------------------------------------------------------------------------
     * Sets the asynchronous flag. This setting does not affect the statement's capability
     * to be executed synchronously by directly calling Execute().
    */
    SqDataStatement & SetAsync(bool async)
    {
        setAsync(async);
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Executes the statement synchronously or asynchronously.
    */
    SQInteger Execute(bool reset)
    {
        return static_cast< SQInteger >(execute(reset));
    }

    /* --------------------------------------------------------------------------------------------
     * Executes the statement asynchronously.
    */
    SqDataStatement & ExecuteAsync(bool reset)
    {
        executeAsync(reset);
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Waits for the execution completion for asynchronous statements or returns immediately for synchronous ones.
    */
    SQInteger Wait(SQInteger milliseconds)
    {
        return static_cast< SQInteger >(wait(static_cast< long >(milliseconds)));
    }

    /* --------------------------------------------------------------------------------------------
     * Resets the Statement so that it can be filled with a new SQL command.
    */
    SqDataStatement & Reset(SqDataSession & session)
    {
        reset(session);
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * 
    */
    SqDataStatement & Use(LightObj & obj)
    {
        return  UseEx(obj, String(), Poco::Data::AbstractBinding::PD_IN);
    }

    /* --------------------------------------------------------------------------------------------
     * 
    */
    SqDataStatement & UseAs(LightObj & obj, StackStrF & name)
    {
        return  UseEx(obj, String(name.mPtr, name.GetSize()), Poco::Data::AbstractBinding::PD_IN);
    }

    /* --------------------------------------------------------------------------------------------
     *
    */
    SqDataStatement & UseEx(LightObj & obj, const std::string & name, Poco::Data::AbstractBinding::Direction dir);

    /* --------------------------------------------------------------------------------------------
     * 
    */
    SqDataStatement & Bind(LightObj & obj)
    {
        return  BindEx(obj, String(), Poco::Data::AbstractBinding::PD_IN);
    }

    /* --------------------------------------------------------------------------------------------
     * 
    */
    SqDataStatement & BindAs(LightObj & obj, StackStrF & name)
    {
        return  BindEx(obj, String(name.mPtr, name.GetSize()), Poco::Data::AbstractBinding::PD_IN);
    }

    /* --------------------------------------------------------------------------------------------
     *
    */
    SqDataStatement & BindEx(LightObj & obj, const std::string & name, Poco::Data::AbstractBinding::Direction dir);
};

// ------------------------------------------------------------------------------------------------
template < class T > inline SqDataBinding< T > & SqDataBinding< T >::Bind(SqDataStatement & stmt)
{
    return *this;
}

// ------------------------------------------------------------------------------------------------
template < class T > inline SqDataBinding< T > & SqDataBinding< T >::BindAs(SqDataStatement & stmt, StackStrF & name)
{
    return *this;
}

} // Namespace:: SqMod
