#include <demangler/details/node/TemplateParam.hh>

#include <cassert>
#include <limits>

#include <demangler/details/node/SeqId.hh>

namespace demangler
{
namespace details
{
namespace node
{
TemplateParam::TemplateParam() noexcept
  : Node{Type::TemplateParam},
    substitution{nullptr},
    index{std::numeric_limits<unsigned int>::max()}
{
}

std::ostream& TemplateParam::print(PrintOptions const& opt,
                                   std::ostream& out) const
{
  assert(this->substitution);
  this->substitution->print(opt, out);
  return out;
}

std::unique_ptr<TemplateParam> TemplateParam::parse(State& s)
{
  assert(s.nextChar() == 'T');
  s.advance(1);
  auto const index = SeqId::parse(s);
  auto ret = std::make_unique<TemplateParam>();
  ret->index = index;
  if (!s.awaiting_template)
    ret->assignSubstitution(s.template_substitutions);
  return ret;
}

void TemplateParam::assignSubstitution(
    std::vector<Node*> const& template_substitutions)
{
  if (this->index >= template_substitutions.size())
    throw std::runtime_error("TemplateParam with index too high");
  this->substitution = template_substitutions[this->index];
}
}
}
}
