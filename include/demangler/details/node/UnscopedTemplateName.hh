#ifndef DEMANGLER_DETAILS_NODE_UNSCOPEDTEMPLATENAME_HH_
#define DEMANGLER_DETAILS_NODE_UNSCOPEDTEMPLATENAME_HH_

#include <memory>

#include <gsl/string_span>

#include <demangler/details/Node.hh>

namespace demangler
{
namespace details
{
namespace node
{
class UnscopedTemplateName : public Node
{
public:
  UnscopedTemplateName() noexcept;
  UnscopedTemplateName(clone_tag, UnscopedTemplateName const& b);
  UnscopedTemplateName(UnscopedTemplateName const& b) noexcept = delete;
  UnscopedTemplateName(UnscopedTemplateName&& b) noexcept = default;
  ~UnscopedTemplateName() noexcept = default;

  UnscopedTemplateName& operator=(UnscopedTemplateName const& rhs) noexcept = delete;
  UnscopedTemplateName& operator=(UnscopedTemplateName&& rhs) noexcept =
      default;

  std::ostream& print(PrintOptions const& opt,
                      std::ostream& out) const override final;
  std::unique_ptr<Node> deepClone() const override final;

  static std::unique_ptr<UnscopedTemplateName> parse(State& s);
};
}
}
}

#endif /* !DEMANGLER_DETAILS_NODE_UNSCOPEDTEMPLATENAME_HH_ */
