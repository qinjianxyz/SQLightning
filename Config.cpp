//
//  Config.cpp
//  ece141b-index
//
//  Created by Jian Qin on 5/26/22.
//

#include "Config.hpp"

namespace ECE141 {

const bool Config::indexMode  = true;

// {block=0, row=1, view=2};
size_t Config::cacheSize[3] = {1, 1, 1};

}

