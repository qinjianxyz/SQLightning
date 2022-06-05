//
//  Application.hpp
//  Developed by Jian Qin & Xiaoqiang Qi
//  Mentored by Rick Gessner
//

#ifndef Application_hpp
#define Application_hpp

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <memory>
#include <vector>
#include <algorithm>
#include <filesystem>

#include "Tokenizer.hpp"
#include "CmdProcessor.hpp"
#include "DBProcessor.hpp"
#include "Database.hpp"
#include "SQLProcessor.hpp"
#include "Joins.hpp"
#include "CommandView.hpp"
#include "FolderView.hpp"
#include "Config.hpp"

namespace ECE141 {

class DBProcessor;
class SQLProcessor;

/**
 The application class is the main controller of the database
 */
class Application : public CmdProcessor {
    
public:
    Application(std::ostream &anOutput);
    virtual                 ~Application();
    std::string             getVersion() { return "1.0";}
    CmdProcessor*           recognizes(Tokenizer &aTokenizer) override;
    Statement*              makeStatement(Tokenizer &aTokenizer, StatusResult &aResult) override;
    
    StatusResult            handleInput(std::istream &anInput);
    StatusResult            run(Statement *aStmt) override;
    StatusResult            runScript(std::string aFileName);
    StatusResult            createDatabase(const std::string &aName);
    StatusResult            showDatabases() const;
    StatusResult            dropDatabase(const std::string &aName);
    StatusResult            dumpDatabase(const std::string &aName);
    StatusResult            useDatabase(const std::string &aName);
    StatusResult            dbExists(const std::string &aDBName);
    Database*               getDatabaseInUse();
    std::ostream&           getStream() { return output; };
    
protected:
    std::unique_ptr<DBProcessor>    dbProc;
    std::unique_ptr<SQLProcessor>   sqlProc;
    std::shared_ptr<Database>       databaseInuse;
};

}

#endif /* Application_hpp */
