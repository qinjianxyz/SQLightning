//
//  DBProcessor.hpp
//  Developed by Jian Qin & Xiaoqiang Qi
//  Mentored by Rick Gessner
//

#ifndef DBProcessor_hpp
#define DBProcessor_hpp

#include <filesystem>
#include <stdio.h>
#include <map>
#include <fstream>

#include "Config.hpp"
#include "Application.hpp"
#include "Helpers.hpp"
#include "CmdProcessor.hpp"
#include "Application.hpp"
#include "Tokenizer.hpp"
#include "DBStatement.hpp"



namespace SQLightning {

class Application;

/**
 The DBProcessor class hands raw input for database related commands
 */
class DBProcessor : public CmdProcessor {
public:
    
    DBProcessor(std::ostream &anOutput, Application* anApp);
    ~DBProcessor();
    
    static const auto openNew = std::ios::in | std::ios::out | std::ios::trunc;
    static const auto openExisting = std::ios::in | std::ios::out;
    
    //cmd processor interface...
    CmdProcessor* recognizes(Tokenizer &aTokenizer) override;
    Statement*    makeStatement(Tokenizer &aTokenizer, StatusResult &aResult) override;
    StatusResult  run(Statement *aStmt) override;
    
    // try to match the command
    bool  isCreateDatabase(Tokenizer& aTokenizer);
    bool  isDumpDatabase(Tokenizer& aTokenizer);
    bool  isDropDatabase(Tokenizer& aTokenizer);
    bool  isShowDatabases(Tokenizer& aTokenizer);
    bool  isUseDatabase(Tokenizer& aTokenizer);
    
    // make statement from commands
    DBStatement* makeStatementAsCreate(Tokenizer& aTokenizer);
    DBStatement* makeStatementAsDrop(Tokenizer& aTokenizer);
    DBStatement* makeStatementAsShow(Tokenizer& aTokenizer);
    DBStatement* makeStatementAsUse(Tokenizer& aTokenizer);
    DBStatement* makeStatementAsDump(Tokenizer& aTokenizer);
    
    // retrieve database object from the tokenizer
    std::string     retrieveDatabaseObj(Tokenizer& aTokenizer);

protected:
    std::map<std::string, int> commandLength{
        {"create database", 2},
        {"drop database", 2},
        {"show databases", 2},
        {"use", 1},
        {"dump database", 2}
    };
    
    Application*  appRunning;
    
};

}
#endif /* DBProcessor_hpp */
