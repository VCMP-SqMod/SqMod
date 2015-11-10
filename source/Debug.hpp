#ifndef _DEBUG_HPP_
#define _DEBUG_HPP_

// ------------------------------------------------------------------------------------------------
#include "Common.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Class meant to help with the debugging process by offering tools for extracting information.
*/
class Debug
{
    /* --------------------------------------------------------------------------------------------
     * Allow only the smart pointer to delete this class instance as soon as it's not needed.
    */
    friend class std::unique_ptr< Debug, void(*)(Debug *) >;

protected:

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    Debug();

    /* --------------------------------------------------------------------------------------------
     * Copy constructor (disabled).
    */
    Debug(const Debug & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move constructor (disabled).
    */
    Debug(Debug && o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~Debug();

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator (disabled).
    */
    Debug & operator = (const Debug & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator (disabled).
    */
    Debug & operator = (Debug && o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Called by the smart pointer to delete the instance of this class.
    */
    static void _Finalizer(Debug * ptr);

public:

    // --------------------------------------------------------------------------------------------
    typedef std::unique_ptr< Debug, void(*)(Debug *) >  Pointer;

    /* --------------------------------------------------------------------------------------------
     * Creates an instance of this type if one doesn't already exist and returns it.
    */
    static Pointer Inst();

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    static constexpr Uint8 WRN_PRINT = (1 << 0);
    static constexpr Uint8 ERR_PRINT = (1 << 1);
    static constexpr Uint8 FTL_PRINT = (1 << 2);
    static constexpr Uint8 PRINT_ALL = (WRN_PRINT | ERR_PRINT | FTL_PRINT);

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    static constexpr Uint8 WRN_TRACE = (1 << 3);
    static constexpr Uint8 ERR_TRACE = (1 << 4);
    static constexpr Uint8 FTL_TRACE = (1 << 5);
    static constexpr Uint8 TRACE_ALL = (WRN_TRACE | ERR_TRACE | FTL_TRACE);

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    HSQUIRRELVM GetVM();

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void SetVM(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void PrintTrace(SQInt32 lvl, SQInt32 end);

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void PrintCallstack(SQInt32 lvl, SQInt32 end);

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    void SetInf(const char * type, const char * func);

    /* --------------------------------------------------------------------------------------------
     * Throws a warning message.
    */
    void Wrn(const char * type, const char * func, const char * fmt, va_list args);

    /* --------------------------------------------------------------------------------------------
     * Throws a error message.
    */
    void Err(const char * type, const char * func, const char * fmt, va_list args);

    /* --------------------------------------------------------------------------------------------
     * Throws a fatal message.
    */
    void Ftl(const char * type, const char * func, const char * fmt, va_list args);

    /* --------------------------------------------------------------------------------------------
     * Throws a warning message.
    */
    void Wrn(const char * fmt, ...);

    /* --------------------------------------------------------------------------------------------
     * Throws a error message.
    */
    void Err(const char * fmt, ...);

    /* --------------------------------------------------------------------------------------------
     * Throws a fatal message.
    */
    void Ftl(const char * fmt, ...);

    /* --------------------------------------------------------------------------------------------
     * Throws a warning message.
    */
    void Wrn(const char * func, const char * fmt, ...);

    /* --------------------------------------------------------------------------------------------
     * Throws a error message.
    */
    void Err(const char * func, const char * fmt, ...);

    /* --------------------------------------------------------------------------------------------
     * Throws a fatal message.
    */
    void Ftl(const char * func, const char * fmt, ...);

    /* --------------------------------------------------------------------------------------------
     * Throws a warning message.
    */
    void Wrn(const char * type, const char * func, const char * fmt, ...);

    /* --------------------------------------------------------------------------------------------
     * Throws a error message.
    */
    void Err(const char * type, const char * func, const char * fmt, ...);

    /* --------------------------------------------------------------------------------------------
     * Throws a fatal message.
    */
    void Ftl(const char * type, const char * func, const char * fmt, ...);

protected:

    /* --------------------------------------------------------------------------------------------
     * Internal function used to output a traceback of function calls when issues occur.
    */
    void InternalTrace();

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    String EnvName(SQInt32 level);

private:

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    HSQUIRRELVM m_VM;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    Uint8       m_Opts;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    String      m_Type;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    String      m_Func;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    String      m_RootName;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    String      m_UnknownType;

    /* --------------------------------------------------------------------------------------------
     * ...
    */
    String      m_UnknownFunc;
};

// ------------------------------------------------------------------------------------------------
extern const Debug::Pointer _Dbg;

} // Namespace:: SqMod

#endif // _DEBUG_HPP_
