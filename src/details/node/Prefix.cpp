#include <demangler/details/node/Prefix.hh>

#include <cassert>
#include <cctype>
#include <stdexcept>

#include <demangler/details/Utils.hh>
#include <demangler/details/node/Substitution.hh>
#include <demangler/details/node/TemplateArgs.hh>
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
  assert(this->getNodeCount() > 0);
  for (auto i = 0u; i < this->getNodeCount(); ++i)
    this->getNode(i)->print(opt, out);
  return out;
}

std::unique_ptr<Prefix> Prefix::parse(State& s, string_type ctorname)
{
  auto ret = std::make_unique<Prefix>();

  if (s.symbol[0] == 'S' && s.symbol[1] != 't')
    ret->addNode(Substitution::parse(s));
  else
    ret->addNode(UnqualifiedName::parse(s, ctorname));
  if (s.nextChar() == 'I')
    ret->addNode(TemplateArgs::parse(s));
  return ret;
}
}
}
}
