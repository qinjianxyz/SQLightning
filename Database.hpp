//
//  Database.hpp
//  Developed by Jian Qin & Xiaoqiang Qi
//  Mentored by Rick Gessner
//

#ifndef Database_hpp
#define Database_hpp

#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <map>

#include "Joins.hpp"
#include "CommandView.hpp"
#include "Tokenizer.hpp"
#include "Config.hpp"
#include "Helpers.hpp"
#include "TabularView.hpp"
#include "describeView.hpp"
#include "Storage.hpp"
#include "Entity.hpp"
#include "Row.hpp"
#include "Query.hpp"
#include "Compare.hpp"
#include "SelectView.hpp"
#include "IndexMeta.hpp"
#include "Index.hpp"

namespace SQLightning {

/**
 This class contains meta information for the database such as version entity map
 */
class DatabaseMeta : public Storable {
public:
    StatusResult  encode(std::ostream &anOutput) const override;
    StatusResult  decode(std::istream &anInput) override;
    ~DatabaseMeta() {}
    std::unordered_map<std::string, size_t> entityList;
    size_t version;
};

/**
 The database class is the main controller of table operations.
 */
class Database : public Storage {
public:
    Database(std::string aPath, std::ostream &anOutput);
    ~Database();
    
    StatusResult    changeEntity(const std::string& aTableName, bool addField, const std::string& aFieldName,
                                 const DataTypes aType = DataTypes::no_type, const size_t aSize = 0);
    StatusResult    createTable(const Entity& anEntity);
    StatusResult    describeTable(const std::string &aName);
    StatusResult    dropTable(const std::string &aName);
    StatusResult    dumpTable(const std::string &aName);
    StatusResult    showTables();
    StatusResult    updateRows(const Query& aQuery, const KeyValues& aMap);
    StatusResult    deleteRows(const Query& aQuery);
    StatusResult    insertRow(const std::string& theTableName, const std::vector<std::string>& columns, RowCollection rows);
    StatusResult    showQuery(const Query& aQuery);
    StatusResult    tableExists(const std::string aName);
    StatusResult    showIndexes();
    StatusResult    showTableIndex(const std::string& aTableName, const std::vector<std::string> fieldNames);
    DatabaseMeta    getMeta() const { return meta; }
    std::string     getPath() const { return path; }
    
    
protected:
    StatusResult    readIndexMeta();
    StatusResult    writeIndexMeta();
    StatusResult    readMeta();
    StatusResult    writeMeta();
    StatusResult    storeIndex(const std::string& aTableName, const std::string& aFieldName, const std::string& aKey, const int& aBlockNumber);
    StatusResult    joinTables(const Query& aQuery, RowCollection& rows);
    StatusResult    joinEntity(const Query& aQuery, Entity& anEntity);
    StatusResult    selectRowsByBruteForce(const Query& aQuery, RowCollection& rows);
    StatusResult    selectRowsByIndex(const Query& aQuery, RowCollection& rows, const std::string& aFieldName);
    StatusResult    filterRows(const Query& aQuery, RowCollection& rows);
    StatusResult    orderRows(const Query& aQuery, RowCollection& rows);
    StatusResult    limitRows(const Query& aQuery, RowCollection& rows);
    StatusResult    updateRowsContent(const Query& aQuery, const KeyValues& aMap, RowCollection rows);
    StatusResult    combineJoinedRows(const Join& aJoin, RowCollection& rows, RowCollection& theSecondRowCollection);
    StatusResult    combineJoinRowBySecondaryRow(const Join& aJoin, std::shared_ptr<Row>& thePrime, std::shared_ptr<Row>& theSecond, RowCollection& theFinalOutput, size_t& theCount);
    StatusResult    writeJoinedRow(const Join& aJoin, std::shared_ptr<Row>& aRow, const KeyValuePair& aPair);
    
    
    DatabaseMeta    meta;
    std::string     path;
    std::ostream    &output;
    std::shared_ptr<IndexMeta> theIndexMeta;
};

}
#endif /* Database_hpp */
