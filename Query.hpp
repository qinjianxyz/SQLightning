//
//  Query.hpp
//  Developed by Jian Qin & Xiaoqiang Qi
//  Mentored by Rick Gessner
//

#ifndef Query_h
#define Query_h

#include <string>
#include <vector>

#include "Filters.hpp"
#include "Joins.hpp"

namespace ECE141 {
/**
 The query class contains all information about a query from the select statement
 */
class Query {
    
public:
    Query() : tableName("") {}
    ~Query() {}
    
    std::vector<std::string>  columns;
    std::string tableName;
    std::vector<std::string>  orders;
    std::vector<size_t>       limits;
    Join                      joins;
    Filters                   filters;
    
};
}


#endif /* Query_h */
