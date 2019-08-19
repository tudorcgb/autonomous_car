#ifndef BEHAVIOUR_TREE
#define BEHAVIOUR_TREE

#include <functional>
#include <mutex>

#include "observer.h"
#include "state.h"
#include "idlestate.h"
#include "parkingstate.h"
#include "stopsignstate.h"
#include "lanefollow.h"
#include "overtakestate.h"
#include "pathplanningstate.h"
#include "crossroadstate.h"

#include "pathtracking.h"
#include "positiondrive.h"
#include "imu.h"
#include "imuencoderposition.h"
#include "image.h"
#include "caffenetwork.h"

#include "TrafficSignRecognition.h"
#include "ObstacleDetection.h"

#include "carcontrol.h"
#include "gps.h"

#include "timer.h"

#ifndef FN_PTR
// typedef void* (*FnPtr)(void*);
typedef std::function<void(void*)> FnPtr;
#endif // FNPTR

enum states 
{
    IDLE=0,
    PATH_PLANNING,
    CROSSROAD,
    STOP,
    LANE_FOLLOW,
    OVERTAKE,
    PARKING
};

class Machine : public Subject
{
public:
    Machine();
    void runTask(FnPtr routine);
    void abortTask();
    void* run(void*);
    void* switchState(void*);

    CarControl carControl;
    
    bool waitForGPS;
    GPSConnection gps;
private:
    FnPtr runningTask;
    State* currentState;
    states state;
    State* allStates[7];

    PathTracking path;
    PositionDrive drive;
    IMU imu;
    IMUEncoderPosition positionSystem;
    Image img;
    CaffeNetwork net;
    TrafficSignRecognition trafficSign;
    ObstacleDetection obstacleDetection;

    std::mutex crossLock;

    // GPS gps;
    // GPSConnection gps;

    Timer t;
    Timer t1;
    Timer t2;

    bool idle;
    bool cross;
    bool exitCross;
    bool stopSign;
    bool stopCross;
    bool exitStop;
    bool obstacle;
    bool exitObstacle;
    bool laneFollow;
    bool parkingSign;
    bool exitParking;
    bool pathFinished;
};

#endif // BEHAVIOUR_TREE