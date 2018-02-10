#include <demangler/details/node/Type.hh>

#include <cassert>
#include <iostream>
#include <stdexcept>
#include <unordered_map>

#include <demangler/details/node/BuiltinType.hh>
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

auto const cv_qualifiers_map = std::unordered_map<char, std::string>{
    {'K', " const"},
    {'O', "&&"},
    {'P', "*"},
    {'R', "&"},
    {'V', " volatile"},
    {'r', " restrict"},
};
}

Type::Type() noexcept
  : Node{Node::Type::Type}
{
}

Type::Type(clone_tag, Type const& b)
  : Node{clone_tag{}, b}, cv_qualifiers{b.cv_qualifiers}
{
}

std::ostream& Type::print(PrintOptions const& opt, std::ostream& out) const
{
  assert(this->getNodeCount() != 0);
  for (auto i = size_t{0}; i < this->getNodeCount(); ++i)
    this->getNode(i)->print(opt, out);
  for (auto rit = this->cv_qualifiers.rbegin();
       rit != this->cv_qualifiers.rend();
       ++rit)
  {
    auto const qual = *rit;
    auto const qualit = cv_qualifiers_map.find(qual);
    if (qualit == cv_qualifiers_map.end())
      throw std::logic_error(std::string("Invalid cv-qualifier: ") + qual);
    out << qualit->second;
  }
  return out;
}

std::unique_ptr<Node> Type::deepClone() const
{
  return std::make_unique<Type>(clone_tag{}, *this);
}

std::unique_ptr<Type> Type::parse(State& s, bool parse_template_args)
{
  auto ret = std::make_unique<Type>();

  while (cv_qualifiers_map.find(s.nextChar()) != cv_qualifiers_map.end())
  {
    ret->cv_qualifiers += s.nextChar();
    s.advance(1);
  }
  auto it = builtin_types_map.find(s.nextChar());
  if (it != builtin_types_map.end())
  {
    ret->addNode(std::make_unique<BuiltinType>(it->second));
    s.advance(1);
    return ret;
  }
  else if (s.nextChar() == 'D')
  {
    s.advance(1);
    return parseD(s);
  }
  else if (s.nextChar() == 'u')
  {
    s.advance(1);
    ret->addNode(SourceName::parse(s));
    return ret;
  }
  else if (s.nextChar() == 'S')
  {
      ret->addNode(Name::parse(s));
      return ret;
  }
  else if (s.nextChar() == 'T')
  {
    ret->addNode(TemplateParam::parse(s));
    return ret;
  }
  else
  {
    auto name = Name::parse(s, parse_template_args);
    s.user_substitutions.emplace_back(name.get());
    ret->addNode(std::move(name));
    return ret;
  }
  throw std::runtime_error("Unknown type: " + s.toString());
}

std::unique_ptr<Type> Type::parseD(State& s)
{
  auto ret = std::make_unique<Type>();

  auto const it = builtin_D_type.find(s.nextChar());
  if (it != builtin_D_type.end())
  {
    s.advance(1);
    ret->addNode(std::make_unique<BuiltinType>(it->second));
    return ret;
  }

  switch (s.nextChar())
  {
  case 'T':
  case 't':
    throw std::runtime_error("Unsupported type DT/Dt (decltype expr)");
  case 'p':
    throw std::runtime_error("Unsupported type Dp (pack expansion)");
  case 'F':
    throw std::runtime_error("Unsupported type DF (unknown)");
  case 'v':
    throw std::runtime_error("Unsupported type Dv (unknown)");
  default:
    throw std::runtime_error("Unknown D code in type: " + s.toString());
  }
}
}
}
}
