#ifndef DEMANGLER_DETAILS_STATE_HH_
#define DEMANGLER_DETAILS_STATE_HH_

#include <functional>
#include <memory>
#include <vector>

#include <gsl/string_span>

namespace demangler
{
namespace details
{
class Node;
struct State
{
  void advance(unsigned int n = 1) noexcept
  {
    this->symbol = this->symbol.subspan(n);
  }
  std::string toString() const
  {
    return gsl::to_string(this->symbol);
  }
  bool empty() const noexcept
  {
    return this->symbol.empty();
  }
  char nextChar() const noexcept
  {
    return this->symbol[0];
  }
  gsl::cstring_span<>::index_type charsRemaining() const noexcept
  {
    return this->symbol.size();
  }
  char peekChar(unsigned int idx) const noexcept
  {
    return this->symbol[idx];
  }

  gsl::cstring_span<> symbol;
  Node* parent;
  std::vector<Node const*> user_substitutions;

  // If a cast operator is templated, the return type (which is the name of the
  // operator and thus included in the signature) might reference template
  // arguments. At the time of demangling, the template substitutions are not
  // yet known. This boolean should be set to true in such event where
  // substition will be resolved later.
  bool awaiting_template;
  std::function<void(State& s)> template_substitutions_resolver;
  std::vector<Node const*> template_substitutions;
};
}
}

#endif /* !DEMANGLER_DETAILS_STATE_HH_ */
