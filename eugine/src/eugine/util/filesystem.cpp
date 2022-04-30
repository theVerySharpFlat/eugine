#include "filesystem.h"
#include "eugine/core/core.h"
#include "ghc/filesystem.hpp"
#include <istream>
#include <whereami.h>

namespace eg::filesystem {
    std::string getFileContents(const fs::path& name) {
        try {
            fs::ifstream ifs(name, std::ios::in | std::ios::binary | std::ios::ate);

            if(!ifs.is_open())
                eg::error("failed to open file \"{}\"", name.c_str());

            fs::ifstream::pos_type fileSize = ifs.tellg();
            ifs.seekg(0, std::ios::beg);

            std::vector<char> bytes(fileSize);
            ifs.read(&bytes[0], fileSize);

            return std::string(&bytes[0], fileSize);
            
        } catch(const std::exception& e) {
            eg::error("getFileContentsError(): {}", e.what());
            return "";
        }
    }

    void gotoProjectRoot() {
        i32 pathSize = wai_getExecutablePath(NULL, 0, NULL);
        char* executablePath = (char*) alloca(sizeof(char) * pathSize + 1);
        i32 dirsize;
        wai_getExecutablePath(executablePath, pathSize, &dirsize);
        executablePath[dirsize] = '\0';
        info("path: {}", executablePath);

        fs::path currentPath(executablePath);
        EG_ASSERT(fs::is_directory(currentPath), "path: {} is not a directory!", currentPath.string());
        EG_ASSERT(currentPath.filename().string() == "bin", "binary is not in the correct directory!");
        try {
            fs::current_path(currentPath.parent_path());
        } catch(const std::exception& e) {
            error("gotoProjectRoot error: {}", e.what());
        }
    }
}