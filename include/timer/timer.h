#ifndef TIMER_H
#define TIMER_H

#include <iostream>
#include <unistd.h>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <chrono>

#ifndef FN_PTR
// typedef void* (*FnPtr)(void*);
typedef std::function<void(void*)> FnPtr;
#endif // FNPTR

class Timer
{
public:
    /*!
     * \name Timer
     * \brief Construct a new Timer object
     */
    Timer();

    /*!
     * \name Timer
     * \brief Construct a new Timer object
     * \param name_         name of the timer
     * \param interval_     the interval for between consecutive calling of callback function
     * \param callback      callback function. To be used: std::bind(&Class::method, instance, (*void)(param))
     */
    Timer(const std::string name_, const int interval_, FnPtr callback);

    /*!
     * \Timer
     * \brief Destroy the Timer object
     * 
     */
    ~Timer();

    /*!
     * \name start
     * \brief Starts the timer.
     */
    void start();

    /*!
     * \name stop
     * \brief Stops the timer.
     */
    void stop();

    /*!
     * \name getTimerID
     * \brief Get the Timer ID
     * \return int 
     */
    int getTimerID();

    /*!
     * \name setInterval
     * \brief Set the Interval
     * \param interval 
     */
    void setInterval(const int interval);

    /*!
     * \name getInterval
     * \brief Get the Interval
     * \return int 
     */
    int getInterval();

    /*!
     * \name getName
     * \brief Get the Name
     * \return std::string 
     */
    std::string getName();

private:
    // void timerHandler();
    int id;
    int interval;
    std::string name;
    bool running;
    std::mutex lock;

    std::thread timerThread;
    std::condition_variable timerCond;

    std::thread callbackThread;
    std::condition_variable callbackCond;
    std::mutex callbackLock;
    FnPtr callbackFunc;
    int callbackProcessingTime;
};

#endif // TIMER_H