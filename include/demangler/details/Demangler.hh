#ifndef DEMANGLER_DETAILS_DEMANGLER_HH_
#define DEMANGLER_DETAILS_DEMANGLER_HH_

#include <string>
#include <vector>

#include <gsl/string_span>

namespace demangler
{
namespace details
{
class Demangler
{
public:
  Demangler(std::string const& symbol, bool params, bool templates);
  Demangler(Demangler const& b) = default;
  Demangler(Demangler&& b) noexcept = default;
  ~Demangler() noexcept = default;

  Demangler& operator=(Demangler const& rhs) = default;
  Demangler& operator=(Demangler&& rhs) noexcept = default;

  std::string const& getDemangled() const noexcept;

private:
  // High level methods.
  void demangle();
  void demangleSpecialName();
  void demangleData();
  void demangleBareFunctionType();
  std::string demangleType();

  // Components extraction methods.
  gsl::cstring_span<> extractSourceName();
  int extractDecimal();
  int extractSeqid();

  // Substitution methods.
  bool nextIsSubstitution() const noexcept;
  std::string demangleSubstitution();
  std::string demanglePresetSubstitution();
  std::string demangleSeqSubstitution();

  // Utility (mostly parsing-related) methods.
  void skipChar() noexcept;
  void skipChars(unsigned int n) noexcept;
  char nextChar() const noexcept;
  bool empty() const noexcept;
  std::string remainingStr() const;

  std::string const& symbol;
  gsl::cstring_span<> remaining;
  bool add_params;
  bool add_templates;
  std::string demangled;
  std::vector<gsl::cstring_span<>> substitutions;
};
}
}

#endif /* !DEMANGLER_DETAILS_DEMANGLER_HH_ */
