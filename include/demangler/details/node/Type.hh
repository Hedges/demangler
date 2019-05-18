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
  ~Type() noexcept override = default;

  Type& operator=(Type const& rhs) noexcept = delete;
  Type& operator=(Type&& rhs) noexcept = default;

  std::ostream& print(PrintOptions const& opt,
                      std::ostream& out,
                      bool print_cvr) const;
  std::ostream& print(PrintOptions const& opt,
                      std::ostream& out) const override final;
  std::unique_ptr<Node> deepClone() const override final;

  // parse_template_args: Whether, if Type holds a Name, it should eat the
  // template-args if it encounters one (`I...E`).
  static std::unique_ptr<Type> parse(State& s, bool parse_template_args = true);

  bool isIntegral() const noexcept;
  gsl::cstring_span<> getCVRefQualifiers() const noexcept;

private:
  static std::unique_ptr<Type> parseD(State& s, std::unique_ptr<Type>&& ret);
  static std::unique_ptr<Type> parseDp(State& s, std::unique_ptr<Type>&& ret);
  static std::unique_ptr<Type> parseF(State& s, std::unique_ptr<Type>&& ret);

  void printDpNode(PrintOptions const& opt, std::ostream& out) const;

  gsl::cstring_span<> cvref_qualifiers;
  std::unique_ptr<Node> substitution_made;
  bool is_dp{false};
};
}
}
}

#endif /* !DEMANGLER_DETAILS_NODE_TYPE_HH_ */
