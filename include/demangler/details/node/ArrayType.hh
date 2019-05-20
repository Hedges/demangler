#ifndef DEMANGLER_DETAILS_NODE_ARRAYTYPE_HH_
#define DEMANGLER_DETAILS_NODE_ARRAYTYPE_HH_

#include <demangler/details/Node.hh>

namespace demangler
{
namespace details
{
namespace node
{
class ArrayType : public Node
{
public:
  ArrayType() noexcept;
  ArrayType(clone_tag, ArrayType const& b);
  ArrayType(ArrayType const& b) noexcept = delete;
  ArrayType(ArrayType&& b) noexcept = default;
  ~ArrayType() noexcept override = default;

  ArrayType& operator=(ArrayType const& rhs) noexcept = delete;
  ArrayType& operator=(ArrayType&& rhs) noexcept = default;

  std::ostream& print(PrintOptions const& opt,
                      std::ostream& out) const override final;
  std::unique_ptr<Node> deepClone() const override final;

  static std::unique_ptr<ArrayType> parse(State& s);

private:
};
}
}
}

#endif /* !DEMANGLER_DETAILS_NODE_ARRAYTYPE_HH_ */
