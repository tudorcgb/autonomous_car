#include "TrafficSignRecognition.h"
#include "image.h"

#include <unistd.h>

#include <chrono>

VideoWriter videoResult("result1.avi", CV_FOURCC('M', 'J', 'P', 'G'), 20, Size(643, img_height), true);




TrafficSignRecognition::TrafficSignRecognition() : image(Mat(0,0,CV_8UC3))
{

	hog = HOGDescriptor (
		Size(32, 32), //winSize
		Size(8, 8), //blocksize
		Size(4, 4), //blockStride, 50% block size
		Size(8, 8), //cellSize,
		9, //nbins,
		1, //derivAper,
		-1, //winSigma,
		0, //histogramNormType,
		0.2, //L2HysThresh,
		0,//gammal correction,
		64,//nlevels=64
		1);

	parkingSignBool = false;
	stopSignBool = false;

	if (write_video) {
		videoResult.release();
		videoResult = VideoWriter("result1.avi", CV_FOURCC('M', 'J', 'P', 'G'), 20, Size(img_width, img_height), true);
	}

	loadAutoTrainedSVM();

	getSlidingWindowsROIs(imageROIs, imageROIsInBigROI, rect);

}

//TrafficSignRecognition::TrafficSignRecognition(int overlaping_rects_threshold, float overlaping_rects_eps, int step_size_ratio_sliding_windows, int background_images_ratio, int number_of_sliding_windows_widths, float red_hue_color_threshold, float blue_hue_color_threshold, bool write_video, bool show_image_result, int xROIOffset, int yROIOffset, int ROIWidth, int ROIHeight)
//{
//}

//returns just stop sign
void TrafficSignRecognition::trafficSignInImage(Mat src, bool &parkingSign,bool &stopSign)
{		
		CapturedImage capImg;
		capImg.src = src(bigRoi.rect);
		//capImg.filePath = imagePath;
		//capImg.roi = bigRoi.rect;
		addHueMatToCapturedImgROI(capImg);

		//drawRoisTest(capImg, imageROIs);
		
		addPosibleThresholdROIs(capImg, imageROIsInBigROI);

		Mat testMat = Mat::zeros(capImg.possibleSigns.size(), descriptor_size, CV_32FC1);
		createTrainHogMatrix(testMat,capImg.possibleSigns);

		Mat preditionResults = Mat::zeros(capImg.possibleSigns.size(), 1, CV_32FC1);
		svmPredict(testMat, preditionResults);

		vector<Rect> overlappingRectanglesStopSign;
		vector<Rect> overlappingRectanglesParkingSign;
		//just stop sign
	groupOverlappingRectangles(capImg, preditionResults, overlappingRectanglesStopSign, overlappingRectanglesParkingSign,parkingSign,stopSign);
	
}

void* TrafficSignRecognition::trafficSignInImageA(void* notUsed)
{		
	// while (true)
	{
		if (this->image.rows == 480 && this->image.cols == 640)
	{
		if(bigRoi.rect.width < (this->image.cols + bigRoi.rect.x) && bigRoi.rect.height < (this->image.rows + bigRoi.rect.y)){
		CapturedImage capImg;
		
		capImg.src = this->image(bigRoi.rect);
		//capImg.filePath = imagePath;
		//capImg.roi = bigRoi.rect;
		addHueMatToCapturedImgROI(capImg);

		//drawRoisTest(capImg, imageROIs);
		
		addPosibleThresholdROIs(capImg, imageROIsInBigROI);

		Mat testMat = Mat::zeros(capImg.possibleSigns.size(), descriptor_size, CV_32FC1);
		createTrainHogMatrix(testMat,capImg.possibleSigns);

		Mat preditionResults = Mat::zeros(capImg.possibleSigns.size(), 1, CV_32FC1);
		svmPredict(testMat, preditionResults);

		vector<Rect> overlappingRectanglesStopSign;
		vector<Rect> overlappingRectanglesParkingSign;
		//just stop sign
		groupOverlappingRectangles(capImg, preditionResults, overlappingRectanglesStopSign, overlappingRectanglesParkingSign,parkingSignBool,stopSignBool);

		std::cout << "Detection: stop: " << stopSignBool << std::endl;
		// sleep(0.3);
		}
	}
	}
	
}


TrafficSignRecognition::~TrafficSignRecognition()
{
}

void TrafficSignRecognition::update(Subject* imageObj)
{
	if (((Image*)imageObj)->frame().rows == 480)
	{
		this->image = ((Image*)imageObj)->frame();
	}
	// bool parking = false;
	// bool stop = false;

	// std::chrono::high_resolution_clock::time_point begin = std::chrono::high_resolution_clock::now();
	// this->trafficSignInImageA(parking, stop);
	// std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();

    // std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count() << std::endl;
    // std::cout << "Parking: " << parking << " Stop: " << stop << std::endl;
	// imshow("image", this->image);
	// waitKey();
}