#ifndef __AST_RETURN_NODE_H
#define __AST_RETURN_NODE_H

#include "AST/ast.hpp"
#include "AST/expression.hpp"

class ReturnNode : public AstNode {
    private:
        // TODO: expression
        ExpressionNode* expr = nullptr;
        AstNode* next_stmt = nullptr;

    public:
        ReturnNode(const uint32_t line, const uint32_t col, ExpressionNode* expr
                /* TODO: expression */);
        ~ReturnNode() = default;

        void print() override;
        void visitChildNodes(AstNodeVisitor& p_visitor) override;
        void accept(AstNodeVisitor& p_visitor) override;
        void setNextNode(AstNode* next_node) override;
        AstNode* getNextNode() override;

};

#endif
