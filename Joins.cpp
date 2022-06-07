//
//  Joins.cpp
//  Developed by Jian Qin & Xiaoqiang Qi
//  Mentored by Rick Gessner
//


#include "Joins.hpp"

namespace SQLightning {
const std::set<Keywords> Join::gJoinTypes = {
    SQLightning::Keywords::cross_kw, SQLightning::Keywords::full_kw, SQLightning::Keywords::inner_kw,
    SQLightning::Keywords::left_kw,  SQLightning::Keywords::right_kw
};
}
