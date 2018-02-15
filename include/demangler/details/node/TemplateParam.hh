#ifndef DEMANGLER_DETAILS_NODE_TEMPLATEPARAM_HH_
#define DEMANGLER_DETAILS_NODE_TEMPLATEPARAM_HH_

#include <memory>
#include <vector>

#include <demangler/details/Node.hh>

namespace demangler
{
namespace details
{
namespace node
{
class TemplateParam : public Node
{
public:
  TemplateParam() noexcept;
  TemplateParam(clone_tag, TemplateParam const& b);
  TemplateParam(TemplateParam const& b) noexcept = delete;
  TemplateParam(TemplateParam&& b) noexcept = default;
  ~TemplateParam() noexcept;

  TemplateParam& operator=(TemplateParam const& rhs) noexcept = delete;
  TemplateParam& operator=(TemplateParam&& rhs) noexcept = default;

  std::ostream& print(PrintOptions const& opt,
                      std::ostream& out) const override final;
  std::unique_ptr<Node> deepClone() const override final;
  static std::unique_ptr<TemplateParam> parse(State& s);
  // Parses the template-param and returns a reference to the substituted node.
  // This method consumes characters from the state.
  static Node const& parseParamNode(State& s);
  void performSubstitution(State const& s);

private:
  static Node const* getSubstitutedNodePtr(State const& s, std::size_t idx);
  void assignSubstitution(Node const* subst);
  void removeClone(TemplateParam const* clone) const;
  void removeOriginal();

  Node const* substitution;
  // We need to keep a list of clones, for when the substitution has not yet
  // been performed. Once the substitution is performed, all cloned nodes have
  // to be updated as well.
  // original holds a pointer to the original node.
  mutable std::vector<TemplateParam*> clones;
  TemplateParam const* original;
  unsigned int index;
};
}
}
}

#endif /* !DEMANGLER_DETAILS_NODE_TEMPLATEPARAM_HH_ */
