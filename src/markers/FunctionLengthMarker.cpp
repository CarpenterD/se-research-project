// StdLib deps
#include <iostream>
#include <sstream>

// Internal deps
#include "FunctionLengthMarker.hpp"
#include "utils/FileIO.hpp"

// Clang deps
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/CommandLine.h"



// ------------------------------------------------------------------------
// Anonymous namespace to hide internal methods and classes
// ------------------------------------------------------------------------
namespace
{
    class FuncDeclVisitor : public clang::RecursiveASTVisitor<FuncDeclVisitor>
    {
        public:
            std::vector<clang::FunctionDecl *> functions;
            
            explicit FuncDeclVisitor(clang::SourceManager& sm);
            bool VisitFunctionDecl(clang::FunctionDecl* func);
            
        private:
            clang::SourceManager& sm;
            
    };


    class MyASTConsumer : public clang::ASTConsumer
    {
        public:
            explicit MyASTConsumer(clang::CompilerInstance& ci, FunctionLengthMarker &marker, TestResult &result);
            virtual void HandleTranslationUnit(clang::ASTContext& astContext);

        private:
            clang::ASTContext &ctx;
            clang::SourceManager &sm;
            FuncDeclVisitor visitor;

            FunctionLengthMarker &marker;
            TestResult &result;
            
            void ProcessFunction(clang::FunctionDecl &func);
            int CalculateStmtLength(clang::Stmt &stmt);
    };


    class FunctionDeclFrontendAction : public clang::ASTFrontendAction
    {
        public:
            FunctionDeclFrontendAction(FunctionLengthMarker &marker, TestResult &result);
            virtual std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance& CI, clang::StringRef file);

        private:
            TestResult &result;
            FunctionLengthMarker &marker;
    };


    // helper functions
    SourceLocation ConvertSrcLoc(clang::SourceManager &sm, clang::SourceLocation sl) {
        SourceLocation loc(sm.getFilename(sl).str(), sm.getSpellingLineNumber(sl), sm.getSpellingColumnNumber(sl));
        return loc;
    };
}



// ------------------------------------------------------------------------
// Definitions for FuncDeclVisitor
// ------------------------------------------------------------------------
// constructor
FuncDeclVisitor::FuncDeclVisitor(clang::SourceManager& sm)
    : sm(sm)
{
    // empty constructor
}

// (override) recursive AST visiting function 
bool FuncDeclVisitor::VisitFunctionDecl(clang::FunctionDecl* func)
{
    if (func != NULL && sm.isWrittenInMainFile(func->getSourceRange().getBegin()))
    {
        functions.push_back(func);
    }
    return true;
}



// ------------------------------------------------------------------------
// Definitions for MyASTConsumer
// ------------------------------------------------------------------------
MyASTConsumer::MyASTConsumer(clang::CompilerInstance& ci, FunctionLengthMarker &marker, TestResult &result)
    : ctx(ci.getASTContext()), sm(ci.getSourceManager()), visitor(sm), marker(marker), result(result)
{
    // empty constructor
}

void MyASTConsumer::HandleTranslationUnit(clang::ASTContext& astContext) {
    // find function definitions and process one by one
    visitor.TraverseDecl(astContext.getTranslationUnitDecl());
    for (size_t f = 0; f < visitor.functions.size(); f++) {
        if (visitor.functions[f] != NULL) {
            ProcessFunction(*visitor.functions[f]);
        }
    }

    // add comment to test result
    double ratio = result.feedback.size() / (double) visitor.functions.size();
    if (ratio == 0) {
        result.remarks = "All functions were of an apppropriate and readable length.";
    } else if (ratio <= 0.2) {
        result.remarks = "Functions were generally of an apppropriate length.";
    } else if (ratio <= 0.5) {
        result.remarks = "A number of functions were longer than reccomended, affecting the readability of the code.";
    } else {
        result.remarks = "Functions were generally long and lacked readability. Improvement required.";
    }
}

void MyASTConsumer::ProcessFunction(clang::FunctionDecl &func) {
    // get name & location
    std::string name = func.getNameInfo().getName().getAsString();
    SourceLocation loc = ConvertSrcLoc(sm, func.getSourceRange().getBegin());
#ifdef DEBUG
    std::clog << "Function '" << name << "' at l:" << std::to_string(loc.line) << "c:" << std::to_string(loc.column) << std::endl;
#endif
    if (!func.isThisDeclarationADefinition() ||  !func.hasBody()) {
        return;
    }

    int length = CalculateStmtLength(*func.getBody()); // already checked for body; safe to dereference

    if (length > marker.AcceptableThreshold){
        // penalise
        std::stringstream msg;
        msg << "The function '" << name << "' is too long and impedes readability. ";
        msg << "Break the function into a series of smaller and more independent functions. ";
        msg << "Note that ideally a single function should perform a single, logically-interconnected task.";
        result.feedback.push_back(FeedbackItem(SourceLocation(loc.file, loc.line, loc.column), msg.str(), FeedbackSeverity::MODERATE));
    } else if (length > marker.SuggestionThreshold) {
        // make suggestion
        std::stringstream msg;
        msg << "The function '" << name << "' is quite lengthy. ";
        msg << "To increase readability, consider moving independent sections of code from '" << name << "' ";
        msg << "into a new function and calling it in place of the moved sections.";
        result.feedback.push_back(FeedbackItem(SourceLocation(loc.file, loc.line, loc.column), msg.str()));
    }
    
    return;
};

int MyASTConsumer::CalculateStmtLength(clang::Stmt &stmt){
    SourceLocation start = ConvertSrcLoc(sm, stmt.getSourceRange().getBegin());
    SourceLocation end   = ConvertSrcLoc(sm, stmt.getSourceRange().getEnd());

#ifdef DEBUG
    std::clog << "\tFinding length of statement from (" + std::to_string(start.line) + ":" + std::to_string(start.column);
    std::clog << ") to (" + std::to_string(end.line) + ":" + std::to_string(end.column) + ")" << std::endl;
#endif
    
    /* one line function eg.
     *  void foo(int n) {return n+1;}
    */
    if (start.line == end.line) {
        return 1;
    }

    // (default) stmt starts and ends on different lines
    return end.line - start.line - 1; // minus one to *exclude* both brackets
}



// ------------------------------------------------------------------------
// Definitions for FunctionDeclFrontendAction
// ------------------------------------------------------------------------
FunctionDeclFrontendAction::FunctionDeclFrontendAction(FunctionLengthMarker &marker, TestResult &result)
    : marker(marker), result(result)
{
    // nothing to see here
};

std::unique_ptr<clang::ASTConsumer> FunctionDeclFrontendAction::CreateASTConsumer(clang::CompilerInstance& CI, clang::StringRef file) {
    // pass CompilerInstance to ASTConsumer
    return std::make_unique<MyASTConsumer>(CI, marker, result); 
}



// ------------------------------------------------------------------------
// Definitions for FunctionLengthMarker
// ------------------------------------------------------------------------
FunctionLengthMarker::FunctionLengthMarker(TestConfig config) {
#ifdef DEBUG
    std::cout << "Creating FunctionLengthMarker" << std::endl;
#endif
    StdOptions::Initialise(config.Options);
    CompilerOptions::Initialise(config.Options);
    FuncLenMkrOptions::Initialise(config.Options);
}

TestResult FunctionLengthMarker::Mark(std::string file) {
    // create TestResult object to parse to clang classes etc.
    TestResult result;
    result.testName = "FunctionLengthMarker";
    result.testDescription = "Checks the length of function blocks.";

    // create and launch frontend action 
    std::vector<std::string> args;
    // args.push_back("-std=c99");
    args.push_back("-I/home/dylan/llvm-project/build/lib/clang/13.0.0/include");
    // args.push_back("-v");

    // read source file
    std::stringstream buf;
    buf << CreateInputStream(file)->rdbuf();
    std::string src = buf.str();

    bool actionStatus = clang::tooling::runToolOnCodeWithArgs(std::make_unique<FunctionDeclFrontendAction>(*this, result), llvm::Twine(src), args, file, "./stylemarker");

    // Create results object
    result.marksTotal = MarksAvailable;
    result.marksAwarded = MarksAvailable;
    return result;
}
