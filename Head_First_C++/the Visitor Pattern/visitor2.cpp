#include <iostream>
#include <string>
#include <memory>
#include <sstream>
using namespace std;
#include <boost/lexical_cast.hpp>
#include <boost/functional.hpp>
using namespace boost;

struct Expression
{
    virtual ~Expression() = default;
};

struct DoubleExpression: Expression
{
    double value;
    explicit DoubleExpression(const double value): value{value} {}
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
};

struct ExpressionPrinter
{
    void print(Expression* e)
    {
        if (auto de = dynamic_cast<DoubleExpression*>(e))
        {
            oss << de->value;
        }
        else if (auto ae = dynamic_cast<AdditionExpression*>(e))
        {
            oss << "(";
            print(ae->left, oss);
            oss << "+";
            print(ae->right, oss);
            oss << ")";
        }
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
    ExpressionPrinter ep;
    ep.print(e);
    cout << ep.str() << endl;
    return 0;
}