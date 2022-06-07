//
//  View.hpp
//  Developed by Jian Qin & Xiaoqiang Qi
//  Mentored by Rick Gessner
//

#ifndef View_h
#define View_h


#include <iostream>
#include "Errors.hpp"

namespace SQLightning {

/**
 completely generic view, which you will subclass to show information
 */
class View {
public:
    View(std::ostream& anOutput) : output(anOutput) {}
    virtual                 ~View() {}
    virtual StatusResult    show()=0;
protected:
    std::ostream& output;
};

}

#endif /* View_h */
