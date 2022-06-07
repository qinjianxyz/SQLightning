//
//  ParseHelper.hpp
//  Developed by Jian Qin & Xiaoqiang Qi
//  Mentored by Rick Gessner
//

#ifndef ParseHelper_hpp
#define ParseHelper_hpp

#include <stdio.h>
#include "keywords.hpp"
#include "BasicTypes.hpp"
#include "Tokenizer.hpp"
#include "Attribute.hpp"
#include "Helpers.hpp"
#include "Entity.hpp"

namespace SQLightning {

//-------------------------------------------------

class Entity;

/**
 The ParseHelper class helps the tokenizer to parse raw input into database understandable objects
 */
struct ParseHelper {
    
    ParseHelper(Tokenizer &aTokenizer) : tokenizer(aTokenizer) {}
    
    StatusResult parseAttributeOptions(Attribute &anAttribute);
    
    StatusResult parseAttribute(Attribute &anAttribute);
    
    StatusResult parseIdentifierList(StringList &aList);
    
    StatusResult parseKeyValues(KeyValues &aList,Entity &anEnity);
    
    StatusResult parseValueList(StringList &aList);
    
protected:
    Tokenizer &tokenizer;
};

}

#endif /* ParseHelper_hpp */

