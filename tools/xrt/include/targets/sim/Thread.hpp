#pragma once
#include <iostream>
#include <string>
#include <thread>
#include <map>
#include <utility>
#include <vector>
#include <unistd.h>
#include <algorithm>

/**
 * A base class for multi-threading implementation that application threads
 * must extend.
 */
class Thread {
    /**
     *  A map from thread ID to the thread name.
     */
    static std::map<std::thread::id, std::string> sThreadIdMap;

    /**
     * A list of active threads.
     */
    static std::vector<Thread *> sThreadList;

    /**
     * The thread that does the work.
     */
    std::thread *mWorkerThread;

    /**
     * The id of the thread.
     */
    const std::string mThreadId;

    /**
     * Flag signaling if the thread has been interrupted;
     */
    bool mInterrupted;

    /**
     * Helper method for task execution.
     */
    void execute();

protected:
/**
*  Is true if the thread has finished executing.
*/
    bool mDone = true;
public:

    /**
     * Identifies the current running thread with the specified thread name.
     *
     * @param threadName The name of the running thread.
     */
    static void identify(const std::string &threadName);

    /**
     * Returns the name of the current running thread.
     *
     * @return the name of the current running thread.
     */
    static const std::string &getCurrentThreadName();

    /**
     * Pauses the current thread for the specified number of milliseconds.
     * @param milliseconds The number of milliseconds to sleep.
     */
    static void sleepm(uint32_t milliseconds);

    /**
     * Interrupts all running threads.
     */
    static void interruptAll();

    static void joinAll();

    /**
     * Thread constructor.
     *
     * @param threadId The string id of the new thread.
     */
    explicit Thread(std::string threadId);

    /**
     * Starts the thread.
     */
    void start();

    /**
     * The method executed in the new thread.
     */
    virtual void run() = 0;

    /**
     * Returns the kernel ID of the thread.
     */
    std::thread::id getId() const;

    /**
     * Returns the string ID of the thread.
     */
    std::string getStringId() const;

    /**
     * Waits for this thread to finish.
     */
    void join();

    /**
     * Returns true if the thread has finished executing.
     */
    bool isDone() const;

    /**
     * Returns true if the thread has been interrupted.
     * @return true if the thread has been interrupted.
     */
    bool isInterrupted() const;

    /**
     * Interrupts the thread, setting the flag to true.
     */
    virtual void interrupt();

    /**
     * Thread destructor.
     */
    virtual ~Thread();

    static void listAll(bool justactive = false);
};