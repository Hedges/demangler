#ifndef DEMANGLER_DETAILS_NODE_TEMPLATEPARAM_HH_
#define DEMANGLER_DETAILS_NODE_TEMPLATEPARAM_HH_

#include <memory>

#include <demangler/details/Node.hh>

namespace demangler
{
namespace details
{
namespace node
{
class TemplateParam : public Node
{
public:
  TemplateParam() noexcept;
  TemplateParam(clone_tag, TemplateParam const& b);
  TemplateParam(TemplateParam const& b) noexcept = delete;
  TemplateParam(TemplateParam&& b) noexcept = default;
  ~TemplateParam() noexcept = default;

  TemplateParam& operator=(TemplateParam const& rhs) noexcept = delete;
  TemplateParam& operator=(TemplateParam&& rhs) noexcept = default;

  std::ostream& print(PrintOptions const& opt,
                      std::ostream& out) const override final;
  std::unique_ptr<Node> deepClone() const override final;
  static std::unique_ptr<TemplateParam> parse(State& s);
  void assignSubstitution(std::vector<Node*> const& template_substitutions);

private:
  Node const* substitution;
  unsigned int index;
};
}
}
}

#endif /* !DEMANGLER_DETAILS_NODE_TEMPLATEPARAM_HH_ */
