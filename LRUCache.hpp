//
//  LRUCache.hpp
//  ece141b-cache
//
//  Created by Jian Qin on 6/2/22.
//

#ifndef LRUCache_hpp
#define LRUCache_hpp

#include <stdio.h>
#include <list>
#include <unordered_map>

namespace ECE141 {

template<typename KeyT, typename ValueT>
class LRUCache {
public:
    LRUCache() {}
    ~LRUCache() {}
    
    // OCF
    
    // put a key-value pair in cache
    void put(const KeyT &key, const ValueT& value) {
        if (contains(key)) {
            // update an existing pair
            theCacheTable[key]->second = value;
            theCacheList.splice(theCacheList.begin(), theCacheList, theCacheTable[key]);
        }
        else {
            // add a new pair
            theCacheList.push_front({ key, value });
            theCacheTable[key] = theCacheList.begin();
            // remove redundant pair
            if (size() > maxSize()) {
                theCacheTable.erase(theCacheList.back().first);
                theCacheList.pop_back();
            }
        }
    }
    
    // get a value from cache corresponding to the key
    ValueT& get(const KeyT& key) {
        theCacheList.splice(theCacheList.begin(), theCacheList, theCacheTable[key]);
        return theCacheTable[key]->second;
    }
    
    bool contains(const KeyT& key) const {
        return theCacheTable.count(key);
    }
    
    bool del(const KeyT& key) {
        if (contains(key)) {
            theCacheList.erase(theCacheTable[key]);
            theCacheTable.erase(key);
            
            return true;
        }
        return false;
    }
    
    size_t size() const {
        return theCacheTable.size();
    }
    
    static size_t maxSize() {
        return 1000;
    }
    
protected:
    using KeyValueList = std::list<std::pair<KeyT, ValueT>>;
    KeyValueList theCacheList;
    // "typename" is necessary to specify explicitly "iterator" is a type name instead of variable or function name
    std::unordered_map<KeyT, typename KeyValueList::iterator> theCacheTable;
    
};

}

#endif /* LRUCache_hpp */
