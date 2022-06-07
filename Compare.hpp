//
//  Compare.hpp
//  Developed by Jian Qin & Xiaoqiang Qi
//  Mentored by Rick Gessner
//

#ifndef compare_h
#define compare_h

#include <sstream>

class CompareHelper {
public:
    template <typename T>
    static int compareValue(const T& arg1, const T& arg2) {
        if (arg1 < arg2) {
            return -1;
        } else if (arg1 > arg2) {
            return 1;
        } else {
            return 0; // equal
        }
    }
    
    static int compareValue(const std::string& aStr1, const std::string& aStr2) {
        if (aStr1 < aStr2) {
            return -1;
        } else if (aStr1 > aStr2) {
            return 1;
        } else {
            return 0; // equal
        }
    }
    
    template <typename T1, typename T2>
    static int compareValue(const T1& arg1, const T2& arg2) {
        if (arg1 < static_cast<T1>(arg2)) {
            return -1;
        } else if (arg1 > static_cast<T1>(arg2)) {
            return 1;
        } else {
            return 0; // equal
        }
    }
    
    template <typename T1>
    static int compareValue(const T1& arg1, const std::string& aStr) {
        std::stringstream temp;
        temp << arg1;
        std::string theStr = temp.str();
        return compareValue(aStr, theStr);
    }
    
    static int compareValue(const std::string& aStr, const bool& arg2) {
        return compareValue(arg2, aStr);
    }
    
    static int compareValue(const std::string& aStr, const int& arg2) {
        return compareValue(arg2, aStr);
    }
    
    static int compareValue(const std::string& aStr, const double& arg2) {
        return compareValue(arg2, aStr);
    }
    
    // is equal
    template <typename T1, typename T2>
    static bool isEqual(const T1& arg1, const T2& arg2) {
        return 0 == compareValue(arg1, arg2);
    }
    
    // Less than ------------------------------------------
    template <typename T1, typename T2>
    static bool isLessThan(const T1& arg1, const T2& arg2) {
        return -1 == compareValue(arg1, arg2);
    }
    
    // Greater than ------------------------------------------
    template <typename T1, typename T2>
    static bool isGreaterThan(const T1& arg1, const T2& arg2) {
        return 1 == compareValue(arg1, arg2);
    }
    
    // Less than or equal to ------------------------------------------
    template <typename T1, typename T2>
    static bool isLessThanOrEqualTo(const T1& arg1, const T2& arg2) {
        return !isGreaterThan(arg1, arg2);
    }
    
    // Greater than ------------------------------------------
    template <typename T1, typename T2>
    static bool isGreaterThanOrEqualTo(const T1& arg1, const T2& arg2) {
        return !isLessThan(arg1, arg2);
    }
};





#endif /* compare_h */
