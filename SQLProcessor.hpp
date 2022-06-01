//
//  SQLProcessor.hpp
//  Developed by Jian Qin & Xiaoqiang Qi
//  Mentored by Rick Gessner
//

#ifndef SQLProcessor_hpp
#define SQLProcessor_hpp

#include <stdio.h>
#include "CmdProcessor.hpp"
#include "Application.hpp"
#include "Tokenizer.hpp"
#include "SQLStatement.hpp"

namespace ECE141 {

class Statement;
class DBProcessor;
class Entity;
class Database;
class Application;

/**
 The class handles all SQL commands from the command line
 */
class SQLProcessor : public CmdProcessor {
public:
    
    SQLProcessor(std::ostream &anOutput, Application* anApp);
    virtual ~SQLProcessor();
    
    CmdProcessor* recognizes(Tokenizer &aTokenizer) override;
    Statement*    makeStatement(Tokenizer &aTokenizer, StatusResult &aResult) override;
    
    StatusResult  run(Statement *aStmt) override;
    
protected:
    
    Application* appRunning;
};

}
#endif /* SQLProcessor_hpp */
