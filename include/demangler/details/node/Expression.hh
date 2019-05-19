#ifndef DEMANGLER_DETAILS_NODE_EXPRESSION_HH_
#define DEMANGLER_DETAILS_NODE_EXPRESSION_HH_

#include <gsl/string_span>

#include <demangler/details/Node.hh>
#include <demangler/details/node/OperatorName.hh>

namespace demangler
{
namespace details
{
namespace node
{
class Expression : public Node
{
public:
  Expression() noexcept;
  Expression(clone_tag, Expression const& b);
  Expression(Expression const& b) noexcept = delete;
  Expression(Expression&& b) noexcept = default;
  ~Expression() noexcept override = default;

  Expression& operator=(Expression const& rhs) noexcept = delete;
  Expression& operator=(Expression&& rhs) noexcept = default;

  std::ostream& print(PrintOptions const& opt,
                      std::ostream& out) const override final;
  std::unique_ptr<Node> deepClone() const override final;

  static std::unique_ptr<Expression> parse(State& s);

private:
  static std::unique_ptr<Expression> parsePrefix(State& s);
  static std::unique_ptr<Expression> parseBasicOp(
      State& s, std::unique_ptr<OperatorName>&& opnode);
  static std::unique_ptr<Expression> parsecl(State& s);
  static std::unique_ptr<Expression> parsedt(State& s);
  static std::unique_ptr<Expression> parsecv(State& s);

  bool is_prefix_op{false};
  bool is_basic_op{false};
  gsl::cstring_span<> op;
};
}
}
}

#endif /* !DEMANGLER_DETAILS_NODE_EXPRESSION_HH_ */
