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

  // Nested names generate a lot of substitutions: a::b::c::d generates a,
  // a::b, a::b::c and a::b::c::d.
  // Not all those substitutions are kept in our AST. In the above example,
  // only a::b::c::d is kept.
  // This holds owning pointers to what will later be used for substitutions.
  std::vector<std::unique_ptr<Node>> substitutions_made;
};
}
}
}

#endif /* !DEMANGLER_DETAILS_NODE_NESTEDNAME_HH_ */
