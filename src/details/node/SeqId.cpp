#include <demangler/details/node/SeqId.hh>

#include <algorithm>
#include <cassert>
#include <cctype>
#include <stdexcept>

namespace demangler
{
namespace details
{
namespace node
{
unsigned int SeqId::parse(State& s)
{
  static auto const digits =
      gsl::cstring_span<>{"0123456789ABCDEFGHIJKLMNOPQRSTUVVWXYZ"};
  auto it = std::find(digits.begin(), digits.end(), s.nextChar());
  assert(it != digits.end());
  auto ret = 0u;
  auto i = 0;

  while (it != digits.end())
  {
    ret = ret * digits.size() + std::distance(digits.begin(), it);
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
