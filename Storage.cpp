//
//  Storage.cpp
//  Developed by Jian Qin & Xiaoqiang Qi
//  Mentored by Rick Gessner
//

#include "Storage.hpp"

namespace ECE141 {

Storage::Storage(std::string aPath) : BlockIO(aPath, false) {}

Storage::~Storage() {}

StatusResult Storage::each(const BlockVisitor &aVisitor) {
    size_t theCount = getBlockCount();
    Block theBlock;
    for(size_t i = 0; i < theCount; i++) {
        if(readBlock(i, theBlock)) {
            if(!aVisitor(theBlock, i)) {
                break;
            }
        }
    }
    return StatusResult{};
}

StatusResult Storage::pushPayload(Block& aBlock, const Storable& aStorable) {
    std::stringstream ss;
    aStorable.encode(ss);
    memcpy(aBlock.payload, ss.str().c_str(), kPayloadSize);
    return StatusResult{};
}

std::stringstream Storage::pullPayload(Block& aBlock) {
    std::stringstream ss;
    ss << aBlock.payload;
    return ss;
}

size_t Storage::writeEntity(const Entity &anEntity) {
    stream->clear();
    size_t theBlockNum = findNextFreeBlock();
    Block theBlock(BlockType::entity_block);
    pushPayload(theBlock, anEntity);
    writeBlock(theBlockNum, theBlock);
    return theBlockNum;
}

StatusResult Storage::updateEntity(const Entity &anEntity, const size_t aBlockNumber) {
    Block theBlock(BlockType::entity_block);
    pushPayload(theBlock, anEntity);
    writeBlock(aBlockNumber, theBlock);
    return StatusResult{};
}

StatusResult Storage::readEntity(const size_t aBlockNum, Entity& anEntity) {
    Block theBlock;
    readBlock(aBlockNum, theBlock);
    if (static_cast<char>(BlockType::entity_block) != theBlock.header.type) {
        return StatusResult{ readError };
    }
    auto ss = pullPayload(theBlock);
    anEntity.decode(ss);
    return StatusResult{};
}

StatusResult Storage::readRow(const size_t aBlockNum, Row& aRow) {
    Block theBlock;
    readBlock(aBlockNum, theBlock);
    if (static_cast<char>(BlockType::data_block) != theBlock.header.type) {
        return StatusResult{};
    }
    auto ss = pullPayload(theBlock);
    aRow.decode(ss);
    aRow.setRowNumber(aBlockNum); // set row number
    return StatusResult{};
}

StatusResult Storage::readRowName(const size_t aBlockNum, std::string& aString) {
    Block theBlock;
    readBlock(aBlockNum, theBlock);
    if (static_cast<char>(BlockType::data_block) != theBlock.header.type) {
        return StatusResult{};
    }
    auto ss = pullPayload(theBlock);
    StreamDecoder aDecoder(&ss);
    aDecoder.decode(aString); // decode only the name
    return StatusResult{};
}

size_t Storage::writeNewRow(const Row& aRow) {
    stream->clear();
    size_t theBlockNum = findNextFreeBlock();
    Block theBlock(BlockType::data_block);
    pushPayload(theBlock, aRow);
    writeBlock(theBlockNum, theBlock);
    return theBlockNum;
}

StatusResult Storage::writeExistingRow(const Row& aRow) {
    stream->clear();
    size_t theBlockNum = aRow.getNum();
    Block theBlock(BlockType::data_block);
    pushPayload(theBlock, aRow);
    writeBlock(theBlockNum, theBlock);
    return StatusResult{};
}

StatusResult Storage::clearBlock(const size_t aBlockNum) {
    StatusResult theRes;
    Block theNewBlock;
    theNewBlock.header.type = static_cast<char>(BlockType::free_block);
    theNewBlock.payload[0] = '\0';
    theRes = writeBlock(aBlockNum, theNewBlock);
    if (!theRes) return theRes;
    return theRes;
}

}

