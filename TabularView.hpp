//
//  TabularView.hpp
//  Developed by Jian Qin & Xiaoqiang Qi
//  Mentored by Rick Gessner
//

#ifndef TabularView_h
#define TabularView_h

#include <iostream>
#include <sstream>
#include <iomanip>
#include <map>
#include <vector>
#include <string>
#include "View.hpp"
#include "Row.hpp"
#include "Database.hpp"
#include "Helpers.hpp"

namespace ECE141 {

/**
 The class is a general view for any output in the command line
 */
using TabularViewRow = std::vector<std::string>;
class TabularView : public View {
public:
    
    TabularView(std::ostream& anOutput) : View(anOutput) {}

    virtual ~TabularView() {}
    
    // USE: create header for tablular view...
    StatusResult showHeader() {
        for (size_t i = 0; i < headers.size(); i++) {
            std::cout << columnSeparater << std::setfill(' ') << std::left << std::setw(static_cast<int>(widths[i])) << headers[i];
        }
        std::cout << columnSeparater << "\n";
        showSeparateLine();
        return StatusResult{};
    }
    
    // USE: this function shows all the fields in a row...
    StatusResult showRow(TabularViewRow &aRow) {
        for (size_t i = 0; i < aRow.size(); i++) {
            std::cout << columnSeparater << std::setfill(' ') << std::left << std::setw(static_cast<int>(widths[i])) << aRow[i];
        }
        std::cout << columnSeparater << "\n";
        showSeparateLine();
        return StatusResult{};
    }

    StatusResult showSeparateLine() {
        for (auto& width : widths) {
            std::cout << columnSeparater << Helpers::dashGenerator(width);
        }
        std::cout << columnSeparater << "\n";
        return StatusResult{};
    }

    // find maximum length in each column
    StatusResult countWidth() {
        if (0 == headers.size()) return StatusResult{ invalidArguments };
        for (auto& header : headers) {
            widths.push_back(header.size());
        }
        for (auto& dataRow : dataRows) {
            for (size_t i = 0; i < dataRow.size(); i++) {
                widths[i] = std::max(widths[i], static_cast<size_t>(dataRow[i].size()));
            }
        }
        return StatusResult{};
    }
    
    // USE: this is the main show() for the view, where it presents all the rows...
    StatusResult show() override {
        StatusResult theRes;
        theRes = countWidth();
        if (!theRes) return theRes;

        theRes = showHeader();
        if (!theRes) return theRes;

        for(auto &theRow : dataRows) {
            theRes = showRow(theRow);
            if (!theRes) return theRes;
        }
        std::cout << "table size : " << dataRows.size() << "\n";
        return StatusResult{};
    }
    
    StatusResult setHeaders(TabularViewRow aRow) {
        headers = aRow;
        return StatusResult{};
    }
    
    StatusResult addData(std::vector<TabularViewRow> aRows) {
        dataRows = aRows;
        return StatusResult{};
    }


    
protected:
    std::vector<size_t>         widths;   //  write widths when adding value
    std::vector<std::string>    headers;
    std::vector<TabularViewRow> dataRows;
    std::string columnSeparater = "|";

};

}

#endif /* TabularView_h */
