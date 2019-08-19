#ifndef SERIALHANDLER_H
#include "serialhandler.h"
#endif

#include <termios.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string>
#include <cstdlib>

SerialHandler::SerialHandler(): serialPort((FindFile("/dev/serial/by-id/").findFirstBySubstr("STM")), STM_BAUDRATE, serial::Timeout::simpleTimeout(1000))
{
    this->running = false;
    this->th = std::thread(&SerialHandler::readThread, this);
    this->running = true;
}

SerialHandler::SerialHandler(const char* device, const char* historyFileName) : serialPort(device, STM_BAUDRATE, serial::Timeout::simpleTimeout(1000))
{
    this->running = false;
    this->th = std::thread(&SerialHandler::readThread, this);
    this->running = true;
}

bool SerialHandler::send(const char* message)
{
    this->lock.lock();
    size_t written = this->serialPort.write(std::string(message));
    this->lock.unlock();
    if (written != std::string(message).size())
    {
        return false;
    }
    return true;
}

bool SerialHandler::send(const std::string message)
{
    this->lock.lock();
    size_t written = this->serialPort.write(message);
    this->lock.unlock();
    if (written != message.size())
    {
        return false;
    }
    return true;
}

bool SerialHandler::sendMove(double velocity, double angle)
{
    std::string message = this->messageConverter.mctl(velocity, angle);
    if (message.empty())
    {
        return false;
    }
    return this->send(message);
}

bool SerialHandler::sendBrake(double angle)
{
    std::string message = this->messageConverter.brake(angle);
    if (message.empty())
    {
        return false;
    }
    // std::cout << message.c_str();
    return this->send(message);
}

//TODO sendBezierCurve
bool SerialHandler::sendPidActivation(bool activate)
{
    std::string message = this->messageConverter.pida(activate);
    if (message.empty())
    {
        return false;
    }
    return this->send(message);
}

bool SerialHandler::sendPidValue(double kp, double ki, double kd, double tf)
{
    std::string message = this->messageConverter.pids(kp, ki, kd, tf);
    if (message.empty())
    {
        return false;
    }
    return this->send(message);
}

bool SerialHandler::sendSafetyStopActivation(bool activate)
{
    std::string message = this->messageConverter.sfbr(activate);
    if (message.empty())
    {
        return false;
    }
    this->send(message);
}

bool SerialHandler::sendDistanceSensorPublisherActivation(bool activate)
{
    std::string message = this->messageConverter.dspb(activate);
    if (message.empty())
    {
        return false;
    }
    return this->send(message);
}

bool SerialHandler::sendEncoderActivation(bool activate)
{
    std::string message = this->messageConverter.enpb(activate);
    if (message.empty())
    {
        return false;
    }
    return this->send(message);
}

bool SerialHandler::sendBezierCurve(   std::complex<double> f_A, 
                            std::complex<double> f_B,
                            std::complex<double> f_C,
                            std::complex<double> f_D,
                            double time,
                            bool isForward
                        )
{
    std::string message = this->messageConverter.spln(f_A, f_B, f_C, f_D, time, isForward);
    if (message.empty())
    {
        return false;
    }
    return this->send(message);
}

void SerialHandler::close()
{
    this->serialPort.close();
    // this->historyFile.close();
}

void SerialHandler::readThread()
{
    while(this->running)
    {
        // std::cout << "try to read" << std::endl;
        if (this->serialPort.waitReadable())
        {
            this->buff = this->serialPort.readline();
            // std::cout << "Response: " << this->buff << std::endl;
            this->checkWaiter(this->buff);
        }
    }
}

void SerialHandler::addWaiter(const std::string key, FnPtr callbackFunction)
{
    this->waiters[key].push_back(callbackFunction);
    this->acks[key] = false;
}

void SerialHandler::checkWaiter(const std::string response){
    std::string key = response.substr(1, 4);
    std::map<std::string, CallbackEvent >::iterator it;
    it = this->waiters.find(key);
    if (it != this->waiters.end())
    {
        if (response.find("ack")!= std::string::npos)
        {
            // std::cout << response << std::endl;
            std::lock_guard<std::mutex> lk(this->mutexes[key]);
            this->acks[key] = true;
        }
        this->condVars[key].notify_one();
        CallbackEvent l_waiters = this->waiters[key];
        for(FnPtr func : l_waiters)
        {
            func((void*)response.substr(6, response.length()-6-4).c_str());
        }
        // std::cout << "resp: " << response.substr(6, response.length()-6-4) << std::endl;
        // std::cout << "Waiter checked" << std::endl;
    }
}

void SerialHandler::deleteWaiter(const std::string key, FnPtr callbackFunction)
{
    std::map<std::string, CallbackEvent >::iterator it;
    this->acks.erase(key);
    this->condVars.erase(key);
    this->mutexes.erase(key);
    it = this->waiters.find(key);
    if (it != this->waiters.end())
    {
        for (int i = 0; i < it->second.size(); i++)
        {
            if (&it->second[i] == &callbackFunction)
            {
                it->second.erase(it->second.begin() + i);
                if (it->second.size() == 0)
                {
                    this->waiters.erase(key);
                }
                return;
            }
        }
    }
}

using namespace std::chrono_literals;

bool SerialHandler::waitWaiter(const std::string key, const double timeout)
{
    std::unique_lock<std::mutex> lk(this->mutexes[key]);
    if(this->condVars[key].wait_for(lk, timeout*1000ms, [this, key]{return this->acks[key] == true;}))
    {
        return true;
    }
    else
    {
        return false;
    }
}

void SerialHandler::clearWaiter(const std::string key)
{
    this->acks[key] = false;
}

void SerialHandler::stopThread()
{
    this->running = false;
    this->th.join();
    this->serialPort.close();
    this->historyFile.close();
}
