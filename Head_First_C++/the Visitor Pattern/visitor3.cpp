#include <iostream>
#include <string>
#include <memory>
#include <sstream>
using namespace std;

struct Expression
{
    virtual void accept(ExpressionVisitor* visitor) = 0;
};

struct DoubleExpression: Expression
{
    double value;
    explicit DoubleExpression(const double value): value{value} {}
    void accept(ExpressionVisitor* visitor) override
    {
        visitor->visit(this);
    }
};

struct AdditionExpression: Expression
{
    Expression* left;
    Expression* right;
    AdditionExpression(Expression* const left, Expression* const right):
        left{left}, right{right} {}
    ~AdditionExpression()
    {
        delete left;
        delete right;
    }

    void accept(ExpressionVisitor* visitor) override
    {
        visitor->visit(this);
    }
};

struct ExpressionVisitor
{
    virtual void visit(DoubleExpression* de) = 0;
    virtual void visit(AdditionExpression* ae) = 0;
}

struct ExpressionPrinter: ExpressionVisitor
{

    void visit(DoubleExpression* de) override
    {
        oss << de->value;
    }

    void visit(AdditionExpression* ae) override
    {
        oss << "(";
        ae->left->accept(this);
        oss << "+";
        ae->right->accept(this);
        oss << ")";
    }

    string str() const {
        return oss.str();
    }
private:
    ostringstream oss;
};

int main()
{
    auto e = new AdditionExpression
    {
        new DoubleExpression{1},
        new AdditionExpression
        {
            new DoubleExpression{2},
            new DoubleExpression{3}
        }
    };
    ostringstream oss;
    ExpressionPrinter ep;
    ep.visit(e);
    cout << ep.str() << endl; //(1+(2+3))
    return 0;
}