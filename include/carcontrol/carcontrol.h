#ifndef CARCONTROL_H
#define CARCONTROL_H

#include <unistd.h>

#ifndef SERIALHANDELR_H
#include "serialhandler.h"
#endif // SERIALHANDELR_H

#define DEFAULT_TIMEOUT 3.0

enum active_functions
{
    DISABLE_ALL                 = 0,
    ENCODER_PUBLISHER           = 1,
    PID_ACTIVATION              = 2,
    DISTANCE_SENSORS_PUBLISHER  = 4,
    SAFETTY_STOP                = 8
};

class CarControl
{
public:
    /*!
     * \name CarControl
     * \brief Construct a new Car Control object
     */
    CarControl();

    /*!
     * \name enableEncoder
     * \brief Enable Encoder publisher
     * \throw CommunicationException
     */
    
    void enableEncoder() throw();
    /*!
     * \name enableEncoder
     * \brief   Enable Encoder and call the sepcified function every time a new value for encoder is received.
     *          Node: The callback function of type FnPtr can receive any insput of type (void*). Usually it
     *          it receive a string of numbers or the string 'ack' but you allways need to validate the input
     *          parameter.
     * \param callbackFunc function that will be called every time a new value is read from the encoder
     * \throw CommunicationException
     */
    void enableEncoder(FnPtr callbackFunc) throw();

    /*!
     * \name disableEncoder
     * \brief Disable Encoder publisher.
     * \throw CommunicationException
     */
    void disableEncoder() throw();

    /*!
     * \name enablePID
     * \brief Enable PID
     * \throw CommunicationException
     */
    void enablePID() throw();

    /*!
     * \name disablePID
     * \brief Disable PID
     * \throw CommunicationException
     */
    void disablePID() throw();

    /*!
     * \name PIDValue
     * \brief // TODO : Not implemented
     * \throw CommunicationException
     */
    void PIDValue() throw();

    /*!
     * \name move
     * \brief Send move command
     * \param speed 
     * \param angle 
     * \throw CommunicationException
     */
    void move(const double speed, const double angle) throw();

    /*!
     * \name brake
     * \brief Send brake command
     * \param angle 
     * \throw CommunicationException
     */
    void brake(double angle) throw();

    /*!
     * \name moveDistance
     * \brief // TODO: Not implemented
     * \param distance 
     * \param angle 
     * \throw CommunicationException
     */
    void moveDistance(const double distance, const double angle) throw();

    /*!
     * \name moveBezier
     * \brief Send moveBezier command and the car will follow the path described by the spline given the 4 points
     * \param A           first coordinate on the curve
     * \param B           second coordinate on the curve
     * \param C           third coordinate on the curve
     * \param D           forth coordinate on the curve
     * \param tim         movemet duration in seconds
     * \param isForward   forward/backward movement
     * \param isForward 
     */
    void moveBezier(const std::complex<double> A,
                    const std::complex<double> B,
                    const std::complex<double> C,
                    const std::complex<double> D,
                    double time,
                    bool isForward);

    /*!
     * \enable
     * \brief   Enables and disables different features specified by flag argument: 
     *          DISABLE_ALL, PID_ACTIVATION, ENCODER_PUBLISHER, SAFETTY_STOP, DISTANCE_SENSORS_PUBLISHER
     *          and combination with logic "or"
     * \param flag 
     * \throw CommunicationException
     */
    void enable(const uint8_t flag) throw();

    /*!
     * \name getSpeed
     * \brief // TODO: Not implemented
     * \return double 
     */
    double getSpeed();

    /*!
     * \name getYaw
     * \brief // TODO: Not implemented
     * \return double 
     */
    double getYaw();

    /*!
     * \name close
     * \brief Close serial connection anf stop threads
     */
    void close();

    /*!
     * \brief Set the Timeout
     * \param timeout_ 
     */
    void setTimeout(double timeout_);

    /*!
     * \brief Get the Timeout
     * \return double 
     */
    double getTimeout();

    void* ackFunction(void*);
private:
    SerialHandler serialHandler;
    uint8_t flags;
    double timeout;
};

#endif // CARCONTROL_H