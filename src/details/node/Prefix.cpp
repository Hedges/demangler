#include <demangler/details/node/Prefix.hh>

#include <cassert>
#include <cctype>
#include <stdexcept>

#include <demangler/details/Utils.hh>
#include <demangler/details/node/UnqualifiedName.hh>

namespace demangler
{
namespace details
{
namespace node
{
Prefix::Prefix() noexcept : Node{Type::Prefix}
{
}

std::ostream& Prefix::print(PrintOptions const& opt, std::ostream& out) const
{
  assert(this->getNodeCount() == 1);
  this->getNode(0)->print(opt, out);
  return out;
}

std::unique_ptr<Prefix> Prefix::parse(State& s, string_type ctorname)
{
  auto ret = std::make_unique<Prefix>();

  ret->addNode(UnqualifiedName::parse(s, ctorname));
  return ret;
}
}
}
}
