#include <demangler/details/node/TemplateArg.hh>

#include <cassert>
#include <cctype>
#include <stdexcept>

#include <demangler/details/Utils.hh>
#include <demangler/details/node/ExprPrimary.hh>
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
  assert(this->getNodeCount() > 0);
  bool first = true;
  for (auto idx = size_t{0}; idx < this->getNodeCount(); ++idx)
  {
    auto const& node = *this->getNode(idx);
    if (node.getType() != Type::TemplateArg || node.getNodeCount() > 0)
    {
      if (first)
        first = false;
      else
        out << ',' << ' ';
      this->getNode(idx)->print(opt, out);
    }
  }
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
  {
    s.advance(1);
    while (!s.empty() && s.nextChar() != 'E')
      ret->addNode(TemplateArg::parse(s));
    if (s.empty())
      throw std::runtime_error("Unfinished parameter pack");
    if (ret->getNodeCount() == 0)
      ret->setEmpty(true);
    s.advance(1); // Consume 'E'
  }
  else if (s.nextChar() == 'L')
    ret->addNode(ExprPrimary::parse(s));
  else
    ret->addNode(node::Type::parse(s));
  return ret;
}
}
}
}
