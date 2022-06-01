//
//  Entity.hpp
//  Developed by Jian Qin & Xiaoqiang Qi
//  Mentored by Rick Gessner
//

#ifndef Entity_hpp
#define Entity_hpp

#include <stdio.h>
#include <vector>
#include <optional>
#include <memory>
#include <string>
#include <set>
#include <memory>
#include <string>
#include <algorithm>
#include <iostream>
#include <sstream>

#include "Tokenizer.hpp"
#include "Helpers.hpp"
#include "Errors.hpp"
#include "BasicTypes.hpp"
#include "Attribute.hpp"
#include "Row.hpp"
#include "TypedStream.hpp"


namespace ECE141 {

using AttributeOpt = std::optional<Attribute>;
using AttributeList = std::vector<Attribute>;

/**
 The entity class defines the schema of a table.
 */
class Entity : public Storable {
public:
    Entity(std::string aName = "");
    Entity(const Entity &aCopy);
    ~Entity();
    
    void                                setName(const std::string);
    std::string                         getName() const {return name;}
    std::vector<Attribute>              getAttributes() const {return attributes;}
    Entity&                             addAttribute(const Attribute &anAttribute);
    AttributeOpt                        getAttribute(const std::string &aName) const;
    AttributeOpt                        getPrimaryKey() const;
    size_t                              getNextPrimaryKey();
    StatusResult                        addPrimaryKey(Row& aRow);
    StatusResult                        removeAttribute(const std::string& aName);
    
    StatusResult                        encode(std::ostream& anOutput) const override;
    StatusResult                        decode(std::istream& anInput) override;
    StatusResult                        isValidInsertData(const std::vector<std::string>& columns, const RowCollection& rows) const;
    StatusResult                        isValidColumn(const std::vector<std::string>& columns) const;
    StatusResult                        isValidDatatype(const RowCollection& rows) const;
    StatusResult                        writeDatatype(RowCollection& rows) const;
    StatusResult                        writeDatatype(RowCollection& rows, std::set<std::string> theCols) const;
protected:
    
    std::string                         name;
    size_t                              autoincr;  //auto_increment (primary key)
    std::vector<Attribute>              attributes;
    
};

}
#endif /* Entity_hpp */
