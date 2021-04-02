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
    class Visitor: public clang::RecursiveASTVisitor<Visitor>
    {
        public:
            std::vector<clang::CompoundStmt *> stmts;
            std::vector<clang::VarDecl *> vars;
            std::vector<clang::RecordDecl *> records;
            
            explicit Visitor(clang::SourceManager& sm);
            bool VisitCompoundStmt(clang::CompoundStmt* stmt);
            bool VisitVarDecl(clang::VarDecl* decl);
            bool VisitRecordDecl(clang::RecordDecl* decl);
            
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
            Visitor visitor;

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

    std::string FormatLocationRange(SourceLocation beg, SourceLocation end, bool collapse=false) {
        if (collapse) {
            std::string line = (beg.line==end.line)? std::to_string(beg.line) : std::to_string(beg.line) + "->" + std::to_string(end.line);
            std::string clmn = (beg.column==end.column)? std::to_string(beg.column) : std::to_string(beg.column) + "->" + std::to_string(end.column);
            return "("+line+":"+clmn+")";
        } else {
            return "(" + std::to_string(beg.line) + ":" + std::to_string(beg.column) + ") -> (" + std::to_string(end.line) + ":" + std::to_string(end.column) + ")";
        }
    }
}


// ------------------------------------------------------------------------
// Definitions for Visitor
// ------------------------------------------------------------------------
// constructor
Visitor::Visitor(clang::SourceManager& sm)
    : sm(sm)
{
    // empty constructor
}

// (override) recursive AST visiting function 
bool Visitor::VisitCompoundStmt(clang::CompoundStmt* stmt)
{
    if (stmt != NULL && sm.isWrittenInMainFile(stmt->getSourceRange().getBegin()))
    {
        stmts.push_back(stmt);
    }
    return true;
}

// (override) recursive AST visiting function 
bool Visitor::VisitVarDecl(clang::VarDecl* decl)
{
    if (decl != NULL && sm.isWrittenInMainFile(decl->getSourceRange().getBegin()) && decl->hasGlobalStorage() && !decl->isStaticLocal())
    {
        vars.push_back(decl);
    }
    return true;
}

// (override) recursive AST visiting function 
bool Visitor::VisitRecordDecl(clang::RecordDecl *decl)
{
    if (decl != NULL && sm.isWrittenInMainFile(decl->getSourceRange().getBegin()))
    {
        records.push_back(decl);
    }
    return true;
}



// ------------------------------------------------------------------------
// Definitions for MyASTConsumer
// ------------------------------------------------------------------------
ICASTConsumer::ICASTConsumer(clang::CompilerInstance& ci, IndentationCheck &marker)
    : ctx(ci.getASTContext()), sm(ci.getSourceManager()), visitor(sm), marker(marker)
{
    // empty constructor
}

void ICASTConsumer::HandleTranslationUnit(clang::ASTContext& astContext) {
    // find function definitions and process one by one
    visitor.TraverseDecl(astContext.getTranslationUnitDecl());

    // compound statements
    for (size_t i = 0; i < visitor.stmts.size(); i++)
    {
        clang::CompoundStmt &stmt = *visitor.stmts[i];

#ifdef DEBUG
        SourceLocation lb = ConvertSrcLoc(sm, stmt.getLBracLoc());
        SourceLocation rb = ConvertSrcLoc(sm, stmt.getRBracLoc());
        std::clog << "Found CompoundStmt " << FormatLocationRange(lb, rb) << " with "  << stmt.size() <<  " child statement(s)" << std::endl;
#endif

        for (clang::Stmt::child_iterator iter = stmt.child_begin(), last = stmt.child_end(); iter != last; ++iter) {
            clang::Stmt *child = *iter;

            SourceLocation cBeg = ConvertSrcLoc(sm, child->getBeginLoc());
            SourceLocation cEnd = ConvertSrcLoc(sm, child->getEndLoc());

            std::clog << "\t" << child->getStmtClassName() << " " << FormatLocationRange(cBeg, cEnd, true) << std::endl;
        }
    }

    // global variables
    for (size_t i = 0; i < visitor.vars.size(); i++)
    {
        clang::VarDecl &decl = *visitor.vars[i];
#ifdef DEBUG
        SourceLocation beg = ConvertSrcLoc(sm, decl.getBeginLoc());
        SourceLocation end = ConvertSrcLoc(sm, decl.getEndLoc());

        SourceLocation tBeg = ConvertSrcLoc(sm, decl.getTypeSpecStartLoc());
        SourceLocation tEnd = ConvertSrcLoc(sm, decl.getTypeSpecEndLoc());

        std::clog << "Found VarDecl \"" << decl.getNameAsString() << "\" " << FormatLocationRange(beg, end, true) << std::endl;

        decl.dump();
#endif
    }

    // structs
    for (size_t i = 0; i < visitor.records.size(); i++)
    {
        clang::RecordDecl &record = *visitor.records[i];
#ifdef DEBUG
        SourceLocation beg = ConvertSrcLoc(sm, record.getBeginLoc());
        SourceLocation end = ConvertSrcLoc(sm, record.getEndLoc());
        std::clog << "Found RecordDecl \"" << record.getNameAsString() << "\" " << FormatLocationRange(beg, end) << std::endl;
#endif
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
