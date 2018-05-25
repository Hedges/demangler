#ifndef DEMANGLER_DETAILS_NODE_PREFIX_HH_
#define DEMANGLER_DETAILS_NODE_PREFIX_HH_

#include <memory>

#include <gsl/string_span>

#include <demangler/details/Node.hh>

namespace demangler
{
namespace details
{
namespace node
{
class Prefix : public Node
{
public:
  Prefix() noexcept;
  Prefix(clone_tag, Prefix const& b);
  Prefix(Prefix const& b) noexcept = delete;
  Prefix(Prefix&& b) noexcept = default;
  ~Prefix() noexcept = default;

  Prefix& operator=(Prefix const& rhs) noexcept = delete;
  Prefix& operator=(Prefix&& rhs) noexcept = default;

  std::ostream& print(PrintOptions const& opt,
                      std::ostream& out) const override final;
  std::unique_ptr<Node> deepClone() const override final;

  static std::unique_ptr<Prefix> parse(State& s,
                                       string_type ctorname = {},
                                       bool parse_template = true);
  void parseTemplate(State& s);
  bool willHaveReturnType() const noexcept;

private:
  std::unique_ptr<Node const> substition_made;
};
}
}
}

#endif /* !DEMANGLER_DETAILS_NODE_PREFIX_HH_ */
