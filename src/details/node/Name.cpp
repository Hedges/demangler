#include <demangler/details/node/Name.hh>

#include <cassert>
#include <cctype>
#include <stdexcept>

#include <demangler/details/Utils.hh>
#include <demangler/details/node/UnscopedName.hh>

namespace demangler
{
namespace details
{
namespace node
{
Name::Name() noexcept : Node{Type::Name}
{
}

std::ostream& Name::print(PrintOptions const& opt, std::ostream& out) const
{
  assert(this->getNodeCount() == 1);
  this->getNode(0)->print(opt, out);
  return out;
}

std::unique_ptr<Name> Name::parse(State &s)
{
  auto ret = std::make_unique<Name>();

  ret->addNode(UnscopedName::parse(s));
  return ret;
}
}
}
}
