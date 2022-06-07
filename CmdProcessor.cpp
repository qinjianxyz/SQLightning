//
//  CmdProcessor.cpp
//  Developed by Jian Qin & Xiaoqiang Qi
//  Mentored by Rick Gessner
//

#include "CmdProcessor.hpp"

namespace SQLightning {

CmdProcessor::CmdProcessor(std::ostream &anOutput)
: output(anOutput){
}

CmdProcessor::~CmdProcessor() {}

CmdProcessor* CmdProcessor::recognizes(Tokenizer &aTokenizer) {
    return nullptr;
}

}
