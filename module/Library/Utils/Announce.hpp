#pragma once

// ------------------------------------------------------------------------------------------------
#include "Core/Utility.hpp"
#include "Misc/Functions.hpp"

// ------------------------------------------------------------------------------------------------
#include <mutex>
#include <thread>
#include <vector>
#include <memory>
#include <chrono>
#include <atomic>
#include <utility>
#include <sstream>
#include <iostream>

// ------------------------------------------------------------------------------------------------
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPCredentials.h"
#include "Poco/StreamCopier.h"
#include "Poco/NullStream.h"
#include "Poco/Exception.h"
#include "Poco/Path.h"
#include "Poco/URI.h"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------

using Poco::Net::HTTPClientSession;
using Poco::Net::HTTPRequest;
using Poco::Net::HTTPResponse;
using Poco::Net::HTTPMessage;
using Poco::StreamCopier;

/* ------------------------------------------------------------------------------------------------
 * Helper utility that can be used to announce the server to a master-list.
*/
struct SqAnnouncer : SqChainedInstances< SqAnnouncer >
{
    /* --------------------------------------------------------------------------------------------
     * Announcer status.
    */
    bool mRun;

    /* --------------------------------------------------------------------------------------------
     * Log failure messages.
    */
    bool mLog;

    /* --------------------------------------------------------------------------------------------
     * Announce count.
    */
    uint32_t mCount;

    /* --------------------------------------------------------------------------------------------
     * Success count (since last failure).
    */
    uint32_t mSuccess;

    /* --------------------------------------------------------------------------------------------
     * Failure count.
    */
    uint32_t mFailures;

    /* --------------------------------------------------------------------------------------------
     * Update interval (seconds).
    */
    uint32_t mInterval;

    /* --------------------------------------------------------------------------------------------
     * The number failures to ignore.
    */
    uint32_t mRetries;

    /* --------------------------------------------------------------------------------------------
     * Server port.
    */
    const uint32_t mPort;

    /* --------------------------------------------------------------------------------------------
     * Server version.
    */
    const uint32_t mVersion;

    /* --------------------------------------------------------------------------------------------
     * Master-list address.
    */
    Poco::URI mURI;

    /* --------------------------------------------------------------------------------------------
     * Synchronization mutex.
    */
    std::mutex mMtx;

    /* --------------------------------------------------------------------------------------------
     * Processing thread.
    */
    std::thread mThread;

    /* --------------------------------------------------------------------------------------------
     * Default interval constructor.
    */
    explicit SqAnnouncer(StackStrF & uri)
        : SqAnnouncer(60,uri)
    {
    }

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    SqAnnouncer(SQInteger interval, StackStrF & uri)
        : mRun(false), mLog(true)
        , mCount(0)
        , mSuccess(0)
        , mFailures(0)
        , mInterval(ClampL< SQInteger, uint32_t >(interval))
        , mRetries(10)
        , mPort(GetServerPort())
        , mVersion(GetServerVersion())
        , mURI(uri.mPtr)
        , mMtx(), mThread()
    {
        // Remember this instance
        ChainInstance();
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor (disabled).
    */
    SqAnnouncer(const SqAnnouncer &) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move constructor (disabled).
    */
    SqAnnouncer(SqAnnouncer &&) noexcept = delete;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~SqAnnouncer()
    {
        // Forget about this instance
        UnchainInstance();
        // Terminate the announcer if necessary
        Stop();
    }

    /* --------------------------------------------------------------------------------------------
     * Assignment operator (disabled).
    */
    SqAnnouncer & operator = (const SqAnnouncer &) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move assignment (disabled).
    */
    SqAnnouncer & operator = (SqAnnouncer &&) noexcept = delete;

    /* --------------------------------------------------------------------------------------------
     * Start the announcer.
    */
    SqAnnouncer & Run()
    {
        // Is the processing thread running?
        if (mThread.joinable())
        {
            STHROWF("Announcer is already running");
        }
        // Allow the loop to run
        mRun = true;
        // Create the processing thread
        mThread = std::thread(&SqAnnouncer::Proc, this);
        // Allow chaining
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Start the announcer.
    */
    SqAnnouncer & Stop()
    {
        // Is the processing thread running?
        if (mThread.joinable())
        {
            // Acquire exclusive access
            mMtx.lock();
            // Stop the loop
            mRun = false;
            // Yield exclusive access
            mMtx.unlock();
            // Wait for the thread
            mThread.join();
        } else mRun = false; // Just in case
        // Allow chaining
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * See if the announcer is running.
    */
    bool Running()
    {
        // Acquire exclusive access to the announcer
        std::lock_guard< std::mutex > guard(mMtx);
        // Return the requested value
        return mRun;
    }

    /* --------------------------------------------------------------------------------------------
     * Check if logging is enabled when failed.
    */
    bool GetLog()
    {
        // Acquire exclusive access to the announcer
        std::lock_guard< std::mutex > guard(mMtx);
        // Return the requested value
        return mLog;
    }

    /* --------------------------------------------------------------------------------------------
     * Set if logging is enabled when failed.
    */
    void SetLog(bool toggle)
    {
        // Acquire exclusive access to the announcer
        std::lock_guard< std::mutex > guard(mMtx);
        // Modify the requested value
        mLog = toggle;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the announce interval (seconds).
    */
    uint32_t GetInterval()
    {
        // Acquire exclusive access to the announcer
        std::lock_guard< std::mutex > guard(mMtx);
        // Return the requested value
        return mInterval;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the announce interval (seconds).
    */
    void SetInterval(uint32_t seconds)
    {
        // Acquire exclusive access to the announcer
        std::lock_guard< std::mutex > guard(mMtx);
        // Modify the requested value
        mInterval = seconds;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the announce retries.
    */
    uint32_t GetRetries()
    {
        // Acquire exclusive access to the announcer
        std::lock_guard< std::mutex > guard(mMtx);
        // Return the requested value
        return mRetries;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the announce retries.
    */
    void SetRetries(uint32_t seconds)
    {
        // Acquire exclusive access to the announcer
        std::lock_guard< std::mutex > guard(mMtx);
        // Modify the requested value
        mRetries = seconds;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the announce count (total).
    */
    uint32_t GetCount()
    {
        // Acquire exclusive access to the announcer
        std::lock_guard< std::mutex > guard(mMtx);
        // Return the requested value
        return mCount;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the announce success count (since last failure).
    */
    uint32_t GetSuccess()
    {
        // Acquire exclusive access to the announcer
        std::lock_guard< std::mutex > guard(mMtx);
        // Return the requested value
        return mSuccess;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the announce failure count.
    */
    uint32_t GetFailures()
    {
        // Acquire exclusive access to the announcer
        std::lock_guard< std::mutex > guard(mMtx);
        // Return the requested value
        return mFailures;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the announce URI.
    */
    void SetURI(StackStrF & uri)
    {
        // Acquire exclusive access to the announcer
        std::lock_guard< std::mutex > guard(mMtx);
        // Modify the requested value
        mURI = uri.mPtr;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the announce URI.
    */
    String GetURI()
    {
        // Acquire exclusive access to the announcer
        std::lock_guard< std::mutex > guard(mMtx);
        // Return the requested value
        return mURI.toString();
    }

protected:

    /* --------------------------------------------------------------------------------------------
     * Processing thread.
    */
    void Proc()
    {
        while (mRun)
        {
            try
            {
                using namespace std::chrono_literals;
                // Grab the current time point
                std::chrono::time_point< std::chrono::steady_clock > next = std::chrono::steady_clock::now();
                // Set the time-point for next update
                next += std::chrono::seconds(mInterval);
                // Try to announce
                DoAnnounce();
                // Sleep until the next appointed update time point
                for (std::chrono::time_point< std::chrono::steady_clock > curr;
                    mRun && (next > curr);
                    curr = std::chrono::steady_clock::now())
                {
                    std::this_thread::sleep_for(50ms); // Sleep for 1/20'th of a second
                }
            }
            catch (const Poco::Exception & e)
            {
                if (mLog) LogErr("Announcer failed: %s -> %s", e.what(), e.message().c_str());
            }
            catch (const std::exception & e)
            {
                if (mLog) LogErr("Announcer failed: %s", e.what());
            }
            catch (...)
            {
                LogFtl("Unknown exception while announcing. Announcer stopped");
                // This is bad. Avoid a crash!
                return;
            }
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Announce succeeded.
    */
    void DoSucceeded()
    {
        // Acquire exclusive access to the announcer
        std::lock_guard< std::mutex > guard(mMtx);
        // Increment announce counter
        mCount += 1;
        // Increment success counter
        mSuccess += 1;
        // Reset failure counter
        mFailures = 0;
    }

    /* --------------------------------------------------------------------------------------------
     * Announce failed.
    */
    void DoFailed()
    {
        // Acquire exclusive access to the announcer
        std::lock_guard< std::mutex > guard(mMtx);
        // Increment announce counter
        mCount += 1;
        // Reset success counter
        mSuccess = 0;
        // Increment failure counter
        mFailures += 1;
        // Check failure limit
        if (mFailures >= mRetries)
        {
            // Stop the announcer
            mRun = false;
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Processing thread.
    */
    void DoAnnounce()
    {
        // Acquire exclusive access to the announcer
        mMtx.lock();
        // Create a new HTTP client session
        HTTPClientSession session(mURI.getHost(), mURI.getPort());
        // Create a new HTTP request
        HTTPRequest request(HTTPRequest::HTTP_POST, mURI.getPathAndQuery(), HTTPMessage::HTTP_1_1);
        // Set request content type header
        request.setContentType("application/x-www-form-urlencoded");
        // Set the user agent for the announcer
        request.add("User-Agent", "VCMP/0.4");
        // Set the server version for the announcer
        request.add("VCMP-Version", fmt::format("{}", mVersion));
        // Generate the request body
        String body = fmt::format("port={}", mPort);
        // Release exclusive access to the announcer
        mMtx.unlock(); // No need to block while waiting for response
        // Create a new HTTP response
        HTTPResponse response;
        // Set the request body length
        request.setContentLength(body.size());
        // Send the request
        try
        {
            std::ostream & os = session.sendRequest(request);
            // Send the content
            os << body;
        }
        catch (const Poco::Exception & e)
        {
            if (mLog) LogErr("Request failed: %s -> %s", e.what(), e.message().c_str());
            // Failed!
            DoFailed();
            // Probably the server is offline
            return;
        }
        catch (const std::exception & e)
        {
            if (mLog) LogErr("Request failed: %s", e.what());
            // Failed!
            DoFailed();
            // Probably the server is offline
            return;
        }
        // Receive the response
        std::istream & is = session.receiveResponse(response);
        // Validate the response from the server
        if (response.getStatus() != Poco::Net::HTTPResponse::HTTP_OK)
        {
            std::stringstream ss;
            // Extract response contents
            StreamCopier::copyStream(is, ss);
            // Log the result
            if (mLog) LogErr("Announce failed: %s", ss.str().c_str());
            // Failed!
            DoFailed();
        }
        else
        {
            DoSucceeded(); // Succeeded!
        }
    }
};

} // Namespace:: SqMod
