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
std::array<OperatorName::OperatorTypeInfo, OperatorName::nb_operators> const
    OperatorName::operator_typemap = {{
// clang-format off
        {OperatorType::nw,  OperatorArity::Unknown, "new",      "nw"},
        {OperatorType::na,  OperatorArity::Unknown, "new[]",    "na"},
        {OperatorType::dl,  OperatorArity::Unknown, "delete",   "dl"},
        {OperatorType::da,  OperatorArity::Unknown, "delete[]", "da"},
        {OperatorType::ps,  OperatorArity::Unary,   "+",        "ps"},
        {OperatorType::ng,  OperatorArity::Unary,   "-",        "ng"},
        {OperatorType::ad,  OperatorArity::Unary,   "&",        "ad"},
        {OperatorType::de,  OperatorArity::Unary,   "*",        "de"},
        {OperatorType::co,  OperatorArity::Unary,   "~",        "co"},
        {OperatorType::pl,  OperatorArity::Binary,  "+",        "pl"},
        {OperatorType::mi,  OperatorArity::Binary,  "-",        "mi"},
        {OperatorType::ml,  OperatorArity::Binary,  "*",        "ml"},
        {OperatorType::dv,  OperatorArity::Binary,  "/",        "dv"},
        {OperatorType::rm,  OperatorArity::Binary,  "%",        "rm"},
        {OperatorType::an,  OperatorArity::Binary,  "&",        "an"},
        {OperatorType::or_, OperatorArity::Binary,  "|",        "or"},
        {OperatorType::eo,  OperatorArity::Binary,  "^",        "eo"},
        {OperatorType::aS,  OperatorArity::Binary,  "=",        "aS"},
        {OperatorType::pL,  OperatorArity::Binary,  "+=",       "pL"},
        {OperatorType::mI,  OperatorArity::Binary,  "-=",       "mI"},
        {OperatorType::mL,  OperatorArity::Binary,  "*=",       "mL"},
        {OperatorType::dV,  OperatorArity::Binary,  "/=",       "dV"},
        {OperatorType::rM,  OperatorArity::Binary,  "%=",       "rM"},
        {OperatorType::aM,  OperatorArity::Binary,  "&=",       "aM"},
        {OperatorType::oR,  OperatorArity::Binary,  "|=",       "oR"},
        {OperatorType::eO,  OperatorArity::Binary,  "^=",       "eO"},
        {OperatorType::ls,  OperatorArity::Binary,  "<<",       "ls"},
        {OperatorType::rs,  OperatorArity::Binary,  ">>",       "rs"},
        {OperatorType::lS,  OperatorArity::Binary,  "<<=",      "lS"},
        {OperatorType::rS,  OperatorArity::Binary,  ">>=",      "rS"},
        {OperatorType::eq,  OperatorArity::Binary,  "==",       "eq"},
        {OperatorType::ne,  OperatorArity::Binary,  "!-",       "ne"},
        {OperatorType::lt,  OperatorArity::Binary,  "<",        "lt"},
        {OperatorType::gt,  OperatorArity::Binary,  ">",        "gt"},
        {OperatorType::le,  OperatorArity::Binary,  "<=",       "le"},
        {OperatorType::ge,  OperatorArity::Binary,  ">=",       "ge"},
        {OperatorType::nt,  OperatorArity::Unary,   "!",        "nt"},
        {OperatorType::aa,  OperatorArity::Binary,  "&&",       "aa"},
        {OperatorType::oo,  OperatorArity::Binary,  "||",       "oo"},
        {OperatorType::pp,  OperatorArity::Unary,   "++",       "pp"},
        {OperatorType::mm,  OperatorArity::Unary,   "--",       "mm"},
        {OperatorType::cm,  OperatorArity::Binary,  ",",        "cm"},
        {OperatorType::pm,  OperatorArity::Binary,  "->*",      "pm"},
        {OperatorType::pt,  OperatorArity::Binary,  "->",       "pt"},
        {OperatorType::cl,  OperatorArity::Unary,   "()",       "cl"},
        {OperatorType::ix,  OperatorArity::Binary,  "[]",       "ix"},
        {OperatorType::qu,  OperatorArity::Unknown, "?",        "qu"},
        {OperatorType::cv,  OperatorArity::Unary,   "",         "cv"},
        {OperatorType::li,  OperatorArity::Unknown, "",         "li"},
        {OperatorType::v,   OperatorArity::Unknown, "",         "v"},
// clang-format on
    }};

OperatorName::OperatorName() noexcept
  : Node{Type::OperatorName}, typeinfo{nullptr}
{
}

OperatorName::OperatorName(clone_tag, OperatorName const& b)
  : Node{clone_tag{}, b}, typeinfo{b.typeinfo}
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
  {
    s.awaiting_template = true;
    ret->addNode(node::Type::parse(s, false));
    s.awaiting_template = false;
    s.template_substitutions_resolver = [node = ret.get()] (State const& s) {
      node->assignTemplateSubstitutions(s);
    };
  }
  return ret;
}

bool OperatorName::isCastOperator() const noexcept
{
  return this->typeinfo->operator_type == OperatorType::cv;
}

OperatorName::OperatorArity OperatorName::getArity() const noexcept
{
  return this->typeinfo->operator_arity;
}

bool OperatorName::isUnaryOp() const noexcept
{
  return this->typeinfo->operator_arity == OperatorArity::Unary;
}

bool OperatorName::isBinaryOp() const noexcept
{
  return this->typeinfo->operator_arity == OperatorArity::Binary;
}

bool OperatorName::isTernaryOp() const noexcept
{
  return this->typeinfo->operator_arity == OperatorArity::Ternary;
}

gsl::cstring_span<> OperatorName::getMangled() const noexcept
{
  return this->typeinfo->mangled_name;
}

gsl::cstring_span<> OperatorName::getOperatorSign() const noexcept
{
  return this->typeinfo->full_name;
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
