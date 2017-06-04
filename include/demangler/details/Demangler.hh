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
  void nodeSpecialName();
  void nodeEncoding();
  void nodeBareFunctionType();
  std::string nodeType();

  // Name methods.
  std::string nodeName();
  std::string nodeNestedName();
  std::string nodeUnscopedName();
  std::string nodeUnqualifiedName();
  gsl::cstring_span<> nodeSourceName();
  std::string nodeTemplateParam();
  std::string nodeTemplateArgs();
  std::string nodeTemplateArg();
  std::string nodePrefix();

  // Expression methods
  std::string nodeExprPrimary();

  // Components extraction methods.
  int nodeNumber();
  int nodeSeqId();

  // Substitution methods.
  bool nextIsSubstitution() const noexcept;
  std::string nodeSubstitution();
  std::string nodeSubstitutionSx();
  std::string nodeSubstitutionSeqId();

  // Utility (mostly parsing-related) methods.
  void advance(unsigned int n) noexcept;
  char nextChar() const noexcept;
  bool empty() const noexcept;
  std::string remainingStr() const;

  std::string const& symbol;
  gsl::cstring_span<> remaining;
  bool add_params;
  bool add_templates;
  std::string demangled;
  std::vector<std::string> substitutions;
};
}
}

#endif /* !DEMANGLER_DETAILS_DEMANGLER_HH_ */
