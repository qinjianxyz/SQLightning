//
//  Joins.hpp
//  Developed by Jian Qin & Xiaoqiang Qi
//  Mentored by Rick Gessner
//

#ifndef Join_h
#define Join_h

#include <set>
#include <string>
#include "BasicTypes.hpp"
#include "Errors.hpp"
#include "keywords.hpp"
#include "Row.hpp"

namespace SQLightning {

class Join {
public:
    const static std::set<Keywords> gJoinTypes;
    
    Join(const std::string &aTable = "", Keywords aType = Keywords::join_kw, const std::string &aLhs = "", const std::string &aRhs = "")
    : secondaryTable(aTable), joinType(aType), lhs(aLhs), rhs(aRhs), inUse(false) {}
    
    std::string secondaryTable;
    Keywords    joinType;
    std::string lhs;
    std::string rhs;
    bool        inUse;
    
};

using JoinList = std::vector<Join>;

}

#endif /* Join_h */
