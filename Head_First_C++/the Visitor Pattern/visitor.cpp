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
    virtual void print(ostringstream& oss) = 0;
};

struct DoubleExpression: Expression
{
    double value;
    explicit DoubleExpression(const double value): value{value} {}
    void print(ostringstream& oss) override
    {
        oss << "(";
        left->print(oss);
        oss << "+";
        right->print(oss);
        oss << ")";
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
    e->print(oss);
    cout << oss.str() << endl;
    return 0;
}