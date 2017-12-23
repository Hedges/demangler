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

BuiltinType::BuiltinType(clone_tag, BuiltinType const& b)
  : Node{clone_tag{}, b}, builtintype{b.builtintype}
{
}

std::ostream& BuiltinType::print(PrintOptions const&, std::ostream& out) const
{
  printStringSpan(out, this->builtintype);
  return out;
}

std::unique_ptr<Node> BuiltinType::deepClone() const
{
  return std::make_unique<BuiltinType>(clone_tag{}, *this);
}

gsl::cstring_span<> BuiltinType::getTypeName() const noexcept
{
  return this->builtintype;
}
}
}
}
