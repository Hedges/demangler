#ifndef DEMANGLER_DETAILS_NODE_NAME_HH_
#define DEMANGLER_DETAILS_NODE_NAME_HH_

#include <memory>

#include <gsl/string_span>

#include <demangler/details/Node.hh>

namespace demangler
{
namespace details
{
namespace node
{
class Name : public Node
{
public:
  Name() noexcept;
  Name(clone_tag, Name const& b);
  Name(Name const& b) noexcept = delete;
  Name(Name&& b) noexcept = default;
  ~Name() noexcept = default;

  Name& operator=(Name const& rhs) noexcept = delete;
  Name& operator=(Name&& rhs) noexcept = default;

  std::ostream& print(PrintOptions const& opt,
                      std::ostream& out) const override final;
  std::unique_ptr<Node> deepClone() const override final;

  // parse_template_args: Whether Name should eat the template-args if it
  // encounters one (`I...E`).
  static std::unique_ptr<Name> parse(State& s, bool parse_template_args = true);
};
}
}
}

#endif /* !DEMANGLER_DETAILS_NODE_NAME_HH_ */
