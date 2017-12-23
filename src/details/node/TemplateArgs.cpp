#include <demangler/details/node/TemplateArgs.hh>

#include <cassert>
#include <cctype>
#include <stdexcept>

#include <demangler/details/Utils.hh>
#include <demangler/details/node/TemplateArg.hh>

namespace demangler
{
namespace details
{
namespace node
{
TemplateArgs::TemplateArgs() noexcept : Node{Type::TemplateArgs}
{
}

TemplateArgs::TemplateArgs(clone_tag, TemplateArgs const& b)
  : Node{clone_tag{}, b}
{
}

std::ostream& TemplateArgs::print(PrintOptions const& opt,
                                  std::ostream& out) const
{
  assert(this->getNodeCount() > 0);
  if (!opt.add_templates)
    return out;

  out << '<';
  for (auto i = 0u; i < this->getNodeCount(); ++i)
  {
    if (i)
      out << ", ";
    this->getNode(i)->print(opt, out);
  }
  out << '>';
  return out;
}

std::unique_ptr<Node> TemplateArgs::deepClone() const
{
  return std::make_unique<TemplateArgs>(clone_tag{}, *this);
}

std::unique_ptr<TemplateArgs> TemplateArgs::parse(State& s)
{
  assert(!s.empty());
  assert(s.nextChar() == 'I');
  auto oldsymbol = s.symbol;
  s.advance(1);
  auto ret = std::make_unique<TemplateArgs>();
  while (!s.empty() && s.nextChar() != 'E')
    ret->addNode(TemplateArg::parse(s));
  if (s.empty())
  {
    // Restore oldsymbol for displaying.
    s.symbol = oldsymbol;
    throw std::runtime_error("Unfinised template: " + s.toString());
  }
  s.advance(1);
  return ret;
}
}
}
}
