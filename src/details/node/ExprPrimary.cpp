#include <demangler/details/node/ExprPrimary.hh>

#include <cassert>
#include <cctype>
#include <stdexcept>

#include <demangler/details/node/BuiltinType.hh>
#include <demangler/details/node/Number.hh>
#include <demangler/details/node/Type.hh>

namespace demangler
{
namespace details
{
namespace node
{
ExprPrimary::ExprPrimary() noexcept : Node{Type::ExprPrimary}
{
}

ExprPrimary::ExprPrimary(clone_tag, ExprPrimary const& b) : Node{clone_tag{}, b}
{
}

std::ostream& ExprPrimary::print(PrintOptions const& opt,
                                 std::ostream& out) const
{
  assert(this->getNodeCount() == 2);

  // If we have a boolean value, print "true"/"false" instead of
  // "(bool)1"/"(bool)0".
  auto const& type_subnode = *this->getNode(0)->getNode(0);
  if (type_subnode.getType() == Type::BuiltinType &&
      static_cast<BuiltinType const&>(type_subnode).getMangledName() == "b")
    out << (static_cast<Number const&>(*this->getNode(1)).getNumber() ?
                "true" :
                "false");
  else
  {
    // Otherwise, print "(type)value".
    out << '(';
    this->getNode(0)->print(opt, out);
    out << ')';
    this->getNode(1)->print(opt, out);
  }
  return out;
}

std::unique_ptr<Node> ExprPrimary::deepClone() const
{
  return std::make_unique<ExprPrimary>(clone_tag{}, *this);
}

std::unique_ptr<ExprPrimary> ExprPrimary::parse(State& s)
{
  assert(s.nextChar() == 'L');

  auto ret = std::make_unique<ExprPrimary>();
  s.advance(1);
  auto type = node::Type::parse(s);
  if (!type->isIntegral())
    throw std::runtime_error("Unsupported non-integral expr-primary");
  ret->addNode(std::move(type));
  ret->addNode(Number::parse(s));
  if (s.nextChar() != 'E')
    throw std::runtime_error(std::string("Unfinished ExprPrimary: ") +
                             s.symbol.data());
  s.advance(1);
  return ret;
}
}
}
}

