//
//  CommandView.hpp
//  Developed by Jian Qin & Xiaoqiang Qi
//  Mentored by Rick Gessner
//

#ifndef CommandView_hpp
#define CommandView_hpp

#include <stdio.h>
#include "keywords.hpp"
#include "View.hpp"

namespace  ECE141 {

/**
 This class outputs the query information in the terminal
 */
class CommandView : public View {
public:
    Keywords theKeyword;
    bool theIndicator;
    size_t theNumberRowsAffected;
    size_t theTimeElapsed;
    
    CommandView(std::ostream& anOutput, Keywords aKeyword, bool anIndicator, size_t aNumberRowsAffected, size_t aTimeElapsed)
    : View(anOutput), theKeyword(aKeyword), theIndicator(anIndicator), theNumberRowsAffected(aNumberRowsAffected)
    , theTimeElapsed(aTimeElapsed) {}
    
    StatusResult show() override {
        std::string theResponse("QUERY ");
        if (!theIndicator) theResponse += "NOT ";
        theResponse += "OK, ";
        output << theResponse << theNumberRowsAffected << " Rows Affected (" << theTimeElapsed << " sec)" << std::endl;
        return StatusResult{};
    }
    
    
};
}

#endif /* CommandView_hpp */
