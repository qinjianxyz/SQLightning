//
//  TestManually.cpp
//  Developed by Jian Qin & Xiaoqiang Qi
//  Mentored by Rick Gessner
//

#ifndef TestManually_h
#define TestManually_h

#include "Application.hpp"
#include "Errors.hpp"
#include <sstream>


void showError(SQLightning::StatusResult &aResult, std::ostream &anOutput) {
    
    static std::map<SQLightning::Errors, std::string> theErrorMessages = {
        {SQLightning::illegalIdentifier, "Illegal identifier"},
        {SQLightning::unknownIdentifier, "Unknown identifier"},
        {SQLightning::databaseExists, "Database exists"},
        {SQLightning::tableExists, "Table Exists"},
        {SQLightning::syntaxError, "Syntax Error"},
        {SQLightning::unknownCommand, "Unknown command"},
        {SQLightning::unknownDatabase,"Unknown database"},
        {SQLightning::unknownTable,   "Unknown table"},
        {SQLightning::unknownError,   "Unknown error"},
        {SQLightning::databaseRemovalError, "Database removal error"}
    };
    
    std::string theMessage="Unknown Error";
    if(theErrorMessages.count(aResult.error)) {
        theMessage=theErrorMessages[aResult.error];
    }
    anOutput << "Error (" << aResult.error << ") " << theMessage << "\n";
}


bool doManualTesting() {
    SQLightning::Application   theApp(std::cout);
    SQLightning::StatusResult  theResult{};
    
    std::string theUserInput;
    bool running=true;
    do {
        std::cout << "\n> ";
        if(std::getline(std::cin, theUserInput)) {
            if(theUserInput.length()) {
                std::stringstream theStream(theUserInput);
                theResult=theApp.handleInput(theStream);
                if(theResult==SQLightning::userTerminated) {
                    running=false;
                }
                else if(!theResult) {
                    showError(theResult, std::cout);
                }
            }
        }
    }
    while (running);
    return theResult;
}

#endif /* TestManually_h */
