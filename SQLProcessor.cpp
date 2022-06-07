//
//  SQLProcessor.cpp
//  Developed by Jian Qin & Xiaoqiang Qi
//  Mentored by Rick Gessner
//

#include "SQLProcessor.hpp"

namespace SQLightning {


SQLProcessor::SQLProcessor(std::ostream &anOutput, Application* anApp)
:  CmdProcessor(anOutput), appRunning(anApp) {
}

SQLProcessor::~SQLProcessor() {}

CmdProcessor* SQLProcessor::recognizes(Tokenizer &aTokenizer) {
    if (CreateTableStatement::recognize(aTokenizer) || DropTableStatement::recognize(aTokenizer)
        || DescribeStatement::recognize(aTokenizer) || ShowTablesStatement::recognize(aTokenizer)
        || InsertTableStatement::recognize(aTokenizer) || SelectStatement::recognize(aTokenizer)
        || UpdateRowsStatement::recognize(aTokenizer) || DeleteRowsStatement::recognize(aTokenizer)
        || ShowIndexesStatement::recognize(aTokenizer) || ShowTableIndexStatement::recognize(aTokenizer)
        || UpdateTableStatement::recognize(aTokenizer)) {
        return this;
    }
    return nullptr;
}

Statement* SQLProcessor::makeStatement(Tokenizer &aTokenizer, StatusResult &aResult) {
    Statement* aStatement = nullptr;
    if (UpdateTableStatement::recognize(aTokenizer)) {
       aStatement = new UpdateTableStatement(appRunning->getDatabaseInUse());
    }
    if (CreateTableStatement::recognize(aTokenizer)) {
       aStatement = new CreateTableStatement(appRunning->getDatabaseInUse());
    }
    if (DropTableStatement::recognize(aTokenizer)) {
        aStatement = new DropTableStatement(appRunning->getDatabaseInUse());
    }
    if (ShowTablesStatement::recognize(aTokenizer)) {
        aStatement = new ShowTablesStatement(appRunning->getDatabaseInUse());
    }
    if (DescribeStatement::recognize(aTokenizer)) {
        aStatement = new DescribeStatement(appRunning->getDatabaseInUse());
    }
    if (InsertTableStatement::recognize(aTokenizer)) {
        aStatement = new InsertTableStatement(appRunning->getDatabaseInUse());
    }
    if (SelectStatement::recognize(aTokenizer)) {
        aStatement = new SelectStatement(appRunning->getDatabaseInUse());
    }
    if (UpdateRowsStatement::recognize(aTokenizer)) {
        aStatement = new UpdateRowsStatement(appRunning->getDatabaseInUse());
    }
    if (DeleteRowsStatement::recognize(aTokenizer)) {
        aStatement = new DeleteRowsStatement(appRunning->getDatabaseInUse());
    }
    if (ShowIndexesStatement::recognize(aTokenizer)) {
        aStatement = new ShowIndexesStatement(appRunning->getDatabaseInUse());
    }
    if (ShowTableIndexStatement::recognize(aTokenizer)) {
        aStatement = new ShowTableIndexStatement(appRunning->getDatabaseInUse());
    }

    if (aStatement) {
        aStatement->parse(aTokenizer);
    }
    return aStatement;
}

StatusResult  SQLProcessor::run(Statement *aStmt) {
    return aStmt->run(output);
}

}
