#ifndef DEMANGLER_DETAILTS_CVQUALIFIERS_HH_
#define DEMANGLER_DETAILTS_CVQUALIFIERS_HH_

#include <ostream>
#include <string>

#include <gsl/string_span>

#include <demangler/details/State.hh>

namespace demangler
{
namespace details
{
gsl::cstring_span<> extractCVRefQualifiers(State& s);
void printCVRefQualifiers(std::ostream& out,
                          gsl::cstring_span<> cvref_qualifiers);
std::string collapseCVRefQualifiers(gsl::cstring_span<> lcvref,
                                    gsl::cstring_span<> rcvref) noexcept;
}
}

#endif /* !DEMANGLER_DETAILTS_CVQUALIFIERS_HH_ */
