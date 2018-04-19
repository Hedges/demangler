#ifndef DEMANGLER_DETAILS_NODE_LOCALNAME_HH_
#define DEMANGLER_DETAILS_NODE_LOCALNAME_HH_

#include <memory>

#include <gsl/string_span>

#include <demangler/details/Node.hh>
#include <demangler/details/node/Type.hh>

namespace demangler
{
namespace details
{
namespace node
{
class LocalName : public Node
{
public:
  LocalName() noexcept;
  LocalName(clone_tag, LocalName const& b);
  LocalName(LocalName const& b) noexcept = delete;
  LocalName(LocalName&& b) noexcept = default;
  ~LocalName() noexcept = default;

  LocalName& operator=(LocalName const& rhs) noexcept = delete;
  LocalName& operator=(LocalName&& rhs) noexcept = default;

  std::ostream& print(PrintOptions const& opt,
                      std::ostream& out) const override final;
  std::unique_ptr<Node> deepClone() const override final;

  static std::unique_ptr<LocalName> parse(State& s);

private:
  bool has_discriminator;
  unsigned int discriminator;
};
}
}
}

#endif /* !DEMANGLER_DETAILS_NODE_LOCALNAME_HH_ */
