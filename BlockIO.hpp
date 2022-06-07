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
#include "Config.hpp"
#include "LRUCache.hpp"

namespace SQLightning {

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

/**
 Each index meta can handle approximately 5000 blocks.
 Ex. if you want to insert 10 million blocks, set meta size to 2000.
 */
const size_t metaSize = 50;
const size_t kBlockSize = 1024;
const size_t kPayloadSize = kBlockSize - sizeof(BlockHeader);
const size_t kIndexMetaBlockSize = kBlockSize * (metaSize - 1);
const size_t kIndexMetaPayloadSize = kIndexMetaBlockSize - sizeof(BlockHeader);

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

/**
 special block for IndexMeta
 */
class IndexMetaBlock {
public:
    IndexMetaBlock(BlockType aType = BlockType::meta_block);
    
    IndexMetaBlock(const Block &aCopy);
    
    IndexMetaBlock& operator=(const Block &aCopy);
    
    BlockHeader     header;
    
    char            payload[kIndexMetaPayloadSize];
};

// Interface to Handle Blocks
class BlockIO {
public:
    
    BlockIO(std::string aPath, bool asNew);
    ~BlockIO();
    size_t                getBlockCount();
    StatusResult          clearBlock(const size_t aBlockNum);
    StatusResult          readBlock (size_t aBlockNumber, Block &aBlock);
    StatusResult          writeBlock(size_t aBlockNumber, Block &aBlock);
    StatusResult          readIndexMetaBlock (IndexMetaBlock &aBlock);
    StatusResult          writeIndexMetaBlock(IndexMetaBlock &aBlock);
protected:
    LRUCache<size_t, Block>         blockCache;
    size_t                          findNextFreeBlock();
    std::unique_ptr<std::fstream>   stream;
};

}


#endif /* BlockIO_hpp */
