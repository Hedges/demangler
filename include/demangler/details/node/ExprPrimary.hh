#ifndef DEMANGLER_DETAILS_NODE_EXPRPRIMARY_HH_
#define DEMANGLER_DETAILS_NODE_EXPRPRIMARY_HH_

#include <memory>

#include <gsl/string_span>

#include <demangler/details/Node.hh>
#include <demangler/details/node/Type.hh>

namespace demangler
{
namespace details
{
namespace node
{
class ExprPrimary : public Node
{
public:
  ExprPrimary() noexcept;
  ExprPrimary(clone_tag, ExprPrimary const& b);
  ExprPrimary(ExprPrimary const& b) noexcept = delete;
  ExprPrimary(ExprPrimary&& b) noexcept = default;
  ~ExprPrimary() noexcept = default;

  ExprPrimary& operator=(ExprPrimary const& rhs) noexcept = delete;
  ExprPrimary& operator=(ExprPrimary&& rhs) noexcept = default;

  std::ostream& print(PrintOptions const& opt,
                      std::ostream& out) const override final;
  std::unique_ptr<Node> deepClone() const override final;

  static std::unique_ptr<ExprPrimary> parse(State& s);

private:
};
}
}
}

#endif /* !DEMANGLER_DETAILS_NODE_EXPRPRIMARY_HH_ */
