#include "statemachine.h"

Machine::Machine() :    idle(true), cross(false), exitCross(false), stopSign(false), 
                        exitStop(false), obstacle(false), exitObstacle(false), 
                        laneFollow(false), parkingSign(false), exitParking(false),
                        pathFinished(false), stopCross(false), imu("/media/pi/PiDrive1/Holistic2_v4/master/lib/imu/","RTIMULib"),
                        positionSystem(NULL), carControl(), path("NOD1", "NOD134"), drive(NULL, &path),
                        net("resources/caffemodel/model_test_small.prototxt", "resources/caffemodel/CAR_iter_4000.caffemodel"),
                        // net("resources/caffemodel/model.prototxt", "/media/pi/PiDrive1/Dataset/trained/CAR_iter_4000.caffemodel"),
                        trafficSign(), obstacleDetection(), 
                        // img("/media/pi/PiDrive1/Workspace/TraffisSign/imgtovid.avi"),
                        // img("/media/pi/PiDrive1/Workspace/output_1555424602.779776.avi"),
                        img(), waitForGPS(false),
                        t("image", 50000, std::bind(&Image::nextFrame, &img, (void*)(NULL))),
                        t1("sign", 100000, std::bind(&TrafficSignRecognition::trafficSignInImageA, &trafficSign, (void*)(NULL))),
                        t2("obstacle", 50000, std::bind(&ObstacleDetection::detectByGreenThresholdNoTransform, &obstacleDetection, (void*)(NULL))), gps(4, 12346)
{
    // std::shared_ptr<State> idleState = std::shared_ptr<IdleState>();
    // std::shared_ptr<State> pathPlanning = std::shared_ptr<PathPlanningState>();
    // std::shared_ptr<State> laneFollow = std::shared_ptr<LaneFollowState>();
    // std::shared_ptr<State> overtake = std::shared_ptr<OvertakeState>();
    // std::shared_ptr<State> parking = std::shared_ptr<ParkingState>();
    // std::shared_ptr<State> stop = std::shared_ptr<StopSignState>();
    // std::shared_ptr<State> crossroad = std::shared_ptr<CrossroadState>();

    State* idleState = new IdleState();
    State* pathPlanning = new PathPlanningState();
    State* laneFollow = new LaneFollowState();
    State* overtake = new OvertakeState();
    State* parking = new ParkingState();
    State* stop = new StopSignState();
    State* crossroad = new CrossroadState();

    this->allStates[IDLE] = idleState;
    this->allStates[PATH_PLANNING] = pathPlanning;
    this->allStates[CROSSROAD] = crossroad;
    this->allStates[STOP] = stop;
    this->allStates[LANE_FOLLOW] = laneFollow;
    this->allStates[OVERTAKE] = overtake;
    this->allStates[PARKING] = parking;

    {
        std::lock_guard<std::mutex> lock(this->crossLock);
        path.addToCrossRoad(std::complex<double>(3.825, 2.475));
        path.addToCrossRoad(std::complex<double>(1.125, 4.725));
        path.addToCrossRoad(std::complex<double>(1.125, 2.475));
        path.addToCrossRoad(std::complex<double>(3.825, 0.225));
    }

    gps.start();
    sleep(4.0);

    // imu.start();
    sleep(1);
    carControl.enableEncoder(std::bind(&IMUEncoderPosition::readEncoder, &positionSystem, std::placeholders::_1));
    imu.attachObserver(&positionSystem);
    gps.attachObserver(&positionSystem);
    positionSystem.attachObserver(&drive);
    positionSystem.attachObserver(&path);
    // t = Timer("image", 50000, std::bind(&Image::nextFrame, &img, (void*)(NULL)));
    t.start();
    img.attachObserver(&net);
    img.attachObserver(&trafficSign);
    img.attachObserver(&obstacleDetection);

    // t1 = Timer("sign", 500000, std::bind(&TrafficSignRecognition::trafficSignInImageA, &trafficSign, (void*)(NULL)));
    t1.start();
    t2.start();
    // std::thread signDetection(&TrafficSignRecognition::trafficSignInImage, &trafficSign, parkingSign, stopSign);

    state = IDLE;
}

void* Machine::switchState(void*)
{
    // while (true)
    {
        stopSign = trafficSign.stopSignBool;
        // parkingSign = trafficSign.parkingSignBool;
        obstacle = obstacleDetection.obstacleVal;
        // std::cout << "\t\t\t\tDIST_CROSS: " << path.distanceToCross() << std::endl;
        // std::cout << "\t\tDistToGoal: " << path.distanceToCross() << std::endl;
        if (path.distanceToCross() <= 1.0)
        {
            cross = true;
        }
        else
        {
            cross = false;
        }
        if (stopSign)
        {
            stopCross = true;
        }
        if (path.distanceToGoal() < 0.2)
        {
            parkingSign = true;
        }
        else
        {
            parkingSign = false;
        }

        // if (stopSign)
        // {
        //     state = STOP;
        // }

        // std::cout << "stopS: " << stopSign << "\r" << std::endl;
        // std::cout << "parkS: " << parkingSign << "\r" << std::endl;
        // std::cout << "obstacle: " << obstacle << "\r" << std::endl;
        switch (state)
        {
            case IDLE:
            {
                // currentState = this->allStates[IDLE];
                // state = PATH_PLANNING;
                break;
            }
            case PATH_PLANNING:
            {       
                if (cross &&
                    !exitCross)         { state = CROSSROAD; }
                // else if (stopSign &&
                //         !exitStop)      { state = STOP; }
                else if (obstacle &&
                        !exitObstacle)  { state = OVERTAKE; }
                else if (parkingSign &&
                        !exitParking)   { state = PARKING; }
                // else if (pathFinished)  { state = LANE_FOLLOW; }
                // else                    { state = PATH_PLANNING; }
                else                    { state = LANE_FOLLOW; }
                break;
            }
            case CROSSROAD:
            {
                if (exitCross)          { state = LANE_FOLLOW; }
                else                    { state = CROSSROAD; }
                // else if (   stopSign &&
                //             !exitStop)  { state = STOP; }
                // else                    { state = CROSSROAD; }
                // if (   stopSign &&
                //         !exitStop)  { state = STOP; }
                // else                { state = LANE_FOLLOW; }
                // state = LANE_FOLLOW;
                break;
            }
            // case STOP:
            // {
            //     // drive.done = true;
            //     if (exitStop)           { state = CROSSROAD; } // TODO: comm
            //     // else                    { state = STOP; }
            //     // state = CROSSROAD; // TODO: uncomm
            //     break;
            // }
            case LANE_FOLLOW:
            {
                // if (stopSign &&
                //     !exitStop)          { state = STOP; }
                if (parkingSign &&
                        !exitParking)   { state = PARKING; }
                else if (obstacle &&
                        !exitObstacle)  { state = OVERTAKE; }
                else if (cross &&
                        !exitCross)     { state = CROSSROAD; }
                else                    { state = LANE_FOLLOW; }
                break;
            }
            case OVERTAKE:
            {
                if (exitObstacle)       { state = LANE_FOLLOW; }
                else if (cross &&
                        !exitCross)     { state = CROSSROAD; }
                else if (parkingSign &&
                        !exitParking)   { state = PARKING; }
                // else if (stopSign &&
                //         !exitStop)      { state = STOP; }
                else                    { state = OVERTAKE; }
                break;
            }
            case PARKING:
            {
                sleep(1.0);
                // if (exitParking)        { state = PATH_PLANNING; }
                // else                    { state = PARKING; }
                // break;
                state = LANE_FOLLOW;
            }
        }
        // std::cout << state << std::endl;
    }
}

void* Machine::run(void*)
{
    while (true)
    {
        switch (state)
        {
            // case IDLE:          { currentState = allStates[IDLE]; break; }
            // case PATH_PLANNING: { currentState = allStates[PATH_PLANNING]; break; }
            // case CROSSROAD:     { currentState = allStates[CROSSROAD]; break; }
            // case STOP:          { currentState = allStates[STOP]; break; }
            // case PARKING:       { currentState = allStates[PARKING]; break; }
            // case OVERTAKE:      { currentState = allStates[OVERTAKE]; break; }
            // case LANE_FOLLOW:   { currentState = allStates[LANE_FOLLOW]; break; }
            case IDLE:
            {
                std::cout << "IDLE " << std::endl;
                sleep(2.0);
                carControl.enablePID();
                sleep(2.0);
                while (gps.position.getPosition() == std::complex<double>(0,0) && this->waitForGPS)
                {
                    sleep(0.2);
                }
                state = LANE_FOLLOW;
                break;
            }
            case PATH_PLANNING:
            {
                // if (!pathFinished)
                {
                    std::cout << "PATH " << std::endl;
                    // this->path = PathTracking("NOD1", "NOD134");
                    pathFinished = true;
                }
                break;
            }
            case CROSSROAD:
            {
                std::cout << "exit_cross: " << exitCross << std::endl;
                if (!exitCross)
                {
                    if (stopCross)
                    {
                        carControl.brake(0.0);
                        sleep(2.0);
                    }
                    stopCross = false;
                    std::cout << "CROSSROAD " << std::endl;
                    exitCross = false;
                    std::thread driveThread(&PositionDrive::drivePoints, &drive, 6);
                    // pthread_mutex_lock(&drive.lock_angle);
                    while (!drive.done)
                    {
                        std::cout << "CROSS WHILE " << std::endl;
                        carControl.move(0.2, drive.angle);
                        // pthread_mutex_trylock(&drive.lock_angle);
                        // if (pthread_mutex_unlock(&drive.lock_angle) != 0)
                        // {
                        //     std::cout << "error lock" << std::endl;
                        // }
                        drive.nsleep(100);
                        std::cout << "Sleep" << std::endl;
                    }
                    // pthread_mutex_trylock(&drive.lock_angle);
                    // if (pthread_mutex_unlock(&drive.lock_angle) != 0)
                    // {   
                    //     std::cout << "error lock" << std::endl;
                    // }
                    driveThread.join();
                    std::cout << "exit" << std::endl;
                    exitCross = true;
                    exitStop = false;
                    drive.done = false;
                    stopCross = false;
                    if (path.crossRoad.size() > path.crossIndex + 1)
                    {
                        {
                            std::lock_guard<std::mutex> lock(this->crossLock);
                            path.crossRoad.erase(path.crossRoad.begin() + path.crossIndex, path.crossRoad.begin() + path.crossIndex + 1);
                        }
                    }
                }
                break;
            }
            // case STOP:
            // {
            //     // if (!exitStop)
            //     {
            //         std::cout << "STOP " << std::endl;
            //         exitStop = false;
            //         carControl.brake(0.0);
            //         sleep(2.0);
            //         exitStop = true;
            //     }
            //     break;
            // }
            case PARKING:
            {
                // if (!exitParking)
                {
                    std::cout << "PARKING " << std::endl;
                    exitParking = false;
                    // TODO: parking
                    carControl.brake(0.0);
                    sleep(1.0);
                    path = PathTracking("NOD76", "NOD0");
                    {
                        std::lock_guard<std::mutex> lock(this->crossLock);
                        path.addToCrossRoad(std::complex<double>(3.825, 2.475));
                        path.addToCrossRoad(std::complex<double>(1.125, 4.725));
                        path.addToCrossRoad(std::complex<double>(1.125, 2.475));
                        path.addToCrossRoad(std::complex<double>(3.825, 0.225));
                        exitParking = true;
                    }
                }
                break;
            }
            case OVERTAKE:
            {
                if (!exitObstacle)
                {
                    std::cout << "OVERTAKE " << std::endl;
                    exitObstacle = false;
                    std::vector<std::complex<double>> vect;
                    this->path.nextObstaclesNodes(this->path.pathPosition().getPosition(), 5, vect);
                    std::thread driveThread(&PositionDrive::drivePointsObs, &drive, std::ref(vect));
                    // pthread_mutex_lock(&drive.lock_angle);
                    while (!drive.done)
                    {
                        std::cout << "OVERTAKE WHILE " << std::endl;
                        carControl.move(0.2, drive.angle);
                        // pthread_mutex_unlock(&drive.lock_angle);
                        drive.nsleep(100);
                        // std::cout << "Sleep" << std::endl;
                    }
                    // pthread_mutex_unlock(&drive.lock_angle);
                    driveThread.join();
                    std::cout << "exit" << std::endl;
                    carControl.brake(0.0);
                    exitObstacle = true;
                    exitCross = false;
                    exitParking = false;
                    exitStop = false;
                    drive.done = false;
                }
                break;
            }
            case LANE_FOLLOW:
            {
                exitCross = false;
                std::cout << "LANE " << std::endl;
                while (state == LANE_FOLLOW)
                {
                    // std::cout << "\t\t\t\tDIST_CROSS: " << path.distanceToCross() << std::endl;
                    // std::cout << "\t\t\tLANE_DIST: " << path.displacement() << std::endl;
                    // float dir = -1 ? (path.displacement() < 0) : 1;
                    // if (abs(path.displacement()) > 0.10)
                    // {
                    //     carControl.move(0.2, dir*23.0);
                    //     std::cout << "LANE with margin: " << dir*23.0 << std::endl;
                    // }
                    // else
                    std::cout << "\t\tDistToPark: " << path.distanceToGoal() << std::endl;
                    {
                        std::cout << "LANE: " << 23.0*(2.0*net.infer()-1.0) << std::endl;
                        carControl.move(0.2, 23.0*(2.0*net.infer()-1.0));
                        sleep(0.07);
                    }
                }
                exitStop = false;
                exitParking = false;
                exitObstacle = false;
                exitCross = false;
                // std::cout << "exit_cross from Lane: " << exitCross << std::endl;
                break;
            }
        }
    }
    // currentState->run();
}