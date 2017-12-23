#ifndef DEMANGLER_DETAILS_NODE_UNSCOPEDNAME_HH_
#define DEMANGLER_DETAILS_NODE_UNSCOPEDNAME_HH_

#include <memory>

#include <gsl/string_span>

#include <demangler/details/Node.hh>

namespace demangler
{
namespace details
{
namespace node
{
class UnscopedName : public Node
{
public:
  UnscopedName() noexcept;
  UnscopedName(clone_tag, UnscopedName const& b);
  UnscopedName(UnscopedName const& b) noexcept = delete;
  UnscopedName(UnscopedName&& b) noexcept = default;
  ~UnscopedName() noexcept = default;

  UnscopedName& operator=(UnscopedName const& rhs) noexcept = delete;
  UnscopedName& operator=(UnscopedName&& rhs) noexcept = default;

  std::ostream& print(PrintOptions const& opt,
                      std::ostream& out) const override final;
  std::unique_ptr<Node> deepClone() const override final;

  static std::unique_ptr<UnscopedName> parse(State& s);

private:
  bool is_std;
};
}
}
}

#endif /* !DEMANGLER_DETAILS_NODE_UNSCOPEDNAME_HH_ */
