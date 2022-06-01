//
//  Filters.cpp
//  Datatabase5
//
//  Created by rick gessner on 3/5/21.
//  Copyright © 2021 rick gessner. All rights reserved.
//

#include "Filters.hpp"

namespace ECE141 {

using Comparator = bool (*)(const Value &aLHS, const Value &aRHS);

static std::map<Operators, Comparator> comparators {
    {Operators::equal_op, CompareHelper::isEqual<Value, Value>},
    {Operators::lt_op, CompareHelper::isLessThan<Value, Value>},
    {Operators::lte_op, CompareHelper::isLessThanOrEqualTo<Value, Value>},
    {Operators::gt_op, CompareHelper::isGreaterThan<Value, Value>},
    {Operators::gte_op, CompareHelper::isGreaterThanOrEqualTo<Value, Value>}
};

bool Expression::operator()(KeyValues &aList) {
    Value theLHS{lhs.value};
    Value theRHS{rhs.value};
    
    if(TokenType::identifier==lhs.ttype) {
        theLHS = aList[lhs.name]; //get row value
    }
    
    if(TokenType::identifier==rhs.ttype) {
        theRHS=aList[rhs.name]; //get row value
    }
    
    return comparators.count(op)
    ? comparators[op](theLHS, theRHS) : false;
}

//--------------------------------------------------------------

Filters::Filters()  {}

Filters::Filters(const Filters &aCopy)  {}

Filters::~Filters() {}

Filters& Filters::add(Expression *anExpression) {
    expressions.push_back(std::shared_ptr<Expression>(anExpression));
    return *this;
}

// compare expressions to row; return true if matches
bool Filters::matches(KeyValues &aList) const {
    
    bool theCombinationLogic = true;
    
    for(auto &theExpr : expressions) {
        bool theComparison = (*theExpr)(aList);
        if (theExpr->logic == Logical::not_op) {
            theCombinationLogic = !theComparison;
        } else if (theExpr->logic == Logical::and_op) {
            theCombinationLogic = theComparison && theCombinationLogic;
        } else if (theExpr->logic == Logical::or_op) {
            theCombinationLogic = theComparison || theCombinationLogic;
        } else if (theExpr->logic == Logical::no_op) {
            theCombinationLogic = theComparison;
        }
    }
    return theCombinationLogic;
}


// where operand is field, number, string...
StatusResult parseOperand(Tokenizer &aTokenizer, Entity &anEntity, Operand &anOperand) {
    StatusResult theResult{noError};
    Token &theToken = aTokenizer.current();
    if(TokenType::identifier==theToken.type) {
        if(auto theAttr = anEntity.getAttribute(theToken.data)) {
            anOperand.ttype=theToken.type;
            anOperand.name=theToken.data;
            anOperand.dtype=theAttr->getType();
        }
        else {
            anOperand.ttype=TokenType::string;
            anOperand.dtype=DataTypes::varchar_type;
            anOperand.value=theToken.data;
        }
    }
    else if(TokenType::number==theToken.type) {
        anOperand.ttype=TokenType::number;
        anOperand.dtype=DataTypes::int_type;
        if (theToken.data.find('.')!=std::string::npos) {
            anOperand.dtype=DataTypes::float_type;
            anOperand.value=std::stof(theToken.data);
        }
        else anOperand.value=std::stoi(theToken.data);
    } else {
        theResult.error=syntaxError;
    }
    if(theResult) aTokenizer.next();
    return theResult;
}


bool validateOperands(Operand &aLHS, Operand &aRHS, Entity &anEntity) {
    if(TokenType::identifier==aLHS.ttype) { //most common case...
        return true;
    }
    else if(TokenType::identifier==aRHS.ttype) {
        return true;
    }
    return false;
}

// parse the where clause
StatusResult Filters::parse(Tokenizer &aTokenizer,Entity &anEntity) {
    StatusResult  theResult{noError};
    while(theResult && (2 < aTokenizer.remaining())) {
        Operand theLHS, theRHS;
        Logical theLogic = Logical::no_op;
        
        Token &theToken = aTokenizer.current();
        if (theToken.type == TokenType::keyword) {
            if (!gExpressionLogs.count(theToken.data)) {
                break;
            }
            theLogic = gExpressionLogs[theToken.data];
            aTokenizer.next();
        }
        
        if(aTokenizer.current().type!=TokenType::identifier) return theResult;
        if((theResult=parseOperand(aTokenizer,anEntity,theLHS))) {
            Token &theToken=aTokenizer.current();
            if(theToken.type==TokenType::operators) {
                Operators theOp=Helpers::toOperator(theToken.data);
                aTokenizer.next();
                if((theResult=parseOperand(aTokenizer,anEntity,theRHS))) {
                    if(validateOperands(theLHS, theRHS, anEntity)) {
                        Expression* theExp = new Expression(theLHS, theOp, theRHS);
                        theExp->logic = theLogic;
                        add(theExp);
                        if(aTokenizer.skipIf(semicolon)) {
                            break;
                        }
                    } else {
                        theResult.error=syntaxError;
                    }
                }
            }
        }
        else theResult.error=syntaxError;
    }
    return theResult;
}

}

