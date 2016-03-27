#ifndef _SQTCC_SYMBOL_HPP_
#define _SQTCC_SYMBOL_HPP_

// ------------------------------------------------------------------------------------------------
#include "Common.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 *
*/
class Symbol
{
    // --------------------------------------------------------------------------------------------
    friend class State;

public:

    // --------------------------------------------------------------------------------------------
    typedef void * SymPtr; // The type of value that represents a symbol in the compiler state.

private:

    // --------------------------------------------------------------------------------------------
    SymPtr      m_Handle; // Pointer to the actual retrieved symbol.
    StateHnd    m_State; // The state from where this symbol was retrieved.
    String      m_Name; // The name of the retrieved symbol.

protected:

    /* --------------------------------------------------------------------------------------------
     * Use the specified symbol and name.
    */
    Symbol(const StateHnd & state, CSStr name, SymPtr sym);

public:

    /* --------------------------------------------------------------------------------------------
     * Extract the symbol from the specified state.
    */
    Symbol(const State & state, CSStr name);

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    Symbol(const Symbol & o)
        : m_Handle(o.m_Handle)
        , m_State(o.m_State)
        , m_Name(o.m_Name)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    Symbol(Symbol && o)
        : m_Handle(o.m_Handle)
        , m_State(o.m_State)
        , m_Name(o.m_Name)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~Symbol()
    {
        /* Let the reference manager destroy the state when necessary. */
    }

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    Symbol & operator = (const Symbol & o)
    {
        m_Handle = o.m_Handle;
        m_State = o.m_State;
        m_Name = o.m_Name;
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    Symbol & operator = (Symbol && o)
    {
        m_Handle = o.m_Handle;
        m_State = o.m_State;
        m_Name = o.m_Name;
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Perform an equality comparison between two symbols.
    */
    bool operator == (const Symbol & o) const
    {
        return (m_Handle == o.m_Handle);
    }

    /* --------------------------------------------------------------------------------------------
     * Perform an inequality comparison between two symbols.
    */
    bool operator != (const Symbol & o) const
    {
        return (m_Handle != o.m_Handle);
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to the raw state handle.
    */
    operator TCCState * ()
    {
        return m_State;
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to the raw state handle.
    */
    operator TCCState * () const
    {
        return m_State;
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare two instances of this type.
    */
    Int32 Cmp(const Symbol & o) const
    {
        if (m_Handle == m_Handle)
        {
            return 0;
        }
        else if (m_Handle > o.m_Handle)
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
    const String & ToString() const
    {
        return m_Name;
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to retrieve the name from instances of this type.
    */
    static SQInteger Typename(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * See whether this symbol is valid.
    */
    bool IsValid() const
    {
        return m_Handle;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated symbol handle.
    */
    SymPtr GetHandle() const
    {
        return m_Handle;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated state handle.
    */
    const StateHnd & GetState() const
    {
        return m_State;
    }

    /* --------------------------------------------------------------------------------------------
     * Return the number of active references to this state handle.
    */
    Uint32 GetRefCount() const
    {
        return m_State.Count();
    }

    /* --------------------------------------------------------------------------------------------
     * Release the reference to the associated database state.
    */
    void Release()
    {
        m_Handle = nullptr;
        m_Name.clear();
        m_State.Drop();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated state as a reference in script.
    */
    State GetStateRef() const;

    /* --------------------------------------------------------------------------------------------
     * Forward this call to the actual associated symbol.
    */
    static SQInteger Forward(HSQUIRRELVM vm);
};

} // Namespace:: SqMod

#endif // _SQTCC_SYMBOL_HPP_
