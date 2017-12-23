#include <demangler/details/node/Constructor.hh>

#include <cassert>
#include <stdexcept>
#include <unordered_map>

#include <demangler/details/Utils.hh>

namespace demangler
{
namespace details
{
namespace node
{
Constructor::Constructor(string_type subst) noexcept
  : Node{Type::Constructor}, substitution{subst}
{
}

Constructor::Constructor(clone_tag, Constructor const& b)
  : Node{clone_tag{}, b}, substitution{b.substitution}
{
}

std::unique_ptr<Node> Constructor::deepClone() const
{
  return std::make_unique<Constructor>(clone_tag{}, *this);
}

std::ostream& Constructor::print(PrintOptions const&, std::ostream& out) const
{
  printStringSpan(out, this->substitution);
  return out;
}
}
}
}
