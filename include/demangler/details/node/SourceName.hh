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
  explicit SourceName(string_type pname) noexcept;
  SourceName(clone_tag, SourceName const& b);
  SourceName(SourceName const& b) noexcept = delete;
  SourceName(SourceName&& b) noexcept = default;
  ~SourceName() noexcept = default;

  SourceName& operator=(SourceName const& rhs) noexcept = delete;
  SourceName& operator=(SourceName&& rhs) noexcept = default;

  std::ostream& print(PrintOptions const& opt,
                      std::ostream& out) const override final;
  std::unique_ptr<Node> deepClone() const override final;

  static std::unique_ptr<SourceName> parse(State& s);
  string_type getName() const noexcept;

private:
  string_type name;
};
}
}
}

#endif /* !DEMANGLER_DETAILS_NODE_SOURCENAME_HH_ */
