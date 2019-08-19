#include "caffenetwork.h"
#include "image.h"

#include <chrono>

// CaffeNetwork::CaffeNetwork() 
// {
//     cv::startWindowThread();
// }



CaffeNetwork::CaffeNetwork(const std::string& model_file, const std::string& weights_file)
{
    caffe::Caffe::set_mode(caffe::Caffe::CPU);

    this->net.reset(new caffe::Net<float>(model_file, caffe::TEST));
    this->net->CopyTrainedLayersFrom(weights_file);

    CHECK_EQ(this->net->num_inputs(), 1) << "Network should have exactly one input.";
    CHECK_EQ(this->net->num_outputs(), 1) << "Network should have exactly one output.";

    caffe::Blob<float>* input_layer = net->input_blobs()[0];
    this->num_channels = input_layer->channels();
    CHECK(this->num_channels == 3) << "Input layer should have 3 channels";
    this->input_geometry = cv::Size(input_layer->width(), input_layer->height());

    caffe::Blob<float>* output_layer = this->net->output_blobs()[0];
}

CaffeNetwork::~CaffeNetwork()
{

}

CaffeNetwork::CaffeNetwork(const CaffeNetwork& caffe_)
{

}

double CaffeNetwork::infer()
{
    // std::chrono::high_resolution_clock::time_point begin = std::chrono::high_resolution_clock::now();
    caffe::Blob<float>* input_layer = net->input_blobs()[0];
    input_layer->Reshape(1, num_channels, input_geometry.height, input_geometry.width);
    net->Reshape();

    std::vector<cv::Mat> input_channels;
    WrapInputLayer(&input_channels);

    Preprocess(this->image, &input_channels);

    // caffe::BlobProto blob_proto;
    // blob_proto.set_channels(3);
    // blob_proto.set_height(this->image.rows);
    // blob_proto.set_width(this->image.cols);
    // blob_proto.clear_data();
    // cv::Mat m(this->image.rows, this->image.cols, CV_32FC3);
    // this->image.convertTo(m, CV_32FC3, 1.0/255.0);
    // std::cout << "size: " << this->image.size() << std::endl;
    // for (int c = 0; c < 3; ++c) 
    // {
    //     for (int h = 0; h < this->image.rows; ++h) 
    //     {
    //         for (int w = 0; w < this->image.cols; ++w) 
    //         {
    //             blob_proto.add_data(m.at<cv::Vec3f>(h, w)[c]);
    //         }
    //     }
    // }
    // blob_proto.set_num(1);
    // input_layer->FromProto(blob_proto);

    // std::chrono::high_resolution_clock::time_point begin = std::chrono::high_resolution_clock::now();
    this->net->Forward();

    caffe::Blob<float>* output_layer = this->net->output_blobs()[0];
    // std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();

    // std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count() << std::endl;

    return output_layer->cpu_data()[0];

}

void CaffeNetwork::WrapInputLayer(std::vector<cv::Mat>* input_channels)
{
    caffe::Blob<float>* input_layer = net->input_blobs()[0];

    int width = input_layer->width();
    int height = input_layer->height();
    float* input_data = input_layer->mutable_cpu_data();
    for (int i = 0; i < input_layer->channels(); i++)
    {
        cv::Mat channel(height, width, CV_32FC1, input_data);
        input_channels->push_back(channel);
        input_data += width*height;
    }
}

void CaffeNetwork::Preprocess(const cv::Mat& img, std::vector<cv::Mat>* input_channels)
{
    cv::Mat sample;
    if (img.channels() == 3 && num_channels == 1)
        cv::cvtColor(img, sample, cv::COLOR_BGR2GRAY);
    else if (img.channels() == 4 && num_channels == 1)
        cv::cvtColor(img, sample, cv::COLOR_BGRA2GRAY);
    else if (img.channels() == 4 && num_channels == 3)
        cv::cvtColor(img, sample, cv::COLOR_BGRA2BGR);
    else if (img.channels() == 1 && num_channels == 3)
        cv::cvtColor(img, sample, cv::COLOR_GRAY2BGR);
    else
        sample = img;
    
    cv::Mat sample_resized;
    if (sample.size() != input_geometry)
        cv::resize(sample, sample_resized, input_geometry);
    else
        sample_resized = sample;
    
    cv::Mat sample_float;
    if (num_channels == 3)
        sample_resized.convertTo(sample_float, CV_32FC3, 1.0/255.0);
    else
        sample_resized.convertTo(sample_float, CV_32FC1, 1.0/255.0);

    /* This operation will write the separate BGR planes directly to the
    * input layer of the network because it is wrapped by the cv::Mat
    * objects in input_channels. */
    cv::split(sample_float, *input_channels);

    CHECK(reinterpret_cast<float*>(input_channels->at(0).data)
        == net->input_blobs()[0]->cpu_data())
    << "Input channels are not wrapping the input layer of the network.";
}

void CaffeNetwork::update(Subject* imageObj)
{
    // this->image = ((Image*)imageObj)->frame();
    // this->image = ((Image*)imageObj)->getROI(0, 210, 640, 256);
    if (((Image*)imageObj)->frame().rows == 480)
    {
        this->image = ((Image*)imageObj)->getROI(0, 144, 640, 256);
        cv::resize(this->image, this->image, cv::Size(80, 32));
        // this->image = ((Image*)imageObj)->getROI(0, 210, 640, 256);
        // cv::resize(this->image, this->image, cv::Size(120, 48));
    }
    // cv::imshow("image", this->image);
    // cv::waitKey();
    // cv::resize(this->image, this->image, cv::Size(120, 48));
    // cv::imshow("image", this->image);
    // cv::waitKey();
    // std::cout << "result: " << 23.0*(2.0*this->infer()-1.0) << std::endl;
}