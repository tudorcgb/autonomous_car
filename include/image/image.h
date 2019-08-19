#ifndef IMAGE_H
#define IMAGE_H

#ifndef OBSERVER_H
#include "observer.h"
#endif // OBSERVER_H

#include <opencv2/opencv.hpp>
#include <ctime>

#ifndef CV_LOAD_IMAGE_COLOR
// #ifdef IMREAD_COLOR
#define CV_LOAD_IMAGE_COLOR cv::IMREAD_COLOR

class Image : public Subject
{
public:
    /*!
     * \name Image
     * \brief Construct a new Image object
     * \param width     default: 640
     * \param height    default: 480
     * \param exposure  default: 0.01
     * \param mode      default: CV_LOAD_IMAGE_COLOR
     */
    Image(int width=640, int height=480, double exposure=3.0/100.0, int mode=CV_LOAD_IMAGE_COLOR);

    /*!
     * \name Image
     * \brief Construct a new Image object
     * \param path      path to the video file
     * \param width     default: 640
     * \param height    default: 480
     * \param mode      default: CV_LOAD_IMAGE_COLOR
     */
    Image(std::string path, int width=640, int height=480, int mode=CV_LOAD_IMAGE_COLOR);

    /*!
     * \name Image
     * \brief Copy constructor.
     * \param img 
     */
    Image(const Image& img);

    /*!
     * \name operator=
     * \brief Assignment operator
     * \param img 
     * \return Image& 
     */
    Image& operator=(const Image& img);

    /*!
     * \name setSize
     * \brief Set the Size object
     * \param width 
     * \param height 
     */
    void setSize(int width, int height);

    /*!
     * \name setExposure
     * \brief Set the Exposure object
     * \param exposure 
     */
    void setExposure(double exposure);

    /*!
     * \name setColorMode
     * \brief Set the Color Mode object
     * \param mode 
     */
    void setColorMode(int mode);

    /*!
     * \name width
     * \brief Getter for width_
     * \return int 
     */
    int width();

    /*!
     * \name height
     * \brief Getter foe height_
     * \return int 
     */
    int height();

    /*!
     * \exposure
     * \brief Getter for exposure_
     * \return double 
     */
    double exposure();

    /*!
     * \name mode
     * \brief Getter for mode
     * \return int 
     */
    int mode();

    /*!
     * name frameNo
     * \brief Getter for frameNo_
     * \return int 
     */
    int frameNo();

    /*!
     * \name frame
     * \brief Getter for frame_
     * \return cv::Mat& 
     */
    cv::Mat& frame();

    /*!
     * \name read
     * \brief Reads next image
     * \param cv::Mat& 
     * \throws
     */
    void read(cv::Mat& frame);

    /*!
     * \name getROI
     * \brief Returns current frame cropped to the specified Region of interest.
     * \param x 
     * \param y 
     * \param roiWidth 
     * \param roiHeight 
     * \return cv::Mat& 
     */
    cv::Mat getROI(const int& x, const int& y, const int& roiWidth, const int& roiHeight);

    /*!
     * \name nextFrame
     * \brief Get next frame
     * \throws
     */
    void* nextFrame(void*);

    std::string getPath();

    /*!
     * \name close
     * \brief Close VideoCapture
     */
    void close();

private:
    cv::VideoCapture cap;
    std::time_t updateTime;
    int width_;
    int height_;
    double exposure_;
    int mode_;
    int frameNo_;
    cv::Mat frame_;
    std::string path;
};

// #endif // IMREAD_COLOR
#endif // CV_LOAD_IMAGE_COLOR
#endif // IMAGE_H