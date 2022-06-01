//
//  BlockIO.hpp
//  Developed by Jian Qin & Xiaoqiang Qi
//  Mentored by Rick Gessner
//

#ifndef BlockIO_hpp
#define BlockIO_hpp

#include <stdio.h>
#include <iostream>
#include <functional>
#include <optional>
#include <fstream>
#include <cstring>

#include "Errors.hpp"
#include "Entity.hpp"

namespace ECE141 {

/**
 The class contains customed defined block types
 */
enum class BlockType {
    data_block    =  'D',
    free_block    =  'F',
    meta_block    =  'M',
    entity_block  =  'E',
    index_block   =  'I',
    unknown_block =  'U',
};


/**
 The structure contains meta info about a block
 */
struct BlockHeader {
    
    BlockHeader(BlockType aType=BlockType::data_block)
    : type(static_cast<char>(aType)) {}
    
    BlockHeader(const BlockHeader &aCopy) {
        *this = aCopy;
    }
    
    void empty() {
        type = static_cast<char>(BlockType::free_block);
    }
    
    BlockHeader& operator=(const BlockHeader &aCopy) {
        type = aCopy.type;
        return *this;
    }
    
    char      type;     // char version of block type
};


const size_t kBlockSize = 2048;
const size_t kPayloadSize = kBlockSize - sizeof(BlockHeader);


/**
 The class is a polymorphic basic unit of Storage
 */
class Block {
public:
    Block(BlockType aType = BlockType::data_block);
    
    Block(const Block &aCopy);
    
    Block& operator=(const Block &aCopy);
    
    BlockHeader     header;
    
    char            payload[kPayloadSize];
};

// Interface to Handle Blocks
class BlockIO {
public:
    
    BlockIO(std::string aPath, bool asNew);
    ~BlockIO();
    size_t                getBlockCount();
    StatusResult          readBlock (size_t aBlockNumber, Block &aBlock);
    StatusResult          writeBlock(size_t aBlockNumber, Block &aBlock);
protected:
    size_t                findNextFreeBlock();
    std::unique_ptr<std::fstream>       stream;
};

}


#endif /* BlockIO_hpp */
