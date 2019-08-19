#include <errno.h>
#include <string.h>
// #include <libgen.h>

#include "findfile.h"

#ifdef FILESYS
namespace fs = std::experimental::filesystem;
#endif // FILESYS

FindFile::FindFile(const std::string path)
{
    if (this->existPath(path))
    {
        // Directory exists
        this->path = path;
    }
    else
    {
        // Directory does not exist
        std::cerr << "Directory does not exist " << strerror(errno) << std::endl;
        throw std::system_error(errno, std::generic_category(), strerror(errno));
    }
}

bool FindFile::existPath(const std::string path)
{
#ifdef FILESYS
    if(fs::exists(path))    { return true; }    // Directory exists
    else                    { return false; }   // Directory does not exist
#else
    DIR* dir = opendir(path.c_str());
    if (dir)    { closedir(dir); return true; }     // Directory exists
    else        { return false; }                   // Directory does not exist
#endif // FILESYS
}

void FindFile::setPath(const std::string path)
{
    if (this->existPath(path))
    {
        // Directory exists
        this->path = path;
    }
    else
    {
        // Directory does not exist
        std::cerr << "Directory does not exist " << strerror(errno) << std::endl;
        throw std::system_error(errno, std::generic_category(), strerror(errno));
    }
}

bool FindFile::find(const std::string file)
{
#ifdef FILESYS
    for (const  auto & entry : fs::directory_iterator(path))
    {
        if (entry.path().filename().string() == file)
        {
            return true;
        }
    }
    return false;
#else
    size_t len = strlen(file.c_str());
    DIR *dir = opendir(this->path.c_str());
    dirent *dp;
    while ((dp = readdir(dir)) != NULL)
    {
        if (!strcmp(dp->d_name, file.c_str()))
        {
            (void)closedir(dir);
            return true;
        }
    }
    (void)closedir(dir);
    return false;
#endif // FILESYS
}

int FindFile::findBySubstr(const std::string substr, std::vector<std::string>& found)
{
    int count = 0;
#ifdef FILESYS
    for (const  auto & entry : fs::directory_iterator(path))
    {
        if (entry.path().filename().string().find(substr) >= 0)
        {
            found.push_back(entry.path().string());
        }
    }
#else
    size_t len = strlen(substr.c_str());
    DIR *dir = opendir(this->path.c_str());
    dirent *dp;
    char filename[256] = {0};
    while ((dp = readdir(dir)) != NULL)
    {
        if (strstr(dp->d_name, substr.c_str()))
        {
            sprintf(filename, "%s/%s", this->path.c_str(), dp->d_name);
            found.push_back(filename);
        }
    }
    (void)closedir(dir);
#endif // FILESYS
    return found.size();
}

std::string FindFile::findFirstBySubstr(const std::string substr)
{
#ifdef FILESYS
    for (const  auto & entry : fs::directory_iterator(path))
    {
        if (entry.path().filename().string().find(substr) >= 0)
        {
            return entry.path().string();
        }
    }
#else
    size_t len = strlen(substr.c_str());
    DIR *dir = opendir(this->path.c_str());
    dirent *dp;
    char filename[256] = {0};
    while ((dp = readdir(dir)) != NULL)
    {
        if (strstr(dp->d_name, substr.c_str()))
        {
            std::string filePath;
            if (this->path[this->path.size()-1] == '/')
            {
                filePath = this->path.substr(0, this->path.size()-1);
            }
            else
            {
                filePath = this->path;
            }
            sprintf(filename, "%s/%s", filePath.c_str(), dp->d_name);
            (void)closedir(dir);
            return filename;
        }
    }
#endif // FILESYS
    std::cerr << "File not found " << std::endl;
    std::string msg = std::string("File containing ") + substr + std::string(" in directory ") + this->path + (" not found.");
    throw std::system_error(-1, std::generic_category(), msg);
}

