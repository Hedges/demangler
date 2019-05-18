#include <demangler/details/node/Type.hh>

#include <algorithm>
#include <cassert>
#include <stdexcept>
#include <unordered_map>

#include <demangler/details/node/BareFunctionType.hh>
#include <demangler/details/node/BuiltinType.hh>
#include <demangler/details/node/Holder.hh>
#include <demangler/details/node/Name.hh>
#include <demangler/details/node/SourceName.hh>
#include <demangler/details/node/TemplateParam.hh>

namespace demangler
{
namespace details
{
namespace node
{
namespace
{
auto const builtin_types_map = std::unordered_map<char, std::string>{
    {'a', "signed char"}, {'b', "bool"},
    {'c', "char"},        {'d', "double"},
    {'e', "long double"}, {'f', "float"},
    {'g', "__float128"},  {'h', "unsigned char"},
    {'i', "int"},         {'j', "unsigned int"},
    {'l', "long"},        {'m', "unsigned long"},
    {'n', "__int128"},    {'o', "unsigned __int128"},
    {'s', "short"},       {'t', "unsigned short"},
    {'v', "void"},        {'w', "wchar_t"},
    {'x', "long long"},   {'y', "unsigned long long"},
    {'z', "..."},
};

auto const builtin_D_type = std::unordered_map<char, std::string>{
    {'a', "auto"},
    {'c', "decltype(auto)"},
    {'d', "decimal64"},
    {'e', "decimal128"},
    {'f', "decimal32"},
    {'h', "half"},
    {'i', "char32_t"},
    {'n', "decltype(nullptr)"},
    {'s', "char16_t"},
};

auto const cvr_qualifiers_map = std::unordered_map<char, std::string>{
    {'K', " const"},
    {'O', "&&"},
    {'P', "*"},
    {'R', "&"},
    {'V', " volatile"},
    {'r', " restrict"},
};

gsl::cstring_span<> extractCVRefQualifiers(State& s)
{
  auto const* begin_cvrptr = s.symbol.data();

  while (!s.empty() &&
         cvr_qualifiers_map.find(s.nextChar()) != cvr_qualifiers_map.end())
    s.advance(1);
  return {begin_cvrptr, (s.symbol.data() - begin_cvrptr)};
}

std::string collapseCVRefQualifiers(gsl::cstring_span<> lcvref,
                                            gsl::cstring_span<> rcvref) noexcept
{
  if (lcvref[0] == 'R' && rcvref[0] == 'L')
    return gsl::to_string(rcvref);
  return gsl::to_string(lcvref);
}

void printCVRefQualifiers(std::ostream& out, gsl::cstring_span<> cvref_qualifiers)
{
  for (auto rit = cvref_qualifiers.rbegin(); rit != cvref_qualifiers.rend();
       ++rit)
  {
    auto const qual = *rit;
    auto const qualit = cvr_qualifiers_map.find(qual);
    if (qualit == cvr_qualifiers_map.end())
      throw std::logic_error(std::string("Invalid cv-qualifier: ") + qual);
    out << qualit->second;
  }
}
}

Type::Type() noexcept : Node{Node::Type::Type}
{
}

Type::Type(clone_tag, Type const& b)
  : Node{clone_tag{}, b}, cvref_qualifiers{b.cvref_qualifiers}
{
}

std::ostream& Type::print(PrintOptions const& opt,
                          std::ostream& out,
                          bool print_cvr) const
{
  if (this->getNodeCount() == 0)
    out << "<empty parameter pack>";
  else if (this->getNodeCount() == 1)
  {
    if (!this->is_dp)
    {
      this->getNode(0)->print(opt, out);
      if (print_cvr)
        printCVRefQualifiers(out, this->cvref_qualifiers);
    }
    else
      this->printDpNode(opt, out);
  }
  else
  {
    this->getNode(0)->print(opt, out);
    out << ' ';
    if (!this->cvref_qualifiers.empty() && print_cvr)
    {
      out << '(';
      printCVRefQualifiers(out, this->cvref_qualifiers);
      out << ')';
    }
    this->getNode(1)->print(opt, out);
  }
  return out;
}

std::ostream& Type::print(PrintOptions const& opt, std::ostream& out) const
{
  return this->print(opt, out, true);
}

std::unique_ptr<Node> Type::deepClone() const
{
  return std::make_unique<Type>(clone_tag{}, *this);
}

std::unique_ptr<Type> Type::parse(State& s, bool parse_template_args)
{
  auto ret = std::make_unique<Type>();
  auto generates_substitution = true;

  ret->cvref_qualifiers = extractCVRefQualifiers(s);
  if (s.empty())
    throw std::runtime_error("Unfinished Type");
  auto it = builtin_types_map.find(s.nextChar());
  if (it != builtin_types_map.end())
  {
    ret->addNode(
        std::make_unique<BuiltinType>(s.symbol.subspan(0, 1), it->second));
    s.advance(1);
    generates_substitution = false;
  }
  else if (s.nextChar() == 'D')
  {
    s.advance(1);
    ret = parseD(s, std::move(ret));
  }
  else if (s.nextChar() == 'u')
  {
    s.advance(1);
    ret->addNode(SourceName::parse(s));
  }
  else if (s.nextChar() == 'S')
  {
    ret->addNode(Name::parse(s));
    // Substitutions do not generate substitutions.
    generates_substitution =
        ret->getNode(0)->getNode(0)->getType() != Node::Type::Substitution;
  }
  else if (s.nextChar() == 'T')
  {
    ret->addNode(TemplateParam::parse(s));
    generates_substitution = false;
  }
  else if (s.nextChar() == 'F')
    ret = parseF(s, std::move(ret));
  else
  {
    auto name = Name::parse(s, parse_template_args);
    ret->addNode(std::move(name));
  }
  if (generates_substitution)
  {
    if (!ret->cvref_qualifiers.empty())
    {
      auto tmp = ret->cvref_qualifiers;
      ret->cvref_qualifiers = {};
      ret->substitution_made = ret->deepClone();
      s.user_substitutions.emplace_back(ret->substitution_made.get());
      ret->cvref_qualifiers = tmp;
    }
    s.user_substitutions.emplace_back(ret.get());
  }
  return ret;
}

bool Type::isIntegral() const noexcept
{
  auto const contains = [](auto const& container, auto value) {
    return std::find(container.begin(), container.end(), value) !=
           container.end();
  };

  // Pointers and references are integrals.
  if (contains(this->cvref_qualifiers, 'P') ||
      contains(this->cvref_qualifiers, 'K') ||
      contains(this->cvref_qualifiers, 'R') ||
      contains(this->cvref_qualifiers, 'O'))
    return true;
  auto const& node = *this->getNode(0);
  if (node.getType() == Node::Type::BuiltinType)
    return static_cast<BuiltinType const&>(node).isIntegral();
  return false;
}

gsl::cstring_span<> Type::getCVRefQualifiers() const noexcept
{
  return this->cvref_qualifiers;
}

std::unique_ptr<Type> Type::parseD(State& s, std::unique_ptr<Type>&& ret)
{
  auto const it = builtin_D_type.find(s.nextChar());
  if (it != builtin_D_type.end())
  {
    ret->addNode(std::make_unique<BuiltinType>(
        gsl::cstring_span<>{s.symbol.data() - 1, 2}, it->second));
    s.advance(1);
    return std::move(ret);
  }

  switch (s.nextChar())
  {
  case 'T':
  case 't':
    throw std::runtime_error("Unsupported type DT/Dt (decltype expr)");
  case 'p':
    s.advance(1);
    if (s.empty())
      throw std::runtime_error("Empty parameter pack (Dp)");
    return parseDp(s, std::move(ret));
  case 'F':
    throw std::runtime_error("Unsupported type DF (unknown)");
  case 'v':
    throw std::runtime_error("Unsupported type Dv (unknown)");
  case 'x': // Transaction-safe function.
    s.advance(1);
    if (s.empty())
      throw std::runtime_error("Empty Dx");
    if (s.nextChar() != 'F')
      throw std::runtime_error("Transaction-safe type is not a function");
    return parseF(s, std::move(ret));
  default:
    throw std::runtime_error("Unknown D code in type: " + s.toString());
  }
}

std::unique_ptr<Type> Type::parseDp(State& s, std::unique_ptr<Type>&& ret)
{
  ret->cvref_qualifiers = extractCVRefQualifiers(s);
  if (s.nextChar() == 'T')
  {
    // This node is of type TemplateArg.
    auto const& packnode = TemplateParam::parseParamNode(s);
    if (packnode.getNodeCount() > 0)
      ret->addNode(std::make_unique<Holder>(packnode));
    else
      ret->setEmpty(true);
    s.registerUserSubstitution(ret.get());
  }
  else
    throw std::runtime_error("Invalid symbols in Dp: " +
                             std::string(s.symbol.data()));
  ret->is_dp = true;
  return std::move(ret);
}

std::unique_ptr<Type> Type::parseF(State& s, std::unique_ptr<Type>&& ret)
{
  s.advance(1);
  if (s.empty())
    throw std::runtime_error("Empty function");
  if (s.nextChar() == 'Y')
  {
    // This means extern "C", we don't care.
    s.advance(1);
    if (s.empty())
      throw std::runtime_error("Empty function");
  }

  auto bft = BareFunctionType::parse(s, true, true);
  ret->addNode(bft->retrieveReturnType());
  ret->addNode(std::move(bft));
  return std::move(ret);
}

void Type::printDpNode(PrintOptions const& opt, std::ostream& out) const
{
  auto const* holder = static_cast<Holder const*>(this->getNode(0));
  auto const& template_arg = holder->getHeld();
  for (auto i = std::size_t{0}; i < template_arg.getNodeCount(); ++i)
  {
    if (i > 0)
      out << ',' << ' ';
    auto const* targ_node = template_arg.getNode(i);
    if (targ_node->getType() != Node::Type::Type)
      targ_node->print(opt, out);
    else
    {
      auto const* typenode = static_cast<Type const*>(targ_node);
      typenode->print(opt, out, false);
      printCVRefQualifiers(out,
                           collapseCVRefQualifiers(this->cvref_qualifiers,
                                                   typenode->cvref_qualifiers));
    }
  }
}
}
}
}
