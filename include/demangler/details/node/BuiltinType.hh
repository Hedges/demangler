#ifndef DEMANGLER_DETAILS_NODE_BUILTINTYPE_HH_
#define DEMANGLER_DETAILS_NODE_BUILTINTYPE_HH_

#include <memory>

#include <gsl/string_span>

#include <demangler/details/Node.hh>

namespace demangler
{
namespace details
{
namespace node
{
class BuiltinType : public Node
{
public:
  BuiltinType(gsl::cstring_span<> pmangled,
              gsl::cstring_span<> pbuiltintype) noexcept;
  BuiltinType(clone_tag, BuiltinType const& b);
  BuiltinType(BuiltinType const& b) noexcept = delete;
  BuiltinType(BuiltinType&& b) noexcept = default;
  ~BuiltinType() noexcept override = default;

  BuiltinType& operator=(BuiltinType const& rhs) noexcept = delete;
  BuiltinType& operator=(BuiltinType&& rhs) noexcept = default;

  std::ostream& print(PrintOptions const& opt,
                      std::ostream& out) const override final;
  std::unique_ptr<Node> deepClone() const override final;
  gsl::cstring_span<> getTypeName() const noexcept;
  gsl::cstring_span<> getMangledName() const noexcept;
  bool isIntegral() const;

private:
  gsl::cstring_span<> mangled;
  gsl::cstring_span<> builtintype;
};
}
}
}

#endif /* !DEMANGLER_DETAILS_NODE_BUILTINTYPE_HH_ */
