#ifndef INDENTATION_CHECK_MARKER
#define INDENTATION_CHECK_MARKER

#include "Marker.hpp"
#include "markers/options/CompilerOptions.generated.hpp"

// clang deps
// #include "clang/AST/Stmt.h"


class IndentationCheck: public Marker, CompilerOptions
{
    public:
        IndentationCheck(TestConfig config);

        TestResult Mark(std::string file);
    
    private:
        TestResult result;

        // Marks source code. Called by FrontendAction, stores output in `result` for return by `Mark(std::string file)`
        // void InternalMark(std::vector<clang::CompoundStmt *> &stmts);

};

#endif