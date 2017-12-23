#include <demangler/details/node/OperatorName.hh>

#include <algorithm>
#include <cassert>
#include <gsl/string_span>

#include <demangler/details/node/Type.hh>

namespace demangler
{
namespace details
{
namespace node
{
// TODO(sabour_f): Should be changed for a more appropriate data structure.
std::array<OperatorName::OperatorTypeInfo,
           OperatorName::nb_operators> const OperatorName::operator_typemap = {{
    {OperatorType::nw, "new", "nw"},    {OperatorType::na, "new[]", "na"},
    {OperatorType::dl, "delete", "dl"}, {OperatorType::da, "delete[]", "da"},
    {OperatorType::ps, "+", "ps"},      {OperatorType::ng, "-", "ng"},
    {OperatorType::ad, "&", "ad"},      {OperatorType::de, "*", "de"},
    {OperatorType::co, "~", "co"},      {OperatorType::pl, "+", "pl"},
    {OperatorType::mi, "-", "mi"},      {OperatorType::ml, "*", "ml"},
    {OperatorType::dv, "/", "dv"},      {OperatorType::rm, "%", "rm"},
    {OperatorType::an, "&", "an"},      {OperatorType::or_, "|", "or"},
    {OperatorType::eo, "^", "eo"},      {OperatorType::aS, "=", "aS"},
    {OperatorType::pL, "+=", "pL"},     {OperatorType::mI, "-=", "mI"},
    {OperatorType::mL, "*=", "mL"},     {OperatorType::dV, "/=", "dV"},
    {OperatorType::rM, "%=", "rM"},     {OperatorType::aM, "&=", "aM"},
    {OperatorType::oR, "|=", "oR"},     {OperatorType::eO, "^=", "eO"},
    {OperatorType::ls, "<<", "ls"},     {OperatorType::rs, ">>", "rs"},
    {OperatorType::lS, "<<=", "lS"},    {OperatorType::rS, ">>=", "rS"},
    {OperatorType::eq, "==", "eq"},     {OperatorType::ne, "!-", "ne"},
    {OperatorType::lt, "<", "lt"},      {OperatorType::gt, ">", "gt"},
    {OperatorType::le, "<=", "le"},     {OperatorType::ge, ">=", "ge"},
    {OperatorType::nt, "!", "nt"},      {OperatorType::aa, "&&", "aa"},
    {OperatorType::oo, "||", "oo"},     {OperatorType::pp, "++", "pp"},
    {OperatorType::mm, "--", "mm"},     {OperatorType::cm, ",", "cm"},
    {OperatorType::pm, "->*", "pm"},    {OperatorType::pt, "->", "pt"},
    {OperatorType::cl, "()", "cl"},     {OperatorType::ix, "[]", "ix"},
    {OperatorType::qu, "?", "qu"},      {OperatorType::cv, "", "cv"},
    {OperatorType::li, "", "li"},       {OperatorType::v, "", "v"},
}};

OperatorName::OperatorName() noexcept
  : Node{Type::OperatorName}, typeinfo{nullptr}
{
}

OperatorName::OperatorName(clone_tag, OperatorName const& b)
  : Node{clone_tag{}, b}
{
}

std::ostream& OperatorName::print(PrintOptions const& opt,
                                  std::ostream& out) const
{
  assert(this->typeinfo);
  out << "operator " << this->typeinfo->full_name.data();
  for (auto i = 0u; i < this->getNodeCount(); ++i)
    this->getNode(i)->print(opt, out);
  return out;
}

std::unique_ptr<Node> OperatorName::deepClone() const
{
  return std::make_unique<OperatorName>(clone_tag{}, *this);
}

std::unique_ptr<OperatorName> OperatorName::parse(State& s)
{
  auto const* typeinfo = findOperatorTypeInfo(s);
  if (typeinfo == nullptr)
    return nullptr;
  auto ret = std::make_unique<OperatorName>();
  ret->typeinfo = typeinfo;
  if (typeinfo->operator_type == OperatorType::cv)
    ret->addNode(node::Type::parse(s));
  return ret;
}

OperatorName::OperatorTypeInfo const* OperatorName::findOperatorTypeInfo(
    State& s)
{
  if (s.nextChar() == 'v')
    throw std::runtime_error("Vendor extended operator not supported");
  if (s.charsRemaining() < 2)
    return nullptr;

  char const mangled_operator[] = {s.nextChar(), s.peekChar(1), '\0'};

  auto foundit = std::find_if(operator_typemap.begin(),
                              operator_typemap.end(),
                              [mangled_operator](OperatorTypeInfo const& info) {
                                return mangled_operator == info.mangled_name;
                              });
  if (foundit == operator_typemap.end())
    return nullptr;
  s.advance(2);
  return &*foundit;
}
}
}
}
