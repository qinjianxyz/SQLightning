//
//  Attribute.cpp
//  Developed by Jian Qin & Xiaoqiang Qi
//  Mentored by Rick Gessner
//

#include "Attribute.hpp"

namespace SQLightning {

Attribute::Attribute(DataTypes aType)
: type(aType),size(0),autoIncrement(0),primary(0),nullable(1) {}

Attribute::Attribute(std::string aName, DataTypes aType, uint32_t aSize)  {
    name = aName;
    type = aType;
    size = aSize;
    autoIncrement = 0;
    primary = 0;
    nullable = 1;
}

Attribute::Attribute(const Attribute &aCopy)  {
    name = aCopy.name;
    type = aCopy.type;
    size = aCopy.size;
    autoIncrement = aCopy.autoIncrement;
    primary = aCopy.primary;
    nullable = aCopy.nullable;
}

Attribute::~Attribute()  {}

StatusResult Attribute::encode(std::ostream& anOutput) const {
    StreamEncoder anEncoder(&anOutput);
    anEncoder.encode(name)
        .encode(std::string(1, static_cast<char>(type)))
        .encode(size)
        .encode(autoIncrement)
        .encode(primary)
        .encode(nullable);
    return StatusResult{};
}

StatusResult Attribute::decode(std::istream& anInput) {
    StatusResult theRes;
    StreamDecoder aDecoder(&anInput);
    theRes = decode(aDecoder);
    if (!theRes) return theRes;
    return StatusResult{};
}

StatusResult Attribute::decode(StreamDecoder& aDecoder) {
    std::string nameStr;
    std::string typeStr;
    size_t sizeNum;
    size_t autoIncrementNum;
    size_t primaryNum;
    size_t nullableNum;
    
    aDecoder.decode(nameStr)
        .decode(typeStr)
        .decode(sizeNum)
        .decode(autoIncrementNum)
        .decode(primaryNum)
        .decode(nullableNum);
    setName(nameStr);
    setDataType(Helpers::keywordToDatatype(Helpers::charToKeyword(typeStr[0])));
    setSize(sizeNum);
    setAutoIncrement(autoIncrementNum);
    setPrimaryKey(primaryNum);
    setNullable(nullableNum);
    return StatusResult{};
}

Attribute& Attribute::setName(const std::string &aName)  {
    name = aName;
    return *this;
}

Attribute& Attribute::setDataType(DataTypes aType) {
    type=aType;
    return *this;
}

Attribute& Attribute::setSize(size_t aSize) {
    size=aSize;
    return *this;
}

Attribute& Attribute::setAutoIncrement(bool anAuto) {
    autoIncrement = anAuto;
    return *this;
}

Attribute& Attribute::setPrimaryKey(bool aPrimary) {
    primary=aPrimary;
    return *this;
}

Attribute& Attribute::setNullable(bool aNullable) {
    nullable = aNullable;
    return *this;
}

bool Attribute::isValid() {
    return true;
}

Value Attribute::toValue(const std::string &aValue) const {
    return Value{aValue};
}

}
