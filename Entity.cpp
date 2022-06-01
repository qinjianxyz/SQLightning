//
//  Entity.cpp
//  Developed by Jian Qin & Xiaoqiang Qi
//  Mentored by Rick Gessner
//

#include "Entity.hpp"

namespace ECE141 {

Entity::Entity(std::string aName) : name(aName), autoincr(0) {}

Entity::Entity(const Entity &aCopy) {
    name = aCopy.name;
    autoincr = aCopy.autoincr;
    attributes = aCopy.attributes;
}

Entity::~Entity() {}

StatusResult Entity::encode(std::ostream& anOutput) const {
    StreamEncoder anEncoder(&anOutput);
    anEncoder.encode(name).encode(static_cast<int>(autoincr)).encode(static_cast<int>(attributes.size()));
    for (auto& attribute : attributes) {
        attribute.encode(anOutput);
    }
    return StatusResult{};
}

StatusResult Entity::decode(std::istream& anInput) {
    size_t attrIndex;
    StreamDecoder aDecoder(&anInput);
    aDecoder.decode(name).decode(autoincr).decode(attrIndex);
    for (size_t index = 0; index < attrIndex; index++) {
        Attribute theAttribute;
        theAttribute.decode(aDecoder);
        attributes.push_back(theAttribute);
    }
    return StatusResult{};
}


void Entity::setName(std::string aName) {
    name = aName;
}

Entity& Entity::addAttribute(const Attribute &anAttribute) {
    if(!getAttribute(anAttribute.getName())) {
        attributes.push_back(anAttribute);
    }
    return *this;
}

AttributeOpt Entity::getAttribute(const std::string &aName) const {
    for (auto theAttribute : attributes) {
        if (theAttribute.getName() == aName) {
            return theAttribute;
        }
    }
    return std::nullopt;
}

AttributeOpt Entity::getPrimaryKey() const {
    for (auto theAttribute : attributes) {
        if (theAttribute.isPrimaryKey()) {
            return theAttribute;
        }
    }
    return std::nullopt;
}

StatusResult Entity::removeAttribute(const std::string& aName) {
    StatusResult theRes;
    for (auto it = attributes.begin(); it != attributes.end(); it++) {
        if (aName == it->getName()) {
            attributes.erase(it);
            return theRes;
        }
    }
    theRes = StatusResult{ invalidAttribute };
    return theRes;
}

size_t Entity::getNextPrimaryKey() {
    autoincr++;
    return autoincr;
}

StatusResult Entity::addPrimaryKey(Row& aRow) {
    auto thePrimaryKey = getPrimaryKey();
    if (!thePrimaryKey) {
        return StatusResult{ primaryKeyRequired };
    }
    aRow.getData()[thePrimaryKey->getName()] = static_cast<int>(getNextPrimaryKey());
    return StatusResult{};
}

StatusResult Entity::isValidInsertData(const std::vector<std::string>& columns, const RowCollection& rows) const {
    StatusResult theRes;
    
    // 1.check columns number == data num in each row
    for (auto& theRow : rows) {
        if (columns.size() != theRow->getData().size()) {
            theRes = StatusResult{ invalidAttribute };
        }
        for (auto& col : columns) {
            if (!theRow->getData().count(col)) {
                return StatusResult{ invalidAttribute };
            }
        }
    }
    // 2.check valid and enough columns are given
    theRes = isValidColumn(columns);
    if (!theRes) {
        return theRes;
    }
    
    // 3.check data types are consistent with entity
    theRes = isValidDatatype(rows);
    return theRes;
}

// check valid and enough columns are given
StatusResult Entity::isValidColumn(const std::vector<std::string>& columns) const {
    StatusResult theRes;
    int matchSize = 0;
    bool matchFlag;
    for (auto& attribute : attributes) {
        if (attribute.isPrimaryKey()) continue;
        // try to find a match in given column
        matchFlag = false;
        for (auto& col : columns) {
            if (col == attribute.getName()) {
                matchSize++;
                matchFlag = true;
                break;
            }
        }
        if (matchFlag) continue;
        if (!attribute.isNullable()) {
            return StatusResult{ invalidAttribute };
        }
    }
    
    // redundant columns are given
    if (columns.size() > matchSize) {
        return StatusResult{ unknownAttribute };
    }
    
    return theRes;
}

StatusResult Entity::isValidDatatype(const RowCollection& rows) const {
    for (auto& row : rows) {
        auto& theMap = row->getConstData();
        for (auto it = theMap.begin(); it != theMap.end(); it++) {
            std::string theString = std::get<std::string>(it->second);
            AttributeOpt theAttribute = getAttribute(it->first);
            DataTypes theType = theAttribute.value().getType();
            switch (theType) {
            case DataTypes::int_type:
                if (!Helpers::findPointinNum(theString, 0)) {
                    return StatusResult{ keyValueMismatch };
                }
                break;
            case DataTypes::varchar_type:
                break;
            case DataTypes::float_type:
                if (!Helpers::findPointinNum(theString, 1)) {
                    return StatusResult{ keyValueMismatch };
                }
                break;
            case DataTypes::bool_type:
                if (1 != theString.size() || ('1' != theString[0] && '0' != theString[0])) {
                    return StatusResult{ keyValueMismatch };
                }
                break;
            default:
                break;
            }
        }
    }
    return StatusResult{};
}

StatusResult Entity::writeDatatype(RowCollection& rows) const {
    std::set<std::string> theCols;
    return writeDatatype(rows, theCols);
}

StatusResult Entity::writeDatatype(RowCollection& rows, std::set<std::string> theCols) const {
    bool isAllSelected = 0 == theCols.size();
    for (auto& row : rows) {
        auto& theMap = row->getData();
        for (auto it = theMap.begin(); it != theMap.end(); it++) {
            // onlt rewrite selected columns
            if (!isAllSelected && !theCols.count(it->first)) continue;
            std::string theString = std::get<std::string>(it->second);
            AttributeOpt theAttribute = getAttribute(it->first);
            DataTypes theType = theAttribute.value().getType();
            switch (theType) {
                case DataTypes::int_type:
                    it->second = std::stoi(theString);
                    break;
                case DataTypes::varchar_type:
                    if (theString.size() > theAttribute->getSize()) {
                        it->second = theString.substr(0, theAttribute->getSize());
                    }
                    break;
                case DataTypes::float_type:
                    it->second = std::stod(theString);
                    break;
                case DataTypes::bool_type :
                    it->second = static_cast<bool>(std::stoi(theString));
                    break;
                default:
                    break;
            }
        }
    }
    return StatusResult{};
}
}

