#ifndef DEMANGLER_DETAILS_NODE_USERSUBSTITUTION_HH_
#define DEMANGLER_DETAILS_NODE_USERSUBSTITUTION_HH_

#include <memory>

#include <gsl/string_span>

#include <demangler/details/Node.hh>

namespace demangler
{
namespace details
{
namespace node
{
class UserSubstitution : public Node
{
public:
  explicit UserSubstitution(Node* subst) noexcept;
  UserSubstitution(UserSubstitution const& b) = default;
  UserSubstitution(UserSubstitution&& b) noexcept = default;
  ~UserSubstitution() noexcept = default;

  UserSubstitution& operator=(UserSubstitution const& rhs) = default;
  UserSubstitution& operator=(UserSubstitution&& rhs) noexcept = default;

  std::ostream& print(PrintOptions const& opt,
                      std::ostream& out) const override final;

private:
  Node* substitution;
};
}
}
}

#endif /* !DEMANGLER_DETAILS_NODE_USERSUBSTITUTION_HH_ */
