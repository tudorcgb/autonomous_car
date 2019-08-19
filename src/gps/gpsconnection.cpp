#ifndef GPSCONNECTION_H
#include "gpsconnection.h"
#endif

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <arpa/inet.h>
#include <time.h>
#include <regex>

void run_cmd( const std::string& cmd, std::vector<std::string>& out ){
    FILE*  fp;
    const int sizebuf=1234;
    char buff[sizebuf];
    out = std::vector<std::string> ();
    if ((fp = popen (cmd.c_str (), "r"))== NULL){
        std::cout << "Cannot execute" << std::endl;
        exit(-1);
    }
    std::string cur_string = "";
    while (fgets(buff, sizeof (buff), fp)) {
        cur_string += buff;
    }
    boost::split(out, cur_string, boost::is_any_of(" "));
    pclose(fp);
}


GPSConnection::GPSConnection(int carID, uint16_t negotiationPort)
{
    run_cmd("ip -4 route show default", this->gateway);
    std::string gwIP = this->gateway.at(2);

    this->socket = Socket(AF_INET, SOCK_DGRAM, 0);
    this->socket.connect(gwIP, 0);
    this->hostName = this->socket.getHostName();
    this->hostIP = this->socket.getHostIP();
    this->socket.getSockName((struct sockaddr*)&this->hostAddress);
    this->socket.close();

    std::cout << "hostname: " << this->hostName << std::endl;
    std::cout << "host IP: " << this->hostIP << std::endl;

    this->thisCarId = carID;
    this->carId = this->thisCarId;
    this->negotiationPort = negotiationPort;
    this->carSubscriptionPort = this->negotiationPort + 2;
    this->carCommunicationPort = this->carSubscriptionPort + 2;
    this->maxWaitTimeForServer = 10;
    // this->carPos = std::complex<double>(0,0);
    // this->carOrientation = std::complex<double>(0,0);
    this->position = GPSData(   this->carId, 
                                std::complex<double>(0,0),
                                std::complex<double>(0,0)
                            );
    this->newServerIP = false;
    this->startUp = true;
    this->GSocketPoz = Socket(AF_INET, SOCK_STREAM, 0);
    this->idSentFlag = false;
    this->runCarClient = true;
    this->serverIP = "";
    this->className_ = "GPSConnection";
}

GPSConnection& GPSConnection::operator= (const GPSConnection &gps)
{
    this->gateway = gps.gateway;
    this->socket = gps.socket;
    this->hostAddress = gps.hostAddress;
    this->hostIP = gps.hostIP;
    this->hostName = gps.hostName;
    this->thisCarId = gps.thisCarId;
    this->carId = gps.carId;
    this->negotiationPort = gps.negotiationPort;
    this->carSubscriptionPort = gps.carSubscriptionPort;
    this->carCommunicationPort = gps.carCommunicationPort;
    this->maxWaitTimeForServer = gps.maxWaitTimeForServer;
    // this->carPos = gps.carPos;
    // this->carOrientation = gps.carOrientation;
    this->position = gps.position;
    this->newServerIP = gps.newServerIP;
    this->startUp = gps.startUp;
    this->GSocketPoz = gps.GSocketPoz;
    this->idSentFlag = gps.idSentFlag;
    this->runCarClient = gps.runCarClient;
    this->serverIP = gps.serverIP;
    this->className_ = gps.className();
    return *this;
}

void GPSConnection::getServer()
{
    while (this->runCarClient)
    {
        if (this->serverIP == "")
        {
            try
            {
                this->socket = Socket(AF_INET, SOCK_DGRAM, 0);
                this->socket.bind("", this->negotiationPort);
                int optval = 1;
                this->socket.setSockOpt(SOL_SOCKET, SO_BROADCAST, (char*) &optval, sizeof(optval));
                this->socket.setSockOpt(SOL_SOCKET, SO_REUSEADDR, (char*) &optval, sizeof(optval));

                int waitTime = 2 + floor(10*this->maxWaitTimeForServer*((double) rand() / (RAND_MAX)))/10;
                // TODO: t = time(); server = []

                // Listen for server broadcast
                sockaddr_in tmpServerAddress;
                std::string buf = this->socket.recvfrom(1500,
                                                        0,
                                                        (struct sockaddr*)&tmpServerAddress,
                                                        waitTime);
                std::cout << "buf: " << buf << std::endl;
                char addrBuf[IP_SIZE];
                inet_ntop(tmpServerAddress.sin_family,
                          &tmpServerAddress.sin_addr,
                          addrBuf,
                          sizeof(addrBuf));

                // TODO: tmpServerIP must vee deducted from tmpServerAddress
                std::string tmpServerIP = buf;
                std::cout << "addrBuf: " << tmpServerIP << std::endl;

                // Get server IP
                if (tmpServerIP != this->serverIP)
                {
                    // new server
                    this->sendIDToServer(tmpServerIP);
                    this->newServerIP = true;
                    this->serverIP = tmpServerIP;
                    this->startUp = false;
                }
                else
                {
                    // old server
                    if (this->idSentFlag == false)
                    {
                        this->sendIDToServer(tmpServerIP);
                        std::cout << "Subscribe @ GPS server" << std::endl;
                    }
                    this->newServerIP = false;
                    this->socket.close();
                }
                // this->socket.close();
            }
            catch (const std::system_error& e)
            {
                this->serverIP = "";
                if (this->startUp == false && this->serverIP == "" && this->newServerIP == true)
                {
                    this->GSocketPoz.close();
                    // TODO: free GSocketPoz
                    std::cout << "Socket from get position server closed!" << std::endl;
                }
                std::cout << "Not connected to server! IP: " << this->serverIP << "! Error: " << e.what() << std::endl;
                this->socket.close();
            }
        }
        sleep(0.25);
    }
}

void GPSConnection::sendIDToServer(std::string newServer)
{
    /**
      * Sends thisCarID to server for identification
      */
    // Socket s;
    try
    {
        // s = Socket(AF_INET, SOCK_STREAM, 0);
        this->socket = Socket(AF_INET, SOCK_STREAM, 0);
        std::cout   << "Vehicle "
                    << this->thisCarId
                    << " subscribing to GPS server: "
                    << newServer
                    << ": "
                    << this->carSubscriptionPort
                    << std::endl;
        // s.connect(newServer, this->carSubscriptionPort);
        this->socket.connect(newServer, this->carSubscriptionPort);
        sleep(2);
        std::string strID = std::to_string(this->thisCarId);
        // s.send(strID);
        this->socket.send(strID);
        // s.shutdown();
        this->socket.shutdown();
        // s.close();
        this->socket.close();
        std::cout << "Vehicle ID sent to server---------------------" << std::endl;
        this->idSentFlag = true;
    }
    catch (const std::system_error& e)
    {
        std::cout << "Failed to send ID to server, with error: " << e.what();
        // s.close();
        this->socket.close();
        this->idSentFlag = false;
    }
}

// void GPSConnection::getPositionData(GPSData *gpsData)
void GPSConnection::getPositionData()
{
    while (this->runCarClient)
    {
        if (this->serverIP != "")
        {
            if (this->newServerIP == true)
            {
                try
                {
                    this->GSocketPoz.close();
                }
                catch (const std::system_error& e)
                {
                    std::cout << "Previous socket could not be closed." << std::endl;
                }
            }
            if (!this->GSocketPoz.isOpen())
            {
                try
                {
                    std::cout   << "Attempting to create new socket to receive the position from server "
                                << this->hostIP
                                << ": "
                                << this->carCommunicationPort
                                << std::endl;
                    this->GSocketPoz = Socket(AF_INET, SOCK_STREAM, 0);
                    int optval = 1;
                    this->GSocketPoz.setSockOpt(SOL_SOCKET, SO_REUSEADDR, (char*)&optval, sizeof(optval));
                    this->GSocketPoz.bind(this->hostIP, this->carCommunicationPort);
                    this->GSocketPoz.listen(2);

                    this->newServerIP = false;
                }
                catch (const std::system_error& e)
                {
                    std::cout   << "Creating new socket for get position from server "
                                << this->serverIP
                                << " failed with error: "
                                << e.what()
                                << std::endl;
                    sleep(1);
                    this->GSocketPoz.close();
                    // TODO: free GSocketPoz
                }
            }
            if (this->GSocketPoz.isOpen())
            {
                SOCKET_PTR c = nullptr;
                try
                {
                    c = this->GSocketPoz.accept();
                    std::string buf;
                    buf = c->recv(4096);
                    std::cout << "received: " << buf << std::endl;
                    this->msg2data(buf, this->position);
                    // std::cout << *gpsData << std::endl;
                    std::cout << this->position << std::endl;
                    this->notifyObservers();
                    c->close();
                    c.reset();
                }
                catch (const std::system_error& e)
                {
                    if (c != nullptr)
                        c->close();
                    this->GSocketPoz.close();
                    std::cout   << "Receiving position data from server "
                                << this->serverIP
                                << " failed with error: "
                                << e.what()
                                << std::endl;
                }
            }
        }
        sleep(0.25);
    }
}

void GPSConnection::msg2data(std::string msg, GPSData& gpsData)
{
    const int ID_IDX = 1;
    const int POS_REAL_IDX = 2;
    const int POS_IMAG_IDX = 3;
    const int AZM_REAL_IDX = 4;
    const int AZM_IMAG_IDX = 5;

    const std::string COMPLEX_REGEX = "([+-]?[0-9]*[.]?[0-9]+)[+]([+-]?[0-9]*[.]?[0-9]+)j";
    const std::string NUMBER_REGEX = "([0-9]+)";
    const std::string ID_REGEX = "id:" + NUMBER_REGEX + ";";
    const std::string POSITION_REGEX = "Pos:[\(]" + COMPLEX_REGEX + "[\)];";
    const std::string ORIENTATION_REGEX = "Azm:[\(]" + COMPLEX_REGEX + "[\)]";
    const std::string REGEX = ID_REGEX + POSITION_REGEX + ORIENTATION_REGEX;

//    const std::regex r("id:([0-9]);Pos:[\(]([+-]?[0-9]*[.]?[0-9]+)[+]([+-]?[0-9]*[.]?[0-9]+)j[\)];Azm:[\(]([+-]?[0-9]*[.]?[0-9]+)[+]([+-]?[0-9]*[.]?[0-9]+)j[\)]");
    const std::regex r(REGEX);
    std::smatch sm;
    if (std::regex_search(msg, sm, r))
    {
        int id = std::stoi(sm[ID_IDX]);
        std::complex<double> pos(std::stod(sm[POS_REAL_IDX])/10.0, std::stod(sm[POS_IMAG_IDX])/10.0);
        std::complex<double> azm(std::stod(sm[AZM_REAL_IDX]), std::stod(sm[AZM_IMAG_IDX]));
        if (gpsData.getId() == id)
        {
            gpsData.update(pos, azm);
        }
    }
}

void GPSConnection::start()
{
    this->server = std::thread(&GPSConnection::getServer, this);
    this->localization = std::thread(&GPSConnection::getPositionData, this);
}

void GPSConnection::stop()
{
    this->runCarClient = false;
    this->server.join();
    this->localization.join();
}