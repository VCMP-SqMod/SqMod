// ------------------------------------------------------------------------------------------------
#include "Library/CURL.hpp"

// ------------------------------------------------------------------------------------------------
#include <sqratConst.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQMOD_DECL_TYPENAME(SqCpSslOptions, _SC("SqCprSslOptions"))
SQMOD_DECL_TYPENAME(SqCpError, _SC("SqCprError"))
SQMOD_DECL_TYPENAME(SqCpCookies, _SC("SqCprCookies"))
SQMOD_DECL_TYPENAME(SqCpHeader, _SC("SqCprHeader"))
SQMOD_DECL_TYPENAME(SqCpResponse, _SC("SqCprResponse"))
SQMOD_DECL_TYPENAME(SqCpParameters, _SC("SqCprParameters"))
SQMOD_DECL_TYPENAME(SqCpPayload, _SC("SqCprPayload"))
SQMOD_DECL_TYPENAME(SqCpProxies, _SC("SqCprProxies"))
SQMOD_DECL_TYPENAME(SqCpSession, _SC("SqCprSession"))

// ------------------------------------------------------------------------------------------------
static const EnumElement g_ErrorCodes[] = {
    {_SC("OK"),                             SQInteger(cpr::ErrorCode::OK)},
    {_SC("CONNECTION_FAILURE"),             SQInteger(cpr::ErrorCode::CONNECTION_FAILURE)},
    {_SC("EMPTY_RESPONSE"),                 SQInteger(cpr::ErrorCode::EMPTY_RESPONSE)},
    {_SC("HOST_RESOLUTION_FAILURE"),        SQInteger(cpr::ErrorCode::HOST_RESOLUTION_FAILURE)},
    {_SC("INTERNAL_ERROR"),                 SQInteger(cpr::ErrorCode::INTERNAL_ERROR)},
    {_SC("INVALID_URL_FORMAT"),             SQInteger(cpr::ErrorCode::INVALID_URL_FORMAT)},
    {_SC("NETWORK_RECEIVE_ERROR"),          SQInteger(cpr::ErrorCode::NETWORK_RECEIVE_ERROR)},
    {_SC("NETWORK_SEND_FAILURE"),           SQInteger(cpr::ErrorCode::NETWORK_SEND_FAILURE)},
    {_SC("OPERATION_TIMEDOUT"),             SQInteger(cpr::ErrorCode::OPERATION_TIMEDOUT)},
    {_SC("PROXY_RESOLUTION_FAILURE"),       SQInteger(cpr::ErrorCode::PROXY_RESOLUTION_FAILURE)},
    {_SC("SSL_CONNECT_ERROR"),              SQInteger(cpr::ErrorCode::SSL_CONNECT_ERROR)},
    {_SC("SSL_LOCAL_CERTIFICATE_ERROR"),    SQInteger(cpr::ErrorCode::SSL_LOCAL_CERTIFICATE_ERROR)},
    {_SC("SSL_REMOTE_CERTIFICATE_ERROR"),   SQInteger(cpr::ErrorCode::SSL_REMOTE_CERTIFICATE_ERROR)},
    {_SC("SSL_CACERT_ERROR"),               SQInteger(cpr::ErrorCode::SSL_CACERT_ERROR)},
    {_SC("GENERIC_SSL_ERROR"),              SQInteger(cpr::ErrorCode::GENERIC_SSL_ERROR)},
    {_SC("UNSUPPORTED_PROTOCOL"),           SQInteger(cpr::ErrorCode::UNSUPPORTED_PROTOCOL)},
    {_SC("REQUEST_CANCELLED"),              SQInteger(cpr::ErrorCode::REQUEST_CANCELLED)},
    {_SC("UNKNOWN_ERROR"),                  SQInteger(cpr::ErrorCode::UNKNOWN_ERROR)}
};

// ------------------------------------------------------------------------------------------------
static const EnumElement g_StatusCodes[] = {
    // Information responses
    {_SC("HTTP_CONTINUE"),                          cpr::status::HTTP_CONTINUE},
    {_SC("HTTP_SWITCHING_PROTOCOL"),                cpr::status::HTTP_SWITCHING_PROTOCOL},
    {_SC("HTTP_PROCESSING"),                        cpr::status::HTTP_PROCESSING},
    {_SC("HTTP_EARLY_HINTS"),                       cpr::status::HTTP_EARLY_HINTS},
    // Successful responses
    {_SC("HTTP_OK"),                                cpr::status::HTTP_OK},
    {_SC("HTTP_CREATED"),                           cpr::status::HTTP_CREATED},
    {_SC("HTTP_ACCEPTED"),                          cpr::status::HTTP_ACCEPTED},
    {_SC("HTTP_NON_AUTHORITATIVE_INFORMATION"),     cpr::status::HTTP_NON_AUTHORITATIVE_INFORMATION},
    {_SC("HTTP_NO_CONTENT"),                        cpr::status::HTTP_NO_CONTENT},
    {_SC("HTTP_RESET_CONTENT"),                     cpr::status::HTTP_RESET_CONTENT},
    {_SC("HTTP_PARTIAL_CONTENT"),                   cpr::status::HTTP_PARTIAL_CONTENT},
    {_SC("HTTP_MULTI_STATUS"),                      cpr::status::HTTP_MULTI_STATUS},
    {_SC("HTTP_ALREADY_REPORTED"),                  cpr::status::HTTP_ALREADY_REPORTED},
    {_SC("HTTP_IM_USED"),                           cpr::status::HTTP_IM_USED},
    // Redirection messages
    {_SC("HTTP_MULTIPLE_CHOICE"),                   cpr::status::HTTP_MULTIPLE_CHOICE},
    {_SC("HTTP_MOVED_PERMANENTLY"),                 cpr::status::HTTP_MOVED_PERMANENTLY},
    {_SC("HTTP_FOUND"),                             cpr::status::HTTP_FOUND},
    {_SC("HTTP_SEE_OTHER"),                         cpr::status::HTTP_SEE_OTHER},
    {_SC("HTTP_NOT_MODIFIED"),                      cpr::status::HTTP_NOT_MODIFIED},
    {_SC("HTTP_USE_PROXY"),                         cpr::status::HTTP_USE_PROXY},
    {_SC("HTTP_UNUSED"),                            cpr::status::HTTP_UNUSED},
    {_SC("HTTP_TEMPORARY_REDIRECT"),                cpr::status::HTTP_TEMPORARY_REDIRECT},
    {_SC("HTTP_PERMANENT_REDIRECT"),                cpr::status::HTTP_PERMANENT_REDIRECT},
    // Client error responses
    {_SC("HTTP_BAD_REQUEST"),                       cpr::status::HTTP_BAD_REQUEST},
    {_SC("HTTP_UNAUTHORIZED"),                      cpr::status::HTTP_UNAUTHORIZED},
    {_SC("HTTP_PAYMENT_REQUIRED"),                  cpr::status::HTTP_PAYMENT_REQUIRED},
    {_SC("HTTP_FORBIDDEN"),                         cpr::status::HTTP_FORBIDDEN},
    {_SC("HTTP_NOT_FOUND"),                         cpr::status::HTTP_NOT_FOUND},
    {_SC("HTTP_METHOD_NOT_ALLOWED"),                cpr::status::HTTP_METHOD_NOT_ALLOWED},
    {_SC("HTTP_NOT_ACCEPTABLE"),                    cpr::status::HTTP_NOT_ACCEPTABLE},
    {_SC("HTTP_PROXY_AUTHENTICATION_REQUIRED"),     cpr::status::HTTP_PROXY_AUTHENTICATION_REQUIRED},
    {_SC("HTTP_REQUEST_TIMEOUT"),                   cpr::status::HTTP_REQUEST_TIMEOUT},
    {_SC("HTTP_CONFLICT"),                          cpr::status::HTTP_CONFLICT},
    {_SC("HTTP_GONE"),                              cpr::status::HTTP_GONE},
    {_SC("HTTP_LENGTH_REQUIRED"),                   cpr::status::HTTP_LENGTH_REQUIRED},
    {_SC("HTTP_PRECONDITION_FAILED"),               cpr::status::HTTP_PRECONDITION_FAILED},
    {_SC("HTTP_PAYLOAD_TOO_LARGE"),                 cpr::status::HTTP_PAYLOAD_TOO_LARGE},
    {_SC("HTTP_URI_TOO_LONG"),                      cpr::status::HTTP_URI_TOO_LONG},
    {_SC("HTTP_UNSUPPORTED_MEDIA_TYPE"),            cpr::status::HTTP_UNSUPPORTED_MEDIA_TYPE},
    {_SC("HTTP_REQUESTED_RANGE_NOT_SATISFIABLE"),   cpr::status::HTTP_REQUESTED_RANGE_NOT_SATISFIABLE},
    {_SC("HTTP_EXPECTATION_FAILED"),                cpr::status::HTTP_EXPECTATION_FAILED},
    {_SC("HTTP_IM_A_TEAPOT"),                       cpr::status::HTTP_IM_A_TEAPOT},
    {_SC("HTTP_MISDIRECTED_REQUEST"),               cpr::status::HTTP_MISDIRECTED_REQUEST},
    {_SC("HTTP_UNPROCESSABLE_ENTITY"),              cpr::status::HTTP_UNPROCESSABLE_ENTITY},
    {_SC("HTTP_LOCKED"),                            cpr::status::HTTP_LOCKED},
    {_SC("HTTP_FAILED_DEPENDENCY"),                 cpr::status::HTTP_FAILED_DEPENDENCY},
    {_SC("HTTP_TOO_EARLY"),                         cpr::status::HTTP_TOO_EARLY},
    {_SC("HTTP_UPGRADE_REQUIRED"),                  cpr::status::HTTP_UPGRADE_REQUIRED},
    {_SC("HTTP_PRECONDITION_REQUIRED"),             cpr::status::HTTP_PRECONDITION_REQUIRED},
    {_SC("HTTP_TOO_MANY_REQUESTS"),                 cpr::status::HTTP_TOO_MANY_REQUESTS},
    {_SC("HTTP_REQUEST_HEADER_FIELDS_TOO_LARGE"),   cpr::status::HTTP_REQUEST_HEADER_FIELDS_TOO_LARGE},
    {_SC("HTTP_UNAVAILABLE_FOR_LEGAL_REASONS"),     cpr::status::HTTP_UNAVAILABLE_FOR_LEGAL_REASONS},
    // Server response errors
    {_SC("HTTP_INTERNAL_SERVER_ERROR"),             cpr::status::HTTP_INTERNAL_SERVER_ERROR},
    {_SC("HTTP_NOT_IMPLEMENTED"),                   cpr::status::HTTP_NOT_IMPLEMENTED},
    {_SC("HTTP_BAD_GATEWAY"),                       cpr::status::HTTP_BAD_GATEWAY},
    {_SC("HTTP_SERVICE_UNAVAILABLE"),               cpr::status::HTTP_SERVICE_UNAVAILABLE},
    {_SC("HTTP_GATEWAY_TIMEOUT"),                   cpr::status::HTTP_GATEWAY_TIMEOUT},
    {_SC("HTTP_HTTP_VERSION_NOT_SUPPORTED"),        cpr::status::HTTP_HTTP_VERSION_NOT_SUPPORTED},
    {_SC("HTTP_VARIANT_ALSO_NEGOTIATES"),           cpr::status::HTTP_VARIANT_ALSO_NEGOTIATES},
    {_SC("HTTP_INSUFFICIENT_STORAGE"),              cpr::status::HTTP_INSUFFICIENT_STORAGE},
    {_SC("HTTP_LOOP_DETECTED"),                     cpr::status::HTTP_LOOP_DETECTED},
    {_SC("HTTP_NOT_EXTENDED"),                      cpr::status::HTTP_NOT_EXTENDED},
    {_SC("HTTP_NETWORK_AUTHENTICATION_REQUIRED"),   cpr::status::HTTP_NETWORK_AUTHENTICATION_REQUIRED},

    {_SC("INFO_CODE_OFFSET"),                       cpr::status::INFO_CODE_OFFSET},
    {_SC("SUCCESS_CODE_OFFSET"),                    cpr::status::SUCCESS_CODE_OFFSET},
    {_SC("REDIRECT_CODE_OFFSET"),                   cpr::status::REDIRECT_CODE_OFFSET},
    {_SC("CLIENT_ERROR_CODE_OFFSET"),               cpr::status::CLIENT_ERROR_CODE_OFFSET},
    {_SC("SERVER_ERROR_CODE_OFFSET"),               cpr::status::SERVER_ERROR_CODE_OFFSET},
    {_SC("MISC_CODE_OFFSET"),                       cpr::status::MISC_CODE_OFFSET}
};

// ------------------------------------------------------------------------------------------------
static const EnumElements g_EnumList[] = {
    {_SC("SqCprErrorCode"), g_ErrorCodes},
    {_SC("SqCprStatusCode"), g_StatusCodes}
};

// ================================================================================================
void Register_CURL(HSQUIRRELVM vm)
{
    Table cpns(vm);

    // --------------------------------------------------------------------------------------------
    cpns.Bind(_SC("SslOptions"),
        Class< CpSslOptions >(vm, SqCpSslOptions::Str)
        // Constructors
        .Ctor()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqCpSslOptions::Fn)
        // Properties
        .Prop(_SC("CertFile"), &CpSslOptions::GetCertFile)
        .Prop(_SC("CertType"), &CpSslOptions::GetCertType, &CpSslOptions::SetCertType)
        .Prop(_SC("KeyFile"), &CpSslOptions::GetKeyFile)
        .Prop(_SC("KeyType"), &CpSslOptions::GetKeyType, &CpSslOptions::SetKeyType)
        .Prop(_SC("KeyPass"), &CpSslOptions::GetKeyPass, &CpSslOptions::SetKeyPass)
        .Prop(_SC("ALPN"), &CpSslOptions::GetALPN, &CpSslOptions::SetALPN)
        .Prop(_SC("NPM"), &CpSslOptions::GetNPM, &CpSslOptions::SetNPM)
        .Prop(_SC("VerifyHost"), &CpSslOptions::GetVerifyHost, &CpSslOptions::SetVerifyHost)
        .Prop(_SC("VerifyPeer"), &CpSslOptions::GetVerifyPeer, &CpSslOptions::SetVerifyPeer)
        .Prop(_SC("VerifyStatus"), &CpSslOptions::GetVerifyStatus, &CpSslOptions::SetVerifyStatus)
        .Prop(_SC("SslVersion"), &CpSslOptions::GetSslVersion)
        .Prop(_SC("MaxVersion"), &CpSslOptions::GetMaxVersion)
        .Prop(_SC("CaInfo"), &CpSslOptions::GetCaInfo, &CpSslOptions::SetCaInfo)
        .Prop(_SC("CaPath"), &CpSslOptions::GetCaPath, &CpSslOptions::SetCaPath)
        .Prop(_SC("CrlFile"), &CpSslOptions::GetCrlFile, &CpSslOptions::SetCrlFile)
        .Prop(_SC("Ciphers"), &CpSslOptions::GetCiphers, &CpSslOptions::SetCiphers)
        .Prop(_SC("TLS13Ciphers"), &CpSslOptions::GetTLS13Ciphers, &CpSslOptions::SetTLS13Ciphers)
        .Prop(_SC("SessionIdCache"), &CpSslOptions::GetSessionIdCache, &CpSslOptions::SetSessionIdCache)
        // Member Methods
        .Func(_SC("SetCertFile"), &CpSslOptions::SetCertFile)
        .Func(_SC("SetPemCertFile"), &CpSslOptions::SetPemCertFile)
        .Func(_SC("SetDerCertFile"), &CpSslOptions::SetDerCertFile)
        .Func(_SC("SetKeyFile"), &CpSslOptions::SetKeyFile)
        .Func(_SC("SetPemKeyFile"), &CpSslOptions::SetPemKeyFile)
        .Func(_SC("SetDerKeyFile"), &CpSslOptions::SetDerKeyFile)
        .Func(_SC("SetTLSv1"), &CpSslOptions::SetTLSv1)
        .Func(_SC("SetSSLv2"), &CpSslOptions::SetSSLv2)
        .Func(_SC("SetSSLv3"), &CpSslOptions::SetSSLv3)
        .Func(_SC("SetTLSv1_0"), &CpSslOptions::SetTLSv1_0)
        .Func(_SC("SetTLSv1_1"), &CpSslOptions::SetTLSv1_1)
        .Func(_SC("SetTLSv1_2"), &CpSslOptions::SetTLSv1_2)
        .Func(_SC("SetTLSv1_3"), &CpSslOptions::SetTLSv1_3)
        .Func(_SC("SetMaxTLSVersion"), &CpSslOptions::SetMaxTLSVersion)
        .Func(_SC("SetMaxTLSv1_0"), &CpSslOptions::SetMaxTLSv1_0)
        .Func(_SC("SetMaxTLSv1_1"), &CpSslOptions::SetMaxTLSv1_1)
        .Func(_SC("SetMaxTLSv1_2"), &CpSslOptions::SetMaxTLSv1_2)
        .Func(_SC("SetMaxTLSv1_3"), &CpSslOptions::SetMaxTLSv1_3)
    );

    // --------------------------------------------------------------------------------------------
    cpns.Bind(_SC("Error"),
        Class< CpError >(vm, SqCpError::Str)
        // Constructors
        .Ctor()
        .Ctor< SQInteger, StackStrF & >()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqCpError::Fn)
        // Properties
        .Prop(_SC("Code"), &CpError::GetCode, &CpError::SetCode)
        .Prop(_SC("Message"), &CpError::GetMessage, &CpError::SetMessage)
    );

    // --------------------------------------------------------------------------------------------
    cpns.Bind(_SC("Cookies"),
        Class< CpCookies >(vm, SqCpCookies::Str)
        // Constructors
        .Ctor()
        .Ctor< bool >()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqCpCookies::Fn)
        // Properties
        .Prop(_SC("Size"), &CpCookies::Size)
        .Prop(_SC("Empty"), &CpCookies::Empty)
        // Member Methods
        .FmtFunc(_SC("Count"), &CpCookies::Count)
        .FmtFunc(_SC("Erase"), &CpCookies::Erase)
        .FmtFunc(_SC("Has"), &CpCookies::Has)
        .FmtFunc(_SC("Get"), &CpCookies::Get)
        .FmtFunc(_SC("Set"), &CpCookies::Set)
        .CbFunc(_SC("Each"), &CpCookies::Each)
        .CbFunc(_SC("While"), &CpCookies::While)
    );

    // --------------------------------------------------------------------------------------------
    cpns.Bind(_SC("Header"),
        Class< CpHeader >(vm, SqCpHeader::Str)
        // Constructors
        .Ctor()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqCpHeader::Fn)
        // Properties
        .Prop(_SC("Size"), &CpHeader::Size)
        .Prop(_SC("Empty"), &CpHeader::Empty)
        // Member Methods
        .FmtFunc(_SC("Count"), &CpHeader::Count)
        .FmtFunc(_SC("Erase"), &CpHeader::Erase)
        .FmtFunc(_SC("Has"), &CpHeader::Has)
        .FmtFunc(_SC("Get"), &CpHeader::Get)
        .FmtFunc(_SC("Set"), &CpHeader::Set)
        .CbFunc(_SC("Each"), &CpHeader::Each)
        .CbFunc(_SC("While"), &CpHeader::While)
    );

    // --------------------------------------------------------------------------------------------
    cpns.Bind(_SC("Response"),
        Class< CpResponse >(vm, SqCpResponse::Str)
        // Constructors
        .Ctor()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqCpResponse::Fn)
        // Properties
        .Prop(_SC("CertInfo"), &CpResponse::GetCertInfoArray)
        .Prop(_SC("StatusCode"), &CpResponse::GetStatusCode, &CpResponse::SetStatusCode)
        .Prop(_SC("Text"), &CpResponse::GetText, &CpResponse::SetText)
        .Prop(_SC("Header"), &CpResponse::GetHeader, &CpResponse::SetHeader)
        .Prop(_SC("URL"), &CpResponse::GetURL, &CpResponse::SetURL)
        .Prop(_SC("Cookies"), &CpResponse::GetCookies, &CpResponse::SetCookies)
        .Prop(_SC("Error"), &CpResponse::GetError, &CpResponse::SetError)
        .Prop(_SC("RawHeader"), &CpResponse::GetRawHeader, &CpResponse::SetRawHeader)
        .Prop(_SC("StatusLine"), &CpResponse::GetStatusLine, &CpResponse::SetStatusLine)
        .Prop(_SC("Reason"), &CpResponse::GetReason, &CpResponse::SetReason)
        .Prop(_SC("UploadedBytes"), &CpResponse::GetUploadedBytes, &CpResponse::SetUploadedBytes)
        .Prop(_SC("DownloadedBytes"), &CpResponse::GetDownloadedBytes, &CpResponse::SetDownloadedBytes)
        .Prop(_SC("RedirectCount"), &CpResponse::GetRedirectCount, &CpResponse::SetRedirectCount)
        // Member Methods
        .FmtFunc(_SC("StealHeader"), &CpResponse::StealHeader)
        .FmtFunc(_SC("YieldHeader"), &CpResponse::YieldHeader)
        .FmtFunc(_SC("StealCookies"), &CpResponse::StealCookies)
        .FmtFunc(_SC("YieldCookies"), &CpResponse::YieldCookies)
    );

    // --------------------------------------------------------------------------------------------
    cpns.Bind(_SC("Parameters"),
        Class< CpParameters >(vm, SqCpParameters::Str)
        // Constructors
        .Ctor()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqCpParameters::Fn)
        // Properties
        .Prop(_SC("Size"), &CpParameters::Size)
        .Prop(_SC("Empty"), &CpParameters::Empty)
        // Member Methods
        .FmtFunc(_SC("Count"), &CpParameters::Count)
        .FmtFunc(_SC("Erase"), &CpParameters::Erase)
        .FmtFunc(_SC("Has"), &CpParameters::Has)
        .FmtFunc(_SC("Get"), &CpParameters::Get)
        .FmtFunc(_SC("Set"), &CpParameters::Set)
        .CbFunc(_SC("Each"), &CpParameters::Each)
        .CbFunc(_SC("While"), &CpParameters::While)
    );

    // --------------------------------------------------------------------------------------------
    cpns.Bind(_SC("Payload"),
        Class< CpPayload >(vm, SqCpPayload::Str)
        // Constructors
        .Ctor()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqCpPayload::Fn)
        // Properties
        .Prop(_SC("Size"), &CpPayload::Size)
        .Prop(_SC("Empty"), &CpPayload::Empty)
        // Member Methods
        .FmtFunc(_SC("Count"), &CpPayload::Count)
        .FmtFunc(_SC("Erase"), &CpPayload::Erase)
        .FmtFunc(_SC("Has"), &CpPayload::Has)
        .FmtFunc(_SC("Get"), &CpPayload::Get)
        .FmtFunc(_SC("Set"), &CpPayload::Set)
        .FmtFunc(_SC("Add"), &CpPayload::Add)
        .CbFunc(_SC("Each"), &CpPayload::Each)
        .CbFunc(_SC("While"), &CpPayload::While)
    );

    // --------------------------------------------------------------------------------------------
    cpns.Bind(_SC("Proxies"),
        Class< CpProxies >(vm, SqCpProxies::Str)
        // Constructors
        .Ctor()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqCpProxies::Fn)
        // Properties
        .Prop(_SC("Size"), &CpProxies::Size)
        .Prop(_SC("Empty"), &CpProxies::Empty)
        // Member Methods
        .FmtFunc(_SC("Count"), &CpProxies::Count)
        .FmtFunc(_SC("Erase"), &CpProxies::Erase)
        .FmtFunc(_SC("Has"), &CpProxies::Has)
        .FmtFunc(_SC("Get"), &CpProxies::Get)
        .FmtFunc(_SC("Set"), &CpProxies::Set)
        .CbFunc(_SC("Each"), &CpProxies::Each)
        .CbFunc(_SC("While"), &CpProxies::While)
    );

    // --------------------------------------------------------------------------------------------
    cpns.Bind(_SC("Session"),
        Class< CpSession, NoCopy< CpSession > >(vm, SqCpSession::Str)
        // Constructors
        .Ctor()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqCpSession::Fn)
        // Member Methods
        .FmtFunc(_SC("SetURL"), &CpSession::SetURL_)
        .Func(_SC("SetParameters"), &CpSession::SetParameters_)
        .Func(_SC("YieldParameters"), &CpSession::YieldParameters)
        .Func(_SC("SetHeader"), &CpSession::SetHeader_)
        .Func(_SC("SetTimeout"), &CpSession::SetTimeout_)
        .Func(_SC("SetConnectTimeout"), &CpSession::SetConnectTimeout_)
        .FmtFunc(_SC("SetAuth"), &CpSession::SetAuth_)
        .FmtFunc(_SC("SetDigest"), &CpSession::SetDigest_)
        .FmtFunc(_SC("SetUserAgent"), &CpSession::SetUserAgent_)
        .Func(_SC("SetPayload"), &CpSession::SetPayload_)
        .Func(_SC("YieldPayload"), &CpSession::YieldPayload)
        .Func(_SC("SetProxies"), &CpSession::SetProxies_)
        .Func(_SC("YieldProxies"), &CpSession::YieldProxies)
        .FmtFunc(_SC("SetNTLM"), &CpSession::SetNTLM_)
        .Func(_SC("SetRedirect"), &CpSession::SetRedirect_)
        .Func(_SC("SetMaxRedirects"), &CpSession::SetMaxRedirects_)
        .Func(_SC("SetCookies"), &CpSession::SetCookies_)
        .FmtFunc(_SC("SetBody"), &CpSession::SetBody_)
        .Func(_SC("SetLowSpeed"), &CpSession::SetLowSpeed_)
        .Func(_SC("SetVerifySsl"), &CpSession::SetVerifySsl_)
        .Func(_SC("SetUnixSocket"), &CpSession::SetUnixSocket_)
        .Func(_SC("SetSslOptions"), &CpSession::SetSslOptions_)
        .FmtFunc(_SC("SetVerbose"), &CpSession::SetVerbose_)
        .Func(_SC("Delete"), &CpSession::DoDelete)
        .Func(_SC("Get"), &CpSession::DoGet)
        .Func(_SC("Head"), &CpSession::DoHead)
        .Func(_SC("Options"), &CpSession::DoOptions)
        .Func(_SC("Patch"), &CpSession::DoPatch)
        .Func(_SC("Post"), &CpSession::DoPost)
        .Func(_SC("Put"), &CpSession::DoPut)
    );

    RootTable(vm).Bind(_SC("SqCPR"), cpns);

    // --------------------------------------------------------------------------------------------
    RegisterEnumerations(vm, g_EnumList);

    RootTable(vm)
    .Func(_SC("IsInformationalStatus"), cpr::status::is_informational)
    .Func(_SC("IsSuccessStatus"), cpr::status::is_success)
    .Func(_SC("IsRedirectStatus"), cpr::status::is_redirect)
    .Func(_SC("IsClientErrorStatus"), cpr::status::is_client_error)
    .Func(_SC("IsServerErrorStatus"), cpr::status::is_server_error);
}

} // Namespace:: SqMod
