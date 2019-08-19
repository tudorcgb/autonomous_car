#ifndef SOCKET_H
#include "socket.h"
#endif

#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>

Socket::Socket()
{

}

Socket::Socket(int domain, int type, int protocol)
{
    // Open socket
    this->socket = ::socket(domain, type, protocol);
    if (this->socket < 0)
    {
        this->socketIsOpen = false;
        std::cerr << "error while opening socket: " << strerror(errno) << std::endl;
        // If socket open faild then throw exception
        throw std::system_error(errno, std::generic_category(), strerror(errno));
    }
    else
    {
        this->socketIsOpen = true;
    }

    this->port = 0;
    this->hostIP = "";

    memset(&this->socketAddress, 0, sizeof(this->socketAddress));
    this->socketAddress.sin_family = domain;
    this->socketAddress.sin_addr.s_addr = htonl(INADDR_ANY);

    memset(&this->socketInfo, 0, sizeof(this->socketInfo));
    this->socketInfo.ai_family = domain;
    this->socketInfo.ai_socktype = type;
    this->socketInfo.ai_protocol = protocol;
}

int Socket::setSockOpt(int level, int optname, void *optval, unsigned int optlen)
{
    int retVal = setsockopt(this->socket, level, optname, optval, optlen);
    if (retVal < 0)
    {
        std::cerr << "setsockopt error: " << strerror(errno) << std::endl;
        throw std::system_error(errno, std::generic_category(), strerror(errno));
    }
    return retVal;
}

int Socket::getSockOpt(int level, int optname, void *optval, unsigned int optlen)
{
    int retVal = getsockopt(this->socket, level, optname, optval, &optlen);
    if (retVal < 0)
    {
        std::cerr << "getsockopt error: " << strerror(errno) << std::endl;
        throw std::system_error(errno, std::generic_category(), strerror(errno));
    }
    return retVal;
}

int Socket::bind(std::string ip, uint16_t port)
{
    this->port = port;
    // Get port
    this->socketAddress.sin_port = htons(port);
    int retVal = ::bind(this->socket, (struct sockaddr*)&this->socketAddress, sizeof(this->socketAddress));
    if (retVal < 0)
    {
        std::cerr << "binding error: " << strerror(errno) << std::endl;
        throw std::system_error(errno, std::generic_category(), strerror(errno));
    }
    return retVal;
}

// TODO ip
int Socket::connect(std::string ip, uint16_t port)
{
    sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = this->socketAddress.sin_family;
    // Get port
    addr.sin_port = htons(port);
    inet_pton(this->socketAddress.sin_family, ip.c_str(), &addr.sin_addr);
    int retVal = ::connect(this->socket, (struct sockaddr*)&addr, sizeof(addr));
    if (retVal < 0)
    {
        std::cerr << "connect error: " << strerror(errno) << std::endl;
        throw std::system_error(errno, std::generic_category(), strerror(errno));
    }
    return retVal;
}

int Socket::listen(int backlog)
{
    int retVal = ::listen(this->socket, backlog);
    if (retVal < 0)
    {
        std::cerr << "listen error: " << strerror(errno) << std::endl;
        throw std::system_error(errno, std::generic_category(), strerror(errno));
    }
    return retVal;
}

SOCKET_PTR Socket::accept()
{
    SOCKET_PTR newSocket;
    sockaddr_in addr;
    socklen_t addrSize;
    addrSize = sizeof(addr);
    try {
        int socket_fd = ::accept(this->socket, (struct sockaddr*)&addr, &addrSize);
        if (socket_fd < 0)
        {
            std::cerr << "accept error: " << strerror(errno) << std::endl;
            throw std::system_error(errno, std::generic_category(), strerror(errno));
            return NULL;
        }
        newSocket = std::make_unique<Socket>(this->socketInfo.ai_family, this->socketInfo.ai_socktype, this->socketInfo.ai_protocol);
        newSocket->socket = socket_fd;
        newSocket->port = this->port;

        char host[NI_MAXHOST];
        char service[NI_MAXSERV];

        memset(host, 0, NI_MAXHOST);
        memset(service, 0, NI_MAXSERV);

        if (getnameinfo((sockaddr*)&addr, sizeof(addr), host, NI_MAXHOST, service, NI_MAXSERV, 0) < 0)
        {
            std::cerr << "accept error -> getnameinfo error: " << strerror(errno) << std::endl;\
            throw std::system_error(errno, std::generic_category(), strerror(errno));
            return NULL;
        }
        inet_ntop(this->socketInfo.ai_family, &addr.sin_addr, host, NI_MAXHOST);
        std::cout << host << " connected on port " << ntohs(addr.sin_port) << std::endl;
        newSocket->hostIP = host;
    }
    catch (const std::system_error& e)
    {
        throw e;
    }
    return newSocket;
}

int Socket::send(std::string message)
{
    const char* buf = message.c_str();
    int retVal = ::send(this->socket, buf, (int)strlen(buf), 0);
    if (retVal < 0)
    {
        std::cerr << "send error: " << strerror(errno) << std::endl;
        throw std::system_error(errno, std::generic_category(), strerror(errno));
    }
    return retVal;
}

std::string Socket::recv(int len)
{
    std::string buf;
    char buffer[len];
    memset(buffer, 0, len);
    int receivedBytes = ::recv(this->socket, buffer, len, 0);
    if (receivedBytes < 0)
    {
        std::cerr << "recv error: " << strerror(errno) << std::endl;
        throw std::system_error(errno, std::generic_category(), strerror(errno));
        return NULL;
    }
    else
    {
        buf.reserve(len);
        buf = std::string(buffer);
    }
    return buf;

}

std::string Socket::recv(int len, int s_timeout)
{
    std::string buf;
    struct timeval timeout;

    try
    {
        timeout.tv_sec = s_timeout;
        timeout.tv_usec = 0;

        this->setSockOpt(SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

        buf =  this->recv(len);

        timeout.tv_sec = 0;
        timeout.tv_usec = 0;

        this->setSockOpt(SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
    }
    catch (const std::system_error& e)
    {
        throw e;
        return NULL;
    }

    return buf;
}

std::string Socket::recvfrom(int len, int flag, sockaddr* socketAddress)
{
    std::string buf;
    sockaddr_in cliendAddress;
    char buffer[len];
    socklen_t l;
    memset(&cliendAddress, 0, sizeof(cliendAddress));
    memset(buffer, 0, len);
    int receivedBytes = ::recvfrom(this->socket, buffer, len, flag, (struct sockaddr*)&cliendAddress, &l);
    std::cout << "received: " << receivedBytes << " bytes" << std::endl;
    if (receivedBytes < 0)
    {
        std::cerr << "recv error: " << strerror(errno) << std::endl;
        throw std::system_error(errno, std::generic_category(), strerror(errno));
        return NULL;
    }
    else
    {
        buf.reserve(len);
        buf = std::string(buffer);
        socketAddress = (struct sockaddr*)&cliendAddress;
        return buffer;
    }
}

std::string Socket::recvfrom(int len, int flag, sockaddr *socketAddress, int s_timeout)
{
    std::string buf;
    struct timeval timeout;

    try
    {
        timeout.tv_sec = s_timeout;
        timeout.tv_usec = 0;

        this->setSockOpt(SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

        buf = this->recvfrom(len, flag, socketAddress);

        timeout.tv_sec = 0;
        timeout.tv_usec = 0;

        this->setSockOpt(SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
    }
    catch (const std::system_error& e)
    {
        throw e;
        return NULL;
    }

    return buf;
}

std::string Socket::getHostName()
{
    char hostname[1023];
    memset(hostname, 0, 1023);
    int status = ::gethostname(hostname, 1023);
    if (status < 0)
    {
        std::cerr << "gethostname error: " << strerror(errno) << std::endl;
        throw std::system_error(errno, std::generic_category(), strerror(errno));
        return NULL;
    }
    return(std::string(hostname));
}

int Socket::getSockName(sockaddr* addr)
{
    socklen_t l = sizeof(struct sockaddr_in);

    int status = ::getsockname(this->socket, addr, &l);
    if (status < 0)
    {
        std::cerr << "getsockname error: " << strerror(errno) << std::endl;
        throw std::system_error(errno, std::generic_category(), strerror(errno));
    }
    return status;
}

std::string Socket::getHostIP()
{
    char ip[IP_SIZE];
    sockaddr_in addr;
    this->getSockName((struct sockaddr*)&addr);
    inet_ntop(this->socketAddress.sin_family, &addr.sin_addr, ip, sizeof(ip));
    return std::string(ip);
}

int Socket::shutdown()
{
    int status = 0;
    if (this->socket >= 0)
    {
        status = ::shutdown(this->socket, SHUT_RDWR);
        if (status < 0)
        {
            this->socketIsOpen = true;
            std::cerr << "shutdown error: " << strerror(errno) << std::endl;
            throw std::system_error(errno, std::generic_category(), strerror(errno));
        }
    }
    return status;
}

int Socket::close()
{
    int status = 0;
    if (this->socket >= 0)
    {
        status = ::close(this->socket);
        if (status < 0)
        {
            this->socketIsOpen = true;
            std::cerr << "close error: " << strerror(errno) << std::endl;
            throw std::system_error(errno, std::generic_category(), strerror(errno));
        }
        else
        {
            this->socketIsOpen = false;
        }
    }
    return status;
}

bool Socket::isOpen()
{
    return this->socketIsOpen;
}

Socket::Socket(const Socket &socketObj)
{
    this->socket = socketObj.socket;
    this->socketAddress = socketObj.socketAddress;
    this->socketInfo = socketObj.socketInfo;
    this->hostIP = socketObj.hostIP;
    this->port = socketObj.port;
    this->socketIsOpen = socketObj.socketIsOpen;
}

Socket& Socket::operator=(const Socket &socketObj)
{
    this->socket = socketObj.socket;
    this->socketAddress = socketObj.socketAddress;
    this->socketInfo = socketObj.socketInfo;
    this->hostIP = socketObj.hostIP;
    this->port = socketObj.port;
    this->socketIsOpen = socketObj.socketIsOpen;
    return *this;
}



