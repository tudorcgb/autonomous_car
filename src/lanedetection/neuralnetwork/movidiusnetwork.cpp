#include "movidiusnetwork.h"
#include "image.h"

#include <chrono>

// Load a graph file
// caller must free the buffer returned.
void *LoadFile(const char *path, unsigned int *length)
{
	FILE *fp;
	char *buf;

	fp = fopen(path, "rb");
	if(fp == NULL)
		return 0;
	fseek(fp, 0, SEEK_END);
	*length = ftell(fp);
	rewind(fp);
	if(!(buf = (char*) malloc(*length)))
	{
		fclose(fp);
		return 0;
	}
	if(fread(buf, 1, *length, fp) != *length)
	{
		fclose(fp);
		free(buf);
		return 0;
	}
	fclose(fp);
	return buf;
}

MovidiusNet::MovidiusNet(const std::string& graph_file)
{
    ncStatus_t retCode;

    int loglevel = 2;
    retCode = ncGlobalSetOption(NC_RW_LOG_LEVEL, &loglevel, sizeof(loglevel));

    retCode = this->getDevice();
    if (retCode != NC_OK)
    {
        std::cout << "Movidius device name error." << std::endl;
        exit(-1);
    }

    retCode = this->openDevice();
    if (retCode != NC_OK)
    {
        std::cout << "Movidius device open error." << std::endl;
        exit(-1);
    }

    std::cout << "Successfully opened NCS device!" << std::endl;

    retCode = this->readGraph(graph_file);
    if (retCode != NC_OK)
    {
        std::cout << "Movidius read graph error." << std::endl;
        exit(-1);
    }
    
    std::cout << "Successfully allocated graph." << std::endl;
}

ncStatus_t MovidiusNet::getDevice()
{
    return ncDeviceCreate(0, &this->deviceHandle);
}

ncStatus_t MovidiusNet::openDevice()
{
    return ncDeviceOpen(this->deviceHandle);
}

ncStatus_t MovidiusNet::readGraph(std::string graph_file)
{
    ncStatus_t retCode;
    retCode = ncGraphCreate(graph_file.c_str(), &this->graphHandle);
    this->graphBuffer = LoadFile(graph_file.c_str(), &this->graphBufferLength);
    retCode = ncGraphAllocateWithFifos(this->deviceHandle, this->graphHandle, this->graphBuffer, this->graphBufferLength, &this->inputFIFO, &this->outputFIFO);
    free(this->graphBuffer);
    return retCode;
    // return ncGraphCreate(graph_file.c_str(), &this->graphHandle);
    // return mvncAllocateGraph(this->deviceHandle, &this->graphHandle, this->graphFileBuf, this->graphFileLen);
}

ncStatus_t MovidiusNet::allocateGraph()
{
    return ncGraphAllocate(this->deviceHandle, this->graphHandle, this->graphBuffer, this->graphBufferLength);
}

double MovidiusNet::infer()
{
    ncStatus_t retCode;
    std::chrono::high_resolution_clock::time_point begin = std::chrono::high_resolution_clock::now();
    cv::Mat img(this->image.rows, this->image.cols, CV_32FC3);
    this->image.convertTo(img, CV_32FC3, 1.0/255.0);
    float* inputBuffer;
    unsigned int tensorLength = img.rows*img.cols*3*sizeof(float);
    inputBuffer = (float*)malloc(tensorLength);
    unsigned int index = 0;
    for (int c = 0; c < 3; ++c) 
    {
        for (int h = 0; h < img.rows; ++h) 
        {
            for (int w = 0; w < img.cols; ++w) 
            {
                inputBuffer[index] = img.at<cv::Vec3f>(h, w)[c];
            }
        }
    }

    retCode = ncGraphQueueInferenceWithFifoElem(
                this->graphHandle, 
                this->inputFIFO, 
                this->outputFIFO,
                (void*)inputBuffer,
                &tensorLength, 0);

    float* result;
    unsigned int fifoOutputSize = 0;
    unsigned int optionDataLen = sizeof(fifoOutputSize);
    ncFifoGetOption(this->outputFIFO, NC_RO_FIFO_ELEMENT_DATA_SIZE, &fifoOutputSize, &optionDataLen);
    result = (float*)malloc(fifoOutputSize);
    retCode = ncFifoReadElem(outputFIFO, result, &fifoOutputSize, NULL);

    std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();

    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count() << std::endl;

    // std::cout << "rrr: " << result[0] << std::endl;
    
    float res = result[0];

    free(result);
    
    return res;
}

MovidiusNet::~MovidiusNet()
{
    ncFifoDestroy(&this->inputFIFO);
    ncFifoDestroy(&this->outputFIFO);
    ncGraphDestroy(&graphHandle);
    ncDeviceClose(deviceHandle);
    ncDeviceDestroy(&deviceHandle);
}

void MovidiusNet::update(Subject* imageObj)
{
    // this->image = ((Image*)imageObj)->frame();
    if (((Image*)imageObj)->frame().rows == 480)
    {
        this->image = ((Image*)imageObj)->getROI(0, 210, 640, 256);
        cv::resize(this->image, this->image, cv::Size(120, 48));
    }
    // cv::imshow("image", this->image);
    // cv::waitKey();
    // std::cout << "result: " << 23.0*(2.0*this->infer()-1.0) << std::endl;
}