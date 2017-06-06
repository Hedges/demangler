#include <demangler/details/node/Substitution.hh>

#include <cassert>
#include <cctype>
#include <stdexcept>
#include <unordered_map>

#include <demangler/details/Utils.hh>
#include <demangler/details/node/BuiltinSubstitution.hh>
#include <demangler/details/node/SeqId.hh>
#include <demangler/details/node/UserSubstitution.hh>

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
  assert(s.symbol.size() >= 2);
  assert(s.nextChar() == 'S');
  s.advance(1);
  auto ret = std::make_unique<Substitution>();

  if (std::isdigit(s.nextChar()) || std::isupper(s.nextChar()))
  {
    auto const oldsymbol = s.symbol;
    auto seqid = SeqId::parse(s);
    if (seqid >= s.user_substitutions.size())
      throw std::runtime_error("Invalid substitution index (got " +
                               std::to_string(s.user_substitutions.size()) +
                               " so far): " + s.toString());
    ret->addNode(
        std::make_unique<UserSubstitution>(s.user_substitutions[seqid]));
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
  if (s.nextChar() == '_')
  {
    if (s.user_substitutions.empty())
      throw std::runtime_error("Invalid substitution index (got " +
                               std::to_string(s.user_substitutions.size()) +
                               " so far): " + s.toString());
    ret->addNode(std::make_unique<UserSubstitution>(s.user_substitutions[0]));
    s.advance(1);
    return ret;
  }

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
