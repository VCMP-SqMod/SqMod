#pragma once

// ------------------------------------------------------------------------------------------------
#include "Core/Utility.hpp"
#include "Library/IO/Buffer.hpp"

// ------------------------------------------------------------------------------------------------
#include <sajson.h>
#include <fmt/args.h>
#include <fmt/format.h>
#include <fmt/xchar.h>


// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * JSON serializer. The generated JSON output is always minified for efficiency reasons.
*/
struct CtxJSON
{
    /* --------------------------------------------------------------------------------------------
     * Output string.
    */
    String mOutput{};

    /* --------------------------------------------------------------------------------------------
     * Prefer a table with named members even when a simple array would do the job.
     * Take a Vector3 for example. Compact array [x, y, z] or named object {x: #.#, y: #.#, z: #.#}
    */
    bool mObjectOverArray{true};

    /* --------------------------------------------------------------------------------------------
     * How nested are we currently.
    */
    uint32_t mDepth{0};

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    CtxJSON() noexcept = default;

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit CtxJSON(bool ooa) noexcept
        : CtxJSON()
    {
        mObjectOverArray = ooa;
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    CtxJSON(const CtxJSON &) = default;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    CtxJSON(CtxJSON &&) noexcept = default;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~CtxJSON() = default;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    CtxJSON & operator = (const CtxJSON &) = default;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    CtxJSON & operator = (CtxJSON &&) noexcept = default;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the current depth.
    */
    SQMOD_NODISCARD const String & GetOutput() const noexcept
    {
        return mOutput;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the current depth.
    */
    SQMOD_NODISCARD SQInteger GetDepth() const noexcept
    {
        return mDepth;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve whether objects are preferred over arrays.
    */
    SQMOD_NODISCARD bool GetObjectOverArray() const noexcept
    {
        return mObjectOverArray;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve whether objects are preferred over arrays.
    */
    CtxJSON & SetObjectOverArray(bool toggle) noexcept
    {
        mObjectOverArray = toggle;
        // Allow chaining
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Increase indentation by one level.
    */
    void Advance() noexcept
    {
        ++mDepth;
    }

    /* --------------------------------------------------------------------------------------------
     * Decrease indentation by one level.
    */
    void Retreat() noexcept
    {
        assert(mDepth > 0);
        if (mDepth) --mDepth;
    }

    /* --------------------------------------------------------------------------------------------
     * Begin writing an array.
    */
    CtxJSON & OpenArray();

    /* --------------------------------------------------------------------------------------------
     * Stop writing an array.
    */
    CtxJSON & CloseArray();

    /* --------------------------------------------------------------------------------------------
     * Begin writing an object.
    */
    CtxJSON & OpenObject();

    /* --------------------------------------------------------------------------------------------
     * Stop writing an object.
    */
    CtxJSON & CloseObject();

    /* --------------------------------------------------------------------------------------------
     * Begin writing a key value.
    */
    CtxJSON & MakeKey();

    /* --------------------------------------------------------------------------------------------
     * Check whether the specified weak-ref points to a type of value that must be wrapped.
    */
    SQMOD_NODISCARD static bool CheckWeakRefWrap(HSQUIRRELVM vm, SQInteger idx) noexcept;

    /* --------------------------------------------------------------------------------------------
     * Serialize given arguments.
    */
    SQRESULT SerializeParams(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * Serialize the value a specific position in the stack.
    */
    SQRESULT SerializeAt(HSQUIRRELVM vm, SQInteger idx);

    /* --------------------------------------------------------------------------------------------
     * Serialize the array a specific position in the stack. Stack index must be absolute!
    */
    SQRESULT SerializeArray(HSQUIRRELVM vm, SQInteger idx);

    /* --------------------------------------------------------------------------------------------
     * Serialize the table a specific position in the stack. Stack index must be absolute!
    */
    SQRESULT SerializeTable(HSQUIRRELVM vm, SQInteger idx);

    /* --------------------------------------------------------------------------------------------
     * Serialize the instance a specific position in the stack. Stack index must be absolute!
    */
    SQRESULT SerializeInstance(HSQUIRRELVM vm, SQInteger idx);

    /* --------------------------------------------------------------------------------------------
     * Serialize the weak-ref a specific position in the stack. Stack index must be absolute!
    */
    SQRESULT SerializeWeakRef(HSQUIRRELVM vm, SQInteger idx);

    /* --------------------------------------------------------------------------------------------
     * Serialize a value to the current container. It assumes an array or object is currently open.
    */
    SQRESULT PushValues(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * Serialize a key/value pair to the current object. It assumes an object is currently open.
    */
    SQRESULT PushElement(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * Push a key in the output. It assumes an object was open and previous element closed properly.
    */
    CtxJSON & PushKey(StackStrF & key);

    /* --------------------------------------------------------------------------------------------
     * Write a null value to the output.
    */
    void PushNull();

    /* --------------------------------------------------------------------------------------------
     * Write an integer value to the output.
    */
    void PushInteger(SQInteger value);

    /* --------------------------------------------------------------------------------------------
     * Write a single precision floating point value to the output.
    */
    void PushFloat(float value);

    /* --------------------------------------------------------------------------------------------
     * Write a double precision floating point value to the output.
    */
    void PushDouble(double value);

    /* --------------------------------------------------------------------------------------------
     * Write a boolean value to the output.
    */
    void PushBool(bool value);

    /* --------------------------------------------------------------------------------------------
     * Write a string value to the output.
    */
    void PushString(const SQChar * str);

    /* --------------------------------------------------------------------------------------------
     * Write a string value to the output.
    */
    void PushString(const SQChar * str, size_t length);
};

} // Namespace:: SqMod
