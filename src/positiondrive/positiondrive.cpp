#include "positiondrive.h"
#include "pathDrriveTest.hpp"

using namespace cv;

int PositionDrive::nsleep(long miliseconds)
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

PositionDrive::PositionDrive() {}

PositionDrive::PositionDrive(const CarControl* c_, const PathTracking* path_)
{
    this->c = c;
    this->path = path_;
    this->angle = 0.0;
    if (pthread_mutex_init(&lock_angle, NULL) != 0) 
    { 
        printf("\n mutex init has failed\n"); 
        exit(-1);
    }
}

// PositionDrive::PoistionDrive(const PositionDrive& positionDrive)
// {
//     this->c = positionDrive.c;
//     this->path = positionDrive.path;
//     this->angle = positionDrive.angle;
//     this->lock_angle = positionDrive.lock_angle;  
//     this->x = positionDrive.x;
//     this->y = positionDrive.y  
//     this->curent_yaw_rad = positionDrive.curent_yaw_rad;
// }

PositionDrive::~PositionDrive() {}

void PositionDrive::drive(std::vector<Point> &dstPoints)
{
    float curentX = 0.0, curentY = 539.0 - 63.0;

	float curentAngle = curent_yaw_rad;

	while (dstPoints.size() > 2)
	{       
        // pthread_mutex_t lock_angle;
        Point dstPoint = dstPoints.at(0);
		printf("dstPoint x:%d y:%d \n",dstPoint.x,dstPoint.y);
		bool pointReached = pointInsideCircle(Point( curentX , curentY), dstPoints.at(0));
        bool pointNotModified = true;
        float previousEstimatedDistance = 0.0;
        float distanceDriven = 0.0;
        bool noPointEliminated = true;
        //bool pointReached = false;
        while (!pointReached) {
            std::cout << "while" << std::endl;
            curentX = x, curentY = y;
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
            // navPoints.push_back(dstPoint);
            float distanceToPoint, angleToPoint;
            angleAndDirectionOfPoints2(curentX,curentY,dstPoint.x,dstPoint.y,  (curent_yaw_rad*180/PI), angleToPoint,distanceToPoint);
            //printf("Local: x:%d y:%d distanceToPointDst:%f , angleToPointDst: %f;\n",pcurent.x ,pcurent.y, distanceToPoint, angleToPoint);
            if (angleToPoint > 20.0) {
                try
                {
                    // c->move(0.2,23.0);
                    pthread_mutex_lock(&lock_angle);
                    angle = 23.0;
                    pthread_mutex_unlock(&lock_angle);
                }
                catch (std::exception e)
                {
                    std::cout << e.what() << std::endl;
                }
                //printf(" c->move(0.2,%.2f);\n",23.0);
            }
            else if (angleToPoint < -20.0  ) {
                try
                {
                    // c->move(0.2,-23.0);
                    pthread_mutex_lock(&lock_angle);
                    angle = -23.0;
                    pthread_mutex_unlock(&lock_angle); 
                }
                catch (std::exception e)
                {
                    std::cout << e.what() << std::endl;
                }
                //printf(" c->move(0.2,%.2f);\n",-23.0);
            }
            else{
                try
                {
                    // c->move(0.2,angleToPoint);
                    pthread_mutex_lock(&lock_angle);
                    angle = angleToPoint;
                    pthread_mutex_unlock(&lock_angle);
                }
                catch (std::exception e)
                {
                    std::cout << e.what() << std::endl;
                }
                //printf(" c->move(0.2,%.2f);\n",angleToPoint);
            }
            nsleep(100);
            pointReached = pointInsideCircle(pcurent,dstPoint);
        }
        dstPoints.erase(dstPoints.begin());
    }
}

void PositionDrive::drivePoints(const int pointsNo)
{
    // pthread_mutex_lock(&lock_angle);
        done = false;;
    // pthread_mutex_unlock(&lock_angle);
    std::vector<Point> points;
    std::vector<std::complex<double>> pointsVector;
    this->path->nextNodes(pointsNo, pointsVector);
    for (int i = 0; i < pointsVector.size() ; i++)
    {
        std::complex<double> point = pointsVector[i];
        float distanceToPointP01,distanceToPointP11, angleToPointP11, innerAngleP11;
        Point p = Point(point.real()*100 + 27, (double)521.5 - (double)(point.imag()) *100);
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
        // originalPoints.push_back(p);
    }
    this->drive(points);
    std::cout << "drive" << std::endl;

    std::cout << "done" << std::endl;
    // pthread_mutex_lock(&lock_angle);
        done = true;
    // pthread_mutex_unlock(&lock_angle);
    std::cout << "Try to exit." << std::endl;
    // exit(1);
    // std::cout << "exit_sys" << std::endl;
}

void PositionDrive::drivePointsObs(std::vector<std::complex<double>> &pointsVector)
{
    pthread_mutex_lock(&lock_angle);
        done = false;;
    pthread_mutex_unlock(&lock_angle);
    std::vector<Point> points;
    for (int i = 0; i < pointsVector.size() ; i++)
    {
        std::complex<double> point = pointsVector[i];
        float distanceToPointP01,distanceToPointP11, angleToPointP11, innerAngleP11;
        Point p = Point(point.real()*100 + 27, (double)521.5 - (double)(point.imag()) *100);
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
        // originalPoints.push_back(p);
    }
    this->drive(points);

    pthread_mutex_lock(&lock_angle);
        done = true;
    pthread_mutex_unlock(&lock_angle);
}

void PositionDrive::update(Subject* positionObj)
{
    this->x = ((IMUEncoderPosition*)positionObj)->getX();
    this->y = ((IMUEncoderPosition*)positionObj)->getY();
    this->curent_yaw_rad = ((IMUEncoderPosition*)positionObj)->yaw();
}