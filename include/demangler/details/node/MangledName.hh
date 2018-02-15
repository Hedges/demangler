#ifndef DEMANGLER_DETAILS_NODE_MANGLEDNAME_HH_
#define DEMANGLER_DETAILS_NODE_MANGLEDNAME_HH_

#include <memory>

#include <gsl/string_span>

#include <demangler/details/Node.hh>

namespace demangler
{
namespace details
{
namespace node
{
class MangledName : public Node
{
public:
  MangledName() noexcept;
  MangledName(clone_tag, MangledName const& b);
  MangledName(MangledName const& b) noexcept = delete;
  MangledName(MangledName&& b) noexcept = default;
  ~MangledName() noexcept = default;

  MangledName& operator=(MangledName const& rhs) noexcept = delete;
  MangledName& operator=(MangledName&& rhs) noexcept = default;

  std::ostream& print(PrintOptions const& opt,
                      std::ostream& out) const override final;
  std::unique_ptr<Node> deepClone() const override final;

  static constexpr bool isMangled(string_type symbol) noexcept
  {
    return symbol.size() > 2 && symbol[0] == '_' && symbol[1] == 'Z';
  }

  // in_expr: Whether the encoding parsing is inside an expr-primary.
  static std::unique_ptr<MangledName> parse(State& s, bool in_expr = false);
};
}
}
}

#endif /* !DEMANGLER_DETAILS_NODE_MANGLEDNAME_HH_ */
