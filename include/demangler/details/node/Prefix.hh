#ifndef DEMANGLER_DETAILS_NODE_PREFIX_HH_
#define DEMANGLER_DETAILS_NODE_PREFIX_HH_

#include <memory>

#include <gsl/string_span>

#include <demangler/details/Node.hh>

namespace demangler
{
namespace details
{
namespace node
{
class Prefix : public Node
{
public:
  Prefix() noexcept;
  Prefix(Prefix const& b) = default;
  Prefix(Prefix&& b) noexcept = default;
  ~Prefix() noexcept = default;

  Prefix& operator=(Prefix const& rhs) = default;
  Prefix& operator=(Prefix&& rhs) noexcept = default;

  std::ostream& print(PrintOptions const& opt,
                      std::ostream& out) const override final;

  static std::unique_ptr<Prefix> parse(State& s, string_type ctorname = {});
};
}
}
}

#endif /* !DEMANGLER_DETAILS_NODE_PREFIX_HH_ */
