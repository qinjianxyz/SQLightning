//
//  Row.cpp
//  Developed by Jian Qin & Xiaoqiang Qi
//  Mentored by Rick Gessner
//

#include "Row.hpp"

namespace ECE141 {

Row::Row(size_t aRowNum) : rowNum(aRowNum) {}

Row::Row(const Row &aRow) {
    *this=aRow;
}

StatusResult Row::setRowNumber(size_t aNum) {
    rowNum = aNum;
    return StatusResult{};
};

size_t Row::getNum() const {
    return rowNum;
}

Row& Row::operator=(const Row &aRow) {
    for (auto it = aRow.data.begin(); it != aRow.data.end(); it++) {
        set(it->first, it->second);
    }
    tableName = aRow.tableName;
    return *this;
}

Row::~Row() {}

bool Row::operator==(Row &aCopy) const {return false;}

StatusResult Row::set(const std::string &aKey, const Value &aValue) {
    data[aKey] = aValue;
    return StatusResult{};
}

StatusResult Row::setTableName(const std::string& aName) {
    tableName = aName;
    return StatusResult{};
}

StatusResult Row::removeKey(const std::string& aKey) {
    if (!data.count(aKey)) {
        return StatusResult{ unknownAttribute };
    }
    data.erase(aKey);
    return StatusResult{};
}

StatusResult Row::encode(std::ostream& anOutput) const {
    StreamEncoder theEncoder(&anOutput);
    theEncoder.encode(tableName);
    for (auto& item : data) {
        theEncoder.encode(item.first).encode(item.second);
    }
    return StatusResult{};
}

StatusResult Row::decode(std::istream& anInput) {
    StreamDecoder aDecoder(&anInput);
    aDecoder.decode(tableName);
    while (aDecoder.more()) {
        std::string name;
        Value aValue;
        aDecoder.decode(name).decode(aValue);
        set(name, aValue);
    }
    return StatusResult{};
}

ValueOpt Row::getValue(std::string& aKey) const {
    if (data.count(aKey)) {
        return data.at(aKey);
    }
    else {
        return std::nullopt;
    }
}


}


