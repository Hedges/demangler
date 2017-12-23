#include <demangler/details/node/UnscopedName.hh>

#include <cassert>
#include <cctype>
#include <stdexcept>

#include <demangler/details/Utils.hh>
#include <demangler/details/node/UnqualifiedName.hh>

namespace demangler
{
namespace details
{
namespace node
{
UnscopedName::UnscopedName() noexcept : Node{Type::UnscopedName}, is_std{false}
{
}

UnscopedName::UnscopedName(clone_tag, UnscopedName const& b)
  : Node{clone_tag{}, b}, is_std{b.is_std}
{
}

std::ostream& UnscopedName::print(PrintOptions const& opt,
                                  std::ostream& out) const
{
  assert(this->getNodeCount() == 1);
  if (this->is_std)
    out << "std::";
  this->getNode(0)->print(opt, out);
  return out;
}

std::unique_ptr<Node> UnscopedName::deepClone() const
{
  return std::make_unique<UnscopedName>(clone_tag{}, *this);
}

std::unique_ptr<UnscopedName> UnscopedName::parse(State& s)
{
  auto ret = std::make_unique<UnscopedName>();
  if (s.symbol.size() >= 2 && s.symbol[0] == 'S' && s.symbol[1] == 't')
  {
    s.advance(2);
    ret->is_std = true;
  }
  ret->addNode(UnqualifiedName::parse(s));
  return ret;
}
}
}
}
