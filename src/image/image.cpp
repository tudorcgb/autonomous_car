#ifndef IMAGE_H
#include "image.h"
#endif // IMAGE_H

// #include <QtCore>
#include <exception>

Image::Image(int width, int height, double exposure, int mode) :
    width_(width), height_(height), exposure_(exposure), mode_(mode),
    cap(cv::VideoCapture(0)), frameNo_(0), updateTime(std::time(nullptr))
{
    this->cap.set(cv::CAP_PROP_FRAME_WIDTH, width);
    this->cap.set(cv::CAP_PROP_FRAME_HEIGHT, height);
    this->cap.set(cv::CAP_PROP_EXPOSURE, exposure);
    this->className_ = "Image";
}

Image::Image(std::string path_, int width, int height, int mode) :
    width_(width), height_(height), mode_(mode), path(path_),
    cap(cv::VideoCapture(path_)), frameNo_(0), updateTime(std::time(nullptr))
{
    std::cout << path_ << std::endl;
    this->cap.set(cv::CAP_PROP_FRAME_WIDTH, width);
    this->cap.set(cv::CAP_PROP_FRAME_HEIGHT, height);
    this->className_ = "Image";
    // cv::namedWindow("image");
    // cv::startWindowThread();
}

Image::Image(const Image& img)
{
    this->cap = img.cap;
    this->updateTime = img.updateTime;
    this->width_ = img.width_;
    this->height_ = img.height_;
    this->exposure_ = img.exposure_;
    this->mode_ = img.mode_;
    this->frameNo_ = img.frameNo_;
    this->className_ = img.className();
}

Image& Image::operator=(const Image& img)
{
    this->cap = img.cap;
    this->updateTime = img.updateTime;
    this->width_ = img.width_;
    this->height_ = img.height_;
    this->exposure_ = img.exposure_;
    this->mode_ = img.mode_;
    this->frameNo_ = img.frameNo_;
    this->className_ = img.className();
    return *this;
}

void Image::setSize(int width, int height)
{
    this->width_ = width;
    this->height_ = height;
    this->cap.set(cv::CAP_PROP_FRAME_WIDTH, this->width_);
    this->cap.set(cv::CAP_PROP_FRAME_HEIGHT, this->height_);
}

void Image::setExposure(double exposure)
{
    this->exposure_ = exposure;
    this->cap.set(cv::CAP_PROP_EXPOSURE, this->exposure_);
}

void Image::setColorMode(int mode)
{
    this->mode_ = mode;
}

void Image::read(cv::Mat& frame)
{
    if (this->cap.isOpened())   { this->cap.read(frame); }
    else                        { throw std::system_error(-1, std::generic_category(), "VideoCapture not opened."); }
}

void* Image::nextFrame(void*)
{
    if (this->cap.isOpened())   
    { 
        this->cap.read(this->frame_); 
        // cv::imshow("image", this->frame_); 
        // cv::waitKey();
        // QThread::msleep(40);
        if (this->frame_.rows == 480 && this->frame_.cols == 640)
        {
            this->notifyObservers();
        }
    }
    else                        { throw std::system_error(-1, std::generic_category(), "VideoCapture not opened."); }
}

cv::Mat Image::getROI(const int& x, const int& y, const int& roiWidth, const int& roiHeight)
{
    if ((x >= 0 && x <= this->width_) &&
        (y >= 0 && y <= this->height_) &&
        (roiWidth > 0 && roiHeight > 0) &&
        (x + roiWidth <= this->width_) &&
        (y + roiHeight <= this->height_))
    {
        cv::Rect roi(x, y, roiWidth, roiHeight);
        return this->frame_(roi);
    }
    else
    {
        std::cout << "x: " << x << " y: " << y << " roiWidth: " << roiWidth << " width: " << width_ << 
        " roiHeight: " << roiHeight <<  " height: " << height_ << " x + roiWidth: " << x + roiWidth << " y + roiHeight: " << y + roiHeight << std::endl;
        // throw std::system_error(-1, std::generic_category(), "ROI out of bound.");
    }
    
}

void Image::close()
{
    this->cap.release();
}

int Image::width()          { return this->width_; }
int Image::height()         { return this->height_; }
double Image::exposure()    { return this->exposure_; }
int Image::mode()           { return this->mode_; }
int Image::frameNo()        { return this->frameNo_; }
cv::Mat& Image::frame()     { return this->frame_; }
std::string Image::getPath(){ return this->path; }