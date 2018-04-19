#ifndef DEMANGLER_DETAILTS_CVQUALIFIERS_HH_
#define DEMANGLER_DETAILTS_CVQUALIFIERS_HH_

#include <ostream>

#include <gsl/string_span>

#include <demangler/details/State.hh>

namespace demangler
{
namespace details
{
gsl::cstring_span<> parseCVQualifiers(State& s);
std::ostream& printCVQualifiers(std::ostream& out, gsl::cstring_span<> cvquals);
}
}

#endif /* !DEMANGLER_DETAILTS_CVQUALIFIERS_HH_ */
