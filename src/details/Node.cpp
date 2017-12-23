#include <demangler/details/Node.hh>

namespace demangler
{
namespace details
{
Node::Node(Type t) noexcept : type{t}
{
}

void Node::addNode(std::unique_ptr<Node>&& n)
{
  this->children.emplace_back(std::move(n));
}

Node const* Node::getNode(std::size_t index) const noexcept
{
  return this->children[index].get();
}

size_t Node::getNodeCount() const noexcept
{
  return this->children.size();
}

Node::Type Node::getType() const noexcept
{
  return this->type;
}

Node::Node(clone_tag, Node const& b) : type{b.type}
{
  this->children.reserve(b.children.size());
  for (auto const& child : b.children)
    this->children.emplace_back(child->deepClone());
}
}
}
