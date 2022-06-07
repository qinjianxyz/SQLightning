//
//  DBStatement.hpp
//  Developed by Jian Qin & Xiaoqiang Qi
//  Mentored by Rick Gessner
//

#ifndef DBStatement_hpp
#define DBStatement_hpp

#include "Statement.hpp"

namespace SQLightning {

/**
 The DBStatement class contains information about a database operation from the command line
 */
class DBStatement : public Statement {
public:
    DBStatement(Keywords aStatementType = Keywords::unknown_kw, std::string aStr = "");
    virtual ~DBStatement();
    
    const char* getStatementName() const override { return databaseObj.c_str(); }
    
protected:
    std::string databaseObj;
};

}

#endif /* DBStatement_hpp */
