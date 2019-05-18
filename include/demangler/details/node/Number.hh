#ifndef DEMANGLER_DETAILS_NODE_NUMBER_HH_
#define DEMANGLER_DETAILS_NODE_NUMBER_HH_

#include <cstdint>

#include <demangler/details/Node.hh>
#include <demangler/details/State.hh>

namespace demangler
{
namespace details
{
namespace node
{
class Number : public Node
{
public:
  Number() noexcept;
  Number(clone_tag, Number const& b);
  Number(Number const& b) noexcept = delete;
  Number(Number&& b) noexcept = default;
  ~Number() noexcept override = default;

  Number& operator=(Number const& rhs) noexcept = delete;
  Number& operator=(Number&& rhs) noexcept = default;

  std::ostream& print(PrintOptions const& opt,
                      std::ostream& out) const override final;
  std::unique_ptr<Node> deepClone() const override final;

  static std::unique_ptr<Number> parse(State& s);

  int64_t getNumber() const noexcept;

private:
  int64_t number;
};
}
}
}

#endif /* !DEMANGLER_DETAILS_NODE_NUMBER_HH_ */
