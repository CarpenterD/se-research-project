// StdLib deps
#include <iostream>
#include <sstream>

// Internal deps
#include "IdentifierMarker.hpp"
#include "utils/FileIO.hpp"

// Clang deps
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/Tooling.h"


// ------------------------------------------------------------------------
// Definitions for ASTVisitor
// ------------------------------------------------------------------------

IdentifierMarker::ASTVisitor::ASTVisitor(clang::CompilerInstance& ci)
    : ctx(ci.getASTContext()), sm(ci.getSourceManager())
{
    // empty constructor
}

// (override) recursive AST visiting function 
bool IdentifierMarker::ASTVisitor::VisitNamedDecl(clang::NamedDecl* decl)
{
    if (decl != NULL && sm.isWrittenInMainFile(decl->getSourceRange().getBegin()))
    {
        decls.push_back(decl);
    }
    return true;
}



// ------------------------------------------------------------------------
// Definitions for internal AST Consumer
// ------------------------------------------------------------------------

IdentifierMarker::ASTConsumer::ASTConsumer(clang::CompilerInstance& instance, IdentifierMarker &marker)
    : ci(instance), visitor(instance), marker(marker)
{
    
}

void IdentifierMarker::ASTConsumer::HandleTranslationUnit(clang::ASTContext& astContext) {
    visitor.TraverseDecl(astContext.getTranslationUnitDecl());
    marker.ProcessNamedDecls(ci, visitor.decls);
}



// ------------------------------------------------------------------------
// Definitions for internal FrontendAction
// ------------------------------------------------------------------------

IdentifierMarker::FrontendAction::FrontendAction(IdentifierMarker &marker)
    : marker(marker)
{
    // nothing to see here
};

std::unique_ptr<clang::ASTConsumer> IdentifierMarker::FrontendAction::CreateASTConsumer(clang::CompilerInstance& CI, clang::StringRef file) {
    // pass CompilerInstance to ASTConsumer
    return std::make_unique<ASTConsumer>(CI, marker); 
}



// ------------------------------------------------------------------------
// Definitions for Identifier
// ------------------------------------------------------------------------
IdentifierMarker::Identifier::Identifier(clang::NamedDecl &namedDecl)
: decl(namedDecl)
{
    name = decl.getNameAsString();
    style = GetIdentifierStyle(name);
}
