#include <demangler/details/node/BuiltinType.hh>

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
BuiltinType::BuiltinType(gsl::cstring_span<> type) noexcept
  : Node{Type::BuiltinType}, builtintype{type}
{
}

std::ostream& BuiltinType::print(PrintOptions const&, std::ostream& out) const
{
  printStringSpan(out, this->builtintype);
  return out;
}

gsl::cstring_span<> BuiltinType::getTypeName() const noexcept
{
  return this->builtintype;
}
}
}
}
