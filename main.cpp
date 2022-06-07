//
//  main.cpp
//  Developed by Jian Qin & Xiaoqiang Qi
//  Mentored by Rick Gessner
//

#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#include <functional>
#include <variant>
#include <memory>

#include "TestManually.hpp"
#include "TestAutomatic.hpp"

int main(int argc, const char * argv[]) {
  srand(static_cast<uint32_t>(time(0)));
  if(argc>1) {
    
    std::stringstream theOutput;
    SQLightning::TestAutomatic theTests(theOutput);

    using TestCall = std::function<bool()>;
    static std::map<std::string, TestCall> theCalls {
      {"App",         [&](){return theTests.doAppTest();         }},
      {"AddColumn",   [&](){return theTests.doAddColumnTest();   }},
      {"BlockCache",  [&](){return theTests.doBlockCacheTest();  }},
      {"Compile",     [&](){return theTests.doCompileTest();     }},
      {"Delete",      [&](){return theTests.doDeleteTest();      }},
      {"DropTable",   [&](){return theTests.doDropTest();        }},
      {"DBCommands",  [&](){return theTests.doDBCommandsTest();  }},
      {"Index",       [&](){return theTests.doIndexTest();       }},
      {"Insert",      [&](){return theTests.doInsertTest();      }},
      {"Joins",       [&](){return theTests.doJoinTest();        }},
      {"Reader",      [&](){return theTests.doReaderTest();      }},
      {"RemoveColumn",[&](){return theTests.doRemoveColumnTest();}},
      {"RowCache",    [&](){return theTests.doRowCacheTest();    }},
      {"Select",      [&](){return theTests.doSelectTest();      }},
      {"Tables",      [&](){return theTests.doTablesTest();      }},
      {"Update",      [&](){return theTests.doUpdateTest();      }},
      {"Stress",      [&](){return theTests.doStressTest();      }},
      {"ViewCache",   [&](){return theTests.doViewCacheTest();   }},
    };
    
    std::string theCmd(argv[1]);
    if(theCalls.count(theCmd)) {
      bool theResult = theCalls[theCmd]();
      const char* theStatus[]={"FAIL","PASS"};
      std::cout << theCmd << " test " << theStatus[theResult] << "\n";
      std::cout << "------------------------------\n"
        << theOutput.str() << "\n";
    }
    else std::cout << "Unknown test\n";
  }
  else {
    doManualTesting();
  }
  return 0;
}

