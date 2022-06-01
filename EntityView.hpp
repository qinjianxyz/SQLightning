//
//  EntityView.hpp
//  Developed by Jian Qin & Xiaoqiang Qi
//  Mentored by Rick Gessner
//

#ifndef EntityView_hpp
#define EntityView_hpp

#include <stdio.h>
#include "Config.hpp"
#include <string>
#include <map>
#include "Database.hpp"
#include "View.hpp"

namespace ECE141 {

class Database;
/**
 The EntityView Class shows the information about an Entity in the command line
 */
class EntityView : public View {
public:
    EntityView(std::ostream& anOutput, const Database* aDatabase): View(anOutput), theDatabase(aDatabase) {}
    
    StatusResult show() override {
        Timer theTimer = Config::getTimer();
        theTimer.reset();
        size_t theCount{1};
        auto theMap = theDatabase->getMeta().entityList;
        std::vector<std::string> keys;
        for(auto it = theMap.begin(); it != theMap.end(); it++) {
            keys.push_back(it->first);
        }
        for (auto key : keys) {
            std::cout << "Table " << theCount << ": " << key << std::endl;
            theCount++;
        }
        output << (--theCount) << " rows in set (" << theTimer.elapsed() << " sec)" << std::endl;
        return StatusResult{};
    }
    
    const Database*     theDatabase;
};

};

#endif /* EntityView_hpp */
