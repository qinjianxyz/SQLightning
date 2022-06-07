//
//  SQLStatement.hpp
//  Developed by Jian Qin & Xiaoqiang Qi
//  Mentored by Rick Gessner
//

#ifndef SQLStatement_hpp
#define SQLStatement_hpp

#include <stdio.h>
#include <algorithm>

#include "Statement.hpp"
#include "Entity.hpp"
#include "Database.hpp"
#include "Errors.hpp"
#include "Tokenizer.hpp"
#include "Helpers.hpp"

namespace SQLightning {

class Statement;
class DBProcessor;
class Entity;
class Database;
struct StatusResult;

/**
 The base class for all SQL Statements. Each subclass contains all information about a table operation
 */
class SQLStatement : public Statement {
public:
    SQLStatement(Database* aDatabase, Keywords aStatementType = Keywords::unknown_kw, std::string aName = "")
    : Statement(aStatementType), tableName(aName), db(aDatabase) {}
    virtual ~SQLStatement() {}
    
    const char* getStatementName() const override { return tableName.c_str(); }
protected:
    std::string tableName;
    Database* db;
};

using EntityOpt = std::optional<Entity>;
using AttributeOpt = std::optional<Attribute>;

class UpdateTableStatement : public SQLStatement {
public:
    UpdateTableStatement(Database* aDatabase, Keywords aStatementType = Keywords::update_kw, std::string aName = ""
                        , std::string aField = "", DataTypes aType = DataTypes::no_type) : SQLStatement(aDatabase, aStatementType, aName)
                        , field(aField), type(aType), add(true), size(0) {}
    
    virtual ~UpdateTableStatement() {}
    StatusResult parse(Tokenizer& aTokenizer) override;
    StatusResult run(std::ostream& aStream) const override;
    static bool  recognize(Tokenizer& aTokenizer);

    bool        add; // true means add, false means delete
    size_t      size;
    std::string field;
    DataTypes   type;
};


class CreateTableStatement : public SQLStatement {
public:
    CreateTableStatement(Database* aDatabase, Keywords aStatementType = Keywords::create_kw, std::string aName = "")
    : SQLStatement(aDatabase, aStatementType, aName), theEntity(aName) {}
    virtual ~CreateTableStatement() {}
    StatusResult parse(Tokenizer& aTokenizer) override;
    StatusResult run(std::ostream& aStream) const override;
    static bool recognize(Tokenizer& aTokenizer);
    
protected:
    StatusResult parseEntity(Tokenizer& aTokenizer);
    StatusResult parseAttribute(Tokenizer& aTokenizer);
    StatusResult parseAttributeData(Tokenizer& aTokenizer, Attribute& anAttr);
    StatusResult parseAttributeDatatype(Tokenizer& aTokenizer, Attribute& anAttr);
    StatusResult parseAttributeSize(Tokenizer& aTokenizer, Attribute& anAttr);
    StatusResult parseAttributeProperty(Tokenizer& aTokenizer, Attribute& anAttr);
    
    Entity theEntity;
};

class InsertTableStatement : public SQLStatement {
public:
    InsertTableStatement(Database* aDatabase, Keywords aStatementType = Keywords::insert_kw, std::string aName = "")
    : SQLStatement(aDatabase, aStatementType, aName) {}
    virtual ~InsertTableStatement() {}
    
    StatusResult parse(Tokenizer& aTokenizer) override;
    StatusResult run(std::ostream& aStream) const override;
    static bool recognize(Tokenizer& aTokenizer);
    
protected:
    std::string  parseValue(Tokenizer& aTokenizer);
    StatusResult parseRow(Tokenizer& aTokenizer);
    StatusResult parseRows(Tokenizer& aTokenizer);
    StatusResult parseColumn(Tokenizer& aTokenizer);
    StatusResult parseColumns(Tokenizer& aTokenizer);
    std::vector<std::string>    columns;
    RowCollection               rows;
};


class SelectStatement : public SQLStatement {
public:
    SelectStatement(Database* aDatabase, Keywords aStatementType = Keywords::select_kw, std::string aName = "")
    : SQLStatement(aDatabase, aStatementType, aName) {}
    virtual ~SelectStatement() {}
    StatusResult parse(Tokenizer& aTokenizer) override;
    StatusResult run(std::ostream& aStream) const override;
    static bool recognize(Tokenizer& aTokenizer);
protected:
    StatusResult parseField(Tokenizer& aTokenizer, std::string& aString);
    StatusResult parseTableField(Tokenizer& aTokenizer, std::string& aTableName, std::string& aString);
    StatusResult parseSearchCriteria(Tokenizer& aTokenizer);
    StatusResult parseClause(Tokenizer& aTokenizer);
    StatusResult parseOrderClause(Tokenizer& aTokenizer);
    StatusResult parseWhereClause(Tokenizer& aTokenizer);
    StatusResult parseLimitClause(Tokenizer& aTokenizer);
    StatusResult parseJoinClause(Tokenizer& aTokenizer);
    Query theQuery;
};
using ClauseParser = StatusResult(SelectStatement::*)(Tokenizer& aTokenizer);

class UpdateRowsStatement : public SelectStatement {
public:
    UpdateRowsStatement(Database* aDatabase, Keywords aStatementType = Keywords::select_kw, std::string aName = "")
        : SelectStatement(aDatabase, aStatementType, aName) {}
    virtual ~UpdateRowsStatement() {}

    StatusResult parse(Tokenizer& aTokenizer) override;
    StatusResult run(std::ostream& aStream) const override;
    static bool recognize(Tokenizer& aTokenizer);

protected:
    StatusResult parseInputValues(Tokenizer& aTokenizer);
    StatusResult parseInputValue(Tokenizer& aTokenizer);
    KeyValues inputValues;
};

class DeleteRowsStatement : public SelectStatement {
public:
    DeleteRowsStatement(Database* aDatabase, Keywords aStatementType = Keywords::select_kw, std::string aName = "")
        : SelectStatement(aDatabase, aStatementType, aName) {}
    virtual ~DeleteRowsStatement() {}

    StatusResult parse(Tokenizer& aTokenizer) override;
    StatusResult run(std::ostream& aStream) const override;
    static bool recognize(Tokenizer& aTokenizer);

protected:


};

class DropTableStatement : public SQLStatement {
public:
    DropTableStatement(Database* aDatabase, Keywords aStatementType = Keywords::drop_kw, std::string aName = "")
    : SQLStatement(aDatabase, aStatementType, aName){}
    virtual ~DropTableStatement() {}
    
    StatusResult parse(Tokenizer& aTokenizer) override;
    StatusResult run(std::ostream& aStream) const override;
    static bool recognize(Tokenizer& aTokenizer);
protected:
    
};

class ShowTablesStatement : public SQLStatement {
public:
    ShowTablesStatement(Database* aDatabase, Keywords aStatementType = Keywords::show_kw, std::string aName = "")
    : SQLStatement(aDatabase, aStatementType, aName) {}
    virtual ~ShowTablesStatement() {}
    
    StatusResult parse(Tokenizer& aTokenizer) override;
    StatusResult run(std::ostream& aStream) const override;
    static bool recognize(Tokenizer& aTokenizer);
protected:
    
};

class DescribeStatement : public SQLStatement {
public:
    DescribeStatement(Database* aDatabase, Keywords aStatementType = Keywords::describe_kw, std::string aName = "")
    : SQLStatement(aDatabase, aStatementType, aName) {}
    virtual ~DescribeStatement() {}
    
    StatusResult parse(Tokenizer& aTokenizer) override;
    StatusResult run(std::ostream& aStream) const override;
    static bool recognize(Tokenizer& aTokenizer);
protected:
    
};

class ShowIndexesStatement : public SQLStatement {
public:
    ShowIndexesStatement(Database* aDatabase, Keywords aStatementType = Keywords::show_kw, std::string aName = "")
        : SQLStatement(aDatabase, aStatementType, aName) {}
    virtual ~ShowIndexesStatement() {}

    StatusResult parse(Tokenizer& aTokenizer) override;
    StatusResult run(std::ostream& aStream) const override;
    static bool recognize(Tokenizer& aTokenizer);
protected:

};

class ShowTableIndexStatement : public SQLStatement {
public:
    ShowTableIndexStatement(Database* aDatabase, Keywords aStatementType = Keywords::show_kw, std::string aName = "")
        : SQLStatement(aDatabase, aStatementType, aName) {}
    virtual ~ShowTableIndexStatement() {}

    StatusResult parse(Tokenizer& aTokenizer) override;
    StatusResult run(std::ostream& aStream) const override;
    static bool recognize(Tokenizer& aTokenizer);
protected:
    std::vector<std::string> fieldNames;
};

}
#endif /* SQLStatement_hpp */
