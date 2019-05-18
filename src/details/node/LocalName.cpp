#include <demangler/details/node/LocalName.hh>

#include <cassert>
#include <cctype>
#include <stdexcept>

#include <demangler/details/node/Encoding.hh>
#include <demangler/details/node/Name.hh>

namespace demangler
{
namespace details
{
namespace node
{
namespace 
{
unsigned int parseDiscriminator(State& s)
{
  // Parsing discriminator
  s.advance(1);
  if (s.empty())
    throw std::runtime_error("Unfinished discriminator.");

  if (s.nextChar() != '_')
  {
    if (!std::isdigit(s.nextChar()))
      throw std::runtime_error("Invalid single-digit discriminator.");
    auto const ret = static_cast<unsigned int>(s.nextChar() - '0' + 2);
    s.advance(1);
    return ret;
  }
  else
  {
    s.advance(1);
    if (s.empty())
      throw std::runtime_error("Unfinished multi-digits discriminator.");
    auto ret = 2;
    while (!s.empty() && std::isdigit(s.nextChar()))
    {
      ret = ret * 10 + s.nextChar() - '0';
      s.advance(1);
    }
    if (s.empty() || s.nextChar() != '_')
      throw std::runtime_error("Invalid multi-digits discriminator.");
    s.advance(1);
    return static_cast<unsigned int>(ret);
  }
}
}

LocalName::LocalName() noexcept : Node{Type::LocalName}, has_discriminator{false}
{
}

LocalName::LocalName(clone_tag, LocalName const& b)
  : Node{clone_tag{}, b},
    has_discriminator{b.has_discriminator},
    discriminator{b.discriminator}
{
}

std::ostream& LocalName::print(PrintOptions const& opt,
                                 std::ostream& out) const
{
  assert(this->getNodeCount() > 0);
  this->getNode(0)->print(opt, out);
  if (this->getNodeCount() == 2)
  {
    out << ':' << ':';
    this->getNode(1)->print(opt, out);
  }
  return out;
}

std::unique_ptr<Node> LocalName::deepClone() const
{
  return std::make_unique<LocalName>(clone_tag{}, *this);
}

std::unique_ptr<LocalName> LocalName::parse(State& s)
{
  assert(s.nextChar() == 'Z');

  s.advance(1);
  if (s.empty())
    throw std::runtime_error("Unfinished local-name.");
  auto ret = std::make_unique<LocalName>();
  ret->addNode(Encoding::parse(s));
  if (s.charsRemaining() < 2 || s.nextChar() != 'E')
    throw std::runtime_error("Invalid local-name.");
  s.advance(1);
  if (s.nextChar() == 's') // No name, it's a string.
    s.advance(1);
  else
    ret->addNode(Name::parse(s));
  if (!s.empty() && s.nextChar() == '_')
  {
    ret->discriminator = parseDiscriminator(s);
    ret->has_discriminator = true;
  }
  return ret;
}
}
}
}


