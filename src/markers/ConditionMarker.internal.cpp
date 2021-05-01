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
// Definitions for ASTVisitor
// ------------------------------------------------------------------------

ConditionMarker::ASTVisitor::ASTVisitor(clang::CompilerInstance& ci, ConditionMarker &marker)
    : ci(ci), ctx(ci.getASTContext()), sm(ci.getSourceManager()), marker(marker)
{
    // empty constructor
}

// (override) recursive AST visiting function 
bool ConditionMarker::ASTVisitor::VisitIfStmt(clang::IfStmt* stmt)
{
    if (stmt != NULL && sm.isWrittenInMainFile(stmt->getSourceRange().getBegin()))
    {
        marker.HandleIfStmt(ci, *stmt);
    }
    return true;
}

// (override) recursive AST visiting function 
bool ConditionMarker::ASTVisitor::VisitBinaryOperator(clang::BinaryOperator* op)
{
    if (op != NULL && sm.isWrittenInMainFile(op->getSourceRange().getBegin()))
    {
        marker.HandleBinaryOperator(ci, *op);
    }
    return true;
}



// ------------------------------------------------------------------------
// Definitions for internal AST Consumer
// ------------------------------------------------------------------------

ConditionMarker::ASTConsumer::ASTConsumer(clang::CompilerInstance& instance, ConditionMarker &marker)
    : ci(instance), visitor(instance, marker), marker(marker)
{
    
}

void ConditionMarker::ASTConsumer::HandleTranslationUnit(clang::ASTContext& astContext) {
    visitor.TraverseDecl(astContext.getTranslationUnitDecl());
}



// ------------------------------------------------------------------------
// Definitions for internal FrontendAction
// ------------------------------------------------------------------------

ConditionMarker::FrontendAction::FrontendAction(ConditionMarker &marker)
    : marker(marker)
{
    // nothing to see here
};

std::unique_ptr<clang::ASTConsumer> ConditionMarker::FrontendAction::CreateASTConsumer(clang::CompilerInstance& CI, clang::StringRef file) {
    // pass CompilerInstance to ASTConsumer
    return std::make_unique<ASTConsumer>(CI, marker); 
}
