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
  Substitution(clone_tag, Substitution const& b);
  Substitution(Substitution const& b) noexcept = delete;
  Substitution(Substitution&& b) noexcept = default;
  ~Substitution() noexcept = default;

  Substitution& operator=(Substitution const& rhs) noexcept = delete;
  Substitution& operator=(Substitution&& rhs) noexcept = default;

  std::ostream& print(PrintOptions const& opt,
                      std::ostream& out) const override final;
  std::unique_ptr<Node> deepClone() const override final;
  Node const* getSubstitutedNode() const noexcept;

  static std::unique_ptr<Substitution> parse(State& s);

private:
  Node const* node;
};
}
}
}

#endif /* !DEMANGLER_DETAILS_NODE_SUBSTITUTION_HH_ */
