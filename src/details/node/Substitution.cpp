#include <demangler/details/node/Substitution.hh>

#include <cassert>
#include <cctype>
#include <stdexcept>
#include <unordered_map>

#include <demangler/details/Utils.hh>
#include <demangler/details/node/BuiltinSubstitution.hh>

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
}

Substitution::Substitution() noexcept : Node{Type::Substitution}
{
}

std::ostream& Substitution::print(PrintOptions const& opt,
                                  std::ostream& out) const
{
  assert(this->getNodeCount() == 1);
  this->getNode(0)->print(opt, out);
  return out;
}

std::unique_ptr<Substitution> Substitution::parse(State& s)
{
  assert(s.nextChar() == 'S');
  s.advance(1);
  auto ret = std::make_unique<Substitution>();
  auto const it = builtin_substitutions.find(s.nextChar());
  if (it != builtin_substitutions.end())
  {
    ret->addNode(std::make_unique<BuiltinSubstitution>(it->second));
    s.advance(1);
    return ret;
  }
  throw std::runtime_error("Unsupported substitution: " + s.toString());
}
}
}
}
