#include <demangler/details/node/Holder.hh>

namespace demangler
{
namespace details
{
namespace node
{
Holder::Holder(Node const& pheld) noexcept : Node{Type::HolderNode}, held{pheld}
{
}

Holder::Holder(clone_tag, Holder const& b) : Node{clone_tag{}, b}, held{b.held}
{
}

std::ostream& Holder::print(PrintOptions const& opt, std::ostream& out) const
{
  return this->held.get().print(opt, out);
}

std::unique_ptr<Node> Holder::deepClone() const
{
  return std::make_unique<Holder>(clone_tag{}, *this);
}

Node const& Holder::getHeld() const noexcept
{
  return this->held;
}
}
}
}
