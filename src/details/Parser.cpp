#include <demangler/details/Parser.hh>

#include <stdexcept>

#include <demangler/details/node/MangledName.hh>

namespace demangler
{
namespace details
{
std::unique_ptr<Node> Parser::parse(std::string const& symbol)
{
  if (!node::MangledName::isMangled(symbol))
    return nullptr;
  State s{symbol, nullptr, {}, false, {}, {}, {}};
  auto ret = node::MangledName::parse(s);
  if (!s.empty())
    throw std::runtime_error{"Symbol not entirely consumed: " +
                             gsl::to_string(s.symbol)};
  return ret;
}
}
}
