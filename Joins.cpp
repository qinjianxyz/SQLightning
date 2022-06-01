//
//  Joins.cpp
//  Developed by Jian Qin & Xiaoqiang Qi
//  Mentored by Rick Gessner
//


#include "Joins.hpp"

namespace ECE141 {
const std::set<Keywords> Join::gJoinTypes = {
    ECE141::Keywords::cross_kw, ECE141::Keywords::full_kw, ECE141::Keywords::inner_kw,
    ECE141::Keywords::left_kw,  ECE141::Keywords::right_kw
};
}
