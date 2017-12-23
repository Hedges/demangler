#ifndef DEMANGLER_DETAILS_NODE_UNQUALIFIEDNAME_HH_
#define DEMANGLER_DETAILS_NODE_UNQUALIFIEDNAME_HH_

#include <memory>

#include <gsl/string_span>

#include <demangler/details/Node.hh>

namespace demangler
{
namespace details
{
namespace node
{
class UnqualifiedName : public Node
{
public:
  UnqualifiedName() noexcept;
  UnqualifiedName(clone_tag, UnqualifiedName const& b);
  UnqualifiedName(UnqualifiedName const& b) noexcept = delete;
  UnqualifiedName(UnqualifiedName&& b) noexcept = default;
  ~UnqualifiedName() noexcept = default;

  UnqualifiedName& operator=(UnqualifiedName const& rhs) noexcept = delete;
  UnqualifiedName& operator=(UnqualifiedName&& rhs) noexcept = default;

  std::ostream& print(PrintOptions const& opt,
                      std::ostream& out) const override final;
  std::unique_ptr<Node> deepClone() const override final;

  static std::unique_ptr<UnqualifiedName> parse(State& s,
                                                string_type ctorname = {});
};
}
}
}

#endif /* !DEMANGLER_DETAILS_NODE_UNQUALIFIEDNAME_HH_ */
