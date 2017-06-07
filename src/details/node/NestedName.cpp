#include <demangler/details/node/NestedName.hh>

#include <cassert>
#include <cctype>
#include <stdexcept>

#include <demangler/details/Utils.hh>
#include <demangler/details/node/Prefix.hh>
#include <demangler/details/node/SourceName.hh>
#include <demangler/details/node/Substitution.hh>

namespace demangler
{
namespace details
{
namespace node
{
NestedName::NestedName() noexcept : Node{Type::NestedName}
{
}

std::ostream& NestedName::print(PrintOptions const& opt,
                                std::ostream& out) const
{
  assert(this->getNodeCount() > 0);
  for (auto i = 0u; i < this->getNodeCount(); ++i)
  {
    if (i)
      out << "::";
    this->getNode(i)->print(opt, out);
  }
  return out;
}

std::unique_ptr<NestedName> NestedName::parse(State& s)
{
  assert(s.nextChar() == 'N');
  auto const oldsymbol = s.symbol;
  s.advance(1);
  auto ret = std::make_unique<NestedName>();
  Node const* lastnode = nullptr;

  while (!s.empty() && s.nextChar() != 'E')
  {
    if (lastnode)
      ret->addNode(Prefix::parse(s, getLastName(lastnode)));
    else
      ret->addNode(Prefix::parse(s));
    lastnode = ret->getNode(ret->getNodeCount() - 1);
  }
  if (s.empty())
  {
    // Restore oldsymbol for displaying.
    s.symbol = oldsymbol;
    throw std::runtime_error("Unfinished nested name: " + s.toString());
  }
  s.user_substitutions.emplace_back(ret.get());
  s.advance(1);
  return ret;
}

NestedName::string_type NestedName::getLastName(Node const* lastnode)
{
  switch (lastnode->getType())
  {
  case Type::Prefix:
  case Type::UnqualifiedName:
    return getLastName(lastnode->getNode(0));
  case Type::Substitution:
    return getLastName(
        static_cast<Substitution const*>(lastnode)->getSubstitutedNode());
  case Type::SourceName:
    return static_cast<SourceName const*>(lastnode)->getName();
  case Type::NestedName:
  {
    for (auto i = lastnode->getNodeCount(); i > 0; --i)
    {
      auto const* node = lastnode->getNode(i - 1);
      if (node->getType() == Type::TemplateArgs)
        continue;
      else
        return getLastName(node);
    }
  default:
    return {};
  }
  }
}
}
}
}
