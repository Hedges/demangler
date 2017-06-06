#ifndef DEMANGLER_DETAILS_NODE_CONSTRUCTOR_HH_
#define DEMANGLER_DETAILS_NODE_CONSTRUCTOR_HH_

#include <memory>

#include <gsl/string_span>

#include <demangler/details/Node.hh>

namespace demangler
{
namespace details
{
namespace node
{
class Constructor : public Node
{
public:
  explicit Constructor(string_type subst) noexcept;
  Constructor(Constructor const& b) = default;
  Constructor(Constructor&& b) noexcept = default;
  ~Constructor() noexcept = default;

  Constructor& operator=(Constructor const& rhs) = default;
  Constructor& operator=(Constructor&& rhs) noexcept = default;

  std::ostream& print(PrintOptions const& opt,
                      std::ostream& out) const override final;

private:
  string_type substitution;
};
}
}
}

#endif /* !DEMANGLER_DETAILS_NODE_CONSTRUCTOR_HH_ */
