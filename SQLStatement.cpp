//
//  SQLStatement.cpp
//  Developed by Jian Qin & Xiaoqiang Qi
//  Mentored by Rick Gessner
//

#include "SQLStatement.hpp"

namespace SQLightning {

StatusResult UpdateTableStatement::parse(Tokenizer &aTokenizer) {
    
    // parse table name
    aTokenizer.next(2);
    tableName = aTokenizer.current().data;
    aTokenizer.next();
    
    // parse action
    if (aTokenizer.current().keyword == Keywords::drop_kw) {
        // change type
        add = false;
        aTokenizer.next();
        // parse field name
        field = aTokenizer.current().data;
    } else {
        // add case
        if (aTokenizer.current().keyword != Keywords::add_kw) {
            return StatusResult{ unexpectedKeyword };
        }
        
        // move on
        aTokenizer.next();
        
        // parse field name
        field = aTokenizer.current().data;
        aTokenizer.next();
        
        // parse type and size
        if (aTokenizer.current().type != TokenType::keyword) {
            return StatusResult{ illegalIdentifier };
        }
        type = Helpers::keywordToDatatype(aTokenizer.current().keyword);
    }
    aTokenizer.skipTo(";");
    aTokenizer.next();
    return StatusResult{};
}


StatusResult InsertTableStatement::parse(Tokenizer &aTokenizer) {
    // parse table name
    aTokenizer.next(2);
    tableName = aTokenizer.current().data;
    aTokenizer.next();
    
    // parse column
    if (!parseColumns(aTokenizer)) {
        return StatusResult{invalidCommand};
    }
    
    // skip to values
    aTokenizer.skipTo(Keywords::values_kw);
    aTokenizer.next();
    
    // parse row values
    if (!parseRows(aTokenizer)) {
        return StatusResult{invalidCommand};
    }
    
    return StatusResult{};
}

std::string InsertTableStatement::parseValue(Tokenizer& aTokenizer) {
    std::string theValue = aTokenizer.current().data;
    aTokenizer.next();
    return theValue;
}

StatusResult InsertTableStatement::parseRow(Tokenizer& aTokenizer) {
    std::shared_ptr<Row> theRow = std::make_shared<Row>();
    theRow->setTableName(tableName);
    rows.push_back(theRow);
    size_t theIndex{0};
    if (aTokenizer.skipIf('(')) {
        bool hasComma = true;
        while (hasComma) {
            aTokenizer.skipIf('\'');
            aTokenizer.skipIf('\"');
            std::string theValue = parseValue(aTokenizer);
            rows[rows.size()-1]->set(columns[theIndex++], theValue);
            aTokenizer.skipIf('\'');
            aTokenizer.skipIf('\"');
            hasComma = aTokenizer.skipIf(',');
        }
        if (!aTokenizer.skipIf(')')) {
            return StatusResult{invalidCommand};
        }
    }
    
    return StatusResult{};
}

StatusResult InsertTableStatement::parseRows(Tokenizer& aTokenizer) {
    bool hasMore = true;
    while (hasMore) {
        if (!parseRow(aTokenizer)) {
            return StatusResult{invalidCommand};
        }
        if (!aTokenizer.skipIf(',')) {
            hasMore = false;
        }
    }
    return StatusResult{};
}

StatusResult InsertTableStatement::parseColumns(Tokenizer& aTokenizer) {
    if (aTokenizer.skipIf('(')) {
        bool hasComma = true;
        while (hasComma && parseColumn(aTokenizer)) {
            hasComma = aTokenizer.skipIf(',');
        }
        if (aTokenizer.skipIf(')')) {
            return StatusResult{};
        }
    }
    return StatusResult{invalidCommand};
}

StatusResult InsertTableStatement::parseColumn(Tokenizer& aTokenizer) {
    aTokenizer.skipIf('\'');
    aTokenizer.skipIf('\"');
    columns.push_back(aTokenizer.current().data);
    aTokenizer.skipIf('\'');
    aTokenizer.skipIf('\"');
    aTokenizer.next();
    return StatusResult{};
}

StatusResult CreateTableStatement::parse(Tokenizer& aTokenizer) {
    // parse table name
    aTokenizer.next(2);
    tableName = aTokenizer.current().data;
    theEntity.setName(tableName);
    aTokenizer.next();
    
    // parse entity
    return parseEntity(aTokenizer);
}

StatusResult CreateTableStatement::parseEntity(Tokenizer& aTokenizer) {
    
    StatusResult result;
    if (aTokenizer.skipIf('(')) {
        bool hasComma = true;
        while (hasComma && parseAttribute(aTokenizer)) {
            hasComma = aTokenizer.skipIf(',');
        }
        if (aTokenizer.skipIf(')')) {
            return StatusResult{};
        }
    }
    
    return StatusResult{invalidCommand};
}

StatusResult CreateTableStatement::parseAttribute(Tokenizer& aTokenizer) {
    StatusResult theResult;
    Attribute anAttr;
    
    // parse data
    if (!parseAttributeData(aTokenizer, anAttr)) {
        return StatusResult{ invalidCommand };
    }
    // parse data type
    if (!parseAttributeDatatype(aTokenizer, anAttr)) {
        return StatusResult{ invalidCommand };
    }
    // parse size (optional)
    if (aTokenizer.skipIf('(')) {
        if (!parseAttributeSize(aTokenizer, anAttr)) {
            return StatusResult{ invalidCommand };
        }
    }
    
    // parse properties
    while ("," != aTokenizer.peek(0).data) {
        if (!parseAttributeProperty(aTokenizer, anAttr)) {
            break;
        }
    }
    theEntity.addAttribute(anAttr);
    return theResult;
}

StatusResult CreateTableStatement::parseAttributeData(Tokenizer& aTokenizer, Attribute& anAttr) {
    anAttr.setName(aTokenizer.current().data);
    aTokenizer.next();
    return StatusResult{};
}

StatusResult CreateTableStatement::parseAttributeDatatype(Tokenizer& aTokenizer, Attribute& anAttr) {
    if (!Helpers::isDatatype(aTokenizer.current().keyword)) {
        return StatusResult{ invalidCommand };
    }
    anAttr.setDataType(Helpers::keywordToDatatype(aTokenizer.current().keyword));
    aTokenizer.next();
    return StatusResult{};
}

StatusResult CreateTableStatement::parseAttributeSize(Tokenizer& aTokenizer, Attribute& anAttr) {
    anAttr.setSize(stoi(aTokenizer.current().data));
    aTokenizer.next();
    if (aTokenizer.skipIf(')')) {
        return StatusResult{};
    }
    return StatusResult{ invalidCommand };
}


StatusResult CreateTableStatement::parseAttributeProperty(Tokenizer& aTokenizer, Attribute& anAttr) {
    switch (aTokenizer.current().keyword)
    {
        case Keywords::auto_increment_kw:
            anAttr.setAutoIncrement(true);
            aTokenizer.next();
            break;
        case Keywords::not_kw:
            aTokenizer.next();
            if (aTokenizer.skipIf(Keywords::null_kw)) {
                anAttr.setNullable(false);
                break;
            }
            return StatusResult{ invalidCommand };
        case Keywords::primary_kw:
            aTokenizer.next();
            if (aTokenizer.skipIf(Keywords::key_kw)) {
                anAttr.setPrimaryKey(true);
                break;
            }
            return StatusResult{ invalidCommand };
            
        default:
            return StatusResult{invalidCommand};
    }
    
    return StatusResult{};
}

StatusResult SelectStatement::parse(Tokenizer& aTokenizer) {
    StatusResult theRes;
    
    // skip "select"
    aTokenizer.next(1);
    
    // parse columns
    theRes = Helpers::parseComma(aTokenizer, theQuery.columns);
    if (!theRes) return theRes;
    
    // skip from
    if (!aTokenizer.skipIf(Keywords::from_kw)) {
        return StatusResult{ keywordExpected };
    }
    // parse table name
    theQuery.tableName = aTokenizer.current().data;
    aTokenizer.next();
    
    theRes = parseSearchCriteria(aTokenizer);
    if (!theRes) return theRes;
    
    return theRes;
}

StatusResult SelectStatement::parseSearchCriteria(Tokenizer& aTokenizer) {
    StatusResult theRes;
    while (aTokenizer.more()) {
        theRes = parseClause(aTokenizer);
        if (!theRes) return theRes;
    }
    return theRes;
}

StatusResult SelectStatement::parseClause(Tokenizer& aTokenizer) {
    static std::map<Keywords, ClauseParser> parserMap {
        {Keywords::where_kw, &SelectStatement::parseWhereClause},
        {Keywords::limit_kw, &SelectStatement::parseLimitClause},
        {Keywords::order_kw, &SelectStatement::parseOrderClause},
        {Keywords::left_kw,  &SelectStatement::parseJoinClause},
        {Keywords::right_kw, &SelectStatement::parseJoinClause}
    };
    if (!parserMap.count(aTokenizer.current().keyword)) return StatusResult{ keywordExpected };
    
    return (this->*(parserMap[aTokenizer.current().keyword]))(aTokenizer);
}

// jointype JOIN tablename ON table1.field=table2.field
StatusResult SelectStatement::parseJoinClause(Tokenizer &aTokenizer) {
    Token &theToken = aTokenizer.current();
    StatusResult theRes{joinTypeExpected}; //add joinTypeExpected to your errors file if missing...
    theQuery.joins.inUse = true;

    if (!Join::gJoinTypes.count(theToken.keyword)) {
        theRes = StatusResult{ joinTypeExpected };
        return theRes;
    }
    theQuery.joins.joinType = theToken.keyword;
    aTokenizer.next(1); // yank the 'join-type' token (e.g. left, right)
    if (!aTokenizer.skipIf(Keywords::join_kw)) {
        theRes = StatusResult{ keywordExpected };
        return theRes;
    }
    // parse secoindary table name
    theRes = parseField(aTokenizer, theQuery.joins.secondaryTable);
    if (!theRes) return theRes;
    // skip "on"
    if (!aTokenizer.skipIf(Keywords::on_kw)) {
        theRes = StatusResult{ keywordExpected };
        return theRes;
    }
    // parse left operand
    theRes = parseTableField(aTokenizer, theQuery.tableName, theQuery.joins.lhs);
    if (!theRes) return theRes;
    // skip "="
    if (!aTokenizer.skipIf('=')) {
        theRes = StatusResult{ keywordExpected };
        return theRes;
    }
    // parse right operand
    theRes = parseTableField(aTokenizer, theQuery.joins.secondaryTable, theQuery.joins.rhs);
    if (!theRes) return theRes;

    return theRes;
}

StatusResult SelectStatement::parseTableField(Tokenizer& aTokenizer, std::string& aTableName, std::string& aString) {
    if (aTokenizer.current().data == aTableName) {
        aTokenizer.next();
        if (aTokenizer.current().data == ".") {
            aTokenizer.next();
            aString = aTokenizer.current().data;
            aTokenizer.next();
            return aString.length() > 0 ? StatusResult{} : StatusResult{ syntaxError };
        }
        
    }
    return StatusResult{ syntaxError };
}
    
StatusResult SelectStatement::parseField(Tokenizer& aTokenizer, std::string& aString) {
    aString = aTokenizer.current().data;
    aTokenizer.next();
    return aString.length() > 0 ? StatusResult{} : StatusResult{ identifierExpected };
}



StatusResult SelectStatement::parseOrderClause(Tokenizer& aTokenizer) {
    StatusResult theRes;
    if (aTokenizer.skipIf(Keywords::order_kw)) {
        if (aTokenizer.skipIf(Keywords::by_kw)) {
            theRes = Helpers::parseComma(aTokenizer, theQuery.orders);
            if (!theRes) return theRes;
        }
        else {
            return StatusResult{ keywordExpected };
        }
    }
    else {
        return StatusResult{ keywordExpected };
    }
    return theRes;
}

StatusResult SelectStatement::parseWhereClause(Tokenizer& aTokenizer) {
    if (aTokenizer.skipIf(Keywords::where_kw)) {
        Entity theEntity;
        if (!db->readEntity(db->getMeta().entityList[theQuery.tableName], theEntity)) {
            return StatusResult{ unknownEntity };
        }
        theQuery.filters.parse(aTokenizer, theEntity);
    }
    else {
        return StatusResult{ keywordExpected };
    }
    return StatusResult{};
}

StatusResult SelectStatement::parseLimitClause(Tokenizer& aTokenizer) {
    StatusResult theRes;
    if (aTokenizer.skipIf(Keywords::limit_kw)) {
        std::vector<std::string> theStrVec;
        theRes = Helpers::parseComma(aTokenizer, theStrVec);
        if (!theRes) return theRes;
        for (auto& str : theStrVec) {
            theQuery.limits.push_back(stoi(str));
        }
    }
    else {
        return StatusResult{ keywordExpected };
    }
    return theRes;
}

StatusResult UpdateRowsStatement::parse(Tokenizer& aTokenizer) {
    StatusResult theRes;
    
    aTokenizer.next(1);  // skip "update"
    theQuery.tableName = aTokenizer.current().data;
    aTokenizer.next(1);  // skip table name
    aTokenizer.next(1);  // skip "set"
    theRes = parseInputValues(aTokenizer); // parse input values
    if (!theRes) return theRes;
    theRes = parseSearchCriteria(aTokenizer); // parse input values
    if (!theRes) return theRes;
    
    return theRes;
}

StatusResult UpdateRowsStatement::parseInputValues(Tokenizer& aTokenizer) {
    StatusResult theRes;
    while (aTokenizer.more()) {
        theRes = parseInputValue(aTokenizer);
        if (!theRes) return theRes;
        
        if (!aTokenizer.more() || !aTokenizer.skipIf(',')) break;
    }
    return theRes;
    
}

StatusResult UpdateRowsStatement::parseInputValue(Tokenizer& aTokenizer) {
    StatusResult theRes;
    std::string theKey = aTokenizer.current().data;
    aTokenizer.next(); // skip the key
    if (!aTokenizer.skipIf('=')) {
        return StatusResult{ syntaxError };
    }
    Value theValue = aTokenizer.current().data;
    aTokenizer.next(); // skip the value
    inputValues[theKey] = theValue;
    return theRes;
}

StatusResult DeleteRowsStatement::parse(Tokenizer& aTokenizer) {
    StatusResult theRes;
    
    aTokenizer.next(2);  // skip "delete from"
    theQuery.tableName = aTokenizer.current().data; // parse table name
    aTokenizer.next(1);  // skip table name
    theRes = parseSearchCriteria(aTokenizer); // parse input values
    if (!theRes) return theRes;
    
    return theRes;
}


StatusResult DropTableStatement::parse(Tokenizer& aTokenizer) {
    aTokenizer.next(2);
    tableName = aTokenizer.current().data;
    aTokenizer.next();
    return StatusResult{};
}

StatusResult ShowTablesStatement::parse(Tokenizer& aTokenizer) {
    aTokenizer.next(2);
    return StatusResult{};
}

StatusResult DescribeStatement::parse(Tokenizer& aTokenizer) {
    aTokenizer.next();
    tableName = aTokenizer.current().data;
    aTokenizer.next();
    return StatusResult{};
}

StatusResult ShowIndexesStatement::parse(Tokenizer& aTokenizer) {
    aTokenizer.next(2);
    return StatusResult{};
}

StatusResult ShowTableIndexStatement::parse(Tokenizer& aTokenizer) {
    StatusResult theRes;
    aTokenizer.next(2); // skip "show index"
    theRes = Helpers::parseComma(aTokenizer, fieldNames); // parse field names
    aTokenizer.next(1); // skip "from"
    tableName = aTokenizer.current().data;  // parse table name
    aTokenizer.next();

    return theRes;
}

StatusResult CreateTableStatement::run(std::ostream& aStream) const {
    if (!db) {
        return StatusResult(Errors::noDatabaseSpecified);
    }
    return db->createTable(theEntity);
}

StatusResult DropTableStatement::run(std::ostream& aStream) const {
    if (!db) {
        return StatusResult(Errors::noDatabaseSpecified);
    }
    return db->dropTable(tableName);
}

StatusResult ShowTablesStatement::run(std::ostream& aStream) const {
    if (!db) {
        return StatusResult(Errors::noDatabaseSpecified);
    }
    return db->showTables();
}

StatusResult DescribeStatement::run(std::ostream& aStream) const {
    if (!db) {
        return StatusResult(Errors::noDatabaseSpecified);
    }
    return db->describeTable(tableName);
}

StatusResult InsertTableStatement::run(std::ostream& aStream) const {
    if (!db) {
        return StatusResult(Errors::noDatabaseSpecified);
    }
    return db->insertRow(tableName, columns, rows);
}

StatusResult SelectStatement::run(std::ostream& aStream) const {
    if (!db) {
        return StatusResult(Errors::noDatabaseSpecified);
    }
    return db->showQuery(theQuery);
}

StatusResult UpdateRowsStatement::run(std::ostream& aStream) const {
    if (!db) {
        return StatusResult(Errors::noDatabaseSpecified);
    }
    return db->updateRows(theQuery, inputValues);
}

StatusResult DeleteRowsStatement::run(std::ostream& aStream) const {
    if (!db) {
        return StatusResult(Errors::noDatabaseSpecified);
    }
    return db->deleteRows(theQuery);
}

StatusResult UpdateTableStatement::run(std::ostream& aStream) const {
    if (!db) {
        return StatusResult(Errors::noDatabaseSpecified);
    }
    return db->changeEntity(tableName, add, field, type, size);
}

StatusResult ShowIndexesStatement::run(std::ostream& aStream) const {
    if (!db) {
        return StatusResult(Errors::noDatabaseSpecified);
    }
    return db->showIndexes();
}

StatusResult ShowTableIndexStatement::run(std::ostream& aStream) const {
    if (!db) {
        return StatusResult(Errors::noDatabaseSpecified);
    }
    return db->showTableIndex(tableName, fieldNames);
}

bool CreateTableStatement::recognize(Tokenizer& aTokenizer) {
    return Helpers::isEqualCmd(aTokenizer, { Keywords::create_kw, Keywords::table_kw, Keywords::unknown_kw });
}

bool UpdateTableStatement::recognize(Tokenizer &aTokenizer) {
    return Helpers::isEqualCmd(aTokenizer, { Keywords::alter_kw, Keywords::table_kw, Keywords::unknown_kw });
}

bool DropTableStatement::recognize(Tokenizer& aTokenizer) {
    return Helpers::isEqualCmd(aTokenizer, { Keywords::drop_kw, Keywords::table_kw, Keywords::unknown_kw });
}

bool DescribeStatement::recognize(Tokenizer& aTokenizer) {
    return Helpers::isEqualCmd(aTokenizer, { Keywords::describe_kw, Keywords::unknown_kw });
}

bool ShowTablesStatement::recognize(Tokenizer& aTokenizer) {
    return Helpers::isEqualCmd(aTokenizer, { Keywords::show_kw, Keywords::tables_kw });
}

bool InsertTableStatement::recognize(Tokenizer& aTokenizer) {
    return Helpers::isEqualCmd(aTokenizer, { Keywords::insert_kw, Keywords::into_kw, Keywords::unknown_kw });
}

bool SelectStatement::recognize(Tokenizer& aTokenizer) {
    return Helpers::isEqualCmd(aTokenizer, { Keywords::select_kw});
}

bool UpdateRowsStatement::recognize(Tokenizer& aTokenizer) {
    return Helpers::isEqualCmd(aTokenizer, { Keywords::update_kw });
}

bool DeleteRowsStatement::recognize(Tokenizer& aTokenizer) {
    return Helpers::isEqualCmd(aTokenizer, { Keywords::delete_kw, Keywords::from_kw });
}

bool ShowIndexesStatement::recognize(Tokenizer& aTokenizer) {
    return Helpers::isEqualCmd(aTokenizer, { Keywords::show_kw, Keywords::indexes_kw });
}

bool ShowTableIndexStatement::recognize(Tokenizer& aTokenizer) {
    return Helpers::isEqualCmd(aTokenizer, { Keywords::show_kw, Keywords::index_kw });
}

}
