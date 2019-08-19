#ifndef MOVIDIUSNETWORK_H
#define MOVIDIUSNETWORK_H

#include <mvnc.h>

#include "ineuralnetwork.h"
#include "observer.h"

#define NAME_SIZE 100

class MovidiusNet : public ConvNeuralNetwork , public IObserver
{
public:
    MovidiusNet(const std::string& graph_file);
    MovidiusNet(const MovidiusNet& movidius);
    ~MovidiusNet();
    ncStatus_t getDevice();
    ncStatus_t openDevice();
    ncStatus_t readGraph(std::string graph_file);
    ncStatus_t allocateGraph();
    double infer() override;
    void update(Subject* subject) override;
private:
    // char deviceName[NAME_SIZE];
    struct ncDeviceHandle_t* deviceHandle;
    void* graphBuffer;
    unsigned int graphBufferLength;
    ncGraphHandle_t* graphHandle;
    struct ncFifoHandle_t* inputFIFO;
    struct ncFifoHandle_t* outputFIFO;
};

#endif // MOVIDIUSNETWORK_H