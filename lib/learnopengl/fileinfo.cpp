#include <learnopengl/fileinfo.hpp>
#include <utility>
#include <fstream>
#include <filesystem>

namespace learnopengl {

FileInfo::FileInfo(std::string relativePath) : _relativePath(std::move(relativePath)) {}

std::string FileInfo::absolutePath() const
{
    static int MAX_LEVEL = 5;
    int currentLevel = 0;
    while(currentLevel < MAX_LEVEL)
    {
        std::string prefixPath;
        for(int i = 0; i < currentLevel; ++i) { prefixPath += "../"; }

        const auto filePath = prefixPath + _relativePath;

        if(std::filesystem::exists(filePath))
        {
            return std::filesystem::absolute(filePath).generic_string();
        }

        ++currentLevel;
    }

    return _relativePath;
}

}
