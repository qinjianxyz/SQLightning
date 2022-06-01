//
//  Index.hpp
//  RGAssignment3
//
//  Created by rick gessner on 4/2/21.
//

#ifndef Index_hpp
#define Index_hpp

#include <stdio.h>
#include <map>
#include <functional>
#include <string>
#include <vector>

#include "Storage.hpp"
#include "BasicTypes.hpp"
#include "Errors.hpp"
#include "Storable.hpp"
#include "TypedStream.hpp"


namespace ECE141 {

class Index : public Storable {
public:
    Index() {};
    ~Index() = default;
    
    StatusResult encode(std::ostream& anOutput) const {
        StatusResult theRes;
        StreamEncoder anEncoder(&anOutput);
        for (auto it = theMap.begin(); it != theMap.end(); it++) {
            anEncoder.encode(it->first).encode(it->second);
        }
        return theRes;
    }
    
    StatusResult decode(std::istream& anInput) {
        StatusResult theRes;
        StreamDecoder aDecoder(&anInput);
        std::string theName;
        int theValue;
        while (aDecoder.more()) {
            aDecoder.decode(theName).decode(theValue);
            theMap.insert(std::make_pair(theName, theValue));
        }
        return theRes;
    }
    
    StatusResult set(const std::string &aKey, const int &aValue) {
        theMap[aKey] = aValue;
        return StatusResult{};
    }
    
    StatusResult del(const size_t aBlockNumber) {
        for (auto it = theMap.begin(); it != theMap.end(); it++) {
            if ((it->second) == aBlockNumber) {
                theMap.erase(it);
                return StatusResult{};
            }
        }
        return StatusResult{ databaseRemovalError };
    }
    
    std::map<std::string, int>& getData() {
        return theMap;
    }
    
    size_t size() {
        return theMap.size();
    }
    
protected:
    std::map<std::string, int> theMap;
};

/*
 enum class IndexType {intKey=0, strKey};
 using IndexVisitor = std::function<bool(const IndexKey&, uint32_t)>;
 using IndexMap = std::map<std::string, std::unique_ptr<Index>>;
 struct Index : public Storable, BlockIterator {
 
 Index(Storage &aStorage, uint32_t aBlockNum=0, IndexType aType=IndexType::intKey)
 :  storage(aStorage), type(aType), blockNum(aBlockNum) {
 changed = false;
 entityId=0;
 }
 
 class ValueProxy {
 public:
 Index     &index;
 IndexKey  key;
 IndexType type;
 
 ValueProxy(Index &anIndex, uint32_t aKey)
 : index(anIndex), key(aKey), type(IndexType::intKey) {}
 
 ValueProxy(Index &anIndex, const std::string &aKey)
 : index(anIndex), key(aKey), type(IndexType::strKey) {}
 
 ValueProxy& operator= (uint32_t aValue) {
 index.setKeyValue(key,aValue);
 return *this;
 }
 
 operator IntOpt() {return index.valueAt(key);}
 }; //value proxy
 
 ValueProxy operator[](const std::string &aKey) {
 return ValueProxy(*this,aKey);
 }
 
 ValueProxy operator[](uint32_t aKey) {
 return ValueProxy(*this,aKey);
 }
 
 uint32_t getBlockNum() const {return blockNum;}
 Index&   setBlockNum(uint32_t aBlockNum) {
 blockNum=aBlockNum;
 return *this;
 }
 
 bool    isChanged() {return changed;}
 Index&  setChanged(bool aChanged) {
 changed=aChanged; return *this;
 }
 
 StorageInfo getStorageInfo(size_t aSize) {
 //student complete...
 }
 
 IntOpt valueAt(IndexKey &aKey) {
 return exists(aKey) ? data[aKey] : (IntOpt)(std::nullopt);
 }
 
 bool setKeyValue(IndexKey &aKey, uint32_t aValue) {
 data[aKey]=aValue;
 return changed=true; //side-effect indended!
 }
 
 StatusResult erase(const std::string &aKey) {
 //student implement
 return StatusResult{Errors::noError};
 }
 
 StatusResult erase(uint32_t aKey) {
 //student implement
 return StatusResult{Errors::noError};
 }
 
 size_t getSize() {return data.size();}
 
 bool exists(IndexKey &aKey) {
 return data.count(aKey);
 }
 
 StatusResult encode(std::ostream &anOutput) override {
 //student implement
 return StatusResult{Errors::noError};
 }
 
 StatusResult decode(std::istream &anInput) override {
 //student implement...
 return StatusResult{Errors::noError};
 }
 
 //visit blocks associated with index
 bool each(BlockVisitor aVisitor) override {
 Block theBlock;
 for(auto thePair : data) {
 if(storage.readBlock(thePair.second, theBlock)) {
 if(!aVisitor(theBlock,thePair.second)) {return false;}
 }
 }
 return true;
 }
 
 //visit index values (key, value)...
 bool eachKV(IndexVisitor aCall) {
 for(auto thePair : data) {
 if(!aCall(thePair.first,thePair.second)) {
 return false;
 }
 }
 return true;
 }
 
 protected:
 
 Storage                       &storage;
 std::map<IndexKey, uint32_t>  data;
 IndexType                     type;
 std::string                   name;
 bool                          changed;
 uint32_t                      blockNum; //where index storage begins
 uint32_t                      entityId;
 };
 */

}


#endif /* Index_hpp */
