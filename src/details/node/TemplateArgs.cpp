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
  bool first = true;
  for (auto i = 0u; i < this->getNodeCount(); ++i)
  {
    auto const& node = *this->getNode(i);
    if (!node.isEmpty())
    {
      if (first)
        first = false;
      else
        out << ',' << ' ';
      node.print(opt, out);
    }
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
    throw std::runtime_error("Unfinished template: " + s.toString());
  }
  s.template_substitutions.resize(ret->getNodeCount());
  for (auto i = size_t{0}; i < ret->getNodeCount(); ++i)
    s.template_substitutions[i] = ret->getNode(i);
  if (s.template_substitutions_resolver)
  {
    s.template_substitutions_resolver(s);
    s.template_substitutions_resolver = nullptr;
  }
  s.advance(1);
  return ret;
}
}
}
}
