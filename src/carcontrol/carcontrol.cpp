#ifndef CARCONTROL_H
#include "carcontrol.h"
#include "connexc.h"
#endif // CARCONTROL_H

CarControl::CarControl() : serialHandler()
{
    this->flags = DISABLE_ALL;
    this->timeout = DEFAULT_TIMEOUT;
    // sleep(1); // TODO: verify why sleep
}

void CarControl::enable(const uint8_t flag) throw()
{
    bool sent = false;
    bool enableEncoderPublisher         = (flag & ENCODER_PUBLISHER) ? true : false;
    bool enablePID                      = (flag & PID_ACTIVATION) ? true : false; 
    bool enableSafetyStop               = (flag & DISTANCE_SENSORS_PUBLISHER) ? true : false;
    bool enableDistanceSensorsPubliser  = (flag & SAFETTY_STOP) ? true : false;

    {
        // Encoder Publisher
        {
            this->serialHandler.addWaiter("ENPB", std::bind(&CarControl::ackFunction, this, (void*)(NULL)));
            sent = this->serialHandler.sendEncoderActivation(enableEncoderPublisher);
            if (sent)
            {
                if(this->serialHandler.waitWaiter("ENPB", this->timeout))
                {
                    std::cout << "Encoder publisher " << enableEncoderPublisher << std::endl;
                }
                else
                {
                    // throw ConnectionException("Response was not received!", -1, 0);
                    std::cout << "enpb exc" << std::endl;
                }
            }
            else
            {
                std::cout << "Sending problem" << std::endl;
            }
            this->serialHandler.deleteWaiter("ENPB", std::bind(&CarControl::ackFunction, this, (void*)(NULL)));
        }

        // PID
        {
            this->serialHandler.addWaiter("PIDA", std::bind(&CarControl::ackFunction, this, (void*)(NULL)));
            sent = this->serialHandler.sendPidActivation(enablePID);
            if (sent)
            {
                if (this->serialHandler.waitWaiter("PIDA", this->timeout))
                {
                    std::cout << "PID " << enablePID << std::endl;
                }
                else
                {
                    // throw ConnectionException("Response was not received!", -1, 0);
                    std::cout << "pida exc" << std::endl;
                }
            }
            else
            {
                std::cout << "Sending problem" << std::endl;
            }
            this->serialHandler.deleteWaiter("PIDA", std::bind(&CarControl::ackFunction, this, (void*)(NULL)));
        }

        // Safety Stop
        {
            this->serialHandler.addWaiter("SFBR", std::bind(&CarControl::ackFunction, this, (void*)(NULL)));
            sent = this->serialHandler.sendSafetyStopActivation(enableSafetyStop);
            if (sent)
            {
                if (this->serialHandler.waitWaiter("SFBR", this->timeout))
                {
                    std::cout << "Safety stop " << enableSafetyStop << std::endl;
                }
                else
                {
                    // throw ConnectionException("Response was not received!", -1, 0);=
                    std::cout << "sfrb exc" << std::endl;
                }
            }
            else
            {
                std::cout << "Sending problem" << std::endl;
            }
            this->serialHandler.deleteWaiter("SFBR", std::bind(&CarControl::ackFunction, this, (void*)(NULL)));
        }

        // Distance Sensors Publisher
        {
            this->serialHandler.addWaiter("DSPB", std::bind(&CarControl::ackFunction, this, (void*)(NULL)));
            sent = this->serialHandler.sendDistanceSensorPublisherActivation(enableDistanceSensorsPubliser);
            if (sent)
            {
                if (this->serialHandler.waitWaiter("DSPB", this->timeout))
                {
                    std::cout << "Distance Sensors publisher " << enableDistanceSensorsPubliser << std::endl;
                }
                else
                {
                    // throw ConnectionException("Response was not received!", -1, 0);
                    std::cout << "dspb exc" << std::endl;
                }
            }
            else
            {
                std::cout << "Sending problem" << std::endl;
            }
            this->serialHandler.deleteWaiter("DSPB", std::bind(&CarControl::ackFunction, this, (void*)(NULL)));
        }
    }
    this->flags = flag;
}

void CarControl::enableEncoder() throw()
{
    bool sent = false;
    this->serialHandler.addWaiter("ENPB", std::bind(&CarControl::ackFunction, this, (void*)(NULL)));
    sent = this->serialHandler.sendEncoderActivation(true);
    if (sent)
    {
        if(this->serialHandler.waitWaiter("ENPB", this->timeout))
        {
            std::cout << "Encoder publisher activated " << std::endl;
        }
        else
        {
            // throw ConnectionException("Response was not received!", -1, 0);
            std::cout << "enbp exc" << std::endl;
        }
    }
    else
    {
        std::cout << "Sending problem" << std::endl;
    }
    this->serialHandler.deleteWaiter("ENPB", std::bind(&CarControl::ackFunction, this, (void*)(NULL)));
    this->flags = this->flags | ENCODER_PUBLISHER;
}

void CarControl::enableEncoder(FnPtr callbackFunc) throw()
{
    bool sent = false;
    this->serialHandler.addWaiter("ENPB", std::bind(&CarControl::ackFunction, this, (void*)(NULL)));
    sent = this->serialHandler.sendEncoderActivation(true);
    if (sent)
    {
        if(this->serialHandler.waitWaiter("ENPB", this->timeout))
        {
            std::cout << "Encoder publisher activated " << std::endl;
        }
        else
        {
            // throw ConnectionException("Response was not received!", -1, 0);
            std::cout << "enpb exc" << std::endl;
        }
    }
    else
    {
        std::cout << "Sending problem" << std::endl;
    }
    this->serialHandler.deleteWaiter("ENPB", std::bind(&CarControl::ackFunction, this, (void*)(NULL)));
    this->serialHandler.addWaiter("ENPB", callbackFunc);
    this->flags = this->flags | ENCODER_PUBLISHER;
}

void CarControl::disableEncoder() throw()
{
    bool sent = false;
    this->serialHandler.addWaiter("ENPB", std::bind(&CarControl::ackFunction, this, (void*)(NULL)));
    sent = this->serialHandler.sendEncoderActivation(false);
    if (sent)
    {
        if(this->serialHandler.waitWaiter("ENPB", this->timeout))
        {
            std::cout << "Encoder publisher activated " << std::endl;
        }
        else
        {
            // throw ConnectionException("Response was not received!", -1, 0);
            std::cout << "enpb exc" << std::endl;
        }
    }
    else
    {
        std::cout << "Sending problem" << std::endl;
    }
    this->serialHandler.deleteWaiter("ENPB", std::bind(&CarControl::ackFunction, this, (void*)(NULL)));
    this->flags = this->flags & (~ENCODER_PUBLISHER);
}

void CarControl::enablePID() throw()
{
    bool sent = false;

    this->serialHandler.addWaiter("PIDA", std::bind(&CarControl::ackFunction, this, (void*)(NULL)));
    sent = this->serialHandler.sendPidActivation(true);
    if (sent)
    {
        if (this->serialHandler.waitWaiter("PIDA", this->timeout))
        {
            std::cout << "PID activated" << std::endl;
        }
        else
        {
            // throw ConnectionException("Response was not received!", -1, 0);
            std::cout << "pida exc" << std::endl;
        }
    }
    else
    {
        std::cout << "Sending problem" << std::endl;
    }
    this->serialHandler.deleteWaiter("PIDA", std::bind(&CarControl::ackFunction, this, (void*)(NULL)));
    this->flags = this->flags | PID_ACTIVATION;
}

void CarControl::disablePID() throw()
{
    bool sent = false;

    this->serialHandler.addWaiter("PIDA", std::bind(&CarControl::ackFunction, this, (void*)(NULL)));
    sent = this->serialHandler.sendPidActivation(false);
    if (sent)
    {
        if (this->serialHandler.waitWaiter("PIDA", this->timeout))
        {
            std::cout << "PID activated" << std::endl;
        }
        else
        {
            // throw ConnectionException("Response was not received!", -1, 0);
            std::cout << "pida exc" << std::endl;
        }
    }
    else
    {
        std::cout << "Sending problem" << std::endl;
    }
    this->serialHandler.deleteWaiter("PIDA", std::bind(&CarControl::ackFunction, this, (void*)(NULL)));
    this->flags = this->flags & (~PID_ACTIVATION);
}

void CarControl::move(const double speed, const double angle) throw()
{
    bool sent = false;

    this->serialHandler.addWaiter("MCTL", std::bind(&CarControl::ackFunction, this, (void*)(NULL)));
    sent = this->serialHandler.sendMove(speed, angle);
    if (sent)
    {
        if (this->serialHandler.waitWaiter("MCTL", this->timeout))
        {
            // TODO: Logger
            // std::cout << "MCTL received" << std::endl;
        }
        else
        {
            // throw ConnectionException("Response was not received!", -1, 0);
            std::cout << "mctl exc" << std::endl;
        }
    }
    else
    {
        std::cout << "Sending problem" << std::endl;
    }
    this->serialHandler.deleteWaiter("MCTL", std::bind(&CarControl::ackFunction, this, (void*)(NULL)));
}

void CarControl::brake(double angle) throw()
{
    bool sent = false;

    this->serialHandler.addWaiter("BRAK", std::bind(&CarControl::ackFunction, this, (void*)(NULL)));
    sent = this->serialHandler.sendBrake(angle);
    if (sent)
    {
        if (this->serialHandler.waitWaiter("BRAK", this->timeout))
        {
            std::cout << "BRAK received" << std::endl;
        }
        else
        {
            // throw ConnectionException("Response was not received!", -1, 0);
            std::cout << "brak exc" << std::endl;
        }
    }
    else
    {
        std::cout << "Sending problem" << std::endl;
    }
    this->serialHandler.deleteWaiter("BRAK", std::bind(&CarControl::ackFunction, this, (void*)(NULL)));
}

void CarControl::moveBezier(
                    const std::complex<double> A,
                    const std::complex<double> B,
                    const std::complex<double> C,
                    const std::complex<double> D,
                    double time,
                    bool isForward
)
{
    bool sent = false;

    this->serialHandler.addWaiter("SPLN", std::bind(&CarControl::ackFunction, this, (void*)(NULL)));
    sent = this->serialHandler.sendBezierCurve(A, B, C, D, time, true);
    if (sent)
    {
        if (this->serialHandler.waitWaiter("SPLN", this->timeout))
        {
            std::cout << "SPLN received" << std::endl;
        }
        else
        {
            // throw ConnectionException("Response was not received!", -1, 0);
            std::cout << " exc" << std::endl;
        }
    }
    else
    {
        std::cout << "sending problem" << std::endl;
    }
    this->serialHandler.deleteWaiter("SPLN", std::bind(&CarControl::ackFunction, this, (void*)(NULL)));
}

void CarControl::close()
{
    this->serialHandler.close();
}

void* CarControl::ackFunction(void*) {}

void CarControl::setTimeout(double timeout_)    { this->timeout = timeout_; }
double CarControl::getTimeout()                 { return this->timeout; }