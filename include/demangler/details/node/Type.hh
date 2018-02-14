#ifndef DEMANGLER_DETAILS_NODE_TYPE_HH_
#define DEMANGLER_DETAILS_NODE_TYPE_HH_

#include <memory>
#include <string>

#include <gsl/string_span>

#include <demangler/details/Node.hh>

namespace demangler
{
namespace details
{
namespace node
{
class Type : public Node
{
public:
  Type() noexcept;
  Type(clone_tag, Type const& b);
  Type(Type const& b) noexcept = delete;
  Type(Type&& b) noexcept = default;
  ~Type() noexcept = default;

  Type& operator=(Type const& rhs) noexcept = delete;
  Type& operator=(Type&& rhs) noexcept = default;

  std::ostream& print(PrintOptions const& opt,
                      std::ostream& out) const override final;
  std::unique_ptr<Node> deepClone() const override final;

  // parse_template_args: Whether, if Type holds a Name, it should eat the
  // template-args if it encounters one (`I...E`).
  static std::unique_ptr<Type> parse(State& s, bool parse_template_args = true);

  bool isIntegral() const noexcept;

private:
  static std::unique_ptr<Type> parseD(State& s, std::unique_ptr<Type>&& ret);
  static std::unique_ptr<Type> parseDp(State& s, std::unique_ptr<Type>&& ret);

  std::string cv_qualifiers;
};
}
}
}

#endif /* !DEMANGLER_DETAILS_NODE_TYPE_HH_ */
