#pragma once
#include "common.h"
#include "observer.h"

using namespace std;
// namespace fs = std::filesystem;
using namespace cv;
using namespace cv::ml;

const int img_width = 640;
const int img_height = 480;


const int xROIOffset = 360;
const int yROIOffset = 10;
const int ROIWidth = 280;
const int ROIHeight = 150;

typedef struct {
	//todo
	int xOffset = xROIOffset;
	int yOffset = yROIOffset;
	//int yEndOffset = yROIEndOffset;
	//int xEndOffset = xROIEndOffset;
	//Rect rect = Rect(xOffset, yOffset, img_width - xOffset - xEndOffset, img_height - yOffset - yEndOffset);
	Rect rect = Rect(xOffset, yOffset, ROIWidth, ROIHeight);

}TrafficSignsROI;
typedef enum {
	RED = 0,
	BLUE = 1,
	NO_SIGN = -1
} SignColorEnum;

typedef struct {

	Mat src;
	Mat hue;
	int width;
	int height;
	Rect roi;
	SignColorEnum sign_color = NO_SIGN;
	int predictedClass = 0;
	vector<float> hogDescriptor;


}AnnotedImage;

typedef struct {
	Mat src;
	Mat src_hue;
	vector<AnnotedImage> possibleSigns;
	string filePath;
	Rect roi = Rect(0, 0, img_width, img_height);
	Mat hsv;
}CapturedImage;



class TrafficSignRecognition : public IObserver
{
private:
	
	int overlaping_rects_threshold = 1;
	int overlaping_rects_eps = 1.0;
	int step_size_ratio_sliding_windows = 5;
	int background_images_ratio = 6;
	int number_of_sliding_windows_widths = 6;
	int red_hue_color_threshold = 0.2;
	int blue_hue_color_threshold = 0.4;
	int write_video = false;
	int show_image_result = false;
	//int xROIOffset = 360;
	//int yROIOffset = 10;
	//int ROIWidth = 280;
	//int ROIHeight = 150;
	int descriptor_size = 441;

	//red range
	const Scalar red_low = Scalar(170, 100, 100);
	const Scalar red_high = Scalar(180, 200, 255);

	//blue range
	const Scalar blue_low = Scalar(100, 0, 50);
	const Scalar blue_high = Scalar(150, 50, 110);

	Ptr<SVM> svm = SVM::create();
	//create image roi Rects
	vector<Rect> imageROIs;
	vector<Rect> imageROIsInBigROI;

	TrafficSignsROI bigRoi;
	HOGDescriptor hog;
	Rect rect = bigRoi.rect;

	Mat image;


	void loadAutoTrainedSVM() {

		svm = SVM::load("resources/svmAutoTrained");	// TODO path
	}

	void getSlidingWindowsROIs(vector<Rect> &imageROIs, vector<Rect> &imageROIsInBigROI, Rect &bigRoi) {

		//int noWinWidths = number_of_sliding_windows_widths;
		int winWidth[15] = { 40,50,60,70,90,100,120,140,180 };
		for (int scale = 0; scale < number_of_sliding_windows_widths; scale++) {
			int window_colls = winWidth[scale];
			int window_rows = window_colls;
			int step_size = window_colls / step_size_ratio_sliding_windows;

			for (int i = bigRoi.y; i < bigRoi.y + bigRoi.height - window_rows; i += step_size) {
				for (int j = bigRoi.x; j < bigRoi.x + bigRoi.width - window_colls; j += step_size) {
					Rect roi(j, i, window_colls, window_rows);
					imageROIs.push_back(roi);

				}
			}
			for (int i = 0; i < bigRoi.height - window_rows - step_size; i += step_size) {
				for (int j = 0; j < bigRoi.width - window_colls - step_size; j += step_size) {
					Rect roi(j, i, window_colls, window_rows);
					imageROIsInBigROI.push_back(roi);

				}
			}
		}
	}
	void addHueMatToCapturedImgROI(CapturedImage &img) {
		Mat HSV;

		cvtColor(img.src, HSV, CV_BGR2HSV);
		//cvtColor(img.src, HSV, CV_BGR2HSV);
		cvtColor(img.src, img.src, CV_BGR2GRAY);
		//vector<Mat> hsv_planes;
		//split(HSV, hsv_planes);
		//Mat h = hsv_planes[0]; // H channel
		//img.src_hue = h;
		img.hsv = HSV;
	}
	void thresholdCapturedImageHSV(CapturedImage &img, Mat &lower_blue_hue_range, Mat &lower_red_hue_range) {

		//Mat lower_blue_hue_range;
		inRange(img.hsv, blue_low, blue_high, lower_blue_hue_range);

		//Mat lower_red_hue_range;
		inRange(img.hsv, red_low, red_high, lower_red_hue_range);


	}
	void countBlueHueAndRedHue(Mat &lower_blue_hue_range, Mat &lower_red_hue_range, Rect roi, float &blueRation, float &redRatio, int img_width = 32, int img_height = 32) {
		int blue_pixels = 0;
		int red_pixels = 0;


		int w = 1;
		for (int i = roi.y; i < roi.y + roi.height; i++) {
			const uchar* img_red = lower_red_hue_range.ptr<uchar>(i);
			const uchar* img_blue = lower_blue_hue_range.ptr<uchar>(i);

			for (int j = roi.x; j < roi.x + roi.width; j++) {

				uchar valBlue = lower_blue_hue_range.at<uchar>(i, j);
				uchar valRed = lower_red_hue_range.at<uchar>(i, j);

				if (valBlue == 255) {
					blue_pixels++;
				}

				//todo else ?
				if (valRed == 255) {
					red_pixels++;
				}
			}
		}

		float area = img_width * img_height;

		blueRation = (float)blue_pixels;
		redRatio = (float)red_pixels;
	}
	void addPosibleThresholdROIs(CapturedImage &img, vector<Rect> imageROIs, float redRatioThreshold = 0.2, float blueRatioThreshold = 0.2, bool groupRects = false) {


		Mat lower_blue_hue_range;
		Mat lower_red_hue_range;

		// TickMeter t;
		// t.reset();

		//thresholdCapturedImage(img, lower_blue_hue_range, lower_red_hue_range);
		thresholdCapturedImageHSV(img, lower_blue_hue_range, lower_red_hue_range);

		// t.start();
		for (Rect roi : imageROIs) {

			AnnotedImage smallRoi;
			float blueRatio = 0.0, redRatio = 0.0;

			
			countBlueHueAndRedHue(lower_blue_hue_range, lower_red_hue_range, roi, blueRatio, redRatio, roi.width, roi.height);
		
			if (redRatio > 200) {
				smallRoi.sign_color = RED;
				resize(img.src(roi), smallRoi.src, Size(32, 32), false);
				smallRoi.roi = roi;
				img.possibleSigns.push_back(smallRoi);
			}
			
			else if (blueRatio > 200) {
				smallRoi.sign_color = BLUE;
				resize(img.src(roi), smallRoi.src, Size(32, 32), false);
				smallRoi.roi = roi;
				img.possibleSigns.push_back(smallRoi);
			}


		}

	}
	void createTrainHogMatrix(Mat &testHOG, vector<AnnotedImage> &testImgs) {
		int testImgsLenght = testImgs.size();
		//testHOG = Mat::zeros(testImgsLenght, descriptor_size, CV_32FC1);

		for (int y = 0; y < testImgs.size(); y++) {
			//hog.compute(testImgs[y].src, vector<float> (testHOG.begin<float>(), testHOG.end<float>()));
			vector<float> descriptors;
			hog.compute(testImgs[y].src, descriptors);
			//testHOG.row(y).setTo(descriptors);
			memcpy(testHOG.data + (y * descriptor_size * sizeof(float)), descriptors.data(), descriptors.size() * sizeof(float));
			//testHOG.push_back(descriptors);
		}
	}
	void svmPredict(Mat &testMat, Mat &testResponse)
	{
		svm->predict(testMat, testResponse);
	}
	void groupOverlappingRectangles(CapturedImage img, Mat preditionResults, vector<Rect> &overlappingRectanglesStopSign, vector<Rect> &overlappingRectanglesParkingSign, bool &parkingSign, bool &stopSign) {
		//todo array of RectaglesWithClass type , for each class of sign
		//vector<Rect> overlappingRectanglesStopSign;
		//vector<Rect> overlappingRectanglesParkingSign;

		int i = 0;
		for (AnnotedImage detectedSign : img.possibleSigns) {
			int predClass = preditionResults.at<float>(i, 0);
			if (predClass == 45) {
				//objectRectangles.push_back(detectedSign.roi);
				overlappingRectanglesParkingSign.push_back(detectedSign.roi);
			}
			else if (predClass == 21) {
				overlappingRectanglesStopSign.push_back(detectedSign.roi);
			}
			i++;
		}
		groupRectangles(overlappingRectanglesStopSign, overlaping_rects_threshold, overlaping_rects_eps);
		groupRectangles(overlappingRectanglesParkingSign, overlaping_rects_threshold, overlaping_rects_eps);
		if (overlappingRectanglesStopSign.size() > 0) {
			stopSign = true;
		}
		else
		{
			stopSign = false;
		}
		if (overlappingRectanglesParkingSign.size() > 0) {
			parkingSign = true;
		}
		else
		{
			parkingSign = false;
		}
	}
public:
	//TrafficSignRecognition with normal paramteres
	TrafficSignRecognition();
	/*TrafficSignRecognition(
		int overlaping_rects_threshold = 3,
		float overlaping_rects_eps = 0.6,
		int step_size_ratio_sliding_windows = 5,
		int background_images_ratio = 4,
		int number_of_sliding_windows_widths = 4,
		float red_hue_color_threshold = 0.2,
		float blue_hue_color_threshold = 0.4,
		bool write_video = false,
		bool show_image_result = false,
		int xROIOffset = 360,
		int yROIOffset = 10,
		int ROIWidth = 280,
		int ROIHeight = 150);*/

	bool parkingSignBool;
	bool stopSignBool;

	void trafficSignInImage(Mat image, bool &parkingSign, bool &stopSign);

	void* trafficSignInImageA(void*);

	~TrafficSignRecognition();

	void update(Subject* subject) override;
};

