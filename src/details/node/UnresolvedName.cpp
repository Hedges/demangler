#include <demangler/details/node/UnresolvedName.hh>

#include <stdexcept>

#include <demangler/details/node/Type.hh>
#include <demangler/details/node/SourceName.hh>

namespace demangler
{
namespace details
{
namespace node
{

UnresolvedName::UnresolvedName() noexcept : Node{Type::UnresolvedName}
{
}

UnresolvedName::UnresolvedName(clone_tag, UnresolvedName const& b)
  : Node{clone_tag{}, b}
{
}

std::ostream& UnresolvedName::print(PrintOptions const& opt,
                                    std::ostream& out) const
{
  if (this->is_global_scope)
    out << "::";
  for (auto i = std::size_t{0}; i < this->getNodeCount(); ++i)
    this->getNode(i)->print(opt, out);
  return out;
}

std::unique_ptr<Node> UnresolvedName::deepClone() const
{
  return std::make_unique<UnresolvedName>(clone_tag{}, *this);
}

std::unique_ptr<UnresolvedName> UnresolvedName::parse(State& s)
{
  auto ret = std::make_unique<UnresolvedName>();

  if (s.hasNext("gs"))
  {
    ret->is_global_scope = true;
    s.advance(2);
  }
  if (s.hasNext("sr"))
    throw std::runtime_error("Unsupported sr/srN");
  else
    // TODO(ethiraric): Implement the rest.
    // Normally a "simple-id" (source-name [template-args])
    ret->addNode(SourceName::parse(s));
  return ret;
}
}
}
}
