#ifndef DEMANGLER_DETAILS_NODE_SUBSTITUTION_HH_
#define DEMANGLER_DETAILS_NODE_SUBSTITUTION_HH_

#include <memory>

#include <gsl/string_span>

#include <demangler/details/Node.hh>

namespace demangler
{
namespace details
{
namespace node
{
class Substitution : public Node
{
public:
  Substitution() noexcept;
  Substitution(Substitution const& b) = default;
  Substitution(Substitution&& b) noexcept = default;
  ~Substitution() noexcept = default;

  Substitution& operator=(Substitution const& rhs) = default;
  Substitution& operator=(Substitution&& rhs) noexcept = default;

  std::ostream& print(PrintOptions const& opt,
                      std::ostream& out) const override final;
  Node const* getSubstitutedNode() const noexcept;

  static std::unique_ptr<Substitution> parse(State& s);

private:
  Node const* node;
};
}
}
}

#endif /* !DEMANGLER_DETAILS_NODE_SUBSTITUTION_HH_ */
