#ifndef FINDFILE_H
#define FINDFILE_H

#include <iostream>
#include <vector>

// #define FILESYS

#ifdef FILESYS
#include <experimental/filesystem>
#else
#include <dirent.h>
#endif // FILESYS

class FindFile
{
public:
    /*!
     * \name FindFile
     * \brief Construct a new Find File object..
     * \param path 
     */
    FindFile(const std::string path = ".");

    /*!
     * \name setPath
     * \brief Set the Path.
     * \param path 
     */
    void setPath(const std::string path);

    /*!
     * \name existPath
     * \brief Verify if the specidied path exists
     * \param path 
     * \return true 
     * \return false 
     */
    bool existPath(const std::string path);

    /*!
     * \name find
     * \brief Search for the specified file.
     * \param file 
     * \return true 
     * \return false 
     */
    bool find(const std::string file);

    /*!
     * \find
     * \brief Search for the specified file in the specified path.
     * \param path 
     * \param file 
     * \return true 
     * \return false 
     */
    bool find(const std::string path, const std::string file);

    /*!
     * \findBySubstr
     * \brief Find by a portion of the filename
     * \param substr 
     * \param found 
     * \return int 
     */
    int findBySubstr(const std::string substr, std::vector<std::string>& found);
    
    /*!
     * \name findFirstBySubstr
     * \brief Find first file that contains the substr
     * \param substr 
     * \return std::string 
     */
    std::string findFirstBySubstr(const std::string substr);
private:
    std::string path;
};

#endif // FINDFILE_H