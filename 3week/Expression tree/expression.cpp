#include "Common.h"
#include <functional>

class ExpressionValue : public Expression
{
public:
	explicit ExpressionValue(int v)
		: value(v)
	{};

	int Evaluate() const override
	{
		return value;
	}

	std::string ToString() const override
	{
		return std::to_string(value);
	}

private:
	const int value;
};

class ExpressionNode : public Expression
{
public:

	using FuncExpression = std::function <int(int, int)>;

	ExpressionNode(ExpressionPtr l, ExpressionPtr r, std::string op, FuncExpression f)
		: left(move(l))
		, right(move(r))
		, operation(op)
		, func(f)
	{};

	ExpressionNode() {};

	int Evaluate() const override
	{
		return func(left->Evaluate(), right->Evaluate());
	}

	std::string ToString() const override
	{
		return "(" + left->ToString() + ")"
			+ operation
			+ "(" + right->ToString() + ")";
	}

private:

	ExpressionPtr left;
	ExpressionPtr right;
	const std::string operation;
	const FuncExpression func;
};

ExpressionPtr Value(int value)
{
	return std::make_unique<ExpressionValue>(value);
}

ExpressionPtr Sum(ExpressionPtr left, ExpressionPtr right)
{
	return std::make_unique<ExpressionNode>(
		move(left), move(right), "+",
		[](int a, int b) { return a + b; });
}

ExpressionPtr Product(ExpressionPtr left, ExpressionPtr right)
{
	return std::make_unique<ExpressionNode>(
		move(left), move(right), "*",
		[](int a, int b) { return a * b; });
}