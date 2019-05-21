#include <demangler/details/CVQualifiers.hh>

#include <cassert>

namespace demangler
{
namespace details
{
namespace
{
auto const cvr_qualifiers_map = std::unordered_map<char, std::string>{
    {'K', " const"},
    {'O', "&&"},
    {'P', "*"},
    {'R', "&"},
    {'V', " volatile"},
    {'r', " restrict"},
};
}

gsl::cstring_span<> extractCVRefQualifiers(State& s)
{
  auto const* begin_cvrptr = s.symbol.data();

  while (!s.empty() &&
         cvr_qualifiers_map.find(s.nextChar()) != cvr_qualifiers_map.end())
    s.advance(1);
  return {begin_cvrptr, (s.symbol.data() - begin_cvrptr)};
}

void printCVRefQualifiers(std::ostream& out, gsl::cstring_span<> cvref_qualifiers)
{
  for (auto rit = cvref_qualifiers.rbegin(); rit != cvref_qualifiers.rend();
       ++rit)
  {
    auto const qual = *rit;
    auto const qualit = cvr_qualifiers_map.find(qual);
    if (qualit == cvr_qualifiers_map.end())
      throw std::logic_error(std::string("Invalid cv-qualifier: ") + qual);
    out << qualit->second;
  }
}

std::string collapseCVRefQualifiers(gsl::cstring_span<> lcvref,
                                    gsl::cstring_span<> rcvref) noexcept
{
  if (lcvref[0] == 'R' && rcvref[0] == 'L')
    return gsl::to_string(rcvref);
  return gsl::to_string(lcvref);
}
}
}
