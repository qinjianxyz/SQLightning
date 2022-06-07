//
//  Filters.hpp
//  Developed by Jian Qin & Xiaoqiang Qi
//  Mentored by Rick Gessner
//

#ifndef Filters_h
#define Filters_h

#include <stdio.h>
#include <vector>
#include <memory>
#include <string>
#include <limits>

#include "keywords.hpp"
#include "Helpers.hpp"
#include "Entity.hpp"
#include "Attribute.hpp"
#include "Compare.hpp"
#include "Errors.hpp"
#include "Tokenizer.hpp"
#include "BasicTypes.hpp"

namespace SQLightning {

class Row;
class Entity;

struct Operand {
    Operand() {}
    Operand(std::string &aName, TokenType aType, Value &aValue, size_t anId=0)
    : ttype(aType), dtype(DataTypes::varchar_type), name(aName),
    value(aValue), entityId(anId) {}
    
    TokenType   ttype; // is it a field, or const (#, string)...
    DataTypes   dtype;
    std::string name;  // attr name
    Value       value;
    size_t      entityId;
};

//---------------------------------------------------

struct Expression {
    Operand     lhs;   // id
    Operand     rhs;   // usually a constant; maybe a field
    Operators   op;    // =
    Logical     logic; // and, or , not
    
    Expression(Operand &aLHSOperand, Operators anOp, Operand &aRHSOperand)
    : lhs(aLHSOperand), rhs(aRHSOperand),
    op(anOp), logic(Logical::no_op) {}
    
    bool operator()(KeyValues &aList);
};

using Expressions = std::vector<std::shared_ptr<Expression>>;

//---------------------------------------------------

class Filters {
public:
    
    Filters();
    Filters(const Filters &aFilters);
    ~Filters();
    size_t        getCount() const {return expressions.size();}
    bool          matches(KeyValues &aList) const;
    Filters&      add(Expression *anExpression);
    StatusResult  parse(Tokenizer &aTokenizer, Entity &anEntity);
    
protected:
    Expressions   expressions;
};

}

#endif /* Filters_h */
