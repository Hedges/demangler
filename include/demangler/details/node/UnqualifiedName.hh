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
  UnqualifiedName(UnqualifiedName const& b) = default;
  UnqualifiedName(UnqualifiedName&& b) noexcept = default;
  ~UnqualifiedName() noexcept = default;

  UnqualifiedName& operator=(UnqualifiedName const& rhs) = default;
  UnqualifiedName& operator=(UnqualifiedName&& rhs) noexcept = default;

  std::ostream& print(PrintOptions const& opt,
                      std::ostream& out) const override final;

  static std::unique_ptr<UnqualifiedName> parse(State &s);
};
}
}
}

#endif /* !DEMANGLER_DETAILS_NODE_UNQUALIFIEDNAME_HH_ */
