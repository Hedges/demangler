#ifndef DEMANGLER_DETAILS_NODE_ENCODING_HH_
#define DEMANGLER_DETAILS_NODE_ENCODING_HH_

#include <memory>

#include <gsl/string_span>

#include <demangler/details/Node.hh>

namespace demangler
{
namespace details
{
namespace node
{
class Encoding : public Node
{
public:
  Encoding() noexcept;
  Encoding(clone_tag, Encoding const& b);
  Encoding(Encoding const& b) noexcept = delete;
  Encoding(Encoding&& b) noexcept = default;
  ~Encoding() noexcept = default;

  Encoding& operator=(Encoding const& rhs) noexcept = delete;
  Encoding& operator=(Encoding&& rhs) noexcept = default;

  std::ostream& print(PrintOptions const& opt,
                      std::ostream& out) const override final;
  std::unique_ptr<Node> deepClone() const override final;

  // in_expr: Whether the encoding parsing is inside an expr-primary.
  static std::unique_ptr<Encoding> parse(State& s, bool in_expr = false);
};
}
}
}

#endif /* !DEMANGLER_DETAILS_NODE_ENCODING_HH_ */
