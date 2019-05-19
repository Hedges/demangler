#include <demangler/details/node/Decltype.hh>

#include <cassert>

#include <demangler/details/node/Expression.hh>

namespace demangler
{
namespace details
{
namespace node
{
Decltype::Decltype() noexcept : Node{Type::Decltype}
{
}

Decltype::Decltype(clone_tag, Decltype const& b) : Node{clone_tag{}, b}
{
}

std::ostream& Decltype::print(PrintOptions const& opt,
                                std::ostream& out) const
{
  out << "decltype(";
  this->getNode(0)->print(opt, out);
  out << ')';
  return out;
}

std::unique_ptr<Node> Decltype::deepClone() const
{
  return std::make_unique<Decltype>(clone_tag{}, *this);
}

std::unique_ptr<Decltype> Decltype::parse(State& s)
{
  assert(s.hasNext("dt") || s.hasNext("DT"));

  auto ret = std::make_unique<Decltype>();
  s.advance(2);
  ret->addNode(Expression::parse(s));
  if (!s.hasNext('E'))
    throw std::runtime_error("Invalid decltype");
  s.advance(1);
  return ret;
}
}
}
}
