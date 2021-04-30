#ifndef CONDITION_MARKER
#define CONDITION_MARKER

#include "Marker.hpp"
#include "markers/options/CompilerOptions.generated.hpp"

#include "clang/Frontend/CompilerInstance.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Frontend/FrontendActions.h"

class ConditionMarker: public Marker, CompilerOptions
{
    public:
        ConditionMarker(TestConfig config);

        TestResult Mark(std::string file);
        std::string GetFeedback();

    private:
        TestResult result;


        // INNER CLASSES
        class ASTConsumer : public clang::ASTConsumer
        {
            public:
                explicit ASTConsumer(clang::CompilerInstance& instance, ConditionMarker &marker);
                virtual void HandleTranslationUnit(clang::ASTContext& astContext);

            private:
                clang::CompilerInstance &ci;
                clang::ASTContext &ctx;
                clang::SourceManager &sm;

                clang::ast_matchers::MatchFinder finder;

                ConditionMarker &marker;
                
        };

        class FrontendAction : public clang::FrontendAction
        {
            public:
                FrontendAction(ConditionMarker &marker);
                virtual std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance& CI, clang::StringRef file);

            private:
                ConditionMarker &marker;
        };

        class BoolEqualityCallback : public clang::ast_matchers::MatchFinder::MatchCallback
        {
            public :
                BoolEqualityCallback(clang::ASTContext &context, TestResult &result);
                virtual void run(const clang::ast_matchers::MatchFinder::MatchResult &result);

            private:
                clang::ASTContext &context;
                TestResult &result;
        };

        class BoolLogicCallback : public clang::ast_matchers::MatchFinder::MatchCallback
        {
            public :
                BoolLogicCallback(clang::ASTContext &context, TestResult &result);
                virtual void run(const clang::ast_matchers::MatchFinder::MatchResult &result);

            private:
                clang::ASTContext &context;
                TestResult &result;
        };
};

#endif