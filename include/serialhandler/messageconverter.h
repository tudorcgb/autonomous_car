#ifndef MESSAGECONVERTER_H
#define MESSAGECONVERTER_H

#include <iostream>
#include <string>
#include <sstream>
#include <complex>

/*!
 * \brief The MessageConverter class, it contains functions which generate the message in the correct form.
 */
class MessageConverter
{
public:
    /*!
     * \name MessageConverter
     * \brief Constructor
     */
    MessageConverter();

    /*!
     * \name mctl
     * \brief It generates a message to control the motor speed and the steering angle.
     * \param velocity      motor PWM signal, or, if PID activated, the reference (in cm/s)
     * \param angle         steering servo angle
     * \return the formatted message
     */
    std::string mctl(double velocity, double angle);

    /*!
     * \name brake
     * \brief It generates a message to act on the brake and apply a steering angle.
     * \param angle         steering servo angle
     * \return the formatted message
     */
    std::string brake(double angle);
    //TODO spln

    /*!
     * \name pids
     * \brief It generates a message for setting the PID with the given parameters.
     * \param kp        proportional factor
     * \param ki        integral factor
     * \param kd        derivative factor
     * \param tf        filter time constant
     * \return the formatted message
     */
    std::string pids(double kp, double ki, double kd, double tf);

    /*!
     * \name pida
     * \brief It generates a message for activating PID usage.
     * \param activate      boolean value for activating PID
     * \return the formatted message
     */
    std::string pida(bool activate);

    /*!
     * \name sfbr
     * \brief It generates a message for activating the safety brake usage.
     * \param activate      boolean value for activating safety brake
     * \return the formatted message
     */
    std::string sfbr(bool activate);

    /*!
     * \name enpb
     * \brief It generates a message for activating the encoder echoer usage.
     * \param activate      boolean value for activating encoder echoer usage
     * \return the formatted message
     */
    std::string enpb(bool activate);

    /*!
     * \name dspb
     * \brief It generates a message for activating the distance sensor echoer usage.
     * \param activate      boolean value for activating distance sensor echoer
     * \return the formatted message
     */
    std::string dspb(bool activate);

    std::string spln(   std::complex<double> f_A, 
                        std::complex<double> f_B,
                        std::complex<double> f_C,
                        std::complex<double> f_D,
                        double time,
                        bool isForward
                    );
private:
    std::ostringstream messageBuffer;
};

#endif // MESSAGECONVERTER_H
