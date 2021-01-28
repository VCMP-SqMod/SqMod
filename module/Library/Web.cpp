// ------------------------------------------------------------------------------------------------
#include "Library/Web.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
struct CvInit {
#if defined(NO_SSL)
	CvInit() { mg_init_library(MG_FEATURES_FILES|MG_FEATURES_IPV6|MG_FEATURES_WEBSOCKET|MG_FEATURES_CACHE|MG_FEATURES_HTTP2); }
#else
	CvInit() { mg_init_library(MG_FEATURES_FILES|MG_FEATURES_SSL|MG_FEATURES_IPV6|MG_FEATURES_WEBSOCKET|MG_FEATURES_CACHE|MG_FEATURES_HTTP2); }
#endif
} g_CvInit;

// ================================================================================================
void Register_Web(HSQUIRRELVM vm)
{
    Table wbns(vm);



    RootTable(vm).Bind(_SC("SqWeb"), wbns);
}

} // Namespace:: SqMod
