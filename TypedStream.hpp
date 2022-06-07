//
//  TypedStream.hpp
//  Developed by Jian Qin & Xiaoqiang Qi
//  Mentored by Rick Gessner
//

#ifndef TypedStream_hpp
#define TypedStream_hpp

#include <sstream>
#include <iostream>
#include "Errors.hpp"
#include "BasicTypes.hpp"
#include "Tokenizer.hpp"

namespace SQLightning {

class StreamEncoder {
public:
    StreamEncoder(std::ostream* aStream) : theStream(aStream) {}
    StatusResult setStream(std::ostream* aStream);
    
    
    StreamEncoder& encode(Value aValue);
protected:
    std::ostream* theStream;
};

class StreamDecoder {
public:
    StreamDecoder(std::istream* aStream) : theStream(aStream), theTokenizer(*aStream) {
        theTokenizer.tokenize();
    }
    StatusResult setStream(std::istream* aStream);
    
    // todo: can we use less function?
    StreamDecoder& decode(int& aNumber);
    StreamDecoder& decode(size_t& anUnsignedNumber);
    StreamDecoder& decode(bool& aBoolean);
    StreamDecoder& decode(double& aDouble);
    StreamDecoder& decode(std::string& aStr);
    StreamDecoder& decode(Value& aValue);
    bool more() { return theTokenizer.more(); }
    
protected:
    std::istream* theStream;
    Tokenizer theTokenizer;
};
}


#endif /* TypedStream_hpp */
