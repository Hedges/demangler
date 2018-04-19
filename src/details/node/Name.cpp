#include <demangler/details/node/Name.hh>

#include <cassert>
#include <cctype>
#include <stdexcept>

#include <demangler/details/Utils.hh>
#include <demangler/details/node/LocalName.hh>
#include <demangler/details/node/NestedName.hh>
#include <demangler/details/node/Substitution.hh>
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

Name::Name(clone_tag, Name const& b) : Node{clone_tag{}, b}
{
}

std::ostream& Name::print(PrintOptions const& opt, std::ostream& out) const
{
  assert(this->getNodeCount() > 0);
  for (auto i = 0u; i < this->getNodeCount(); ++i)
    this->getNode(i)->print(opt, out);
  return out;
}

std::unique_ptr<Node> Name::deepClone() const
{
  return std::make_unique<Name>(clone_tag{}, *this);
}

gsl::cstring_span<> Name::getCVQuals() const noexcept
{
  assert(this->getNodeCount() > 0);
  auto const& firstnode = *this->getNode(0);
  if (firstnode.getType() == Type::NestedName)
  {
    auto const& nested_node = static_cast<NestedName const&>(firstnode);
    return nested_node.getCVQuals();
  }
  return "";
}

std::unique_ptr<Name> Name::parse(State& s, bool parse_template_args)
{
  auto ret = std::make_unique<Name>();

  if (s.nextChar() == 'N')
    ret->addNode(NestedName::parse(s));
  else if (s.nextChar() == 'S')
  {
    if (s.charsRemaining() == 1)
      throw std::runtime_error("Empty substitution");
    if (s.peekChar(1) == 't')
      ret->addNode(UnscopedName::parse(s));
    else
      ret->addNode(Substitution::parse(s));
  }
  else if (s.nextChar() == 'Z')
    ret->addNode(LocalName::parse(s));
  else
    ret->addNode(UnqualifiedName::parse(s));
  if (!s.empty() && s.nextChar() == 'I' && parse_template_args)
  {
    if (ret->getNode(0)->getType() == Type::UnqualifiedName)
      s.registerUserSubstitution(ret->getNode(0));
    ret->addNode(TemplateArgs::parse(s));
  }
  return ret;
}
}
}
}
