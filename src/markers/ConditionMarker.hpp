#ifndef CONDITION_MARKER
#define CONDITION_MARKER

#include "Marker.hpp"
#include "markers/options/CompilerOptions.generated.hpp"

// clang deps
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendActions.h"

class ConditionMarker: public Marker, CompilerOptions
{
    public:
        ConditionMarker(TestConfig config);

        TestResult Mark(std::string file);
        std::string GetFeedback();

    private:
        TestResult result;

        void HandleIfStmt(clang::CompilerInstance &ci, clang::IfStmt &stmt);
        void HandleBinaryOperator(clang::CompilerInstance &ci, clang::BinaryOperator &op);

        static bool ParentIsIfStmt(clang::ASTContext &ctx, clang::IfStmt &stmt);
        static clang::ReturnStmt *ResolveStmtToSingleReturn(clang::ASTContext &ctx, clang::Stmt &stmt);
        static clang::IntegerLiteral *GetLiteralReturnValue(clang::ASTContext &ctx, clang::ReturnStmt &retStmt);

        // INNER CLASSES

        class ASTVisitor: public clang::RecursiveASTVisitor<ASTVisitor>
        {
            public:                
                explicit ASTVisitor(clang::CompilerInstance &ci, ConditionMarker &marker);
                bool VisitIfStmt(clang::IfStmt *stmt);
                bool VisitBinaryOperator(clang::BinaryOperator *op);
                
            private:
                clang::CompilerInstance &ci;
                clang::ASTContext &ctx;
                clang::SourceManager &sm;
                ConditionMarker &marker;
                
        };

        class ASTConsumer : public clang::ASTConsumer
        {
            public:
                explicit ASTConsumer(clang::CompilerInstance &instance, ConditionMarker &marker);
                virtual void HandleTranslationUnit(clang::ASTContext &astContext);

            private:
                clang::CompilerInstance &ci;
                ASTVisitor visitor;
                ConditionMarker &marker;
        };

        class FrontendAction : public clang::ASTFrontendAction
        {
            public:
                FrontendAction(ConditionMarker &marker);
                virtual std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance &CI, clang::StringRef file);

            private:
                ConditionMarker &marker;
        };
};

#endif