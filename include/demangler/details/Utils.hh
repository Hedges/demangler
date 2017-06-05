#ifndef DEMANGLER_DETAILS_UTILS_HH_
#define DEMANGLER_DETAILS_UTILS_HH_

#include <ostream>

#include <gsl/string_span>

namespace demangler
{
namespace details
{
inline std::ostream& printStringSpan(std::ostream& out,
                                     gsl::cstring_span<> span)
{
  out.write(span.data(), span.size());
  return out;
}
}
}

#endif /* !DEMANGLER_DETAILS_UTILS_HH_ */
