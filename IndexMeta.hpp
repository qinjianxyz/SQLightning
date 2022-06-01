//
//  IndexMeta.hpp
//  Developed by Jian Qin & Xiaoqiang Qi
//  Mentored by Rick Gessner
//

#ifndef IndexMeta_hpp
#define IndexMeta_hpp

#include <iostream>
#include "Errors.hpp"
#include "Storable.hpp"
#include "TypedStream.hpp"

namespace ECE141 {
    using FieldIndexMap = std::map<std::string, int>;
    using TableIndexMap = std::map<std::string, FieldIndexMap>;

    class IndexMeta : public Storable  {
    public:
        IndexMeta() {}
        virtual ~IndexMeta() {}
        static IndexMeta createIndexMeta() { // should be used when initializing a database
            return IndexMeta{};
        }

        StatusResult encode(std::ostream& anOutput) const override;
        StatusResult decode(std::istream& anInput) override;
        StatusResult addNewFieldIndex(const std::string aTableName, const std::string aFieldName, const int aBlockNumber);
        StatusResult updateFieldIndex(const std::string aTableName, const std::string aFieldName, const int aBlockNumber);
        StatusResult deleteFieldIndex(const std::string aTableName, const std::string aFieldName);
        StatusResult deleteTableIndex(const std::string aTableName);
        StatusResult getFieldIndex(const std::string aTableName, const std::string aFieldName, int& aBlockNumber);

        StatusResult isContainTable(const std::string aTableName);
        StatusResult isContainField(const std::string aTableName, const std::string aFieldName);


    protected:
        StatusResult addNewTableIndex(const std::string aTableName);  // called only inside addNewFieldIndex()
        TableIndexMap theTableIndexMap;
    };
    

}

#endif /* IndexMeta_hpp */
