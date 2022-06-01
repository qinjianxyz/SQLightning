//
//  AboutUs.hpp
//  Developed by Jian Qin & Xiaoqiang Qi
//  Mentored by Rick Gessner
//

#ifndef AboutMe_hpp
#define AboutMe_hpp

#include <stdio.h>
#include <string>
#include <optional>
#include <vector>

namespace ECE141 {

using StringOpt = std::optional<std::string>;

/**
 The class contains information about the developers of this project
 */
class AboutUs {
public:
    AboutUs() {};
    ~AboutUs() = default;
    static const std::vector<std::string> names;
    static const std::vector<std::string> githubUsernames;
    
    size_t getTeamSize() const { return 2; }
    
    StringOpt getName(size_t anIndex) const {
        if (anIndex < getTeamSize()) {
            return names[anIndex];
        }
        return std::nullopt;
    }
    
    StringOpt getGithubUsername(size_t anIndex) const {
        if (anIndex < getTeamSize()) {
            return githubUsernames[anIndex];
        }
        return std::nullopt;
    }
};
}

#endif /* about_me */
