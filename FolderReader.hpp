//
//  FolderReader.hpp
//  Developed by Jian Qin & Xiaoqiang Qi
//  Mentored by Rick Gessner
//

#ifndef FolderReader_h
#define FolderReader_h

#include <string>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

namespace SQLightning {

using FileVisitor = std::function<bool(const std::string&)>;

/**
 The folder visitor class handles operation related to files
 */
class FolderReader {
public:
    FolderReader(const char *aPath) : path(aPath) {}
    virtual ~FolderReader() {}
    
    virtual bool exists(const std::string &aFilename) {
        std::ifstream theStream(aFilename);
        return !theStream ? false : true;
    }
    
    virtual void each(const std::string &anExt,
                      const FileVisitor &aVisitor) const {
        // iterate through every file
        for (const auto & file : fs::directory_iterator(path)) {
            if (anExt == file.path().extension().string()) {
                aVisitor(file.path().string());
            }
        }
    };
    
    std::string path;
};

}

#endif /* FolderReader_h */
