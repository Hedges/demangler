#include <demangler/details/node/Expression.hh>

#include <cassert>
#include <stdexcept>

#include <demangler/details/node/Type.hh>
#include <demangler/details/node/UnresolvedName.hh>

namespace demangler
{
namespace details
{
namespace node
{

Expression::Expression() noexcept : Node{Type::Expression}
{
}

Expression::Expression(clone_tag, Expression const& b)
  : Node{clone_tag{}, b}, is_prefix_op{b.is_prefix_op}
{
}

std::ostream& Expression::print(PrintOptions const& opt,
                                std::ostream& out) const
{
  if (this->is_basic_op)
  {
    assert(this->getNodeCount() >= 2);
    assert(this->getNode(0)->getType() == Type::OperatorName);
    auto const& opnode = static_cast<OperatorName const&>(*this->getNode(0));
    if (opnode.isUnaryOp())
    {
      assert(this->getNodeCount() == 2);
      out << gsl::to_string(opnode.getOperatorSign()) << '(';
      this->getNode(1)->print(opt, out);
      out << ')';
    }
    else if (opnode.isBinaryOp())
    {
      assert(this->getNodeCount() == 3);
      this->getNode(1)->print(opt, out);
      out << gsl::to_string(opnode.getOperatorSign()) << '(';
      this->getNode(2)->print(opt, out);
    }
    else if (opnode.isTernaryOp())
    {
      assert(this->getNodeCount() == 4);
      // TODO(ethiraric): Implement
      assert(false); // Implement print with ternary op
    }
    else
      assert(false); // Unknown basic op. Should have asserted earlier
  }
  else if (this->is_prefix_op)
  {
    assert(this->getNodeCount() == 2);
    assert(this->getNode(0)->getType() == Type::OperatorName);
    auto const& opnode = static_cast<OperatorName const&>(*this->getNode(0));
    out << gsl::to_string(opnode.getOperatorSign());
    this->getNode(1)->print(opt, out);
  }
  else
  {
    assert(!this->op.empty());
    if (this->op == "cv")
    {
      assert(this->getNodeCount() >= 1);
      out << '(';
      this->getNode(0)->print(opt, out);
      out << ')' << '(';
      if (this->getNodeCount() > 1)
      {
        this->getNode(1)->print(opt, out);
        for (auto i = std::size_t{2}; i < this->getNodeCount(); ++i)
        {
          out << ',' << ' ';
          this->getNode(i)->print(opt, out);
        }
      }
      out << ')';
    }
    else if (this->op == "cl")
    {
      assert(this->getNodeCount() >= 1);
      out << '(';
      this->getNode(0)->print(opt, out);
      out << ')' << '(';
      if (this->getNodeCount() > 1)
      {
        this->getNode(0)->print(opt, out);
        for (auto i = std::size_t{2}; i < this->getNodeCount(); ++i)
        {
          out << ',' << ' ';
          this->getNode(i)->print(opt, out);
        }
      }
      out << ')';
    }
    else if (this->op == "dt")
    {
      out << '(';
      assert(this->getNodeCount() == 2);
      this->getNode(0)->print(opt, out);
      out << ')';
      out << '.';
      this->getNode(1)->print(opt, out);
    }
    else
      assert(false); // Unknown non-basic op
  }
  return out;
}

std::unique_ptr<Node> Expression::deepClone() const
{
  return std::make_unique<Expression>(clone_tag{}, *this);
}

std::unique_ptr<Expression> Expression::parse(State& s)
{
  auto const oldsymbol = s.symbol;
  auto ret = std::make_unique<Expression>();

  if (s.empty())
    throw std::runtime_error("Empty expression");
  try
  {
    if (s.hasNext("mm_") || s.hasNext("pp_"))
      return parsePrefix(s);
    else if (s.hasNext("cv"))
      return parsecv(s);
    else if (s.hasNext("cl"))
      return parsecl(s);

    auto opnode = OperatorName::parse(s);
    if (opnode)
      return parseBasicOp(s, std::move(opnode));
    else if (s.hasNext("dt"))
      return parsedt(s);
    else
      throw std::runtime_error{"Unknown expression"};
  }
  catch (std::exception const& e)
  {
    throw std::runtime_error{std::string{e.what()} + " (" +
                             gsl::to_string(oldsymbol) + ')'};
  }
  return ret;
}

std::unique_ptr<Expression> Expression::parsePrefix(State& s)
{
  auto ret = std::make_unique<Expression>();
  ret->is_prefix_op = true;
  ret->addNode(OperatorName::parse(s));
  s.advance(1);                       // '_'
  ret->addNode(Expression::parse(s)); // Unary op
  return ret;
}

std::unique_ptr<Expression> Expression::parseBasicOp(
    State& s, std::unique_ptr<OperatorName>&& opnode)
{
  auto ret = std::make_unique<Expression>();
  if (opnode->getArity() == OperatorName::OperatorArity::Unknown)
    throw std::runtime_error{"Unknown arity"};
  auto const expressions =
      opnode->isUnaryOp() ? 1 : (opnode->isBinaryOp() ? 2 : 3);
  ret->addNode(std::move(opnode));
  for (auto i = 0; i < expressions; ++i)
    ret->addNode(Expression::parse(s));
  ret->is_basic_op = true;
  return ret;
}

std::unique_ptr<Expression> Expression::parsecl(State& s)
{
  auto ret = std::make_unique<Expression>();
  ret->op = "cl";
  s.advance(2);
  ret->addNode(Expression::parse(s));
  while (!s.empty() && s.nextChar() != 'E')
    ret->addNode(Expression::parse(s));
  if (s.empty())
    throw std::runtime_error("Unterminated () expression");
  s.advance(1);
  return ret;
}

std::unique_ptr<Expression> Expression::parsedt(State& s)
{
  auto ret = std::make_unique<Expression>();
  ret->op = "dt";
  s.advance(2);
  ret->addNode(Expression::parse(s));
  ret->addNode(UnresolvedName::parse(s));
  return ret;
}

std::unique_ptr<Expression> Expression::parsecv(State& s)
{
  auto ret = std::make_unique<Expression>();
  ret->op = "cv";
  s.advance(2);
  ret->addNode(node::Type::parse(s));
  if (s.symbol.size() < 2)
    throw std::runtime_error("Unterminated cast expression");
  if (s.hasNext('_'))
  {
    s.advance(1);
    while (!s.hasNext('E'))
      ret->addNode(Expression::parse(s));
    if (s.empty())
      throw std::runtime_error("Unterminated cast expression");
    s.advance(1);
  }
  else
    ret->addNode(Expression::parse(s));
  return ret;
}
}
}
}
