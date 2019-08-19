#include "timer.h"

Timer::Timer()
{

}

Timer::Timer(const std::string name_, const int interval_, FnPtr callback) : 
    name(name_), interval(interval_), running(false), callbackFunc(callback)
{
    
}

Timer::~Timer() 
{
    this->stop();
}

using namespace std::chrono_literals;

void Timer::start()
{
    this->stop();

    {
        std::unique_lock<std::mutex> locked(this->lock);
        this->running = true;
    }

    this->timerThread = std::thread([=]()
    {
        std::unique_lock<std::mutex> locked(this->lock);

        while (this->running)
        {
            // {
            //     std::unique_lock<std::mutex> cLocked(this->callbackLock);
            //     if (this->callbackProcessingTime > this->interval && this->callbackProcessingTime < 500000)
            //     {
            //         // std::cout << "processing: " << this->callbackProcessingTime << "\t interval: "<< this->interval << std::endl;
            //         this->interval = this->callbackProcessingTime + 100; 
            //     }
            // }

            auto result = this->timerCond.wait_for(locked, this->interval*1us);


            if (result == std::cv_status::timeout)
            {
                this->callbackCond.notify_one();
            }
        }
    });

    this->callbackThread = std::thread([=]()
    {
        std::unique_lock<std::mutex> locked(this->lock);

        while (this->running)
        {
            // std::chrono::high_resolution_clock::time_point begin = std::chrono::high_resolution_clock::now();
            this->callbackCond.wait(locked);
            this->callbackFunc(NULL);
            // std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();

            // {
            //     std::unique_lock<std::mutex> cLocked(this->callbackLock);
            //     this->callbackProcessingTime = std::chrono::duration_cast<std::chrono::microseconds>(end-begin).count();
            // }
        }
    });
}

void Timer::stop()
{
    bool state = false;
    {
        std::unique_lock<std::mutex> locked(this->lock);
        if (this->running == true)
        {
            this->running = false;
            state = true;
        }
        else
        {
            state = false;
        }
    }


    if (state == true)
    {
        this->timerThread.join();
        this->callbackThread.join();
    }
}

// void Timer::timerHandler()
// {

// }

int Timer::getTimerID()                         { return this->id; }
void Timer::setInterval(const int interval_)    { this->interval = interval_; }
int Timer::getInterval()                        { return this->interval; }
std::string Timer::getName()                    { return this->name; }