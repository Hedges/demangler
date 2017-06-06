#include <demangler/details/node/BuiltinSubstitution.hh>

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
BuiltinSubstitution::BuiltinSubstitution(gsl::cstring_span<> subst) noexcept
  : Node{Type::BuiltinSubstitution}, substitution{subst}
{
}

std::ostream& BuiltinSubstitution::print(PrintOptions const&,
                                         std::ostream& out) const
{
  printStringSpan(out, this->substitution);
  return out;
}

gsl::cstring_span<> BuiltinSubstitution::getSubstitution() const noexcept
{
  return this->substitution;
}
}
}
}
