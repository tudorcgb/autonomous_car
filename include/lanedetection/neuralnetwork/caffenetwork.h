#ifndef CAFFENETWORK_H
#define CAFFENETWORK_H

#include <caffe/caffe.hpp>

#include "ineuralnetwork.h"
#include "observer.h"

/*!
 * \name CaffeNetwork
 * \brief Neural network developed in caffe
 * \implements IObserver and ConvNeuralNetwork
 */
class CaffeNetwork : public IObserver, public ConvNeuralNetwork
{
public:

    /*!
     * \name CaffeNetwork
     * \brief Construct a new Caffe Network object
     * \param model_file    model file that contain the structure of the network
     * \param weights_file  file that contain trained weights
     */
    CaffeNetwork(const std::string& model_file, const std::string& weights_file);

    /*!
     * \name CaffeNetwork
     * \brief Copy constructor.
     * \param caffe_ 
     */
    CaffeNetwork(const CaffeNetwork& caffe_);
    
    /*!
     * \name ~CaffeNetwork
     * \brief Destroy the Caffe Network object
     */
    ~CaffeNetwork();

    /*!
     * \name WrapInputLayer
     * \brief Wraps the input image in more cv::Mat objects representing the colours channels.
     * \param input_channels 
     */
    void WrapInputLayer(std::vector<cv::Mat>* input_channels);

    /*!
     * \name Preprocess
     * \brief   Converts image to correct number of chennles, and the data is normalized in [0,1] interval.
     *          Also, the image is converted to float32 and splited in channels
     * \param img 
     * \param input_channels 
     */
    void Preprocess(const cv::Mat& img, std::vector<cv::Mat>* input_channels);

    /*!
     * \name infer
     * \brief Runs one forward step and return the output of the network
     * \return double 
     */
    double infer() override;

    /*!
     * \name update
     * \brief This method updates image object of the class CaffeNetwork
     * \param subject 
     */
    void update(Subject* subject) override;
private:
    std::shared_ptr<caffe::Net<float> > net;
    int num_channels;
    cv::Size input_geometry;
};

#endif // CAFFENETWORK_H