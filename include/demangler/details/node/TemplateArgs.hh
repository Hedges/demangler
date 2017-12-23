#ifndef DEMANGLER_DETAILS_NODE_TEMPLATEARGS_HH_
#define DEMANGLER_DETAILS_NODE_TEMPLATEARGS_HH_

#include <memory>

#include <gsl/string_span>

#include <demangler/details/Node.hh>

namespace demangler
{
namespace details
{
namespace node
{
class TemplateArgs : public Node
{
public:
  TemplateArgs() noexcept;
  TemplateArgs(clone_tag, TemplateArgs const& b);
  TemplateArgs(TemplateArgs const& b) noexcept = delete;
  TemplateArgs(TemplateArgs&& b) noexcept = default;
  ~TemplateArgs() noexcept = default;

  TemplateArgs& operator=(TemplateArgs const& rhs) noexcept = delete;
  TemplateArgs& operator=(TemplateArgs&& rhs) noexcept = default;

  std::ostream& print(PrintOptions const& opt,
                      std::ostream& out) const override final;
  std::unique_ptr<Node> deepClone() const override final;

  static std::unique_ptr<TemplateArgs> parse(State& s);
};
}
}
}

#endif /* !DEMANGLER_DETAILS_NODE_TEMPLATEARGS_HH_ */
