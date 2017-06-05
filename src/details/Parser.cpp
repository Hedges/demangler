#include <demangler/details/Parser.hh>

#include <demangler/details/node/MangledName.hh>

namespace demangler
{
namespace details
{
std::unique_ptr<Node> Parser::parse(std::string const& symbol)
{
  State s{symbol, nullptr};
  return node::MangledName::parse(s);
}
}
}
