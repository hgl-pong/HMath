#ifndef H_FILESYSTEM_H
#define H_FILESYSTEM_H

#include <filesystem>

namespace MathLib
{
    namespace FileSystem
    {
        inline std::string GetFileExtension(const std::string& filename)
        {
            size_t lastindex = filename.find_last_of("."); // find last occurrence of '.'
            return filename.substr(lastindex + 1); // get substring from last occurrence of '.' to end of string
        }

        inline bool IsFileExists(const std::string& filename)
        {
            return std::filesystem::exists(filename);
        }

        inline bool IsDirectoryExists(const std::string& directory)
        { 
            return std::filesystem::exists(directory) && std::filesystem::is_directory(directory); 
        }

        inline bool CreateDirectory(const std::string& directory)
        {
            return std::filesystem::create_directory(directory);
        }
    } // namespace FileSystem
}// namespace MathLib

#endif // !H_FILESYSTEM_H
