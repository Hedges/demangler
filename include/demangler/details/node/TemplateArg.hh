#ifndef DEMANGLER_DETAILS_NODE_TEMPLATEARG_HH_
#define DEMANGLER_DETAILS_NODE_TEMPLATEARG_HH_

#include <memory>

#include <gsl/string_span>

#include <demangler/details/Node.hh>

namespace demangler
{
namespace details
{
namespace node
{
class TemplateArg : public Node
{
public:
  TemplateArg() noexcept;
  TemplateArg(clone_tag, TemplateArg const& b);
  TemplateArg(TemplateArg const& b) noexcept = delete;
  TemplateArg(TemplateArg&& b) noexcept = default;
  ~TemplateArg() noexcept = default;

  TemplateArg& operator=(TemplateArg const& rhs) noexcept = delete;
  TemplateArg& operator=(TemplateArg&& rhs) noexcept = default;

  std::ostream& print(PrintOptions const& opt,
                      std::ostream& out) const override final;
  std::unique_ptr<Node> deepClone() const override final;

  static std::unique_ptr<TemplateArg> parse(State& s);
};
}
}
}

#endif /* !DEMANGLER_DETAILS_NODE_TEMPLATEARG_HH_ */
