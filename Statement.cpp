//
//  Statement.cpp
//  Developed by Jian Qin & Xiaoqiang Qi
//  Mentored by Rick Gessner
//

#include "Statement.hpp"

namespace ECE141 {

Statement::Statement(Keywords aStatementType, std::string anExtra) : extras(anExtra), stmtType(aStatementType){}

Statement::Statement(const Statement &aCopy) : extras(aCopy.extras), stmtType(aCopy.stmtType) {}

Statement::~Statement() {}

StatusResult Statement::parse(Tokenizer &aTokenizer) {
    return StatusResult{};
}

StatusResult Statement::run(std::ostream &aStream) const {
    return StatusResult();
}

}
