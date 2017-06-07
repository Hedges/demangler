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
  assert(this->getNodeCount() == 2);
  this->getNode(0)->print(opt, out);
  out << "::";
  this->getNode(1)->print(opt, out);
  return out;
}

std::unique_ptr<NestedName> NestedName::parse(State& s)
{
  assert(s.nextChar() == 'N');
  auto const oldsymbol = s.symbol;
  s.advance(1);
  auto ret = std::make_unique<NestedName>();
  auto lastnode = std::unique_ptr<Node>{nullptr};
  auto prefixnode = std::make_unique<Prefix>();

  lastnode = Prefix::parse(s);
  while (!s.empty() && s.nextChar() != 'E')
  {
    auto lastname = getLastName(lastnode.get());
    prefixnode->addNode(std::move(lastnode));
    lastnode = Prefix::parse(s, lastname);
  }
  if (s.empty())
  {
    // Restore oldsymbol for displaying.
    s.symbol = oldsymbol;
    throw std::runtime_error("Unfinished nested name: " + s.toString());
  }
  s.user_substitutions.emplace_back(prefixnode.get());
  ret->addNode(std::move(prefixnode));
  ret->addNode(std::move(lastnode));
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
