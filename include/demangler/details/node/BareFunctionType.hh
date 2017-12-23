#ifndef DEMANGLER_DETAILS_NODE_BAREFUNCTIONTYPE_HH_
#define DEMANGLER_DETAILS_NODE_BAREFUNCTIONTYPE_HH_

#include <memory>

#include <gsl/string_span>

#include <demangler/details/Node.hh>

namespace demangler
{
namespace details
{
namespace node
{
class BareFunctionType : public Node
{
public:
  BareFunctionType() noexcept;
  BareFunctionType(clone_tag, BareFunctionType const& b);
  BareFunctionType(BareFunctionType const& b) noexcept = delete;
  BareFunctionType(BareFunctionType&& b) noexcept = default;
  ~BareFunctionType() noexcept = default;

  BareFunctionType& operator=(BareFunctionType const& rhs) noexcept = delete;
  BareFunctionType& operator=(BareFunctionType&& rhs) noexcept = default;

  std::ostream& print(PrintOptions const& opt,
                      std::ostream& out) const override final;
  std::unique_ptr<Node> deepClone() const override final;

  static std::unique_ptr<BareFunctionType> parse(State& s);

private:
  bool isVoidFunction() const noexcept;
};
}
}
}

#endif /* !DEMANGLER_DETAILS_NODE_BAREFUNCTIONTYPE_HH_ */
