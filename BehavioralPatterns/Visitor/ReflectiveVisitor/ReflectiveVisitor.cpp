#include <sstream>
#include <string>
#include <iostream>
#include <memory>

struct Expression
{
    virtual ~Expression() = default;
    // Intrusive visitor
    virtual void print(std::ostringstream& oss) = 0;
};

struct DoubleExpression : Expression
{
    double value;
    explicit DoubleExpression(const double value)
        : value{ value } {}

    void print(std::ostringstream& oss) override
    {
        oss << value;
    }
};

struct AdditionExpression : Expression
{
    Expression* left, * right;

    AdditionExpression(Expression* const left, Expression* const right)
        : left{ left }, right{ right } {}

    ~AdditionExpression()
    {
        delete left;
        delete right;
    }

    void print(std::ostringstream& oss) override
    {
        oss << "(";
        left->print(oss);
        oss << "+";
        right->print(oss);
        oss << ")";
    }
};

struct ExpressionPrinter
{
    std::ostringstream oss;

    void print(Expression* e)
    {
        if (auto de = dynamic_cast<DoubleExpression*>(e))
        {
            oss << de->value;
        }
        else if (auto ae = dynamic_cast<AdditionExpression*>(e))
        {
            oss << "(";
            print(ae->left);
            oss << "+";
            print(ae->right);
            oss << ")";
        }
    }

    std::string str() const { return oss.str(); }
};

int main()
{
    // Intrusive visitor
    auto exp = std::make_unique<AdditionExpression>
        (
            new DoubleExpression{ 1.0 },
            new AdditionExpression
            {
                new DoubleExpression{2.0},
                new DoubleExpression{3.0}
            }
    );
    std::ostringstream oss;
    exp->print(oss);
    std::cout << oss.str() << std::endl << std::endl;

    // Reflective visitor
    ExpressionPrinter printer;
    printer.print(exp.get());
    std::cout << printer.str() << std::endl;

    return 0;
}
