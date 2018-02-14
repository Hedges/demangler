#include <demangler/details/node/BuiltinType.hh>

#include <algorithm>
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
BuiltinType::BuiltinType(gsl::cstring_span<> pmangled,
                         gsl::cstring_span<> type) noexcept
  : Node{Type::BuiltinType}, mangled{pmangled}, builtintype{type}
{
}

BuiltinType::BuiltinType(clone_tag, BuiltinType const& b)
  : Node{clone_tag{}, b}, mangled{b.mangled}, builtintype{b.builtintype}
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

gsl::cstring_span<> BuiltinType::getMangledName() const noexcept
{
  return this->mangled;
}

bool BuiltinType::isIntegral() const noexcept
{
  using namespace span_literals;
  if (this->mangled.size() == 1)
  {
    // List of mangled builtin types that are integers.
    auto const integrals = "abcehijlmnostwxy"_cs;
    return std::find(integrals.begin(), integrals.end(), this->mangled[0]) !=
           integrals.end();
  }
  else if (this->mangled.size() == 2 && this->mangled[0] == 'D')
  {
    // List of mangled builtin types that are integers.
    // They are Dh, Di and Ds.
    auto const integrals = "his"_cs;
    return std::find(integrals.begin(), integrals.end(), this->mangled[1]) !=
           integrals.end();
  }
  else
    throw std::runtime_error("Unknown builtin type: " +
                             gsl::to_string(this->mangled));
}
}
}
}
