#include <demangler/details/node/TemplateArg.hh>

#include <cassert>
#include <cctype>
#include <stdexcept>

#include <demangler/details/Utils.hh>
#include <demangler/details/node/Type.hh>

namespace demangler
{
namespace details
{
namespace node
{
TemplateArg::TemplateArg() noexcept : Node{Type::TemplateArg}
{
}

TemplateArg::TemplateArg(clone_tag, TemplateArg const& b) : Node{clone_tag{}, b}
{
}

std::ostream& TemplateArg::print(PrintOptions const& opt,
                                 std::ostream& out) const
{
  assert(this->getNodeCount() == 1);
  this->getNode(0)->print(opt, out);
  return out;
}

std::unique_ptr<Node> TemplateArg::deepClone() const
{
  return std::make_unique<TemplateArg>(clone_tag{}, *this);
}

std::unique_ptr<TemplateArg> TemplateArg::parse(State& s)
{
  assert(!s.empty());
  auto ret = std::make_unique<TemplateArg>();
  if (s.nextChar() == 'X')
    throw std::runtime_error("Expression unsupported in templates");
  else if (s.nextChar() == 'J')
    throw std::runtime_error("Argument pack unsupported in templates");
  else if (s.nextChar() == 'X')
    throw std::runtime_error("Expression unsupported in templates");
  else if (s.nextChar() == 'L')
    throw std::runtime_error("Expr primary unsupported in templates");
  else
    ret->addNode(node::Type::parse(s));
  return ret;
}
}
}
}
