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
        clang::CompoundStmt &stmt = *stmts[i];
#ifdef DEBUG
        SourceLocation lb = ConvertSrcLoc(sm, stmt.getLBracLoc());
        SourceLocation rb = ConvertSrcLoc(sm, stmt.getRBracLoc());
        std::clog << "Found CompoundStmt " << FormatLocationRange(lb, rb) << " with "  << stmt.size() <<  " child statement(s)" << std::endl;
#endif
        SourceLocation indentStart = lb;

        // get parent
        clang::DynTypedNodeList parentList = PMapCxt.getParents(clang::DynTypedNode::create(stmt));
        const clang::DynTypedNode *parent = parentList.begin();

        if (parent != NULL)
        {
            clang::ASTNodeKind parentKind = parent->getNodeKind();
            std::stringstream logstream;
            logstream << "\t[PARENT] " << parentKind.asStringRef().str() << " ";

            if (parentKind.isSame(clang::ASTNodeKind::getFromNodeKind<clang::FunctionDecl>()))
            {
                // parent is function
                const clang::FunctionDecl *castParent = parent->get<clang::FunctionDecl>();
                indentStart = ConvertSrcLoc(sm, castParent->getBeginLoc());
                logstream << "'" << castParent->getDeclName().getAsString() << "' at ";
                logstream << FormatLocationRange(ConvertSrcLoc(sm, castParent->getBeginLoc()), ConvertSrcLoc(sm, castParent->getEndLoc()));
            } 
            else if (parentKind.isSame(clang::ASTNodeKind::getFromNodeKind<clang::ForStmt>()))
            {
                // parent is 'for' loop
                const clang::ForStmt *castParent = parent->get<clang::ForStmt>();
                indentStart = ConvertSrcLoc(sm, castParent->getBeginLoc());
                logstream << " at " << FormatLocationRange(ConvertSrcLoc(sm, castParent->getBeginLoc()), ConvertSrcLoc(sm, castParent->getEndLoc()));
            }
            else if (parentKind.isSame(clang::ASTNodeKind::getFromNodeKind<clang::WhileStmt>()))
            {
                // parent is 'while' loop
                const clang::WhileStmt *castParent = parent->get<clang::WhileStmt>();
                indentStart = ConvertSrcLoc(sm, castParent->getBeginLoc());
                logstream << " at " << FormatLocationRange(ConvertSrcLoc(sm, castParent->getBeginLoc()), ConvertSrcLoc(sm, castParent->getEndLoc()));
            }
            else if (parentKind.isSame(clang::ASTNodeKind::getFromNodeKind<clang::DoStmt>()))
            {
                // parent is 'do while' loop
                const clang::DoStmt *castParent = parent->get<clang::DoStmt>();
                indentStart = ConvertSrcLoc(sm, castParent->getBeginLoc());
                logstream << " at " << FormatLocationRange(ConvertSrcLoc(sm, castParent->getBeginLoc()), ConvertSrcLoc(sm, castParent->getEndLoc()));
            }
            else if (parentKind.isSame(clang::ASTNodeKind::getFromNodeKind<clang::IfStmt>()))
            {
                // parent is 'if' statement
                const clang::IfStmt *castParent = parent->get<clang::IfStmt>();
                indentStart = ConvertSrcLoc(sm, castParent->getBeginLoc());
                logstream << " at " << FormatLocationRange(ConvertSrcLoc(sm, castParent->getBeginLoc()), ConvertSrcLoc(sm, castParent->getEndLoc()));
            }
            else if (parentKind.isSame(clang::ASTNodeKind::getFromNodeKind<clang::SwitchStmt>()))
            {
                // parent is 'switch' statement - SPECIAL CASE - contents of 'case' blocks are actually children of switch
                const clang::SwitchStmt *castParent = parent->get<clang::SwitchStmt>();
                indentStart = ConvertSrcLoc(sm, castParent->getBeginLoc());
                logstream << " at " << FormatLocationRange(ConvertSrcLoc(sm, castParent->getBeginLoc()), ConvertSrcLoc(sm, castParent->getEndLoc()));
            }
            else if (parentKind.isSame(clang::ASTNodeKind::getFromNodeKind<clang::CompoundStmt>()))
            {
                // parent is compound statement -> child is almost certainly a 'floating' { ... } block
                const clang::CompoundStmt *castParent = parent->get<clang::CompoundStmt>();
                indentStart = ConvertSrcLoc(sm, castParent->getBeginLoc());
                logstream << " at " << FormatLocationRange(ConvertSrcLoc(sm, castParent->getBeginLoc()), ConvertSrcLoc(sm, castParent->getEndLoc()));
            }
            else
            {
                throw std::invalid_argument("Unrecognised parent type found while processing CompoundStmt at " + FormatLocationRange(lb, rb) + ". ASTNodeType = " + parentKind.asStringRef().str());
            }

            // log
            #ifdef DEBUG
            std::clog << logstream.str() << std::endl;
            #endif
        }

        #ifdef DEBUG
        std::clog << "\t[INDENT START] " << FormatSourceLocation(indentStart) << std::endl;
        #endif

        // get children
        for (clang::Stmt::child_iterator iter = stmt.child_begin(), last = stmt.child_end(); iter != last; ++iter) {
            clang::Stmt *child = *iter;

            SourceLocation cBeg = ConvertSrcLoc(sm, child->getBeginLoc());
            SourceLocation cEnd = ConvertSrcLoc(sm, child->getEndLoc());

            #ifdef DEBUG
            std::clog << "\t" << child->getStmtClassName() << " " << FormatLocationRange(cBeg, cEnd, true) << std::endl;
            #endif
        }
    }

    // global variables
    for (size_t i = 0; i < vars.size(); i++)
    {
        clang::VarDecl &decl = *vars[i];

        SourceLocation beg = ConvertSrcLoc(sm, decl.getBeginLoc());
        SourceLocation end = ConvertSrcLoc(sm, decl.getEndLoc());

        #ifdef DEBUG
        std::clog << "Found VarDecl \"" << decl.getNameAsString() << "\" " << FormatLocationRange(beg, end, true) << std::endl;
        #endif
    }

    // structs
    for (size_t i = 0; i < records.size(); i++)
    {
        clang::RecordDecl &record = *records[i];

        SourceLocation beg = ConvertSrcLoc(sm, record.getBeginLoc());
        SourceLocation end = ConvertSrcLoc(sm, record.getEndLoc());

        #ifdef DEBUG
        std::clog << "Found RecordDecl \"" << record.getNameAsString() << "\" " << FormatLocationRange(beg, end) << std::endl;
        #endif
    }
}