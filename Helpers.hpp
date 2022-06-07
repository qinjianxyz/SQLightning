//
//  Helpers.hpp
//  Developed by Jian Qin & Xiaoqiang Qi
//  Mentored by Rick Gessner
//

#ifndef Helpers_h
#define Helpers_h

#include "BasicTypes.hpp"
#include "keywords.hpp"
#include "Tokenizer.hpp"

namespace SQLightning {


/**
 The file contains helper functions used throughout the database
 */

template<typename T, size_t aSize>
bool in_array(T (&anArray)[aSize], const T &aValue) {
    for(size_t i=0;i<aSize;i++) {
        if(anArray[i]==aValue) return true;
    }
    return false;
}

static std::map<std::string, Operators> gExpressionOps = {
    {"=",   Operators::equal_op},
    {"<",   Operators::lt_op},
    {"<=",  Operators::lte_op},
    {">",   Operators::gt_op},
    {">=",  Operators::gte_op},
    {"!=",  Operators::notequal_op},
    {"not", Operators::notequal_op},
};

static std::map<std::string, Logical> gExpressionLogs = {
    {"not", Logical::not_op},
    {"and", Logical::and_op},
    {"or",  Logical::or_op}
};

static std::map<std::string, Operators> gOperators = {
    std::make_pair(".",   Operators::dot_op),
    std::make_pair("+",   Operators::add_op),
    std::make_pair("-",   Operators::subtract_op),
    std::make_pair("*",   Operators::multiply_op),
    std::make_pair("/",   Operators::divide_op),
    std::make_pair("^",   Operators::power_op),
    std::make_pair("%",   Operators::mod_op),
    std::make_pair("=",   Operators::equal_op),
    std::make_pair("!=",  Operators::notequal_op),
    std::make_pair("not", Operators::notequal_op),
    std::make_pair("<",   Operators::lt_op),
    std::make_pair("<=",  Operators::lte_op),
    std::make_pair(">",   Operators::gt_op),
    std::make_pair(">=",  Operators::gte_op),
    std::make_pair("and", Operators::and_op),
    std::make_pair("or",  Operators::or_op),
    std::make_pair("nor", Operators::nor_op),
    std::make_pair("between", Operators::between_op),
};

//a list of known functions...
static std::map<std::string,int> gFunctions = {
    std::make_pair("avg", 10),
    std::make_pair("count", 20),
    std::make_pair("max", 30),
    std::make_pair("min", 40)
};


//This map binds a keyword string with a Keyword (token)...
static std::map<std::string,  Keywords> gDictionary = {
    std::make_pair("add",       Keywords::add_kw),
    std::make_pair("all",       Keywords::all_kw),
    std::make_pair("alter",     Keywords::alter_kw),
    std::make_pair("and",       Keywords::and_kw),
    std::make_pair("as",        Keywords::as_kw),
    std::make_pair("asc",       Keywords::asc_kw),
    std::make_pair("avg",       SQLightning::Keywords::avg_kw),
    std::make_pair("auto_increment", Keywords::auto_increment_kw),
    std::make_pair("between",   SQLightning::Keywords::between_kw),
    std::make_pair("boolean",   SQLightning::Keywords::boolean_kw),
    std::make_pair("by",        SQLightning::Keywords::by_kw),
    std::make_pair("change",    SQLightning::Keywords::change_kw),
    std::make_pair("changed",    SQLightning::Keywords::changed_kw),
    std::make_pair("char",      SQLightning::Keywords::char_kw),
    std::make_pair("column",    SQLightning::Keywords::column_kw),
    std::make_pair("count",     SQLightning::Keywords::count_kw),
    std::make_pair("create",    SQLightning::Keywords::create_kw),
    std::make_pair("cross",     SQLightning::Keywords::cross_kw),
    std::make_pair("current_date",  Keywords::current_date_kw),
    std::make_pair("current_time",  Keywords::current_time_kw),
    std::make_pair("current_timestamp", Keywords::current_timestamp_kw),
    std::make_pair("database",  SQLightning::Keywords::database_kw),
    std::make_pair("databases", SQLightning::Keywords::databases_kw),
    std::make_pair("datetime",  SQLightning::Keywords::datetime_kw),
    std::make_pair("decimal",   SQLightning::Keywords::decimal_kw),
    std::make_pair("desc",      SQLightning::Keywords::desc_kw),
    std::make_pair("delete",    SQLightning::Keywords::delete_kw),
    std::make_pair("describe",  SQLightning::Keywords::describe_kw),
    std::make_pair("distinct",  SQLightning::Keywords::distinct_kw),
    std::make_pair("double",    SQLightning::Keywords::double_kw),
    std::make_pair("drop",      SQLightning::Keywords::drop_kw),
    std::make_pair("dump",      SQLightning::Keywords::dump_kw),
    std::make_pair("enum",      SQLightning::Keywords::enum_kw),
    std::make_pair("explain",   SQLightning::Keywords::explain_kw),
    std::make_pair("false",     SQLightning::Keywords::false_kw),
    std::make_pair("float",     SQLightning::Keywords::float_kw),
    std::make_pair("foreign",   SQLightning::Keywords::foreign_kw),
    std::make_pair("from",      SQLightning::Keywords::from_kw),
    std::make_pair("full",      SQLightning::Keywords::full_kw),
    std::make_pair("group",     SQLightning::Keywords::group_kw),
    std::make_pair("help",      SQLightning::Keywords::help_kw),
    std::make_pair("in",        SQLightning::Keywords::in_kw),
    std::make_pair("index",     SQLightning::Keywords::index_kw),
    std::make_pair("indexes",   SQLightning::Keywords::indexes_kw),
    std::make_pair("inner",     SQLightning::Keywords::inner_kw),
    std::make_pair("insert",    SQLightning::Keywords::insert_kw),
    std::make_pair("int",       SQLightning::Keywords::integer_kw),
    std::make_pair("integer",   SQLightning::Keywords::integer_kw),
    std::make_pair("into",      SQLightning::Keywords::into_kw),
    std::make_pair("join",      SQLightning::Keywords::join_kw),
    std::make_pair("key",       SQLightning::Keywords::key_kw),
    std::make_pair("last",      SQLightning::Keywords::last_kw),
    std::make_pair("left",      SQLightning::Keywords::left_kw),
    std::make_pair("like",      SQLightning::Keywords::like_kw),
    std::make_pair("limit",     SQLightning::Keywords::limit_kw),
    std::make_pair("max",       SQLightning::Keywords::max_kw),
    std::make_pair("min",       SQLightning::Keywords::min_kw),
    std::make_pair("modify",    SQLightning::Keywords::modify_kw),
    std::make_pair("not",       SQLightning::Keywords::not_kw),
    std::make_pair("null",      SQLightning::Keywords::null_kw),
    std::make_pair("on",        SQLightning::Keywords::on_kw),
    std::make_pair("or",        SQLightning::Keywords::or_kw),
    std::make_pair("order",     SQLightning::Keywords::order_kw),
    std::make_pair("outer",     SQLightning::Keywords::outer_kw),
    std::make_pair("primary",   SQLightning::Keywords::primary_kw),
    std::make_pair("query",     SQLightning::Keywords::query_kw),
    std::make_pair("quit",      SQLightning::Keywords::quit_kw),
    std::make_pair("references", SQLightning::Keywords::references_kw),
    std::make_pair("right",     SQLightning::Keywords::right_kw),
    std::make_pair("rows",      SQLightning::Keywords::rows_kw),
    std::make_pair("select",    SQLightning::Keywords::select_kw),
    std::make_pair("self",      SQLightning::Keywords::self_kw),
    std::make_pair("set",       SQLightning::Keywords::set_kw),
    std::make_pair("show",      SQLightning::Keywords::show_kw),
    std::make_pair("sum",       SQLightning::Keywords::sum_kw),
    std::make_pair("table",     SQLightning::Keywords::table_kw),
    std::make_pair("tables",    SQLightning::Keywords::tables_kw),
    std::make_pair("true",      SQLightning::Keywords::true_kw),
    std::make_pair("unique",    SQLightning::Keywords::unique_kw),
    std::make_pair("update",    SQLightning::Keywords::update_kw),
    std::make_pair("use",       SQLightning::Keywords::use_kw),
    std::make_pair("values",    SQLightning::Keywords::values_kw),
    std::make_pair("varchar",   SQLightning::Keywords::varchar_kw),
    std::make_pair("version",   SQLightning::Keywords::version_kw),
    std::make_pair("where",     SQLightning::Keywords::where_kw),
    std::make_pair("run",       SQLightning::Keywords::run_kw)
};

static std::map<Keywords, DataTypes> gKeywordTypes = {
    std::make_pair(Keywords::boolean_kw, DataTypes::bool_type),
    std::make_pair(Keywords::datetime_kw, DataTypes::datetime_type),
    std::make_pair(Keywords::float_kw, DataTypes::float_type),
    std::make_pair(Keywords::integer_kw, DataTypes::int_type),
    std::make_pair(Keywords::varchar_kw, DataTypes::varchar_type)
};

class Helpers {
public:
    
    //be very careful about collisions!
    static size_t hashString(const char *str) {
        uint32_t h{0};
        unsigned char *p;
        const int gMultiplier = 37;
        for (p = (unsigned char*)str; *p != '\0'; p++)
            h = gMultiplier * h + *p;
        return h;
    }
    
    static Keywords getKeywordId(const std::string aKeyword) {
        auto theIter = gDictionary.find(aKeyword);
        if (theIter != gDictionary.end()) {
            return theIter->second;
        }
        return Keywords::unknown_kw;
    }
    
    template<typename T, size_t aSize>
    static bool in_array(T (&anArray)[aSize], T &aValue) {
        return std::find(std::begin(anArray), std::end(anArray), aValue);
    };
    
    //convert from char to keyword...
    static Keywords charToKeyword(char aChar) {
        switch(toupper(aChar)) {
            case 'I': return Keywords::integer_kw;
            case 'T': return Keywords::datetime_kw;
            case 'B': return Keywords::boolean_kw;
            case 'F': return Keywords::float_kw;
            case 'V': return Keywords::varchar_kw;
            default:  return Keywords::unknown_kw;
        }
    }
    
    static const char* dataTypeToString(DataTypes aType) {
        switch(aType) {
            case DataTypes::no_type:        return "none";
            case DataTypes::bool_type:      return "bool";
            case DataTypes::datetime_type:  return "datetime";
            case DataTypes::float_type:     return "float";
            case DataTypes::int_type:       return "int";
            case DataTypes::varchar_type:   return "varchar";
            default: return nullptr;
        }
    }
    
    static const char* keywordToString(Keywords aType) {
        switch(aType) {
            case Keywords::boolean_kw:    return "bool";
            case Keywords::create_kw:     return "create";
            case Keywords::database_kw:   return "database";
            case Keywords::databases_kw:  return "databases";
            case Keywords::datetime_kw:   return "datetime";
            case Keywords::describe_kw:   return "describe";
            case Keywords::drop_kw:       return "drop";
            case Keywords::float_kw:      return "float";
            case Keywords::integer_kw:    return "integer";
            case Keywords::show_kw:       return "show";
            case Keywords::table_kw:      return "table";
            case Keywords::tables_kw:     return "tables";
            case Keywords::use_kw:        return "use";
            case Keywords::varchar_kw:    return "varchar";
            default:                      return "unknown";
        }
    }
    
    // USE: ---validate that given keyword is a datatype...
    static bool isDatatype(Keywords aKeyword) {
        switch(aKeyword) {
            case Keywords::char_kw:
            case Keywords::datetime_kw:
            case Keywords::float_kw:
            case Keywords::integer_kw:
            case Keywords::varchar_kw:
                return true;
            default: return false;
        }
    }
    
    static const DataTypes keywordToDatatype(Keywords aType) {
        switch (aType) {
            case Keywords::integer_kw:  return DataTypes::int_type;
            case Keywords::float_kw:    return DataTypes::float_type;
            case Keywords::boolean_kw:  return DataTypes::bool_type;
            case Keywords::datetime_kw: return DataTypes::datetime_type;
            case Keywords::varchar_kw:  return DataTypes::varchar_type;
            default:                    return DataTypes::no_type;
        }
    }
    
    static DataTypes getTypeForKeyword(Keywords aKeyword) {
        return gKeywordTypes.count(aKeyword)
        ? gKeywordTypes[aKeyword] : DataTypes::no_type;
    }
    
    static Value writeDefaultValueForDataType(DataTypes aType) {
        switch (aType) {
            case DataTypes::int_type     :  return 0;
            case DataTypes::float_type   :  return 0.0;
            case DataTypes::bool_type    :  return false;
            case DataTypes::varchar_type :  return "";
            default                      :  return 0;
        }
    }
    
    static Operators toOperator(std::string aString) {
        auto theIter = gOperators.find(aString);
        if (theIter != gOperators.end()) {
            return theIter->second;
        }
        return Operators::unknown_op;
    }
    
    static int getFunctionId(const std::string anIdentifier) {
        auto theIter = gFunctions.find(anIdentifier);
        if (theIter != gFunctions.end()) {
            return theIter->second;
        }
        return 0;
    }
    
    static bool isNumericKeyword(Keywords aKeyword) {
        static Keywords theTypes[]={Keywords::decimal_kw, Keywords::double_kw, Keywords::float_kw, Keywords::integer_kw};
        for(auto k : theTypes) {
            if(aKeyword==k) return true;
        }
        return false;
    }
    
    static bool findPointinNum(std::string aString, int num) {
        for (auto ch : aString) {
            if ('.' == ch) {
                num--;
                if (num < 0) return false;
            }
            if (ch > '9' && ch < '0') {
                return false;
            }
            
        }
        return true;
    }
    
    static bool isEqualCmd(Tokenizer& aTokenizer, std::vector<Keywords> aKeywordList) {
        if (aTokenizer.remaining() < aKeywordList.size()) return false;
        for (int i = 0; i < aKeywordList.size(); i++) {
            if (aKeywordList[i] != aTokenizer.peek(i).keyword) {
                return false;
            }
        }
        return true;
    }
    
    static std::string valueToString(Value aValue) {
        switch (aValue.index())
        {
            case 0:
                return std::get<bool>(aValue) ? "true" : "false";
                break;
            case 1:
                return std::to_string(std::get<int>(aValue));
                break;
            case 2:
                return std::to_string(std::get<double>(aValue));
                break;
            case 3:
                return std::get<std::string>(aValue);
            default:
                break;
        }
        return "";
    }
    
    static std::string dashGenerator(size_t aNum) {
        std::string theOutput = "";
        for (size_t index = 0; index < aNum; index++) {
            theOutput += "-";
        }
        return theOutput;
    }
    
    static StatusResult parseComma(Tokenizer& aTokenizer, std::vector<std::string>& strVec) {
        while (aTokenizer.more()) {
            strVec.push_back(aTokenizer.current().data);
            aTokenizer.next();
            if (!aTokenizer.more() || "," != aTokenizer.peek(0).data) break;
            aTokenizer.next();
        }
        return StatusResult{};
    }
    
};

}

#endif /* Helpers_h */
