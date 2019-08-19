#if 1

#include "pathtracking.h"

#include <string>
#include <iostream>
#include <experimental/filesystem>
#include <unistd.h>

#include <signal.h>

#include "carcontrol.h"

#include "findfile.h"

#include "RTIMULib.h"

#include "math.h"

#include <opencv2/core.hpp>

#include <opencv2/highgui.hpp>

#include <opencv2/imgproc/imgproc.hpp>

#include <pthread.h>

#include <cstdlib>

#include <unistd.h>   

#include <errno.h>    

#include "pathDrriveTest.hpp"

#include <time.h>   /* Needed for struct timespec */

#include "ObstacleDetection.h" 

//#include "map.h"

using namespace std;

using namespace cv;

#define NUM_THREADS 5
#define PI 3.14159265

pthread_mutex_t lock_encoder; 
pthread_mutex_t lock_yaw; 
pthread_mutex_t lock_XY; 
pthread_mutex_t lock_StableXY; 

CarControl *c;
RTIMU *imu;

int sampleCount = 0;
int sampleRate = 0;
// float xAnt = Map::getInstance()["NOD1"].coord().real() * 100 + 22.5;
// float yAnt = 516.5 - Map::getInstance()["NOD1"].coord().imag() * 100;

//  float xAnt = 205;
//  float yAnt = 291;

float xAnt = 30;
 float yAnt = 66;

float x = 0.0;
float y = 63.0;
float rotPerMetter = 148.00 ; // = 1 cm ? ...1 pixel = 1.28
int totalRots = 0;
uint64_t rateTimer;
uint64_t displayTimer;
uint64_t now;
vector<Point> points;
vector<Point> navPoints;
vector<Point> originalPoints;
float curent_yaw_rad = 0.0;
float initial_angle_yaw = 0.0;
Mat mapTrack;
int readingCounter = 0;
int readingIMUCounter = 0;
//float stableX = 0.0, stableY = 63.0;

bool runningIMU = false;


int nsleep(long miliseconds)
{
   struct timespec req, rem;

   if(miliseconds > 999)
   {   
        req.tv_sec = (int)(miliseconds / 1000);                            
        req.tv_nsec = (miliseconds - ((long)req.tv_sec * 1000)) * 1000000; 
   }   
   else
   {   
        req.tv_sec = 0;                         
        req.tv_nsec = miliseconds * 1000000;    
   }   
   return nanosleep(&req , NULL);
}

void interruptHandler(sig_t s){
    c->brake(0.0);
    c->close();
    exit(1); 
}

void storePoint(int x, int y){

    points.push_back(Point(x,y));

}

void drawPointsOnMap(){
    mapTrack = imread("/media/pi/PiDrive1/Holistic2/master/resources/Race_resize_mark.png");

	for(Point p : points){
        circle(mapTrack,p,1,Scalar(0,255,0),1);
    }
    for(Point p : navPoints){
        circle(mapTrack,p,3,Scalar(255,255,0),1);
    }
    for(Point p : originalPoints){
        circle(mapTrack,p,1,Scalar(0,255,255),1);
    }

    imwrite("/media/pi/PiDrive1/Holistic2/master/resources/mapat2.jpg",mapTrack);

}
//sets an intial angle value so that starting angle is 0
void setAngleToZero(){
        imu->IMURead();
        RTIMU_DATA imuData = imu->getIMUData();
        float theta = (float)imuData.fusionPose.z();
        initial_angle_yaw = theta;
        printf("initial angle:%f\n",initial_angle_yaw);
}

//set curent_yaw_rad as angle relative to car 0 angle
void readIMU(){
    while(runningIMU){
        usleep(imu->IMUGetPollInterval() * 1000);
        
        imu->IMURead();
        RTIMU_DATA imuData = imu->getIMUData();
        float theta = (float)imuData.fusionPose.z();

        pthread_mutex_lock(&lock_yaw);
            curent_yaw_rad = theta - initial_angle_yaw;
        pthread_mutex_unlock(&lock_yaw); 

    }
}

// encoder callback, updates X,Y value based on curent yaw read and distance traveled since last encoder callback
void* print(void* str)
{

    //  display 10 times per second 198906242
    float local_yaw = 0.0;
    pthread_mutex_lock(&lock_yaw);
        local_yaw = curent_yaw_rad;
    pthread_mutex_unlock(&lock_yaw); 
        double flt = atoi( (char*)str);
        double du = (flt / rotPerMetter);
        float locx = xAnt + (du * (float)cos(local_yaw));
        float locy = yAnt + (du * (float)sin(local_yaw)); // + sin.. for y growind downwords

    pthread_mutex_lock(&lock_XY);
        x = locx;
        y = locy;
        xAnt = x;
        yAnt = y;
    pthread_mutex_unlock(&lock_XY); 
        readingCounter++;
        if(readingCounter == 20){            
            storePoint(x, y);
            readingCounter = 0;
        }
}

//inits imu , reads calibration file and sets imu parameters
void initImu(){
    
    RTIMUSettings *settings = new RTIMUSettings("/media/pi/PiDrive1/Holistic2/master/src/imu/","RTIMULib");

    imu = RTIMU::createIMU(settings);

    if ((imu == NULL) || (imu->IMUType() == RTIMU_TYPE_NULL)) {
        printf("No IMU found\n");
        exit(1);
    }

    //  This is an opportunity to manually override any settings before the call IMUInit

    //  set up IMU

    imu->IMUInit();
    runningIMU = true;

    //  this is a convenient place to change fusion parameters
    //  The slerp power valule controls the influence of the measured state to correct the predicted state
    //  0 = measured state ignored (just gyros), 1 = measured state overrides predicted state.
    //  In between 0 and 1 mixes the two conditions default 0.02
    imu->setSlerpPower(0);
    imu->setGyroEnable(true);
    imu->setAccelEnable(false);
    imu->setCompassEnable(true);

    //  set up for rate timer
    rateTimer = displayTimer = RTMath::currentUSecsSinceEpoch();
    
    usleep(imu->IMUGetPollInterval() * 1000);
    setAngleToZero();
}


//debug test point for drive function
void testPoints(vector<Point> &points) {
    //small turns
    // points.push_back(Point(70, 66));
    // points.push_back(Point(112, 63));
    // points.push_back(Point(161, 61));
    // points.push_back(Point(207, 53));
    // points.push_back(Point(244, 51));
    // points.push_back(Point(301, 59));
    // points.push_back(Point(344, 64));
    // points.push_back(Point(388, 67));
    // points.push_back(Point(440, 71));
    // points.push_back(Point(492, 78));
	
	//points.push_back(Point(28, 66));
	points.push_back(Point(68, 66));
	points.push_back(Point(110, 67));
	points.push_back(Point(112, 114));
	points.push_back(Point(114, 158));
	points.push_back(Point(114, 201));
	// points.push_back(Point(114, 246));
	// points.push_back(Point(116, 290));
	// points.push_back(Point(155, 291));
	// points.push_back(Point(204, 288));
	// points.push_back(Point(251, 291));
	// points.push_back(Point(296, 291));
	// points.push_back(Point(342, 292));
	// points.push_back(Point(380, 294));
	// points.push_back(Point(430, 294));
	// points.push_back(Point(427, 249));
	// points.push_back(Point(430, 202));
	// points.push_back(Point(424, 158));
	// points.push_back(Point(424, 110));
	// points.push_back(Point(422, 73));
	// points.push_back(Point(388, 60));
	// points.push_back(Point(380, 25));
	// points.push_back(Point(335, 24));
	// points.push_back(Point(288, 24));
	// points.push_back(Point(245, 27));
	// points.push_back(Point(204, 24));


}

//converts complex points read from map (with Y axis growing upwards) to cv::Point struct with coordinates on X axis sfrom 0 to 853 and Y  from 0 to 539
void complexPointsToPoinst(vector<Point> &points){

    PathTracking p("NOD1", "NOD45");
    std::vector<std::complex<double>> pointsVector;
    p.nextNodes(15, pointsVector);
    for (int i = 0; i < pointsVector.size() ; i++)
    {
        std::complex<double> point = pointsVector[i];
        float distanceToPointP01,distanceToPointP11, angleToPointP11, innerAngleP11;
        Point p = Point(point.real()*100 + 22.5, (double)516.5 - (double)(point.imag()) *100);
        // if(i < pointsVector.size() - 2){
        //     Point p1 = Point(pointsVector.at(i+1).real()*100 + 22.5, (double)516.5 - (double)(pointsVector.at(i+1).imag()) *100);
        //     Point p2 = Point(pointsVector.at(i+2).real()*100 + 22.5, (double)516.5 - (double)(pointsVector.at(i+2).imag()) *100);
        //     distanceAnglePointToVector(p.x,p.y,p1, p2, distanceToPointP01, distanceToPointP11 , angleToPointP11, innerAngleP11);
        //     if(innerAngleP11 > 30.0){
        //         moveCurveStartingPoint(p,p2,angleToPointP11);
        //         printf("Moved dstPoint x:%d y:%d \n",p.x,p.y);
        //     }
        // }
        points.push_back(p);
        originalPoints.push_back(p);
    }
    
}

void drive5(vector<Point> &dstPoints){

    float curentX = 0.0, curentY = 539.0 - 63.0;

	float curentAngle = curent_yaw_rad;

	while (dstPoints.size() > 2)
	{       

        Point dstPoint = dstPoints.at(0);
		printf("dstPoint x:%d y:%d \n",dstPoint.x,dstPoint.y);
		bool pointReached = pointInsideCircle(Point( curentX , curentY), dstPoints.at(0));
        bool pointNotModified = true;
        float previousEstimatedDistance = 0.0;
        float distanceDriven = 0.0;
        bool noPointEliminated = true;
        //bool pointReached = false;
        while (!pointReached) {
            pthread_mutex_lock(&lock_XY);
                curentX = x, curentY = y;
            pthread_mutex_unlock(&lock_XY); 
            Point pcurent = Point(curentX, curentY);
            
            if(dstPoints.size() > 2){
                float distanceToPointP0,distanceToPointP1, angleToPointP1, innerAngleP1;
                distanceAnglePointToVector(curentX,curentY, dstPoints.at(0), dstPoints.at(1), distanceToPointP0, distanceToPointP1 , angleToPointP1, innerAngleP1);
                if(pointNotModified){
                    float distanceToPointP01,distanceToPointP11, angleToPointP11, innerAngleP11;
                    distanceAnglePointToVector(dstPoints.at(0).x,dstPoints.at(0).y,dstPoints.at(1), dstPoints.at(2), distanceToPointP01, distanceToPointP11 , angleToPointP11, innerAngleP11);
                    if(innerAngleP11 > 45.0){
                        dstPoint = moveCurveStartingPoint(dstPoint.x,dstPoint.y,angleToPointP11, (curent_yaw_rad*180/PI));
                        printf("Moved dstPoint x:%d y:%d \n",dstPoint.x,dstPoint.y);
                        pointNotModified = false;
                    }
                }
                distanceDriven += (previousEstimatedDistance - distanceToPointP0);
                if(innerAngleP1 > 50.0 && ( (distanceDriven >= distanceToPointP1) || noPointEliminated)){
                    //printf("estim dist: %f driven dist %f \n",previousEstimatedDistance,distanceDriven );
                    distanceDriven = 0.0;
                    previousEstimatedDistance = distanceToPointP1;
                    dstPoints.erase(dstPoints.begin());
                    dstPoint = dstPoints.at(0);
                    //dstPoint = moveCurveStartingPoint(dstPoint.x,dstPoint.y,angleToPointP1, (curent_yaw_rad*180/PI));
                    noPointEliminated = false;
                    
                    printf("Point eliminated next dstPoint x:%d y:%d angle:%f \n",dstPoint.x,dstPoint.y,innerAngleP1);
                }
            }
            navPoints.push_back(dstPoint);
            float distanceToPoint, angleToPoint;
            angleAndDirectionOfPoints2(curentX,curentY,dstPoint.x,dstPoint.y,  (curent_yaw_rad*180/PI), angleToPoint,distanceToPoint);
            //printf("Local: x:%d y:%d distanceToPointDst:%f , angleToPointDst: %f;\n",pcurent.x ,pcurent.y, distanceToPoint, angleToPoint);
            if (angleToPoint > 16.0) {
                c->move(0.2,23.0);
                //printf(" c->move(0.2,%.2f);\n",23.0);
                }
            else if (angleToPoint < -16.0  ) {
                c->move(0.2,-23.0);
                //printf(" c->move(0.2,%.2f);\n",-23.0);
                }
            else{
                c->move(0.2,angleToPoint);
                //printf(" c->move(0.2,%.2f);\n",angleToPoint);
                }
            nsleep(100);
            pointReached = pointInsideCircle(pcurent,dstPoint);
        }
        dstPoints.erase(dstPoints.begin());    
    }

}
int main(int argc, char **argv)
{

    vector<Point> dstPoints;
    complexPointsToPoinst(dstPoints);

    sleep(1);

    signal (SIGINT, interruptHandler);

    c = new CarControl();

    initImu();

    if (pthread_mutex_init(&lock_encoder, NULL) != 0) 
        { 
            printf("\n mutex init has failed\n"); 
            return 1; 
        } 
    if (pthread_mutex_init(&lock_yaw, NULL) != 0) 
    { 
    printf("\n mutex init has failed\n"); 
    return 1; 
    } 
    if (pthread_mutex_init(&lock_XY, NULL) != 0) 
    { 
    printf("\n mutex init has failed\n"); 
    return 1; 
    } 

   pthread_t threads[NUM_THREADS];
   int rc;
   int i = 1;
   int thread_stat;
   
    rc = pthread_create(&threads[i], NULL, readIMU, (void *)i);

    c->brake(0.0);

    c->enablePID();

    c->enableEncoder(print);

    sleep(1);

    drive5(dstPoints);

    c->brake(0.0);

    c->disableEncoder();

    c->disablePID();

    c->close();

    drawPointsOnMap();

    sleep(1);

    printf("X: %f Y: %f \n",x,y);
    printf("end angle:%f\n", curent_yaw_rad);

    runningIMU = false;                                                                                            
    int status = pthread_join( rc, (void *)&thread_stat);                        
    if ( status <  0)                                                              
        perror("pthread_join failed\n");  
    printf("Thread join\n");  

    pthread_mutex_destroy(&lock_encoder); 
    printf("Lock destroyed\n");

    return 0;
}


#endif