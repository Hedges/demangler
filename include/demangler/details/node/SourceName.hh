#ifndef DEMANGLER_DETAILS_NODE_SOURCENAME_HH_
#define DEMANGLER_DETAILS_NODE_SOURCENAME_HH_

#include <memory>

#include <gsl/string_span>

#include <demangler/details/Node.hh>

namespace demangler
{
namespace details
{
namespace node
{
class SourceName : public Node
{
public:
  SourceName() noexcept;
  SourceName(SourceName const& b) = default;
  SourceName(SourceName&& b) noexcept = default;
  ~SourceName() noexcept = default;

  SourceName& operator=(SourceName const& rhs) = default;
  SourceName& operator=(SourceName&& rhs) noexcept = default;

  std::ostream& print(PrintOptions const& opt,
                      std::ostream& out) const override final;

  static std::unique_ptr<SourceName> parse(State &s);

private:
  string_type name;
};
}
}
}

#endif /* !DEMANGLER_DETAILS_NODE_SOURCENAME_HH_ */
