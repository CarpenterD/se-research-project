// StdLib deps
#include <iostream>
#include <sstream>

// Internal deps
#include "IndentationCheck.hpp"
#include "utils/FileIO.hpp"

// Clang deps
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/AST/ASTTypeTraits.h"
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
    class CompoundStmtVisitor: public clang::RecursiveASTVisitor<CompoundStmtVisitor>
    {
        public:
            std::vector<clang::CompoundStmt *> stmts;
            
            explicit CompoundStmtVisitor(clang::SourceManager& sm);
            bool VisitCompoundStmt(clang::CompoundStmt* stmt);
            
        private:
            clang::SourceManager& sm;
            
    };


    class ICASTConsumer: public clang::ASTConsumer
    {
        public:
            explicit ICASTConsumer(clang::CompilerInstance& ci, IndentationCheck &marker);
            virtual void HandleTranslationUnit(clang::ASTContext& astContext);

        private:
            clang::ASTContext &ctx;
            clang::SourceManager &sm;
            CompoundStmtVisitor compoundStmtVisitor;

            IndentationCheck &marker;
            
    };


    // FrontendAction used for IndentationCheck 
    class ICFrontendAction: public clang::ASTFrontendAction
    {
        public:
            ICFrontendAction(IndentationCheck &marker);
            virtual std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance& CI, clang::StringRef file);

        private:
            IndentationCheck &marker;
    };


    // helper functions
    SourceLocation ConvertSrcLoc(clang::SourceManager &sm, clang::SourceLocation sl) {
        SourceLocation loc(sm.getFilename(sl).str(), sm.getSpellingLineNumber(sl), sm.getSpellingColumnNumber(sl));
        return loc;
    };
}


// ------------------------------------------------------------------------
// Definitions for CompoundStmtVisitor
// ------------------------------------------------------------------------
// constructor
CompoundStmtVisitor::CompoundStmtVisitor(clang::SourceManager& sm)
    : sm(sm)
{
    // empty constructor
}

// (override) recursive AST visiting function 
bool CompoundStmtVisitor::VisitCompoundStmt(clang::CompoundStmt* stmt)
{
    if (stmt != NULL && sm.isWrittenInMainFile(stmt->getSourceRange().getBegin()))
    {
#ifdef DEBUG
        SourceLocation lb = ConvertSrcLoc(sm, stmt->getLBracLoc());
        SourceLocation rb = ConvertSrcLoc(sm, stmt->getRBracLoc());
        std::clog << "Found CompoundStmt from (" << lb.line << ":" << lb.column << ") to (" << rb.line << ":" << rb.column << ")" << std::endl;
#endif
        stmts.push_back(stmt);
    }
    return true;
}



// ------------------------------------------------------------------------
// Definitions for MyASTConsumer
// ------------------------------------------------------------------------
ICASTConsumer::ICASTConsumer(clang::CompilerInstance& ci, IndentationCheck &marker)
    : ctx(ci.getASTContext()), sm(ci.getSourceManager()), compoundStmtVisitor(sm), marker(marker)
{
    // empty constructor
}

void ICASTConsumer::HandleTranslationUnit(clang::ASTContext& astContext) {
    // find function definitions and process one by one
    compoundStmtVisitor.TraverseDecl(astContext.getTranslationUnitDecl());

    for (size_t i = 0; i < compoundStmtVisitor.stmts.size(); i++)
    {
        clang::CompoundStmt &stmt = *compoundStmtVisitor.stmts[i];
    }
    
}



// ------------------------------------------------------------------------
// Definitions for ICFrontendAction
// ------------------------------------------------------------------------
ICFrontendAction::ICFrontendAction(IndentationCheck &marker)
    : marker(marker)
{
    // nothing to see here
};

std::unique_ptr<clang::ASTConsumer> ICFrontendAction::CreateASTConsumer(clang::CompilerInstance& CI, clang::StringRef file) {
    // pass CompilerInstance to ASTConsumer
    return std::make_unique<ICASTConsumer>(CI, marker); 
}



// ------------------------------------------------------------------------
// Definitions for IndentationCheck
// ------------------------------------------------------------------------
IndentationCheck::IndentationCheck(TestConfig config) {
#ifdef DEBUG
    std::cout << "Creating IndentationCheck" << std::endl;
#endif
    StdOptions::Initialise(config.Options);
    CompilerOptions::Initialise(config.Options);
}

TestResult IndentationCheck::Mark(std::string file) {
    // create TestResult object to parse to clang classes etc.
    TestResult result;
    result.testName = "IndentationCheck";
    result.testDescription = "Checks code indentation";

    // create and launch frontend action 
    std::vector<std::string> args;
    // args.push_back("-std=c99");
    args.push_back("-I/home/dylan/llvm-project/build/lib/clang/13.0.0/include");
    // args.push_back("-v");

    // read source file
    std::stringstream buf;
    buf << CreateInputStream(file)->rdbuf();
    std::string src = buf.str();

    bool actionStatus = clang::tooling::runToolOnCodeWithArgs(std::make_unique<ICFrontendAction>(*this), llvm::Twine(src), args, file, "./stylemarker");

    // Create results object
    result.marksTotal = MarksAvailable;
    result.marksAwarded = MarksAvailable;
    return result;
}
