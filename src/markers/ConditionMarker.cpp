// StdLib deps
#include <iostream>
#include <sstream>


// Internal deps
#include "ConditionMarker.hpp"
#include "utils/FileIO.hpp"

// Clang deps
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/AST/ParentMapContext.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/Tooling.h"



// ------------------------------------------------------------------------
// Anonymous namespace to hide internal methods and classes
// ------------------------------------------------------------------------
namespace
{
    SourceLocation ConvertSrcLoc(clang::SourceManager &sm, clang::SourceLocation sl) {
        SourceLocation loc(sm.getFilename(sl).str(), sm.getSpellingLineNumber(sl), sm.getSpellingColumnNumber(sl));
        return loc;
    };

    std::string getLocationString(clang::SourceLocation loc, clang::SourceManager &sourceManager){
        std::ostringstream stream;
        stream << sourceManager.getFilename(loc).str() << ":" << sourceManager.getSpellingLineNumber(loc) << ":" << sourceManager.getSpellingColumnNumber(loc);
        return stream.str();
    }

    std::string boolToStr(bool val){
        return val? "true":"false";
    }

    int APIntToBool(llvm::APInt num){
        if (num == 0) {
            return 0;
        } else if (num == 1) {
            return 1;
        } else {
            return -1;
        }
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
    result.marksTotal = StdOptions::MarksAvailable;

    int feedbackCount = 0;
    for (size_t i = 0; i < result.feedback.size(); i++)
    {
        if (result.feedback[i].severity > FeedbackSeverity::NEUTRAL){
            feedbackCount++;
        }
    }

    result.marksAwarded = std::max(0, StdOptions::MarksAvailable - (feedbackCount / 3));
    result.testInfo = "Only checking for unnecessary 'if' statements.";

    if (result.marksAwarded == result.marksTotal) {
        result.remarks = "Well done - all conditions were of good quality.";
    } else {
        result.remarks = "Additional work is required to ensure all conditionals are expressive and concise.";
    }
    return result;
}

// Handle IF checking

void ConditionMarker::HandleIfStmt(clang::CompilerInstance &ci, clang::IfStmt &stmt) {
    clang::SourceManager &sm = ci.getSourceManager();
    clang::ASTContext &ctx = ci.getASTContext();

    #ifdef DEBUG
    std::cout << "[CALLED ON IF STATEMENT @ " << getLocationString(stmt.getSourceRange().getBegin(), sm) << "]" << std::endl;
    #endif

    clang::Stmt *elseStmt = stmt.getElse();
    clang::ReturnStmt *ifReturn = NULL;
    clang::ReturnStmt *elReturn = NULL;

    // filter out else ifs and ifs with no else (unless else child is a return statement)
    if ( elseStmt == NULL ) {
        #ifdef DEBUG
        std::cout << "\tNo else -> exiting" << std::endl;
        #endif
        return;
    } else if ( elseStmt->getStmtClass() != clang::Stmt::StmtClass::CompoundStmtClass && elseStmt->getStmtClass() != clang::Stmt::StmtClass::ReturnStmtClass) {
        #ifdef DEBUG
        std::cout << "\tIrrelevant else type: " << elseStmt->getStmtClassName() << " -> exiting" << std::endl;
        #endif
        return;
    }

    // stop if node is the else if
    if ( ParentIsIfStmt(ctx, stmt) ){
        #ifdef DEBUG
        std::cout << "\tI am the 'else if' -> exiting" << std::endl;
        #endif
        return;
    }

    // get children
    int childCount = 0;
    bool triedResolvingIf = false;

    for (clang::Stmt::child_iterator beg = stmt.child_begin(), end = stmt.child_end(); beg != end; ++beg) {
        clang::Stmt *child = *beg;
        if ( child != NULL) {
            // std::cout << "\t\tchild of type: " << child->getStmtClassName() << " @ " << getLocationString(child->getSourceRange().getBegin(), sm) << std::endl;
            childCount++;

            if ( child->getStmtClass() != clang::Stmt::StmtClass::CompoundStmtClass && child->getStmtClass() != clang::Stmt::StmtClass::ReturnStmtClass ){
                continue;
            }

            if ( ifReturn == NULL && !triedResolvingIf){
                triedResolvingIf = true;
                ifReturn = ResolveStmtToSingleReturn(ctx, *child);
            } else if ( elReturn == NULL ) {
                elReturn = ResolveStmtToSingleReturn(ctx, *child);
            }
        }
    }
    if ( ifReturn == NULL || elReturn == NULL ) {
        #ifdef DEBUG
        std::cout << "\tif or else return statements are null (if null: " << boolToStr(ifReturn == NULL) << ", else null: " << boolToStr(elReturn == NULL) << ") -> exiting" << std::endl;
        #endif
        return;
    } else {
        #ifdef DEBUG
        std::cout << "\tif   return @ " << getLocationString(ifReturn->getSourceRange().getBegin(), sm) << std::endl;
        std::cout << "\telse return @ " << getLocationString(elReturn->getSourceRange().getBegin(), sm) << std::endl;
        #endif
    }

    clang::IntegerLiteral *ifLiteral = GetLiteralReturnValue(ctx, *ifReturn);
    clang::IntegerLiteral *elLiteral = GetLiteralReturnValue(ctx, *elReturn);
    if ( ifLiteral == NULL || elLiteral == NULL ) {
        #ifdef DEBUG
        std::cout << "\treturn statements not integer literals (if not: " << boolToStr(ifLiteral == NULL) << ", else not: " << boolToStr(elLiteral == NULL) << ") -> exiting" << std::endl;
        #endif
        return;
    }

    // get integer values
    int ifReturnValue = APIntToBool(ifLiteral->getValue());
    int elReturnValue = APIntToBool(elLiteral->getValue());
    
    if (ifReturnValue == -1 || elReturnValue == -1) {
        #ifdef DEBUG
        std::cout << "\treturn statements not Booleans (if: " << ifReturnValue << ", else: " << elReturnValue << ") -> exiting" << std::endl;
        #endif
        return;
    }

    #ifdef DEBUG
    std::cout << "\tif (condition) return " << boolToStr( ifReturnValue ) << " else return " << boolToStr( elReturnValue ) << std::endl;
    #endif

    // create feedback
    SourceLocation ifBegLoc = ConvertSrcLoc(sm, stmt.getSourceRange().getBegin());
    SourceLocation ifEndLoc = ConvertSrcLoc(sm, stmt.getSourceRange().getEnd());
    std::string feedback = "Unnecessary 'if' statement. Code could be made more concise and expressive by returning the result of the condition directly rather than manually returning 'true' and 'false'.";

    if ( !ifReturnValue && elReturnValue ) {
        feedback += " Note that the condition may need to be negated in order to return the correct value.";
    }

    result.feedback.push_back( FeedbackItem(ifBegLoc, ifEndLoc, feedback, FeedbackSeverity::MILD) );
}

bool ConditionMarker::ParentIsIfStmt(clang::ASTContext &ctx, clang::IfStmt &stmt) {
    
    clang::ParentMapContext &PMapCxt = ctx.getParentMapContext();
    clang::DynTypedNodeList parentList = PMapCxt.getParents(clang::DynTypedNode::create(stmt));
    const clang::DynTypedNode *parent = parentList.begin();

    if (parent == NULL) {
        return false;
    } else {
        return parent->getNodeKind().isSame(clang::ASTNodeKind::getFromNodeKind<clang::IfStmt>());
    }
}

clang::ReturnStmt * ConditionMarker::ResolveStmtToSingleReturn(clang::ASTContext &ctx, clang::Stmt &stmt){
    if ( stmt.getStmtClass() == clang::Stmt::StmtClass::ReturnStmtClass ) {
        return (clang::ReturnStmt *)&stmt;
    }

    if ( stmt.getStmtClass() != clang::Stmt::StmtClass::CompoundStmtClass ) {
        return NULL;
    }

    int count = 0;
    clang::ReturnStmt *retStmt = NULL;

    for (clang::Stmt::child_iterator beg = stmt.child_begin(), end = stmt.child_end(); beg != end; ++beg) {
        clang::Stmt *child = *beg;
        if ( child != NULL) {
            count++;
            if ( child->getStmtClass() == clang::Stmt::StmtClass::ReturnStmtClass ) {
                retStmt = (clang::ReturnStmt *) child;
            }
        }
    }

    if ( count > 1) {
        return NULL;
    }
    return retStmt; // could still be NULL if one was not found
}

clang::IntegerLiteral * ConditionMarker::GetLiteralReturnValue(clang::ASTContext &ctx, clang::ReturnStmt &retStmt){
    clang::Expr *expression = retStmt.getRetValue()->IgnoreUnlessSpelledInSource();

    if (expression->getStmtClass() == clang::Stmt::StmtClass::IntegerLiteralClass ) {
        return (clang::IntegerLiteral *)expression;
    } else {
        return NULL;
    }
}

// Handle CONDITION checking

void ConditionMarker::HandleBinaryOperator(clang::CompilerInstance &ci, clang::BinaryOperator &op) {
    clang::SourceManager &sm = ci.getSourceManager();

    #ifdef DEBUG
    std::cout << "[CALLED ON BINARY OPERATOR @ " << getLocationString(op.getSourceRange().getBegin(), sm) << "]" << std::endl;
    #endif
}
