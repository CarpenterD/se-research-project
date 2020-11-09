#include <iostream>
#include <sstream>

#include "clang/AST/RecursiveASTVisitor.h"
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
using namespace clang::tooling;

//globals
int filepathPrefixLength;

std::string getLocationString(clang::SourceLocation loc, clang::SourceManager* sourceManager){
	std::ostringstream stream;
	stream << sourceManager->getFilename(loc).str() << ":" << sourceManager->getSpellingLineNumber(loc) << ":" << sourceManager->getSpellingColumnNumber(loc);
	return stream.str();
}

std::string boolToStr(bool val){
	return val? "true":"false";
}


class BoolEqualityCallback : public MatchFinder::MatchCallback
{
	clang::Rewriter rewriter_;

public :
	virtual void run(const MatchFinder::MatchResult &result)
	{
		clang::SourceManager* sourceManager = result.SourceManager;
		const clang::LangOptions& langOptions = result.Context->getLangOpts();

		//get AST nodes
		const clang::BinaryOperator* binaryOperator = result.Nodes.getNodeAs<clang::BinaryOperator>("boolEq");
		const clang::CXXBoolLiteralExpr* literal = result.Nodes.getNodeAs<clang::CXXBoolLiteralExpr>("literal");

		//get string representation and location
		clang::CharSourceRange binOpRange = clang::CharSourceRange::getTokenRange(binaryOperator->getSourceRange());
		std::string binOpString = clang::Lexer::getSourceText(binOpRange, *sourceManager, langOptions).str();
		std::cout << "At " << getLocationString(binOpRange.getBegin(), sourceManager) << ": Simplify Boolean comparison" << std::endl;

		//evaluate if of poor quality
		std::string operand = binaryOperator->getOpcodeStr().str();
		bool literalValue = literal->getValue();

		std::cout << "\tThe Boolean comparison '" << binOpString << "' could be simplified to remove the hard-coded '" << boolToStr(literalValue) << "'" << std::endl;
		std::cout << "\tConsider the value of the other side of the '" << operand << "' when the computer evaluates the condition. ";
		if ((operand=="==" && literalValue) || (operand=="!=" && !literalValue) ){
			std::cout << "How does it compare to the result you want?" << std::endl; //the values are the same
		}else{
			std::cout << "How would you modify it into the result you are expecting?" << std::endl; //the value needs to be negated
		}

		// binaryOperator->dumpColor();
		std::cout << std::endl;
	}
};

class BoolLogicCallback : public MatchFinder::MatchCallback
{
	clang::Rewriter rewriter_;

public :
	virtual void run(const MatchFinder::MatchResult &result)
	{
		clang::SourceManager* sourceManager = result.SourceManager;
		const clang::LangOptions& langOptions = result.Context->getLangOpts();

		//get AST nodes
		const clang::BinaryOperator* binaryOperator = result.Nodes.getNodeAs<clang::BinaryOperator>("boolLogic");
		const clang::CXXBoolLiteralExpr* literal = result.Nodes.getNodeAs<clang::CXXBoolLiteralExpr>("literal");

		//get string representation and location
		clang::CharSourceRange binOpRange = clang::CharSourceRange::getTokenRange(binaryOperator->getSourceRange());
		std::string binOpString = clang::Lexer::getSourceText(binOpRange, *sourceManager, langOptions).str();

		//report problem
		std::string operand = binaryOperator->getOpcodeStr().str();
		bool literalValue = literal->getValue();

		std::cout << "At " << getLocationString(binOpRange.getBegin(), sourceManager) << ": Simplify Boolean logic" << std::endl;
		std::cout << "\tThe Boolean logic expression '" << binOpString << "' could be simplified." << std::endl;
		std::cout << "\tConsider the effect a constant value of '" << boolToStr(literalValue) << "' will have on a logical '" << (operand == "&&"? "AND":"OR") << "'. A truth table may be useful for this." << std::endl;

		// binaryOperator->dumpColor();
		std::cout << std::endl;
	}
};


int main(int argc, const char **argv)
{
	llvm::cl::OptionCategory MyToolCategory("replace_for_pattern options");
	CommonOptionsParser OptionsParser(argc, argv, MyToolCategory);
	ClangTool Tool(OptionsParser.getCompilations(), OptionsParser.getSourcePathList());	

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

	// Matches Boolean logic with a Boolean literal - TODO detect and bind logic using 2 literals
	StatementMatcher boolLogicMatcher = binaryOperator(
		anyOf(
			hasOperatorName("&&"),
			hasOperatorName("||")
		), 
		hasEitherOperand(
			cxxBoolLiteral().bind("literal")
		)
	).bind("boolLogic");

	// IfStmtCallback ifCallback;
	BoolEqualityCallback equalityCallback;
	BoolLogicCallback logicCallback;

	MatchFinder finder;
	finder.addMatcher(boolEqualityMatcher, &equalityCallback);
	finder.addMatcher(boolLogicMatcher, &logicCallback);

	return Tool.run(newFrontendActionFactory(&finder).get());
}

