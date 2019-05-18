#include <demangler/details/node/SeqId.hh>

#include <algorithm>
#include <cassert>
#include <cctype>
#include <stdexcept>
#include <string_view>

namespace demangler
{
namespace details
{
namespace node
{
unsigned int SeqId::parse(State& s)
{
  using namespace std::literals;
  constexpr auto digits =
      "0123456789ABCDEFGHIJKLMNOPQRSTUVVWXYZ"sv;
  constexpr auto base = static_cast<unsigned int>(digits.size());

  auto it = std::find(digits.begin(), digits.end(), s.nextChar());
  if (it == digits.end())
    return 0;
    
  auto ret = 0u;
  auto i = 0u;

  while (it != digits.end())
  {
    ret = ret * base +
          static_cast<unsigned int>(std::distance(digits.begin(), it));
    it = std::find(digits.begin(), digits.end(), s.symbol[++i]);
  }
  // Remember that 0 means 1, 1 means 2, and so on.
  ++ret;
  s.advance(i);
  return ret;
}
}
}
}
