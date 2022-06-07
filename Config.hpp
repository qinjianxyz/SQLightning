//
//  Config.hpp
//  Developed by Jian Qin & Xiaoqiang Qi
//  Mentored by Rick Gessner
//

#ifndef Config_h
#define Config_h
#include <sstream>
#include "Timer.hpp"

namespace SQLightning {

enum class CacheType : int {block=0, row=1, view=2};

/**
 This class  contains configuration information about the database such as system path
 */
struct Config {
    
    static const char* getDBExtension() {return ".db";}
    
    // block, row, view
    static size_t cacheSize[3];
    
    static const std::string getStoragePath() {
        
        #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
            return "D:/Temp/sqlightning"; // Windows
        #elif __APPLE__ || defined __linux__ || defined __unix__
            return "/tmp";    // MAC, UNIX, LINUX
        #endif
        
    }
    
    static std::string getDBPath(const std::string &aDBName) {
        std::ostringstream theStream;
        theStream << Config::getStoragePath() << "/" << aDBName << ".db";
        return theStream.str();
    }
    
    static Timer& getTimer() {
        static Timer theTimer;
        return theTimer;
    }
    
    //cachetype: block, row, view...
    static size_t getCacheSize(CacheType aType) {
        return cacheSize[(int)aType];
    }
    
    static void setCacheSize(CacheType aType, size_t aSize) {
        cacheSize[(int)aType]=aSize;
    }
    
    // cachetype: block, row, view...
    static bool useCache(CacheType aType) {
        return cacheSize[(int)aType] > 0;
    }
    
    /**
     Each index meta can handle approximately 10000 blocks.
     Ex. if you want to insert 10 million blocks, set meta size to 1000.
     */
    // check BlockIO
    // static const size_t metaSize;
    
    /**
     Used in findNextFreeBlock() in BlockIO
     */
    static bool useFreeBlock() {
        return true;
    }
    
    /**
     turning on this mode means blocks are retrieved using an index, otherwise by bruteforce
     */
    static bool useIndex() {
        return true;
    }
    
};

}

#endif /* Config_h */
