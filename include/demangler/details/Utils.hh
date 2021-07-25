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

namespace span_literals
{
inline gsl::cstring_span<> operator""_cs(char const* s, size_t len)
{
  return {s, static_cast<gsl::cstring_span<>::index_type>(len)};
}
}
}
}

#endif /* !DEMANGLER_DETAILS_UTILS_HH_ */
