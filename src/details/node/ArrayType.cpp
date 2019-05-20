#include <demangler/details/node/ArrayType.hh>

#include <cassert>

#include <demangler/details/node/Expression.hh>
#include <demangler/details/node/Number.hh>
#include <demangler/details/node/Type.hh>

namespace demangler
{
namespace details
{
namespace node
{
ArrayType::ArrayType() noexcept : Node{Type::ArrayType}
{
}

ArrayType::ArrayType(clone_tag, ArrayType const& b) : Node{clone_tag{}, b}
{
}

std::ostream& ArrayType::print(PrintOptions const& opt, std::ostream& out) const
{
  return out;
}

std::unique_ptr<Node> ArrayType::deepClone() const
{
  return std::make_unique<ArrayType>(clone_tag{}, *this);
}

std::unique_ptr<ArrayType> ArrayType::parse(State& s)
{
  assert(s.hasNext('A'));

  auto ret = std::make_unique<ArrayType>();
  s.advance(1);
  if (std::isdigit(s.nextChar()))
    ret->addNode(Number::parse(s));
  else
    ret->addNode(Expression::parse(s));
  if (!s.hasNext('_'))
    throw std::runtime_error("Invalid arity for array: " +
                             gsl::to_string(s.symbol));
  s.advance(1);
  ret->addNode(node::Type::parse(s));
  return ret;
}
}
}
}
