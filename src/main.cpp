#if 0
#include "serialhandler.h"
#include <unistd.h>
#include <messageconverter.h>
#include <thread>
#include "gpsconnection.h"
#include "gpsdata.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <json/json.h>

//#include <boost/algorithm/string/classification.hpp>
//#include <boost/algorithm/string/split.hpp>

#include "socket.h"
#include "map.h"
#include <regex>
#include <fstream>
#include <streambuf>
#include <functional>
#include <stdio.h>
#include <string>
// #include "path.h"
// #include "vec2.h"
#include "pathtracking.h"


//void run_cmd( const std::string& cmd, std::vector<std::string>& out ){
//    FILE*  fp;
//    const int sizebuf=1234;
//    char buff[sizebuf];
//    out = std::vector<std::string> ();
//    std::cout << "execute cmd: " << cmd << std::endl;
//    if ((fp = popen (cmd.c_str (), "r"))== NULL){
//        std::cout << "Cannot execute" << std::endl;
//        exit(-1);
//    }
//    std::string cur_string = "";
//    while (fgets(buff, sizeof (buff), fp)) {
//        cur_string += buff;
//        std::cout << cur_string;
//    }
//    boost::split(out, cur_string, boost::is_any_of(" "));
////    out.push_back (cur_string.substr (0, cur_string.size() -1));
//    pclose(fp);
//}

void p(std::string c)
{
    std::cout << "merge " << c << std::endl;
}

void ack()
{
    std::cout << "ack" << std::endl;
}

/*
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

//    MessageConverter m;
////    std::cout << m.brake(101.5);
//    SerialHandler *s = new SerialHandler("/dev/ttyACM1", "history.txt");
////    ReadThread t();

//    CallbackEvent c;

//    s->addWaiter("ENPB", p);
//    s->sendEncoderActivation(true);

//    s->addWaiter("PIDA", p);
//    s->sendPidActivation(true);

////    s->addWaiter("BRAK", c, ack, "ack");
//    while(true)
//    {
//        s->sendBrake(10.5);
////        s->sendMove(7.0, 7.0);
//        sleep(2);
////        s->flush();
////        s->read();
//    }

//    s->close();

//    GPSConnection gps(GPSConnection);
    std::vector<std::string> s;
    std::cout << "begin" <<std::endl;
    run_cmd("ip -4 route show default", s);
    std::cout << s.at(0) << std::endl;

    return a.exec();
}
*/

//int main(int argc, char *argv[])
//{
//    Socket socket(AF_INET, SOCK_STREAM, 0);
//    std::cout << "bind: " << socket.bind("", 12346) << std::endl;
//    int optval = 1;
////    std::cout << "setsockopt: " << socket.setSockOpt(SOL_SOCKET, SO_BROADCAST, (char*) &optval) << std::endl;
//    std::cout << "listen: " <<  socket.listen(2) << std::endl;
////    std::cout << "accept: " << socket.accept() << std::endl;
//    Socket* newSocket = socket.accept();
////    socket.close();

//    std::string buf;

//    while (true)
//    {
//        buf.reserve(4096);
////        memset(buf, 0, 4096);

//        int bytesReceived = newSocket->recv(buf, 4096);

//        if (bytesReceived == 0)
//        {
//            std::cout << "Client disconnected" << std::endl;
//            break;
//        }

//        std::cout << buf << std::endl;

//        newSocket->send(buf);
//    }

//    newSocket->close();

//    return 0;
//}

/*
using namespace std;

int main()
{
    // Create a socket
    int listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening == -1)
    {
        cerr << "Can't create a socket! Quitting" << endl;
        return -1;
    }

    // Bind the ip address and port to a socket
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000);
    inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);

    bind(listening, (sockaddr*)&hint, sizeof(hint));

    // Tell Winsock the socket is for listening
    listen(listening, SOMAXCONN);

    // Wait for a connection
    sockaddr_in client;
    socklen_t clientSize = sizeof(client);

    int clientSocket = accept(listening, (sockaddr*)&client, &clientSize);

    char host[NI_MAXHOST];      // Client's remote name
    char service[NI_MAXSERV];   // Service (i.e. port) the client is connect on

    memset(host, 0, NI_MAXHOST); // same as memset(host, 0, NI_MAXHOST);
    memset(service, 0, NI_MAXSERV);

    if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
    {
        cout << host << " connected on port " << service << endl;
    }
    else
    {
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
        cout << host << " connected on port " << ntohs(client.sin_port) << endl;
    }

    // Close listening socket
    close(listening);

    // While loop: accept and echo message back to client
    char buf[4096];

    while (true)
    {
        memset(buf, 0, 4096);

        // Wait for client to send data
        int bytesReceived = recv(clientSocket, buf, 4096, 0);
        if (bytesReceived == -1)
        {
            cerr << "Error in recv(). Quitting" << endl;
            break;
        }

        if (bytesReceived == 0)
        {
            cout << "Client disconnected " << endl;
            break;
        }

        cout << string(buf, 0, bytesReceived) << endl;

        // Echo message back to client
        send(clientSocket, buf, bytesReceived + 1, 0);
    }

    // Close the socket
    close(clientSocket);

    return 0;
}
*/

//int main(int argc, char *argv[])
//{
//    Socket socket(AF_INET, SOCK_DGRAM, 0);
//    std::cout << "bind: " << socket.bind("", 12346) << std::endl;
//    int optval = 1;
//    std::cout << "setsockopt: " << socket.setSockOpt(SOL_SOCKET, SO_BROADCAST, (char*) &optval) << std::endl;
//    std::cout << "setsockopt: " << socket.setSockOpt(SOL_SOCKET, SO_REUSEADDR, (char*) &optval) << std::endl;
////    std::cout << "listen: " <<  socket.listen(2) << std::endl;
////    std::cout << "accept: " << socket.accept() << std::endl;
////    Socket* newSocket = socket.accept();
////    socket.close();

//    std::string buf;
//    buf.reserve(1500);
//    sockaddr* clientAddress;
//    int bytesReceived = socket.recvfrom(buf, 1500, 0, clientAddress, 5);
//    std::cout << "bytes received: " << bytesReceived << std::endl;

//    std::cout << buf << std::endl;

////    while (true)
////    {
////        buf.reserve(4096);
//////        memset(buf, 0, 4096);

////        int bytesReceived = newSocket->recv(buf, 4096);

////        if (bytesReceived == 0)
////        {
////            std::cout << "Client disconnected" << std::endl;
////            break;
////        }

////        std::cout << buf << std::endl;

////        newSocket->send(buf);
////    }

////    newSocket->close();

//    return 0;
//}

// TODO
int main(int argc, char *argv[])
{
    PathTracking path("NOD1", "NOD133");
    //    GPSConnection* g = new GPSConnection(4, 12346);
        GPSConnection *g = new GPSConnection(1, 12346);
    //    g.run();
        // GPSData gpsData = GPSData(1, std::complex<double>(0,0), std::complex<double>(0,0));
        std::thread run (&GPSConnection::getServer, g);
        std::thread getPoz (&GPSConnection::getPositionData, g, path.globalPosition_);

        std::thread running (&PathTracking::update_thread, path); 
        run.join();
        getPoz.join();

    // TODO
    ////    std::string msg = "id:4;Pos:(-99.80+-6.28j);Azm:(-99.21+-12.53j)";
    //    std::string msg = "Pos:(-99.21+-12.53j)";

    //    const std::string COMPLEX_REGEX = "([+-]?[0-9]*[.]?[0-9]+)[+]([+-]?[0-9]*[.]?[0-9]+)j";
    //    const std::string NUMBER_REGEX = "[0-9]+";
    //    const std::string ID_REGEX = "id:" + NUMBER_REGEX;
    //    const std::string POSITION_REGEX = "Pos:[\(]" + COMPLEX_REGEX + "[\)]";
    //    const std::string ORIENTATION_REGEX = "Azm:[\(]" + COMPLEX_REGEX + "[\)]";
    //    const std::string REGEX = ID_REGEX + POSITION_REGEX + ORIENTATION_REGEX;
    ////    const std::regex r("id:([0-9]);Pos:[\(]([+-]?[0-9]*[.]?[0-9]+)[+]([+-]?[0-9]*[.]?[0-9]+)j[\)];Azm:[\(]([+-]?[0-9]*[.]?[0-9]+)[+]([+-]?[0-9]*[.]?[0-9]+)j[\)]");
    //    const std::regex r(POSITION_REGEX);
    //    std::smatch sm;
    //    if (std::regex_search(msg, sm, r))
    //    {
    //        for (int i = 0; i < sm.size(); i++)
    //        {
    //            std::cout << sm[i] << std::endl;
    //        }
    //    }

    //    const std::regex r("id:([0-9]);Pos:[\(]([+-]?[0-9]*[.]?[0-9]+)[+]([+-]?[0-9]*[.]?[0-9]+)j[\)];Azm:[\(]([+-]?[0-9]*[.]?[0-9]+)[+]([+-]?[0-9]*[.]?[0-9]+)j[\)]");
    //    std::smatch sm;
    //    if (std::regex_search(msg, sm, r))
    //    {
    //        for (int i = 0; i < sm.size(); i++)
    //        {
    //            std::cout << sm[i] << std::endl;
    //        }
    //    }

    //    std::string msg = "in(-99.21+-12.53j";

    //    const std::regex r("in[\(]([+-]?[0-9]*[.]?[0-9]+)[+]([+-]?[0-9]*[.]?[0-9]+)j\)]");
    //    std::smatch sm;
    //    if (std::regex_search(msg, sm, r))
    //    {
    //        for (int i = 0; i < sm.size(); i++)
    //        {
    //            std::cout << sm[i] << std::endl;
    //        }
    //    }

    // const std::string JSON_PATH = "/home/mihai/Workspace/BOSCH_2019/Holistic2_v2/master/resources/Map.json";
    const std::string JSON_PATH = "/home/mihai/Workspace/BOSCH_2019/Holistic2_v2/master/resources/Map.json";
    // std::string line;
    // std::string mapJson;
    // std::ifstream myfile(JSON_PATH);
    // if (myfile.is_open())
    // {
    //     std::cout << "open" << std::endl;
    //     while (getline(myfile, line))
    //     {
    //         mapJson += line;
    //     }
    // }
    // else
    // {
    //     std::cout << "not open" << std::endl;
    // }

// TODO
    // Map m(JSON_PATH);
    // Map::getInstance();
    // Map m;
    // m->getInstance();
    Map::getInstance().linkNodes();
    // for (auto & node : m.nodesMap)
    // {
    //     std::cout << node.second->name() << std::endl;
    //     // std::cout << node.second->name() << std::cout;
    // }

    // std::cout << Map::getInstance().nodesSet["NOD0"]->outAhead << std::endl;
    // std::cout << Map::getInstance()["NOD1"] << std::endl;
    NodesVect nodes = Map::getInstance()[std::complex<double>(0, 4.5)];
    // for (auto const &node : nodes)
    // {
    //     std::cout << "from coord: " << *node << std::endl;
    // }
    // NodesVect succ;
    // Map::getInstance()["NOD24"].successors(succ);
    
    // for (auto const &node : succ)
    // {
    //     std::cout << "from succ: " << *node << std::endl;
    // }

    // for (auto const  &edge : Map::getInstance().edgesSet)
    // {
    //     std::cout << *edge.second << std::endl;
    // }

    // PathTracking p(std::complex<double>(0, 4.5), std::complex<double>(7.65,3.15));

    std::cout <<  *Map::getInstance().closest(std::complex<double>(0, 4.8)) << std::endl;

    // Path p(Map::getInstance().nodePointer("NOD1"), Map::getInstance().nodePointer("NOD133"));
    // for (auto &node : p.pathSet)
    // {
    //     std::cout << "path: " << *node << std::endl;
    // }

    // Path p("NOD1", "NOD133");
    // // for (auto &node : p.pathSet)
    // // {
    // //     std::cout << "path: " << *node << std::endl;
    // // }
    // // std::cout << *p.closest(std::complex<double>(1.4, 2)) << std::endl;

    // for (auto &edge : p.edgesSet)
    // {
    //     std::cout << "path: " << *edge.second << std::endl;
    // }

    // PathTracking path("NOD1", "NOD133");
    // // std::thread running (&PathTracking::update_thread, path);
    // // path.run();
    // std::thread run1 (&GPSConnection::getServer, path.g_);
    // std::thread getPoz (&GPSConnection::getPositionData, path.g_, path.globalPosition_);
    // sleep(6);
    // std::thread run2 (&PathTracking::update_thread, path);

    // run1.join();
    // getPoz.join();
    // run2.join();
    // running.join();
    
    // for (auto &edge : p.edgePath)
    // {
    //     std::cout << "path: " << *edge << std::endl;
    // }

    // std::cout << *p.currentEdge(std::complex<double>(2.5, 2.3)) << std::endl;
    // std::cout << p.displacement(std::complex<double>(2.5, 2.3)) << std::endl;
    

    // // Vec2 v(std::complex<double>(0,0), std::complex<double>(0,4));
    // // std::cout << v.distance(std::complex<double>(-1, 3)) << std::endl;
    // // const std::complex<double> point(2,1);
    // // std::cout << v.direction(point) << std::endl;

    return 0;
}

//int main(int argc, char *argv[])
//{
//    std::vector<std::string> s;
//    std::cout << "begin" << std::endl;
//    run_cmd("ip -4 route show default", s);
////    std::cout << s.at(0) << std::endl;
////    std::vector<std::string> splitted;
////    boost::split(splitted, s.at(0), boost::is_any_of(" "));
////    std::cout << splitted.at(2) << std::endl;

//    Socket socket(AF_INET, SOCK_DGRAM, 0);
//    std::cout << "connect: " << socket.connect(s.at(2), 0) << std::endl;
////    std::cout << "bind: " << socket.bind("", 54000) << std::endl;
////    int optval = 1;
////    std::cout << "setsockopt: " << socket.setSockOpt(SOL_SOCKET, SO_BROADCAST, (char*) &optval) << std::endl;
//////    std::cout << "listen: " <<  socket.listen(2) << std::endl;
//////    std::cout << "accept: " << socket.accept() << std::endl;
//////    Socket* newSocket = socket.accept();
//////    socket.close();

////    std::string buf;
////    buf.reserve(1500);
////    sockaddr* clientAddress;
////    int bytesReceived = socket.recvfrom(buf, 1500, 0, clientAddress);
////    std::cout << "bytes received: " << bytesReceived << std::endl;

////    std::cout << buf << std::endl;

////    while (true)
////    {
////        buf.reserve(4096);
//////        memset(buf, 0, 4096);

////        int bytesReceived = newSocket->recv(buf, 4096);

////        if (bytesReceived == 0)
////        {
////            std::cout << "Client disconnected" << std::endl;
////            break;
////        }

////        std::cout << buf << std::endl;

////        newSocket->send(buf);
////    }

////    newSocket->close();

//    return 0;
//}

#endif