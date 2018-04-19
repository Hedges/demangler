#include <demangler/details/CVQualifiers.hh>

#include <cassert>

namespace demangler
{
namespace details
{
namespace
{
constexpr bool isCVQualifier(char c)
{
  return c == 'K' || c == 'v' || c == 'r';
}
}
gsl::cstring_span<> parseCVQualifiers(State& s)
{
  assert(!s.empty());

  if (!isCVQualifier(s.nextChar()))
    return {};
  auto const* pointer = s.symbol.data();
  do
  {
    s.advance(1);
  } while (!s.empty() && isCVQualifier(s.nextChar()));
  return {pointer, s.symbol.data() - pointer};
}

std::ostream& printCVQualifiers(std::ostream& out, gsl::cstring_span<> cvquals)
{
  for (auto i = gsl::cstring_span<>::index_type{0}; i < cvquals.size(); ++i)
  {
    if (i)
      out << ' ';
    switch (cvquals[i])
    {
    case 'K':
      out << "const";
      break;
    case 'v':
      out << "volatile";
      break;
    case 'r':
      out << "restrict";
      break;
    default:
      throw std::runtime_error("Invalid cv-qualifier to print");
    };
  }
  return out;
}
}
}
