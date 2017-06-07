#include <demangler/details/node/Name.hh>

#include <cassert>
#include <cctype>
#include <stdexcept>

#include <demangler/details/Utils.hh>
#include <demangler/details/node/NestedName.hh>
#include <demangler/details/node/TemplateArgs.hh>
#include <demangler/details/node/UnqualifiedName.hh>
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
  assert(this->getNodeCount() > 0);
  for (auto i = 0u; i < this->getNodeCount(); ++i)
    this->getNode(i)->print(opt, out);
  return out;
}

std::unique_ptr<Name> Name::parse(State& s)
{
  auto ret = std::make_unique<Name>();

  if (s.nextChar() == 'N')
    ret->addNode(NestedName::parse(s));
  else if (s.symbol[0] == 'S' && s.symbol[1] != 't')
  {
  }
  else
    ret->addNode(UnqualifiedName::parse(s));
  if (!s.empty() && s.nextChar() == 'I')
    ret->addNode(TemplateArgs::parse(s));
  return ret;
}
}
}
}
