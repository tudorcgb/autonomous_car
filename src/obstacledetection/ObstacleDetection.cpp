#include "ObstacleDetection.h"
#include "image.h"


ObstacleDetection::ObstacleDetection() : obstacleVal(false), image(Mat(0,0,CV_8UC3))
{
}

ObstacleDetection::ObstacleDetection(float proportionOfColorInImage) : obstacleVal(false), image(Mat(0,0,CV_8UC3))
{
}

ObstacleDetection::ObstacleDetection(Rect roi, float proportionOfColorInImage) : obstacleVal(false), image(Mat(0,0,CV_8UC3))
{
}

void ObstacleDetection::segmentGreenObjects(Mat &src,Mat &dst) {
	Mat hsv;
	cvtColor(src, hsv, CV_BGR2HSV);
	inRange(hsv, lower_limit_green, upper_limit_green, dst);
}

float ObstacleDetection::binaryObjectSizeProportionAndObjectCenter(Mat binMat, Point &objCenter) {
	int objPixelsCount = 0;
	int sumX = 0;
	int max_y = NEG_INFINITY;
	int matSize = binMat.cols * binMat.rows;
	int maxYMiddleImage = 0;
	for (int i = 0; i < binMat.rows; i++) {
		const uchar *line = binMat.ptr<uchar>(i);
		for (int j = 0; j < binMat.cols; j++)
		{
			if (line[j] > 0) {
				objPixelsCount++;
				sumX += j;
				max_y = max(i, max_y);
				if (j == 320) {
					maxYMiddleImage = i;
				}
			}
		}
	}
	//objCenter = Point(sumX/ matSize, max_y);
	//distance from x= middle of image
	objCenter = Point(320, maxYMiddleImage + 300);
	return ((float)objPixelsCount / matSize);

}

float ObstacleDetection::distanceToObject(Point from, Point to) {
	//  1 pixel = 0.00863636363 meters
	float scale = 0.000863636363;
	float res = norm(to - from);//euclid distance
	float real_life_dist = res * scale;
	return real_life_dist;
}

void* ObstacleDetection::detectByGreenThresholdNoTransform(void* notUsed)
{
	// bool obstacle = false;
	obstacleVal = false;
	if (this->image.rows == 480 && this->image.cols == 640)
	{
		if(cropObstacle.width < (this->image.cols + cropObstacle.x) && cropObstacle.height < (this->image.rows + cropObstacle.y)){
		Mat croppedSrc = this->image(cropObstacle);
		Mat thresholded_image;
		segmentGreenObjects(croppedSrc, thresholded_image);
		Point objCenter;
		float prortion = binaryObjectSizeProportionAndObjectCenter(thresholded_image, objCenter);

		if (prortion > 0.1) {
			/*distanceToObject(Point(320, 480), objCenter);
			float real_distance = distanceToObject(Point(320, 480), objCenter);
			std::string distance_text = "Distance to object: " + std::to_string(real_distance) + " meters";
			putText(src, distance_text, Point(0, 30), FONT_HERSHEY_PLAIN, 2, Scalar(255, 0, 0), 3);*/
			obstacleVal = true;

		}
		//imshow("src", src);
		//waitKey(0);
		// return obstacle;
		}
	}
}


ObstacleDetection::~ObstacleDetection()
{
}

void ObstacleDetection::update(Subject* imageObj)
{
	if (((Image*)imageObj)->frame().rows == 480)
    {
		this->image = ((Image*)imageObj)->frame();
	}
}
