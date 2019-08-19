#ifndef MESSAGECONVERTER_H
#include "messageconverter.h"
#endif

#include <iomanip>

MessageConverter::MessageConverter()
{
    this->messageBuffer.precision(2);
}

std::string MessageConverter::mctl(double velocity, double angle)
{
    this->messageBuffer.str("");
    this->messageBuffer.clear();
    this->messageBuffer << "#MCTL:" << velocity << ";" << angle << ";;\r\n";
    return messageBuffer.str();
}

std::string MessageConverter::brake(double angle)
{
    this->messageBuffer.str("");
    this->messageBuffer.clear();
    this->messageBuffer << std::fixed << "#BRAK:" << angle << ";;\r\n";
    return messageBuffer.str();
}

std::string MessageConverter::pids(double kp, double ki, double kd, double tf)
{
    this->messageBuffer.str("");
    this->messageBuffer.clear();
    this->messageBuffer << "#PIDS:" << kp << ";" << ki << ";" << kd << ";" << tf << ";;\r\n";
    return messageBuffer.str();
}

std::string MessageConverter::pida(bool activate)
{
    this->messageBuffer.str("");
    this->messageBuffer.clear();

    int l_value = 0;
    if (activate)
    {
        l_value = 1;
    }
    this->messageBuffer << "#PIDA:" << l_value << ";;\r\n";
    return messageBuffer.str();
}

std::string MessageConverter::sfbr(bool activate)
{
    this->messageBuffer.str("");
    this->messageBuffer.clear();

    int l_value = 0;
    if (activate)
    {
        l_value = 1;
    }
    this->messageBuffer << "#SFBR:" << l_value << ";;\r\n";
    return messageBuffer.str();
}

std::string MessageConverter::enpb(bool activate)
{
    this->messageBuffer.str("");
    this->messageBuffer.clear();

    int l_value = 0;
    if (activate)
    {
        l_value = 1;
    }
    this->messageBuffer << "#ENPB:" << l_value << ";;\r\n";
    return messageBuffer.str();
}

std::string MessageConverter::dspb(bool activate)
{
    this->messageBuffer.str("");
    this->messageBuffer.clear();

    int l_value = 0;
    if (activate)
    {
        l_value = 1;
    }
    this->messageBuffer << "#DSPB:" << l_value << ";;\r\n";
    return messageBuffer.str();
}

std::string MessageConverter::spln(   
                        std::complex<double> f_A, 
                        std::complex<double> f_B,
                        std::complex<double> f_C,
                        std::complex<double> f_D,
                        double time,
                        bool isForward
                    )
{
    this->messageBuffer.str("");
    this->messageBuffer.clear();
    int l_value = 0;
    if (isForward)
    {
        l_value = 1;
    }
    this->messageBuffer << "#SPLN:" 
                        << l_value << ";" 
                        << f_A.real() 
                            << ";" 
                            << f_A.imag() 
                            << ";"
                        << f_B.real()
                            << ";"
                            << f_B.imag()
                            << ";"
                        << f_C.real()
                            << ";"
                            << f_C.imag()
                            << ";"
                        << f_D.real()
                            << ";"
                            << f_D.imag()
                            << ";"
                        << time << ";;\r\n";
    return messageBuffer.str();
}
