#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <iostream>
#include <thread>
#include <mutex>
#include <fstream>

/*!
 * \brief The FileHandler class, it contains the functions for file handilng.
 */
class FileHandler
{
private:
    std::mutex lock;
    std::ofstream outFile;

public:
    /*!
     * \name FileHandler
     * \brief Default Constructor.
     */
    FileHandler();

    /*!
     * \name FileHandler
     * \brief Constructor method.
     * \param fileName  file name
     */
    FileHandler(const char* fileName);

    /*!
     * \name write
     * \brief Method for writing into file.
     * \param message   string to be written
     */
    void write(const char* message);

    /*!
     * \name close
     * \brief Method for closing file.
     */
    void close();
};

#endif // FILEHANDLER_H
