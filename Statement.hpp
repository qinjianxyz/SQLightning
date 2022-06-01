//
//  Statement.hpp
//  Developed by Jian Qin & Xiaoqiang Qi
//  Mentored by Rick Gessner
//

#ifndef Statement_hpp
#define Statement_hpp

#include "keywords.hpp"
#include "Tokenizer.hpp"
#include <iostream>
#include <string>

namespace ECE141 {

/**
 The base class for all statement in the database that conveys information about a specific operation
 */
class Statement {
public:
    Statement(Keywords aStatementType=Keywords::unknown_kw, std::string anExtra = "");
    Statement(const Statement &aCopy);
    
    virtual                 ~Statement();
    virtual   StatusResult  parse(Tokenizer &aTokenizer);
    virtual   const char*   getStatementName() const { return "statement"; }
    virtual   StatusResult  run(std::ostream &aStream) const;
    
    std::string             getExtras() const { return extras; }
    Keywords                getType() const { return stmtType; }
    
protected:
    std::string             extras;
    Keywords                stmtType;
};

}

#endif /* Statement_hpp */
