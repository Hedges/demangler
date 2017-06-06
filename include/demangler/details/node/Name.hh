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
  Name(Name const& b) = default;
  Name(Name&& b) noexcept = default;
  ~Name() noexcept = default;

  Name& operator=(Name const& rhs) = default;
  Name& operator=(Name&& rhs) noexcept = default;

  std::ostream& print(PrintOptions const& opt,
                      std::ostream& out) const override final;

  static std::unique_ptr<Name> parse(State &s);
};
}
}
}

#endif /* !DEMANGLER_DETAILS_NODE_NAME_HH_ */
