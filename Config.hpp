//
//  Config.hpp
//  Developed by Jian Qin & Xiaoqiang Qi
//  Mentored by Rick Gessner
//

#ifndef Config_h
#define Config_h
#include <sstream>
#include "Timer.hpp"

namespace ECE141 {

enum class CacheType : int {block=0, rows, views};

/**
 This class  contains configuration information about the database such as system path
 */
struct Config {
    
    static const char* getDBExtension() {return ".db";}
    
    static const std::string getStoragePath() {
        #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
            return "D:/Temp/ece141b"; // Windows
        #elif __APPLE__ || defined __linux__ || defined __unix__
            return "/tmp";  // MAC, UNIX, LINUX
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
    
    /**
     turning on this mode means blocks are retrieved using an index, otherwise by bruteforce
     */
    static const bool indexMode;
    
    /**
     turing on this mode means results are cached in memory for reuse.
     */
    static const bool cacheMode;
    
};

}

#endif /* Config_h */
