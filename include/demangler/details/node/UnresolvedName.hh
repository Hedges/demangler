#ifndef DEMANGLER_DETAILS_NODE_UNRESOLVEDNAME_HH_
#define DEMANGLER_DETAILS_NODE_UNRESOLVEDNAME_HH_

#include <demangler/details/Node.hh>

namespace demangler
{
namespace details
{
namespace node
{
class UnresolvedName : public Node
{
public:
  UnresolvedName() noexcept;
  UnresolvedName(clone_tag, UnresolvedName const& b);
  UnresolvedName(UnresolvedName const& b) noexcept = delete;
  UnresolvedName(UnresolvedName&& b) noexcept = default;
  ~UnresolvedName() noexcept override = default;

  UnresolvedName& operator=(UnresolvedName const& rhs) noexcept = delete;
  UnresolvedName& operator=(UnresolvedName&& rhs) noexcept = default;

  std::ostream& print(PrintOptions const& opt,
                      std::ostream& out) const override final;
  std::unique_ptr<Node> deepClone() const override final;

  static std::unique_ptr<UnresolvedName> parse(State& s);

private:
  bool is_global_scope{false};
};
}
}
}

#endif /* !DEMANGLER_DETAILS_NODE_UNRESOLVEDNAME_HH_ */
