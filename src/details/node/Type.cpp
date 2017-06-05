#include <demangler/details/node/Type.hh>

#include <cassert>
#include <stdexcept>
#include <unordered_map>

#include <demangler/details/node/BuiltinType.hh>

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
}

Type::Type() noexcept : Node{Node::Type::Type}
{
}

std::ostream& Type::print(PrintOptions const& opt, std::ostream& out) const
{
  assert(this->getNodeCount() != 0);
  for (auto i = size_t{0}; i < this->getNodeCount(); ++i)
    this->getNode(i)->print(opt, out);
  return out;
}

std::unique_ptr<Type> Type::parse(State &s)
{
  auto it = builtin_types_map.find(s.nextChar());
  if (it != builtin_types_map.end())
  {
    auto ret = std::make_unique<Type>();
    ret->addNode(std::make_unique<BuiltinType>(it->second));
    s.advance(1);
    return ret;
  }
  throw std::runtime_error("Unknown type: " + s.toString());
}
}
}
}
