//
//  IndexMeta.cpp
//  Developed by Jian Qin & Xiaoqiang Qi
//  Mentored by Rick Gessner
//

#include "IndexMeta.hpp"

namespace ECE141 {
StatusResult IndexMeta::encode(std::ostream& anOutput) const {
    StatusResult theRes;
    StreamEncoder anEncoder(&anOutput);
    for (auto& aTableIndex : theTableIndexMap) {  // iterate all the tables
        anEncoder.encode(aTableIndex.first); // encode table name
        anEncoder.encode(static_cast<int>(aTableIndex.second.size())); // encode table size
        for (auto& aFieldIndex : aTableIndex.second) {  // iterate over each field in a table
            anEncoder.encode(aFieldIndex.first); // encode field name
            anEncoder.encode(static_cast<int>(aFieldIndex.second.size())); // encode field size
            for (auto& index : aFieldIndex.second) {
                anEncoder.encode(index);
            }
        }
    }
    return theRes;
}

StatusResult IndexMeta::decode(std::istream& anInput) {
    StatusResult theRes;
    StreamDecoder aDecoder(&anInput);
    std::string aTableName;
    while (aDecoder.more()) {   // iterate all the tables
        aDecoder.decode(aTableName);
        theRes = addNewTableIndex(aTableName);
        if (!theRes) return theRes;
        size_t theTableSize = 0;
        aDecoder.decode(theTableSize);
        for (size_t index = 0; index < theTableSize; index++) {
            std::string aFieldName;
            int aFieldSize = 0;
            std::vector<int> aBlockVector;
            aDecoder.decode(aFieldName);
            aDecoder.decode(aFieldSize);
            for (size_t index2 = 0; index2 < aFieldSize; index2++) {
                int aBlockNumber;
                aDecoder.decode(aBlockNumber);
                aBlockVector.push_back(aBlockNumber);
            }
            theRes = addNewFieldIndexVector(aTableName, aFieldName, aBlockVector);
            if (!theRes) return theRes;
        }
    }
    return theRes;
    
}

StatusResult IndexMeta::addNewTableIndex(const std::string aTableName) {
    StatusResult theRes;
    if (isContainTable(aTableName)) {
        theRes = StatusResult{ tableExists };
        return theRes;
    }
    theTableIndexMap[aTableName] = {};
    return theRes;
}

StatusResult IndexMeta::addNewFieldIndex(const std::string aTableName, const std::string aFieldName, const int aBlockNumber) {
    StatusResult theRes;
    if (!isContainTable(aTableName)) {
        theRes = addNewTableIndex(aTableName);  // if not exist, create a new table index
        if (!theRes) return theRes;
    }
    if (!theTableIndexMap[aTableName].count(aFieldName)) { // does not exist
        theTableIndexMap[aTableName][aFieldName] = std::vector<int>();
    }
    theTableIndexMap[aTableName][aFieldName].push_back(aBlockNumber);
    return theRes;
}

StatusResult IndexMeta::addNewFieldIndexVector(const std::string aTableName, const std::string aFieldName, const std::vector<int> aBlockVector) {
    StatusResult theRes;
    for (auto& index : aBlockVector) {
        theRes = addNewFieldIndex(aTableName, aFieldName, index);
    }
    return theRes;
}

StatusResult IndexMeta::isContainTable(const std::string aTableName) {
    StatusResult theRes;
    if (!theTableIndexMap.count(aTableName)) {
        theRes = StatusResult{ unknownTable };
    }
    return theRes;
}

StatusResult IndexMeta::isContainField(const std::string aTableName, const std::string aFieldName) {
    StatusResult theRes;
    theRes = isContainTable(aTableName);
    if (!theRes) return theRes;
    if (!theTableIndexMap[aTableName].count(aFieldName)) {
        theRes = StatusResult{ unknownIndex };
        if (!theRes) return theRes;
    }
    return theRes;
}

/**
 Implementation is unclear after converting index to a vector
 
StatusResult IndexMeta::updateFieldIndex(const std::string aTableName, const std::string aFieldName, const int aBlockNumber) {
    StatusResult theRes;
    theRes = isContainField(aTableName, aFieldName);
    if (!theRes) return theRes;
    theTableIndexMap[aTableName][aFieldName] = aBlockNumber;
    return theRes;
}
 */

StatusResult IndexMeta::deleteFieldIndex(const std::string aTableName, const std::string aFieldName) {
    StatusResult theRes;
    theRes = isContainField(aTableName, aFieldName);
    if (!theRes) return theRes;
    theTableIndexMap[aTableName].erase(aFieldName);
    return theRes;
}

StatusResult IndexMeta::deleteTableIndex(const std::string aTableName) {
    StatusResult theRes;
    theRes = isContainTable(aTableName);
    if (!theRes) return theRes;
    theTableIndexMap.erase(aTableName);
    return theRes;
}

StatusResult IndexMeta::getFieldIndex(const std::string aTableName, const std::string aFieldName, std::vector<int>& aBlockVector) {
    StatusResult theRes;
    theRes = isContainField(aTableName, aFieldName);
    if (!theRes) return theRes;
    aBlockVector = theTableIndexMap[aTableName][aFieldName];
    return theRes;
}

}
