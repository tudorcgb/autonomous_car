#ifndef INEURALNETWORK_H
#define INEURALNETWORK_H

#include <opencv2/opencv.hpp>

/*!
 * \name ConvNeuralNetwork
 * \brief Interface for neural neteork
 */
class ConvNeuralNetwork
{
public:
    /*!
     * \name infer
     * \brief Runs one step inferation on image
     * \return double 
     */
    virtual double infer() = 0;

    cv::Mat image;
private:
};

#endif // INEURALNETWORK_H