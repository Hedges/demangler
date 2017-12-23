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
  UserSubstitution(clone_tag, UserSubstitution const& b);
  UserSubstitution(UserSubstitution const& b) noexcept = delete;
  UserSubstitution(UserSubstitution&& b) noexcept = default;
  ~UserSubstitution() noexcept = default;

  UserSubstitution& operator=(UserSubstitution const& rhs) noexcept = delete;
  UserSubstitution& operator=(UserSubstitution&& rhs) noexcept = default;

  std::ostream& print(PrintOptions const& opt,
                      std::ostream& out) const override final;
  std::unique_ptr<Node> deepClone() const override final;

private:
  Node const* substitution;
};
}
}
}

#endif /* !DEMANGLER_DETAILS_NODE_USERSUBSTITUTION_HH_ */
