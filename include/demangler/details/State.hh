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
  void advance(unsigned int n = 1) noexcept;
  std::string toString() const;
  bool empty() const noexcept;
  char nextChar() const noexcept;
  bool hasNext(char c) const noexcept;
  bool hasNext(gsl::cstring_span<> needle) const noexcept;
  gsl::cstring_span<>::index_type charsRemaining() const noexcept;
  char peekChar(unsigned int idx) const noexcept;
  void registerUserSubstitution(Node const* node);
  Node const* getUserSubstitution(std::size_t index) const;
  void dumpSubstitutions() const;
  Node* addOrphanedSubstitution(std::unique_ptr<Node>&& orphan);

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
  std::vector<std::unique_ptr<Node>> orphaned_substitutions;
};
}
}

#endif /* !DEMANGLER_DETAILS_STATE_HH_ */
