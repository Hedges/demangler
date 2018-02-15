#include <demangler/details/node/TemplateParam.hh>

#include <algorithm>
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
    original{nullptr},
    index{std::numeric_limits<unsigned int>::max()}
{
}

// Clones are intentionally not copied when copying a node.
TemplateParam::TemplateParam(clone_tag, TemplateParam const& b)
  : Node{clone_tag{}, b},
    substitution{b.substitution},
    clones{},
    original{&b},
    index{b.index}
{
}

TemplateParam::~TemplateParam() noexcept
{
  if (this->original)
    this->original->removeClone(this);
  for (auto* clone : clones)
    clone->removeOriginal();
}

std::ostream& TemplateParam::print(PrintOptions const& opt,
                                   std::ostream& out) const
{
  assert(this->substitution);
  this->substitution->print(opt, out);
  return out;
}

std::unique_ptr<Node> TemplateParam::deepClone() const
{
  auto clone = std::make_unique<TemplateParam>(clone_tag{}, *this);
  this->clones.emplace_back(clone.get());
  return clone;
}

std::unique_ptr<TemplateParam> TemplateParam::parse(State& s)
{
  assert(s.nextChar() == 'T');
  s.advance(1);
  auto const index = SeqId::parse(s);
  if (s.empty() || s.nextChar() != '_')
    throw std::runtime_error("TemplateParam does not end with '_'");
  s.advance(1);
  auto ret = std::make_unique<TemplateParam>();
  ret->index = index;
  if (!s.awaiting_template)
    ret->performSubstitution(s);
  s.user_substitutions.emplace_back(ret.get());
  return ret;
}

Node const& TemplateParam::parseParamNode(State& s)
{
  assert(s.nextChar() == 'T');
  s.advance(1);
  auto const index = SeqId::parse(s);
  if (s.empty() || s.nextChar() != '_')
    throw std::runtime_error("TemplateParam does not end with '_'");
  s.advance(1);
  auto& subst = *getSubstitutedNodePtr(s, index);
  s.user_substitutions.emplace_back(&subst);
  return subst;
}

void TemplateParam::performSubstitution(State const& s)
{
  this->assignSubstitution(this->getSubstitutedNodePtr(s, this->index));
}

Node const* TemplateParam::getSubstitutedNodePtr(State const& s,
                                                 std::size_t idx)
{
  if (idx >= s.template_substitutions.size())
    throw std::runtime_error("TemplateParam with index too high");
  return s.template_substitutions[idx];
}

void TemplateParam::assignSubstitution(Node const* subst)
{
  this->substitution = subst;
  for (auto* clone : clones)
    clone->assignSubstitution(subst);
}

void TemplateParam::removeClone(TemplateParam const* clone) const
{
  auto it = std::find(this->clones.begin(), this->clones.end(), clone);
  assert(it != this->clones.end());
  this->clones.erase(it);
}

void TemplateParam::removeOriginal()
{
  this->original = nullptr;
}
}
}
}
