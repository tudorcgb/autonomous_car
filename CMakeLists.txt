# Project name and version
cmake_minimum_required(VERSION 3.0.0)
project(drive VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 14)
# set(CMAKE_CXX_FLAGS "-fpermissive -O3 -lcaffe -L/home/mihai/Programs/caffe-master/build/lib -I/home/mihai/Programs/caffe-master/include -L/usr/local/lib -L/usr/local/cuda/lib64 -L/usr/local/cuda/lib -lcublas -lglog -lgflags -lprotobuf -lleveldb -lsnappy -llmdb -lboost_system -lhdf5_hl -lhdf5 -lm -lopencv_core -lopencv_highgui -lopencv_imgproc -lboost_thread -lstdc++ -lcblas -latlas -L/home/blcv/LIB/caffe_bn/build/lib/ -lcaffe -I/usr/local/cuda-10.1/include")
# set(CMAKE_CXX_FLAGS "-fpermissive -O3 -L/opt/movidius/caffe/build/lib -I/opt/movidius/caffe/include -I/opt/movidius/caffe/build/include -L/usr/local/cuda/lib64 -L/usr/local/cuda/lib -lcublas -lglog -lgflags -lprotobuf -lleveldb -lsnappy -llmdb -lboost_system -lhdf5_hl -lhdf5 -lm -lopencv_core -lopencv_highgui -lopencv_imgproc -lboost_thread -lstdc++ -lcblas -latlas -lcaffe -I/usr/local/cuda-10.1/include")
set(CMAKE_CXX_FLAGS "-fpermissive -O3 -Wl,-rpath -Wl,LIBDIR")

# Enable testing
include(CTest)
enable_testing()

# =============SOURCES==============
set(SOURCE_FILES 
    ./src/main.cpp 
    ./src/gps/gpsconnection.cpp 
    ./src/gps/gpsdata.cpp 
    ./src/serialhandler/filehandler.cpp 
    ./src/serialhandler/messageconverter.cpp 
    ./src/serialhandler/serialhandler.cpp 
    ./src/serialhandler/findfile.cpp
    ./src/socket/socket.cpp
    ./src/map/map.cpp
    ./src/map/node.cpp
    ./src/map/nodeset.cpp
    ./src/map/path.cpp
    ./src/map/edge.cpp
    ./src/map/edgeset.cpp
    ./src/map/vec2.cpp
    ./src/map/pathtracking.cpp
    ./src/map/heuristics.cpp
    ./src/imagemapping/point.cpp
    ./src/imagemapping/line.cpp
    ./src/imagemapping/imagecoord.cpp
    ./src/imagemapping/maprepresentation.cpp
    ./src/carcontrol/carcontrol.cpp
    ./src/image/image.cpp
    ./src/timer/timer.cpp
    ./src/lanedetection/neuralnetwork/caffenetwork.cpp
    ./src/lanedetection/neuralnetwork/movidiusnetwork.cpp
    ./src/trafficsign/TrafficSignRecognition.cpp
    # ./src/drivecomands/VectorAngle.cpp
    ./src/statemachine/crossroadstate.cpp
    ./src/statemachine/idlestate.cpp
    ./src/statemachine/lanefollow.cpp
    ./src/statemachine/overtakestate.cpp
    ./src/statemachine/parking.cpp
    ./src/statemachine/pathplanningstate.cpp
    ./src/statemachine/stopsignstate.cpp
    ./src/statemachine/state.cpp
    ./src/statemachine/statemachine.cpp
    ./src/imu/imu.cpp
    ./src/imuencoderposition/imuencoderposition.cpp
    ./src/positiondrive/positiondrive.cpp
    ./src/obstacledetection/ObstacleDetection.cpp

    # ./src/testPoint.cpp
    # ./src/testLine.cpp
    # ./src/imagecoordTest.cpp
    # ./src/mapreprTest.cpp
    # ./src/moveTest.cpp
    # ./src/moveTest2.cpp
    # ./src/gpsTest.cpp
    # ./src/imageTest.cpp
    # ./src/timerTest.cpp
    # ./src/cnnTest.cpp
    # ./src/nodesTest.cpp
    # ./src/movidiusTest.cpp
    # ./src/trafficSignTest.cpp
    # ./src/stateTest.cpp
    # ./src/IMUTest.cpp
    # ./src/driveTest.cpp
    # ./src/cnnTest2.cpp
    ./src/mainState.cpp
    # ./src/obstacleTest.cpp
    
    

    ./include/serialhandler/connexc.h
    # ./lib/serial/src/serial.cc
)
# ===================================

#  =============INCLUDES=============
include_directories( 
    ./include
    ./include/gps
    ./include/serialhandler
    ./include/socket
    ./include/map
    ./include/imagemapping
    ./include/carcontrol
    ./include/observer
    ./include/image
    ./include/timer
    ./include/lanedetection
    ./include/lanedetection/neuralnetwork
    ./include/trafficsign
    ./include/statemachine
    ./include/imu
    ./include/imuencoderposition
    ./include/positiondrive
    ./include/drivecomands
    ./include/obstacledetection
    ./src
    ./lib/serial/include
    /usr/local/cuda-10.1/include
    /usr/local/include/mvnc2
    # /usr/include/caffe
    ${JSONCPPPATH}/include
    ${OpenCV_INCLUDE_DIRS}
    # ${Caffe_INCLUDE_DIRS}
)
add_subdirectory(${CMAKE_SOURCE_DIR}/lib/serial)
# ===================================

# ========FILES TO BE COMPILED=======
add_executable( ${PROJECT_NAME} ${SOURCE_FILES} )
# ===================================



# ========ADDED BY CMAKE TOOL========
set( CPACK_PROJECT_NAME ${PROJECT_NAME} )
set( CPACK_PROJECT_VERSION ${PROJECT_VERSION} )
include( CPack )
# ===================================

# =============LINKING===============
# Add link to pthread
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)

set( OpenCV_DIR "/usr/lib/opencv" )

FIND_PACKAGE(Threads REQUIRED)  
FIND_PACKAGE(PkgConfig REQUIRED)
# FIND_PACKAGE(OpenCV REQUIRED COMPONENTS core imgproc highgui)
FIND_PACKAGE(OpenCV REQUIRED)

# FIND_PACKAGE(Caffe REQUIRED)

pkg_check_modules(JSONCPP jsoncpp)
link_libraries(${JSONCPP_LIBRARIES})

# pkg_check_modules(SERIAL serial)
# link_libraries(${SERIAL_LIBRARIES})

# Add libserial.so
# add_library(serial SHARED IMPORTED)
# set_target_properties(serial PROPERTIES
#     IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/lib/serial/build/libserial.so"
#     INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/lib/serial/include"
# )

add_dependencies (${PROJECT_NAME} serial)

# Linking
target_link_libraries( ${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT} )
# target_link_libraries( ${PROJECT_NAME} serial )
target_link_libraries( ${PROJECT_NAME} ${JSONCPP_LIBRARIES} )
target_link_libraries( ${PROJECT_NAME} serial )
target_link_libraries( ${PROJECT_NAME} ${OpenCV_LIBS} )
target_link_libraries( ${PROJECT_NAME} stdc++fs )
target_link_libraries( ${PROJECT_NAME} mvnc )
TARGET_LINK_LIBRARIES(${PROJECT_NAME} RTIMULib)
# target_link_libraries( ${PROJECT_NAME} ${Caffe_LIBS} )
# target_link_libraries( ${PROJECT_NAME} serial )
# ===================================

find_package(OpenCV REQUIRED)


set(Caffe_DIR "/opt/movidius/caffe/build/install/share/Caffe")  # caffe-wy caffe

# for CaffeConfig.cmake/ caffe-config.cmake
find_package(Caffe REQUIRED)
# offical caffe  : There is no Caffe_INCLUDE_DIRS and Caffe_DEFINITIONS
# refinedet caffe: OK.

add_definitions(${Caffe_DEFINITIONS})

MESSAGE( [Main] " Caffe_INCLUDE_DIRS = ${Caffe_INCLUDE_DIRS}")
MESSAGE( [Main] " Caffe_DEFINITIONS = ${Caffe_DEFINITIONS}")
MESSAGE( [Main] " Caffe_LIBRARIES = ${Caffe_LIBRARIES}") # caffe
# MESSAGE( [Main] " Caffe_CPU_ONLY = ${Caffe_CPU_ONLY}")
MESSAGE( [Main] " Caffe_CPU_ONLY = ON")
MESSAGE( [Main] " Caffe_HAVE_CUDA = ${Caffe_HAVE_CUDA}")
MESSAGE( [Main] " Caffe_HAVE_CUDNN = ${Caffe_HAVE_CUDNN}")


include_directories(${Caffe_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} 
    ${OpenCV_LIBS}
    ${Caffe_LIBRARIES}
)

# find_package(Qt5Core)
# target_link_libraries(${PROJECT_NAME} Qt5::Core)