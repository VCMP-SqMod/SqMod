#pragma once

// ------------------------------------------------------------------------------------------------
#include "Core/Common.hpp"
#include "Core/ThreadPool.hpp"

// ------------------------------------------------------------------------------------------------
#include <cpr/cpr.h>

// ------------------------------------------------------------------------------------------------
#include <algorithm>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Wrapper for cpr::Options that can be bound to the script engine.
*/
struct CpSslOptions : public cpr::SslOptions
{
    using Base = cpr::SslOptions;
    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    CpSslOptions() = default;

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    CpSslOptions(const CpSslOptions &) = default;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    CpSslOptions(CpSslOptions &&) noexcept = default;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~CpSslOptions() = default;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    CpSslOptions & operator = (const CpSslOptions &) = default;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    CpSslOptions & operator = (CpSslOptions &&) noexcept = default;

    /* --------------------------------------------------------------------------------------------
     * Retrieve cpr::SslOptions::cert_file value.
    */
    SQMOD_NODISCARD const std::string & GetCertFile() const { return Base::cert_file; }

    /* --------------------------------------------------------------------------------------------
     * Assign cpr::SslOptions::cert_file and cpr::SslOptions::cert_type values.
    */
    void SetCertFile(StackStrF & type, StackStrF & file)
    {
        cert_file.assign(file.mPtr, file.GetSize());
        cert_type.assign(type.mPtr, type.GetSize());
    }

    /* --------------------------------------------------------------------------------------------
     * Assign cpr::SslOptions::cert_file values.
    */
    void SetPemCertFile(StackStrF & file)
    {
        cert_file.assign(file.mPtr, file.GetSize());
        cert_type.assign("PEM");
    }

    /* --------------------------------------------------------------------------------------------
     * Assign cpr::SslOptions::cert_file values.
    */
    void SetDerCertFile(StackStrF & file)
    {
        cert_file.assign(file.mPtr, file.GetSize());
        cert_type.assign("DER");
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve cpr::SslOptions::cert_type value.
    */
    SQMOD_NODISCARD const std::string & GetCertType() const { return Base::cert_type; }

    /* --------------------------------------------------------------------------------------------
     * Assign cpr::SslOptions::cert_type values.
    */
    void SetCertType(StackStrF & type) { cert_type.assign(type.mPtr, type.GetSize()); }

    /* --------------------------------------------------------------------------------------------
     * Retrieve cpr::SslOptions::key_file value.
    */
    SQMOD_NODISCARD const std::string & GetKeyFile() const { return Base::key_file; }

    /* --------------------------------------------------------------------------------------------
     * Assign cpr::SslOptions::cert_file, cpr::SslOptions::key_pass and cpr::SslOptions::cert_type values.
    */
    void SetKeyFile(StackStrF & type, StackStrF & pass, StackStrF & file)
    {
        key_file.assign(file.mPtr, file.GetSize());
        key_type.assign(type.mPtr, type.GetSize());
        key_pass.assign(pass.mPtr, pass.GetSize());
    }

    /* --------------------------------------------------------------------------------------------
     * Assign cpr::SslOptions::cert_file, cpr::SslOptions::key_pass values.
    */
    void SetPemKeyFile(StackStrF & type, StackStrF & pass, StackStrF & file)
    {
        key_file.assign(file.mPtr, file.GetSize());
        key_type.assign("PEM");
        key_pass.assign(pass.mPtr, pass.GetSize());
    }


    /* --------------------------------------------------------------------------------------------
     * Assign cpr::SslOptions::cert_file, cpr::SslOptions::key_pass values.
    */
    void SetDerKeyFile(StackStrF & type, StackStrF & pass, StackStrF & file)
    {
        key_file.assign(file.mPtr, file.GetSize());
        key_type.assign("DER");
        key_pass.assign(pass.mPtr, pass.GetSize());
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve cpr::SslOptions::key_type value.
    */
    SQMOD_NODISCARD const std::string & GetKeyType() const { return Base::key_type; }

    /* --------------------------------------------------------------------------------------------
     * Assign cpr::SslOptions::key_type values.
    */
    void SetKeyType(StackStrF & type) { key_type.assign(type.mPtr, type.GetSize()); }

    /* --------------------------------------------------------------------------------------------
     * Retrieve cpr::SslOptions::key_pass value.
    */
    SQMOD_NODISCARD const std::string & GetKeyPass() const { return Base::key_pass; }

    /* --------------------------------------------------------------------------------------------
     * Assign cpr::SslOptions::key_pass values.
    */
    void SetKeyPass(StackStrF & pass) { key_pass.assign(pass.mPtr, pass.GetSize()); }

#if SUPPORT_ALPN
    /* --------------------------------------------------------------------------------------------
     * Retrieve cpr::SslOptions::enable_alpn value.
    */
    SQMOD_NODISCARD bool GetALPN() const { return Base::enable_alpn; }

    /* --------------------------------------------------------------------------------------------
     * Modify cpr::SslOptions::enable_alpn value.
    */
    void SetALPN(bool value) { Base::enable_alpn = value; }
#else
    void GetALPN() const { STHROWF("Unsupported"); } // NOLINT(readability-convert-member-functions-to-static)
    void SetALPN(bool) { STHROWF("Unsupported"); } // NOLINT(readability-convert-member-functions-to-static)
#endif // SUPPORT_ALPN

#if SUPPORT_NPN
    /* --------------------------------------------------------------------------------------------
     * Retrieve cpr::SslOptions::enable_npn value.
    */
    SQMOD_NODISCARD bool GetNPM() const { return Base::enable_npn; }

    /* --------------------------------------------------------------------------------------------
     * Modify cpr::SslOptions::enable_npn value.
    */
    void SetNPM(bool value) { Base::enable_npn = value; }
#else
    void GetNPM() const { STHROWF("Unsupported"); } // NOLINT(readability-convert-member-functions-to-static)
    void SetNPM(bool) { STHROWF("Unsupported"); } // NOLINT(readability-convert-member-functions-to-static)
#endif // SUPPORT_NPN

    /* --------------------------------------------------------------------------------------------
     * Retrieve cpr::SslOptions::verify_host value.
    */
    SQMOD_NODISCARD bool GetVerifyHost() const { return Base::verify_host; }

    /* --------------------------------------------------------------------------------------------
     * Modify cpr::SslOptions::verify_host value.
    */
    void SetVerifyHost(bool value) { Base::verify_host = value; }

    /* --------------------------------------------------------------------------------------------
     * Retrieve cpr::SslOptions::verify_peer value.
    */
    SQMOD_NODISCARD bool GetVerifyPeer() const { return Base::verify_peer; }

    /* --------------------------------------------------------------------------------------------
     * Modify cpr::SslOptions::verify_peer value.
    */
    void SetVerifyPeer(bool value) { Base::verify_peer = value; }

    /* --------------------------------------------------------------------------------------------
     * Retrieve cpr::SslOptions::verify_status value.
    */
    SQMOD_NODISCARD bool GetVerifyStatus() const { return Base::verify_status; }

    /* --------------------------------------------------------------------------------------------
     * Modify cpr::SslOptions::verify_status value.
    */
    void SetVerifyStatus(bool value) { Base::verify_status = value; }

    /* --------------------------------------------------------------------------------------------
     * Retrieve cpr::SslOptions::ssl_version value.
    */
    SQMOD_NODISCARD int GetSslVersion() const { return Base::ssl_version; }

#if SUPPORT_MAX_TLS_VERSION
    /* --------------------------------------------------------------------------------------------
     * Retrieve cpr::SslOptions::max_version value.
    */
    SQMOD_NODISCARD int GetMaxVersion() const { return Base::max_version; }
#else
    void GetMaxVersion() const { STHROWF("Unsupported"); } // NOLINT(readability-convert-member-functions-to-static)
#endif // SUPPORT_MAX_TLS_VERSION

    /* --------------------------------------------------------------------------------------------
     * Retrieve cpr::SslOptions::ca_info value.
    */
    SQMOD_NODISCARD const std::string & GetCaInfo() const { return Base::ca_info; }

    /* --------------------------------------------------------------------------------------------
     * Assign cpr::SslOptions::ca_info values.
    */
    void SetCaInfo(StackStrF & info) { ca_info.assign(info.mPtr, info.GetSize()); }

    /* --------------------------------------------------------------------------------------------
     * Retrieve cpr::SslOptions::ca_path value.
    */
    SQMOD_NODISCARD const std::string & GetCaPath() const { return Base::ca_path; }

    /* --------------------------------------------------------------------------------------------
     * Assign cpr::SslOptions::ca_path values.
    */
    void SetCaPath(StackStrF & path) { ca_path.assign(path.mPtr, path.GetSize()); }

    /* --------------------------------------------------------------------------------------------
     * Retrieve cpr::SslOptions::crl_file value.
    */
    SQMOD_NODISCARD const std::string & GetCrlFile() const { return Base::crl_file; }

    /* --------------------------------------------------------------------------------------------
     * Assign cpr::SslOptions::crl_file values.
    */
    void SetCrlFile(StackStrF & file) { crl_file.assign(file.mPtr, file.GetSize()); }

    /* --------------------------------------------------------------------------------------------
     * Retrieve cpr::SslOptions::ciphers value.
    */
    SQMOD_NODISCARD const std::string & GetCiphers() const { return Base::ciphers; }

    /* --------------------------------------------------------------------------------------------
     * Assign cpr::SslOptions::ciphers values.
    */
    void SetCiphers(StackStrF & cph) { ciphers.assign(cph.mPtr, cph.GetSize()); }

#if SUPPORT_TLSv13_CIPHERS
    /* --------------------------------------------------------------------------------------------
     * Retrieve cpr::SslOptions::tls13_ciphers value.
    */
    SQMOD_NODISCARD const std::string & GetTLS13Ciphers() const { return Base::tls13_ciphers; }
    /* --------------------------------------------------------------------------------------------
     * Assign cpr::SslOptions::tls13_ciphers values.
    */
    void SetTLS13Ciphers(StackStrF & cph) { tls13_ciphers.assign(cph.mPtr, cph.GetSize()); }

#else
    void GetTLS13Ciphers() const { STHROWF("Unsupported"); } // NOLINT(readability-convert-member-functions-to-static)
    void SetTLS13Ciphers(StackStrF &) const { STHROWF("Unsupported"); } // NOLINT(readability-convert-member-functions-to-static)
#endif // SUPPORT_TLSv13_CIPHERS

#if SUPPORT_SESSIONID_CACHE
    /* --------------------------------------------------------------------------------------------
     * Retrieve cpr::SslOptions::session_id_cache value.
    */
    SQMOD_NODISCARD bool GetSessionIdCache() const { return Base::session_id_cache; }

    /* --------------------------------------------------------------------------------------------
     * Modify cpr::SslOptions::session_id_cache value.
    */
    void SetSessionIdCache(bool value) { Base::session_id_cache = value; }
#else
    void GetSessionIdCache() const { STHROWF("Unsupported"); } // NOLINT(readability-convert-member-functions-to-static)
    void SetSessionIdCache(bool) const { STHROWF("Unsupported"); } // NOLINT(readability-convert-member-functions-to-static)
#endif // SUPPORT_SESSIONID_CACHE

    /* --------------------------------------------------------------------------------------------
     * Modify cpr::SslOptions::ssl_version value.
    */
    void SetTLSv1() { ssl_version = CURL_SSLVERSION_TLSv1; }

#if SUPPORT_SSLv2
    /* --------------------------------------------------------------------------------------------
     * Modify cpr::SslOptions::ssl_version value.
    */
    void SetSSLv2() { ssl_version = CURL_SSLVERSION_SSLv2; }
#else
    void SetSSLv2() const { STHROWF("Unsupported"); } // NOLINT(readability-convert-member-functions-to-static)
#endif
    /* --------------------------------------------------------------------------------------------
     * Modify cpr::SslOptions::ssl_version value.
    */
#if SUPPORT_SSLv3
    void SetSSLv3() { ssl_version = CURL_SSLVERSION_SSLv3; }
#else
    void SetSSLv3() const { STHROWF("Unsupported"); } // NOLINT(readability-convert-member-functions-to-static)
#endif

    /* --------------------------------------------------------------------------------------------
     * Modify cpr::SslOptions::ssl_version value.
    */
#if SUPPORT_TLSv1_0
    void SetTLSv1_0() { ssl_version = CURL_SSLVERSION_TLSv1_0; }
#else
    void SetTLSv1_0() const { STHROWF("Unsupported"); } // NOLINT(readability-convert-member-functions-to-static)
#endif

    /* --------------------------------------------------------------------------------------------
     * Modify cpr::SslOptions::ssl_version value.
    */
#if SUPPORT_TLSv1_1
    void SetTLSv1_1() { ssl_version = CURL_SSLVERSION_TLSv1_1; }
#else
    void SetTLSv1_1() const { STHROWF("Unsupported"); } // NOLINT(readability-convert-member-functions-to-static)
#endif

    /* --------------------------------------------------------------------------------------------
     * Modify cpr::SslOptions::ssl_version value.
    */
#if SUPPORT_TLSv1_2
    void SetTLSv1_2() { ssl_version = CURL_SSLVERSION_TLSv1_2; }
#else
    void SetTLSv1_2() const { STHROWF("Unsupported"); } // NOLINT(readability-convert-member-functions-to-static)
#endif

    /* --------------------------------------------------------------------------------------------
     * Modify cpr::SslOptions::ssl_version value.
    */
#if SUPPORT_TLSv1_3
    void SetTLSv1_3() { ssl_version = CURL_SSLVERSION_TLSv1_3; }
#else
    void SetTLSv1_3() const { STHROWF("Unsupported"); } // NOLINT(readability-convert-member-functions-to-static)
#endif

    /* --------------------------------------------------------------------------------------------
     * Modify cpr::SslOptions::max_version value.
    */
#if SUPPORT_MAX_TLS_VERSION
    void SetMaxTLSVersion() { max_version = CURL_SSLVERSION_DEFAULT; }
#else
    void SetMaxTLSVersion() const { STHROWF("Unsupported"); } // NOLINT(readability-convert-member-functions-to-static)
#endif

    /* --------------------------------------------------------------------------------------------
     * Modify cpr::SslOptions::max_version value.
    */
#if SUPPORT_MAX_TLSv1_0
    void SetMaxTLSv1_0() { max_version = CURL_SSLVERSION_MAX_TLSv1_0; }
#else
    void SetMaxTLSv1_0() const { STHROWF("Unsupported"); } // NOLINT(readability-convert-member-functions-to-static)
#endif

    /* --------------------------------------------------------------------------------------------
     * Modify cpr::SslOptions::max_version value.
    */
#if SUPPORT_MAX_TLSv1_1
    void SetMaxTLSv1_1() { max_version = CURL_SSLVERSION_MAX_TLSv1_1; }
#else
    void SetMaxTLSv1_1() const { STHROWF("Unsupported"); } // NOLINT(readability-convert-member-functions-to-static)
#endif

    /* --------------------------------------------------------------------------------------------
     * Modify cpr::SslOptions::max_version value.
    */
#if SUPPORT_MAX_TLSv1_2
    void SetMaxTLSv1_2() { max_version = CURL_SSLVERSION_MAX_TLSv1_2; }
#else
    void SetMaxTLSv1_2() const { STHROWF("Unsupported"); } // NOLINT(readability-convert-member-functions-to-static)
#endif

    /* --------------------------------------------------------------------------------------------
     * Modify cpr::SslOptions::max_version value.
    */
#if SUPPORT_MAX_TLSv1_3
    void SetMaxTLSv1_3() { max_version = CURL_SSLVERSION_MAX_TLSv1_3; }
#else
    void SetMaxTLSv1_3() const { STHROWF("Unsupported"); } // NOLINT(readability-convert-member-functions-to-static)
#endif
};

/* ------------------------------------------------------------------------------------------------
 * Wrapper for cpr::Error that can be bound to the script engine.
*/
struct CpError : public cpr::Error
{
    using cpr::Error::Error;
    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    CpError() = default;

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    CpError(SQInteger code, StackStrF & msg)
        : cpr::Error(static_cast< int32_t >(code), msg.ToStr())
    {
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    explicit CpError(const cpr::Error & e) : cpr::Error(e) { }

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    explicit CpError(cpr::Error && e) : cpr::Error(std::move(e)) { }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    CpError(const CpError &) = default;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    CpError(CpError &&) noexcept = default;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~CpError() = default;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    CpError & operator = (const CpError &) = default;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    CpError & operator = (CpError &&) noexcept = default;

    /* --------------------------------------------------------------------------------------------
     * Retrieve cpr::Error::code.
    */
    SQMOD_NODISCARD SQInteger GetCode() const
    {
        return static_cast< SQInteger >(cpr::Error::code);
    }

    /* --------------------------------------------------------------------------------------------
     * Modify cpr::Error::code.
    */
    void SetCode(SQInteger value)
    {
        cpr::Error::code = static_cast< cpr::ErrorCode >(value);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve cpr::Error::text.
    */
    SQMOD_NODISCARD const std::string & GetMessage() const
    {
        return cpr::Error::message;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify cpr::Error::message.
    */
    void SetMessage(StackStrF & str)
    {
        cpr::Error::message.assign(str.mPtr, str.GetSize());
    }
};

/* ------------------------------------------------------------------------------------------------
 * Wrapper for cpr::Cookies that can be bound to the script engine.
*/
struct CpCookies : public cpr::Cookies
{
    using cpr::Cookies::Cookies;
    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    CpCookies() = default;

    /* --------------------------------------------------------------------------------------------
     * Encode constructor.
    */
    explicit CpCookies(bool enc) : cpr::Cookies(enc) { }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    explicit CpCookies(const cpr::Cookies & e) : cpr::Cookies(e) { }

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    explicit CpCookies(cpr::Cookies && e) : cpr::Cookies(std::move(e)) { }
    
    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    CpCookies(const CpCookies &) = default;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    CpCookies(CpCookies &&) noexcept = default;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~CpCookies() = default;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    CpCookies & operator = (const CpCookies &) = default;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    CpCookies & operator = (CpCookies &&) noexcept = default;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the number of values.
    */
    SQMOD_NODISCARD SQInteger Size() const
    {
        return static_cast< SQInteger >(cpr::Cookies::map_.size());
    }

    /* --------------------------------------------------------------------------------------------
     * Check if no value is stored.
    */
    SQMOD_NODISCARD bool Empty() const
    {
        return cpr::Cookies::map_.empty();
    }

    /* --------------------------------------------------------------------------------------------
     * Clear all values.
    */
    void Clear()
    {
        cpr::Cookies::map_.clear();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the number of matching values.
    */
    SQMOD_NODISCARD SQInteger Count(StackStrF & key) const
    {
        return static_cast< SQInteger >(cpr::Cookies::map_.count(key.ToStr()));
    }

    /* --------------------------------------------------------------------------------------------
     * Remove a value.
    */
    bool Erase(StackStrF & key)
    {
        auto itr = cpr::Cookies::map_.find(key.ToStr());
        // Does it exist?
        if (itr == cpr::Cookies::map_.end())
        {
            return false; // Nope
        }
        // Erase it
        cpr::Cookies::map_.erase(itr);
        // Erased
        return true;
    }

    /* --------------------------------------------------------------------------------------------
     * Check if value exists.
    */
    SQMOD_NODISCARD bool Has(StackStrF & key) const
    {
        return cpr::Cookies::map_.find(key.ToStr()) != cpr::Cookies::map_.end();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve value.
    */
    SQMOD_NODISCARD std::string & Get(StackStrF & key)
    {
        auto itr = cpr::Cookies::map_.find(key.ToStr());
        // Does it exist?
        if (itr == cpr::Cookies::map_.end())
        {
            STHROWF("No cookie named: %s", key.mPtr);
        }
        // Return it
        return itr->second;
    }

    /* --------------------------------------------------------------------------------------------
     * Assign value.
    */
    void Set(StackStrF & key, StackStrF & val)
    {
        cpr::Cookies::map_[key.ToStr()] = val.ToStr();
    }

    /* --------------------------------------------------------------------------------------------
     * Iterate all values.
    */
    void Each(Function & fn) const
    {
        for (const auto & p : cpr::Cookies::map_)
        {
            fn.Execute(p.first, p.second);
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Iterate all values until stopped.
    */
    void While(Function & fn) const
    {
        for (const auto & p : cpr::Cookies::map_)
        {
            auto ret = fn.Eval(p.first, p.second);
            // (null || true) == continue & false == break
            if (!ret.IsNull() || !ret.Cast< bool >())
            {
                break;
            }
        }
    }
};

/* ------------------------------------------------------------------------------------------------
 * Wrapper for cpr::Header that can be bound to the script engine.
*/
struct CpHeader
{
    /* --------------------------------------------------------------------------------------------
     * Value container.
    */
    cpr::Header mMap{};
    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    CpHeader() = default;

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    explicit CpHeader(const cpr::Header & e) : mMap(e) { } // NOLINT(modernize-pass-by-value)

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    explicit CpHeader(cpr::Header && e) : mMap(std::move(e)) { }
    
    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    CpHeader(const CpHeader &) = default;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    CpHeader(CpHeader &&) noexcept = default;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~CpHeader() = default;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    CpHeader & operator = (const CpHeader &) = default;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    CpHeader & operator = (CpHeader &&) noexcept = default;

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion operator.
    */
    operator cpr::Header & () { return mMap; } // NOLINT(google-explicit-constructor)

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion operator.
    */
    operator const cpr::Header & () const { return mMap; } // NOLINT(google-explicit-constructor)

    /* --------------------------------------------------------------------------------------------
     * Retrieve the number of values.
    */
    SQMOD_NODISCARD SQInteger Size() const
    {
        return static_cast< SQInteger >(mMap.size());
    }

    /* --------------------------------------------------------------------------------------------
     * Check if no value is stored.
    */
    SQMOD_NODISCARD bool Empty() const
    {
        return mMap.empty();
    }

    /* --------------------------------------------------------------------------------------------
     * Clear all values.
    */
    void Clear()
    {
        mMap.clear();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the number of matching values.
    */
    SQMOD_NODISCARD SQInteger Count(StackStrF & key) const
    {
        return static_cast< SQInteger >(mMap.count(key.ToStr()));
    }

    /* --------------------------------------------------------------------------------------------
     * Remove a value.
    */
    bool Erase(StackStrF & key)
    {
        auto itr = mMap.find(key.ToStr());
        // Does it exist?
        if (itr == mMap.end())
        {
            return false; // Nope
        }
        // Erase it
        mMap.erase(itr);
        // Erased
        return true;
    }

    /* --------------------------------------------------------------------------------------------
     * Check if value exists.
    */
    SQMOD_NODISCARD bool Has(StackStrF & key) const
    {
        return mMap.find(key.ToStr()) != mMap.end();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve value.
    */
    SQMOD_NODISCARD std::string & Get(StackStrF & key)
    {
        auto itr = mMap.find(key.ToStr());
        // Does it exist?
        if (itr == mMap.end())
        {
            STHROWF("No cookie named: %s", key.mPtr);
        }
        // Return it
        return itr->second;
    }

    /* --------------------------------------------------------------------------------------------
     * Assign value.
    */
    void Set(StackStrF & key, StackStrF & val)
    {
        mMap[key.ToStr()] = val.ToStr();
    }

    /* --------------------------------------------------------------------------------------------
     * Iterate all values.
    */
    void Each(Function & fn) const
    {
        for (const auto & p : mMap)
        {
            fn.Execute(p.first, p.second);
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Iterate all values until stopped.
    */
    void While(Function & fn) const
    {
        for (const auto & p : mMap)
        {
            auto ret = fn.Eval(p.first, p.second);
            // (null || true) == continue & false == break
            if (!ret.IsNull() || !ret.Cast< bool >())
            {
                break;
            }
        }
    }
};

/* ------------------------------------------------------------------------------------------------
 * Wrapper for cpr::Response that can be bound to the script engine.
*/
struct CpResponse : public cpr::Response
{
    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    CpResponse() = default;

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    explicit CpResponse(const cpr::Response & r) : cpr::Response(r) { }

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    explicit CpResponse(cpr::Response && r) : cpr::Response(std::move(r)) { }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    CpResponse(const CpResponse &) = default;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    CpResponse(CpResponse &&) noexcept = default;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~CpResponse() = default;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    CpResponse & operator = (const CpResponse &) = default;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    CpResponse & operator = (CpResponse &&) noexcept = default;

    /* --------------------------------------------------------------------------------------------
     * Retrieve certificate information.
    */
    SQMOD_NODISCARD Array GetCertInfoArray()
    {
        if (!cpr::Response::curl_)
        {
            STHROWF("Invalid response instance");
        }
        // Retrieve the info vector
        auto vec = cpr::Response::GetCertInfo();
        // Create a script array
        Array arr(SqVM(), static_cast< SQInteger >(vec.size()));
        // Populate the array with vector elements
        for (size_t i = 0; i < vec.size(); ++i)
        {
            arr.SetValue(static_cast< SQInteger >(i), vec[i]);
        }
        // Return the array
        return arr;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve cpr::Response::status_code.
    */
    SQMOD_NODISCARD SQInteger GetStatusCode() const
    {
        return cpr::Response::status_code;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify cpr::Response::status_code.
    */
    void SetStatusCode(SQInteger value)
    {
        cpr::Response::status_code = value;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve cpr::Response::text.
    */
    SQMOD_NODISCARD const std::string & GetText() const
    {
        return cpr::Response::text;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify cpr::Response::text.
    */
    void SetText(StackStrF & str)
    {
        cpr::Response::text.assign(str.mPtr, str.GetSize());
    }

    /* --------------------------------------------------------------------------------------------
     * Steal values from cpr::Response::header.
    */
    SQMOD_NODISCARD CpHeader StealHeader()
    {
        return CpHeader(std::move(cpr::Response::header));
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve cpr::Response::header.
    */
    SQMOD_NODISCARD CpHeader GetHeader() const
    {
        return CpHeader(cpr::Response::header);
    }

    /* --------------------------------------------------------------------------------------------
     * Modify cpr::Response::header.
    */
    void SetHeader(const CpHeader & value)
    {
        cpr::Response::header = value;
    }

    /* --------------------------------------------------------------------------------------------
     * Yield values to cpr::Response::header.
    */
    void YieldHeader(CpHeader & value)
    {
        cpr::Response::header = std::move(static_cast< cpr::Header & >(value));
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve cpr::Response::url.
    */
    SQMOD_NODISCARD const std::string & GetURL() const
    {
        return cpr::Response::url.str();
    }

    /* --------------------------------------------------------------------------------------------
     * Modify cpr::Response::url.
    */
    void SetURL(StackStrF & str)
    {
        cpr::Response::url = cpr::Url(str.mPtr, str.GetSize());
    }

    /* --------------------------------------------------------------------------------------------
     * Steal values from cpr::Response::cookies.
    */
    SQMOD_NODISCARD CpCookies StealCookies()
    {
        return CpCookies(std::move(cpr::Response::cookies));
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve cpr::Response::cookies.
    */
    SQMOD_NODISCARD CpCookies GetCookies() const
    {
        return CpCookies(cpr::Response::cookies);
    }

    /* --------------------------------------------------------------------------------------------
     * Modify cpr::Response::cookies.
    */
    void SetCookies(const CpCookies & value)
    {
        cpr::Response::cookies = value;
    }

    /* --------------------------------------------------------------------------------------------
     * Yield values to cpr::Response::cookies.
    */
    void YieldCookies(CpCookies & value)
    {
        cpr::Response::cookies = std::move(static_cast< cpr::Cookies & >(value));
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve cpr::Response::url.
    */
    SQMOD_NODISCARD CpError GetError() const
    {
        return CpError(cpr::Response::error);
    }

    /* --------------------------------------------------------------------------------------------
     * Modify cpr::Response::url.
    */
    void SetError(const CpError & e)
    {
        cpr::Response::error = e;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve cpr::Response::raw_header.
    */
    SQMOD_NODISCARD const std::string & GetRawHeader() const
    {
        return cpr::Response::raw_header;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify cpr::Response::raw_header.
    */
    void SetRawHeader(StackStrF & str)
    {
        cpr::Response::raw_header.assign(str.mPtr, str.GetSize());
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve cpr::Response::status_line.
    */
    SQMOD_NODISCARD const std::string & GetStatusLine() const
    {
        return cpr::Response::status_line;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify cpr::Response::status_line.
    */
    void SetStatusLine(StackStrF & str)
    {
        cpr::Response::status_line.assign(str.mPtr, str.GetSize());
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve cpr::Response::reason.
    */
    SQMOD_NODISCARD const std::string & GetReason() const
    {
        return cpr::Response::reason;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify cpr::Response::reason.
    */
    void SetReason(StackStrF & str)
    {
        cpr::Response::reason.assign(str.mPtr, str.GetSize());
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve cpr::Response::uploaded_bytes.
    */
    SQMOD_NODISCARD SQInteger GetUploadedBytes() const
    {
        return static_cast< SQInteger >(cpr::Response::uploaded_bytes);  // possible precision loss!
    }

    /* --------------------------------------------------------------------------------------------
     * Modify cpr::Response::uploaded_bytes.
    */
    void SetUploadedBytes(SQInteger value)
    {
        cpr::Response::uploaded_bytes = static_cast< cpr::cpr_off_t >(value);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve cpr::Response::downloaded_bytes.
    */
    SQMOD_NODISCARD SQInteger GetDownloadedBytes() const
    {
        return static_cast< SQInteger >(cpr::Response::downloaded_bytes);  // possible precision loss!
    }

    /* --------------------------------------------------------------------------------------------
     * Modify cpr::Response::downloaded_bytes.
    */
    void SetDownloadedBytes(SQInteger value)
    {
        cpr::Response::downloaded_bytes = static_cast< cpr::cpr_off_t >(value);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve cpr::Response::redirect_count.
    */
    SQMOD_NODISCARD SQInteger GetRedirectCount() const
    {
        return cpr::Response::redirect_count;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify cpr::Response::redirect_count.
    */
    void SetRedirectCount(SQInteger value)
    {
        cpr::Response::redirect_count = static_cast< long >(value);
    }
};

/* ------------------------------------------------------------------------------------------------
 * Wrapper for cpr::Parameters that can be bound to the script engine.
*/
struct CpParameters : public cpr::Parameters
{
    using cpr::Parameters::Parameters;
    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    CpParameters() = default;

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    explicit CpParameters(const cpr::Parameters & e) : cpr::Parameters(e) { }

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    explicit CpParameters(cpr::Parameters && e) : cpr::Parameters(std::move(e)) { }
    
    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    CpParameters(const CpParameters &) = default;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    CpParameters(CpParameters &&) noexcept = default;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~CpParameters() = default;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    CpParameters & operator = (const CpParameters &) = default;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    CpParameters & operator = (CpParameters &&) noexcept = default;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the number of values.
    */
    SQMOD_NODISCARD SQInteger Size() const
    {
        return static_cast< SQInteger >(cpr::Parameters::containerList_.size());
    }

    /* --------------------------------------------------------------------------------------------
     * Check if no value is stored.
    */
    SQMOD_NODISCARD bool Empty() const
    {
        return cpr::Parameters::containerList_.empty();
    }

    /* --------------------------------------------------------------------------------------------
     * Clear all values.
    */
    void Clear()
    {
        cpr::Parameters::containerList_.clear();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the number of matching values.
    */
    SQInteger Count(StackStrF & key) const
    {
        return static_cast< SQInteger >(std::count_if(
            cpr::Parameters::containerList_.begin(), cpr::Parameters::containerList_.end(),
            [key = key.ToStr()](const cpr::Parameter & p) { return p.key == key; }
        ));
    }

    /* --------------------------------------------------------------------------------------------
     * Remove a value.
    */
    SQInteger Erase(StackStrF & key)
    {
        const size_t n = cpr::Parameters::containerList_.size();
        cpr::Parameters::containerList_.erase(std::remove_if(
            cpr::Parameters::containerList_.begin(), cpr::Parameters::containerList_.end(),
            [key = key.ToStr()](cpr::Parameter & p) { return p.key == key; }), cpr::Parameters::containerList_.end()
        );
        return static_cast< SQInteger >(n - cpr::Parameters::containerList_.size());
    }

    /* --------------------------------------------------------------------------------------------
     * Check if value exists.
    */
    bool Has(StackStrF & key) const
    {
        return std::find_if(
            cpr::Parameters::containerList_.begin(), cpr::Parameters::containerList_.end(),
            [key = key.ToStr()](const cpr::Parameter & p) { return p.key == key; }
        ) != cpr::Parameters::containerList_.end();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve value.
    */
    std::string & Get(StackStrF & key)
    {
        auto itr = std::find_if(
            cpr::Parameters::containerList_.begin(), cpr::Parameters::containerList_.end(),
            [key = key.ToStr()](cpr::Parameter & p) { return p.key == key; }
        );
        // Does it exist?
        if (itr == cpr::Parameters::containerList_.end())
        {
            STHROWF("No parameter named: %s", key.mPtr);
        }
        // Return it
        return itr->value;
    }

    /* --------------------------------------------------------------------------------------------
     * Assign value.
    */
    void Set(StackStrF & key, StackStrF & val)
    {
        auto itr = std::find_if(
            cpr::Parameters::containerList_.begin(), cpr::Parameters::containerList_.end(),
            [key = key.ToStr()](cpr::Parameter & p) { return p.key == key; }
        );
        // Does it exist?
        if (itr == cpr::Parameters::containerList_.end())
        {
            cpr::Parameters::containerList_.emplace_back(key.ToStr(), val.ToStr());
        }
        else
        {
            itr->value.assign(val.mPtr, val.GetSize());
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Iterate all values.
    */
    void Each(Function & fn) const
    {
        for (const auto & p : cpr::Parameters::containerList_)
        {
            fn.Execute(p.key, p.value);
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Iterate all values until stopped.
    */
    void While(Function & fn) const
    {
        for (const auto & p : cpr::Parameters::containerList_)
        {
            auto ret = fn.Eval(p.key, p.value);
            // (null || true) == continue & false == break
            if (!ret.IsNull() || !ret.Cast< bool >())
            {
                break;
            }
        }
    }
};

/* ------------------------------------------------------------------------------------------------
 * Wrapper for cpr::Payload that can be bound to the script engine.
*/
struct CpPayload : public cpr::Payload
{
    using cpr::Payload::Payload;
    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    CpPayload() : cpr::Payload({}) { }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    explicit CpPayload(const cpr::Payload & e) : cpr::Payload(e) { }

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    explicit CpPayload(cpr::Payload && e) : cpr::Payload(std::move(e)) { }
    
    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    CpPayload(const CpPayload &) = default;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    CpPayload(CpPayload &&) noexcept = default;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~CpPayload() = default;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    CpPayload & operator = (const CpPayload &) = default;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    CpPayload & operator = (CpPayload &&) noexcept = default;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the number of values.
    */
    SQMOD_NODISCARD SQInteger Size() const
    {
        return static_cast< SQInteger >(cpr::Payload::containerList_.size());
    }

    /* --------------------------------------------------------------------------------------------
     * Check if no value is stored.
    */
    SQMOD_NODISCARD bool Empty() const
    {
        return cpr::Payload::containerList_.empty();
    }

    /* --------------------------------------------------------------------------------------------
     * Clear all values.
    */
    void Clear()
    {
        cpr::Payload::containerList_.clear();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the number of matching values.
    */
    SQMOD_NODISCARD SQInteger Count(StackStrF & key) const
    {
        return static_cast< SQInteger >(std::count_if(
            cpr::Payload::containerList_.begin(), cpr::Payload::containerList_.end(),
            [key = key.ToStr()](const cpr::Pair & p) { return p.key == key; }
        ));
    }

    /* --------------------------------------------------------------------------------------------
     * Remove a value.
    */
    SQInteger Erase(StackStrF & key)
    {
        const size_t n = cpr::Payload::containerList_.size();
        cpr::Payload::containerList_.erase(std::remove_if(
            cpr::Payload::containerList_.begin(), cpr::Payload::containerList_.end(),
            [key = key.ToStr()](cpr::Pair & p) { return p.key == key; }), cpr::Payload::containerList_.end()
        );
        return static_cast< SQInteger >(n - cpr::Payload::containerList_.size());
    }

    /* --------------------------------------------------------------------------------------------
     * Check if value exists.
    */
    SQMOD_NODISCARD bool Has(StackStrF & key) const
    {
        return std::find_if(
            cpr::Payload::containerList_.begin(), cpr::Payload::containerList_.end(),
            [key = key.ToStr()](const cpr::Pair & p) { return p.key == key; }
        ) != cpr::Payload::containerList_.end();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve value.
    */
    SQMOD_NODISCARD std::string & Get(StackStrF & key)
    {
        auto itr = std::find_if(
            cpr::Payload::containerList_.begin(), cpr::Payload::containerList_.end(),
            [key = key.ToStr()](cpr::Pair & p) { return p.key == key; }
        );
        // Does it exist?
        if (itr == cpr::Payload::containerList_.end())
        {
            STHROWF("No Pair named: %s", key.mPtr);
        }
        // Return it
        return itr->value;
    }

    /* --------------------------------------------------------------------------------------------
     * Assign value.
    */
    void Set(StackStrF & key, StackStrF & val)
    {
        auto itr = std::find_if(
            cpr::Payload::containerList_.begin(), cpr::Payload::containerList_.end(),
            [key = key.ToStr()](cpr::Pair & p) { return p.key == key; }
        );
        // Does it exist?
        if (itr == cpr::Payload::containerList_.end())
        {
            cpr::Payload::containerList_.emplace_back(key.ToStr(), val.ToStr());
        }
        else
        {
            itr->value.assign(val.mPtr, val.GetSize());
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Add value.
    */
    void Add(StackStrF & key, StackStrF & val)
    {
        cpr::Payload::containerList_.emplace_back(key.ToStr(), val.ToStr());
    }

    /* --------------------------------------------------------------------------------------------
     * Iterate all values.
    */
    void Each(Function & fn) const
    {
        for (const auto & p : cpr::Payload::containerList_)
        {
            fn.Execute(p.key, p.value);
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Iterate all values until stopped.
    */
    void While(Function & fn) const
    {
        for (const auto & p : cpr::Payload::containerList_)
        {
            auto ret = fn.Eval(p.key, p.value);
            // (null || true) == continue & false == break
            if (!ret.IsNull() || !ret.Cast< bool >())
            {
                break;
            }
        }
    }
};

/* ------------------------------------------------------------------------------------------------
 * Wrapper for cpr::Proxies that can be bound to the script engine.
*/
struct CpProxies : public cpr::Proxies
{
    using cpr::Proxies::Proxies;
    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    CpProxies() = default;

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    explicit CpProxies(const cpr::Proxies & e) : cpr::Proxies(e) { }

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    explicit CpProxies(cpr::Proxies && e) : cpr::Proxies(std::move(e)) { }
    
    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    CpProxies(const CpProxies &) = default;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    CpProxies(CpProxies &&) noexcept = default;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~CpProxies() = default;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    CpProxies & operator = (const CpProxies &) = default;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    CpProxies & operator = (CpProxies &&) noexcept = default;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the number of values.
    */
    SQMOD_NODISCARD SQInteger Size() const
    {
        return static_cast< SQInteger >(cpr::Proxies::hosts_.size());
    }

    /* --------------------------------------------------------------------------------------------
     * Check if no value is stored.
    */
    SQMOD_NODISCARD bool Empty() const
    {
        return cpr::Proxies::hosts_.empty();
    }

    /* --------------------------------------------------------------------------------------------
     * Clear all values.
    */
    void Clear()
    {
        cpr::Proxies::hosts_.clear();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the number of matching values.
    */
    SQMOD_NODISCARD SQInteger Count(StackStrF & key) const
    {
        return static_cast< SQInteger >(cpr::Proxies::hosts_.count(key.ToStr()));
    }

    /* --------------------------------------------------------------------------------------------
     * Remove a value.
    */
    bool Erase(StackStrF & key)
    {
        auto itr = cpr::Proxies::hosts_.find(key.ToStr());
        // Does it exist?
        if (itr == cpr::Proxies::hosts_.end())
        {
            return false; // Nope
        }
        // Erase it
        cpr::Proxies::hosts_.erase(itr);
        // Erased
        return true;
    }

    /* --------------------------------------------------------------------------------------------
     * Check if value exists.
    */
    SQMOD_NODISCARD bool Has(StackStrF & key) const
    {
        return cpr::Proxies::hosts_.find(key.ToStr()) != cpr::Proxies::hosts_.end();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve value.
    */
    SQMOD_NODISCARD std::string & Get(StackStrF & key)
    {
        auto itr = cpr::Proxies::hosts_.find(key.ToStr());
        // Does it exist?
        if (itr == cpr::Proxies::hosts_.end())
        {
            STHROWF("No cookie named: %s", key.mPtr);
        }
        // Return it
        return itr->second;
    }

    /* --------------------------------------------------------------------------------------------
     * Assign value.
    */
    void Set(StackStrF & key, StackStrF & val)
    {
        cpr::Proxies::hosts_[key.ToStr()] = val.ToStr();
    }

    /* --------------------------------------------------------------------------------------------
     * Iterate all values.
    */
    void Each(Function & fn) const
    {
        for (const auto & p : cpr::Proxies::hosts_)
        {
            fn.Execute(p.first, p.second);
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Iterate all values until stopped.
    */
    void While(Function & fn) const
    {
        for (const auto & p : cpr::Proxies::hosts_)
        {
            auto ret = fn.Eval(p.first, p.second);
            // (null || true) == continue & false == break
            if (!ret.IsNull() || !ret.Cast< bool >())
            {
                break;
            }
        }
    }
};

/* ------------------------------------------------------------------------------------------------
 * Wrapper for cpr::Session that can be bound to the script engine.
*/
struct CpSession : public cpr::Session
{
    // Pointer to the pending action associated with this session, if any.
    ThreadPoolItem * mPending{nullptr};

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    CpSession() = default;

    /* --------------------------------------------------------------------------------------------
     * URL constructor.
    */
    CpSession(StackStrF & url)
        : cpr::Session()
    {
        cpr::Session::SetUrl(cpr::Url(url.mPtr, url.GetSize()));
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor (disabled).
    */
    CpSession(const CpSession &) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    CpSession(CpSession &&) noexcept = default;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    virtual ~CpSession() = default;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator (disabled).
    */
    CpSession & operator = (const CpSession &) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    CpSession & operator = (CpSession &&) noexcept = default;

    /* --------------------------------------------------------------------------------------------
     * Throw exception if the session is locked.
    */
    void LockCheck()
    {
        if (mPending)
        {
            STHROWF("Session is currently locked by pending action.");
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Check if the session is locked.
    */
    SQMOD_NODISCARD bool IsLocked() const
    {
        return mPending != nullptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify URL option.
    */
    CpSession & SetURL_(StackStrF & url)
    {
        LockCheck();
        cpr::Session::SetUrl(cpr::Url(url.mPtr, url.GetSize()));
        return *this; // Allow chaining
    }

    /* --------------------------------------------------------------------------------------------
     * Modify parameters option.
    */
    CpSession & SetParameters_(const CpParameters & parameters)
    {
        LockCheck();
        cpr::Session::SetParameters(parameters);
        return *this; // Allow chaining
    }

    /* --------------------------------------------------------------------------------------------
     * Modify parameters option.
    */
    CpSession & YieldParameters(CpParameters & parameters)
    {
        LockCheck();
        cpr::Session::SetParameters(std::move(static_cast< cpr::Parameters & >(parameters)));
        return *this; // Allow chaining
    }

    /* --------------------------------------------------------------------------------------------
     * Modify header option.
    */
    CpSession & SetHeader_(const CpHeader & header)
    {
        LockCheck();
        cpr::Session::SetHeader(header.mMap);
        return *this; // Allow chaining
    }

    /* --------------------------------------------------------------------------------------------
     * Modify timeout option.
    */
    CpSession & SetTimeout_(SQInteger ms)
    {
        LockCheck();
        cpr::Session::SetTimeout(cpr::Timeout(std::chrono::milliseconds{ms}));
        return *this; // Allow chaining
    }

    /* --------------------------------------------------------------------------------------------
     * Modify connect timeout option.
    */
    CpSession & SetConnectTimeout_(SQInteger ms)
    {
        LockCheck();
        cpr::Session::SetConnectTimeout(cpr::ConnectTimeout(std::chrono::milliseconds{ms}));
        return *this; // Allow chaining
    }

    /* --------------------------------------------------------------------------------------------
     * Modify auth option.
    */
    CpSession & SetAuth_(StackStrF & username, StackStrF & password)
    {
        LockCheck();
        cpr::Session::SetAuth(cpr::Authentication(username.ToStr(), password.ToStr()));
        return *this; // Allow chaining
    }

    /* --------------------------------------------------------------------------------------------
     * Modify digest option.
    */
    CpSession & SetDigest_(StackStrF & username, StackStrF & password)
    {
        LockCheck();
        cpr::Session::SetAuth(cpr::Digest(username.ToStr(), password.ToStr()));
        return *this; // Allow chaining
    }

    /* --------------------------------------------------------------------------------------------
     * Modify user agent option.
    */
    CpSession & SetUserAgent_(StackStrF & agent)
    {
        LockCheck();
        cpr::Session::SetUserAgent(cpr::UserAgent(agent.mPtr, agent.GetSize()));
        return *this; // Allow chaining
    }

    /* --------------------------------------------------------------------------------------------
     * Modify payload option.
    */
    CpSession & SetPayload_(const CpPayload & payload)
    {
        LockCheck();
        cpr::Session::SetPayload(payload);
        return *this; // Allow chaining
    }

    /* --------------------------------------------------------------------------------------------
     * Modify payload option.
    */
    CpSession & YieldPayload(CpPayload & payload)
    {
        LockCheck();
        cpr::Session::SetPayload(std::move(static_cast< cpr::Payload & >(payload)));
        return *this; // Allow chaining
    }

    /* --------------------------------------------------------------------------------------------
     * Modify proxies option.
    */
    CpSession & SetProxies_(const CpProxies & proxies)
    {
        LockCheck();
        cpr::Session::SetProxies(proxies);
        return *this; // Allow chaining
    }

    /* --------------------------------------------------------------------------------------------
     * Modify proxies option.
    */
    CpSession & YieldProxies(CpProxies & proxies)
    {
        LockCheck();
        cpr::Session::SetProxies(std::move(static_cast< cpr::Proxies & >(proxies)));
        return *this; // Allow chaining
    }

    /* --------------------------------------------------------------------------------------------
     * Modify multipart option.
    */
    //CpSession & SetMultipart_(const cpr::Multipart & multipart)
    //{
    //}

    /* --------------------------------------------------------------------------------------------
     * Modify multipart option.
    */
    //CpSession & YieldMultipart(cpr::Multipart && multipart)
    //{
    //}

    /* --------------------------------------------------------------------------------------------
     * Modify NTLM option.
    */
    CpSession & SetNTLM_(StackStrF & username, StackStrF & password)
    {
        LockCheck();
        cpr::Session::SetNTLM(cpr::NTLM(username.ToStr(), password.ToStr()));
        return *this; // Allow chaining
    }

    /* --------------------------------------------------------------------------------------------
     * Modify redirect option.
    */
    CpSession & SetRedirect_(bool redirect)
    {
        LockCheck();
        cpr::Session::SetRedirect(redirect);
        return *this; // Allow chaining
    }

    /* --------------------------------------------------------------------------------------------
     * Modify max-redirects option.
    */
    CpSession & SetMaxRedirects_(SQInteger max_redirects)
    {
        LockCheck();
        cpr::Session::SetMaxRedirects(cpr::MaxRedirects(static_cast< int32_t >(max_redirects)));
        return *this; // Allow chaining
    }

    /* --------------------------------------------------------------------------------------------
     * Modify cookies option.
    */
    CpSession & SetCookies_(const CpCookies & cookies)
    {
        LockCheck();
        cpr::Session::SetCookies(cookies);
        return *this; // Allow chaining
    }

    /* --------------------------------------------------------------------------------------------
     * Modify body option.
    */
    CpSession & SetBody_(StackStrF & body)
    {
        LockCheck();
        cpr::Session::SetBody(cpr::Body(body.mPtr, body.GetSize()));
        return *this; // Allow chaining
    }

    /* --------------------------------------------------------------------------------------------
     * Modify low-speed option.
    */
    CpSession & SetLowSpeed_(SQInteger limit, SQInteger time)
    {
        LockCheck();
        cpr::Session::SetLowSpeed(cpr::LowSpeed(static_cast< int32_t >(limit), static_cast< int32_t >(time)));
        return *this; // Allow chaining
    }

    /* --------------------------------------------------------------------------------------------
     * Modify verify SSL option.
    */
    CpSession & SetVerifySsl_(bool verify)
    {
        LockCheck();
        cpr::Session::SetVerifySsl(cpr::VerifySsl(verify));
        return *this; // Allow chaining
    }

    /* --------------------------------------------------------------------------------------------
     * Modify unix socket option.
    */
    CpSession & SetUnixSocket_(StackStrF & socket)
    {
        LockCheck();
        cpr::Session::SetUnixSocket(cpr::UnixSocket(socket.ToStr()));
        return *this; // Allow chaining
    }

    /* --------------------------------------------------------------------------------------------
     * Modify SSL option.
    */
    CpSession & SetSslOptions_(const CpSslOptions & options)
    {
        LockCheck();
        cpr::Session::SetSslOptions(options);
        return *this; // Allow chaining
    }

    /* --------------------------------------------------------------------------------------------
     * Modify read option.
    */
    //CpSession & SetReadCallback_(const cpr::ReadCallback & read)
    //{
    //}

    /* --------------------------------------------------------------------------------------------
     * Modify header option.
    */
    //CpSession & SetHeaderCallback_(const cpr::HeaderCallback & header)
    //{
    //}

    /* --------------------------------------------------------------------------------------------
     * Modify write option.
    */
    //CpSession & SetWriteCallback_(const cpr::WriteCallback & write)
    //{
    //}

    /* --------------------------------------------------------------------------------------------
     * Modify progress option.
    */
    //CpSession & SetProgressCallback_(const cpr::ProgressCallback & progress)
    //{
    //}

    /* --------------------------------------------------------------------------------------------
     * Modify debug callback option.
    */
    //CpSession & SetDebugCallback_(const cpr::DebugCallback & debug)
    //{
    //}

    /* --------------------------------------------------------------------------------------------
     * Modify verbose option.
    */
    CpSession & SetVerbose_(bool verbose)
    {
        LockCheck();
        cpr::Session::SetVerbose(cpr::Verbose(verbose));
        return *this; // Allow chaining
    }

    /* --------------------------------------------------------------------------------------------
     * Delete request.
    */
    CpResponse DoDelete()
    {
        LockCheck();
        return CpResponse(cpr::Session::Delete());
    }

    /* --------------------------------------------------------------------------------------------
     * Get request.
    */
    CpResponse DoGet()
    {
        LockCheck();
        return CpResponse(cpr::Session::Get());
    }

    /* --------------------------------------------------------------------------------------------
     * Head request.
    */
    CpResponse DoHead()
    {
        LockCheck();
        return CpResponse(cpr::Session::Head());
    }

    /* --------------------------------------------------------------------------------------------
     * Options request.
    */
    CpResponse DoOptions()
    {
        LockCheck();
        return CpResponse(cpr::Session::Options());
    }

    /* --------------------------------------------------------------------------------------------
     * Patch request.
    */
    CpResponse DoPatch()
    {
        LockCheck();
        return CpResponse(cpr::Session::Patch());
    }

    /* --------------------------------------------------------------------------------------------
     * Post request.
    */
    CpResponse DoPost()
    {
        LockCheck();
        return CpResponse(cpr::Session::Post());
    }

    /* --------------------------------------------------------------------------------------------
     * Put request.
    */
    CpResponse DoPut()
    {
        LockCheck();
        return CpResponse(cpr::Session::Put());
    }

    //CpResponse Download(const WriteCallback& write);
    //CpResponse Download(std::ofstream& file);

    /* --------------------------------------------------------------------------------------------
     * Delete async request.
    */
    void DoDelete_(Function & cb);

    /* --------------------------------------------------------------------------------------------
     * Get async request.
    */
    void DoGet_(Function & cb);

    /* --------------------------------------------------------------------------------------------
     * Head async request.
    */
    void DoHead_(Function & cb);

    /* --------------------------------------------------------------------------------------------
     * Options async request.
    */
    void DoOptions_(Function & cb);

    /* --------------------------------------------------------------------------------------------
     * Patch async request.
    */
    void DoPatch_(Function & cb);

    /* --------------------------------------------------------------------------------------------
     * Post async request.
    */
    void DoPost_(Function & cb);

    /* --------------------------------------------------------------------------------------------
     * Put async request.
    */
    void DoPut_(Function & cb);
};

} // Namespace:: SqMod
