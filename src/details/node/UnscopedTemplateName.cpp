#include <demangler/details/node/UnscopedTemplateName.hh>

#include <cassert>
#include <cctype>
#include <stdexcept>

#include <demangler/details/Utils.hh>
#include <demangler/details/node/Substitution.hh>
#include <demangler/details/node/UnscopedName.hh>

namespace demangler
{
namespace details
{
namespace node
{
UnscopedTemplateName::UnscopedTemplateName() noexcept
  : Node{Type::UnscopedTemplateName}
{
}

UnscopedTemplateName::UnscopedTemplateName(clone_tag,
                                           UnscopedTemplateName const& b)
  : Node{clone_tag{}, b}
{
}

std::ostream& UnscopedTemplateName::print(PrintOptions const& opt,
                                          std::ostream& out) const
{
  assert(this->getNodeCount() == 1);
  this->getNode(0)->print(opt, out);
  return out;
}

std::unique_ptr<Node> UnscopedTemplateName::deepClone() const
{
  return std::make_unique<UnscopedTemplateName>(clone_tag{}, *this);
}

std::unique_ptr<UnscopedTemplateName> UnscopedTemplateName::parse(State& s)
{
  auto ret = std::make_unique<UnscopedTemplateName>();
  if (s.symbol.size() >= 2 && s.symbol[0] == 'S' && s.symbol[1] != 't')
    ret->addNode(Substitution::parse(s));
  else
    ret->addNode(UnscopedName::parse(s));
  return ret;
}
}
}
}
