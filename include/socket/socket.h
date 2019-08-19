#ifndef SOCKET_H
#define SOCKET_H

#include <iostream>
#include <sys/socket.h>
//#include <complex.h>
#include <vector>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <memory>

#define IP_SIZE 16
#define ERR_CODE -1
#define SOCKET_PTR std::unique_ptr<Socket>

/*!
 * \brief The Socket class, provides functions for socket communication
 */
class Socket
{
public:
    /*!
     * \name Socket
     * \brief Default Constructor
     */
    Socket();

    /*!
     * \name Socket
     * \brief Constructor method.
     * \param domain    integer, communication domain. Only works for AF_INET (IPv4 protocol)
     * \param type      communication type:
     *                          SOCK_STREAML:   TCP(reliable, connection oriented)
     *                          SOCK_DGRAM:     UDP(unreliable, connectionless)
     * \param protocol  Protocol value for Internet Protocol(IP), which is 0
     */
    Socket(int domain, int type, int protocol);

    /*!
     * \name setSockOpt
     * \brief   The setsockopt() function shall set the option specified by the optname argument,
     *          at the protocol level specified by the level argument, to the value pointed to by the
     *          optval argument for the socket associated with the file descriptor specified by
     *          the socket argument.
     * \param level     The level argument specifies the protocol level at which the option resides.
     *                  To set options at the socket level, specify the level argument as SOL_SOCKET.
     * \param optname   The optname argument specifies a single option to set.
     *                  The <sys/socket.h> header defines the socket-level options. (SO_DEBUG, SO_BROADCAST,
     *                  SO_REUSEADDR etc.)
     * \param optval    Value for argument optname
     * \param optlen    Size of the optval argument
     * \return status (0 if successful, and -1 otherwise)
     */
    int setSockOpt(int level, int optname, void* optval, unsigned int optlen);

    /*!
     * \name getSockOpt
     * \brief   The getsockopt() function shall retrieve the value for the option specified by
     *          the optname argument for the socket specified by the socket argument.
     * \param level     The level argument specifies the protocol level at which the option resides.
     *                  To set options at the socket level, specify the level argument as SOL_SOCKET.
     * \param optname   The optname argument specifies a single option to set.
     *                  The <sys/socket.h> header defines the socket-level options. (SO_DEBUG, SO_BROADCAST,
     *                  SO_REUSEADDR etc.)
     * \param optval    Value for argument optname
     * \param optlen    Size of the optval argument
     * \return status (0 if successful, and -1 otherwise)
     */
    int getSockOpt(int level, int optname, void* optval, unsigned int optlen);

    /*!
     * \name bind
     * \brief   The bind() function shall assign a local socket address address to a socket
     *          identified by descriptor socket that has no local socket address assigned.
     * \param ip    IP to be bound to the socket
     * \param port  port
     * \return status (0 if successful, and -1 otherwise)
     */
    int bind(std::string ip, uint16_t port);

    /*!
     * \name listen
     * \brief   The listen() function shall mark a connection-mode socket, specified by the socket
     *          argument, as accepting connections.
     * \param backlog    limit the number of outstanding connections in the socket's listen queue
     * \return status (0 if successful, and -1 otherwise)
     */
    int listen(int backlog);

    /*!
     * \name accept
     * \brief   The accept() function shall extract the first connection on the queue of pending
     *          connections, create a new socket with the same socket type protocol and address
     *          family as the specified socket, and allocate a new file descriptor for that socket.
     * \return status (0 if successful, and -1 otherwise)
     */
    SOCKET_PTR accept();

    /*!
     * \name connect
     * \brief The connect() function shall attempt to make a connection on a socket.
     * \param ip    peer IP address
     * \param port  peer port
     * \return status (0 if successful, and -1 otherwise)
     */
    int connect(std::string ip, uint16_t port);

    /*!
     * \name send
     * \brief   Used to transmit a message to another socket.
     *          The send() call may be used only when the socket is in a connected state.
     * \param message   message to be sent
     * \return status (0 if successful, and -1 otherwise)
     */
    int send(std::string message);

    /*!
     * \name recv
     * \brief   The recv() function shall receive a message from a connection-mode or connectionless-mode socket.
     *          It is normally used with connected sockets because it does not permit the application to retrieve
     *          the source address of received data.
     * \param len   length of the receiving message
     * \return received message
     */
    std::string recv(int len);

    /*!
     * \name recv
     * \brief   The recv() function shall receive a message from a connection-mode or connectionless-mode socket.
     *          It is normally used with connected sockets because it does not permit the application to retrieve
     *          the source address of received data.
     *          If the message is not received within the specified time, then it is aborted.
     * \param len   length of the receiving message
     * \param time  time to wait after a message before aborting
     * \return received message
     */
    std::string recv(int len, int time);

    /*!
     * \name recvfrom
     * \brief   The recvfrom() function shall receive a message from a connection-mode or connectionless-mode socket.
     *          It is normally used with connectionless-mode sockets because it permits the application to retrieve
     *          the source address of received data.
     * \param len           length of the receiving message
     * \param flag          specifies the type of message reception
     * \param socketAddress A null pointer, or points to a sockaddr structure in which the sending address is to be stored
     * \return received message
     */
    std::string recvfrom(int len, int flag, sockaddr* socketAddress);

    /*!
     * \name recvfrom
     * \brief   The recvfrom() function shall receive a message from a connection-mode or connectionless-mode socket.
     *          It is normally used with connectionless-mode sockets because it permits the application to retrieve
     *          the source address of received data.
     *          If the message is not received within the specified time, then it is aborted.
     * \param len           length of the receiving message
     * \param flag          specifies the type of message reception
     * \param socketAddress A null pointer, or points to a sockaddr structure in which the sending address is to be stored
     * \param time          time to wait after a message before aborting
     * \return received message
     */
    std::string recvfrom(int len, int flag, sockaddr* socketAddress, int time);

    /*!
     * \name setBlocking()
     * \brief not implemented
     * \return
     */
    int setBlocking();

    /*!
     * \name setNonBlocking
     * \brief not implemented
     * \return
     */
    int setNonBlocking();

    /*!
     * \name getHostName
     * \brief Returns the null-terminated hostname.
     * \return host name
     */
    std::string getHostName();

    /*!
     * \name getSockName
     * \brief Returns the current address to which the socket sockfd is bound, in the buffer pointed to by addr.
     * \param addr  pointer to the buffer for address
     * \return status (0 if successful, and -1 otherwise)
     */
    int getSockName(sockaddr* addr);

    /*!
     * \name getHostIP
     * \brief Returns the null-terminated host IP
     * \return host IP
     */
    std::string getHostIP();

    /*!
     * \name shutdown
     * \brief not implemented
     * \return
     */
    int shutdown();

    /*!
     * \name close
     * \brief Method for closing socket.
     * \return status (0 if successful, and -1 otherwise)
     */
    int close();

    /*!
     * \name isOpen
     * \brief Verify if socket is open.
     * \return true if socket is open and false otherwise
     */
    bool isOpen();

    /*!
     * \name Socket
     * \brief Copy constructor.
     */
    Socket(const Socket &);

    /*!
     * \name operator=
     * \brief Assignemt operator.
     * \param obj   Object to be copied.
     * \return copied object;
     */
    Socket& operator= (const Socket &obj);

private:
    int socket;
    bool socketIsOpen;
    std::string hostIP;
    uint16_t port;
    sockaddr_in socketAddress;
    struct addrinfo socketInfo;
};

#endif // SOCKET_H
