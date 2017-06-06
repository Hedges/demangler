#ifndef DEMANGLER_DETAILS_NODE_BUILTINSUBSTITUTION_HH_
#define DEMANGLER_DETAILS_NODE_BUILTINSUBSTITUTION_HH_

#include <memory>

#include <gsl/string_span>

#include <demangler/details/Node.hh>

namespace demangler
{
namespace details
{
namespace node
{
class BuiltinSubstitution : public Node
{
public:
  explicit BuiltinSubstitution(gsl::cstring_span<> subst) noexcept;
  BuiltinSubstitution(BuiltinSubstitution const& b) = default;
  BuiltinSubstitution(BuiltinSubstitution&& b) noexcept = default;
  ~BuiltinSubstitution() noexcept = default;

  BuiltinSubstitution& operator=(BuiltinSubstitution const& rhs) = default;
  BuiltinSubstitution& operator=(BuiltinSubstitution&& rhs) noexcept = default;

  std::ostream& print(PrintOptions const& opt,
                      std::ostream& out) const override final;
  gsl::cstring_span<> getSubstitution() const noexcept;

private:
  gsl::cstring_span<> substitution;
};
}
}
}

#endif /* !DEMANGLER_DETAILS_NODE_BUILTINSUBSTITUTION_HH_ */
