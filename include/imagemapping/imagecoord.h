#ifndef IMAGECOORD_H
#define IMAGECOORD_H

#include <opencv2/opencv.hpp>

#ifndef CV_LOAD_IMAGE_COLOR
// #ifdef IMREAD_COLOR
#define CV_LOAD_IMAGE_COLOR cv::IMREAD_COLOR

class ImageCoord
{
public:
    ImageCoord();

    /*!
     * \name ImageCoord
     * \brief Construct a new Image Coord object.
     * \param startX 
     * \param startY 
     * \param endX 
     * \param endY 
     * \param distance 
     */
    ImageCoord(const int startX, const int startY, const int endX, const int endY, const int distance);

    /*!
     * \name ImageCoord
     * \brief Copy constructor.
     * \param i 
     */
    ImageCoord(const ImageCoord& i);

    /*!
     * \name operator=
     * \brief Assignment operator.
     * \param i 
     * \return ImageCoord& 
     */
    ImageCoord& operator=(const ImageCoord& i);

    /*!
     * \name toImageCoord
     * \brief Translate a point from the map into image.
     * \param point     Point on the map.
     * \return cv::Point2f 
     */
    cv::Point2f toImageCoord(const std::complex<double> point);
    
    /*!
     * \name toImageCoord
     * \brief Translate a point from the map into image.
     * \param x     X coordinate of the point on the map
     * \param y     Y coordinate of the point on the map
     * \return cv::Point2f
     */
    cv::Point2f toImageCoord(const double x, const double y);

    /*!
     * \name toImageCoord
     * \brief Translate a point from the map into image.
     * \param point
     * \return cv::Point2f
     */
    cv::Point2f toImageCoord(const cv::Point2f point);

    /*!
     * \name toMapCoord
     * \brief Translate a point from the image into map.
     * \param point 
     * \return cv::Point2f 
     */
    cv::Point2f toMapCoord(const cv::Point2f point);

    // void toImageVector(std::complex<double> pA, std::complex<double> pB, )
private:
    cv::Point2f startPoint;
    cv::Point2f endPoint;
    int pointDistance;
};

// #endif // IMREAD_COLOR
#endif // CV_LOAD_IMAGE_COLOR
#endif // IMAGECOORD_H