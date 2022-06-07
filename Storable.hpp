//
//  Storable.hpp
//  Developed by Jian Qin & Xiaoqiang Qi
//  Mentored by Rick Gessner
//

#ifndef Storable_hpp
#define Storable_hpp

#include <stdio.h>
#include "TypedStream.hpp"

namespace SQLightning {

/**
 The interface for all things storable in the database as a row
 */
class Storable {
public:
    virtual StatusResult  encode(std::ostream& anOutput) const = 0;
    virtual StatusResult  decode(std::istream& anInput) = 0;
};
}

#endif /* Storable_hpp */
