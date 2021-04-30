// StdLib deps
#include <iostream>
#include <sstream>

// Internal deps
#include "ConditionMarker.hpp"
#include "utils/FileIO.hpp"

// Clang deps
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Lex/Lexer.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Refactoring.h"
#include "clang/Tooling/RefactoringCallbacks.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/CommandLine.h"

using namespace clang::ast_matchers;

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
// Definitions for internal AST Consumer
// ------------------------------------------------------------------------
ConditionMarker::ASTConsumer::ASTConsumer(clang::CompilerInstance& instance, ConditionMarker &marker)
    : ci(instance), ctx(ci.getASTContext()), sm(ci.getSourceManager()), marker(marker)
{
    // Initialise Matchers
	// Matches Boolean equality comparisons with a Boolean literal 
	StatementMatcher boolEqualityMatcher = binaryOperator(
		anyOf(
			hasOperatorName("=="),
			hasOperatorName("!=")
		), 
		hasEitherOperand(
			has(
				ignoringParenImpCasts(
					cxxBoolLiteral().bind("literal")
				)
			)
		)
	).bind("boolEq");

	// Matches Boolean logic with a Boolean literal
	StatementMatcher boolLogicMatcher = binaryOperator(
		anyOf(
			hasOperatorName("&&"),
			hasOperatorName("||")
		), 
		hasEitherOperand(
			cxxBoolLiteral().bind("literal")
		)
	).bind("boolLogic");

	ConditionMarker::BoolEqualityCallback equalityCallback(ctx, marker.result);
	ConditionMarker::BoolLogicCallback logicCallback(ctx, marker.result);

	finder.addMatcher(boolEqualityMatcher, &equalityCallback);
	finder.addMatcher(boolLogicMatcher, &logicCallback);
}

void ConditionMarker::ASTConsumer::HandleTranslationUnit(clang::ASTContext& astContext) {
    // find function definitions and process one by one
    finder.matchAST(astContext);
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
