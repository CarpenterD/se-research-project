#ifndef INDENTATION_CHECK_MARKER
#define INDENTATION_CHECK_MARKER

#include "Marker.hpp"
#include "markers/options/CompilerOptions.generated.hpp"
#include "markers/options/IndentCheckOptions.generated.hpp"

// clang deps
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendActions.h"


class IndentationCheck: public Marker, CompilerOptions, IndentCheckOptions
{
    public:
        IndentationCheck(TestConfig config);

        // Marks source code.
        TestResult Mark(std::string file);
        // Gets the currently used indent width
        int GetIndentWidth();
        // Sets the indent width
        void SetIndentWidth(int width);
    
    private:
        int indentWidth = 4;
        TestResult result;


        // Marks source code. Called by FrontendAction, stores output internally for return by `Mark(std::string file)`
        void CheckIndentation(clang::CompilerInstance& ci, std::vector<clang::CompoundStmt *> &stmts, std::vector<clang::VarDecl *> &vars, std::vector<clang::RecordDecl *> &records);
        // Checks indentation of compound statements
        void CheckIndentationCompoundStmt( clang::SourceManager &sm, clang::CompoundStmt &stmt, const clang::DynTypedNode *parent);
        // Checks indentation of compound statements
        void CheckIndentationVarDecl( clang::SourceManager &sm, clang::VarDecl &decl);
        // Checks indentation of compound statements
        void CheckIndentationRecordDecl( clang::SourceManager &sm, clang::RecordDecl &decl);
        // Retreives the start location of the parent node as a reference for child indentation
        SourceLocation GetParentStartLocation( clang::SourceManager &sm, const clang::DynTypedNode &parent );
        /**
         * Checks the indetation of a block of code.
         * @param kw The start location of the block's keyword
         * @param lb The location of the block's left brace
         * @param lb The location of the block's right brace
         * @param clocs The start locations of each of the block's children
         */
        void CheckBlockIndentation(SourceLocation &kw, SourceLocation &lb, SourceLocation &rb, std::vector<SourceLocation> &clocs);

        // AST Visitor to find various indented AST nodes.
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

        // ASTConsumer used for IndentationCheck 
        class ICASTConsumer: public clang::ASTConsumer
        {
            public:
                explicit ICASTConsumer(clang::CompilerInstance& instance, IndentationCheck &marker);
                virtual void HandleTranslationUnit(clang::ASTContext& astContext);

            private:
                clang::CompilerInstance &ci;
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
};

#endif