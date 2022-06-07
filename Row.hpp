//
//  Row.hpp
//  Developed by Jian Qin & Xiaoqiang Qi
//  Mentored by Rick Gessner
//

#ifndef Row_hpp
#define Row_hpp

#include <stdio.h>
#include <string>
#include <utility>
#include <variant>
#include <vector>
#include <memory>
#include <map>

#include "Attribute.hpp"
#include "Storable.hpp"
#include "TypedStream.hpp"

namespace SQLightning {

/**
 The Row class contains information about a single row stored in the database
 */
class Row : public Storable {
public:
    Row(size_t aRowNum=0);
    Row(const Row &aRow);
    virtual             ~Row();
    Row&                operator=(const Row &aRow);
    bool                operator==(Row &aCopy) const;
    StatusResult        set(const std::string &aKey, const Value &aValue);
    StatusResult        setTableName(const std::string& aName);
    StatusResult        setRowNumber(size_t aNum);
    StatusResult        removeKey(const std::string& aKey);
    
    size_t              getNum() const;
    KeyValues&          getData() {return data;}
    const KeyValues&    getConstData() const { return data; }
    ValueOpt            getValue(std::string& aKey) const;
    std::string         getTableName() const { return tableName; }
    StatusResult        encode(std::ostream& anOutput) const override;
    StatusResult        decode(std::istream& anInput) override;
    
protected:
    size_t              rowNum;
    KeyValues           data;
    std::string         tableName;
};

//-------------------------------------------

using RowCollection = std::vector<std::shared_ptr<Row>>;


}
#endif /* Row_hpp */
