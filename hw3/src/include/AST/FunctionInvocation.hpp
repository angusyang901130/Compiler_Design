#ifndef __AST_FUNCTION_INVOCATION_NODE_H
#define __AST_FUNCTION_INVOCATION_NODE_H

#include "AST/expression.hpp"
#include <vector>

class FunctionInvocationNode : public ExpressionNode {
    private:
        // TODO: function name, expressions
        std::string name;
        std::vector<ExpressionNode*> exprs;

    public:
        FunctionInvocationNode(const uint32_t line, const uint32_t col, const char* name, ExpressionNode* exprs
                            /* TODO: function name, expressions */);
        ~FunctionInvocationNode() = default;

        const char* getFunctName();

        void print() override;
        void visitChildNodes(AstNodeVisitor& p_visitor) override;
        void accept(AstNodeVisitor& p_visitor) override;
        void setNextNode(AstNode* next_node) override;
        AstNode* getNextNode() override;
        void setType(const char* type) override;
        const char* getType() override;

        void setExprs(ExpressionNode* exprs);
};

#endif
