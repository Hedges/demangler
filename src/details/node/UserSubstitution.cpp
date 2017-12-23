#include <demangler/details/node/UserSubstitution.hh>

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
UserSubstitution::UserSubstitution(Node* subst) noexcept
  : Node{Type::UserSubstitution}, substitution{subst}
{
}

UserSubstitution::UserSubstitution(clone_tag, UserSubstitution const& b)
  : Node{clone_tag{}, b}, substitution{b.substitution}
{
}

std::ostream& UserSubstitution::print(PrintOptions const& opt,
                                      std::ostream& out) const
{
  this->substitution->print(opt, out);
  return out;
}

std::unique_ptr<Node> UserSubstitution::deepClone() const
{
  return std::make_unique<UserSubstitution>(clone_tag{}, *this);
}
}
}
}
