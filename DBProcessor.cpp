//
//  DBProcessor.cpp
//  Developed by Jian Qin & Xiaoqiang Qi
//  Mentored by Rick Gessner
//

#include "DBProcessor.hpp"


namespace SQLightning {

using StmtFactory = Statement* (*)(DBProcessor *aProc, Tokenizer &aTokenize);

DBProcessor::DBProcessor(std::ostream &anOutput, Application* anApp) : CmdProcessor(anOutput) {
    appRunning = anApp;
}

DBProcessor::~DBProcessor() {}

CmdProcessor* DBProcessor::recognizes(Tokenizer &aTokenizer) {
    if (isCreateDatabase(aTokenizer) || isDumpDatabase(aTokenizer) ||
        isDropDatabase(aTokenizer) || isShowDatabases(aTokenizer)|| isUseDatabase(aTokenizer)) {
        return this;
    }
    return nullptr;
}

StatusResult DBProcessor::run(Statement *aStmt) {
    switch (aStmt->getType()) {
        case Keywords::create_kw:
            return appRunning->createDatabase(aStmt->getStatementName());
            break;
        case Keywords::drop_kw:
            return appRunning->dropDatabase(aStmt->getStatementName());
            break;
        case Keywords::show_kw:
            return appRunning->showDatabases();
            break;
        case Keywords::use_kw:
            return appRunning->useDatabase(aStmt->getStatementName());
            break;
        case Keywords::dump_kw:
            return appRunning->dumpDatabase(aStmt->getStatementName());
        default: break;
    }
    return StatusResult{ Errors::noError };
}

// USE: retrieve a statement based on given text input...
Statement* DBProcessor::makeStatement(Tokenizer &aTokenizer, StatusResult &aResult) {
    if (isCreateDatabase(aTokenizer)) {
        aTokenizer.next(commandLength["create database"]);
        return makeStatementAsCreate(aTokenizer);
    }
    if (isDropDatabase(aTokenizer)) {
        aTokenizer.next(commandLength["drop database"]);
        return makeStatementAsDrop(aTokenizer);
    }
    if (isShowDatabases(aTokenizer)) {
        aTokenizer.next(commandLength["show databases"]);
        return makeStatementAsShow(aTokenizer);
    }
    if (isUseDatabase(aTokenizer)) {
        aTokenizer.next(commandLength["use"]);
        return makeStatementAsUse(aTokenizer);
    }
    if (isDumpDatabase(aTokenizer)) {
        aTokenizer.next(commandLength["dump database"]);
        return makeStatementAsDump(aTokenizer);
    }
    return nullptr; //means we don't know the statement...
}

// try to match the command, unknown represents the database file name
bool  DBProcessor::isCreateDatabase(Tokenizer& aTokenizer) {
    return Helpers::isEqualCmd(aTokenizer, { Keywords::create_kw, Keywords::database_kw, Keywords::unknown_kw });
}

bool  DBProcessor::isDumpDatabase(Tokenizer& aTokenizer) {
    return Helpers::isEqualCmd(aTokenizer, { Keywords::dump_kw, Keywords::database_kw, Keywords::unknown_kw });
}

bool  DBProcessor::isDropDatabase(Tokenizer& aTokenizer) {
    return Helpers::isEqualCmd(aTokenizer, { Keywords::drop_kw, Keywords::database_kw, Keywords::unknown_kw });
}

bool  DBProcessor::isShowDatabases(Tokenizer& aTokenizer) {
    return Helpers::isEqualCmd(aTokenizer, { Keywords::show_kw, Keywords::databases_kw });
}

bool  DBProcessor::isUseDatabase(Tokenizer& aTokenizer) {
    return Helpers::isEqualCmd(aTokenizer, { Keywords::use_kw, Keywords::unknown_kw });
}

DBStatement* DBProcessor::makeStatementAsCreate(Tokenizer& aTokenizer) {
    return new DBStatement(Keywords::create_kw, retrieveDatabaseObj(aTokenizer));
}
DBStatement* DBProcessor::makeStatementAsDrop(Tokenizer& aTokenizer) {
    return new DBStatement(Keywords::drop_kw, retrieveDatabaseObj(aTokenizer));
    
}
DBStatement* DBProcessor::makeStatementAsShow(Tokenizer& aTokenizer) {
    return new DBStatement(Keywords::show_kw);
}
DBStatement* DBProcessor::makeStatementAsUse(Tokenizer& aTokenizer) {
    return new DBStatement(Keywords::use_kw, retrieveDatabaseObj(aTokenizer));
}
DBStatement* DBProcessor::makeStatementAsDump(Tokenizer& aTokenizer) {
    return new DBStatement(Keywords::dump_kw, retrieveDatabaseObj(aTokenizer));
}

std::string DBProcessor::retrieveDatabaseObj(Tokenizer& aTokenizer) {
    Token theToken = aTokenizer.current();
    aTokenizer.next();
    return theToken.data;
}




}
