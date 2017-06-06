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
  UnscopedTemplateName(UnscopedTemplateName const& b) = default;
  UnscopedTemplateName(UnscopedTemplateName&& b) noexcept = default;
  ~UnscopedTemplateName() noexcept = default;

  UnscopedTemplateName& operator=(UnscopedTemplateName const& rhs) = default;
  UnscopedTemplateName& operator=(UnscopedTemplateName&& rhs) noexcept =
      default;

  std::ostream& print(PrintOptions const& opt,
                      std::ostream& out) const override final;

  static std::unique_ptr<UnscopedTemplateName> parse(State& s);
};
}
}
}

#endif /* !DEMANGLER_DETAILS_NODE_UNSCOPEDTEMPLATENAME_HH_ */
