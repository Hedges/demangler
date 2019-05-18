#ifndef DEMANGLER_DETAILS_NODE_HOLDER_HH_
#define DEMANGLER_DETAILS_NODE_HOLDER_HH_

#include <utility>
#include <vector>

#include <demangler/details/Node.hh>

namespace demangler
{
namespace details
{
namespace node
{
// This one is not a node strictly speaking, but rather an utility that
// contains a non-owning reference to another node.
class Holder : public Node
{
public:
  explicit Holder(Node const& pheld) noexcept;
  Holder(clone_tag, Holder const& b);
  Holder(Holder const& b) noexcept = delete;
  Holder(Holder&& b) noexcept = default;
  ~Holder() noexcept override = default;

  Holder& operator=(Holder const& rhs) noexcept = delete;
  Holder& operator=(Holder&& rhs) noexcept = default;

  std::ostream& print(PrintOptions const& opt,
                      std::ostream& out) const override final;
  std::unique_ptr<Node> deepClone() const override final;
  Node const& getHeld() const noexcept;

private:
  std::reference_wrapper<Node const> held;
};
}
}
}

#endif /* !DEMANGLER_DETAILS_NODE_HOLDER_HH_ */
