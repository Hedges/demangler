#ifndef DEMANGLER_DETAILS_NODE_OPERATORNAME_HH_
#define DEMANGLER_DETAILS_NODE_OPERATORNAME_HH_

#include <array>
#include <memory>

#include <demangler/details/Node.hh>

namespace demangler
{
namespace details
{
namespace node
{
class OperatorName : public Node
{
public:
  OperatorName() noexcept;
  OperatorName(clone_tag, OperatorName const& b);
  OperatorName(OperatorName const& b) noexcept = delete;
  OperatorName(OperatorName&& b) noexcept = default;
  ~OperatorName() noexcept = default;

  OperatorName& operator=(OperatorName const& rhs) noexcept = delete;
  OperatorName& operator=(OperatorName&& rhs) noexcept = default;

  std::ostream& print(PrintOptions const& opt,
                      std::ostream& out) const override final;
  std::unique_ptr<Node> deepClone() const override final;

  static std::unique_ptr<OperatorName> parse(State& s);

private:
  enum class OperatorType
  {
    nw,
    na,
    dl,
    da,
    ps,
    ng,
    ad,
    de,
    co,
    pl,
    mi,
    ml,
    dv,
    rm,
    an,
    or_,
    eo,
    aS,
    pL,
    mI,
    mL,
    dV,
    rM,
    aM,
    oR,
    eO,
    ls,
    rs,
    lS,
    rS,
    eq,
    ne,
    lt,
    gt,
    le,
    ge,
    nt,
    aa,
    oo,
    pp,
    mm,
    cm,
    pm,
    pt,
    cl,
    ix,
    qu,
    cv,
    li,
    v,
    Count,
    None
  };

  struct OperatorTypeInfo
  {
    constexpr OperatorTypeInfo(OperatorType type,
                               gsl::cstring_span<> full,
                               gsl::cstring_span<> mangled)
      : operator_type{type}, full_name{full}, mangled_name{mangled}
    {
    }

    OperatorType operator_type;
    gsl::cstring_span<> full_name;
    gsl::cstring_span<> mangled_name;
  };

  constexpr static auto const nb_operators =
      static_cast<size_t>(OperatorType::Count);
  static std::array<OperatorTypeInfo, nb_operators> const
      operator_typemap;
  static OperatorTypeInfo const* findOperatorTypeInfo(State& s);

  OperatorTypeInfo const* typeinfo;
};
}
}
}

#endif /* !DEMANGLER_DETAILS_NODE_OPERATORNAME_HH_ */
