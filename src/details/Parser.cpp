#include <demangler/details/Parser.hh>

#include <demangler/details/node/MangledName.hh>

namespace demangler
{
namespace details
{
std::unique_ptr<Node> Parser::parse(std::string const& symbol)
{
  if (!node::MangledName::isMangled(symbol))
    return nullptr;
  State s{symbol, nullptr, {}, false, {}, {}};
  return node::MangledName::parse(s);
}
}
}
