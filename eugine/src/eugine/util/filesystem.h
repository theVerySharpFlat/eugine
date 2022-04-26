#ifndef EG_UTIL_FILESYSTEM_H
#define EG_UTIL_FILESYSTEM_H

namespace eg::filesystem {
    std::string getFileContents(const fs::path& path);
}

#endif