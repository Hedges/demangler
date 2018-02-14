#include <demangler/details/Node.hh>

#include <demangler/details/node/TemplateParam.hh>

namespace demangler
{
namespace details
{
namespace
{
constexpr char const* nodeTypeToString(Node::Type t)
{
  switch (t)
  {
#define MAKE_CASE(t)  \
  case Node::Type::t: \
    return #t;
    MAKE_CASE(BareFunctionType)
    MAKE_CASE(BuiltinSubstitution)
    MAKE_CASE(BuiltinType)
    MAKE_CASE(Constructor)
    MAKE_CASE(Encoding)
    MAKE_CASE(ExprPrimary)
    MAKE_CASE(MangledName)
    MAKE_CASE(Name)
    MAKE_CASE(NestedName)
    MAKE_CASE(Number)
    MAKE_CASE(OperatorName)
    MAKE_CASE(Prefix)
    MAKE_CASE(SourceName)
    MAKE_CASE(Substitution)
    MAKE_CASE(TemplateArg)
    MAKE_CASE(TemplateArgs)
    MAKE_CASE(TemplateParam)
    MAKE_CASE(Type)
    MAKE_CASE(UnqualifiedName)
    MAKE_CASE(UnscopedName)
    MAKE_CASE(UnscopedTemplateName)
    MAKE_CASE(UserSubstitution)
    MAKE_CASE(HolderNode)
#undef MAKE_CASE
  }
}
}
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

Node* Node::getNode(std::size_t index) noexcept
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

void Node::assignTemplateSubstitutions(State const& s)
{
  for (auto& child : children)
  {
    if (child->getType() != Type::TemplateParam) // Most frequent path
      child->assignTemplateSubstitutions(s);
    else
      static_cast<node::TemplateParam*>(child.get())->performSubstitution(s);
  }
}

bool Node::isEmpty() const noexcept
{
  return this->empty;
}

void Node::dumpAST(std::ostream& out, size_t indent) const
{
  std::fill_n(std::ostream_iterator<char>(out), indent, ' ');
  out << '{' << ' ' << nodeTypeToString(this->getType());
  if (this->getNodeCount() > 0)
  {
    out << '\n';
    for (auto i = size_t{0}; i < this->getNodeCount(); ++i)
    {
      this->getNode(i)->dumpAST(out, indent + 1);
      out << '\n';
    }
    std::fill_n(std::ostream_iterator<char>(out), indent, ' ');
    out << '}';
  }
  else
    out << ' ' << '}';
}

Node::Node(clone_tag, Node const& b) : type{b.type}
{
  this->children.reserve(b.children.size());
  for (auto const& child : b.children)
    this->addNode(child->deepClone());
}

void Node::setEmpty(bool pempty) noexcept
{
  this->empty = pempty;
}
}
}
