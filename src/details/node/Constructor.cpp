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

std::ostream& Constructor::print(PrintOptions const&, std::ostream& out) const
{
  printStringSpan(out, this->substitution);
  return out;
}
}
}
}
