#pragma once
#include "common.h"
#include "observer.h"

using namespace cv;

class ObstacleDetection : public IObserver
{
private:
	Rect cropObstacle = Rect(166, 101, 348, 106);
	Scalar lower_limit_green = Scalar(30, 100, 50);
	Scalar upper_limit_green = Scalar(80, 255, 255);
	void segmentGreenObjects(Mat &src, Mat &dst);

	float binaryObjectSizeProportionAndObjectCenter(Mat binMat, Point & objCenter);

	float distanceToObject(Point from, Point to);
	
	Mat image;
public:
	ObstacleDetection();
	ObstacleDetection(float proportionOfColorInImage);
	ObstacleDetection(Rect roi ,float proportionOfColorInImage = 0.1);
	void* detectByGreenThresholdNoTransform(void*);
	~ObstacleDetection();
	void update(Subject* subject) override;

	bool obstacleVal;
};

