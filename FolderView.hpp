//
//  FolderView.hpp
//  Developed by Jian Qin & Xiaoqiang Qi
//  Mentored by Rick Gessner
//

#ifndef FolderView_h
#define FolderView_h

#include "FolderReader.hpp"
#include "Config.hpp"
#include "View.hpp"
#include <iomanip>

namespace SQLightning {

/**
 The class shows information about a folder in the command line
 */
class FolderView : public View {
public:
    FolderView(std::ostream& anOutput, const char *aPath, const char *anExtension=".db")
    : View(anOutput), reader(aPath), extension(anExtension) {}
    
    StatusResult show() override {
        Timer theTimer = Config::getTimer();
        theTimer.reset();
        size_t theCount{1};
        reader.each(extension, [&](const std::string &aName) {
            std::cout << "Database " << theCount << ": " << fs::path(aName).filename().string() << std::endl;
            theCount++;
            return true;
        });
        output << (--theCount) << " rows in set (" << theTimer.elapsed() << " sec)" << std::endl;
        return StatusResult{};
    }
    
    FolderReader  reader;
    const char    *extension;
    
};

}

#endif /* FolderView_h */
