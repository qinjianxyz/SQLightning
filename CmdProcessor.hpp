//
//  CmdProcessor.hpp
//  Developed by Jian Qin & Xiaoqiang Qi
//  Mentored by Rick Gessner
//

#ifndef CmdProcessor_hpp
#define CmdProcessor_hpp

#include <stdio.h>
#include <string>
#include <iostream>
#include <memory>

#include "Statement.hpp"
#include "Tokenizer.hpp"

namespace ECE141 {

/**
 This class is the interface of all command processors
 */
class CmdProcessor {
public:
    CmdProcessor(std::ostream &anOutput);
    virtual ~CmdProcessor();
    
    virtual CmdProcessor* recognizes(Tokenizer &aTokenizer)=0;
    virtual Statement*    makeStatement(Tokenizer &aTokenizer,
                                        StatusResult &aResult)=0;
    virtual StatusResult  run(Statement *aStmt)=0;
    
    
    std::ostream &output;
};

}

#endif /* CmdProcessor */
