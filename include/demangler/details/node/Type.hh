#ifndef DEMANGLER_DETAILS_NODE_TYPE_HH_
#define DEMANGLER_DETAILS_NODE_TYPE_HH_

#include <memory>

#include <gsl/string_span>

#include <demangler/details/Node.hh>

namespace demangler
{
namespace details
{
namespace node
{
class Type : public Node
{
public:
  Type() noexcept;
  Type(Type const& b) = default;
  Type(Type&& b) noexcept = default;
  ~Type() noexcept = default;

  Type& operator=(Type const& rhs) = default;
  Type& operator=(Type&& rhs) noexcept = default;

  std::ostream& print(PrintOptions const& opt,
                      std::ostream& out) const override final;

  static std::unique_ptr<Type> parse(State& s);

private:
  static std::unique_ptr<Type> parseD(State& s);

  bool is_pointer;
};
}
}
}

#endif /* !DEMANGLER_DETAILS_NODE_TYPE_HH_ */
