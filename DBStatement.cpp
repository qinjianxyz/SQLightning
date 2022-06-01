//
//  DBStatement.cpp
//  Developed by Jian Qin & Xiaoqiang Qi
//  Mentored by Rick Gessner
//

#include "DBStatement.hpp"

namespace ECE141
{
	DBStatement::DBStatement(Keywords aStatementType, std::string aStr) : Statement(aStatementType) {
		databaseObj = aStr;
	}
	DBStatement::~DBStatement() {

	}
}
