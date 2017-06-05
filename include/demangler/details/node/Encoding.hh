#ifndef DEMANGLER_DETAILS_NODE_ENCODING_HH_
#define DEMANGLER_DETAILS_NODE_ENCODING_HH_

#include <memory>

#include <gsl/string_span>

#include <demangler/details/Node.hh>

namespace demangler
{
namespace details
{
namespace node
{
class Encoding : public Node
{
public:
  Encoding() noexcept;
  Encoding(Encoding const& b) = default;
  Encoding(Encoding&& b) noexcept = default;
  ~Encoding() noexcept = default;

  Encoding& operator=(Encoding const& rhs) = default;
  Encoding& operator=(Encoding&& rhs) noexcept = default;

  std::ostream& print(PrintOptions const& opt,
                      std::ostream& out) const override final;

  static std::unique_ptr<Encoding> parse(State &s);
};
}
}
}

#endif /* !DEMANGLER_DETAILS_NODE_ENCODING_HH_ */
