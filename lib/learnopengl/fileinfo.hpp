#ifndef __LEARNOPENGL_FILEINFO_HPP__
#define __LEARNOPENGL_FILEINFO_HPP__

#include <string>

namespace learnopengl {

class FileInfo
{
public:
    FileInfo(std::string relativePath);

    std::string absolutePath() const;
private:
    std::string _relativePath;
};

}

#endif
