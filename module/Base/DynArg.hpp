#ifndef _BASE_DYNARG_HPP_
#define _BASE_DYNARG_HPP_

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Helper class used to check if a certain operation is possible on the specified argument.
*/
template < typename U > struct SqDynArg
{
    static inline bool CanPop(HSQUIRRELVM vm)
    {
        return (sq_gettype(vm, 2) == OT_INSTANCE);
    }

    template < typename F > static inline auto Do(F & fn, HSQUIRRELVM vm)
    {
        Var< U * > var(vm, 2);
        // Validate the obtained instance
        if (!var.value)
        {
            STHROWF("No such instance");
        }
        // Attempt to perform the requested operation
        return fn(*var.value);
    }

    template < typename F > static inline auto Do(const F & fn, HSQUIRRELVM vm)
    {
        Var< U * > var(vm, 2);
        // Validate the obtained instance
        if (!var.value)
        {
            STHROWF("No such instance");
        }
        // Attempt to perform the requested operation
        return fn(*var.value);
    }
};

/* ------------------------------------------------------------------------------------------------
 * Instance pointer specializations of the argument checking structure.
*/
template < typename U > struct SqDynArg< U * >
{
    static inline bool CanPop(HSQUIRRELVM vm)
    {
        return (sq_gettype(vm, 2) == OT_INSTANCE);
    }

    template < typename F > static inline auto Do(F & fn, HSQUIRRELVM vm)
    {
        Var< U * > var(vm, 2);
        // Validate the obtained instance
        if (!var.value)
        {
            STHROWF("No such instance");
        }
        // Attempt to perform the requested operation
        return fn(var.value);
    }

    template < typename F > static inline auto Do(const F & fn, HSQUIRRELVM vm)
    {
        Var< U * > var(vm, 2);
        // Validate the obtained instance
        if (!var.value)
        {
            STHROWF("No such instance");
        }
        // Attempt to perform the requested operation
        return fn(var.value);
    }
};

/* ------------------------------------------------------------------------------------------------
 * Instance pointer specializations of the argument checking structure.
*/
template < typename U > struct SqDynArg< const U * >
{
    static inline bool CanPop(HSQUIRRELVM vm)
    {
        return (sq_gettype(vm, 2) == OT_INSTANCE);
    }

    template < typename F > static inline auto Do(F & fn, HSQUIRRELVM vm)
    {
        Var< U * > var(vm, 2);
        // Validate the obtained instance
        if (!var.value)
        {
            STHROWF("No such instance");
        }
        // Attempt to perform the requested operation
        return fn(*var.value);
    }

    template < typename F > static inline auto Do(const F & fn, HSQUIRRELVM vm)
    {
        Var< U * > var(vm, 2);
        // Validate the obtained instance
        if (!var.value)
        {
            STHROWF("No such instance");
        }
        // Attempt to perform the requested operation
        return fn(*var.value);
    }
};

/* ------------------------------------------------------------------------------------------------
 * Inherited by the base specializations of the argument checking structure.
*/
template < typename U, SQObjectType SqT > struct SqDynArgBase
{
    static inline bool CanPop(HSQUIRRELVM vm)
    {
        return (sq_gettype(vm, 2) == SqT);
    }

    template < typename F > static inline auto Do(F & fn, HSQUIRRELVM vm)
    {
        Var< U > var(vm, 2);
        // Attempt to perform the requested operation
        return fn(var.value);
    }

    template < typename F > static inline auto Do(const F & fn, HSQUIRRELVM vm)
    {
        Var< U > var(vm, 2);
        // Attempt to perform the requested operation
        return fn(var.value);
    }
};

// ------------------------------------------------------------------------------------------------
template < > struct SqDynArg< char >
    : public SqDynArgBase< char, OT_INTEGER >
{ /* ... */ };

// ------------------------------------------------------------------------------------------------
template < > struct SqDynArg< signed char >
    : public SqDynArgBase< signed char, OT_INTEGER >
{ /* ... */ };

// ------------------------------------------------------------------------------------------------
template < > struct SqDynArg< unsigned char >
    : public SqDynArgBase< unsigned char, OT_INTEGER >
{ /* ... */ };

// ------------------------------------------------------------------------------------------------
template < > struct SqDynArg< signed short >
    : public SqDynArgBase< signed short, OT_INTEGER >
{ /* ... */ };

// ------------------------------------------------------------------------------------------------
template < > struct SqDynArg< unsigned short >
    : public SqDynArgBase< unsigned short, OT_INTEGER >
{ /* ... */ };

// ------------------------------------------------------------------------------------------------
template < > struct SqDynArg< signed int >
    : public SqDynArgBase< signed int, OT_INTEGER >
{ /* ... */ };

// ------------------------------------------------------------------------------------------------
template < > struct SqDynArg< unsigned int >
    : public SqDynArgBase< unsigned int, OT_INTEGER >
{ /* ... */ };

// ------------------------------------------------------------------------------------------------
template < > struct SqDynArg< signed long >
    : public SqDynArgBase< signed long, OT_INTEGER >
{ /* ... */ };

// ------------------------------------------------------------------------------------------------
template < > struct SqDynArg< unsigned long >
    : public SqDynArgBase< unsigned long, OT_INTEGER >
{ /* ... */ };

// ------------------------------------------------------------------------------------------------
template < > struct SqDynArg< signed long long >
    : public SqDynArgBase< signed long long, OT_INTEGER >
{ /* ... */ };

// ------------------------------------------------------------------------------------------------
template < > struct SqDynArg< unsigned long long >
    : public SqDynArgBase< unsigned long long, OT_INTEGER >
{ /* ... */ };

// ------------------------------------------------------------------------------------------------
template < > struct SqDynArg< float >
    : public SqDynArgBase< float, OT_FLOAT >
{ /* ... */ };

// ------------------------------------------------------------------------------------------------
template < > struct SqDynArg< double >
    : public SqDynArgBase< double, OT_FLOAT >
{ /* ... */ };

// ------------------------------------------------------------------------------------------------
template < > struct SqDynArg< bool >
    : public SqDynArgBase< bool, OT_BOOL >
{ /* ... */ };

// ------------------------------------------------------------------------------------------------
template < > struct SqDynArg< SQChar * >
    : public SqDynArgBase< SQChar *, OT_STRING >
{ /* ... */ };

// ------------------------------------------------------------------------------------------------
template < > struct SqDynArg< const SQChar * >
    : public SqDynArgBase< const SQChar *, OT_STRING >
{ /* ... */ };

// ------------------------------------------------------------------------------------------------
template < > struct SqDynArg< String >
    : public SqDynArgBase< String, OT_STRING >
{ /* ... */ };

// ------------------------------------------------------------------------------------------------
template < > struct SqDynArg< Table >
    : public SqDynArgBase< Table, OT_TABLE >
{ /* ... */ };

// ------------------------------------------------------------------------------------------------
template < > struct SqDynArg< Array >
    : public SqDynArgBase< Array, OT_ARRAY >
{ /* ... */ };

/* ------------------------------------------------------------------------------------------------
 * Specialization of the argument checking structure for functions.
*/
template < > struct SqDynArg< Function >
{
    static inline bool CanPop(HSQUIRRELVM vm)
    {
        const SQObjectType type = sq_gettype(vm, 2);
        return (type == OT_CLOSURE || type == OT_NATIVECLOSURE);
    }

    template < typename F > static inline auto Do(F & fn, HSQUIRRELVM vm)
    {
        Var< Function > var(vm, 2);
        // Attempt to perform the requested operation
        return fn(var.value);
    }

    template < typename F > static inline auto Do(const F & fn, HSQUIRRELVM vm)
    {
        Var< Function > var(vm, 2);
        // Attempt to perform the requested operation
        return fn(var.value);
    }
};

/* ------------------------------------------------------------------------------------------------
 * Specialization of the argument checking structure for objects.
*/
template < > struct SqDynArg< Object >
{
    static inline bool CanPop(HSQUIRRELVM /*vm*/)
    {
        return true; // Objects can use any type.
    }

    template < typename F > static inline auto Do(F & fn, HSQUIRRELVM vm)
    {
        Var< Object > var(vm, 2);
        // Attempt to perform the requested operation
        return fn(var.value);
    }

    template < typename F > static inline auto Do(const F & fn, HSQUIRRELVM vm)
    {
        Var< Object > var(vm, 2);
        // Attempt to perform the requested operation
        return fn(var.value);
    }
};

/* ------------------------------------------------------------------------------------------------
 * Specialization of the argument checking structure for null.
*/
template < > struct SqDynArg< std::nullptr_t >
{
    static inline bool CanPop(HSQUIRRELVM vm)
    {
        return (sq_gettype(vm, 2) == OT_NULL);
    }

    template < typename F > static inline auto Do(F & fn, HSQUIRRELVM /*vm*/)
    {
        // Attempt to perform the requested operation
        return fn(nullptr);
    }

    template < typename F > static inline auto Do(const F & fn, HSQUIRRELVM /*vm*/)
    {
        // Attempt to perform the requested operation
        return fn(nullptr);
    }
};

/* ------------------------------------------------------------------------------------------------
 * Used to perform an operation on multiple types specified at compile time.
*/
template < typename... Ts > struct SqDynArgImpl;

/* ------------------------------------------------------------------------------------------------
 * Zeroth case of the operation. No known operation of such pairs of types at this point.
*/
template < > struct SqDynArgImpl< >
{
    template < typename T > static Int32 Try(const T & /*val*/, HSQUIRRELVM vm)
    {
        const String tn1(SqTypeName(vm, 1));
        const String tn2(SqTypeName(vm, 2));
        return sq_throwerror(vm, ToStrF("Such operation is not possible between (%s) and (%s)",
                                        tn1.c_str(), tn2.c_str()));
    }
};

/* ------------------------------------------------------------------------------------------------
 * Argument pack type pealing. Attempt to perform the operation with a specified type.
*/
template < typename U, typename... Ts > struct SqDynArgImpl< U, Ts... >
{
    template < typename F > static Int32 Try(F & fn, HSQUIRRELVM vm)
    {
        typedef typename std::decay< U >::type ArgType;
        // Can the stack value be used with the current type?
        if (SqDynArg< ArgType >::CanPop(vm))
        {
            // If not an instance then don't use a try catch block
            if (sq_gettype(vm, 2) != OT_INSTANCE)
            {
                // Attempt to perform the operation and push the result on the stack
                PushVar(vm, SqDynArg< ArgType >::Do(fn, vm));
                // Specify that we completed the search and have a result
                return 1;
            }
            // Instances require a try/catch block since we can't differentiate types
            try
            {
                // Attempt to perform the operation and push the result on the stack
                PushVar(vm, SqDynArg< ArgType >::Do(fn, vm));
                // Specify that we completed the search and have a result
                return 1;
            }
            catch (const Sqrat::Exception & e)
            {
                // Probably the wrong type
            }
            catch (...)
            {
                // Something very bad happened. At this point, we just don't want to let
                // exceptions propagate to the virtual machine and we must end here.
                // Either way, reaching this point is bad and we just shuved it under
                // the rug. This is bad practice but circumstances forced me.
                // Don't do this at home kids!
                return -1;
            }
        }
        // On to the next type
        return SqDynArgImpl< Ts... >::Try(fn, vm);
    }
};

/* ------------------------------------------------------------------------------------------------
 * Squirrel function that forwards the call to the actual implementation.
*/
template < typename F, typename U, typename... Ts > SQInteger SqDynArgFwd(HSQUIRRELVM vm)
{
    // Make sure that there are enough parameters on the stack
    if (sq_gettop(vm) < 2)
    {
        return sq_throwerror(vm, "Insufficient parameters for such operation");
    }
    // Attempt to create the functor that performs the operation
    F fn(vm);
    // Is this functor in a valid state?
    if (!fn)
    {
        return sq_throwerror(vm, "Functor failed to initialize");
    }
    // Attempt to perform the comparison
    try
    {
        return SqDynArgImpl< U, Ts... >::Try(fn, vm);
    }
    catch (const Sqrat::Exception & e)
    {
        return sq_throwerror(vm, e.what());
    }
    catch (...)
    {
        return sq_throwerror(vm, "Unknown error occurred during comparison");
    }
    // We shouldn't really reach this point but something must be returned
    return sq_throwerror(vm, "Operation encountered unknown behavior");
}

/* ------------------------------------------------------------------------------------------------
 * Functor used to perform comparison.
*/
template < typename T > struct SqDynArgCmpFn
{
    // --------------------------------------------------------------------------------------------
    HSQUIRRELVM     mVM; // The squirrel virtual machine.
    Var< T * >      mVar; // The instance of the base type.

    /* --------------------------------------------------------------------------------------------
     * Base constructor. (required)
    */
    SqDynArgCmpFn(HSQUIRRELVM vm)
        : mVM(vm), mVar(vm, 1)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to boolean. (required)
    */
    operator bool () const
    {
        return (mVar.value != nullptr);
    }

    /* --------------------------------------------------------------------------------------------
     * Function call operator for references. (required)
    */
    template < typename U > SQInteger operator () (U & v) const
    {
        return mVar.value->Cmp(v);
    }

    /* --------------------------------------------------------------------------------------------
     * Function call operator for const references. (required)
    */
    template < typename U > SQInteger operator () (const U & v) const
    {
        return mVar.value->Cmp(v);
    }

    /* --------------------------------------------------------------------------------------------
     * Function call operator for pointers. (required)
    */
    template < typename U > SQInteger operator () (U * v) const
    {
        return mVar.value->Cmp(v);
    }

    /* --------------------------------------------------------------------------------------------
     * Function call operator for const pointers. (required)
    */
    template < typename U > SQInteger operator () (const U * v) const
    {
        return mVar.value->Cmp(v);
    }

    /* --------------------------------------------------------------------------------------------
     * Function call operator for null pointers. (required)
    */
    SQInteger operator () (std::nullptr_t) const
    {
        return mVar.value->Cmp(nullptr);
    }
};

/* ------------------------------------------------------------------------------------------------
 * Functor used to perform addition.
*/
template < typename T > struct SqDynArgAddFn
{
    // --------------------------------------------------------------------------------------------
    HSQUIRRELVM     mVM; // The squirrel virtual machine.
    Var< T * >      mVar; // The instance of the base type.

    /* --------------------------------------------------------------------------------------------
     * Base constructor. (required)
    */
    SqDynArgAddFn(HSQUIRRELVM vm)
        : mVM(vm), mVar(vm, 1)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to boolean. (required)
    */
    operator bool () const
    {
        return (mVar.value != nullptr);
    }

    /* --------------------------------------------------------------------------------------------
     * Function call operator for references. (required)
    */
    template < typename U > T operator () (U & v) const
    {
        return (*mVar.value) + v;
    }

    /* --------------------------------------------------------------------------------------------
     * Function call operator for const references. (required)
    */
    template < typename U > T operator () (const U & v) const
    {
        return (*mVar.value) + v;
    }

    /* --------------------------------------------------------------------------------------------
     * Function call operator for pointers. (required)
    */
    template < typename U > T operator () (U * v) const
    {
        return (*mVar.value) + v;
    }

    /* --------------------------------------------------------------------------------------------
     * Function call operator for const pointers. (required)
    */
    template < typename U > T operator () (const U * v) const
    {
        return (*mVar.value) + v;
    }

    /* --------------------------------------------------------------------------------------------
     * Function call operator for null pointers. (required)
    */
    T operator () (std::nullptr_t) const
    {
        return (*mVar.value);
    }
};

/* ------------------------------------------------------------------------------------------------
 * Functor used to perform subtraction.
*/
template < typename T > struct SqDynArgSubFn
{
    // --------------------------------------------------------------------------------------------
    HSQUIRRELVM     mVM; // The squirrel virtual machine.
    Var< T * >      mVar; // The instance of the base type.

    /* --------------------------------------------------------------------------------------------
     * Base constructor. (required)
    */
    SqDynArgSubFn(HSQUIRRELVM vm)
        : mVM(vm), mVar(vm, 1)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to boolean. (required)
    */
    operator bool () const
    {
        return (mVar.value != nullptr);
    }

    /* --------------------------------------------------------------------------------------------
     * Function call operator for references. (required)
    */
    template < typename U > T operator () (U & v) const
    {
        return (*mVar.value) - v;
    }

    /* --------------------------------------------------------------------------------------------
     * Function call operator for const references. (required)
    */
    template < typename U > T operator () (const U & v) const
    {
        return (*mVar.value) - v;
    }

    /* --------------------------------------------------------------------------------------------
     * Function call operator for pointers. (required)
    */
    template < typename U > T operator () (U * v) const
    {
        return (*mVar.value) - v;
    }

    /* --------------------------------------------------------------------------------------------
     * Function call operator for const pointers. (required)
    */
    template < typename U > T operator () (const U * v) const
    {
        return (*mVar.value) - v;
    }

    /* --------------------------------------------------------------------------------------------
     * Function call operator for null pointers. (required)
    */
    T operator () (std::nullptr_t) const
    {
        return (*mVar.value);
    }
};

/* ------------------------------------------------------------------------------------------------
 * Functor used to perform multiplication.
*/
template < typename T > struct SqDynArgMulFn
{
    // --------------------------------------------------------------------------------------------
    HSQUIRRELVM     mVM; // The squirrel virtual machine.
    Var< T * >      mVar; // The instance of the base type.

    /* --------------------------------------------------------------------------------------------
     * Base constructor. (required)
    */
    SqDynArgMulFn(HSQUIRRELVM vm)
        : mVM(vm), mVar(vm, 1)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to boolean. (required)
    */
    operator bool () const
    {
        return (mVar.value != nullptr);
    }

    /* --------------------------------------------------------------------------------------------
     * Function call operator for references. (required)
    */
    template < typename U > T operator () (U & v) const
    {
        return (*mVar.value) * v;
    }

    /* --------------------------------------------------------------------------------------------
     * Function call operator for const references. (required)
    */
    template < typename U > T operator () (const U & v) const
    {
        return (*mVar.value) * v;
    }

    /* --------------------------------------------------------------------------------------------
     * Function call operator for pointers. (required)
    */
    template < typename U > T operator () (U * v) const
    {
        return (*mVar.value) * v;
    }

    /* --------------------------------------------------------------------------------------------
     * Function call operator for const pointers. (required)
    */
    template < typename U > T operator () (const U * v) const
    {
        return (*mVar.value) * v;
    }

    /* --------------------------------------------------------------------------------------------
     * Function call operator for null pointers. (required)
    */
    T operator () (std::nullptr_t) const
    {
        return (*mVar.value) * static_cast< SQInteger >(0);
    }
};

/* ------------------------------------------------------------------------------------------------
 * Functor used to perform division.
*/
template < typename T > struct SqDynArgDivFn
{
    // --------------------------------------------------------------------------------------------
    HSQUIRRELVM     mVM; // The squirrel virtual machine.
    Var< T * >      mVar; // The instance of the base type.

    /* --------------------------------------------------------------------------------------------
     * Base constructor. (required)
    */
    SqDynArgDivFn(HSQUIRRELVM vm)
        : mVM(vm), mVar(vm, 1)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to boolean. (required)
    */
    operator bool () const
    {
        return (mVar.value != nullptr);
    }

    /* --------------------------------------------------------------------------------------------
     * Function call operator for references. (required)
    */
    template < typename U > T operator () (U & v) const
    {
        return (*mVar.value) / v;
    }

    /* --------------------------------------------------------------------------------------------
     * Function call operator for const references. (required)
    */
    template < typename U > T operator () (const U & v) const
    {
        return (*mVar.value) / v;
    }

    /* --------------------------------------------------------------------------------------------
     * Function call operator for pointers. (required)
    */
    template < typename U > T operator () (U * v) const
    {
        return (*mVar.value) / v;
    }

    /* --------------------------------------------------------------------------------------------
     * Function call operator for const pointers. (required)
    */
    template < typename U > T operator () (const U * v) const
    {
        return (*mVar.value) / v;
    }

    /* --------------------------------------------------------------------------------------------
     * Function call operator for null pointers. (required)
    */
    T operator () (std::nullptr_t) const
    {
        return (*mVar.value) / static_cast< SQInteger >(0);
    }
};

/* ------------------------------------------------------------------------------------------------
 * Functor used to perform modulus.
*/
template < typename T > struct SqDynArgModFn
{
    // --------------------------------------------------------------------------------------------
    HSQUIRRELVM     mVM; // The squirrel virtual machine.
    Var< T * >      mVar; // The instance of the base type.

    /* --------------------------------------------------------------------------------------------
     * Base constructor. (required)
    */
    SqDynArgModFn(HSQUIRRELVM vm)
        : mVM(vm), mVar(vm, 1)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to boolean. (required)
    */
    operator bool () const
    {
        return (mVar.value != nullptr);
    }

    /* --------------------------------------------------------------------------------------------
     * Function call operator for references. (required)
    */
    template < typename U > T operator () (U & v) const
    {
        return (*mVar.value) % v;
    }

    /* --------------------------------------------------------------------------------------------
     * Function call operator for const references. (required)
    */
    template < typename U > T operator () (const U & v) const
    {
        return (*mVar.value) % v;
    }

    /* --------------------------------------------------------------------------------------------
     * Function call operator for pointers. (required)
    */
    template < typename U > T operator () (U * v) const
    {
        return (*mVar.value) % v;
    }

    /* --------------------------------------------------------------------------------------------
     * Function call operator for const pointers. (required)
    */
    template < typename U > T operator () (const U * v) const
    {
        return (*mVar.value) % v;
    }

    /* --------------------------------------------------------------------------------------------
     * Function call operator for null pointers. (required)
    */
    T operator () (std::nullptr_t) const
    {
        return (*mVar.value) % static_cast< SQInteger >(0);
    }
};

} // Namespace:: SqMod

#endif // _BASE_DYNARG_HPP_
