//
//  BlockIO.cpp
//  Developed by Jian Qin & Xiaoqiang Qi
//  Mentored by Rick Gessner
//

#include "BlockIO.hpp"

namespace SQLightning {

const auto openExisting = std::ios::in | std::ios::out | std::ios::binary;
const auto openNew = std::ios::in | std::ios::out | std::ios::binary | std::ios::trunc;

Block::Block(BlockType aType) {
    header.type = static_cast<char>(aType);
}

Block::Block(const Block &aCopy) {
    *this=aCopy;
}

Block& Block::operator=(const Block &aCopy) {
    std::memcpy(payload, aCopy.payload, kPayloadSize);
    header=aCopy.header;
    return *this;
}

IndexMetaBlock::IndexMetaBlock(BlockType aType) {
    header.type = static_cast<char>(aType);
}

IndexMetaBlock::IndexMetaBlock(const Block &aCopy) {
    *this=aCopy;
}

IndexMetaBlock& IndexMetaBlock::operator=(const Block &aCopy) {
    std::memcpy(payload, aCopy.payload, kIndexMetaPayloadSize);
    header=aCopy.header;
    return *this;
}

BlockIO::BlockIO(std::string aPath, bool asNew) {
    if (asNew) {
        stream = std::make_unique<std::fstream>(aPath, openNew);
    } else {
        stream = std::make_unique<std::fstream>(aPath, openExisting);
    }
}

BlockIO::~BlockIO() {}

StatusResult BlockIO::writeBlock(size_t aBlockNum, Block &aBlock) {
    if (Config::useCache(CacheType::block)) {
        blockCache.put(aBlockNum, aBlock);
    }
    stream->clear();
    stream->seekg(aBlockNum * kBlockSize);
    stream->write((char*)&aBlock, kBlockSize);
    stream->flush();
    stream->clear();
    return StatusResult{};
}

StatusResult BlockIO::readBlock(size_t aBlockNum, Block &aBlock) {
    
    if (Config::useCache(CacheType::block)) {
        if (blockCache.contains(aBlockNum)) {
            aBlock = blockCache.get(aBlockNum);
            return StatusResult{};
        }
    }
    stream->clear();
    stream->seekg(aBlockNum * kBlockSize);
    stream->read((char*)&aBlock, kBlockSize);
    stream->clear();
    
    if (Config::useCache(CacheType::block)) {
        blockCache.put(aBlockNum, aBlock);
    }
    return StatusResult{};
}

StatusResult BlockIO::writeIndexMetaBlock(IndexMetaBlock &aBlock) {
    stream->clear();
    stream->seekg(1 * kBlockSize);
    stream->write((char*)&aBlock, kIndexMetaBlockSize);
    stream->flush();
    stream->clear();
    return StatusResult{};
}

StatusResult BlockIO::readIndexMetaBlock(IndexMetaBlock &aBlock) {
    stream->clear();
    stream->seekg(1 * kBlockSize);
    stream->read((char*)&aBlock, kIndexMetaBlockSize);
    stream->clear();
    return StatusResult{};
}

StatusResult BlockIO::clearBlock(const size_t aBlockNum) {
    StatusResult theRes;
    Block theNewBlock;
    theNewBlock.header.type = static_cast<char>(BlockType::free_block);
    theNewBlock.payload[0] = '\0';
    theRes = writeBlock(aBlockNum, theNewBlock);
    if (!theRes) return theRes;
    if (Config::useCache(CacheType::block)) {
        blockCache.del(aBlockNum);
    }
    return theRes;
}

size_t BlockIO::getBlockCount()  {
    stream->clear();
    stream->seekg(0, std::ios::end);
    stream->clear();
    return stream->tellg() / kBlockSize;
}

// get next free block, 0 means error
size_t BlockIO::findNextFreeBlock() {
    size_t theCount = getBlockCount();
    if (0 == theCount) {
        return 0;
    }
    if (Config::useFreeBlock()) {
        Block aBlock;
        size_t i;
        for (i = metaSize; i < theCount; i++) {
            readBlock(i, aBlock);
            if (static_cast<char>(BlockType::free_block) == aBlock.header.type) {
                break;
            }
        }
        return i; // end of file
    }
    return theCount;
}

}
