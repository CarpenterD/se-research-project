// StdLib deps
#include <iostream>
#include <sstream>

// Internal deps
#include "IndentationCheck.hpp"
#include "utils/FileIO.hpp"

// Clang deps
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/AST/ParentMapContext.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/CommandLine.h"


// ------------------------------------------------------------------------
// Definitions for Visitor
// ------------------------------------------------------------------------

IndentationCheck::Visitor::Visitor(clang::SourceManager& sm)
    : sm(sm)
{
    // empty constructor
}

// (override) recursive AST visiting function 
bool IndentationCheck::Visitor::VisitCompoundStmt(clang::CompoundStmt* stmt)
{
    if (stmt != NULL && sm.isWrittenInMainFile(stmt->getSourceRange().getBegin()))
    {
        stmts.push_back(stmt);
    }
    return true;
}

// (override) recursive AST visiting function 
bool IndentationCheck::Visitor::VisitVarDecl(clang::VarDecl* decl)
{
    if (decl != NULL && sm.isWrittenInMainFile(decl->getSourceRange().getBegin()) && decl->hasGlobalStorage() && !decl->isStaticLocal())
    {
        vars.push_back(decl);
    }
    return true;
}

// (override) recursive AST visiting function 
bool IndentationCheck::Visitor::VisitRecordDecl(clang::RecordDecl *decl)
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

IndentationCheck::ICASTConsumer::ICASTConsumer(clang::CompilerInstance& instance, IndentationCheck &marker)
    : ci(instance), ctx(ci.getASTContext()), sm(ci.getSourceManager()), visitor(sm), marker(marker)
{
    // empty constructor
}

void IndentationCheck::ICASTConsumer::HandleTranslationUnit(clang::ASTContext& astContext) {
    // find function definitions and process one by one
    visitor.TraverseDecl(astContext.getTranslationUnitDecl());

    marker.CheckIndentation(ci, visitor.stmts, visitor.vars, visitor.records);
}



// ------------------------------------------------------------------------
// Definitions for ICFrontendAction
// ------------------------------------------------------------------------

IndentationCheck::ICFrontendAction::ICFrontendAction(IndentationCheck &marker)
    : marker(marker)
{
    // nothing to see here
};

std::unique_ptr<clang::ASTConsumer> IndentationCheck::ICFrontendAction::CreateASTConsumer(clang::CompilerInstance& CI, clang::StringRef file) {
    // pass CompilerInstance to ASTConsumer
    return std::make_unique<ICASTConsumer>(CI, marker); 
}

