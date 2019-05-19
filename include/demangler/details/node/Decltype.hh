#ifndef DEMANGLER_DETAILS_NODE_DECLTYPE_HH_
#define DEMANGLER_DETAILS_NODE_DECLTYPE_HH_

#include <demangler/details/Node.hh>

namespace demangler
{
namespace details
{
namespace node
{
class Decltype : public Node
{
public:
  Decltype() noexcept;
  Decltype(clone_tag, Decltype const& b);
  Decltype(Decltype const& b) noexcept = delete;
  Decltype(Decltype&& b) noexcept = default;
  ~Decltype() noexcept override = default;

  Decltype& operator=(Decltype const& rhs) noexcept = delete;
  Decltype& operator=(Decltype&& rhs) noexcept = default;

  std::ostream& print(PrintOptions const& opt,
                      std::ostream& out) const override final;
  std::unique_ptr<Node> deepClone() const override final;

  static std::unique_ptr<Decltype> parse(State& s);

private:
};
}
}
}

#endif /* !DEMANGLER_DETAILS_NODE_DECLTYPE_HH_ */
