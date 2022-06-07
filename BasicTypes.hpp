//
//  BasicTypes.hpp
//  Developed by Jian Qin & Xiaoqiang Qi
//  Mentored by Rick Gessner
//

#ifndef BasicTypes_h
#define BasicTypes_h

#include <string>
#include <variant>
#include <vector>
#include <map>
#include <optional>

namespace SQLightning {

/**
 The class contains customed designed data types
 */
enum class DataTypes {
    no_type='N',  bool_type='B', datetime_type='D',
    float_type='F', int_type='I', varchar_type='V',
};

using StringList    = std::vector<std::string>;
using StringMap     = std::map<std::string, std::string>;
using StringOpt     = std::optional<std::string>;
using IntOpt        = std::optional<uint32_t>;
using Value         = std::variant<bool, int, double, std::string>;
using ValueOpt      = std::optional<Value>;
using KeyValues     = std::map<std::string, Value>;
using KeyValuePair  = std::pair<std::string, Value>;

}
#endif /* BasicTypes_h */
