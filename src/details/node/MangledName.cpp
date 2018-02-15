#include <demangler/details/node/MangledName.hh>

#include <cassert>
#include <stdexcept>

#include <demangler/details/node/Encoding.hh>

namespace demangler
{
namespace details
{
namespace node
{
MangledName::MangledName() noexcept : Node{Type::MangledName}
{
}

MangledName::MangledName(clone_tag, MangledName const& b) : Node{clone_tag{}, b}
{
}

std::ostream& MangledName::print(PrintOptions const& opt,
                                 std::ostream& out) const
{
  assert(this->getNodeCount() == 1);
  this->getNode(0)->print(opt, out);
  return out;
}

std::unique_ptr<Node> MangledName::deepClone() const
{
  return std::make_unique<MangledName>(clone_tag{}, *this);
}

std::unique_ptr<MangledName> MangledName::parse(State& s, bool in_expr)
{
  if (!isMangled(s.symbol))
    throw std::runtime_error("Expected mangled-name: " + s.toString());
  s.advance(2);
  auto encoding = Encoding::parse(s, in_expr);
  auto ret = std::make_unique<MangledName>();
  ret->addNode(std::move(encoding));
  return ret;
}
}
}
}
