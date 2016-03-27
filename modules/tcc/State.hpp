#ifndef _SQTCC_STATE_HPP_
#define _SQTCC_STATE_HPP_

// ------------------------------------------------------------------------------------------------
#include "Common.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Class used to manage a TCC compiler state.
*/
class State
{
private:

    // --------------------------------------------------------------------------------------------
    StateHnd m_Handle; // The handle to the managed compiler state resource.

public:

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    State(CSStr name)
        : m_Handle(name)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Direct handle constructor.
    */
    State(const StateHnd & h)
        : m_Handle(h)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    State(const State & o)
        : m_Handle(o.m_Handle)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    State(State && o)
        : m_Handle(o.m_Handle)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~State()
    {
        /* Let the reference manager destroy the state when necessary. */
    }

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    State & operator = (const State & o)
    {
        m_Handle = o.m_Handle;
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    State & operator = (State && o)
    {
        m_Handle = o.m_Handle;
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Perform an equality comparison between two states.
    */
    bool operator == (const State & o) const
    {
        return (m_Handle == o.m_Handle);
    }

    /* --------------------------------------------------------------------------------------------
     * Perform an inequality comparison between two states.
    */
    bool operator != (const State & o) const
    {
        return (m_Handle != o.m_Handle);
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to the raw state handle.
    */
    operator TCCState * ()
    {
        return m_Handle;
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to the raw state handle.
    */
    operator TCCState * () const
    {
        return m_Handle;
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare two instances of this type.
    */
    Int32 Cmp(const State & o) const
    {
        if (m_Handle == m_Handle)
        {
            return 0;
        }
        else if (m_Handle.m_Hnd > o.m_Handle.m_Hnd)
        {
            return 1;
        }
        else
        {
            return -1;
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to convert an instance of this type to a string.
    */
    CSStr ToString() const
    {
        return m_Handle ? m_Handle->mName.c_str() : _SC("");
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to retrieve the name from instances of this type.
    */
    static SQInteger Typename(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * See whether this state is valid.
    */
    bool IsValid() const
    {
        return m_Handle;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated state handle.
    */
    const StateHnd & GetHandle() const
    {
        return m_Handle;
    }

    /* --------------------------------------------------------------------------------------------
     * Return the number of active references to this state handle.
    */
    Uint32 GetRefCount() const
    {
        return m_Handle.Count();
    }

    /* --------------------------------------------------------------------------------------------
     * Release the reference to the associated database state.
    */
    void Release()
    {
        m_Handle.Drop();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated state name.
    */
    const String & GetName() const
    {
        // Validate the managed state
        m_Handle.Validate();
        // Return the requested information
        return m_Handle->mName;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the associated state name.
    */
    void SetName(CSStr name)
    {
        // Validate the managed state
        m_Handle.Validate();
        // Apply the specified value
        m_Handle->mName.assign(name ? name : _SC(""));
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated state receives events.
    */
    bool GetEvents() const
    {
        // Validate the managed state
        m_Handle.Validate();
        // Return the requested information
        return StateHnd::IsListed(m_Handle.m_Hnd);
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the associated state receives events.
    */
    void SetEvents(bool toggle)
    {
        // Validate the managed state
        m_Handle.Validate();
        // Perform the requested operation
        if (toggle)
        {
            StateHnd::Remember(m_Handle.m_Hnd);
        }
        else
        {
            StateHnd::Forget(m_Handle.m_Hnd);
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Move the associated state to the head of the list.
    */
    void MakeHead() const
    {
        // Validate the managed state
        m_Handle.Validate();
        // Perform the requested operation
        StateHnd::ToHead(m_Handle.m_Hnd);
    }

    /* --------------------------------------------------------------------------------------------
     * Set options as from command line (multiple supported).
    */
    void SetOptions(CSStr opt) const
    {
        m_Handle.SetOptions(opt);
    }

    /* --------------------------------------------------------------------------------------------
     * Define preprocessor symbol 'sym' without value.
    */
    void DefineSymbol(CSStr sym) const
    {
        m_Handle.DefineSymbol(sym, nullptr);
    }

    /* --------------------------------------------------------------------------------------------
     * Define preprocessor symbol 'sym' with value.
    */
    void DefineSymbol(CSStr sym, CSStr value) const
    {
        m_Handle.DefineSymbol(sym, value);
    }

    /* --------------------------------------------------------------------------------------------
     * Undefine preprocess symbol 'sym'.
    */
    void UndefineSymbol(CSStr sym) const
    {
        m_Handle.UndefineSymbol(sym);
    }

    /* --------------------------------------------------------------------------------------------
     * Add include path.
    */
    void AddIncludePath(CSStr path) const
    {
        m_Handle.AddIncludePath(path);
    }

    /* --------------------------------------------------------------------------------------------
     * Add in system include path.
    */
    void AddSysIncludePath(CSStr path) const
    {
        m_Handle.AddSysIncludePath(path);
    }

    /* --------------------------------------------------------------------------------------------
     * Set CONFIG_TCCDIR at runtime.
    */
    void SetLibPath(CSStr path) const
    {
        m_Handle.SetLibPath(path);
    }

    /* --------------------------------------------------------------------------------------------
     * Equivalent to -Lpath option.
    */
    void AddLibraryPath(CSStr path) const
    {
        m_Handle.AddLibraryPath(path);
    }

    /* --------------------------------------------------------------------------------------------
     * The library name is the same as the argument of the '-l' option.
    */
    void AddLibrary(CSStr name) const
    {
        m_Handle.AddLibrary(name);
    }

    /* --------------------------------------------------------------------------------------------
     * Compile a string containing a C source.
    */
    void CompileString(CSStr str) const
    {
        m_Handle.CompileString(str);
    }

    /* --------------------------------------------------------------------------------------------
     * Add a file containing C source.
    */
    void AddSource(CSStr filename) const;

    /* --------------------------------------------------------------------------------------------
     * Add a binary file DLL, Object, Library etc.
    */
    void AddBinary(CSStr filename) const;

    /* --------------------------------------------------------------------------------------------
     * Add a file containing assembly code.
    */
    void AddAsm(CSStr filename) const;

    /* --------------------------------------------------------------------------------------------
     * Add a file containing assembly code.
    */
    void AddAsmPP(CSStr filename) const;

    /* --------------------------------------------------------------------------------------------
     * Do all relocations (needed before using tcc_get_symbol())
    */
    void Relocate() const
    {
        m_Handle.Relocate();
    }

    /* --------------------------------------------------------------------------------------------
     * Return symbol from the compiled code.
    */
    Symbol GetSymbol(CSStr name) const;
};

} // Namespace:: SqMod

#endif // _SQTCC_STATE_HPP_
