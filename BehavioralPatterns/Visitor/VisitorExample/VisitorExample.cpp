#include <string>
#include <sstream>
#include "gtest/gtest.h"

struct Value;
struct AdditionExpression;
struct MultiplicationExpression;

struct ExpressionVisitor
{
    virtual void accept(Value& value) = 0;
    virtual void accept(AdditionExpression& ae) = 0;
    virtual void accept(MultiplicationExpression& me) = 0;
};

struct Expression
{
    virtual void visit(ExpressionVisitor& ev) = 0;
};

struct Value : Expression
{
    int value;

    Value(int value) : value(value) {}

    void visit(ExpressionVisitor& ev) override
    {
        ev.accept(*this);
    }
};

struct AdditionExpression : Expression
{
    Expression& lhs, & rhs;

    AdditionExpression(Expression& lhs, Expression& rhs) : lhs(lhs), rhs(rhs) {}

    void visit(ExpressionVisitor& ev) override
    {
        ev.accept(*this);
    }
};

struct MultiplicationExpression : Expression
{
    Expression& lhs, & rhs;

    MultiplicationExpression(Expression& lhs, Expression& rhs)
        : lhs(lhs), rhs(rhs) {}

    void visit(ExpressionVisitor& ev) override
    {
        ev.accept(*this);
    }
};

struct ExpressionPrinter : ExpressionVisitor
{
    void accept(Value& value) override
    {
        oss << value.value;
    }

    void accept(AdditionExpression& ae) override
    {
        oss << "(";
        ae.lhs.visit(*this);
        oss << "+";
        ae.rhs.visit(*this);
        oss << ")";
    }

    void accept(MultiplicationExpression& me) override
    {
        me.lhs.visit(*this);
        oss << "*";
        me.rhs.visit(*this);
    }

    std::string str() const { return oss.str(); }

private:
    std::ostringstream oss;
};

namespace {

    class FactoryTester : public ::testing::Test {};

    TEST_F(FactoryTester, SimpleAddition)
    {
        Value val{ 2 };
        AdditionExpression simple{ val, val };
        ExpressionPrinter printer;
        printer.accept(simple);
        ASSERT_EQ("(2+2)", printer.str());
    }

    TEST_F(FactoryTester, ProductOfAdditionAndValue)
    {
        Value two{ 2 };
        Value three{ 3 };
        Value four{ 4 };
        AdditionExpression addExp{ two, three };
        MultiplicationExpression multiExp{ addExp, four };
        ExpressionPrinter printer;
        printer.accept(multiExp);
        ASSERT_EQ("(2+3)*4", printer.str());
    }
}  // namespace

int main(int ac, char* av[])
{
    testing::InitGoogleTest(&ac, av);

    return RUN_ALL_TESTS();
}
