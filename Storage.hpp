//
//  Storage.hpp
//  Developed by Jian Qin & Xiaoqiang Qi
//  Mentored by Rick Gessner
//

#ifndef Storage_hpp
#define Storage_hpp

#include <string>
#include <fstream>
#include <iostream>
#include <deque>
#include <stack>
#include <optional>
#include <functional>
#include <cmath>
#include <map>
#include <cstdlib>
#include <optional>
#include <cstring>
#include <sstream>

#include "Config.hpp"
#include "BlockIO.hpp"
#include "Errors.hpp"
#include "Entity.hpp"
#include "Storable.hpp"

namespace ECE141 {

class Database;
using BlockVisitor = std::function<bool(const Block&, size_t)>;

/**
 The controller for storage related operation
 */
class Storage : public BlockIO {
    LRUCache<size_t, Row> rowCache;
public:
    Storage(std::string aPath);
    ~Storage();
    
    StatusResult        each(const BlockVisitor &aVisitor);
    StatusResult        readEntity(const size_t aBlockNum, Entity& anEntity);
    size_t              writeEntity(const Entity &anEntity);
    StatusResult        updateEntity(const Entity &anEntity, const size_t aBlockNumber);
    StatusResult        readRow(const size_t aBlockNum, Row& aRow);
    StatusResult        readRowName(const size_t aBlockNum, std::string& aString);
    StatusResult        clearRow(const size_t aBlockNum);
    size_t              writeNewRow(Row& aRow);
    StatusResult        writeExistingRow(const Row& aRow);
    StatusResult        pushPayload(Block& theBlock, const Storable& theStorable);
    std::stringstream   pullPayload(Block& theBlock);
};
}


#endif /* Storage_hpp */
