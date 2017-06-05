#ifndef DEMANGLER_DETAILS_STATE_HH_
#define DEMANGLER_DETAILS_STATE_HH_

#include <memory>

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

  gsl::cstring_span<> symbol;
  Node* parent;
};
}
}

#endif /* !DEMANGLER_DETAILS_STATE_HH_ */
