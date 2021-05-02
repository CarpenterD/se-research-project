#ifndef IDENTIFIER_MARKER
#define IDENTIFIER_MARKER

#include "Marker.hpp"
#include "markers/options/CompilerOptions.generated.hpp"
#include "markers/options/IdentifierOptions.generated.hpp"

// clang deps
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendActions.h"

class IdentifierMarker: public Marker, CompilerOptions, IdentifierOptions
{
    public:
        IdentifierMarker(TestConfig config);

        TestResult Mark(std::string file);
        std::string GetFeedback();

    private:
        TestResult result;

        void ProcessNamedDecls(clang::CompilerInstance &ci, std::vector<clang::NamedDecl *> &decls);
        static bool IsLoopIndex(clang::ASTContext &ctx, clang::VarDecl &decl);

        // INNER CLASSES

        enum class IdentifierStyle
        {
            UNKNOWN      = 0, // any and all student inventions
            ALL_LOWER    = 1, // int score;
            ALL_UPPER    = 2, // int HIGH_SCORE;
            LOWER_CAMEL  = 3, // int highScore;
            UPPER_CAMEL  = 4, // int HighScore;
            SNAKE        = 5, // int high_score;
            SNAKE_PREFIX = 6, // int _highScore; (anything starting with an underscore)
        };
        static IdentifierStyle GetIdentifierStyle(std::string name);

        struct Identifier {
            public:
                Identifier(clang::NamedDecl &decl);
                clang::NamedDecl &decl;
                IdentifierStyle   style;
                std::string       name;
        };

        void ProcessVarNames   (clang::ASTContext &ctx, clang::SourceManager &sm, std::vector<Identifier> &identifiers, bool asGlobalVars);
        void ProcessLoopIndices(clang::ASTContext &ctx, clang::SourceManager &sm, std::vector<Identifier> &identifiers);
        void ProcessFuncNames  (clang::ASTContext &ctx, clang::SourceManager &sm, std::vector<Identifier> &identifiers);
        void ProcessParamNames (clang::ASTContext &ctx, clang::SourceManager &sm, std::vector<Identifier> &identifiers);

        void EvaluateIdentifierConsistency(clang::SourceManager &sm, std::vector<Identifier> &identifiers, std::string identifierType);

        IdentifierStyle MostCommonStyle(std::vector<Identifier> &identifiers);
        bool IsStyleValid(Identifier identifier, IdentifierStyle mostCommon);

        // AST classes

        class ASTVisitor: public clang::RecursiveASTVisitor<ASTVisitor>
        {
            public:
                std::vector<clang::NamedDecl *> decls;

                explicit ASTVisitor(clang::CompilerInstance &ci);
                bool VisitNamedDecl(clang::NamedDecl* decl);
                
            private:
                clang::ASTContext &ctx;
                clang::SourceManager &sm;
                
        };

        class ASTConsumer : public clang::ASTConsumer
        {
            public:
                explicit ASTConsumer(clang::CompilerInstance &instance, IdentifierMarker &marker);
                virtual void HandleTranslationUnit(clang::ASTContext &astContext);

            private:
                clang::CompilerInstance &ci;
                ASTVisitor visitor;
                IdentifierMarker &marker;
        };

        class FrontendAction : public clang::ASTFrontendAction
        {
            public:
                FrontendAction(IdentifierMarker &marker);
                virtual std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance &CI, clang::StringRef file);

            private:
                IdentifierMarker &marker;
        };

        
};

#endif