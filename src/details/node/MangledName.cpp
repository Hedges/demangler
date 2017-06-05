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
std::ostream& MangledName::print(PrintOptions const& opt,
                                 std::ostream& out) const
{
  assert(this->getNodeCount() == 1);
  this->getNode(0)->print(opt, out);
  return out;
}

std::unique_ptr<MangledName> MangledName::parse(State &s)
{
  if (!isMangled(s.symbol))
    throw std::runtime_error("Expected mangled-name: " + s.toString());
  s.advance(2);
  auto encoding = Encoding::parse(s);
  auto ret = std::make_unique<MangledName>();
  ret->addNode(std::move(encoding));
  return ret;
}

MangledName::MangledName() noexcept : Node{Type::MangledName}
{
}
}
}
}
