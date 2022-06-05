//
//  Application.cpp
//  Developed by Jian Qin & Xiaoqiang Qi
//  Mentored by Rick Gessner
//

#define _CRT_SECURE_NO_WARNINGS

#include "Application.hpp"
#include "TestAutomatic.hpp"
#include "ScriptRunner.hpp"

namespace ECE141 {

// initialize different command processors
Application::Application(std::ostream &anOutput) : CmdProcessor(anOutput) {
    dbProc = std::make_unique<DBProcessor>(anOutput, this);
    sqlProc = std::make_unique<SQLProcessor>(anOutput, this);
}

Application::~Application() {}

// determine whether a keyword can be processed by the application
bool isKnown(Keywords aKeyword) {
    static Keywords theKnown[]=
    { Keywords::quit_kw, Keywords::version_kw, Keywords::help_kw, Keywords::run_kw };
    auto theIt = std::find(std::begin(theKnown), std::end(theKnown), aKeyword);
    return theIt!=std::end(theKnown);
}

// determine if any processor can recognize the Tokenizer
CmdProcessor* Application::recognizes(Tokenizer &aTokenizer) {
    // application can process the tokenizer
    if(isKnown(aTokenizer.current().keyword)) {
        return this;
    }
    // DB Processor can process the tokenizer
    else if (dbProc->recognizes(aTokenizer)) {
        return dbProc.get();
    }
    // SQL Processor can process the tokenizer
    else if (sqlProc->recognizes(aTokenizer)) {
        return sqlProc.get();
    }
    return nullptr;
}

StatusResult Application::run(Statement* aStatement) {
    switch(aStatement->getType()) {
        case Keywords::quit_kw:
            output << "DB::141 is shutting down\n";
            return StatusResult(ECE141::userTerminated);
            break;
        case Keywords::version_kw:
            output << "Version " << getVersion() << "\n";
            break;
        case Keywords::help_kw:
            output << "Help system available\n";
            break;
        case Keywords::run_kw:
            runScript(aStatement->getExtras());
            break;
        default: break;
    }
    return StatusResult{Errors::noError};
}

StatusResult Application::runScript(std::string aFileName) {
    StatusResult theRes;
    std::ifstream theFile;
    std::string thePath = Config::getStoragePath() + "/" + aFileName;
    std::cout << thePath << std::endl;
    
    theFile.open(thePath);

    if (!theFile) {
        theRes = StatusResult{ unknownFile };
        return theRes;
    }
    
    ScriptRunner theRunner(*this);
    theRunner.run(theFile, output);
    
    std::cout << aFileName << std::endl;
    return theRes;
}

Statement* Application::makeStatement(Tokenizer &aTokenizer, StatusResult &aResult) {
    Token theToken=aTokenizer.current();
    if (isKnown(theToken.keyword)) {
        aTokenizer.next();
        if (theToken.keyword == Keywords::run_kw) {
            std::string theFileName = aTokenizer.current().data;
            aTokenizer.next();
            return new Statement(theToken.keyword, theFileName);
        } else {
            return new Statement(theToken.keyword);
        }
        
    }
    return nullptr;
}

StatusResult Application::handleInput(std::istream &anInput){
    Tokenizer theTokenizer(anInput);
    StatusResult theResult = theTokenizer.tokenize();
    
    while (theResult && theTokenizer.more()) {
        Config::getTimer().reset();
        if(auto *theProc=recognizes(theTokenizer)) {
            if(auto *theStatement=theProc->makeStatement(theTokenizer,theResult)) {
                theResult=theProc->run(theStatement);
                if (theResult) theTokenizer.skipIf(';');
                delete theStatement;
            }
        }
        else theResult=unknownCommand;
    }
    return theResult;
}

// create a .db database file in the path located in config
StatusResult Application::createDatabase(const std::string &aName) {
    Timer theTimer = Config::getTimer();
    theTimer.reset();
    bool theSuccessFlag = false;
    size_t theCount{0};
    if (dbExists(aName)) {
        return StatusResult{ databaseCreationError };
    }
    theSuccessFlag = true;
    theCount++;
    
    // todo: wrap the logic below into a function
    // write meta block
    BlockIO theBlockIO(Config::getDBPath(aName), true);
    Block theBlock(BlockType::meta_block);
    theBlock.payload[0] = '\0';
    theBlockIO.writeBlock(0, theBlock);
    
    
    // write index meta block
    IndexMetaBlock theIndexBlock(BlockType::meta_block);
    theIndexBlock.payload[0] = '\0';
    theBlockIO.writeIndexMetaBlock(theIndexBlock);
    CommandView theView(output, Keywords::create_kw, theSuccessFlag, theCount, theTimer.elapsed());
    theView.show();
    return StatusResult{};
}

// output database in the terminal
StatusResult Application::showDatabases() const {
    FolderView theFolderView(output, Config::getStoragePath().c_str());
    theFolderView.show();
    return StatusResult{};
}

// delete database file permanently
StatusResult Application::dropDatabase(const std::string &aName) {
    Timer theTimer = Config::getTimer();
    theTimer.reset();
    bool theSuccessFlag = false;
    size_t theCount{0};
    
    if (!dbExists(aName)) {
        return StatusResult{ unknownDatabase };
    }
    if (databaseInuse && Config::getDBPath(aName) == databaseInuse->getPath()) {
        databaseInuse.reset();
    }
    if (0 != std::remove(Config::getDBPath(aName).c_str())) {
        return StatusResult{ databaseRemovalError };
    }
    theSuccessFlag = true;
    CommandView theView(output, Keywords::drop_kw, theSuccessFlag, theCount, theTimer.elapsed());
    theView.show();
    return StatusResult{};
    
}

// display information about each block
StatusResult Application::dumpDatabase(const std::string &aName)  {
    
    if (!dbExists(aName)) {
        return StatusResult{ unknownDatabase };
    }
    Timer theTimer = Config::getTimer();
    theTimer.reset();
    databaseInuse = std::make_shared<Database>(Config::getDBPath(aName), output);
    size_t theCount = databaseInuse->getBlockCount();
    Block theBlock;
    char theString[20];
    for (size_t index = metaSize; index < theCount; index++) {
        databaseInuse->readBlock(index, theBlock);
        strncpy(theString, theBlock.payload, 20);
        theString[19] = 0;
        std::cout << "Block" << index << ": " << static_cast<char>(theBlock.header.type) << ", Preview: " << theString << std::endl;
    }
    output << theCount << " rows in set (" << theTimer.elapsed() << " sec)" << std::endl;
    return StatusResult{};
}

// call DB object to be loaded into memory...
StatusResult Application::useDatabase(const std::string &aName) {
    
    if (!dbExists(aName)) {
        return StatusResult{ unknownDatabase };
    }
    
    databaseInuse = std::make_shared<Database>(Config::getDBPath(aName), output);
    output << "Database Changed" << std::endl;
    return StatusResult{};
}

Database* Application::getDatabaseInUse() {
    return databaseInuse.get();
}

// input a database name (without ".db" and directory)
StatusResult Application::dbExists(const std::string &aDBName) {
    return fs::exists(fs::path(Config::getDBPath(aDBName))) ? StatusResult{} : StatusResult{unknownDatabase};
}

}
