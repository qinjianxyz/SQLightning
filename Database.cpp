//
//  Database.cpp
//  Developed by Jian Qin & Xiaoqiang Qi
//  Mentored by Rick Gessner
//

#include "Database.hpp"
#include "EntityView.hpp"
#include <string>

namespace ECE141 {

Database::Database(std::string aPath, std::ostream &anOutput)
: Storage(aPath), path(aPath), output(anOutput) {
    theIndexMeta = std::make_shared<IndexMeta>(IndexMeta::createIndexMeta());
    readMeta();
    readIndexMeta();
    meta.version = 1;
}

Database::~Database() {
    writeIndexMeta();
}

StatusResult Database::storeIndex(const std::string& aTableName, const std::string& aFieldName, const std::string& aKey, const int& aBlockNumber) {
    StatusResult theRes;
    Block theBlock(BlockType::index_block);
    Index theIndex;
    // if the index has not been created
    if (!theIndexMeta->isContainField(aTableName, aFieldName)) {
        int theIndexBlockNumber = static_cast<int>(findNextFreeBlock());
        theRes = theIndexMeta->addNewFieldIndex(aTableName, aFieldName, static_cast<int>(theIndexBlockNumber));
        if (!theRes) return theRes;
        
        theIndex.set(aKey, aBlockNumber);
        theRes = pushPayload(theBlock, theIndex);
        if (!theRes) return theRes;
        theRes = writeBlock(theIndexBlockNumber, theBlock);
        return theRes;
    }
    
    // read the block first
    std::vector<int> theIndexBlockVector;
    theIndexMeta->getFieldIndex(aTableName, aFieldName, theIndexBlockVector);
    int theLastIndex = theIndexBlockVector[theIndexBlockVector.size()-1];
    theRes = readBlock(theLastIndex, theBlock);
    if (!theRes) return theRes;
    
    // read the index object
    std::stringstream ss = pullPayload(theBlock);
    if (ss.str() != "") {
        theRes = theIndex.decode(ss);
        if (!theRes) return theRes;
    }
    
    /**
     New concept: limit by index size
     */
    int theIndexLimit = kBlockSize / 30;
    
    // within space limit
    if (theIndex.size() < theIndexLimit) {
        // set the value in map
        theIndex.set(aKey, aBlockNumber);
        theRes = pushPayload(theBlock, theIndex);
        if (!theRes) return theRes;
        theRes = writeBlock(theLastIndex, theBlock);
    } else {
        // write a new block
        Index theNewIndex;
        int theNextIndexBlockNumber = static_cast<int>(findNextFreeBlock());
        theNewIndex.set(aKey, aBlockNumber);
        theRes = pushPayload(theBlock, theNewIndex);
        if (!theRes) return theRes;
        theRes = writeBlock(theNextIndexBlockNumber, theBlock);
        if (!theRes) return theRes;
        theRes = theIndexMeta->addNewFieldIndex(aTableName, aFieldName, static_cast<int>(theNextIndexBlockNumber));
    }
    return theRes;
}

StatusResult Database::readIndexMeta() {
    StatusResult theRes;
    IndexMetaBlock theBlock;
    theRes = readIndexMetaBlock(theBlock);
    if (theBlock.header.type != static_cast<char>(BlockType::meta_block)) {
        theRes = StatusResult{unknownType};
    }
    if (!theRes) return theRes;
    
    std::stringstream ss;
    ss << theBlock.payload;
    if (ss.str() != "") {
        theIndexMeta->decode(ss);
    }
    return theRes;
}

StatusResult Database::writeIndexMeta() {
    StatusResult theRes;
    std::stringstream ss;
    theIndexMeta->encode(ss);
    IndexMetaBlock theBlock(BlockType::meta_block);
    memcpy(theBlock.payload, ss.str().c_str(), kIndexMetaPayloadSize);
    theRes = writeIndexMetaBlock(theBlock);
    return theRes;
}



StatusResult Database::selectRowsByBruteForce(const Query& aQuery, RowCollection& rows) {
    std::string theTableName = aQuery.tableName;
    if (!tableExists(theTableName)) {
        return StatusResult{ unknownTable };
    }
    each([&](const Block &aBlock, size_t aBlockNum) {
        std::shared_ptr<Row> theRow = std::make_shared<Row>();
        auto theRes = readRow(aBlockNum, *theRow);
        
        if (!theRes) return true;
        if (theRow->getTableName() == theTableName) {
            rows.push_back(theRow);
        }
        return true;
    });
    return StatusResult{};
}

StatusResult Database::selectRowsByIndex(const Query& aQuery, RowCollection& rows, const std::string& aFieldName) {
    StatusResult theRes;
    std::string theTableName = aQuery.tableName;
    if (!tableExists(theTableName)) {
        return StatusResult{ unknownTable };
    }
    std::vector<int> theIndexBlockVector;
    theRes = theIndexMeta->getFieldIndex(theTableName, aFieldName, theIndexBlockVector);
    if (!theRes) return StatusResult{};
    
    for (auto& index : theIndexBlockVector) {
        Block theBlock;
        Index theIndex;
        theRes = readBlock(index, theBlock);
        if (!theRes) return theRes;
        std::stringstream ss = pullPayload(theBlock);
        theIndex.decode(ss);
        for (auto it = theIndex.getData().begin(); it != theIndex.getData().end(); it++) {
            std::shared_ptr<Row> theRow = std::make_shared<Row>();
            theRes = readRow(it->second, *theRow);
            if (!theRes) return theRes;
            rows.push_back(theRow);
        }
    }
    return theRes;
}

StatusResult Database::updateRows(const Query& aQuery, const KeyValues& aMap) {
    
    StatusResult theRes;
    Timer theTimer = Config::getTimer();
    bool theSuccessFlag = false;
    
    std::string theTableName = aQuery.tableName;
    if (!tableExists(theTableName)) {
        return StatusResult{ unknownTable };
    }
    
    RowCollection theRows;
    // select rows
    if (Config::indexMode) {
        theRes = selectRowsByIndex(aQuery, theRows, "id");
    } else {
        theRes = selectRowsByBruteForce(aQuery, theRows);
    }
    
    if (!theRes) return theRes;
    
    // filter rows by the query object
    theRes = filterRows(aQuery, theRows);
    if (!theRes) return theRes;
    
    // wirte new values (stored in aMap) into theRows, and change to true datatype according to entity
    theRes = updateRowsContent(aQuery, aMap, theRows);
    if (!theRes) return theRes;

    // write rows to storage
    for (auto& row : theRows) {
        writeExistingRow(*row);
    }

    theSuccessFlag = true;
    CommandView theView(output, Keywords::update_kw, theSuccessFlag, theRows.size(), theTimer.elapsed());
    theView.show();
    return theRes;
}

StatusResult Database::deleteRows(const Query& aQuery) {
    StatusResult theRes;
    Timer theTimer = Config::getTimer();
    
    bool theSuccessFlag = false;
    std::string theTableName = aQuery.tableName;
    if (!tableExists(theTableName)) {
        return StatusResult{ unknownTable };
    }
    
    RowCollection theRows;
    // select rows
    if (Config::indexMode) {
        theRes = selectRowsByIndex(aQuery, theRows, "id");
    } else {
        theRes = selectRowsByBruteForce(aQuery, theRows);
    }
    if (!theRes) return theRes;
    
    // filter rows by the query object
    theRes = filterRows(aQuery, theRows);
    if (!theRes) return theRes;
    
    // update index
    std::vector<int> theIndexBlockVector;
    theIndexMeta->getFieldIndex(theTableName, "id", theIndexBlockVector);
    
    for (auto& index : theIndexBlockVector) {
        Block theBlock;
        Index theIndex;
        theRes = readBlock(index, theBlock);
        if (!theRes) return theRes;
        
        std::stringstream ss = pullPayload(theBlock);
        theIndex.decode(ss);
        
        for (auto& row: theRows) {
            theIndex.del(row->getNum());
        }
        theRes = pushPayload(theBlock, theIndex);
        if (!theRes) return theRes;
        theRes = writeBlock(index, theBlock);
        if (!theRes) return theRes;
    }
    
    
    // clearBlock
    for (auto& row: theRows) {
        clearRow(row->getNum());
    }
    
    theSuccessFlag = true;
    CommandView theView(output, Keywords::delete_kw, theSuccessFlag, theRows.size(), theTimer.elapsed());
    theView.show();
    return theRes;
}

StatusResult Database::joinTables(const Query &aQuery, RowCollection &rows) {
    StatusResult theRes;
    
    // set up the second query to get every row
    // todo (for pa9): make a new abstraction (interface) of search that does use tablename directly
    RowCollection theSecondRowCollection;
    Query theSecondQuery;
    theSecondQuery.tableName = aQuery.joins.secondaryTable;
    theSecondQuery.columns.push_back("*");
    theRes = selectRowsByBruteForce(theSecondQuery, theSecondRowCollection);
    if (!theRes) return theRes;

    // right join
    if (Keywords::right_kw == aQuery.joins.joinType) {
        rows.swap(theSecondRowCollection);
    }
    theRes = combineJoinedRows(aQuery.joins, rows, theSecondRowCollection);
    if (!theRes) return theRes;
    
    return theRes;
}

StatusResult Database::joinEntity(const Query& aQuery, Entity& anEntity) {
    // todo: logic similar to writeJoinedRow()
    // Assumption: left join or right join do not affect joinEntity()
    StatusResult theRes;
    Entity theSecondaryEntity;
    theRes = readEntity(meta.entityList[aQuery.joins.secondaryTable], theSecondaryEntity);
    if (!theRes) return theRes;

    // add original attributes to set
    std::set<std::string> attributeSet;
    for (auto& attri : anEntity.getAttributes()) {
        attributeSet.insert(attri.getName());
    }

    // iterate all attributes in secondary table
    for (auto& attri : theSecondaryEntity.getAttributes()) {
        std::string theKey = attri.getName();
        // if is matched attribute
        if (theKey == aQuery.joins.rhs) {
            continue;
        }
        // if no duplicate
        if (!attributeSet.count(theKey)) {
            anEntity.addAttribute(attri);
        }
        else { // if duplicate
            auto tmpAttribute = anEntity.getAttribute(theKey);
            anEntity.removeAttribute(theKey);
            tmpAttribute->setName(anEntity.getName() + "." + theKey);
            anEntity.addAttribute(tmpAttribute.value());
            tmpAttribute->setName(aQuery.joins.secondaryTable + "." + theKey);
            anEntity.addAttribute(tmpAttribute.value());
        }
    }
    return theRes;
}

StatusResult Database::showQuery(const Query& aQuery) {
    
    StatusResult theRes;
    Timer theTimer = Config::getTimer();
    bool theSuccessFlag = false;
    
    RowCollection theRows;
    if (Config::indexMode) {
        theRes = selectRowsByIndex(aQuery, theRows, "id");
    } else {
        theRes = selectRowsByBruteForce(aQuery, theRows);
    }
    if (!theRes) return theRes;

    // read entity
    Entity theEntity;
    theRes = readEntity(meta.entityList[aQuery.tableName], theEntity);
    if (!theRes) return theRes;
    
    // where
    theRes = filterRows(aQuery, theRows);
    if (!theRes) return theRes;
    
    // join
    if (aQuery.joins.inUse) {
        theRes = joinTables(aQuery, theRows);
        if (!theRes) return theRes;
        theRes = joinEntity(aQuery, theEntity);
        if (!theRes) return theRes;
    }

    // order by
    theRes = orderRows(aQuery, theRows);
    if (!theRes) return theRes;
    
    // limit number
    theRes = limitRows(aQuery, theRows);
    if (!theRes) return theRes;
    
    // command view
    theSuccessFlag = true;
    CommandView* theView = new CommandView(output, Keywords::select_kw, theSuccessFlag, theRows.size(), theTimer.elapsed());
    
    // show selected rows
    SelectView theSelectView(output);
    theSelectView.addData(aQuery, theRows, theEntity);
    theSelectView.addCommand(theView);
    theSelectView.show();
    return theRes;
}

StatusResult Database::combineJoinedRows(const Join& aJoin, RowCollection& rows, RowCollection& theSecondRowCollection) {
    StatusResult theRes;
    
    RowCollection theFinalOutput;
    for (auto& thePrime : rows) {
        size_t theCount{ 0 }; // number of matches in secondary table
        for (auto& theSecond : theSecondRowCollection) {
            theRes = combineJoinRowBySecondaryRow(aJoin, thePrime, theSecond, theFinalOutput, theCount);
            if (!theRes) return theRes;
        }
        // if no matches in right table
        if (theCount == 0) {
            auto theCombinedRow = std::make_shared<Row>(*thePrime.get());
            theFinalOutput.push_back(theCombinedRow);
        }
    }
    rows.swap(theFinalOutput);
    return theRes;
}

StatusResult Database::combineJoinRowBySecondaryRow(const Join& aJoin, std::shared_ptr<Row>& thePrime, std::shared_ptr<Row>& theSecond, RowCollection& theFinalOutput, size_t& theCount) {
    StatusResult theRes;
    if (thePrime->getData()[aJoin.lhs] == theSecond->getData()[aJoin.rhs]) {
        auto theCombinedRow = std::make_shared<Row>(*thePrime.get());
        for (auto it = theSecond->getData().begin(); it != theSecond->getData().end(); it++) {
            theRes = writeJoinedRow(aJoin, theCombinedRow, *it);
            if (!theRes) return theRes;
        }
        theCount++;  // number of matches in secondary table
        theFinalOutput.push_back(theCombinedRow);
    }
    return theRes;
}

StatusResult Database::writeJoinedRow(const Join& aJoin, std::shared_ptr<Row>& aRow, const KeyValuePair& aPair) {
    StatusResult theRes;
    // if this is the matched column
    if (aPair.first == aJoin.rhs) return theRes;

    std::string theKey = aPair.first;
    if (!aRow->getConstData().count(theKey)) {
        aRow->set(theKey, aPair.second);
    }
    else { // if the key already exists
        auto leftValue = aRow->getValue(theKey);
        theRes = aRow->removeKey(theKey);
        if (!theRes) return theRes;
        aRow->set(aRow->getTableName() + "." + theKey, leftValue.value());
        aRow->set(aJoin.secondaryTable + "." + theKey, aPair.second);
    }
    return theRes;
}

StatusResult DatabaseMeta::encode(std::ostream &anOutput) const {
    StreamEncoder anEncoder(&anOutput);
    anEncoder.encode(static_cast<int>(version));
    for (auto& item : entityList) {
        anEncoder.encode(item.first).encode(static_cast<int>(item.second));
    }
    return StatusResult{};
}

StatusResult Database::readMeta() {
    StatusResult theRes;
    Block theBlock;
    theRes = readBlock(0, theBlock);
    if (theBlock.header.type != static_cast<char>(BlockType::meta_block)) {
        theRes = StatusResult{unknownType};
    }
    if (!theRes) return theRes;
    
    std::stringstream ss;
    ss << theBlock.payload;
    if (ss.str() != "") {
        meta.decode(ss);
    }
    return theRes;
}

StatusResult Database::writeMeta() {
    StatusResult theRes;
    std::stringstream ss;
    meta.encode(ss);
    Block theBlock(BlockType::meta_block);
    memcpy(theBlock.payload, ss.str().c_str(), kPayloadSize);
    theRes = writeBlock(0, theBlock);
    return theRes;
}

StatusResult DatabaseMeta::decode(std::istream &anInput) {
    StreamDecoder aDecoder(&anInput);
    aDecoder.decode(version);
    std::string theName;
    size_t theIndex;
    while (aDecoder.more()) {
        aDecoder.decode(theName).decode(theIndex);
        entityList[theName] = theIndex;
    }
    return StatusResult{};
}

StatusResult Database::createTable(const Entity& anEntity) {
    StatusResult theRes{};
    Timer theTimer = Config::getTimer();
    bool theSuccessFlag = false;
    
    size_t theCount{0};
    size_t theBlockNum = writeEntity(anEntity);
    meta.entityList[anEntity.getName()] = theBlockNum;
    theRes = writeMeta();
    if (!theRes) return theRes;
    
    theSuccessFlag = true;
    theCount++;
    CommandView theView(output, Keywords::create_kw, theSuccessFlag, theCount, theTimer.elapsed());
    theView.show();
    return theRes;
}

StatusResult Database::showIndexes() {
    StatusResult theRes;
    EntityView theView(output, this);
    theView.show();
    return theRes;
}

StatusResult Database::showTableIndex(const std::string& aTableName, const std::vector<std::string> fieldNames) {
    StatusResult theRes;
    std::string theFieldName = fieldNames[0];
    if (!theIndexMeta->isContainField(aTableName, theFieldName)) {
        theRes = StatusResult{ unknownIndex };
        if (!theRes) return theRes;
    }
    
    // get index block number
    std::vector<int> theIndexBlockVector;
    theRes = theIndexMeta->getFieldIndex(aTableName, theFieldName, theIndexBlockVector);
    if (!theRes) return theRes;
    
    // create tabular view
    TabularView theView(output);
    theView.setHeaders(std::vector<std::string>{"key", "block#"});
    
    std::vector<std::vector<std::string>> theData;
    size_t theTotalSize = 0;
    for (auto& index : theIndexBlockVector) {
        // read block
        Block theBlock;
        theRes = readBlock(index, theBlock);
        if (!theRes) return theRes;
        
        // read the index object
        Index theIndex;
        std::stringstream ss = pullPayload(theBlock);
        theIndex.decode(ss);
        if (!theRes) return theRes;
        for (auto it = theIndex.getData().begin(); it != theIndex.getData().end(); it++) {
            theData.push_back(std::vector<std::string> {it->first, std::to_string(it->second)});
            theTotalSize++;
        }
    }
     
    theView.addData(theData);
    theView.show();
     
    
    /*
    RowCollection theRows;
    Query theQuery;
    theQuery.tableName = aTableName;
    theQuery.columns.push_back("*");
    theRes = selectRowsByIndex(theQuery, theRows, "id");
     */
    
    if (!theRes) return theRes;
    output << theTotalSize << " rows in set (" << Config::getTimer().elapsed() <<  "secs)" << std::endl;
    return theRes;
}

StatusResult Database::updateRowsContent(const Query& aQuery, const KeyValues& aMap, RowCollection theRows) {
    // aMap: new values, theRows: row object with old value
    StatusResult theRes;
    // write new values into rows (type is string)
    for (auto& row : theRows) {
        for (auto it = aMap.begin(); it != aMap.end(); it++) {
            row->set(it->first, it->second);
        }
    }
    // read entity
    Entity theEntity;
    theRes = readEntity(meta.entityList[aQuery.tableName], theEntity);
    if (!theRes) return theRes;

    // write true datatype in each row
    std::set<std::string> columns;
    for (auto& item : aMap) {
        columns.insert(item.first);
    }
    theRes = theEntity.writeDatatype(theRows, columns);
    if (!theRes) return theRes;

    return theRes;
}

StatusResult Database::describeTable(const std::string &aName) {
    StatusResult theRes{};
    if (!tableExists(aName)) {
        theRes = StatusResult{unknownTable};
    }
    if (!theRes) return theRes;
    
    Timer theTimer = Config::getTimer();
    // read entity
    size_t theIndex = meta.entityList[aName];
    Entity theEntity;
    theRes = readEntity(theIndex, theEntity);
    if (!theRes) return theRes;
    // show information
    DescribeView theDescribeView(output);
    theDescribeView.addData(theEntity);
    theDescribeView.show();
    
    output << theEntity.getAttributes().size() << " rows in set (" << theTimer.elapsed() << " sec)" << std::endl;
    return theRes;
}

StatusResult Database::dropTable(const std::string &aName) {
    StatusResult theRes{};
    
    if (!tableExists(aName)) {
        theRes = StatusResult{unknownTable};
    }
    if (!theRes) return theRes;
    
    Timer theTimer = Config::getTimer();
    bool theSuccessFlag = false;
    size_t theCount{0};
    
    std::vector<size_t> deleteBlockNum;
    // add entity blocks
    deleteBlockNum.push_back(meta.entityList[aName]);

    // add data blocks
    each([&](const Block &aBlock, size_t aBlockNum) {
        std::string theName = "";
        theRes = readRowName(aBlockNum, theName);
        if (!theRes) return theRes;
        if (theName == aName) {
            deleteBlockNum.push_back(aBlockNum);
        }
        return theRes;
    });
    
    // add index block
    Entity theEntity;
    theRes = readEntity(meta.entityList[aName], theEntity);
    std::string thePrimaryKey = theEntity.getPrimaryKey().value().getName();
    std::vector<int> theIndexBlockVector;
    theIndexMeta->getFieldIndex(aName, thePrimaryKey, theIndexBlockVector);
    theIndexMeta->deleteTableIndex(aName);
    
    // clear index blocks
    for (auto& index : theIndexBlockVector) {
        theRes = clearBlock(index);
        if (!theRes) return theRes;
    }

    // delete data blocks
    for (auto& blockNum : deleteBlockNum) {
        theRes = clearBlock(blockNum);
        if (!theRes) return theRes;
        theCount++;
    }
    
    theSuccessFlag = true;
    meta.entityList.erase(aName);
    CommandView theView(output, Keywords::drop_kw, theSuccessFlag, theCount, theTimer.elapsed());
    theView.show();
    return theRes;
}

StatusResult Database::showTables() {
    EntityView theView(output, this);
    theView.show();
    return StatusResult{};
}


StatusResult Database::dumpTable(const std::string &aName) {
    Timer theTimer = Config::getTimer();
    if (!tableExists(aName)) {
        return StatusResult{unknownTable};
    }
    
    size_t theCount{0};
    
    output << theCount << " row in set ( " << theTimer.elapsed() << " sec)" << std::endl;
    return StatusResult{};
}


StatusResult Database::insertRow(const std::string& theTableName, const std::vector<std::string>& columns, RowCollection rows) {
    
    StatusResult theRes;
    Timer theTimer = Config::getTimer();
    bool theSuccessFlag = false;
    size_t theCount{0};
    size_t theRowNumber;
    
    
    if (!meta.entityList.count(theTableName)) {
        theRes = unknownTable;
    }
    if (!theRes) return theRes;
    
    // read entity
    Entity theEntity;
    theRes = readEntity(meta.entityList[theTableName], theEntity);
    if (!theRes) return theRes;
    
    // validate given rows
    theRes = theEntity.isValidInsertData(columns, rows);
    if (!theRes) return theRes;
    
    // write true datatype in each row
    theRes = theEntity.writeDatatype(rows);
    if (!theRes) return theRes;
    
    // store rows
    auto thePrimaryKey = theEntity.getPrimaryKey().value().getName();
    for (auto& row : rows) {
        // add primary key
        theEntity.addPrimaryKey(*row);
        row->setTableName(theTableName);
        theRowNumber = writeNewRow(*row);
        
        // index
        storeIndex(theTableName, thePrimaryKey, Helpers::valueToString(row->getValue(thePrimaryKey).value()), static_cast<int>(theRowNumber));
        
        theCount++;
    }
    updateEntity(theEntity, meta.entityList[theTableName]);
    theSuccessFlag = true;
    CommandView theView(output, Keywords::insert_kw, theSuccessFlag, theCount, theTimer.elapsed());
    theView.show();

    return theRes;
}

StatusResult Database::tableExists(const std::string aName) {
    for(auto it = meta.entityList.begin(); it != meta.entityList.end(); it++) {
        if ((it->first) == aName) {
            return StatusResult{};
        }
    }
    return StatusResult{unknownTable};
}

StatusResult Database::filterRows(const Query& aQuery, RowCollection& rows) {
    RowCollection newRows;
    for (auto& row : rows) {
        if (aQuery.filters.matches(row->getData())) {
            newRows.push_back(row);
        }
    }
    newRows.swap(rows);
    return StatusResult{};
}

StatusResult Database::orderRows(const Query& aQuery, RowCollection& rows) {
    StatusResult theRes;
    if (0 == aQuery.orders.size()) return theRes;
    std::sort(rows.begin(), rows.end(), [&](const std::shared_ptr<Row>& row1, const std::shared_ptr<Row>& row2) {
        bool isSmaller = true;
        // iterate for all conditions
        for (auto& orderCondition : aQuery.orders) {
            auto& value1 = row1->getData()[orderCondition];
            auto& value2 = row2->getData()[orderCondition];
            if (value1.index() != value2.index()) {
                theRes = { Errors::invalidAttribute };
                break;
            }
            int theResult;
            
            std::visit([&](auto const& aLeft) {
                std::visit([&](auto const& aRight) {
                    theResult = CompareHelper::compareValue(aLeft, aRight);
                }, value2);
            }, value1);
            isSmaller = 0 > theResult ? true : false;
            // if it is equal, continue to next order condition; otherwise break and return
            if (0 != theResult) {
                break;
            }
            
        }
        return isSmaller;
    });
    return theRes;
}

StatusResult Database::limitRows(const Query& aQuery, RowCollection& rows) {
    StatusResult theRes{};
    // no limit given
    if (0 == aQuery.limits.size()) {
        return theRes;
    }
    size_t end, offset;

    switch (aQuery.limits.size())
    {
    case 1: // "limit 3" means length is 3
        offset = 0;
        end = aQuery.limits[0];
        break;
    case 2: // "limit 3, 5" means offset is 3 and length is 5
        end = aQuery.limits[0] + aQuery.limits[1];
        offset = aQuery.limits[0];
        break;
    default:
        return theRes = StatusResult{ invalidCommand };
        break;
    }
    // keep offset and end in valid range
    offset = std::min(offset, rows.size());
    end = std::min(end, rows.size());
    RowCollection newRows(rows.begin() + offset, rows.begin() + end);
    newRows.swap(rows);
    return theRes;
}

}
