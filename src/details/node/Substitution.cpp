#include <demangler/details/node/Substitution.hh>

#include <cassert>
#include <cctype>
#include <iostream>
#include <stdexcept>
#include <unordered_map>

#include <demangler/details/Utils.hh>
#include <demangler/details/node/BuiltinSubstitution.hh>
#include <demangler/details/node/SeqId.hh>
#include <demangler/details/node/UserSubstitution.hh>

#include <demangler/details/node/BuiltinType.hh>
#include <demangler/details/node/Name.hh>
#include <demangler/details/node/NestedName.hh>
#include <demangler/details/node/SourceName.hh>
#include <demangler/details/node/TemplateArg.hh>
#include <demangler/details/node/TemplateArgs.hh>
#include <demangler/details/node/Type.hh>
#include <demangler/details/node/UnqualifiedName.hh>

namespace demangler
{
namespace details
{
namespace node
{
namespace
{
auto const builtin_substitutions = std::unordered_map<char, std::string>{
    {'a', "std::allocator"},
    {'b', "std::basic_string"},
    {'s',
     "std::basic_string<char, std::char_traits<char>, std::allocator<char>>"},
    {'i', "std::basic_istream<char, std::char_traits<char>>"},
    {'o', "std::basic_ostream<char, std::char_traits<char>>"},
    {'d', "std::basic_iostream<char, std::char_traits<char>>"}};

std::unordered_map<char, std::unique_ptr<Node const>> getSubstitutions()
{
  auto ret = std::unordered_map<char, std::unique_ptr<Node const>>{};
  static std::string const strings[] = {"std",
                                        "allocator",
                                        "basic_string",
                                        "char",
                                        "char_traits",
                                        "basic_istream",
                                        "basic_ostream",
                                        "basic_iostream"};
  // Returns an AST of a std::`type`<char> class, where `type` is a parameter.
  // Used for char_traits and allocator.
  auto getCharTplType = [&](std::string const& type) {
    auto ret = std::make_unique<NestedName>();
    ret->addNode(std::make_unique<SourceName>(strings[0]));
    auto name = std::make_unique<Name>();
    name->addNode(std::make_unique<SourceName>(type));
    auto tpl = std::make_unique<TemplateArgs>();
    auto tplargs = std::make_unique<TemplateArg>();
    tplargs->addNode(std::make_unique<BuiltinType>("c", strings[3]));
    tpl->addNode(std::move(tplargs));
    name->addNode(std::move(tpl));
    ret->addNode(std::move(name));
    return ret;
  };

  { // std::allocator
    auto allocator = std::make_unique<NestedName>();
    allocator->addNode(std::make_unique<SourceName>(strings[0]));
    allocator->addNode(std::make_unique<SourceName>(strings[1]));
    ret.emplace('a', std::move(allocator));
  }
  { // std::basic_string
    auto basic_string = std::make_unique<NestedName>();
    basic_string->addNode(std::make_unique<SourceName>(strings[0]));
    basic_string->addNode(std::make_unique<SourceName>(strings[2]));
    ret.emplace('b', std::move(basic_string));
  }
  { // std::basic_string<char, std::char_traits<char>, std::allocator<char>>
    auto basic_string = std::make_unique<NestedName>();
    basic_string->addNode(std::make_unique<SourceName>(strings[0]));
    auto name = std::make_unique<Name>();
    name->addNode(std::make_unique<SourceName>(strings[2]));
    auto tpl = std::make_unique<TemplateArgs>();
    auto tplarg = std::make_unique<TemplateArg>();
    tplarg->addNode(std::make_unique<BuiltinType>("c", strings[3]));
    tpl->addNode(std::move(tplarg));
    tpl->addNode(getCharTplType(strings[4]));
    tpl->addNode(getCharTplType(strings[1]));
    name->addNode(std::move(tpl));
    basic_string->addNode(std::move(name));
    ret.emplace('s', std::move(basic_string));
  }
  { // std::basic_istream<char, std::char_traits<char>>
    auto basic_istream = std::make_unique<NestedName>();
    basic_istream->addNode(std::make_unique<SourceName>(strings[0]));
    auto name = std::make_unique<Name>();
    name->addNode(std::make_unique<SourceName>(strings[5]));
    auto tpl = std::make_unique<TemplateArgs>();
    auto tplarg = std::make_unique<TemplateArg>();
    tplarg->addNode(std::make_unique<BuiltinType>("c", strings[3]));
    tpl->addNode(std::move(tplarg));
    tpl->addNode(getCharTplType(strings[4]));
    name->addNode(std::move(tpl));
    basic_istream->addNode(std::move(name));
    ret.emplace('i', std::move(basic_istream));
  }
  { // std::basic_ostream<char, std::char_traits<char>>
    auto basic_ostream = std::make_unique<NestedName>();
    basic_ostream->addNode(std::make_unique<SourceName>(strings[0]));
    auto name = std::make_unique<Name>();
    name->addNode(std::make_unique<SourceName>(strings[6]));
    auto tpl = std::make_unique<TemplateArgs>();
    auto tplarg = std::make_unique<TemplateArg>();
    tplarg->addNode(std::make_unique<BuiltinType>("c", strings[3]));
    tpl->addNode(std::move(tplarg));
    tpl->addNode(getCharTplType(strings[4]));
    name->addNode(std::move(tpl));
    basic_ostream->addNode(std::move(name));
    ret.emplace('o', std::move(basic_ostream));
  }
  { // std::basic_iostream<char, std::char_traits<char>>
    auto basic_iostream = std::make_unique<NestedName>();
    basic_iostream->addNode(std::make_unique<SourceName>(strings[0]));
    auto name = std::make_unique<Name>();
    name->addNode(std::make_unique<SourceName>(strings[7]));
    auto tpl = std::make_unique<TemplateArgs>();
    auto tplarg = std::make_unique<TemplateArg>();
    tplarg->addNode(std::make_unique<BuiltinType>("c", strings[3]));
    tpl->addNode(std::move(tplarg));
    tpl->addNode(getCharTplType(strings[4]));
    name->addNode(std::move(tpl));
    basic_iostream->addNode(std::move(name));
    ret.emplace('d', std::move(basic_iostream));
  }
  return ret;
}

auto const builtin_substitutions2 = getSubstitutions();
}

Substitution::Substitution() noexcept : Node{Type::Substitution}, node{nullptr}
{
}

Substitution::Substitution(clone_tag, Substitution const& b)
  : Node{clone_tag{}, b}, node{b.node}
{
}

std::ostream& Substitution::print(PrintOptions const& opt,
                                  std::ostream& out) const
{
  assert(this->getNodeCount() == 1 || this->node);
  this->getSubstitutedNode()->print(opt, out);
  return out;
}

std::unique_ptr<Node> Substitution::deepClone() const
{
  return std::make_unique<Substitution>(clone_tag{}, *this);
}

Node const* Substitution::getSubstitutedNode() const noexcept
{
  if (this->node)
    return this->node;
  return this->getNode(0);
}

std::unique_ptr<Substitution> Substitution::parse(State& s)
{
  assert(s.symbol.size() >= 2);
  assert(s.nextChar() == 'S');
  s.advance(1);
  auto ret = std::make_unique<Substitution>();

  if (s.empty())
    throw std::runtime_error("Empty substitution");
  if (std::isdigit(s.nextChar()) || std::isupper(s.nextChar()))
  {
    auto const oldsymbol = s.symbol;
    auto seqid = SeqId::parse(s);
    ret->addNode(s.getUserSubstitution(seqid)->deepClone());
    if (s.nextChar() != '_')
    {
      // Restore old symbol state for displaying.
      s.symbol = oldsymbol;
      throw std::runtime_error("Substitution does not end with '_': " +
                               s.toString());
    }
    s.advance(1);
    return ret;
  }
  else if (s.nextChar() == '_')
  {
    ret->addNode(s.getUserSubstitution(0)->deepClone());
    s.advance(1);
    return ret;
  }

  auto const it = builtin_substitutions2.find(s.nextChar());
  if (it != builtin_substitutions2.end())
  {
    // ret->addNode(std::make_unique<BuiltinSubstitution>(it->second));
    ret->node = it->second.get();
    s.advance(1);
    return ret;
  }
  throw std::runtime_error("Unsupported substitution: " + s.toString());
}
}
}
}
