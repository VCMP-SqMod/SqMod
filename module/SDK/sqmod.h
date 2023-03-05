#if !defined(_SQ_MOD_API_H_)
#define _SQ_MOD_API_H_

#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

    #ifndef SQMOD_API_EXPORT
        #if defined(_MSC_VER)
            #define SQMOD_API_EXPORT    extern "C" __declspec(dllexport)
        #elif defined(__GNUC__)
            #define SQMOD_API_EXPORT    extern "C"
        #else
            #define SQMOD_API_EXPORT    extern "C"
        #endif
    #endif

    #define SQMOD_HOST_NAME         "SqModHost"
    #define SQMOD_INITIALIZE_CMD    0xDABBAD00
    #define SQMOD_LOAD_CMD          0xDEADBABE // API is being registered
    #define SQMOD_TERMINATE_CMD     0xDEADC0DE // release your resources
    #define SQMOD_CLOSING_CMD       0xBAAAAAAD // virtual machine is closing
    #define SQMOD_RELEASED_CMD      0xDEADBEAF // virtual machine was closed
    #define SQMOD_API_VER           1

    // --------------------------------------------------------------------------------------------
    typedef int32_t(*ExtPluginCommand_t)(int32_t, int32_t,  int32_t, const uint8_t *, size_t);

    /* --------------------------------------------------------------------------------------------
     * The structure exported by the host plug-in to import the module and squirrel API.
    */
    typedef struct
    {
        uint32_t StructSize;
        /* Register a pointer to a function used to processes commands from script.
            This is like the functionality offered by SendPluginCommand but offers interaction from
            script side without interfering with other native plugins. And also offers a few extra
            methods of identification to provide back and forth communication.
            It offers a bare minimum, primitive way of interacting with the script from native plug-ins.
            return : -1 it failed (no free slot), 0 if it was already registered and 1 if it succeeded.
        */
        int32_t (*RegisterCommand) (ExtPluginCommand_t fn);
        /* Remove a pointer to a function used to processes commands from script.
            return : -1 it failed (no free slot) and 1 if it succeeded.
        */
        int32_t (*UnregisterCommand) (ExtPluginCommand_t fn);
        /* Send a command to all functions currently registered to receive them. This is mostly used by the script.
            target - ideally a unique value that can be used to identify the intended audience for the command.
            req : ideally a unique value that can be used to identify the requested information from the command.
            tag : ideally a unique value that can be used to identify a later response if one is generated.
            data : binary data that represents the command payload. the command is free to interpret it however it wants.
            size :  size of the binary data. most likely in bytes but the command is free to interpret it however it wants.
        */
        int32_t (*SendCommand) (int32_t target, int32_t req, int32_t tag, const uint8_t * data, size_t size);
        /* Send a response to the script that may have resulted from a previous command. This is mostly by the native plug-ins.
            sender : ideally a unique value that can be used to identify the intended who generated the response.
            tag : ideally a unique value that can be used to identify what the generated response might contain/provide.
            data : binary data that represents the command payload. the command is free to interpret it however it wants.
            size :  size of the binary data. most likely in bytes but the command is free to interpret it however it wants.
        */
        int32_t (*SendCommandReply) (int32_t sender, int32_t tag, const uint8_t * data, size_t size);
        /* Forward an event to the script from an external plug-in. This is mostly by the native plug-ins.
            Similar to SendCommandReply but may not have been the result of a previous command.
        */
        int32_t (*SendCommandEvent) (int32_t sender, int32_t tag, const uint8_t * data, size_t size);
    } sq_mod_exports, SQ_MOD_EXPORTS, *HSQ_MOD_EXPORTS;

#ifdef __cplusplus
} /*extern "C"*/
#endif // __cplusplus

#endif /*_SQ_MOD_API_H_*/
