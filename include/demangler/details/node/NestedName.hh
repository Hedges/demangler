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
  NestedName(clone_tag, NestedName const& b);
  NestedName(NestedName const& b) noexcept = delete;
  NestedName(NestedName&& b) noexcept = default;
  ~NestedName() noexcept = default;

  NestedName& operator=(NestedName const& rhs) noexcept = delete;
  NestedName& operator=(NestedName&& rhs) noexcept = default;

  std::ostream& print(PrintOptions const& opt,
                      std::ostream& out) const override final;
  std::unique_ptr<Node> deepClone() const override final;

  static std::unique_ptr<NestedName> parse(State& s);

private:
  // Returns, if any, the last name seen. Used for constructors and destructors.
  static string_type getLastName(Node const* lastnode);
};
}
}
}

#endif /* !DEMANGLER_DETAILS_NODE_NESTEDNAME_HH_ */
