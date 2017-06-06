#include <demangler/details/node/NestedName.hh>

#include <cassert>
#include <cctype>
#include <stdexcept>

#include <demangler/details/Utils.hh>
#include <demangler/details/node/Prefix.hh>
#include <demangler/details/node/SourceName.hh>

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
  s.advance(1);
  return ret;
}

NestedName::string_type NestedName::getLastName(Node const* lastnode)
{
  if (lastnode->getType() == Type::Prefix)
  {
    auto const* node = lastnode->getNode(0);
    if (node->getType() == Type::UnqualifiedName)
    {
      node = node->getNode(0);
      if (node->getType() == Type::SourceName)
        return static_cast<SourceName const*>(node)->getName();
    }
  }
  return {};
}
}
}
}
