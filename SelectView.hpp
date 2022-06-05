//
//  SelectView.hpp
//  Developed by Jian Qin & Xiaoqiang Qi
//  Mentored by Rick Gessner
//

#ifndef SelectView_h
#define SelectView_h

#include <iostream>
#include "TabularView.hpp"

namespace ECE141
{

    class SelectView : public TabularView {
        
    public:
        SelectView(std::ostream& anOutput) : TabularView(anOutput) {}
        
        virtual ~SelectView() {}

        StatusResult addData(const Query& aQuery, const RowCollection& rows, const Entity& anEntity) {
            StatusResult theRes;
            if (0 == rows.size()) return theRes;

            // add headers
            theRes = addHeaders(aQuery, rows, anEntity);
            if (!theRes) return theRes;

            // add rows
            for (auto& row : rows) {
                TabularViewRow aDataRow;
                theRes = addRow(row, aDataRow);
                if (!theRes) return theRes;
                dataRows.push_back(aDataRow);
            }
            return theRes;
        }

        StatusResult addHeaders(const Query& aQuery, const RowCollection& rows, const Entity& anEntity) {
            StatusResult theRes;
            // if select all
            if (aQuery.columns[0] == "*") {
                theRes = addAllHeaders(anEntity);
                if (!theRes) return theRes;
            }
            else {
                theRes = addSelectedHeaders(aQuery);
                if (!theRes) return theRes;
            }
            return theRes;
        }

        StatusResult addAllHeaders(const Entity& anEntity) {
            StatusResult theRes;
            for (auto& keyValue : anEntity.getAttributes()) {
                headers.push_back(keyValue.getName());
            }
            return theRes;
        }

        StatusResult addSelectedHeaders(const Query& aQuery) {
            StatusResult theRes;
            for (auto& key : aQuery.columns) {
                headers.push_back(key);
            }
            return theRes;
        }

        StatusResult addRow(const std::shared_ptr<Row> aRow, TabularViewRow& aDataRow) {
            StatusResult theRes;
            for (int i = 0; i < headers.size(); i++) {
                std::string aValue = aRow->getData().count(headers[i]) ? Helpers::valueToString(aRow->getData()[headers[i]]) : "null";
                aDataRow.push_back(aValue);
            }
            return theRes;
        }

    };

}

#endif /* SelectView_h */
