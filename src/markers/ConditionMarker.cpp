// StdLib deps
#include <iostream>
#include <sstream>

// Internal deps
#include "ConditionMarker.hpp"
#include "utils/FileIO.hpp"

// Clang deps
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/Tooling.h"



// ------------------------------------------------------------------------
// Anonymous namespace to hide internal methods and classes
// ------------------------------------------------------------------------
namespace
{
    std::string getLocationString(clang::SourceLocation loc, clang::SourceManager* sourceManager){
        std::ostringstream stream;
        stream << sourceManager->getFilename(loc).str() << ":" << sourceManager->getSpellingLineNumber(loc) << ":" << sourceManager->getSpellingColumnNumber(loc);
        return stream.str();
    }

    std::string boolToStr(bool val){
        return val? "true":"false";
    }
}



// ------------------------------------------------------------------------
// Definitions for ConditionMarker
// ------------------------------------------------------------------------
ConditionMarker::ConditionMarker(TestConfig config) {
#ifdef DEBUG
    std::cout << "Creating ConditionMarker" << std::endl;
#endif
    StdOptions::Initialise(config.Options);
    CompilerOptions::Initialise(config.Options);
}

TestResult ConditionMarker::Mark(std::string file) {
    // create and launch frontend action 
    std::vector<std::string> args;
    // args.push_back("-std=c99");
    args.push_back("-I/home/dylan/llvm-project/build/lib/clang/13.0.0/include");
    // args.push_back("-v");

    // read source file
    std::stringstream buf;
    buf << CreateInputStream(file)->rdbuf();
    std::string src = buf.str();

    bool actionStatus = clang::tooling::runToolOnCodeWithArgs(std::make_unique<FrontendAction>(*this), llvm::Twine(src), args, file, "./stylemarker");

    // populate results object
    result.testName = "ConditionMarker";
    result.testDescription = "Checks for expressive 'if' statements and safe Boolean conditions";
    result.marksTotal = MarksAvailable;

    return result;
}

void ConditionMarker::HandleIfStmt(clang::CompilerInstance &ci, clang::IfStmt &stmt) {
    #ifdef DEBUG
    std::cout << "[CALLED ON IF STATEMENT]" << std::endl;
    #endif
}

void ConditionMarker::HandleBinaryOperator(clang::CompilerInstance &ci, clang::BinaryOperator &op) {
    #ifdef DEBUG
    std::cout << "[CALLED ON BINARY OPERATOR]" << std::endl;
    #endif
}
