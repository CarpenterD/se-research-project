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
// Anonymous namespace to hide internal methods and classes
// ------------------------------------------------------------------------
namespace
{
    // helper functions
    SourceLocation ConvertSrcLoc(clang::SourceManager &sm, clang::SourceLocation sl) {
        SourceLocation loc(sm.getFilename(sl).str(), sm.getSpellingLineNumber(sl), sm.getSpellingColumnNumber(sl));
        return loc;
    };

    std::string FormatSourceLocation(SourceLocation loc) {
        return "(" + std::to_string(loc.line) + ":" + std::to_string(loc.column) + ")";
    }

    std::string FormatLocationRange(SourceLocation beg, SourceLocation end, bool collapse=false) {
        if (collapse) {
            std::string line = (beg.line==end.line)? std::to_string(beg.line) : std::to_string(beg.line) + "->" + std::to_string(end.line);
            std::string clmn = (beg.column==end.column)? std::to_string(beg.column) : std::to_string(beg.column) + "->" + std::to_string(end.column);
            return "("+line+":"+clmn+")";
        } else {
            return "(" + std::to_string(beg.line) + ":" + std::to_string(beg.column) + ") -> (" + std::to_string(end.line) + ":" + std::to_string(end.column) + ")";
        }
    }

    bool IsValidIndent(int width) {
        return (width > 0) && (width % 4) == 0;
    }
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
    IndentCheckOptions::Initialise(config.Options);
}

int IndentationCheck::GetIndentWidth() {
    return indentWidth;
}

void IndentationCheck::SetIndentWidth(int width) {
    if (width < 0) {
        throw std::invalid_argument("Unable to set indent width of " + std::to_string(width) + "; width can not be negative.");
    }
    indentWidth = width;
}

// Driver marking function called by the test-runner. Triggers Frontend action and hence internal marking function.

TestResult IndentationCheck::Mark(std::string file) {
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

    // populate results object
    result.testName = "IndentationCheck";
    result.testDescription = "Checks code indentation";
    result.marksTotal = MarksAvailable;

    // very primative mark awarding - this should be improved
    if ( result.feedback.size() <= 5) {
        result.marksAwarded = MarksAvailable;
    } else if ( result.feedback.size() <= 20 ) {
        result.marksAwarded = MarksAvailable * 0.5;
    } else {
        result.marksAwarded = 0;
    }

    return result;
}

// Hidden internal marking function. Stored result internally, for eventual retrieval and return my main driver function.

void IndentationCheck::CheckIndentation(clang::CompilerInstance& ci, std::vector<clang::CompoundStmt *> &stmts, std::vector<clang::VarDecl *> &vars, std::vector<clang::RecordDecl *> &records) {
    // capture ASTContext and SourceManager for ease of use
    clang::ASTContext &ctx = ci.getASTContext();
    clang::SourceManager &sm = ci.getSourceManager();

    int iWidth = GetIndentWidth(); // in place of a more power function e.g. GetModalIndentFromSource(clangNodes)
    clang::ParentMapContext &PMapCxt = ctx.getParentMapContext();

    // compound statements
    for (size_t i = 0; i < stmts.size(); i++)
    {
        // get parent
        clang::DynTypedNodeList parentList = PMapCxt.getParents(clang::DynTypedNode::create(*stmts[i]));
        const clang::DynTypedNode *parent = parentList.begin();
        // check stmt
        CheckIndentationCompoundStmt(sm, *stmts[i], parent);
    }

    // global variables
    for (size_t i = 0; i < vars.size(); i++)
    {
        CheckIndentationVarDecl(sm, *vars[i]);
    }

    // structs
    for (size_t i = 0; i < records.size(); i++)
    {
        CheckIndentationRecordDecl(sm, *records[i]);
    }
}

// Retreives the start location of the parent node as a reference for child indentation
SourceLocation IndentationCheck::GetParentStartLocation( clang::SourceManager &sm, const clang::DynTypedNode &parent ) {

    // parent location and type
    SourceLocation pBeg = ConvertSrcLoc(sm, parent.getSourceRange().getBegin());
    SourceLocation pEnd = ConvertSrcLoc(sm, parent.getSourceRange().getEnd());
    clang::ASTNodeKind pKind = parent.getNodeKind();

    SourceLocation indentStart = pBeg;

    std::stringstream logstream;
    #ifdef DEBUG
    logstream << "\t[PARENT] " << pKind.asStringRef().str() << " ";
    #endif

    if (pKind.isSame(clang::ASTNodeKind::getFromNodeKind<clang::FunctionDecl>()))
    {
        const clang::FunctionDecl *castParent = parent.get<clang::FunctionDecl>();
        logstream << "'" << castParent->getDeclName().getAsString() << "' at ";
    } 
    else if (pKind.isSame(clang::ASTNodeKind::getFromNodeKind<clang::ForStmt>()))
    {
        const clang::ForStmt *castParent = parent.get<clang::ForStmt>();
    }
    else if (pKind.isSame(clang::ASTNodeKind::getFromNodeKind<clang::WhileStmt>()))
    {
        const clang::WhileStmt *castParent = parent.get<clang::WhileStmt>();
    }
    else if (pKind.isSame(clang::ASTNodeKind::getFromNodeKind<clang::DoStmt>()))
    {
        const clang::DoStmt *castParent = parent.get<clang::DoStmt>();
    }
    else if (pKind.isSame(clang::ASTNodeKind::getFromNodeKind<clang::IfStmt>()))
    {
        const clang::IfStmt *castParent = parent.get<clang::IfStmt>();
    }
    else if (pKind.isSame(clang::ASTNodeKind::getFromNodeKind<clang::SwitchStmt>()))
    {
        // parent is 'switch' statement - SPECIAL CASE - contents of 'case' blocks may actually be children of switch
        const clang::SwitchStmt *castParent = parent.get<clang::SwitchStmt>();
    }
    else if (pKind.isSame(clang::ASTNodeKind::getFromNodeKind<clang::CompoundStmt>()))
    {
        // parent is compound statement -> child is almost certainly a 'floating' { ... } block
        const clang::CompoundStmt *castParent = parent.get<clang::CompoundStmt>();
    }
    else
    {
        throw std::invalid_argument( "Unrecognised parent type \"" + pKind.asStringRef().str() + "\" found at " + FormatLocationRange(pBeg, pEnd) + "." );
    }

    #ifdef DEBUG
    logstream << " at " << FormatLocationRange( pBeg, pEnd );
    std::clog << logstream.str() << std::endl;
    #endif

    return indentStart;
}

void IndentationCheck::CheckIndentationCompoundStmt( clang::SourceManager &sm, clang::CompoundStmt &stmt, const clang::DynTypedNode *parent ) {
    
    SourceLocation lb = ConvertSrcLoc(sm, stmt.getLBracLoc());
    SourceLocation rb = ConvertSrcLoc(sm, stmt.getRBracLoc());

    #ifdef DEBUG
    std::clog << "Found CompoundStmt " << FormatLocationRange(lb, rb) << " with "  << stmt.size() <<  " child statement(s)" << std::endl;
    #endif
    SourceLocation indentStart = lb;

    if (parent != NULL)
    {
        indentStart = GetParentStartLocation(sm, *parent);
    }

    #ifdef DEBUG
    std::clog << "\t[INDENT START] " << FormatSourceLocation(indentStart) << std::endl;
    #endif

    std::vector<SourceLocation> childLocs;
    
    // get children
    for (clang::Stmt::child_iterator iter = stmt.child_begin(), last = stmt.child_end(); iter != last; ++iter) {
        clang::Stmt *child = *iter;

        childLocs.push_back( ConvertSrcLoc(sm, child->getBeginLoc()) );
        SourceLocation cEnd = ConvertSrcLoc(sm, child->getEndLoc());

        #ifdef DEBUG
        std::clog << "\t" << child->getStmtClassName() << " " << FormatLocationRange( childLocs.back() , cEnd, true) << std::endl;
        #endif
    }
    CheckBlockIndentation(indentStart, lb, rb, childLocs);
}

void IndentationCheck::CheckIndentationVarDecl( clang::SourceManager &sm, clang::VarDecl &decl ) {
    SourceLocation beg = ConvertSrcLoc(sm, decl.getBeginLoc());
    SourceLocation end = ConvertSrcLoc(sm, decl.getEndLoc());

    #ifdef DEBUG
    std::clog << "Found VarDecl \"" << decl.getNameAsString() << "\" " << FormatLocationRange(beg, end, true) << std::endl;
    #endif

    if ( beg.column > 1 ) {
        result.feedback.push_back(FeedbackItem(SourceLocation(beg.file, beg.line, 1), beg, "Global variables should not be indented."));
    }
}

void IndentationCheck::CheckIndentationRecordDecl( clang::SourceManager &sm, clang::RecordDecl &decl ) {
    SourceLocation beg = ConvertSrcLoc(sm, decl.getBeginLoc());
    SourceLocation end = ConvertSrcLoc(sm, decl.getEndLoc());

    #ifdef DEBUG
    std::clog << "Found RecordDecl \"" << decl.getNameAsString() << "\" " << FormatLocationRange(beg, end) << std::endl;
    #endif

    if ( beg.column > 1 ) {
        result.feedback.push_back(FeedbackItem(SourceLocation(beg.file, beg.line, 1), beg, "Struct declarations should not be indented."));
    }

    for (clang::RecordDecl::field_iterator iter = decl.field_begin(), last = decl.field_end(); iter != last; ++iter) {
        clang::FieldDecl *child = *iter;

        SourceLocation fieldStart = ConvertSrcLoc(sm, child->getBeginLoc());

        #ifdef DEBUG
        std::clog << "\t" << child->getDeclKindName() << " " << child->getNameAsString() << " at " << FormatSourceLocation( fieldStart ) << std::endl;
        #endif

        if ( !IsValidIndent(fieldStart.column - beg.column) ) {
            result.feedback.push_back(FeedbackItem(fieldStart, "Struct declarations should not be indented."));
        }
    }
}

void IndentationCheck::CheckBlockIndentation(SourceLocation &kw, SourceLocation &lb, SourceLocation &rb, std::vector<SourceLocation> &clocs) {
    // check that children are indented from parent (if not inline)
    if ( lb.line != rb.line ) {
        for (size_t i = 0; i < clocs.size(); i++)
        {
            int width = clocs[i].column - kw.column;
            if (!IsValidIndent(width)) {
                result.feedback.push_back(FeedbackItem(clocs[i], "Child statements should indented from their parent by a multiple of 4 spaces"));
            } 
        }
    }


    // check lb and rb
    if ( lb.line == rb.line ) {
        // ensure there are only a max of 2 statements on line
        if (clocs.size() > IndentCheckOptions::MaxInlineStmts) {
            result.feedback.push_back(FeedbackItem(clocs[0], "Having multiple statements on a single line saves vertical space but makes it hard to distingush between distinct statements. Moving the statements on to separate lines would significantly increase readability."));
        }
    } else {
        if ( lb.line > kw.line + 1 ) {
            result.feedback.push_back(FeedbackItem(lb, SourceLocation(lb.file, lb.line+1, lb.column), "The opening curly brace should be in line with the parent statement or on the very next line."));
        }
        if ( lb.line == kw.line+1  && !(lb.column == kw.column || IsValidIndent(lb.column - kw.column)) ) {
            result.feedback.push_back(FeedbackItem(lb, SourceLocation(lb.file, lb.line+1, lb.column), "The opening curly brace should be either directly below its parent statement, or below and indented by one level."));
        }
        if ( rb.line != kw.line  && rb.column != kw.column ) {
            result.feedback.push_back(FeedbackItem(rb, SourceLocation(rb.file, rb.line+1, rb.column), "The closing curly brace should line up vertically with the start of the parent keyword e.g. the 'f' in \"for\" or the 'i' in \"if\"."));
        }
    }

    // check that the first and last statements are not on the same line as the brackets
    if ( clocs.size() > 0 && clocs.size() > IndentCheckOptions::MaxInlineStmts && lb.line != rb.line) {
        if (lb.line == clocs[0].line) {
            result.feedback.push_back(FeedbackItem(clocs[0], "To increase clarity and  indentation consistency, statements should not be put on the same line as the opening curly brace."));
        }
        if (rb.line == clocs.back().line) {
            result.feedback.push_back(FeedbackItem(clocs.back(), "To increase clarity and  indentation consistency, statements should not be put on the same line as the closing curly brace."));
        }
    }
    
    //check for sibling statements on the same line (where not all inline)
    if ( !IndentCheckOptions::AllowStmtsOnSameLine && clocs.size() > 1 && lb.line != rb.line ){
        for (size_t i = 0; i < clocs.size()-1; i++)
        {
            if ( clocs[i].line == clocs[i+1].line){
                result.feedback.push_back(FeedbackItem(clocs[i+1], "This statement is on the same line as the one before it. To increase clarity, make sure that every statement is on its own line."));
            }
        }
    }
}