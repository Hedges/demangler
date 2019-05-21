#include <demangler/details/node/NestedName.hh>

#include <cassert>
#include <cctype>
#include <stdexcept>

#include <demangler/details/CVQualifiers.hh>
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
namespace
{
std::unique_ptr<NestedName> makeSubstitution(Node& prefix, Node& lastnode)
{
  auto ret = std::make_unique<NestedName>();
  ret->addNode(prefix.deepClone());
  ret->addNode(lastnode.deepClone());
  return ret;
}
}

NestedName::NestedName() noexcept : Node{Type::NestedName}
{
}

NestedName::NestedName(clone_tag, NestedName const& b)
  : Node{clone_tag{}, b}
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

std::unique_ptr<Node> NestedName::deepClone() const
{
  return std::make_unique<NestedName>(clone_tag{}, *this);
}

gsl::cstring_span<> NestedName::getCVQuals() const noexcept
{
  return this->cvquals;
}

bool NestedName::willHaveReturnType() const noexcept
{
  assert(this->getNodeCount() == 2 &&
         this->getNode(1)->getType() == Node::Type::Prefix);
  return static_cast<Prefix const*>(this->getNode(1))->willHaveReturnType();
}

std::unique_ptr<NestedName> NestedName::parse(State& s)
{
  assert(s.nextChar() == 'N');
  auto const oldsymbol = s.symbol;
  s.advance(1);
  auto ret = std::make_unique<NestedName>();
  auto lastnode = std::unique_ptr<Prefix>{nullptr};
  auto prefixnode = std::make_unique<Prefix>();

  ret->cvquals = extractCVRefQualifiers(s);
  lastnode = Prefix::parse(s, {}, false);
  if (lastnode->getNode(0)->getType() != Type::Substitution)
  {
    ret->substitutions_made.emplace_back(lastnode->deepClone());
    s.user_substitutions.emplace_back(ret->substitutions_made.back().get());
  }
  if (!s.empty() && s.nextChar() == 'I')
  {
    lastnode->parseTemplate(s);
    ret->substitutions_made.emplace_back(lastnode->deepClone());
    s.user_substitutions.emplace_back(ret->substitutions_made.back().get());
  }

  while (!s.empty() && s.nextChar() != 'E')
  {
    auto lastname = getLastName(lastnode.get());
    prefixnode->addNode(std::move(lastnode));
    lastnode = Prefix::parse(s, lastname, false);
    
    // Register substitution.
    ret->substitutions_made.emplace_back(
        makeSubstitution(*prefixnode, *lastnode));
    s.user_substitutions.emplace_back(ret->substitutions_made.back().get());
    if (s.nextChar() == 'I')
    {
      lastnode->parseTemplate(s);
      ret->substitutions_made.emplace_back(
          makeSubstitution(*prefixnode, *lastnode));
      s.user_substitutions.emplace_back(ret->substitutions_made.back().get());
    }
  }
  if (s.empty())
  {
    // Restore oldsymbol for displaying.
    s.symbol = oldsymbol;
    throw std::runtime_error("Unfinished nested name: " + s.toString());
  }

  // Full nested name is not candidate for substitution.
  s.user_substitutions.pop_back();
  ret->substitutions_made.pop_back();

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
