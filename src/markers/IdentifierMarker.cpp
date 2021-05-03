// StdLib deps
#include <iostream>
#include <sstream>


// Internal deps
#include "IdentifierMarker.hpp"
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

    bool HasUpperCase(std::string str) {
        for (size_t i = 0; i < str.length(); i++)
        {
            if ( std::isupper(str[i]) ) {
                return true;
            }
        }
        return false;
    }

    bool HasLowerCase(std::string str) {
        for (size_t i = 0; i < str.length(); i++)
        {
            if ( std::islower(str[i]) ) {
                return true;
            }
        }
        return false;
    }
}



// ------------------------------------------------------------------------
// Definitions for IdentifierMarker
// ------------------------------------------------------------------------
IdentifierMarker::IdentifierMarker(TestConfig config) {
#ifdef DEBUG
    std::cout << "Creating IdentifierMarker" << std::endl;
#endif
    StdOptions::Initialise(config.Options);
    CompilerOptions::Initialise(config.Options);
    IdentifierOptions::Initialise(config.Options);
}

TestResult IdentifierMarker::Mark(std::string file) {
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
    result.testName = "IdentifierMarker";
    result.testDescription = "Checks for consistent identifier naming conventions";
    result.marksTotal = StdOptions::MarksAvailable;
    result.marksAwarded = StdOptions::MarksAvailable;
    return result;
}

// handle all named decls
void IdentifierMarker::ProcessNamedDecls(clang::CompilerInstance &ci, std::vector<clang::NamedDecl*> &decls){
    // Clang utils
    clang::ASTContext &ctx = ci.getASTContext();
    clang::SourceManager &sm = ci.getSourceManager();

    // Identifier containers
    std::vector<Identifier> globalVars;
    std::vector<Identifier> localVars;
    std::vector<Identifier> loopIndices;
    std::vector<Identifier> funcNames;
    std::vector<Identifier> funcParams;

    for (size_t i = 0; i < decls.size(); i++)
    {
        clang::NamedDecl &decl = *decls[i];

        SourceLocation declBegLoc = ConvertSrcLoc(sm, decl.getSourceRange().getBegin());
        SourceLocation declEndLoc = ConvertSrcLoc(sm, decl.getSourceRange().getEnd());

        #ifdef DEBUG
        std::cout << "[Decl] \"" << decl.getNameAsString() << "\" ";
        #endif

        switch (decl.getKind())
        {
            case clang::Decl::Kind::Var:
                {
                    clang::VarDecl & var = (clang::VarDecl &) decl;
                    if (!var.isLocalVarDecl()) {
                        #ifdef DEBUG
                        std::cout << " (Global variable)";
                        #endif
                        globalVars.push_back( Identifier(decl) );
                    } else if (IsLoopIndex(ctx, var)) {
                        #ifdef DEBUG
                        std::cout << " (Loop index)";
                        #endif
                        loopIndices.push_back( Identifier(decl) );
                    } else {
                        #ifdef DEBUG
                        std::cout << " (Local variable)";
                        #endif
                        localVars.push_back( Identifier(decl) );
                    }
                } break;

            case clang::Decl::Kind::Function:
                #ifdef DEBUG
                std::cout << " (Function)";
                #endif
                funcNames.push_back( Identifier(decl) );
                break;

            case clang::Decl::Kind::ParmVar:
                #ifdef DEBUG
                std::cout << " (Parameter)";
                #endif
                funcParams.push_back( Identifier(decl) );
                break;
            
            default:
                // throw std::invalid_argument("Unexpected DeclKind -> \"" + std::string(decl.getDeclKindName()) + "\" at " + FormatLocationRange(declBegLoc, declEndLoc));
                break;
        }

        #ifdef DEBUG
        std::cout << "\t at " << FormatLocationRange(declBegLoc, declEndLoc) << std::endl;
        #endif
    }

    // process containers
    ProcessVarNames(ctx, sm, localVars, false);
    ProcessVarNames(ctx, sm, globalVars, true);
    ProcessLoopIndices(ctx, sm, loopIndices);
    ProcessFuncNames(ctx, sm, funcNames);
    ProcessParamNames(ctx, sm, funcParams);
}

// Identifier Processors
void IdentifierMarker::ProcessVarNames(clang::ASTContext &ctx, clang::SourceManager &sm, std::vector<Identifier> &identifiers, bool asGlobalVars){
    #ifdef DEBUG
    std::cout << "Variable Names (" << (asGlobalVars? "global" : "local") << "):" << std::endl;
    #endif

    for (size_t i = 0; i < identifiers.size(); i++)
    {
        if (identifiers[i].name.size() > IdentifierOptions::DefaultMaxVarLength) {
            result.feedback.push_back(FeedbackItem(ConvertSrcLoc(sm, identifiers[i].decl.getSourceRange().getBegin()), "Lengthy identifier - identifiers should be less than " + std::to_string(IdentifierOptions::DefaultMaxVarLength) +  " characters long"));
        }
    }
    
    std::string typeName = (asGlobalVars? "global" : "local");
    typeName += " variable";
    EvaluateIdentifierConsistency(sm, identifiers, typeName);
}

void IdentifierMarker::ProcessLoopIndices(clang::ASTContext &ctx, clang::SourceManager &sm, std::vector<Identifier> &identifiers){
    #ifdef DEBUG
    std::cout << "Loop Indices:" << std::endl;
    #endif

    EvaluateIdentifierConsistency(sm, identifiers, "loop index");
}

void IdentifierMarker::ProcessFuncNames(clang::ASTContext &ctx, clang::SourceManager &sm, std::vector<Identifier> &identifiers){
    #ifdef DEBUG
    std::cout << "Function Names:" << std::endl;
    #endif

    EvaluateIdentifierConsistency(sm, identifiers, "function");
}

void IdentifierMarker::ProcessParamNames(clang::ASTContext &ctx, clang::SourceManager &sm, std::vector<Identifier> &identifiers){
    #ifdef DEBUG
    std::cout << "Parameter Names:" << std::endl;
    #endif

    EvaluateIdentifierConsistency(sm, identifiers, "parameter");
}

void IdentifierMarker::EvaluateIdentifierConsistency(clang::SourceManager &sm, std::vector<Identifier> &identifiers, std::string identifierType){
    IdentifierStyle mostCommon = MostCommonStyle(identifiers);
    #ifdef DEBUG
    std::cout << "\tMost Common Type - " << (int) mostCommon << std::endl;
    #endif

    for (size_t i = 0; i < identifiers.size(); i++)
    {
        if (!IsStyleValid(identifiers[i], mostCommon)) {
            SourceLocation beg = ConvertSrcLoc(sm, identifiers[i].decl.getBeginLoc());
            result.feedback.push_back(FeedbackItem(beg, "Inconsistent " + identifierType + " name \"" + identifiers[i].name + "\"- most " + identifierType + " identifiers in '" + beg.file + "' use an alternative scheme.", FeedbackSeverity::MILD));
        }
    }
}

// Helpers

bool IdentifierMarker::IsLoopIndex(clang::ASTContext &ctx, clang::VarDecl &decl){
    clang::ParentMapContext &PMapCxt = ctx.getParentMapContext();

    // check that immediate parent is a DeclStmt
    clang::DynTypedNodeList parentList = PMapCxt.getParents(clang::DynTypedNode::create(decl));
    const clang::DynTypedNode *parent = parentList.begin();
    if (parent == NULL || !parent->getNodeKind().isSame(clang::ASTNodeKind::getFromNodeKind<clang::DeclStmt>())) {
        return false;
    }

    // check if grandparent is a for loop
    const clang::DeclStmt *castParent = (*parent).get<clang::DeclStmt>();
    parentList = PMapCxt.getParents(clang::DynTypedNode::create(*castParent));
    const clang::DynTypedNode *grandparent = parentList.begin();
    if (parent == NULL) {
        return false;
    }

    clang::ASTNodeKind parentKind = parent->getNodeKind();
    return  parentKind.isSame(clang::ASTNodeKind::getFromNodeKind<clang::ForStmt>());
}

IdentifierMarker::IdentifierStyle IdentifierMarker::GetIdentifierStyle(std::string name){
    if (name.length() == 0) {
        return IdentifierStyle::UNKNOWN;
    }
    
    bool hasUpper = HasUpperCase(name);
    bool hasLower = HasLowerCase(name);
    bool hasUnder = name.find('_') != std::string::npos;

    bool firstIsUpper = std::isupper(name[0]);
    bool firstIsUnder = name[0] == '_';

    if (firstIsUnder) {
        return IdentifierStyle::SNAKE_PREFIX;
    }

    if (hasUpper && !hasLower) {
        return IdentifierStyle::ALL_UPPER;
    }

    if (hasLower && !hasUpper){
        return hasUnder? IdentifierStyle::SNAKE : IdentifierStyle::ALL_LOWER;
    }

    if (hasUpper && hasLower) {
        if (hasUnder){
            return IdentifierStyle::UNKNOWN;
        }
        return firstIsUpper? IdentifierStyle::UPPER_CAMEL : IdentifierStyle::LOWER_CAMEL;
    }

    return IdentifierStyle::UNKNOWN;
}

IdentifierMarker::IdentifierStyle IdentifierMarker::MostCommonStyle(std::vector<Identifier> &identifiers){
    std::unordered_map<IdentifierStyle, int> occurences;

    for (size_t i = 0; i < identifiers.size(); i++)
    {
        occurences[identifiers[i].style] += 1;
    }
    
    IdentifierStyle mostFrequent = IdentifierStyle::UNKNOWN;
    for (std::unordered_map<IdentifierStyle, int>::iterator o = occurences.begin(); o != occurences.end(); o++){
        if(occurences[o->first] > occurences[mostFrequent]){
            mostFrequent = o->first;
        }
    }

    return mostFrequent;
}

bool IdentifierMarker::IsStyleValid(Identifier identifier, IdentifierStyle mostCommon){
    IdentifierStyle style = identifier.style;

    // handle exceptions
    if (identifier.decl.getKind() == clang::Decl::Kind::Function && identifier.name == "main"){
        return true;
    }

    if (mostCommon == IdentifierStyle::UNKNOWN) {
        return true; // too hard to tell whats right and wrong -> assume that all is well
    };

    if (style == mostCommon) {
        return true;
    }

    // one word in lower camel or snake is identical to all lower
    if (mostCommon == IdentifierStyle::ALL_LOWER && (style==IdentifierStyle::LOWER_CAMEL || style == IdentifierStyle::SNAKE)){
        return true;
    }

    if ((mostCommon==IdentifierStyle::LOWER_CAMEL || mostCommon == IdentifierStyle::SNAKE) && style == IdentifierStyle::ALL_LOWER){
        return true;
    }

    return false;
}
