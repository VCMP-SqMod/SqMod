#pragma once

// ------------------------------------------------------------------------------------------------
#include "Core/Utility.hpp"
#include "Library/Utils/Vector.hpp"

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
    void reset() override
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

/* ------------------------------------------------------------------------------------------------
 * Implementation of AbstractBinding for shared ownership binding of values.
 * Because we cannot take references to script variables, we use this as a proxy.
*/
template <class T> struct ReferenceBinding<std::vector<T>> : public AbstractBinding
{
    using ValType = std::vector<T>;
    using ValPtr = SharedPtr<ValType>;
    using Ptr = SharedPtr<Binding<ValType>>;
    using Iterator = typename ValType::const_iterator;

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    explicit ReferenceBinding(const ValPtr& val,  const std::string& name = "", Direction direction = PD_IN)
        : AbstractBinding(name, direction), m_Value(val), m_Begin(), m_End()
    {
        if (PD_IN == direction && m_Value->size() == 0)
        {
            throw BindingException("It is illegal to bind to an empty data collection");
        }
        m_Begin = m_Value->begin();
        m_End   = m_Value->end();
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
        return m_Value->size();
    }

    /* --------------------------------------------------------------------------------------------
     * Check if binding is available.
    */
    SQMOD_NODISCARD bool canBind() const override
    {
        return (m_Begin != m_End);
    }

    /* --------------------------------------------------------------------------------------------
     * Bind the value.
    */
    void bind(std::size_t pos) override
    {
        poco_assert_dbg(!getBinder().isNull());
        poco_assert_dbg(canBind());
        TypeHandler<T>::bind(pos, *m_Begin, getBinder(), getDirection());
        ++m_Begin;
    }

    /* --------------------------------------------------------------------------------------------
     * Reset the binding.
    */
    void reset() override
    {
        m_Begin = m_Value->begin();
        m_End   = m_Value->end();
    }

private:

    ValPtr      m_Value;
    Iterator    m_Begin;
    Iterator    m_End;
};

/* ------------------------------------------------------------------------------------------------
 * Implementation of AbstractExtraction for shared ownership binding of values.
 * Because we cannot take references to script variables, we use this as a proxy.
*/
template <class T>
class ReferenceExtraction: public AbstractExtraction
{
public:
    using ValType = T;
    using Result = SharedPtr<T>;
    using ValPtr = SharedPtr<ValType>;
    using Type = Extraction<ValType>;
    using Ptr = SharedPtr<Type>;

    /* --------------------------------------------------------------------------------------------
     * Creates an Extraction object at specified position. Uses an empty object T as default value.
    */
    explicit ReferenceExtraction(const Result& result, const Position& pos = Position(0))
        : AbstractExtraction(Limit::LIMIT_UNLIMITED, pos.value())
        , m_Result(result),m_Default(),m_Extracted(false),m_Null(false)
    {
    }

    /* --------------------------------------------------------------------------------------------
     * Creates an Extraction object at specified position. Uses the provided def object as default value.
    */
    ReferenceExtraction(const Result& result, const T& def, const Position& pos = Position(0))
        : AbstractExtraction(Limit::LIMIT_UNLIMITED, pos.value())
        , m_Result(result), m_Default(def), m_Extracted(false), m_Null(false)
    {
    }

    /* --------------------------------------------------------------------------------------------
     * Creates an Extraction object at specified position. Uses the provided def object as default value.
    */
    ReferenceExtraction(const Result& result, T&& def, const Position& pos = Position(0))
        : AbstractExtraction(Limit::LIMIT_UNLIMITED, pos.value())
        , m_Result(result), m_Default(std::move(def)), m_Extracted(false), m_Null(false)
    {
    }

    /* --------------------------------------------------------------------------------------------
     * Destroys the Extraction object.
    */
    ~ReferenceExtraction() override = default;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the number of columns handled.
    */
    SQMOD_NODISCARD std::size_t numOfColumnsHandled() const override
    {
        return TypeHandler<T>::size();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the number of rows handled.
    */
    SQMOD_NODISCARD std::size_t numOfRowsHandled() const override
    {
        return m_Extracted ? 1u : 0;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the number of rows allowed.
    */
    SQMOD_NODISCARD std::size_t numOfRowsAllowed() const override
    {
        return 1u;
    }

    /* --------------------------------------------------------------------------------------------
     * Check if null.
    */
    SQMOD_NODISCARD bool isNull(std::size_t /*row*/) const override
    {
        return m_Null;
    }

    /* --------------------------------------------------------------------------------------------
     * Extract the value.
    */
    std::size_t extract(std::size_t pos) override
    {
        if (m_Extracted)
        {
            throw ExtractException("value already extracted");
        }

        m_Extracted = true;
        AbstractExtractor::Ptr pExt = getExtractor();
        TypeHandler<T>::extract(pos, *m_Result, m_Default, pExt);
        m_Null = isValueNull<T>(*m_Result, pExt->isNull(pos));

        return 1u;
    }

    /* --------------------------------------------------------------------------------------------
     * Reset state.
    */
    void reset() override
    {
        m_Extracted = false;
    }

    /* --------------------------------------------------------------------------------------------
     * See if a value was extracted.
    */
    SQMOD_NODISCARD bool canExtract() const override
    {
        return !m_Extracted;
    }

    /* --------------------------------------------------------------------------------------------
     * Create a preparation instance for this type.
    */
    AbstractPreparation::Ptr createPreparation(AbstractPreparator::Ptr& pPrep, std::size_t pos) override
    {
        return new Preparation<T>(pPrep, pos, *m_Result);
    }

private:

    Result  m_Result;
    T       m_Default;
    bool    m_Extracted;
    bool    m_Null;
};

/* ------------------------------------------------------------------------------------------------
 * Implementation of AbstractExtraction for shared ownership binding of values.
 * Because we cannot take references to script variables, we use this as a proxy.
*/
template <class T>
class ReferenceExtraction<std::vector<T>>: public AbstractExtraction
{
public:
    using ValType = std::vector<T>;
    using Result = SharedPtr<std::vector<T>>;
    using ValPtr = SharedPtr<ValType>;
    using Type = Extraction<ValType>;
    using Ptr = SharedPtr<Type>;

    /* --------------------------------------------------------------------------------------------
     * Creates an Extraction object at specified position. Uses an empty object T as default value.
    */
    explicit ReferenceExtraction(const Result& result, const Position& pos = Position(0))
        : AbstractExtraction(Limit::LIMIT_UNLIMITED, pos.value())
        , m_Result(result), m_Default(), m_Nulls()
    {
        m_Result->clear();
    }

    /* --------------------------------------------------------------------------------------------
     * Creates an Extraction object at specified position. Uses the provided def object as default value.
    */
    ReferenceExtraction(const Result& result, const T& def, const Position& pos = Position(0))
        : AbstractExtraction(Limit::LIMIT_UNLIMITED, pos.value())
        , m_Result(result), m_Default(def), m_Nulls()
    {
        m_Result->clear();
    }

    /* --------------------------------------------------------------------------------------------
     * Destroys the Extraction object.
    */
    ~ReferenceExtraction() override = default;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the number of columns handled.
    */
    SQMOD_NODISCARD std::size_t numOfColumnsHandled() const override
    {
        return TypeHandler<T>::size();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the number of rows handled.
    */
    SQMOD_NODISCARD std::size_t numOfRowsHandled() const override
    {
        return static_cast<std::size_t>(m_Result->size());
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the number of rows allowed.
    */
    SQMOD_NODISCARD std::size_t numOfRowsAllowed() const override
    {
        return getLimit();
    }

    /* --------------------------------------------------------------------------------------------
     * Check if null.
    */
    SQMOD_NODISCARD bool isNull(std::size_t row) const override
    {
        try
        {
            return m_Nulls.at(row);
        }
        catch (std::out_of_range& ex)
        {
            throw RangeException(ex.what());
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Extract the value.
    */
    std::size_t extract(std::size_t pos) override
    {
        AbstractExtractor::Ptr ext = getExtractor();
        m_Result->push_back(m_Default);
        TypeHandler<T>::extract(pos, m_Result->back(), m_Default, ext);
        m_Nulls.push_back(isValueNull(m_Result->back(), ext->isNull(pos)));
        return 1u;
    }

    /* --------------------------------------------------------------------------------------------
     * Create a preparation instance for this type.
    */
    AbstractPreparation::Ptr createPreparation(AbstractPreparator::Ptr& pPrep, std::size_t pos) override
    {
        return new Preparation<T>(pPrep, pos, m_Default);
    }

    /* --------------------------------------------------------------------------------------------
     * Reset state.
    */
    void reset() override
    {
        m_Nulls.clear();
    }

protected:

    /* --------------------------------------------------------------------------------------------
     * Retrieve the container with the extracted values.
    */
    const std::vector<T>& result() const
    {
        return *m_Result;
    }

private:
    Result              m_Result;
    T                   m_Default;
    std::deque< bool >  m_Nulls;
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
     * Use the value to a statement.
    */
    SqDataBinding & Use(SqDataStatement & stmt);

    /* --------------------------------------------------------------------------------------------
     * Use the value to a statement with a specific name.
    */
    SqDataBinding & UseAs(SqDataStatement & stmt, StackStrF & name);

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
     * Sets the asynchronous flag. This setting does not affect the statement's capability
     * to be executed synchronously by directly calling Execute().
    */
    SqDataStatement & SetAsync(bool async)
    {
        setAsync(async);
        return *this;
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
     * Returns true if statement is in a state that allows the internal storage to be modified.
    */
    bool CanModifyStorage()
    {
        return canModifyStorage();
    }

    /* --------------------------------------------------------------------------------------------
     * Returns the internal storage type for the statement.
    */
    SQInteger Storage() const
    {
        return static_cast< SQInteger >(storage());
    }

    /* --------------------------------------------------------------------------------------------
     * Returns the internal storage type for the statement.
    */
    const std::string & GetStorage() const
    {
        return getStorage();
    }

    /* --------------------------------------------------------------------------------------------
     * Sets the internal storage type for the statement.
    */
    void SetStorage(StackStrF & storage)
    {
        setStorage(storage.ToStr());
    }

    /* --------------------------------------------------------------------------------------------
     * Returns the number of columns returned for current data set.
     * Default value indicates current data set (if any).
    */
    SQInteger GetColumnsExtracted(int data_set) const
    {
        return static_cast< SQInteger >(columnsExtracted(data_set));
    }
    SQInteger ColumnsExtracted() const
    {
        return static_cast< SQInteger >(columnsExtracted());
    }

    /* --------------------------------------------------------------------------------------------
     * Returns the number of rows returned for current data set during last statement execution.
     * Default value indicates current data set (if any).
    */
    SQInteger GetRowsExtracted(int data_set) const
    {
        return static_cast< SQInteger >(rowsExtracted(data_set));
    }
    SQInteger RowsExtracted() const
    {
        return static_cast< SQInteger >(rowsExtracted());
    }

    /* --------------------------------------------------------------------------------------------
     * Returns the number of rows extracted so far for the data set.
     * Default value indicates current data set (if any).
    */
    SQInteger GetSubTotalRowCount(int data_set) const
    {
        return static_cast< SQInteger >(subTotalRowCount(data_set));
    }
    SQInteger SubTotalRowCount() const
    {
        return static_cast< SQInteger >(subTotalRowCount());
    }

    /* --------------------------------------------------------------------------------------------
     * Returns the number of extraction storage buffers associated with the current data set.
    */
    SQInteger ExtractionCount() const
    {
        return static_cast< SQInteger >(extractionCount());
    }

    /* --------------------------------------------------------------------------------------------
     * Returns the number of data sets associated with the statement.
    */
    SQInteger DataSetCount() const
    {
        return static_cast< SQInteger >(dataSetCount());
    }

    /* --------------------------------------------------------------------------------------------
     * Returns the index of the next data set.
    */
    SQInteger NextDataSet()
    {
        return static_cast< SQInteger >(nextDataSet());
    }

    /* --------------------------------------------------------------------------------------------
     * Returns the index of the previous data set.
    */
    SQInteger PreviousDataSet()
    {
        return static_cast< SQInteger >(previousDataSet());
    }

    /* --------------------------------------------------------------------------------------------
     * Returns false if the current data set index points to the last data set. Otherwise, it returns true.
    */
    bool HasMoreDataSets() const
    {
        return hasMoreDataSets();
    }

    /* --------------------------------------------------------------------------------------------
     * Executes the statement synchronously or asynchronously.
    */
    SQInteger Execute()
    {
        return static_cast< SQInteger >(execute(true));
    }

    /* --------------------------------------------------------------------------------------------
     * Executes the statement synchronously or asynchronously.
    */
    SQInteger Execute_(bool reset)
    {
        return static_cast< SQInteger >(execute(reset));
    }

    /* --------------------------------------------------------------------------------------------
     * Executes the statement asynchronously.
    */
    SqDataStatement & ExecuteAsync()
    {
        executeAsync(true);
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Executes the statement asynchronously.
    */
    SqDataStatement & ExecuteAsync_(bool reset)
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
     * Swaps the statement with another one.
    */
    void Swap(SqDataStatement & stmt)
    {
        swap(stmt);
    }

    /* --------------------------------------------------------------------------------------------
     * Bind a reference to the statement.
    */
    SqDataStatement & Use(LightObj & obj)
    {
        UseEx(obj, String(), Poco::Data::AbstractBinding::PD_IN);
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Bind a named reference to the statement.
    */
    SqDataStatement & UseAs(LightObj & obj, StackStrF & name)
    {
        UseEx(obj, String(name.mPtr, name.GetSize()), Poco::Data::AbstractBinding::PD_IN);
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Internal function used internally to bind a reference to the statement.
    */
    void UseEx(LightObj & obj, const std::string & name, Poco::Data::AbstractBinding::Direction dir);

    /* --------------------------------------------------------------------------------------------
     * Internal function used internally to bind a instance reference to the statement.
    */
    void UseInst_(LightObj & obj, const std::string & name, Poco::Data::AbstractBinding::Direction dir);

    /* --------------------------------------------------------------------------------------------
     * Bind a value to the statement and mark it as input (i.e alias of Use).
    */
    SqDataStatement & In(LightObj & obj)
    {
        return Use(obj);
    }

    /* --------------------------------------------------------------------------------------------
     * Bind a named value to the statement mark it as input (i.e alias of UseAs).
    */
    SqDataStatement & InAs(LightObj & obj, StackStrF & name)
    {
        return UseAs(obj, name);
    }

    /* --------------------------------------------------------------------------------------------
     * Bind a reference to the statement and mark it as output.
    */
    SqDataStatement & Out(LightObj & obj)
    {
        UseEx(obj, String(), Poco::Data::AbstractBinding::PD_OUT);
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Bind a named reference to the statement and mark it as output.
    */
    SqDataStatement & OutAs(LightObj & obj, StackStrF & name)
    {
        UseEx(obj, String(name.mPtr, name.GetSize()), Poco::Data::AbstractBinding::PD_OUT);
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Bind a value to the statement.
    */
    SqDataStatement & Bind(LightObj & obj)
    {
        BindEx(obj, String(), Poco::Data::AbstractBinding::PD_IN);
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Bind a named value to the statement.
    */
    SqDataStatement & BindAs(LightObj & obj, StackStrF & name)
    {
        BindEx(obj, String(name.mPtr, name.GetSize()), Poco::Data::AbstractBinding::PD_IN);
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Internal function used internally to bind a value to the statement.
    */
    void BindEx(LightObj & obj, const std::string & name, Poco::Data::AbstractBinding::Direction dir);

    /* --------------------------------------------------------------------------------------------
     * Internal function used internally to bind a instance value to the statement.
    */
    void BindInst_(LightObj & obj, const std::string & name, Poco::Data::AbstractBinding::Direction dir);

    /* --------------------------------------------------------------------------------------------
     * Bind a value to the statement and mark it as input/output.
    */
    SqDataStatement & Io(LightObj & obj)
    {
        if (obj.GetType() == OT_INSTANCE)
        {
            UseInst_(obj, String(), Poco::Data::AbstractBinding::PD_IN_OUT);
        }
        else
        {
            Bind(obj);
        }
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Register a single extraction with the statement.
    */
    SqDataStatement & Into(LightObj & obj);

    /* --------------------------------------------------------------------------------------------
     * Register a single extraction with the statement with a default value.
    */
    SqDataStatement & Into_(LightObj & obj, LightObj & def);

    /* --------------------------------------------------------------------------------------------
     * Sets a limit on the maximum number of rows a select is allowed to return.
    */
    SqDataStatement & Limit1(SQInteger limit) { return Limit3(limit, false, false); }
    SqDataStatement & Limit2(SQInteger limit, bool hard) { return Limit3(limit, hard, false); }
    SqDataStatement & Limit3(SQInteger limit, bool hard, bool lower)
    {
        (*this), Poco::Data::Limit(static_cast< Poco::Data::Limit::SizeT >(limit), hard, lower);
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Sets a an extraction range for the maximum number of rows a select is allowed to return.
    */
    SqDataStatement & Range(SQInteger lower, SQInteger upper) { return RangeEx(lower, upper, false); }
    SqDataStatement & RangeEx(SQInteger lower, SQInteger upper, bool hard)
    {
        (*this), Poco::Data::Range(static_cast< Poco::Data::Limit::SizeT >(lower),
                                    static_cast< Poco::Data::Limit::SizeT >(upper), hard);
        return *this;
    }
};

// ------------------------------------------------------------------------------------------------
template < class T > inline SqDataBinding< T > & SqDataBinding< T >::Use(SqDataStatement & stmt)
{
    stmt.addBind(new Poco::Data::ReferenceBinding< T >(mV, String(), Poco::Data::AbstractBinding::PD_IN));
    return *this;
}

// ------------------------------------------------------------------------------------------------
template < class T > inline SqDataBinding< T > & SqDataBinding< T >::UseAs(SqDataStatement & stmt, StackStrF & name)
{
    stmt.addBind(new Poco::Data::ReferenceBinding< T >(mV, name.ToStr(), Poco::Data::AbstractBinding::PD_IN));
    return *this;
}

// ------------------------------------------------------------------------------------------------
template < class T > inline SqDataBinding< T > & SqDataBinding< T >::Bind(SqDataStatement & stmt)
{
    stmt.addBind(new Poco::Data::CopyBinding< T >(*mV, String(), Poco::Data::AbstractBinding::PD_IN));
    return *this;
}

// ------------------------------------------------------------------------------------------------
template < class T > inline SqDataBinding< T > & SqDataBinding< T >::BindAs(SqDataStatement & stmt, StackStrF & name)
{
    stmt.addBind(new Poco::Data::CopyBinding< T >(*mV, name.ToStr(), Poco::Data::AbstractBinding::PD_IN));
    return *this;
}

} // Namespace:: SqMod
