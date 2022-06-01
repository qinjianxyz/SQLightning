//
//  describeView.hpp
//  ece141b-pa8
//
//  Created by Jian Qin on 5/20/22.
//

#ifndef describeView_hpp
#define describeView_hpp

#include <iostream>
#include "TabularView.hpp"
#include "Entity.hpp"

namespace ECE141 {
class DescribeView : public TabularView {
public:
    DescribeView(std::ostream& anOutput) : TabularView(anOutput) {
        headers = std::vector<std::string> {"Field", "Type", "Null", "Key", "Default", "Extras"};
    }
    
    virtual ~DescribeView() {}

    StatusResult addData(const Entity& anEntity) {
        
        StatusResult theRes;
        if (0 == anEntity.getAttributes().size()) return theRes;

        // add rows
        for (auto& attr : anEntity.getAttributes()) {
            
            TabularViewRow aDataRow;
            theRes = addRow(attr, aDataRow);
            if (!theRes) return theRes;
            dataRows.push_back(aDataRow);
            
        }
        return theRes;
    }

    StatusResult addRow(const Attribute& anAttr, TabularViewRow& aDataRow) {
        StatusResult theRes;
        aDataRow.push_back(anAttr.getName());
        aDataRow.push_back(Helpers::keywordToString(Helpers::charToKeyword(static_cast<char>(anAttr.getType()))));
        aDataRow.push_back(anAttr.isNullable() ? "YES" : "NO");
        aDataRow.push_back(anAttr.isPrimaryKey() ? "YES" : "NO");
        aDataRow.push_back(anAttr.isNullable() ? "YES" : "NO");
        aDataRow.push_back(anAttr.isPrimaryKey() ? "auto_increment primary key" : "");
        return theRes;
    }

};
}

#endif /* describeView_hpp */
