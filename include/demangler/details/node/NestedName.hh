#ifndef DEMANGLER_DETAILS_NODE_NESTEDNAME_HH_
#define DEMANGLER_DETAILS_NODE_NESTEDNAME_HH_

#include <memory>

#include <gsl/string_span>

#include <demangler/details/Node.hh>

namespace demangler
{
namespace details
{
namespace node
{
class NestedName : public Node
{
public:
  NestedName() noexcept;
  NestedName(NestedName const& b) = default;
  NestedName(NestedName&& b) noexcept = default;
  ~NestedName() noexcept = default;

  NestedName& operator=(NestedName const& rhs) = default;
  NestedName& operator=(NestedName&& rhs) noexcept = default;

  std::ostream& print(PrintOptions const& opt,
                      std::ostream& out) const override final;

  static std::unique_ptr<NestedName> parse(State& s);
};
}
}
}

#endif /* !DEMANGLER_DETAILS_NODE_NESTEDNAME_HH_ */
