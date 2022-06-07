//
//  Application.hpp
//  Developed by Jian Qin & Xiaoqiang Qi
//  Mentored by Rick Gessner
//

#ifndef Attribute_hpp
#define Attribute_hpp

#include <stdio.h>
#include <string>
#include <vector>
#include <optional>
#include <functional>
#include <iostream>
#include <sstream>

#include "keywords.hpp"
#include "BasicTypes.hpp"
#include "Storable.hpp"
#include "Tokenizer.hpp"
#include "Helpers.hpp"

/**
 The attribute class contains information about a field in the sql database
 */
namespace SQLightning {

class Attribute : public Storable {
protected:
    std::string   name;
    DataTypes     type;
    uint16_t      size : 10; //max=1000
    uint16_t      autoIncrement : 1;
    uint16_t      primary : 1;
    uint16_t      nullable: 1;
    
public:
    
    Attribute(DataTypes aType=DataTypes::no_type);
    Attribute(std::string aName, DataTypes aType, uint32_t aSize=0);
    Attribute(const Attribute &aCopy);
    virtual ~Attribute();
    
    Attribute&          setName(const std::string &aName);
    Attribute&          setDataType(DataTypes aType);
    Attribute&          setSize(size_t aSize);
    Attribute&          setAutoIncrement(bool anAuto);
    Attribute&          setPrimaryKey(bool anAuto);
    Attribute&          setNullable(bool aNullable);
    
    const std::string&  getName() const {return name;}
    DataTypes           getType() const {return type;}
    size_t              getSize() const {return size;}
    Value               toValue(const std::string &aValue) const;
    
    StatusResult        encode(std::ostream& anOutput) const override;
    StatusResult        decode(std::istream& anInput) override;
    StatusResult        decode(StreamDecoder& aDecoder);
    bool                isValid();
    bool                isPrimaryKey() const {return primary;}
    bool                isNullable() const {return nullable;}
    bool                isAutoIncrement() const {return autoIncrement;}
};

}


#endif /* Attribute_hpp */
